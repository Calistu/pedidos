void notf_consultar_fun(GtkWidget *botao, struct _notf_strct *notf){

  gchar id[12], parcela[12];
  sprintf(id, "%i", notf->id);
  sprintf(parcela, "%i", notf->id2);

  if(notf->tipo == TP_TIT_PAG){
    if(janelas_gerenciadas.vetor_janelas[CAD_TIT_PAG_WND].fun())
      return ;
    gtk_entry_set_text(GTK_ENTRY(cad_pag_code_entry), id);
    gtk_entry_set_text(GTK_ENTRY(cad_pag_parcela_spin), parcela);
    gtk_widget_activate(cad_pag_code_entry);
  }

  if(notf->tipo == TP_TIT_REC){
    if(janelas_gerenciadas.vetor_janelas[CAD_TIT_REC_WND].fun())
      return ;
    gtk_entry_set_text(GTK_ENTRY(cad_rec_code_entry), id);
    gtk_entry_set_text(GTK_ENTRY(cad_rec_parcela_spin), parcela);
    gtk_widget_activate(cad_rec_code_entry);
    gtk_widget_activate(cad_rec_parcela_spin);
  }
}

void notf_popupver_fun(GtkTreeView  *tree_view, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data){

  GtkTreeSelection *selection;
  GtkTreeIter iter;
  MYSQL_RES *res;
  MYSQL_ROW row;
  char query[MAX_QUERY_LEN];
  gchar *id = malloc(12), *parcela = malloc(12);

  enum{
    ID_COL,
    PARC_COL,
    NOME_COL,
    DESCR_COL,
    STAT_COL,
    N_COLUMNS
  };

  GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(tree_view));
  selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (tree_view));
  if(!gtk_tree_selection_get_selected(selection, &model, &iter))
    return ;

  gtk_tree_model_get(model, &iter, ID_COL, &id, PARC_COL, &parcela, -1);

  sprintf(query, "select tipo_titulo from titulos where code = %s", id);
  if(!(res = consultar(query))){
    popup(NULL,"Não foi possível consultar título");
    return ;
  }
  if(!(row = mysql_fetch_row(res))){
    popup(NULL,"Título não existente");
    return ;
  }

  struct _notf_strct *notf = malloc(sizeof(struct _notf_strct));
  notf->id = atoi(id);
  notf->id2 = atoi(parcela);
  notf->tipo = atoi(row[0]);

  GtkWidget *botao = gtk_button_new_with_label("Consultar");
  GtkWidget *box = gtk_box_new (0,0);
  GtkWidget *destroyed = gtk_bin_get_child (user_data);

  if(destroyed && GTK_IS_WIDGET(destroyed))
    gtk_widget_destroy(destroyed);

  gtk_box_pack_start(GTK_BOX(box), botao,0,0,5);
  g_signal_connect(botao, "clicked", G_CALLBACK(notf_consultar_fun), notf);
  g_signal_connect(botao, "clicked", G_CALLBACK(esconder_widget_callback), user_data);

  gtk_container_add(GTK_CONTAINER(user_data), box);

  GdkRectangle rect;
  gtk_tree_view_get_cell_area(tree_view, path, column, &rect);
  gtk_popover_set_pointing_to(GTK_POPOVER(user_data), &rect);
  gtk_widget_show_all(user_data);
}

int notificacoes_receber(){
  MYSQL_RES *res;
  MYSQL_ROW row;
  char query[MAX_QUERY_LEN];

  notificacao_pendencias = 0;

  sprintf(query,
    "select p.parcelas_id, p.posicao, DATE_FORMAT(p.data_vencimento,'%%d/%%m/%%Y'), t.razao, tit.status, tit.tipo_titulo from parcelas_tab as p inner join titulos as tit inner join terceiros as t on p.parcelas_id = tit.code  and tit.cliente = t.code where p.data_vencimento >= CURDATE() - INTERVAL %i DAY and p.data_vencimento <= CURDATE() + INTERVAL %i DAY and status != %i"
    ,NOTF_DIAS,NOTF_DIAS,STAT_QUITADO);

  if((res = consultar(query))){
    notificacao_pendencias += mysql_num_rows(res);
  }
  return 0;
}

int notificacoes_button_update(){

  gtk_button_set_image(GTK_BUTTON(penden_button),gtk_image_new_from_file(EMBLEM_GENERIC));
  notificacoes_receber();
  gchar qnt[30];

  sprintf(qnt,"%i",notificacao_pendencias);

  GtkWidget *child = gtk_bin_get_child(GTK_BIN(penden_button));
  if(child){
    g_object_ref(child);
    gtk_container_remove(GTK_CONTAINER(penden_button), child);
  }

  GtkWidget *circle = gtk_layout_new(NULL,NULL);
  GtkWidget *label = gtk_label_new(qnt);
  GtkWidget *box = gtk_box_new(0,0);
  GtkWidget *image = gtk_image_new_from_file(EMBLEM_GENERIC);
  GtkWidget *evento = gtk_event_box_new();

  gtk_box_pack_start(GTK_BOX(box), label,0,0,0);
  if(notificacao_pendencias){
      gtk_widget_set_name(box,"bar_buttons_notif");
  }else{
    gtk_widget_set_name(box,"bar_buttons_notif_clean");
  }


  gtk_layout_put(GTK_LAYOUT(circle),box,20,20);
  gtk_layout_put(GTK_LAYOUT(circle),image,0,0);

  gtk_container_add(GTK_CONTAINER(evento),circle);
  gtk_widget_set_events(evento,GDK_BUTTON_PRESS_MASK);

  gtk_widget_set_size_request(GTK_WIDGET(box),20,20);
  gtk_layout_set_size(GTK_LAYOUT(circle),40,40);

  g_signal_connect(evento,"button_press_event",G_CALLBACK(notificacoes_wnd),NULL);

  gtk_container_add(GTK_CONTAINER(GTK_BIN(penden_button)),evento);
  gtk_widget_show_all(penden_button);

  return 0;
}

void notificacoes_wnd(){

  if(!app_is_ativo())
    return ;

  GtkWidget *notf_wnd = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  /*registrando a janela para o reg_win*/
	janelas_gerenciadas.vetor_janelas[REG_NOTIF].reg_id = REG_NOTIF;
	if(ger_janela_aberta(notf_wnd, &janelas_gerenciadas.vetor_janelas[REG_NOTIF]))
		return ;
	janelas_gerenciadas.vetor_janelas[REG_NOTIF].janela_pointer = notf_wnd;

  g_signal_connect(notf_wnd,"destroy",G_CALLBACK(ger_janela_fechada),&janelas_gerenciadas.vetor_janelas[REG_NOTIF]);

  gtk_widget_set_name(notf_wnd,"notificacoes");
	gtk_window_set_position(GTK_WINDOW(notf_wnd),3);
	gtk_window_set_resizable(GTK_WINDOW(notf_wnd),FALSE);
	gtk_window_set_title(GTK_WINDOW(notf_wnd),"Notificações");
	gtk_window_set_icon_name(GTK_WINDOW(notf_wnd),"software-update-urgent");
  gtk_window_set_transient_for(GTK_WINDOW(notf_wnd),GTK_WINDOW(janela_principal));
  gtk_container_set_border_width (GTK_CONTAINER (notf_wnd), 10);

  GtkWidget *notf_box = gtk_box_new(1,0);
  GtkWidget *notf_scroll = gtk_scrolled_window_new(NULL,NULL);
  GtkWidget *notf_tree_view = gtk_tree_view_new();
  GtkWidget *notf_frame = gtk_frame_new("Notificações");

  GtkWidget *popupver = gtk_popover_new(notf_tree_view);
  gtk_popover_set_modal (GTK_POPOVER(popupver), TRUE);
  gtk_popover_set_constrain_to (GTK_POPOVER(popupver), GTK_POPOVER_CONSTRAINT_WINDOW);

  GtkTreeIter campos;
  MYSQL_RES *res;
  MYSQL_ROW row;

  char query[MAX_QUERY_LEN];
  enum{
    ID_COL,
    PARC_COL,
    NOME_COL,
    DESCR_COL,
    STAT_COL,
    N_COLUMNS
  };

  GtkTreeViewColumn *coluna_id = gtk_tree_view_column_new_with_attributes(
    "ID",
    gtk_cell_renderer_text_new (),
    "text",
    ID_COL,
    NULL);

  GtkTreeViewColumn *coluna_parc = gtk_tree_view_column_new_with_attributes(
    "Parcela",
  gtk_cell_renderer_text_new (),
    "text",
    PARC_COL,
    NULL );

  GtkTreeViewColumn *coluna_nome = gtk_tree_view_column_new_with_attributes(
    "Nome",
    gtk_cell_renderer_text_new (),
    "text",
    NOME_COL,
    NULL);

  GtkTreeViewColumn *coluna_descr = gtk_tree_view_column_new_with_attributes(
    "Descrição",
  gtk_cell_renderer_text_new (),
    "text",
    DESCR_COL,
    NULL );

  GtkTreeViewColumn *coluna_status = gtk_tree_view_column_new_with_attributes(
    "Status",
  gtk_cell_renderer_text_new (),
    "text",
    STAT_COL,
    NULL );

  gtk_tree_view_append_column(GTK_TREE_VIEW(notf_tree_view), coluna_id);
  gtk_tree_view_append_column(GTK_TREE_VIEW(notf_tree_view), coluna_parc);
  gtk_tree_view_append_column(GTK_TREE_VIEW(notf_tree_view), coluna_nome);
  gtk_tree_view_append_column(GTK_TREE_VIEW(notf_tree_view), coluna_descr);
  gtk_tree_view_append_column(GTK_TREE_VIEW(notf_tree_view), coluna_status);

  GtkTreeStore *modelo = gtk_tree_store_new(N_COLUMNS,G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

  sprintf(query,"select p.parcelas_id, p.posicao, DATE_FORMAT(p.data_vencimento,'%%d/%%m/%%Y'), t.razao, tit.status, tit.tipo_titulo from parcelas_tab as p inner join titulos as tit inner join terceiros as t on p.parcelas_id = tit.code  and tit.cliente = t.code where p.data_vencimento >= CURDATE() - INTERVAL %i DAY and p.data_vencimento <= CURDATE() + INTERVAL %i DAY  and status != %i",
  NOTF_DIAS, NOTF_DIAS, STAT_QUITADO
  );
	if((res = consultar(query))){
    int notif_qnt=0;
  	while((row = mysql_fetch_row(res))!=NULL){
      char id[12];
      char parcela[12];
      char nome[20];
      char descr[100];
      char status[30];
      char terceiro[15];

      strcpy(id , row[0]);
      strcpy(parcela , row[1]);

      switch (atoi(row[5])) {
        case TP_TIT_REC:
          sprintf(nome,"Título à Receber");
          sprintf(terceiro,"Cliente");
          break;
        case TP_TIT_PAG:
          sprintf(nome,"Título à Pagar");
          sprintf(terceiro,"Fornecedor");
          break;
        default:
          sprintf(nome,"Título sem Tipo");
          break;
      }

      sprintf(descr,"Parcela %s do Título %s - %s '%s'com vencimento em %s", parcela, id, terceiro, row[3], row[2]);
      switch (atoi(row[4])) {
        case STAT_QUITADO:
          sprintf(status,"Título já Quitado");
          break;
        case STAT_PARC_BAIXA:
          sprintf(status,"Título parcialmente Baixado");
          break;
        case STAT_PENDENTE:
          sprintf(status,"Título pendente");
          break;
        default:
          sprintf(status,"Erro");
          break;
      }
      gtk_tree_store_append(modelo,&campos,NULL);
      g_print("Inserindo codigo: %s nome: %s\n",row[0],row[1]);
      gtk_tree_store_set(modelo,&campos,
      ID_COL, id,
      PARC_COL, parcela,
      NOME_COL, nome,
      DESCR_COL, descr,
      STAT_COL, status
      ,-1);
      notif_qnt++;
  	}

    if(!notif_qnt){
      gtk_tree_store_append(modelo,&campos,NULL);
      gtk_tree_store_set(modelo,&campos,
      NOME_COL,"Aviso",
      DESCR_COL,"Nenhuma notificação!",
      STAT_COL,"Sem status"
      ,-1);
    }
  }
  gtk_tree_view_set_model(GTK_TREE_VIEW(notf_tree_view),GTK_TREE_MODEL(modelo));

  g_signal_connect(notf_tree_view, "row-activated", G_CALLBACK(notf_popupver_fun), popupver);

  gtk_box_pack_start(GTK_BOX(notf_box),notf_tree_view,0,0,10);
  gtk_container_add(GTK_CONTAINER(notf_scroll),notf_box);
  gtk_container_add(GTK_CONTAINER(notf_frame),notf_scroll);
  gtk_container_add(GTK_CONTAINER(notf_wnd),notf_frame);

  gtk_widget_set_size_request(notf_scroll,900,400);
  gtk_widget_set_size_request(notf_box,900,400);
  gtk_widget_set_size_request(notf_wnd,900,400);

  gtk_widget_show_all(notf_wnd);
  return ;
}
