int relat_mov_query_fun()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	int campos_qnt=0;
	char ini_query[] = "select";
	char end_query[MAX_QUERY_LEN];
	char query[MAX_QUERY_LEN];
	char campo_query_cp[MAX_QUERY_ELM_LEN];

	sprintf(query,"select b.query,t.inner_query from criador_relat as a inner join relat_tab_campos as b inner join relat_tabelas_id as t on a.campos = b.code and a.tabela = t.code where a.code = %s",relat_mov_codigo_gchar);

	if((res = consultar(query))==NULL){
		popup(NULL,"Não foi possivel receber campos do relatorio");
		return 1;
	}

	while((row = mysql_fetch_row(res))!=NULL){

		if(campos_qnt==0)
			sprintf(mov_query.campos,"%s, ",row[0]);
		else
		{
			sprintf(mov_query.campos," %s %s,",campo_query_cp,row[0]);//backup + campo novo
		}

		strcpy(campo_query_cp,mov_query.campos); //pega backup query

		strcpy(mov_query.campo_query[campos_qnt],row[0]);

		g_print("%s\n",mov_query.campo_query[campos_qnt]);
		sprintf(end_query,"%s",row[1]);

		campos_qnt++;
	}
	mov_query.campos_qnt = campos_qnt;


	mov_query.campos[strlen(mov_query.campos)-1] = '\0';
	relat_mov_query_gchar = malloc(strlen(ini_query)+strlen(mov_query.campos)+strlen(end_query)+1);
	sprintf(relat_mov_query_gchar,"%s%s%s",ini_query,mov_query.campos,end_query);
	gtk_entry_set_text(GTK_ENTRY(relat_mov_query_entry),relat_mov_query_gchar);
	return 0;
}
