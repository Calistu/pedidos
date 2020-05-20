int cad_ico()
{

	int cont,cont2=0,linha=0;
	
	//imagem dos icones
	prd_ico = gtk_image_new_from_file(PROD_IMG);
	ter_ico = gtk_image_new_from_file(TERC_IMG);

	opr_ico = gtk_image_new_from_file(OPR_IMG);
	trs_ico = gtk_image_new_from_file(TRS_IMG);
	und_ico = gtk_image_new_from_file(UND_IMG);
	vin_ico = gtk_image_new_from_file(VINC_IMG);
	grp_ico = gtk_image_new_from_file(GRP_IMG);
	
	//label dos icones
	prd_lbl = gtk_label_new("Novo Produto");
	ter_lbl = gtk_label_new("Novo Terceiro");
	opr_lbl = gtk_label_new("Novo Operador");	
	trs_lbl = gtk_label_new("Nova Transação");
	und_lbl = gtk_label_new("Nova Unidade Medida");
	vin_lbl = gtk_label_new("Verificar vinculos");
	grp_lbl = gtk_label_new("Grupo de Produtos");
	
	//caixas onde ficarao os icones
	//cria eventos para cada botao
	for(cont=0;cont<CAD_ICO_QNT;cont++)
	{
		cad_box[cont] = gtk_box_new(1,0);
		//sprintf(name,"icone%i",cont);
		gtk_widget_set_name(cad_box[cont],"icone");
		eventos[cont] = gtk_event_box_new();
		gtk_container_add(GTK_CONTAINER(eventos[cont]),cad_box[cont]);
		if(cont2==ICOL)
		{
			linha++;
			cont2=0;
		}
		gtk_box_pack_start(GTK_BOX(cadastrosl[linha]),eventos[cont],0,0,45);
		//memset(name,0x0,strlen(name));
		cont2++;
	}
	
	//icone cadastro produto
    gtk_box_pack_end(GTK_BOX(cad_box[0]),prd_lbl,0,0,0);
	gtk_box_pack_end(GTK_BOX(cad_box[0]),prd_ico,0,0,0);

	//icone cadastro terceiro
	gtk_box_pack_end(GTK_BOX(cad_box[1]),ter_lbl,0,0,0);
	gtk_box_pack_end(GTK_BOX(cad_box[1]),ter_ico,0,0,0);

	//icone cadastro operadores
	gtk_box_pack_end(GTK_BOX(cad_box[2]),opr_lbl,0,0,0);
	gtk_box_pack_end(GTK_BOX(cad_box[2]),opr_ico,0,0,0);
	
	//icone cadastro de transacoes
    gtk_box_pack_end(GTK_BOX(cad_box[3]),trs_lbl,0,0,0);
	gtk_box_pack_end(GTK_BOX(cad_box[3]),trs_ico,0,0,0);
	
	//icone cadastro de unidades
	gtk_box_pack_end(GTK_BOX(cad_box[4]),und_lbl,0,0,0);
	gtk_box_pack_end(GTK_BOX(cad_box[4]),und_ico,0,0,0);
	
	//icone grupos	
	gtk_box_pack_end(GTK_BOX(cad_box[5]),grp_lbl,0,0,0);
	gtk_box_pack_end(GTK_BOX(cad_box[5]),grp_ico,0,0,0);
	
	//icone verificar vinculos	
	gtk_box_pack_end(GTK_BOX(cad_box[6]),vin_lbl,0,0,0);
	gtk_box_pack_end(GTK_BOX(cad_box[6]),vin_ico,0,0,0);
	
	
	g_signal_connect(eventos[0],"button_press_event",G_CALLBACK(cad_prod),NULL);
	g_signal_connect(eventos[0],"button_press_event",G_CALLBACK(inicializar_prod),NULL);
	
	g_signal_connect(eventos[1],"button_press_event",G_CALLBACK(cad_terc),NULL);
	g_signal_connect(eventos[1],"button_press_event",G_CALLBACK(inicializar_ter),NULL);
	
	g_signal_connect(eventos[2],"button_press_event",G_CALLBACK(cad_oper),NULL);

	g_signal_connect(eventos[4],"button_press_event",G_CALLBACK(cad_und),NULL);
	
	g_signal_connect(eventos[5],"button_press_event",G_CALLBACK(cad_grupo),NULL);
	
	g_signal_connect(eventos[6],"button_press_event",G_CALLBACK(vin_chaves),NULL);
	return 0;
}
