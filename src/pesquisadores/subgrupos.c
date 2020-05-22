#define MAX_GRP_CMD 3
#define ROW_0 0
#define ROW_1 1
#define ROW_2 2
#define ROW_3 3
#define ROW_4 4
#define ROW_5 5

void receber_subgrp_code(GtkWidget *button, GtkTreeView *treeview)
{
	GtkTreeSelection *selection;
	GtkTreeModel *model;
	GtkTreeIter iter;
	char *codigo;
	codigo = malloc(MAX_CODE_LEN);
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview));
	if(!gtk_tree_selection_get_selected(selection, &model, &iter))
		return ;
	gtk_tree_model_get (model, &iter, 0, &codigo, -1);
	gtk_entry_set_text(GTK_ENTRY(pesquisa_global_alvo),codigo);
	gtk_widget_activate(GTK_WIDGET(pesquisa_global_alvo));
	gtk_widget_destroy(psq_subgrp_wnd);
}

int entry_subgrp_pesquisa(GtkEntry *widget, GtkTreeView *treeview)
{
	enum {N_COLUMNS=3,COLUMN0=0,COLUMN1=1,COLUMN2=2};
	GtkTreeStore *treestore	=	GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    g_object_ref(G_OBJECT(treestore));
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview),NULL);
    gtk_tree_store_clear(treestore);
	gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview),0),TRUE);
	gtk_tree_view_column_set_visible(gtk_tree_view_get_column(GTK_TREE_VIEW(treeview),2),TRUE);
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview),GTK_TREE_MODEL(treestore));
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[MAX_QUERY_LEN];
	gchar *entrada = malloc(MAX_GRP_LEN);
	entrada = (gchar*)gtk_entry_get_text(widget);
	GtkTreeIter colunas, campos;
	GtkTreeStore *modelo = (GtkTreeStore*) gtk_tree_view_get_model(treeview);
	sprintf(query,"select b.code, b.nome, a.nome from grupos as a inner join grupos as b on a.code = b.pai where b.nome like '%c%s%c'",37,entrada,37);
	res = consultar(query);
	if(res == NULL)
	{
		return 1;
	}
	while((row = mysql_fetch_row(res))!=NULL)
	{
		if(atoi(row[0])!=1)
		{
			gtk_tree_store_append(modelo,&campos,NULL);
			g_print("Inserindo codigo: %s nome: %s\n",row[0],row[1]);
			gtk_tree_store_set(modelo,&campos, COLUMN0,row[0], COLUMN1,row[1], COLUMN2,row[2],-1);
		}
	}
	return 0;
}

int pesquisa_subgrp(GtkWidget *button, GtkEntry *cod_subgrp_entry)
{
	enum {N_COLUMNS=3,COLUMN0=0, COLUMN1=1, COLUMN2=2};
	GtkWidget *scrollwindow;
	GtkTreeViewColumn *coluna1, *coluna2, *coluna3;
	GtkCellRenderer *celula1, *celula2, *celula3;
	GtkWidget *treeview;
	GtkTreeStore *modelo;
	GtkTreeIter colunas, campos;
	GtkTreeIter filhos[6];
	GtkWidget *pesquisa_entry;
	GtkWidget *caixa_grande;
	
	GtkWidget *escolher_campo_button, *escolher_campo_img, *escolher_campo_fixed;
	
	MYSQL_RES *res[6];
	MYSQL_ROW row[6];
	char query[MAX_QUERY_LEN];
	
	caixa_grande = gtk_box_new(1,0);
	pesquisa_entry = gtk_entry_new();
	coluna1 = gtk_tree_view_column_new();
	celula1 = gtk_cell_renderer_text_new();
	coluna2 = gtk_tree_view_column_new();
	celula2 = gtk_cell_renderer_text_new();
	coluna3 = gtk_tree_view_column_new();
	celula3 = gtk_cell_renderer_text_new();
	
	treeview = gtk_tree_view_new();
	
	gtk_tree_view_set_level_indentation(GTK_TREE_VIEW(treeview),30);
	gtk_tree_view_set_enable_tree_lines(GTK_TREE_VIEW(treeview),TRUE);
	gtk_tree_view_set_enable_search(GTK_TREE_VIEW(treeview),TRUE);	
	gtk_tree_view_set_search_entry(GTK_TREE_VIEW(treeview),GTK_ENTRY(pesquisa_entry));
	scrollwindow = gtk_scrolled_window_new(NULL,NULL);
	
	psq_subgrp_wnd = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(psq_subgrp_wnd),3);
	gtk_window_set_keep_above(GTK_WINDOW(psq_subgrp_wnd),TRUE);
	gtk_widget_set_size_request(psq_subgrp_wnd,500,250);
	
	gtk_tree_view_column_pack_start(coluna1,celula1,TRUE);
	gtk_tree_view_column_set_title(coluna1,"Código");
	gtk_tree_view_column_add_attribute(coluna1,celula1,"text",0);
	gtk_tree_view_column_set_visible(coluna1,FALSE);
	gtk_tree_view_column_set_visible(coluna3,FALSE);
	
	gtk_tree_view_column_pack_start(coluna2,celula2,TRUE);
	gtk_tree_view_column_set_title(coluna2,"Nome");
	gtk_tree_view_column_add_attribute(coluna2,celula2,"text",1);

	gtk_tree_view_column_pack_start(coluna3,celula3,TRUE);
	gtk_tree_view_column_set_title(coluna3,"Grupo Pai");
	gtk_tree_view_column_add_attribute(coluna3,celula3,"text",2);

	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview),coluna1);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview),coluna2);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview),coluna3);
	
	gtk_tree_view_set_search_column(GTK_TREE_VIEW(treeview),1);
	modelo = gtk_tree_store_new(N_COLUMNS,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);

	sprintf(query,"select b.code, b.nome, a.nome from grupos as a inner join grupos as b on a.code = b.pai where b.pai = 1");
	res[ROW_0] = consultar(query);
	if(res[ROW_0] == NULL)
	{
		return 1;
	}
	//RAIZ
	while((row[ROW_0] = mysql_fetch_row(res[ROW_0]))!=NULL)
	{
		if(atoi(row[ROW_0][0])!=1)
		{
			gtk_tree_store_append(modelo,&campos,NULL);
			g_print("Inserindo codigo: %s nome: %s\n",row[ROW_0][0],row[ROW_0][1]);
			gtk_tree_store_set(modelo,&campos, COLUMN0,row[ROW_0][0], COLUMN1,row[ROW_0][1],COLUMN2,row[ROW_0][2],-1);
			sprintf(query,"select b.code, b.nome, a.nome from grupos as a inner join grupos as b on a.code = b.pai where b.pai = %s",row[ROW_0][0]);
			res[ROW_1] = consultar(query);
			if(res[ROW_1] == NULL)
			{
				return 1;
			}
			//camada 2
			while((row[ROW_1] = mysql_fetch_row(res[ROW_1]))!=NULL)
			{
				gtk_tree_store_append(modelo,&filhos[ROW_1],&campos);
				g_print("Inserindo codigo: %s nome: %s\n",row[ROW_1][0],row[ROW_1][1]);
				gtk_tree_store_set(modelo,&filhos[ROW_1], COLUMN0,row[ROW_1][0], COLUMN1,row[ROW_1][1],COLUMN2,row[ROW_1][2],-1);
				sprintf(query,"select b.code, b.nome, a.nome from grupos as a inner join grupos as b on a.code = b.pai where b.pai = %s",row[ROW_1][0]);
				res[ROW_2] = consultar(query);					
				if(res[ROW_2] == NULL)	
				{
					return 1;
				}
				//camada 3
				while((row[ROW_2] = mysql_fetch_row(res[ROW_2]))!=NULL)
				{
					gtk_tree_store_append(modelo,&filhos[ROW_2],&filhos[ROW_1]);
					g_print("Inserindo codigo: %s nome: %s\n",row[ROW_2][0],row[ROW_2][1]);
					gtk_tree_store_set(modelo,&filhos[ROW_2], COLUMN0,row[ROW_2][0], COLUMN1,row[ROW_2][1],COLUMN2,row[ROW_2][2],-1);
					sprintf(query,"select b.code, b.nome, a.nome from grupos as a inner join grupos as b on a.code = b.pai where b.pai = %s",row[ROW_2][0]);
					res[ROW_3] = consultar(query);
					if(res[ROW_3] == NULL)
					{
						return 1;
					}
					//camada 4
					while((row[ROW_3] = mysql_fetch_row(res[ROW_3]))!=NULL)
					{
						gtk_tree_store_append(modelo,&filhos[ROW_3],&filhos[ROW_2]);
						g_print("Inserindo codigo: %s nome: %s\n",row[ROW_3][0],row[ROW_3][1]);
						gtk_tree_store_set(modelo,&filhos[ROW_3], COLUMN0,row[ROW_3][0], COLUMN1,row[ROW_3][1],COLUMN2,row[ROW_3][2],-1);
						sprintf(query,"select b.code, b.nome, a.nome from grupos as a inner join grupos as b on a.code = b.pai where b.pai = %s",row[ROW_3][0]);
						res[ROW_4] = consultar(query);
						if(res[ROW_4] == NULL)
						{
							return 1;
						}
						//camada 5
						while((row[ROW_4] = mysql_fetch_row(res[ROW_4]))!=NULL)
						{
							gtk_tree_store_append(modelo,&filhos[ROW_4],&filhos[ROW_3]);
							g_print("Inserindo codigo: %s nome: %s\n",row[ROW_4][0],row[ROW_4][1]);
							gtk_tree_store_set(modelo,&filhos[ROW_4], COLUMN0,row[ROW_4][0], COLUMN1,row[ROW_4][1],COLUMN2,row[ROW_4][2],-1);
							sprintf(query,"select b.code, b.nome, a.nome from grupos as a inner join grupos as b on a.code = b.pai where b.pai = %s",row[ROW_4][0]);
							res[ROW_5] = consultar(query);
							if(res[ROW_5] == NULL)
							{
								return 1;
							}
							while((row[ROW_5] = mysql_fetch_row(res[ROW_5]))!=NULL)
							{
								gtk_tree_store_append(modelo,&filhos[ROW_5],&filhos[ROW_4]);
								g_print("Inserindo codigo: %s nome: %s\n",row[ROW_5][0],row[ROW_5][1]);
								gtk_tree_store_set(modelo,&filhos[ROW_5], COLUMN0,row[ROW_5][0], COLUMN1,row[ROW_5][1],COLUMN2,row[ROW_5][2],-1);
							}
						}
					}
				}
			}
		}
	}
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeview),GTK_TREE_MODEL(modelo));
	#ifdef __linux__
	gtk_container_add(GTK_CONTAINER(scrollwindow),treeview);
	#endif
	#ifdef WIN32
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollwindow),treeview);
	#endif
	
	escolher_campo_button = gtk_button_new_with_label("Escolher");
	escolher_campo_img = gtk_image_new_from_file(IMG_PROCR);
	escolher_campo_fixed = gtk_fixed_new();
	gtk_button_set_image(GTK_BUTTON(escolher_campo_button),escolher_campo_img);
	gtk_fixed_put(GTK_FIXED(escolher_campo_fixed),escolher_campo_button,20,10);
	
	gtk_widget_set_size_request(scrollwindow,450,200);
	gtk_box_pack_start(GTK_BOX(caixa_grande),pesquisa_entry,0,0,0);
	gtk_container_set_border_width(GTK_CONTAINER(psq_subgrp_wnd),10);
	gtk_box_pack_start(GTK_BOX(caixa_grande),scrollwindow,0,0,10);
	gtk_box_pack_start(GTK_BOX(caixa_grande),escolher_campo_fixed,0,0,10);
	gtk_container_add(GTK_CONTAINER(psq_subgrp_wnd),caixa_grande);
	g_signal_connect(pesquisa_entry,"activate",G_CALLBACK(entry_subgrp_pesquisa),treeview);
	pesquisa_global_alvo = GTK_ENTRY(cod_subgrp_entry);
	g_signal_connect(escolher_campo_button,"clicked",G_CALLBACK(receber_subgrp_code),treeview);
	gtk_widget_show_all(psq_subgrp_wnd);
	return 0;
}

