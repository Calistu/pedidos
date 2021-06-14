void est_ent_alterar_fun()
{
	char query[MAX_QUERY_LEN], data_gchar[42];
	MYSQL_RES *estado;
	MYSQL_ROW campo;
	int dia,mes,ano;

	GDateTime  *gdate;
	GTimeZone *timezone;

	alterando_mov_ent_est=1;

	if(est_ent_codigo_fun()!=0)
		return ;

	sprintf(query,"select * from movimento_estoque where code = %s",est_ent_cod_gchar);
	if(!(estado = consultar(query)))
		return ;

	if(!(campo = mysql_fetch_row(estado))){
		popup(NULL,"O movimento não existe");
		return ;
	}

	if(sscanf(campo[DATA_MOV_EST], "%d-%d-%d", &ano, &mes, &dia) == EOF){
    g_print("Erro no parser de data: %s\n",strerror(errno));
    return ;
  }

	timezone = g_time_zone_new_identifier(NULL);

	gdate = g_date_time_new(timezone,ano,mes,dia,0,0,0);
	if(!g_date_time_format(gdate,"%d/%m/%Y")){
		popup(NULL,"Data incorreta");
	}

	if(atoi(campo[EST_MOV_EST])){
		gtk_combo_box_set_active_id(GTK_COMBO_BOX(est_ent_est_combo),campo[EST_MOV_EST]);
	}

	gtk_entry_set_text(GTK_ENTRY(est_ent_data_entry),g_date_time_format(gdate,"%d/%m/%Y"));
	gtk_widget_activate(est_ent_data_entry);
	gtk_entry_set_text(GTK_ENTRY(est_ent_client_entry),campo[CLI_MOV_EST]);
	gtk_widget_activate(est_ent_client_entry);
	gtk_entry_set_text(GTK_ENTRY(est_ent_prod_entry),campo[PROD_MOV_EST]);
	gtk_widget_activate(est_ent_prod_entry);
	gtk_entry_set_text(GTK_ENTRY(est_ent_qnt_entry),campo[ENTS_MOV_EST]);
	gtk_widget_activate(est_ent_qnt_entry);


	if(atoi(campo[PED_MOV_EST])>0)
		gtk_entry_set_text(GTK_ENTRY(est_ent_ped_entry),campo[PED_MOV_EST]);
	gtk_widget_activate(est_ent_ped_entry);

	if(atoi(campo[TIPO_MOV_EST])  == COMPRA)
		gtk_combo_box_set_active(GTK_COMBO_BOX(est_ent_tipo_combo),1);

	if(atoi(campo[TIPO_MOV_EST])  == DEV_VENDA)
		gtk_combo_box_set_active(GTK_COMBO_BOX(est_ent_tipo_combo),2);

	gtk_combo_box_set_active(GTK_COMBO_BOX(est_ent_est_combo),atoi(campo[EST_MOV_EST]));

	est_ent_obs_gchar = strdup(campo[OBS_MOV_EST]);
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(est_ent_obs_view));
	gtk_text_buffer_set_text(GTK_TEXT_BUFFER(buffer), est_ent_obs_gchar, strlen(est_ent_obs_gchar));

	gtk_widget_set_sensitive(est_ent_altera_button,FALSE);
	gtk_widget_set_sensitive(est_ent_cod_entry,FALSE);
	gtk_widget_set_sensitive(est_ent_psqcod_button,FALSE);
}
