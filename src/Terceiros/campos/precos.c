int bloco_qnt=0;
int rec_precos();
int posicoes[MAX_PROD];
GtkWidget *vinc_janela_ter;
GtkWidget *confirmar_preco_buttom,*cancelar_preco_buttom,*campo_nome_prod;
GtkWidget *treeview_ter_prc;
#define MARGEM_VIN_D 5

struct campo_vinc
{
	GtkWidget *produto;
	GtkWidget *terceiro;
	GtkWidget *preco;
}vinculo;

int atualiza_ter_prc_treeview(GtkWidget *treeview)
{
	GtkTreeIter iter1, iter2;
	char formatar_preco[MAX_PRECO_LEN];
	char *query;
	enum {N_COLUMNS=2,COLUMN0=0,COLUMN1=1,COLUMN2=2,COLUMN3=0};
	GtkTreeStore *modelo = (GtkTreeStore *)gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
	
	MYSQL_ROW campos;
	MYSQL_RES *vetor;
	
	query = malloc(QUERY_LEN);
	
	if(alterando_ter==0)
	{
		if(code_terc()!=0)
			return 0;
	}
			
	sprintf(query,"select a.code,a.nome,valor from precos as b join produtos as a on a.code = b.produto where terceiro = %s",codigos_ter);

	vetor = consultar(query);
	gtk_tree_store_clear(modelo);
	if(vetor==NULL)
	{
		g_print("Query de precos voltou com vetor nulo\n");
		autologger("Query de precos voltou com vetor nulo\n");
		return 1;
	}
	
	while((campos = mysql_fetch_row(vetor))!=NULL)
	{	
		sprintf(formatar_preco,"R$ %.2f",atof(campos[2]));
		gtk_tree_store_append(modelo,&iter1,NULL);
		gtk_tree_store_set(modelo,&iter1,COLUMN0,campos[0],COLUMN1,formatar_preco,-1);
		gtk_tree_store_append(modelo,&iter2,&iter1);
		gtk_tree_store_set(modelo,&iter2,0,"Nome Produto: ",1,campos[1],-1);
		bloco_qnt++;
	}	
	return 0;
}

int rec_prod(GtkWidget *widget,GtkWidget *nome)
{
	char *query,*prod_nome;
	MYSQL_RES *vetor;
	MYSQL_ROW campos;
	query = malloc(QUERY_LEN);
	prod_nome = malloc(MAX_NAME_LEN);
	sprintf(query,"select nome from produtos where code = %s\n",gtk_entry_get_text(GTK_ENTRY(widget)));;
	vetor = consultar(query);
	if(vetor==NULL)
	{	
		g_print("Vetor voltou nulo");
		return 1;
	}	
	campos = mysql_fetch_row(vetor);
	if(campos==NULL)
	{	
		popup(NULL,"O código do produto não existe");
		return 1;
	}
	strcpy(prod_nome,campos[0]);
	gtk_entry_set_text(GTK_ENTRY(nome),prod_nome);
	gtk_widget_grab_focus(GTK_WIDGET(vinculo.preco));
	return 0;
}
int validar_preco(GtkWidget *widget,struct campo_vinc *vinculo)
{
	if(critica_real((char*)gtk_entry_get_text(GTK_ENTRY(vinculo->preco)),
	(GtkWidget*)vinculo->preco)!=0)
		return 1;
	
	g_print("valor passou: %s\n",gtk_entry_get_text(GTK_ENTRY(vinculo->preco)));
	gtk_widget_grab_focus(concluir_ter_buttom);
	return 0;		
}

int diminuir_treeview(GtkTreeView *treeview,GtkTreeIter *iter,GtkTreePath *path, int row)
{
	GtkWidget *precos_scroll_window = gtk_widget_get_parent(GTK_WIDGET(precos_scroll_caixa));
	gtk_widget_set_size_request(GTK_WIDGET(treeview),200,500);
	gtk_widget_set_size_request(precos_scroll_caixa,200,500);
	gtk_widget_set_size_request(precos_scroll_window,200,500);
	return 0;
}

int atualiza_preco(GtkWidget *widget,int *pos)
{
	char *query;
	int erro=0;
	int posicao_do_envio = *pos;
	gchar *valor;
	valor  = malloc(VLR_MAX_LEN);
	query  = malloc(QUERY_LEN);
	botao_mais = gtk_button_new_with_label(IMG_MAIS);
	g_print("iniciando funcao enviar_preco\n");

	g_print("%i blocos\n",bloco_qnt);
	g_print("enviando preços da box %i\n",posicao_do_envio);
	if(preco_entry[posicao_do_envio]==NULL)
	{
		g_print("Erro de memoria\n");	
		return 1;
	}
	valor = (gchar*)gtk_entry_get_text(GTK_ENTRY(preco_entry[posicao_do_envio]));
	if(critica_real((char*)valor,preco_entry[posicao_do_envio])!=0)
	{
			popup(NULL,"Valor incorreto para campo preço");
			return 0;
	}
	sprintf(query,"update precos set valor = %s where code = %i;",valor,codigo_preco[posicao_do_envio]);
	g_print("%s\n",query);
	erro = enviar_query(query);
	if(erro!=0)
	{
			popup(NULL,"Erro ao tentar atualizar o preço");
			return 1;
	}
	popup(NULL,"Preço atualizado com sucesso");
	return 0;
}

int rem_preco(GtkWidget *widget,int *codigo)
{
	char query[MAX_QUERY_LEN];
	int posicao = *codigo;
	sprintf(query,"delete from precos where code = %i;",codigo_preco[posicao]);
	if(enviar_query(query)==0)
	{
		atualiza_ter_prc_treeview(treeview_ter_prc);
	}
	else
	{
		g_print("Ocorreu erro na query para deletar preços\n");
		autologger("Ocorreu erro na query para deletar preços\n");
		popup(NULL,"Ocorreu algum erro ao tentar deletar\n\tConsulte com suporte");
	}
	g_print("codigo :%i\n",*codigo_preco);	
	return 0;
}

int verificar_entrada(GtkWidget *widget,int *pos)
{
	int posicao_do_envio = *pos;
	g_print("box %i\n",posicao_do_envio);
	gchar *valor = (gchar*) gtk_entry_get_text(GTK_ENTRY(preco_entry[posicao_do_envio]));
	if(critica_real(valor,widget)!=0)
	{
		vet_erro[VIN_ERR] = 1;
	}
	return 0;
}



void criar_vinc(GtkWidget *widget,struct campo_vinc *vinculo)
{
	char *query;
	int erro;
	query = malloc(QUERY_LEN);
	gchar *terc = (gchar*) gtk_entry_get_text(GTK_ENTRY(vinculo->terceiro));
	g_print("Valor terc: %s\n",terc);
	gchar *prod = (gchar*) gtk_entry_get_text(GTK_ENTRY(vinculo->produto));
	g_print("Valor prod: %s\n",prod);
	gchar *valor = (gchar*) gtk_entry_get_text(GTK_ENTRY(vinculo->preco));
	g_print("Valor valor: %s",valor);
	if(critica_real(valor,vinculo->preco)!=0)
	{
		vet_erro[PRC_ERR] = 1;
		return;
	}
	sprintf(query,"insert into precos(terceiro,produto,valor) values(%s,%s,%s)",codigos_ter,prod,valor);
	autologger(query);
	g_print("%s\n",query);
	erro = enviar_query	(query);
	if(erro!=0)
	{
		g_print("Erro ao tentar vincular terceiro ao produto\n");
		return;
	}
	popup(NULL,"Concluido");
	gtk_entry_set_text(GTK_ENTRY(campo_nome_prod),"Insira o código");
	gtk_entry_set_text(GTK_ENTRY(vinculo->produto),"");
	gtk_entry_set_text(GTK_ENTRY(vinculo->preco),"");

	atualiza_ter_prc_treeview(treeview_ter_prc);
}

int add_vinc_prod_cli(int codigo)
{
	
	GtkWidget *caixa;
	GtkWidget *psq_ter_prod_prc_button,*psq_ter_prod_prc_img,*psq_ter_prod_prc_box;
	GtkWidget *cli_label,*cli_entry,*cli_box,*fixed;
	GtkWidget *prod_label,*prod_entry,*prod_box;
	GtkWidget *preco_label,*preco_entry,*preco_box;
	GtkWidget *confirma_img,*cancela_img,*opcoes_box;

	char *query,*msg,*razao;
	MYSQL_ROW campos;
	query = malloc(QUERY_LEN);
	msg = malloc(30);
	razao = malloc(MAX_RAZ_LEN);
	vinc_janela_ter = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_keep_above(GTK_WINDOW(vinc_janela_ter),TRUE);
	gtk_window_set_position(GTK_WINDOW(vinc_janela_ter),3);
	gtk_window_set_title(GTK_WINDOW(vinc_janela_ter),"Vínculo");
	gtk_widget_set_size_request(vinc_janela_ter,200,320);
	
	fixed   = gtk_fixed_new();
	caixa     = gtk_box_new(1,0);
	
	cli_label = gtk_label_new("Cliente");
	cli_entry = gtk_entry_new();
	cli_box   = gtk_box_new(1,0);
	gtk_box_pack_start(GTK_BOX(cli_box),cli_label,0,0,0);
	gtk_box_pack_start(GTK_BOX(cli_box),cli_entry,0,0,10);
	gtk_fixed_put(GTK_FIXED(fixed),cli_box,MARGEM_VIN_D,5);
	
	prod_label = gtk_label_new("Produto");
	prod_entry = gtk_entry_new();
	campo_nome_prod = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(campo_nome_prod),"Insira o código");
	
	psq_ter_prod_prc_button = gtk_button_new();
	psq_ter_prod_prc_img = gtk_image_new_from_file(IMG_PESQ);
	gtk_button_set_image(GTK_BUTTON(psq_ter_prod_prc_button),psq_ter_prod_prc_img);
	psq_ter_prod_prc_box = gtk_box_new(0,0);
	gtk_box_pack_start(GTK_BOX(psq_ter_prod_prc_box),prod_entry,0,0,0);
	gtk_box_pack_start(GTK_BOX(psq_ter_prod_prc_box),psq_ter_prod_prc_button,0,0,0);
	
	prod_box   = gtk_box_new(1,0);
	gtk_box_pack_start(GTK_BOX(prod_box),prod_label,0,0,0);
	gtk_box_pack_start(GTK_BOX(prod_box),campo_nome_prod,0,0,0);	
	gtk_box_pack_start(GTK_BOX(prod_box),psq_ter_prod_prc_box,0,0,5);	
	gtk_fixed_put(GTK_FIXED(fixed),prod_box,MARGEM_VIN_D,80);
	
	preco_label = gtk_label_new("Preço");
	preco_entry = gtk_entry_new();
	preco_box   = gtk_box_new(1,0);
	gtk_box_pack_start(GTK_BOX(preco_box),preco_label,0,0,0);
	gtk_box_pack_start(GTK_BOX(preco_box),preco_entry,0,0,5);	
	gtk_fixed_put(GTK_FIXED(fixed),preco_box,MARGEM_VIN_D,180);
	
	opcoes_box   = gtk_box_new(0,0);
	confirma_img = gtk_image_new_from_file(IMG_OK);
	cancela_img = gtk_image_new_from_file(IMG_CANCEL);
	confirmar_preco_buttom = gtk_button_new_with_label("Confirmar");	
	cancelar_preco_buttom = gtk_button_new_with_label("Cancelar");
	gtk_button_set_image(GTK_BUTTON(confirmar_preco_buttom),confirma_img);
	gtk_button_set_image(GTK_BUTTON(cancelar_preco_buttom),cancela_img);
	gtk_box_pack_start(GTK_BOX(opcoes_box),confirmar_preco_buttom,0,0,0);
	gtk_box_pack_start(GTK_BOX(opcoes_box),cancelar_preco_buttom,0,0,5);	
	gtk_fixed_put(GTK_FIXED(fixed),opcoes_box,5,250);
	codigos_ter = (gchar *) gtk_entry_get_text(GTK_ENTRY(code_ter_field));
	sprintf(query,"select razao from terceiros where code = '%s'",codigos_ter);
	vetor = consultar(query);
	if(vetor==NULL)
	{
		g_print("erro no vetor mysql\n");	
		autologger("vetor mysql com problema");
		return 1;
	}
	campos = mysql_fetch_row(vetor);
	if(campos!=NULL)
	{
		strcpy(razao,campos[0]);
		gtk_entry_set_text(GTK_ENTRY(cli_entry),razao);
	}
	else
	{
		sprintf(msg,"O terceiro de código %s não existe!",codigos_ter);
		popup(NULL,msg);
		return 1;
	}
	vinculo.terceiro = cli_entry;
	vinculo.produto = prod_entry;
	vinculo.preco = preco_entry;
	
	g_signal_connect(prod_entry,"activate",G_CALLBACK(rec_prod),campo_nome_prod);
	g_signal_connect(preco_entry,"activate",G_CALLBACK(validar_preco),&vinculo);
	g_signal_connect(confirmar_preco_buttom,"clicked",G_CALLBACK(criar_vinc),&vinculo);
	g_signal_connect(GTK_BUTTON(psq_ter_prod_prc_button),"clicked",G_CALLBACK(psq_prod),prod_entry);
	
	gtk_box_pack_start(GTK_BOX(caixa),fixed,0,0,0);
	gtk_container_add(GTK_CONTAINER(vinc_janela_ter),caixa);
	gtk_widget_set_sensitive(GTK_WIDGET(cli_entry),FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(campo_nome_prod),FALSE);
	gtk_widget_show_all(vinc_janela_ter);
	return 0;
}
int rec_precos()
{

	char *query;
	char *entry_text;
	char *frow;
	char formatar_preco[MAX_PRECO_LEN];
	enum {N_COLUMNS=2,COLUMN0=0,COLUMN1=1,COLUMN2=2,COLUMN3=0};
	
	GtkTreeStore *modelo;
	GtkTreeViewColumn *coluna1, *coluna2, *coluna3;
	GtkCellRenderer *celula1, *celula2, *celula3;
	GtkTreeIter iter1, iter2;
	treeview_ter_prc = gtk_tree_view_new();
	modelo = gtk_tree_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING);
	
	coluna1 = gtk_tree_view_column_new();
	coluna2 = gtk_tree_view_column_new();
	
	celula1 = gtk_cell_renderer_accel_new();
	celula2 = gtk_cell_renderer_accel_new();
	
	gtk_tree_view_column_pack_start(coluna1,celula1,TRUE);
	gtk_tree_view_column_add_attribute(coluna1,celula1,"text",0);
	gtk_tree_view_column_pack_start(coluna2,celula2,TRUE);
	gtk_tree_view_column_add_attribute(coluna2,celula2,"text",1);

	gtk_tree_view_column_set_title(coluna1,"Código Produto");
	gtk_tree_view_column_set_title(coluna2,"Preço Produto");
	
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_ter_prc),coluna1);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview_ter_prc),coluna2);
	
	frow = malloc(ENTRADA);
	bloco_qnt=0;
	MYSQL_ROW campos;
	query = malloc(QUERY_LEN);
	if(code_terc()!=0)
		return 0;
	sprintf(query,"select b.code,a.nome,valor from precos as b join produtos as a on a.code = b.produto where terceiro = %s",codigos_ter);
	entry_text = malloc(CODE_LEN);
	vetor = consultar(query);
	if(vetor==NULL)
	{
		g_print("Query de precos voltou com vetor nulo\n");
		autologger("Query de precos voltou com vetor nulo\n");
		return 1;
	}
	
	while((campos = mysql_fetch_row(vetor))!=NULL)
	{	
		sprintf(formatar_preco,"R$ %.2f",atof(campos[2]));
		gtk_tree_store_append(modelo,&iter1,NULL);
		gtk_tree_store_set(modelo,&iter1,COLUMN0,campos[0],COLUMN1,formatar_preco,-1);
		gtk_tree_store_append(modelo,&iter2,&iter1);
		gtk_tree_store_set(modelo,&iter2,0,"Nome Produto: ",1,campos[1],-1);
		bloco_qnt++;
	}
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeview_ter_prc),GTK_TREE_MODEL(modelo));
	gtk_container_add(GTK_CONTAINER(precos_scroll_caixa),treeview_ter_prc);
	
	if(bloco_qnt==0)
	{
		g_print("Terceiro sem nenhum preco vinculado\n");
		autologger("Terceiro sem nenhum preco vinculado");
		return 0;
	}

	g_signal_connect(treeview_ter_prc,"row-collapsed",G_CALLBACK(diminuir_treeview),0);
	
	gtk_widget_show_all(precos_scroll_caixa);
	return 0;
}

