int sig_und()
{
	sigla_und = malloc(51);
	sigla_und = (gchar *) gtk_entry_get_text(GTK_ENTRY(sigla_field));
	if(strlen(sigla_und)>50)
	{
		popup(NULL,"Sigla muito grande");
		gtk_widget_grab_focus(sigla_field);
		sigla_und_err=1;
		return 1;
	}
	else
	if(strlen(sigla_und)<=0)
	{
		popup(NULL,"Por favor, insira uma Sigla");
		gtk_widget_grab_focus(sigla_field);
		sigla_und_err=1;
		return 1;	
	}
	else
	{
		sigla_und_err=0;
		g_print("unidade: %s\n",sigla_und);
		gtk_widget_grab_focus(mult_field);	
	}
	return 0;
}
