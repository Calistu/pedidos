#include "impressao.c"
#define CLI_ROW_POS 2
#define IMP_PORT1 "LPT1"
#define IMP_PORT2 "LPT2"
#define BROTHER_IMP 1
#define SAMSUNG_IMP 2
int imp_opc=0;
GtkWidget *msg_abrir_orc_window;
static GtkWidget *botao_radio1,*botao_radio2,*botao_radio3,*botao_radio4;
static char*gerando_file;

int iniciar_impressao(char *gerado)
{
	#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
	char *chamada;
	chamada = malloc(strlen(gerando_file));
	#pragma GCC diagnostic warning "-Wunused-but-set-variable"
	g_print("%s para impressora\n",gerado);	
	#ifdef WIN32
	g_print("%s para LPT1\n",gerado);
	if(imp_opc==1)
		sprintf(chamada,"xcopy %s LPT1",gerado);
	if(imp_opc==2)
		sprintf(chamada,"xcopy %s LPT2",gerado);
	
	STARTUPINFO infoBina={sizeof(infoBina)};
	PROCESS_INFORMATION processInfoBina;
	infoBina.dwFlags = STARTF_USESHOWWINDOW;
	infoBina.wShowWindow = SW_HIDE;
	if (CreateProcess(NULL, chamada, NULL, NULL, FALSE, 0, NULL, NULL, &infoBina, &processInfoBina)) 
    {
		WaitForSingleObject(processInfoBina.hProcess, INFINITE);
		CloseHandle(processInfoBina.hProcess);
		CloseHandle(processInfoBina.hThread);
	}	
	#endif
	#ifdef __linux__
		popup(NULL,gerado);
	#endif
	return 0;
}

int desenhar_orcamento()
{
	char *chamada,*gerado;
	chamada = malloc(strlen(PDF_GEN)+strlen(gerando_file)*2+10);
	gerado = malloc(strlen(gerando_file));
	sprintf(gerado,"%s",gerando_file);
	gerado[strlen(gerado)-5] = '\0';
	g_print("de %s para %s.pdf\n",gerando_file,gerado);
	sprintf(chamada,"%s %s %s.pdf",PDF_GEN,gerando_file,gerado);
	#ifdef WIN32
	STARTUPINFO infoBina={sizeof(infoBina)};
	PROCESS_INFORMATION processInfoBina;
	infoBina.dwFlags = STARTF_USESHOWWINDOW;
	infoBina.wShowWindow = SW_HIDE;
	if (CreateProcess(NULL, chamada, NULL, NULL, FALSE, 0, NULL, NULL, &infoBina, &processInfoBina)) 
    {
		WaitForSingleObject(processInfoBina.hProcess, INFINITE);
		CloseHandle(processInfoBina.hProcess);
		CloseHandle(processInfoBina.hThread);
		sprintf(gerado,"%s.pdf",gerado);
		iniciar_impressao(gerado);
	}
	else
	{
		popup(NULL,"Não foi possivel gerar documento");
		return 1;
	}
	#endif 
	#ifdef __linux__
	if(system(chamada)!=0)
	{
		popup(NULL,"Não foi possivel gerar documento");
		return 1;
	}
	iniciar_impressao(gerado);
	#endif
	return 0;
}

int iniciar_escolha()
{	
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(botao_radio1)))
	{
		g_print("Escolhida impressora 1\n");
		imp_opc = BROTHER_IMP;
		desenhar_orcamento();
	}
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(botao_radio2)))
	{
		
		imp_opc = SAMSUNG_IMP;
		desenhar_orcamento();
	}
	g_print("Escolhida impressora %i\n",imp_opc);
	return 0;
}

int escolher_finalizacao()
{
	GtkWidget *janela;
	GtkWidget *botoes_frame,*botoes_caixa;
	
	GtkWidget *botao_confirma,*botao_cancela;
	GtkWidget *caixa_opcoes;
	
	GtkWidget *caixa_grande;

	janela = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(janela,400,400);
	gtk_window_set_position(GTK_WINDOW(janela),3);
	gtk_window_set_keep_above(GTK_WINDOW(janela),TRUE);
	
	botoes_caixa = gtk_box_new(1,0);
	botoes_frame = gtk_frame_new("Escolha como finalizar:");
	
	caixa_grande = gtk_box_new(1,0);
		
	botao_confirma = gtk_button_new_with_label("Concluir");
	botao_cancela = gtk_button_new_with_label("Cancelar");
	caixa_opcoes = gtk_box_new(0,0);
	gtk_box_pack_start(GTK_BOX(caixa_opcoes),botao_confirma,0,0,50);
	gtk_box_pack_start(GTK_BOX(caixa_opcoes),botao_cancela,0,0,5);
	
	botao_radio1 = gtk_radio_button_new_with_label(NULL,"Enviar para Impressora BROTHER");
	botao_radio2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(botao_radio1),"Enviar para Impressora SAMSUNG");
	
	botao_radio3 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(botao_radio2),"Abrir PDF");
	botao_radio4 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(botao_radio3),"Abrir HTML");

	gtk_box_pack_start(GTK_BOX(botoes_caixa),botao_radio1,0,0,5);
	gtk_box_pack_start(GTK_BOX(botoes_caixa),botao_radio2,0,0,5);
	gtk_box_pack_start(GTK_BOX(botoes_caixa),botao_radio3,0,0,5);
	gtk_box_pack_start(GTK_BOX(botoes_caixa),botao_radio4,0,0,5);
		
	gtk_container_add(GTK_CONTAINER(botoes_frame),botoes_caixa);

	gtk_box_pack_start(GTK_BOX(caixa_grande),botoes_frame,0,0,50);
	gtk_box_pack_start(GTK_BOX(caixa_grande),caixa_opcoes,0,0,10);
	
	gtk_container_add(GTK_CONTAINER(janela),caixa_grande);
	g_signal_connect (botao_confirma,"clicked",G_CALLBACK(iniciar_escolha),NULL);
	g_signal_connect (botao_cancela,"clicked",G_CALLBACK(close_window_callback),janela);
	gtk_widget_show_all(janela);
	return 0;
}

int gerar_orc()
{
	g_print("gerando orçamento...\n");
	int cont,color=0;
	char *query;
	int conta_linhas=0;
	int erro;
	char *formata_float,*formata_float2,*formata_float3;
	float chartofloat,totalfloat;
	MYSQL_RES *vetor;
	MYSQL_ROW campos;
	MYSQL_RES *res;
	MYSQL_ROW row;
	conta_linhas = 0;
	gerando_file = malloc(MAX_PATH_LEN*2);
	
	formata_float = malloc(MAX_PRECO_LEN); //desconto
	formata_float2 = malloc(MAX_PRECO_LEN); //quantidade
	formata_float3 = malloc(MAX_PRECO_LEN); //preco
	
	query = malloc(MAX_QUERY_LEN);
	if(codigo_orc()!=0)
		return 1;
	sprintf(gerando_file,"%simp%s.html",ORC_PATH,codigo_orc_gchar);

	
	sprintf(query,"select * from orcamentos where code = %s",codigo_orc_gchar);
	vetor = consultar(query);
	if(vetor==NULL)
	{
		popup(NULL,"Erro na query! Por favor, Consulte com suporte.");
		g_print("Erro na query de codigo no orcamento\n");
		autologger("Erro na query de codigo no orcamento\n");
		autologger(query);
		vet_erro[COD_ERR] = 0;
		gtk_widget_grab_focus(codigo_orc_entry);
		return 1;
	}
	gerar_total_geral();
	campos = mysql_fetch_row(vetor);
	if(campos == NULL)
	{
		if(codigo_cli_orc()!=0)
					return 1;	
		sprintf(query,"insert into orcamentos(code,vendedor,cliente,dia,observacoes,total) values(%s,1,%s,'%s-%s-%s','%s',0.0);",codigo_orc_gchar,cliente_orc_gchar,ano_sys,mes_sys,dia_sys,observacoes_orc_gchar);
		erro = enviar_query(query);
		if(erro != 0 )
		{
			popup(NULL,"Erro ao tentar gerar orçamento");
			return 1;
		}
		for(cont=1;cont<itens_qnt-1;cont++)
		{
			if(ativos[cont].id == 1)
			{
				if(codigo_cli_orc()!=0)
					return 1;			
				if(qnt_prod_orc(qnt_prod_orc_entry[cont],cont)!=0)
					return 1;
				if(preco_prod_orc(preco_prod_orc_entry[cont],cont)!=0)
					return 1;
				if(total_prod_orc(total_prod_orc_entry[cont],cont)!=0)
					return 1;
				if(strlen(ativos[cont].desconto_c)<=0)
				{
					strcpy(ativos[cont].desconto_c,"0.0");
				}
				
				sprintf(query,"insert into Produto_Orcamento(code, item, produto, unidades, valor_unit, tipodesc, desconto, total) values(%s, %i, %i, %s, %s, %i, %s, %s);",codigo_orc_gchar,cont,ativos[cont].produto, ativos[cont].qnt_c, ativos[cont].preco_c, ativos[cont].tipodesc,  ativos[cont].desconto_c , ativos[cont].total_c);
				erro = enviar_query(query);
				if(erro != 0 )
				{
					popup(NULL,"Erro ao tentar gerar orçamento");
					return 1;
				}
				sprintf(query,"update orcamentos set total = (select sum(total) from Produto_Orcamento where code = %s) where code = %s",codigo_orc_gchar,codigo_orc_gchar);
				erro = enviar_query(query);
				if(erro != 0 )
				{
					popup(NULL,"Erro ao tentar gerar orçamento");
					return 1;
				}
			}
		}
		if(ativos_qnt<=1)
		{
			popup(NULL,"Não há produtos no orçamento");
			return 1;
		}
		
	}
	else
	{
		gtk_entry_set_text(GTK_ENTRY(cliente_orc_entry),campos[CLI_ROW_POS]);
		if(codigo_cli_orc()!=0)
			return 1;			
	}
	
	orc = fopen(gerando_file,"w");
	if(orc==NULL)
	{
		popup(NULL,"Não está sendo possivel gerar o arquivo\n\tFeche o navegador");
		return 1;
	}
	
	sprintf(query,"select distinct (%s%s),o.total from orcamentos as o join terceiros as t where o.code = %s",DATE_QUERY,codigo_orc_gchar,codigo_orc_gchar);
	
	vetor = consultar(query);
	if(vetor==NULL)
	{
		popup(NULL,"Erro na query! Por favor, Consulte com suporte.");
		g_print("Erro na query de codigo no orcamento\n");
		autologger("Erro na query de codigo no orcamento\n");
		autologger(query);
		fclose(orc);
		return 1;
	}	
	campos = mysql_fetch_row(vetor);
	if(campos == NULL)
	{
		popup(NULL,"Erro ao tentar receber orçamento");
		fclose(orc);
		return 1;
	}
	fprintf(orc,"<!DOCTYPE html>\n");
	fprintf(orc,"<html>\n");
	fprintf(orc,"<head>\n");
	fprintf(orc,"<meta charset=\"utf-8\"/>");
	fprintf(orc,"<link href=\"%s\" rel=\"stylesheet\">\n",CSS_ORC);
	fprintf(orc,"<title>Orcamento</title>\n");
	fprintf(orc,"</head>\n");
	
	fprintf(orc,"<body>\n");
	
	fprintf(orc,"<div id=\"caixa-imp\">\n");
	fprintf(orc,"<div id=\"div-titulo\">\n");
	fprintf(orc,"<img id=\"logo-img\" src=\"%s\" alt=\"PETITTO\">\n",IMG_IMP_LOGO);
	fprintf(orc,"<div id=\"orc-infos\" align=left>\n");
	
	fprintf(orc,"<table>\n");
	
	fprintf(orc,"<tr>\n");
	fprintf(orc,"<td id=\"info-row1\">Número do Orcamento</td>\n");
	//fprintf(orc,"<td id=\"info-row1\">Cliente:</td>\n"); 
	fprintf(orc,"<td id=\"info-row1\">Data</td>\n");
	fprintf(orc,"</tr>\n");
	
	fprintf(orc,"<tr>\n");
	fprintf(orc,"<td>%s</td>\n",codigo_orc_gchar);
	fprintf(orc,"<td>%s</td>\n",campos[0]);
	fprintf(orc,"<tr>\n");
	
	fprintf(orc,"</table>\n");
	
	fprintf(orc,"</div>\n");
	
	if(imp_cli(cliente_orc_gchar)!=0)
		return 1;
	sprintf(query,"select p.code,p.nome,o.unidades,u.nome,o.valor_unit,o.tipodesc,o.desconto,o.total from Produto_Orcamento as o inner join produtos as p on p.code = o.produto join unidades as u on u.code = p.unidade where o.code = %s;",codigo_orc_gchar);
	vetor = consultar(query);
	if(vetor==NULL)
	{
		popup(NULL,"Erro na query! Por favor, Consulte com suporte.");
		g_print("Erro na query de codigo no orcamento\n");
		autologger("Erro na query de codigo no orcamento\n");
		autologger(query);
		vet_erro[COD_ERR] = 0;
		gtk_widget_grab_focus(codigo_orc_entry);
		fclose(orc);
		return 1;
	}
	cont=1;
	fprintf(orc,"<div id=\"separator\">");fprintf(orc,"</div>");
	fprintf(orc,"<div id=\"campo-itens\">\n");
	fprintf(orc,"<table>\n");
	fprintf(orc,"<tr>\n");
	//fprintf(orc,"<td id=\"prod-row1\">Código</td>\n",IMG_IMP_QNT);  
	fprintf(orc,"<td id=\"prod-row1\"><img src=\"%s\" alt=\"\">Quantidade Unitária</td>\n",IMG_IMP_QNT);  
	fprintf(orc,"<td id=\"prod-row1\"><img src=\"%s\" alt=\"\">Descrição do Produto</td>\n",IMG_IMP_PROD);
	fprintf(orc,"<td id=\"prod-row1\"><img src=\"%s\" alt=\"\">Valor Unitário.</td>\n",IMG_MONEY);
	fprintf(orc,"<td id=\"prod-row1\">Desconto</td>\n");
	fprintf(orc,"<td id=\"prod-row1\">Valor Total</td>\n");
	fprintf(orc,"</tr>\n");
	
	while((campos = mysql_fetch_row(vetor))!=NULL)
	{
		if(color==1)
		{
			fprintf(orc,"<tr id=\"coluna-colorida\">\n");		
			color=0;
		}
		else
		{
			fprintf(orc,"<tr>\n");		
			color=1;
		}
		if(campos[3][strlen(campos[3])-1] != 's'&&atoi(campos[2])>1)
			fprintf(orc,"<td>%s %ss</td>\n",campos[2],campos[3]);  
		else
			fprintf(orc,"<td>%s %s</td>\n",campos[2],campos[3]);  
		fprintf(orc,"<td>Cod. %s: %s</td>\n",campos[0],campos[1]);
		sprintf(formata_float,"%s",campos[4]);
		critica_real(formata_float,NULL);
		fprintf(orc,"<td>R$ %.2f</td>\n",atof(formata_float));
		
		if(atoi(campos[5])==0)
		{
			sprintf(formata_float,"%s",campos[6]);//pega desconto
			g_print("campos[5] : %s\n",campos[6]);
			critica_real(formata_float,NULL);
			
			sprintf(formata_float2,"%s",campos[2]);//pega quantidade
			sprintf(formata_float3,"%s",campos[4]);//pega preco
			critica_real(formata_float2,NULL);
			critica_real(formata_float3,NULL);
			totalfloat = atof(formata_float2)*atof(formata_float3);
			
			//conversao de R$ para %
			//2.2 = 100
			//1.1 = x
			
			chartofloat = (atof(formata_float)*100)/totalfloat;
			fprintf(orc,"<td>%.2f %c</td>\n",chartofloat,37);//desconto em R$
		}
		else
		if(atoi(campos[5])==1)
		{
			sprintf(formata_float,"%s",campos[6]);
			g_print("campos[5] : %s\n",campos[6]);
			critica_real(formata_float,NULL);
			fprintf(orc,"<td>%.2f %c</td>\n",atof(formata_float),37);//desconto em %
		}
		
		sprintf(formata_float,"%s",campos[7]);
		g_print("campos[5] : %s\n",campos[7]);
		critica_real(formata_float,NULL);
		fprintf(orc,"<td>R$ %.2f</td>\n",atof(formata_float));//total
		fprintf(orc,"</tr>\n");
		conta_linhas++;
		g_print("linha %i\n",conta_linhas);
		if(conta_linhas>=12)
		{
			for(int row1=0;row1<5;row1++)
			{
				fprintf(orc,"<tr id=\"pula-linha\">\n");
				for(int row2=0;row2<5;row2++)
					fprintf(orc,"<td id=\"pula-linha\"></td>\n");
				fprintf(orc,"</tr>\n");
			}
			conta_linhas=-10;
		}
		cont++;
	}
	if(cont == 0)
	{
		popup(NULL,"Sem itens no orçamento");
		fclose(orc);
		return 0;
	}
	sprintf(query,"select total,observacoes from orcamentos where code = %s",codigo_orc_gchar);	
	res = consultar(query);
	if(res==NULL)
	{
		popup(NULL,"Erro de query! Consulte suporte");
		autologger("Erro ao tentar receber total dos produtos no orcamento");
		return 1;
	}
	if((row = mysql_fetch_row(res))==NULL)
	{
		popup(NULL,"Nenhum Total encontrado\n\tCaso seja erro, consulte suporte");
		autologger("Erro ao tentar receber total dos produtos no orcamento");
		return 1;
	}
	sprintf(formata_float,"%s",row[0]);
	//critica_real(formata_float,NULL);
	//fprintf(orc,"<td></td>\n");	
	//fprintf(orc,"<td></td>\n");	
	//fprintf(orc,"<td></td>\n");	
	//fprintf(orc,"<td></td>\n");	
	fprintf(orc,"<td  colspan=\"5\" id=\"total-geral\">Total Geral: R$ %.2f</td>\n",atof(formata_float));	
	fprintf(orc,"</table>\n");

	fprintf(orc,"<div>\n");

	fprintf(orc,"</div>\n");	
	fprintf(orc,"</div>\n");
	observacoes_orc_get();
	fprintf(orc,"<div id=\"obs-div1\">\n");
	fprintf(orc,"Observacões\n");
	fprintf(orc,"<div id=\"obs-div2\">\n");
	fprintf(orc,"Cod. Cliente : %s %s",cliente_orc_gchar,observacoes_orc_gchar);
	fprintf(orc,"</div>\n");
	fprintf(orc,"</div>\n");
	fprintf(orc,"</body>\n");
	fprintf(orc,"</html>\n");
	fclose(orc);
	escolher_finalizacao();
	g_print("Abrindo janela de escolha para o arquivo\n");

	
	return 0;
}