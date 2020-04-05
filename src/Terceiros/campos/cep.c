int cep_terc()
{
	char *query;
	int cont=0,cont2=0;
	MYSQL_RES *vetor;
	MYSQL_ROW campos;
	query = malloc(QUERY_LEN);
	cep_ter = (gchar *) gtk_entry_get_text(GTK_ENTRY(cep_ter_field));
	if(strlen(cep_ter)>=MAX_CEP_LEN)
	{
		popup(NULL,"CEP muito grande\nPor favor verifique");
		gtk_widget_grab_focus(GTK_WIDGET(cep_ter_field));
		vet_erro[CEP_ERR]=1;
		return 1;
	}
	if(strlen(cep_ter)<=0&&criticar.cep!=0)
	{
		popup(NULL,"Por favor insira um cep");
		gtk_widget_grab_focus(GTK_WIDGET(cep_ter_field));
		vet_erro[CEP_ERR]=1;
		return 1;
	}
	if(strlen(cep_ter)!=CEP_LEN)
	{
		popup(NULL,"Insira o CEP com apenas numeros");
		return 1;
	}
	g_print("CEP: %s\n",cep_ter);
	autologger("CEP:");
	autologger(cep_ter);
	sprintf(query,"select descricao,tipo from logradouro where CEP = '%s'",cep_ter);
	vetor = consultar(query);
	if(vetor==NULL)
	{
		popup(NULL,"Erro na query para CEP\n\tConsulte suporte");
		return 1;
	}
	if((campos = mysql_fetch_row(vetor))==NULL)
	{
		popup(NULL,"CEP não encontrado,\n\tpor favor insira o endereço manualmente");
		autologger("CEP não encontrado,\n\tpor favor insira o endereço manualmente");
		autologger(cep_ter);
		return 1;
	}
	g_print("cep_len: %li\n",strlen(cep_ter));
	gtk_entry_set_text(GTK_ENTRY(address_ter_field),campos[0]);
	for(cont=0;cont<=5;cont++)
	{
		if(strcmp(campos[1],tip_logds[cont])==0)
		{
			gtk_combo_box_set_active(GTK_COMBO_BOX(rua_combo),cont+1);
			cont2 = 1;
 		}
	}
	if(cont2==0)
	{
		gtk_combo_box_set_active(GTK_COMBO_BOX(rua_combo),7);
	}
	vet_erro[CEP_ERR]=0;
	gtk_widget_grab_focus(address_ter_field);
	g_print("cep: %s\n",cep_ter);
	return 0;
}
