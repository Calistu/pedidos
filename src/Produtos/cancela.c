void cancelar_prod()
{
	char *code;
	code = malloc(10);
	code[0] = '\0';
	cancelando_prod=1;
	sprintf(code,"%i",tasker("produtos"));
	gtk_entry_set_text(GTK_ENTRY(codigo_prod_field),code);
	gtk_entry_set_text(GTK_ENTRY(nome_prod_field),"");
	gtk_entry_set_text(GTK_ENTRY(preco_prod_field),"");
	gtk_entry_set_text(GTK_ENTRY(peso_prod_field),"");
	gtk_entry_set_text(GTK_ENTRY(qnt_atacado_field),"");
	gtk_entry_set_text(GTK_ENTRY(campo_nome_unidade),"");
	gtk_entry_set_text(GTK_ENTRY(campo_nome_fornecedor),"");
	gtk_entry_set_text(GTK_ENTRY(campo_nome_grupo),"");
	gtk_entry_set_text(GTK_ENTRY(campo_nome_qnt_atacado),"");
	gtk_entry_set_text(GTK_ENTRY(unidade_prod_field),"");
	gtk_entry_set_text(GTK_ENTRY(fornecedor_prod_field),"");
	gtk_entry_set_text(GTK_ENTRY(grupo_prod_field),"");
	gtk_entry_set_text(GTK_ENTRY(preco_faturado_prod_field),"");
	notebook_preco_grupo();
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(observacao_prod_field));
	gtk_text_buffer_get_start_iter (buffer,&inicio);
	gtk_text_buffer_get_end_iter (buffer,&fim);
	gtk_text_buffer_delete (buffer,&inicio,&fim);	
	gtk_widget_set_sensitive(GTK_WIDGET(botao_mais),FALSE);
	grupos_prod=NULL;
	concluindo_prod=0;
	alterando_prod=0;
	cancelando_prod=0;
	gtk_label_set_text(GTK_LABEL(acao_atual2),"Cadastrando");
	gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook_cad_prod),0);	
}