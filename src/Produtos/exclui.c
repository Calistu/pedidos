int exclui_prod()
{
	g_print("Iniciando deleta_prod()\n");
	char stringer[10];
	char query[100];
	int erro;
	gchar *cod_delel;
	MYSQL_RES *estado;
	MYSQL_ROW campo;
	GtkTextBuffer *buffer;
	GtkTextIter inicio,fim;
	alterando_prod=0;
	concluindo_prod=0;
	cod_delel = (gchar *)gtk_entry_get_text(GTK_ENTRY(codigo_prod_field));
	sprintf(query,"select code from produtos where code = '%s';",cod_delel);
	autologger(query);
	estado = consultar(query);
	campo = mysql_fetch_row(estado);
	if(campo==NULL)
	{
		popup(NULL,"Produto já não existe");
		sprintf(stringer,"%i",tasker("produtos"));
		gtk_entry_set_text(GTK_ENTRY(codigo_prod_field),stringer);
		gtk_entry_set_text(GTK_ENTRY(nome_prod_field),"");
		gtk_entry_set_text(GTK_ENTRY(preco_prod_field),"");
		gtk_entry_set_text(GTK_ENTRY(peso_prod_field),"");
		gtk_entry_set_text(GTK_ENTRY(fornecedor_prod_field),"");
		gtk_entry_set_text(GTK_ENTRY(grupo_prod_field),"");
		gtk_entry_set_text(GTK_ENTRY(preco_faturado_prod_field),"");
		buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(observacao_prod_field));
		gtk_text_buffer_get_start_iter (buffer,&inicio);
		gtk_text_buffer_get_end_iter (buffer,&fim);
		gtk_text_buffer_delete (buffer,&inicio,&fim);
		gtk_widget_grab_focus (GTK_WIDGET(psq_prod_codigo_button));
		return 1;
	}
	sprintf(query,"delete from produtos where code = '%s';",cod_delel);
	erro = enviar_query(query);
	if(erro != 0)
	{
		popup(NULL,"Erro ao tentar exluir produto");
		return 1;
	}
	query[0] = '\0';
	sprintf(query,"select code from produtos where code = '%s';",cod_delel);
	estado = consultar(query);
	campo = mysql_fetch_row(estado);
	if(campo==NULL)
	{
		popup(NULL,"Deletado com sucesso");
		cancelar_prod();
		return 0;
	}
	else
	{
		popup(NULL,"Não foi possivel deletar");
	}
	return 0;
}
