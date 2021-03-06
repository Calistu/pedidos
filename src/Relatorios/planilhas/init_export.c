int planilha_init_export(int module){
	GtkWidget *box, *fixed, *frame;
	GtkWidget *planilha_export_button;

	planilha_export_conf_wnd = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_name(planilha_relats_wnd,"relat-fixos");
	gtk_widget_set_size_request(planilha_export_conf_wnd, 500, 300);

	gtk_window_set_position(GTK_WINDOW(planilha_export_conf_wnd),3);
	gtk_window_set_resizable(GTK_WINDOW(planilha_export_conf_wnd),FALSE);
	gtk_window_set_title(GTK_WINDOW(planilha_export_conf_wnd),"Gerando Planilha");
	gtk_window_set_icon_name(GTK_WINDOW(planilha_export_conf_wnd),"text-x-generic");
	gtk_window_set_transient_for(GTK_WINDOW(planilha_export_conf_wnd),GTK_WINDOW(janela_principal));

	janelas_gerenciadas.vetor_janelas[REG_PLANL_CONF_ICONS_WIN].reg_id = REG_PLANL_CONF_ICONS_WIN;
	janelas_gerenciadas.vetor_janelas[REG_PLANL_CONF_ICONS_WIN].aberta = 1;
	if(ger_janela_aberta(planilha_export_conf_wnd, &janelas_gerenciadas.vetor_janelas[REG_PLANL_CONF_ICONS_WIN]))
		return 1;
	janelas_gerenciadas.vetor_janelas[REG_PLANL_CONF_ICONS_WIN].janela_pointer = planilha_export_conf_wnd;

	GtkWidget *planilha_export_module_box = gtk_box_new(0,0); 
	GtkWidget *planilha_export_module_frame = gtk_frame_new("Módulo a gerar:");	
	planilha_export_module_combo = gtk_combo_box_text_new();

	gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(planilha_export_module_combo), PLANL_MOD_TERCEIROS, "Cadastros de Terceiros" );
	gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(planilha_export_module_combo), PLANL_MOD_PRODUTOS, "Cadastros de Produtos" );
	gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(planilha_export_module_combo), PLANL_MOD_MOVIMENTOS, "Movimentos de Estoque" );
	gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(planilha_export_module_combo), PLANL_MOD_FINAN_REC, "Contas à Receber" );
	gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(planilha_export_module_combo), PLANL_MOD_FINAN_PAG, "Contas à Pagar" );
	gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(planilha_export_module_combo), PLANL_MOD_FINAN_FLUXO, "Fluxo de Caixa" );
	
	gtk_combo_box_set_active(GTK_COMBO_BOX(planilha_export_module_combo), module);
	gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(planilha_export_module_combo), GTK_SENSITIVITY_OFF);

	planilha_export_open_check = gtk_check_button_new_with_label("Abrir relatório após geração?");
	
	GtkWidget *planilha_export_path_box = gtk_box_new(0,0); 
	GtkWidget *planilha_export_path_frame = gtk_frame_new("Caminho para exportação:");	
	planilha_export_path_entry = gtk_entry_new();
	planilha_export_path_button = gtk_file_chooser_button_new("Caminho para exportação", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
	gtk_box_pack_start(GTK_BOX(planilha_export_path_box), planilha_export_path_entry, 0,0,5);
	gtk_box_pack_start(GTK_BOX(planilha_export_path_box), planilha_export_path_button, 0,0,5);
	gtk_container_add(GTK_CONTAINER(planilha_export_path_frame), planilha_export_path_box);

	GtkWidget *planilha_export_box = gtk_box_new(0,0);
	planilha_export_button = gtk_button_new_with_label("Gerar Planilha");
	gtk_button_set_image(GTK_BUTTON(planilha_export_button), gtk_image_new_from_file(IMG_OK));
	gtk_box_pack_start(GTK_BOX(planilha_export_box), planilha_export_button, 0,0,5);

	box = gtk_box_new(1,0);
	frame = gtk_frame_new("Gerar backup");
	gtk_box_pack_start(GTK_BOX(box), planilha_export_module_combo, 0,0,5);
	gtk_box_pack_start(GTK_BOX(box), planilha_export_path_frame, 0,0,5);
	gtk_box_pack_start(GTK_BOX(box), planilha_export_open_check, 0,0,5);
	gtk_box_pack_start(GTK_BOX(box), planilha_export_box, 0,0,5);
	

	gtk_container_add(GTK_CONTAINER(frame), box);
	gtk_container_add(GTK_CONTAINER(planilha_export_conf_wnd), frame);

	FILE *pathcache = fopen(EXP_PTH_CACHE, "r");
	if(pathcache){
		char path[MAX_PATH_LEN];
		fgets(path,MAX_PATH_LEN,pathcache);
		gtk_entry_set_text(GTK_ENTRY(planilha_export_path_entry), path);
		fclose(pathcache);
	}

	planilhas_gerar_querys[PLANL_MOD_TERCEIROS] = malloc(MAX_QUERY_LEN);
	sprintf(planilhas_gerar_querys[PLANL_MOD_TERCEIROS], 
		"SELECT * FROM terceiros");
	
	planilhas_gerar_querys[PLANL_MOD_PRODUTOS] = malloc(MAX_QUERY_LEN);
	sprintf(planilhas_gerar_querys[PLANL_MOD_PRODUTOS], 
		"SELECT * FROM produtos");
	
	planilhas_gerar_querys[PLANL_MOD_MOVIMENTOS] = malloc(MAX_QUERY_LEN);
	sprintf(planilhas_gerar_querys[PLANL_MOD_MOVIMENTOS], 
		"SELECT me.code as 'Código Movimento',  me.pedido as 'Código Pedido', p.nome as 'Produto', t.razao as 'Cliente/Fornecedor', me.entradas - saidas as 'Entradas/Saidas', DATE_FORMAT(data_mov, '%%d/%%m/%%Y'),  CASE  WHEN tipo_mov = 0 THEN 'Venda' WHEN tipo_mov = 1 THEN 'Dev. Venda' WHEN tipo_mov = 2 THEN 'Compra' WHEN tipo_mov = 3 THEN 'Dev. Compra' WHEN tipo_mov = 4 THEN 'Outras' END as 'Operação', me.obs as 'Observação' FROM movimento_estoque me  INNER JOIN estoques e  on me.estoque = e.code INNER JOIN terceiros t  on me.cliente = t.code  INNER JOIN produtos p  on me.produto = p.code");
	
	planilhas_gerar_querys[PLANL_MOD_FINAN_REC] = malloc(MAX_QUERY_LEN);
	sprintf(planilhas_gerar_querys[PLANL_MOD_FINAN_REC], 
		"SELECT  t.code as 'Código Título',pt.posicao as 'Parcela',t2.razao as 'Cliente/Fornecedor',CASE WHEN status = 0 THEN 'Baixado'WHEN status = 1 THEN 'Parciamente Baixado'WHEN status = 2 THEN 'Pendente'END as 'Situação do Título',t.qnt_parcelas as 'Qnt. Parcelas',CASE WHEN t.tipo_titulo = 0 THEN 'Receber'WHEN t.tipo_titulo = 1 THEN 'Pagar'END  as 'Tipo Título',b.nome ,DATE_FORMAT(pt.data_criacao, '%%d/%%m/%%Y') as 'Criado em',DATE_FORMAT(pt.data_vencimento , '%%d/%%m/%%Y') as 'Vence em',  REPLACE(valor, '.', ',') as 'Valor' FROM titulos t INNER JOIN terceiros t2 ON t2.code = t.cliente INNER JOIN parcelas_tab pt ON t.code = pt.parcelas_id LEFT JOIN bancos b ON b.code = pt.banco WHERE t.tipo_titulo = 0");
	
	planilhas_gerar_querys[PLANL_MOD_FINAN_PAG] = malloc(MAX_QUERY_LEN);
	sprintf(planilhas_gerar_querys[PLANL_MOD_FINAN_PAG], 
		"SELECT  t.code as 'Código Título',pt.posicao as 'Parcela',t2.razao as 'Cliente/Fornecedor',CASE WHEN status = 0 THEN 'Baixado'WHEN status = 1 THEN 'Parciamente Baixado'WHEN status = 2 THEN 'Pendente'END as 'Situação do Título',t.qnt_parcelas as 'Qnt. Parcelas',CASE WHEN t.tipo_titulo = 0 THEN 'Receber'WHEN t.tipo_titulo = 1 THEN 'Pagar'END as 'Tipo Título' ,b.nome ,DATE_FORMAT(pt.data_criacao, '%%d/%%m/%%Y') as 'Criado em',DATE_FORMAT(pt.data_vencimento , '%%d/%%m/%%Y') as 'Vence em',  REPLACE(valor, '.', ',') as 'Valor' FROM titulos t INNER JOIN terceiros t2 ON t2.code = t.cliente INNER JOIN parcelas_tab pt ON t.code = pt.parcelas_id LEFT JOIN bancos b ON b.code = pt.banco WHERE t.tipo_titulo = 1");

	planilhas_gerar_querys[PLANL_MOD_FINAN_FLUXO] = malloc(MAX_QUERY_LEN);
	sprintf(planilhas_gerar_querys[PLANL_MOD_FINAN_FLUXO], 
		"SELECT   t.code as 'Código Título', pt.posicao as 'Parcela', t2.razao as 'Cliente/Fornecedor', CASE  WHEN status = 0 THEN 'Baixado' WHEN status = 1 THEN 'Parciamente Baixado' WHEN status = 2 THEN 'Pendente' END as 'Situação do Título', t.qnt_parcelas as 'Qnt. Parcelas', CASE  WHEN t.tipo_titulo = 0 THEN 'Receber' WHEN t.tipo_titulo = 1 THEN 'Pagar' END as 'Tipo Título' , b.nome , DATE_FORMAT(pt.data_criacao, '%%d/%%m/%%Y') as 'Criado em', DATE_FORMAT(pt.data_vencimento , '%%d/%%m/%%Y') as 'Vence em', CASE  WHEN t.tipo_titulo = 0 THEN REPLACE(valor, '.', ',') WHEN t.tipo_titulo = 1 THEN REPLACE(concat('-',valor), '.', ',') END as 'Valor' FROM titulos t  INNER JOIN terceiros t2  ON t2.code = t.cliente  INNER JOIN parcelas_tab pt  ON t.code = pt.parcelas_id  LEFT JOIN bancos b  ON b.code = pt.banco;");

	g_signal_connect(planilha_export_path_button,"file-set",G_CALLBACK(get_filename_to_entry),planilha_export_path_entry);
	g_signal_connect(planilha_export_conf_wnd,"destroy",G_CALLBACK(ger_janela_fechada),&janelas_gerenciadas.vetor_janelas[REG_PLANL_CONF_ICONS_WIN]);
	g_signal_connect(planilha_export_button,"clicked",G_CALLBACK(planilhas_gerar),NULL);

	gtk_widget_show_all(planilha_export_conf_wnd);
	return 0;
}