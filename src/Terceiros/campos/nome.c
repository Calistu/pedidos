int name_terc()
{
	nomes_ter = (gchar *) gtk_entry_get_text(GTK_ENTRY(name_ter_field));
	if(strlen(nomes_ter)<=0)
	{
		popup(NULL,"Por favor insira um nome para o terceiro");
		gtk_widget_grab_focus(GTK_WIDGET(name_ter_field));
		return 1;
	}

	gtk_widget_grab_focus(ter_nome_fantasia_entry);
	return 0;
}
