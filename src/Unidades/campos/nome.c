int name_und()
{
	nomes_und = malloc(20);
	if(nomes_und==NULL)
	{
		printf("erro de memoria %s\n",strerror(errno));
		return 1;
	}
	nomes_und = (gchar *) gtk_entry_get_text(GTK_ENTRY(name_und_field));	
	if(strlen(nomes_und)>=20)
	{
		popup(NULL,"Nome inserido é muito grande");
		gtk_widget_grab_focus(GTK_WIDGET(name_und_field));
		g_print("nome: %s\n",nomes_und);
		return 1;
	}
	else
	if(strlen(nomes_und)<=0)
	{
		popup(NULL,"Por favor, insira um Nome");
		gtk_widget_grab_focus(GTK_WIDGET(name_und_field));
		g_print("nome: %s\n",nomes_und);
		return 1;		
	}
	else
	{
		
		gtk_widget_grab_focus(sigla_und_field);
	}
	g_print("nome: %s\n",nomes_und);
	
	return 0;
}
