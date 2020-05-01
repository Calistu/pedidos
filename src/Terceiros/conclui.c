int conclui_ter(GtkWidget* nome, gpointer *botao)
{	
	int err;
	char *code;
	code = malloc(10);
	char *query;
	if(code_terc()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),0);
		gtk_widget_grab_focus(code_ter_field);
		return 1;	
	}
	if(name_terc()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),0);
		gtk_widget_grab_focus(name_ter_field);
		return 1;
	}
	if(escolha_doc()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),0);
		gtk_widget_grab_focus(doc_combo);
		return 1;
	}
	if(inscr_terc()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),0);
		gtk_widget_grab_focus(inscr_ter_field);
		return 1;
	}
	if(escolha_tipo_ter()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),0);
		gtk_widget_grab_focus(type_ter_field);
		return 1;
	}
	if(cep_terc()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),0);
		gtk_widget_grab_focus(cep_ter_field);
		return 1;
	}
	if(address_terc()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),0);
		gtk_widget_grab_focus(address_ter_field);
		return 1;
	}
	if(cidade_terc()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),0);
		gtk_widget_grab_focus(cidade_ter_field);
		return 1;	
	}
	
	if( uf_terc()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),0);
		gtk_widget_grab_focus(uf_ter_field);
		return 1;
	}
	
	if(numrua()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),0);
		gtk_widget_grab_focus(address_num_field);
		return 1;
	}
	if(cel()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),1);
		gtk_widget_grab_focus(celular_ter_field);
		return 1;
	}
	if(tel()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),1);
		gtk_widget_grab_focus(telefone_ter_field);
		return 1;
	}
	
	if(contc()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),1);
		gtk_widget_grab_focus(contatoc_ter_field);
		return 1;
	}
	if(contt()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),1);
		gtk_widget_grab_focus(contatot_ter_field);
		return 1;
	}
	if(email_fun()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),1);
		gtk_widget_grab_focus(email_ter_field);
		return 1;
	}
	if(contato_email()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),1);
		gtk_widget_grab_focus(contatoe_ter_field);
		return 1;
	}
	if(transp_nomec()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),2);
		gtk_widget_grab_focus(transp_nome_entry);
		return 1;
	}
	if(transp_cnpjc()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),2);
		gtk_widget_grab_focus(transp_cnpj_entry);
		return 1;
	}
	
	if(transp_ie_fun()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),2);
		gtk_widget_grab_focus(transp_ie_entry);
		return 1;
	}
	
	if(transp_cepc()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),2);
		gtk_widget_grab_focus(transp_cep_entry);
		return 1;

	}
	if(transp_logr_fun()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),2);
		gtk_widget_grab_focus(transp_logradouro_entry);
		return 1;		
	}
	if(transp_num_fun()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),2);
		gtk_widget_grab_focus(transp_num_entry);
		return 1;		
	}
	
	if(transp_cidadec()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),2);
		gtk_widget_grab_focus(transp_cidade_entry);
		return 1;		
	}
	
	if(transp_estado_fun()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),2);
		gtk_widget_grab_focus(transp_estado_entry);
		return 1;		
	}
	
	if(prazo_fun()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),3);
		gtk_widget_grab_focus(prazo_ter_field);
		return 1;
	}
	if(obs()!=0)
	{
		gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),0);
		gtk_widget_grab_focus(observacoes_ter_field);
		return 1;
	}
	
	g_print("alocando memoria para query %i\n",(int)(QUERY_LEN+INSERT_QUERY));
	query = malloc((int)(QUERY_LEN+INSERT_QUERY));
	if(query==NULL)
	{
			popup(NULL,"Erro de memoria");
			autologger("Erro de memoria");
			return 1;
	}
	g_print("iniciando concluir_ter()\n");
	
	if(alterando_ter==0)
	{
		sprintf(query,CAD_QUERY_TER,CAD_QUERY_TER_VAR);	
	}
	else
	{
		sprintf(query,UPDATE_QUERY_TER,UPDATE_QUERY_TER_VAR);
	}
	g_print("[...] Criando conexão com o banco\n\n");
	MYSQL connect;
	mysql_init(&connect);
	g_print("[...] Tentando conexão com o banco\n\n");
	if(mysql_real_connect(&connect,"localhost",USER,PASS,"erp",0,NULL,0))
	{
		g_print("[!] conectado ao banco com sucesso\n\n");
	}
	else
	{
		popup(NULL,"Algum erro no servidor\nLigue para suporte");
		
		g_print("[!] Erro ao tentar se conectar %s\n",mysql_error(&connect));
		autologger("[!] Erro ao tentar se conectar\n");
		autologger((char*)mysql_error(&connect));
		return 1;
	}	
	if(stoi(codigos_ter)==-1)
	{
		if(strlen(codigos_ter)>8)
		{
			g_print("Codigo terceiro muito grande %s\n",codigos_ter);
			autologger("Codigo terceiro muito grande %s");
			popup(NULL,"O código do terceiro deve ser menor\n");
			return -1;
		}
		else
		{
			g_print("Codigo do terceiro incorreto %s\n",codigos_ter);
			popup(NULL,"Codigo terceiro deve ser numérico\nPor favor corrija");
			gtk_widget_grab_focus(GTK_WIDGET(code_ter_field));
			return 1;
		}
	}
	g_print("%s\n",query);
	autologger(query);

	err = mysql_query(&connect,query);
	if(err!=0)
	{
		g_print("Query para tabela terceiros\n");
		autologger("Query para tabela terceiros");
		g_print("codigo do erro %i\n",err);
		g_print("%s\n",mysql_error(&connect));
		g_print("%s\n\n",query);
		autologger(query);
		gtk_button_set_label(GTK_BUTTON(botao),"erro");
		popup(NULL,(gchar*)mysql_error(&connect));
		return 1;
	}

	g_print("Query para tabela terceiros\n");
	g_print("Query envida com sucesso\n");
	gtk_button_set_label(GTK_BUTTON(botao),"concluido");
	popup(NULL,"Concluido");
	gtk_label_set_text(GTK_LABEL(acao_atual2),"Cadastrando");
	alterando_ter=0;
	code[0] = '\0';
	sprintf(code,"%i",tasker("terceiros"));
	
	//terceiros
	gtk_entry_set_text(GTK_ENTRY(code_ter_field),code);
	gtk_entry_set_text(GTK_ENTRY(name_ter_field),"");
	gtk_entry_set_text(GTK_ENTRY(inscr_ter_field),"");
	gtk_entry_set_text(GTK_ENTRY(cep_ter_field),"");
	gtk_entry_set_text(GTK_ENTRY(address_ter_field),"");
	gtk_combo_box_set_active(GTK_COMBO_BOX(type_ter_field),0);
	gtk_entry_set_text(GTK_ENTRY(cidade_ter_field),"");
	gtk_entry_set_text(GTK_ENTRY(prazo_ter_field),"");
	gtk_entry_set_text(GTK_ENTRY(prazo_ter_field),"");
	gtk_entry_set_text(GTK_ENTRY(address_num_field),"");
	gtk_entry_set_text(GTK_ENTRY(uf_ter_field),"");
	gtk_entry_set_text(GTK_ENTRY(doc_ter_field),"");
	gtk_entry_set_text(GTK_ENTRY(celular_ter_field),"");
	gtk_entry_set_text(GTK_ENTRY(contatoc_ter_field),"");
	gtk_entry_set_text(GTK_ENTRY(telefone_ter_field),"");
	gtk_entry_set_text(GTK_ENTRY(contatot_ter_field),"");
	gtk_entry_set_text(GTK_ENTRY(email_ter_field),"");
	gtk_entry_set_text(GTK_ENTRY(contatoe_ter_field),"");
	
	//entrega
	gtk_entry_set_text(GTK_ENTRY(transp_codigo_entry),"");
	gtk_entry_set_text(GTK_ENTRY(transp_nome_entry),"");
	gtk_entry_set_text(GTK_ENTRY(transp_cnpj_entry),"");
	gtk_entry_set_text(GTK_ENTRY(transp_ie_entry),"");
	gtk_entry_set_text(GTK_ENTRY(transp_num_entry),"");
	gtk_entry_set_text(GTK_ENTRY(transp_logradouro_entry),"");
	gtk_entry_set_text(GTK_ENTRY(transp_cidade_entry),"");
	gtk_entry_set_text(GTK_ENTRY(transp_estado_entry),"");
	gtk_entry_set_text(GTK_ENTRY(transp_cep_entry),"");
	gtk_entry_set_text(GTK_ENTRY(transp_telefone_entry),"");
	
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(observacoes_ter_field));
	gtk_text_buffer_get_start_iter (buffer,&inicio);
	gtk_text_buffer_get_end_iter (buffer,&fim);
	gtk_text_buffer_delete (buffer,&inicio,&fim);	
	
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(transp_obs_entry));
	gtk_text_buffer_get_start_iter (buffer,&inicio);
	gtk_text_buffer_get_end_iter (buffer,&fim);
	gtk_text_buffer_delete (buffer,&inicio,&fim);
	
	gtk_widget_set_sensitive(GTK_WIDGET(botao_mais),FALSE);		
	gtk_notebook_set_current_page(GTK_NOTEBOOK(ter_notebook),0);
	printf("finalizando conclui_ter()\n");
	gtk_widget_set_sensitive(GTK_WIDGET(code_ter_field),TRUE);
//	gtk_widget_grab_focus (GTK_WIDGET(name_ter_field));
	return 0;
}
