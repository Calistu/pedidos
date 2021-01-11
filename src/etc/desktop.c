static void criar_janela_princ(){

	janela_principal = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	gtk_window_maximize(GTK_WINDOW(janela_principal));
	gtk_window_set_position(GTK_WINDOW(janela_principal),0);
	gtk_window_set_resizable(GTK_WINDOW(janela_principal),TRUE);
	gtk_window_set_title(GTK_WINDOW(janela_principal),"Pedidos");
	gtk_container_set_border_width(GTK_CONTAINER(janela_principal),0);
	gtk_window_set_default_size(GTK_WINDOW(janela_principal),1166,568);
	gtk_window_set_icon_name(GTK_WINDOW(janela_principal),"accessories-dictionary");

	gtk_window_set_deletable (GTK_WINDOW(janela_principal), FALSE);

	janelas_gerenciadas.principal.reg_id = REG_PRINC_WIN;

	if(ger_janela_aberta(janela_principal, &janelas_gerenciadas.principal))
		return ;

	janelas_gerenciadas.principal.aberta = 1;
	janelas_gerenciadas.aplicacao.criada = 1;
	janelas_gerenciadas.principal.janela_pointer = janela_principal;

	gtk_widget_show_all(janela_principal);

	//keybinds made on code
	g_signal_connect(GTK_WIDGET(janela_principal),"key_press_event",G_CALLBACK(sobre_infos_keyfun),NULL);
	g_signal_connect(GTK_WIDGET(janela_principal),"key_press_event",G_CALLBACK(tecla_menu),NULL);
	g_signal_connect(GTK_WIDGET(janela_principal),"key_press_event",G_CALLBACK(atalho_fechar_sessao),NULL);

	//g_signal_connect(janela_principal,"destroy",G_CALLBACK(ger_janela_fechada),&janelas_gerenciadas.principal); //works only on gtk_window_set_deletable = TRUE
	//g_signal_connect(janela_principal,"destroy",G_CALLBACK(encerrar),janela_principal); //works only on gtk_window_set_deletable = TRUE
	return ;
}

int desktop(){
	int err=0;
	GtkWidget  *juncao;
	GtkWidget  *layout;
	GtkWidget *fixed_menu;

	GtkWidget *hostname_fixed, *hostname_label;
	GtkWidget *nome_usuario_label,*nome_usuario_fixed;
	GtkWidget *nivel_usuario_fixed ,*nivel_usuario_label;
	gchar *nome_usuario_gchar;

	MYSQL_RES *res;
	MYSQL_ROW row;
	char *query = malloc(MAX_QUERY_LEN);
	char markup[500];

	if(validar_sessao_criada())
    return 1;

	if(janelas_gerenciadas.fundo_inicializacao.aberta)
		gtk_widget_destroy(janelas_gerenciadas.fundo_inicializacao.janela_pointer);

	if(!janelas_gerenciadas.aplicacao.criada){
		sprintf(query,"select * from wnd_logger where operador = %i and id_janela = %i order by tempo desc limit 1",sessao_oper.code,REG_CORRECT_FINAL);
		if(!(res = consultar(query))){
			popup(NULL,"Falha ao verificar dados da sessão anterior");
			return 1;
		}

		if((row = mysql_fetch_row(res))){
			if(atoi(row[2])!=0){
				encerramento_brusco = 1;
			}

		}else{
				encerramento_brusco = 0;
		}

		sprintf(query,"insert into wnd_logger(id_janela,nome_janela,estado,qnt_aberta,operador,tempo) values(%i,'%s',%i,%i,%i,NOW())",
		REG_CORRECT_FINAL,
		"Encerrando...",
		1,
		0,
		sessao_oper.code);
		err = mysql_query(&conectar,query);
		if(err){
			popup(NULL,"Não foi possivel salvar status da sessão\n");
			file_logger(query);
			file_logger((char*)mysql_error(&conectar));
		}
	}

	app = gtk_application_new ("calisto.pedidos", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (criar_janela_princ), NULL);
	int status = g_application_run (G_APPLICATION (app), 0, NULL);
	if(status){
		popup(NULL,"Não foi possível carregar App");
		return 1;
	}
	g_object_unref (app);
	while (g_main_context_pending(NULL))
		g_main_context_iteration(NULL,FALSE);


	if(sessao_oper.status_sessao == SESSAO_LOGADA){
		sprintf(query,"select *, data_vencimento - now() from contratos where ativo = 1");
		if(!(res = consultar(query))){
			popup(NULL,"Erro ao buscar status do serviço");
			return 1;
		}
		if(!(row = mysql_fetch_row(res))){
			autologger("Não existem contratos ativos");
			ativar.ativo = 0;
		}else{
			if(atoi(row[CONTRATOS_COLS_QNT])<=0){
				popup(NULL,"Serviço Expirado");
				ativar.ativo = 0;
			}else
				ativar.ativo = 1;
		}
	}


	sessao_set_nonemodules();

	if( ativar.ativo && sessao_oper.status_sessao == SESSAO_LOGADA ){
		ativar.cadastro = atoi( row[CONTRATOS_CAD_COL] );
		ativar.compras = atoi( row[CONTRATOS_CMP_COL] );
		ativar.faturamento = atoi( row[CONTRATOS_FAT_COL] );
		ativar.estoque = atoi( row[CONTRATOS_EST_COL] );
		ativar.financeiro = atoi( row[CONTRATOS_FIN_COL] );
		ativar.marketing = atoi( row[CONTRATOS_MARKT_COL] );
		ativar.relatorios = atoi( row[CONTRATOS_REL_COL] );
	}

	if(sessao_oper.nivel>=NIVEL_TECNICO){
		sessao_set_allmodules();
	}

	fixed_menu = gtk_fixed_new();

	pegar_data();

	layout = gtk_layout_new(NULL,NULL);

	sprintf(query,"select * from perfil_desktop where code = %i",sessao_oper.code);
	if((res = consultar(query))==NULL){
		popup(NULL,"Erro ao receber dados para personalizacao do sistema");
		gtk_main_quit();
		return 1;
	}
	if(!(row = mysql_fetch_row(res))){
		popup(NULL,"Sem dados para personalizar o sistema");
		gtk_main_quit();
		return 1;
	}

	personalizacao.tema = atoi(row[2]);
	personalizacao.janela_init = atoi(row[3]);
	personalizacao.janela_keep_above = atoi(row[4]);

	GtkSettings *settings;
	settings = gtk_settings_get_default();

	if(personalizacao.tema>=0)
		if(nomes_temas[personalizacao.tema])
		g_object_set(settings, "gtk-theme-name",nomes_temas[personalizacao.tema],NULL);

	sprintf(query,"select desktop_img from perfil_desktop where code = %i",sessao_oper.code);
	res = consultar(query);
	if(!res){
		popup(NULL,"Não foi possível receber dados do perfil");
		encerrando();
		return 1;
	}

	nome_usuario_gchar = malloc(MAX_OPER_LEN+10);
	if((row = mysql_fetch_row(res))){
		imagem_desktop = gtk_image_new_from_file(DESKTOP);
		sprintf(nome_usuario_gchar,"Operador: %s",sessao_oper.nome);
		nome_usuario_label = gtk_label_new(nome_usuario_gchar);
		gtk_widget_set_name(nome_usuario_label,"nome_operador");
		trocar_desktop(NULL,NULL,atoi(row[1]));
	}
	else{
		popup(NULL,"Login sem dados de perfil");
		encerrando();
		return 1;
	}

	while (g_main_context_pending(NULL))
		g_main_context_iteration(NULL,FALSE);

	if(sessao_oper.nivel>=NIVEL_TECNICO)
	{
		GtkSettings *settings;
		imagem_desktop = gtk_image_new_from_file(OPER_DESKTOP);
		settings = gtk_settings_get_default();
		g_object_set(settings, "gtk-theme-name",NIVEL_TECNICO_THEME,NULL);
	}

	caixa_infos = gtk_box_new(1,0);

	superior = gtk_box_new(0,0);
	superior_1 = gtk_box_new(1,0);
	superior_2 = gtk_box_new(0,0);

	inferior = gtk_box_new(0,0);
	inferior_1 = gtk_box_new(1,0);
	inferior_2 = gtk_box_new(1,0);

	barra = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_widget_set_name(barra,"barra");

	juncao = gtk_box_new(1,0);
	area_de_trabalho = gtk_box_new(0,0);

	cad_emp_recebe();

	razao = gtk_label_new(cad_emp_strc.xNome);
	endereco = gtk_label_new(cad_emp_strc.xLgr);
	cnpj = gtk_label_new(cad_emp_strc.CNPJ);
	gtk_label_set_selectable(GTK_LABEL(cnpj),TRUE);

	gtk_widget_set_name(razao,"infos");
	gtk_widget_set_name(cnpj,"infos");
	gtk_widget_set_name(endereco,"infos");

	fixed_razao = gtk_fixed_new();
	fixed_endereco = gtk_fixed_new();
	fixed_cnpj = gtk_fixed_new();

	gtk_fixed_put(GTK_FIXED(fixed_razao),razao,50,300);
	gtk_fixed_put(GTK_FIXED(fixed_endereco),endereco,50,5);
	gtk_fixed_put(GTK_FIXED(fixed_cnpj),cnpj,50,5);

	sprintf(query, "select code, nome, nivel from niveis_gerenciais;");

	niveis_gerenciais = malloc(MAX_NIVEL_GER_NOME*MAX_NIVEL_GER_QNT);
	oper_perm_qnt_niveis = 0;

	if((res = consultar(query))==NULL)
		return 1;

	while((row = mysql_fetch_row(res))!=NULL){

		niveis_gerenciais[oper_perm_qnt_niveis] = malloc(MAX_NIVEL_GER_NOME);

		strcpy(niveis_gerenciais[oper_perm_qnt_niveis],row[1]);

		oper_perm_qnt_niveis++;

		if(oper_perm_qnt_niveis>=MAX_NIVEL_GER_QNT)
			break;
	}

	if(oper_perm_qnt_niveis<=0){
		popup(NULL,"Faltam dados para niveis gerenciais");
		return 1;
	}

	nivel_usuario_label = gtk_label_new("Nível Indefinido");

	nivel_usuario_fixed = gtk_fixed_new();

	if(sessao_oper.nivel<oper_perm_qnt_niveis)
		gtk_label_set_text(GTK_LABEL(nivel_usuario_label), niveis_gerenciais[sessao_oper.nivel]);

	if(sessao_oper.nivel >= NIVEL_TECNICO && sessao_oper.nivel < OPER_MAX_NIVEL )
		gtk_label_set_text(GTK_LABEL(nivel_usuario_label), "Nível Técnico");

	if(sessao_oper.nivel >= OPER_MAX_NIVEL)
		gtk_label_set_text(GTK_LABEL(nivel_usuario_label),"O Criador");

	gtk_widget_set_name(nivel_usuario_label,"nivel_operador");

	hostname_fixed = gtk_fixed_new();
	struct _maquina *maquina = maquinas_get_atual();
	char *endereco_maquina;
	if(maquina)
		endereco_maquina = malloc(strlen(server_confs.server_endereco) + strlen(maquina->hostname) + 2);
	else{
		return 1;
	}

	sprintf(endereco_maquina,"%s@%s", server_confs.server_endereco, maquina->hostname);

	hostname_label = gtk_label_new(endereco_maquina);
	gtk_label_set_selectable(GTK_LABEL(hostname_label),TRUE);
	while (g_main_context_pending(NULL))
		g_main_context_iteration(NULL,FALSE);

	gtk_widget_set_name(hostname_label,"hostname-label");

	nome_usuario_fixed = gtk_fixed_new();
	gtk_fixed_put(GTK_FIXED(nome_usuario_fixed),nome_usuario_label,100,200);
	gtk_fixed_put(GTK_FIXED(nivel_usuario_fixed),nivel_usuario_label,100,220);

	gtk_fixed_put(GTK_FIXED(hostname_fixed),hostname_label,100,240);

	gtk_box_pack_start(GTK_BOX(caixa_infos),fixed_razao,0,0,0);
	gtk_box_pack_start(GTK_BOX(caixa_infos),fixed_endereco,0,0,0);
	gtk_box_pack_start(GTK_BOX(caixa_infos),fixed_cnpj,0,0,0);

	gtk_box_pack_start(GTK_BOX(superior),superior_1,0,0,0);
	gtk_box_pack_end(GTK_BOX(superior),superior_2,0,0,0);

	gtk_box_pack_start(GTK_BOX(inferior),inferior_1,0,0,0);
	gtk_box_pack_end(GTK_BOX(inferior),inferior_2,0,0,0);

	gtk_widget_set_size_request(GTK_WIDGET(superior),1291,0);
	gtk_widget_set_size_request(GTK_WIDGET(inferior),1291,0);

	gtk_widget_set_size_request(GTK_WIDGET(area_de_trabalho),1290,750);
	gtk_widget_set_size_request(GTK_WIDGET(juncao),1290,750);
	gtk_box_pack_start(GTK_BOX(juncao),superior,0,0,0);
	gtk_box_pack_start(GTK_BOX(juncao),inferior,0,0,0);

	gtk_box_pack_start(GTK_BOX(area_de_trabalho),juncao,0,0,0);

	gtk_widget_set_size_request(barra,80,750);

	notificacoes_receber();

	GtkWidget *barra_icones = barra_icones_wnd();
	gtk_widget_set_size_request(barra_icones,80,750);
	gtk_container_add(GTK_CONTAINER(barra),barra_icones);

	GtkWidget *caixa_calendario = get_desktop_calendario();

	gtk_box_pack_end(GTK_BOX(area_de_trabalho),barra,0,0,0);

	gtk_widget_set_size_request(imagem_desktop,1290,750);
	gtk_layout_put(GTK_LAYOUT(layout), imagem_desktop, 0, 0);
	gtk_layout_put(GTK_LAYOUT(layout),nome_usuario_fixed,0,0);
	gtk_layout_put(GTK_LAYOUT(layout),nivel_usuario_fixed,0,0);
	gtk_layout_put(GTK_LAYOUT(layout),hostname_fixed,0,0);
	gtk_layout_put(GTK_LAYOUT(layout),caixa_infos,0,0);
	if(sessao_oper.nivel >= NIVEL_GERENCIAL)
		gtk_layout_put(GTK_LAYOUT(layout),caixa_calendario,100,400);
	gtk_layout_put(GTK_LAYOUT(layout),area_de_trabalho,0,0);

	gtk_container_add(GTK_CONTAINER(janela_principal),layout);

	while (g_main_context_pending(NULL))
		g_main_context_iteration(NULL,FALSE);

	gtk_box_pack_end(GTK_BOX(superior_2),fixed_menu,0,0,0);

	gtk_widget_show_all(janela_principal);

	configurar_parametros();

	iniciar_gerenciador_janela();

	if(encerramento_brusco){
		reportar_encerramento_brusco();
		encerramento_brusco = 0;
	}

	janelas_gerenciadas.aplicacao.criada = 1;
	return 0;
}
