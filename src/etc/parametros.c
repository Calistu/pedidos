#include "personalizacao.c"
//combo_box de terceiros 
GtkWidget **campos_de_critica;
//combo_box de produtos
GtkWidget *prod_fornecedor,*prod_grupo,*prod_preco,*prod_total,*prod_peso,*prod_unidade,*prod_marca;
int ter_critic_campos_qnt =10;
struct 
{
	int janela_init;
	int janela_keep_above;
	char wallpaper[300];
}personalizacao;
struct
{
	struct parametros
	{
		int doc;
		int tipodoc;
		int endereco;
		int cep;
		int tipo;
		int celular;
		int contatoc;
		int telefone;
		int contatot;
		int email;
		int contatoe;
		int observacoes;
		int fornecedor;
		int grupo;
		int preco;
		int total;
		int peso;
		int unidade;
		int marca;
		int vincula_prod_obs;
	}criticar;
}terceiros,produtos;
#define CAMPOS_QNT 22
int vet_erro[CAMPOS_QNT+1];

#define COD_ERR 0
#define DOC_ERR 1
#define NOM_ERR 2
#define END_ERR 3
#define TIP_ERR 4
#define CEL_ERR 5
#define CNC_ERR 6
#define TEL_ERR 7
#define CNT_ERR 8
#define EML_ERR 9
#define CNE_ERR 10
#define OBS_ERR 11
#define PRC_ERR 12
#define VIN_ERR 13
#define PES_ERR 14
#define FOR_ERR 15
#define UND_ERR 16
#define GRP_ERR 17
#define MRC_ERR 18
#define CEP_ERR 19
#define PRD_ERR 20
#define CLI_ERR 21
#define QNT_ERR 22

char *critica_campos[] = {"ter_doc","ter_tipo_doc",
	"ter_endereco","ter_cep",
	"ter_tipo","ter_celular",
	"ter_contatoc","ter_telefone",
	"ter_contatot","ter_email",
	"ter_contatoe"};
char *desktop_images_vet[] = {THUMBDESKTOP1,THUMBDESKTOP2,THUMBDESKTOP3,THUMBDESKTOP4,THUMBDESKTOP5,THUMBDESKTOP6};

int configurar_parametros()
{
	int cont;
	char *query;
	query = malloc(MAX_QUERY_LEN);
	MYSQL_RES *res;
	MYSQL_ROW  row;
	for(cont=0;cont<=ter_critic_campos_qnt;cont++)
	{
		sprintf(query,"select critica from criticas where opcao_nome = 'terceiros' and campo_nome = '%s'",critica_campos[cont]);
		res = consultar(query);
		if(res!=NULL)
		{
			row = mysql_fetch_row(res);
			if(row!=NULL)
			{
				if(atoi(row[0])==1)
				{
					switch(cont)
					{
						case 0:
							terceiros.criticar.doc = 1;
							break;
						case 1:
							terceiros.criticar.tipodoc = 1;
							break;
						case 2:
							terceiros.criticar.endereco =1;
							break;
						case 3:
							terceiros.criticar.cep = 1;
							break;
						case 4:
							terceiros.criticar.tipo = 1;
							break;
						case 5:
							terceiros.criticar.celular =1;
							break;
						case 6:
							terceiros.criticar.contatoc =1 ;
							break;
						case 7:
							terceiros.criticar.telefone =1 ;
							break;
						case 8:
							terceiros.criticar.contatot =1;
							break;
						case 9:
							terceiros.criticar.email = 1;
							break;
						case 10:
							terceiros.criticar.contatoe = 1; 
							break;
					}
				}
			}
		}
	}
	return 0;
}
int ler_parametros()
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *query;
	int ter_critic_campos_qnt =10,cont=0;
	g_print("Lendo Parametros\n");
	query = malloc(sizeof(char*)*MAX_QUERY_LEN);
	for(cont=0;cont<=ter_critic_campos_qnt;cont++)
	{
		sprintf(query,"select critica from criticas where opcao_nome = 'terceiros' and campo_nome = '%s'",critica_campos[cont]);
		res = consultar(query);
		if(res!=NULL)
		{
			row = mysql_fetch_row(res);
			if(row!=NULL)
			{
				if(atoi(row[0])==1)
				{
					switch(cont)
					{
					case 0:
							terceiros.criticar.doc = 1;
							break;
						case 1:
							terceiros.criticar.tipodoc = 1;
							break;
						case 2:
							terceiros.criticar.endereco =1;
							break;
						case 3:
							terceiros.criticar.cep = 1;
							break;
						case 4:
							terceiros.criticar.tipo = 1;
							break;
						case 5:
							terceiros.criticar.celular =1;
							break;
						case 6:
							terceiros.criticar.contatoc =1 ;
							break;
						case 7:
							terceiros.criticar.telefone =1 ;
							break;
						case 8:
							terceiros.criticar.contatot =1;
							break;
						case 9:
							terceiros.criticar.email = 1;
							break;
						case 10:
							terceiros.criticar.contatoe = 1; 
							break;
					}
					gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(campos_de_critica[cont]),TRUE);
				}
			}
		}
		g_print("Parametros recebidos!\n");
	}
	return 0;
}

int atualizar_criticas()
{
	char *query;
	int cont=0;
	int erro;
	query = malloc(sizeof(char*)*MAX_QUERY_LEN);
	for(cont=0;cont<=ter_critic_campos_qnt;cont++)
	{		
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(campos_de_critica[cont]))==TRUE)
		{
			sprintf(query,"update criticas set critica = 1 where opcao_nome = 'terceiros' and campo_nome = '%s'",critica_campos[cont]);
			erro = enviar_query(query);
			if(erro != 0)
			{
				popup(NULL,"Erro ao tentar atualizar parametros");
				return 1;
			}
		}
		else
		{
			sprintf(query,"update criticas set critica = 0 where opcao_nome = 'terceiros' and campo_nome = '%s'",critica_campos[cont]);
			erro = enviar_query(query);
			if(erro != 0)
			{
				popup(NULL,"Erro ao tentar atualizar parametros");
				return 1;
			}
		}
	}
	if(configurar_parametros()==0)
		popup(NULL,"Criticas Parametrizadas");
	return 0;
}

int parametrizar()
{
	char *parametro_char;
	GtkWidget *janela_parametros;
	GtkWidget *notebook;
	GtkWidget *caixona,*caixa_superior;
	GtkWidget *geral_criticas_frame,*ter_criticas_frame,*prod_criticas_frame;
	GtkWidget *geral_criticas_box,*ter_criticas_box,*prod_criticas_box;
	
	char *wallpapers_nome[] = {"Grey","Cascate","Vulcon","Maré","Wallpaper 5","Wallpaper 6"};
	GtkWidget **caixa_wallpapers,**image_wallpapers,**label_wallpapers,**event_wallpapers,
	*wallpapers_box,*wallpapers_scroll,*wallpapers_frame;
	
	GtkWidget *geral_label,*terc_label,*prod_label,*orc_label;
	GtkWidget *geral_box,*terc_box,*prod_box,*orc_box;
	
	GtkWidget *opcoes_box;
	
	GtkWidget *atualizar_button;
	GtkWidget *atualizar_image;
	
	int cont;
	int *vet_pos;
	janela_parametros = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(janela_parametros,600,400);
	gtk_window_set_keep_above(GTK_WINDOW(janela_parametros),TRUE);
	gtk_window_set_position(GTK_WINDOW(janela_parametros),GTK_WIN_POS_CENTER_ALWAYS);
	notebook = gtk_notebook_new();
	vet_pos = malloc(sizeof(int*)*WALLPAPERS_QNT);
	caixa_wallpapers = malloc(sizeof(GtkBox*)*WALLPAPERS_QNT);
	image_wallpapers = malloc(sizeof(GtkImage*)*WALLPAPERS_QNT);
	label_wallpapers = malloc(sizeof(GtkLabel*)*WALLPAPERS_QNT);
	event_wallpapers = malloc(sizeof(GtkEventBox*)*WALLPAPERS_QNT);
	wallpapers_scroll = gtk_scrolled_window_new(NULL,NULL);
	wallpapers_box = gtk_box_new(0,0);
	for(cont=0;cont<WALLPAPERS_QNT;cont++)
	{
		/*
		 * wallpapers_nome  = char
		 * wallpapers_label = gtk_label
		 */
		image_wallpapers[cont] = gtk_image_new_from_file(desktop_images_vet[cont]);
		label_wallpapers[cont] = gtk_label_new(wallpapers_nome[cont]);
		caixa_wallpapers[cont] = gtk_box_new(1,0);
		event_wallpapers[cont] = gtk_event_box_new();
		gtk_box_pack_start(GTK_BOX(caixa_wallpapers[cont]),image_wallpapers[cont],0,0,0);
		gtk_box_pack_start(GTK_BOX(caixa_wallpapers[cont]),label_wallpapers[cont],0,0,0);
		gtk_container_add(GTK_CONTAINER(event_wallpapers[cont]),caixa_wallpapers[cont]);
		gtk_box_pack_start(GTK_BOX(wallpapers_box),event_wallpapers[cont],0,0,20);
		vet_pos[cont] = cont;
		//gtk_widget_add_events(event_wallpapers[cont],GDK_ALL_EVENTS_MASK);
		#pragma GCC diagnostic ignored "-Wint-conversion"
		g_signal_connect(event_wallpapers[cont],"button-press-event",G_CALLBACK(trocar_desktop),vet_pos[cont]);
		#pragma GCC diagnostic warning "-Wint-conversion"
	}
	gtk_widget_set_size_request(wallpapers_box,600,100);
	gtk_widget_set_size_request(wallpapers_scroll,600,100);
	#ifdef WIN32
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(wallpapers_scroll),wallpapers_box);
	#endif
	#ifdef __linux__
	gtk_container_add(GTK_CONTAINER(wallpapers_scroll),wallpapers_box);
	#endif

	wallpapers_frame = gtk_frame_new("Imagens da área de Trabalho");
	gtk_container_add(GTK_CONTAINER(wallpapers_frame),wallpapers_scroll);
	campos_de_critica = malloc(sizeof(GtkCheckButton*)*11);
	parametro_char = malloc(MAX_PARAM_LEN);
	geral_label = gtk_label_new("GERAL");
	terc_label = gtk_label_new("TERCEIROS");
	prod_label = gtk_label_new("PRODUTOS");
	
	caixona = gtk_box_new(1,0);
	opcoes_box = gtk_box_new(0,0);
	geral_box = gtk_box_new(1,0);
	terc_box = gtk_box_new(1,0);
	prod_box = gtk_box_new(1,0);
	
	gtk_box_pack_start(GTK_BOX(geral_box),wallpapers_frame,0,0,10);
	
	atualizar_button = gtk_button_new_with_label("Atualizar");
	atualizar_image = gtk_image_new_from_file(IMG_REC);
	gtk_button_set_image(GTK_BUTTON(atualizar_button),atualizar_image);
	gtk_box_pack_start(GTK_BOX(opcoes_box),atualizar_button,0,0,30);
	
	ter_criticas_frame = gtk_frame_new("Campos Criticados");
	ter_criticas_box = gtk_box_new(1,0);
	
	campos_de_critica[0] = gtk_check_button_new_with_label("Documento (RG/CNPJ)");
	gtk_box_pack_start(GTK_BOX(ter_criticas_box),campos_de_critica[0],0,0,0);
	
	campos_de_critica[1] = gtk_check_button_new_with_label("Tipo Documento");
	gtk_box_pack_start(GTK_BOX(ter_criticas_box),campos_de_critica[1],0,0,0);
	
	campos_de_critica[2] = gtk_check_button_new_with_label("Endereço");
	gtk_box_pack_start(GTK_BOX(ter_criticas_box),campos_de_critica[2],0,0,0);
	
	campos_de_critica[3] = gtk_check_button_new_with_label("CEP");
	gtk_box_pack_start(GTK_BOX(ter_criticas_box),campos_de_critica[3],0,0,0);
	
	campos_de_critica[4] = gtk_check_button_new_with_label("Tipo (Cliente/Fornecedor)");
	gtk_box_pack_start(GTK_BOX(ter_criticas_box),campos_de_critica[4],0,0,0);
	
	campos_de_critica[5] = gtk_check_button_new_with_label("Celular");
	gtk_box_pack_start(GTK_BOX(ter_criticas_box),campos_de_critica[5],0,0,0);
	
	campos_de_critica[6] = gtk_check_button_new_with_label("Contato Celular");
	gtk_box_pack_start(GTK_BOX(ter_criticas_box),campos_de_critica[6],0,0,0);
	
	campos_de_critica[7] = gtk_check_button_new_with_label("Telefone");
	gtk_box_pack_start(GTK_BOX(ter_criticas_box),campos_de_critica[7],0,0,0);
	
	campos_de_critica[8] = gtk_check_button_new_with_label("Contato Telefone");
	gtk_box_pack_start(GTK_BOX(ter_criticas_box),campos_de_critica[8],0,0,0);
	
	campos_de_critica[9] = gtk_check_button_new_with_label("Email");
	gtk_box_pack_start(GTK_BOX(ter_criticas_box),campos_de_critica[9],0,0,0);
	
	campos_de_critica[10] = gtk_check_button_new_with_label("Contato Email");
	gtk_box_pack_start(GTK_BOX(ter_criticas_box),campos_de_critica[10],0,0,0);
	
	gtk_container_add(GTK_CONTAINER(ter_criticas_frame),ter_criticas_box);
	gtk_box_pack_start(GTK_BOX(terc_box),ter_criticas_frame,0,0,0);
	
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),geral_box,geral_label);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),terc_box,terc_label);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook),prod_box,prod_label);
	
	ler_parametros();
	
	gtk_box_pack_start(GTK_BOX(caixona),notebook,0,0,0);
	gtk_box_pack_start(GTK_BOX(caixona),opcoes_box,0,0,20);
	gtk_container_add(GTK_CONTAINER(janela_parametros),caixona);
	
	g_signal_connect(atualizar_button,"clicked",G_CALLBACK(atualizar_criticas),NULL);
	gtk_widget_show_all(janela_parametros);
	return 0;
}
