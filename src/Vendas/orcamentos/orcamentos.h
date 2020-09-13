#define ORC_COD_COL 0
#define ORC_TIPMOV_COL 1
#define ORC_VENDD_COL 2
#define ORC_CLI__COL 3
#define ORC_DATE_COL 4
#define ORC_PAGCOND_COL 5
#define ORC_BANC_COL 6
#define ORC_TOTAL_COL 7
#define ORC_OBS__COL 8

#define ORC_PROD_COD_COL 0
#define ORC_PROD_ITM_COL 1
#define ORC_PROD_PROD_COL 2
#define ORC_PROD_SUBGRP_COL 3
#define ORC_PROD_UND_COL 4
#define ORC_PROD_VLR_COL 5
#define ORC_PROD_VLR_ORIG_COL 6
#define ORC_PROD_TIP_DESC_COL 7
#define ORC_PROD_DESC_COL 8
#define ORC_PROD_TOTAL_COL 9
#define ORC_PROD_OBS_COL 10

static struct itens_struct
{
	int id;
	int item;
	int produto;
	char produto_nome[MAX_NAME_LEN];
	int subgrupo;
	char subgrupo_nome[MAX_NAME_LEN];
	float qnt_f;
	float preco_f;
	float desconto_f;
	int tipodesc;
	float total_f;
	char grupos_nome[MAX_SUBGRUPO*MAX_GRP_LEN+MAX_SUBGRUPO];
	char qnt_c[MAX_PRECO_LEN];
	char preco_c[MAX_PRECO_LEN];
	char desconto_c[MAX_PRECO_LEN];
	char total_c[MAX_PRECO_LEN];
	char origem_preco[15];
}ativos[MAX_PROD_ORC+1],excluidos[MAX_PROD_ORC+1];

static struct _orc_valores{
	float valor_prds;
	float valor_prds_desc;
	float valor_prds_liquido;

	float valor_frete;
	float desconto_frete;
	float valor_frete_liquido;

	float desconto_total;
	float valor_total;
}orc_valores;

static struct _orc_infos{
	int code;
	int cliente_code;
	int vendedor;
	int tipo_mov;
	char data[MAX_DATE_LEN];
	float total;
	char observacoes[MAX_OBS_LEN];
}orc_infos;

static struct _orc_parcelas{
	int pagcond_code;
	int parcelas_qnt;
	int banco;
	char *parcelas_data[MAX_PARCELAS_QNT+1];
	float parcelas_vlr[MAX_PARCELAS_QNT+1];
	float valor_faltante;
	float total_geral;
}orc_parcelas;

struct _orc{
	struct _orc_infos infos;
	struct _orc_valores valores;
	struct _orc_parcelas parcelas;
};

#define PROD_LINHAS_ORC 1
#define DATE_QUERY "select DATE_FORMAT(dia,\"%d/%m/%Y\") from orcamentos where code = "

GtkWidget *orc_data_vlr_lists_box, **orc_data_lists_entry, **orc_vlr_lists_entry, *orc_data_vlr_lists_grid;

GtkWidget *orc_prods_grid;

static float saldo_limite = 3;

static int tirar_linha(int);
static int vnd_orc();

static int recebendo_prod_orc=0;
static int alterando_orc=0;
static int concluindo_orc=0;
static int copiando_orc=0;
static int excluindo_orc=0;

static int aviso_estoque[MAX_PROD_ORC+1];
static int orc_pag_cond_activated=0;

static int movendo_scroll=0;
static int adicionando_linha_ignore=0;

static int pag_cond=0, tipo_pag=0;

static int rec_altera_qnt=1;
static GtkWidget *orc_notebook;

static int preco_prod_orc(GtkWidget *widget,int posicao);
static int preco_alterado[MAX_PROD+1], valor_orig[MAX_PROD+1];
static int pressionado=0;
static int ativos_qnt=1;

static float total_geral_orc =0 ;
static float desconto_geral_orc =0 ;

//COMBO
GtkWidget **tipodesconto_prod_orc_combo;
GtkWidget *orc_pag_cond_entry, *orc_pag_cond_nome, *orc_pag_cond_frame, *orc_pag_cond_fixed, *orc_pag_cond_box, *orc_pag_cond_psq_button;
gchar *orc_pag_cond_gchar;
//  BUTTONS

GtkWidget *pesquisa_orc,*pesquisa_ter,**pesquisa_prod;

GtkWidget *concluir_orc_button,*gerar_orc_button,*pedido_orc_button,*alterar_orc_button,*cancelar_orc_button,*excluir_orc_button, *copia_orc_button;
GtkWidget *concluir_orc_img_button,*gerar_orc_img_button,*pedido_orc_img_button,*alterar_orc_img_button,*cancelar_orc_img_button,*excluir_orc_img_button;
GtkWidget *caixa_opcoes_orc;
//  GCHAR
static gchar *orc_bnc_code_gchar;
static gchar *data_orc_gchar;

static gchar *cliente_orc_gchar;

static gchar *codigo_orc_gchar,
//*operacao_orc_gchar,
*observacoes_orc_gchar;

static gchar *codigo_prod_orc_gchar,
//*descricao_prod_orc_gchar,
*subgrp_prod_orc_cod_gchar,
*qnt_prod_orc_gchar,
*preco_prod_orc_gchar,
*orig_preco_prod_orc_gchar,
*desconto_prod_orc_gchar,
*total_prod_orc_gchar;

static int gerar_total_geral();
static char *item_frame_char;
static GtkWidget *janela_orcamento;

//  FRAMES/BOX
GtkWidget **codigo_prod_orc_frame;
GtkWidget **desconto_prod_orc_frame,**desconto_prod_orc_box;
GtkWidget **preco_prod_orc_frame,**preco_prod_orc_box;
GtkWidget **qnt_prod_orc_frame,**qnt_prod_orc_box;
GtkWidget **subgrp_prod_orc_frame,**subgrp_prod_orc_box;
GtkWidget **total_prod_orc_frame,**total_prod_orc_box, **total_prod_orc_fixed;


GtkWidget *data_orc_frame;
GtkWidget *codigo_orc_frame,*operacao_orc_frame,*cliente_orc_frame,*observacoes_orc_frame,**linhas_prod_orc_frame;
GtkWidget *total_geral_orc_frame,*desconto_geral_orc_frame,*frete_orc_frame;

// FIXEDs
GtkWidget *orc_infos_fixed,*orc_itens_fixed,*orc_geral_fixed;
GtkWidget *opcoes_orc_fixed;

GtkWidget **obs_prod_orc_scroll,**obs_prod_orc_fixed;

// LABELS

//static GtkWidget *data_orc_label;
GtkWidget *total_geral_orc_label,*desconto_geral_orc_label, *frete_orc_label;

GtkWidget *codigo_orc_label,*operacao_orc_label,*cliente_orc_label;
GtkWidget **codigo_prod_orc_label,
**descricao_prod_orc_label,
**qnt_prod_orc_label,
**preco_prod_orc_label,
**orig_preco_prod_orc_combo,
**desconto_prod_orc_label;

//  ENTRYs

GtkWidget *codigo_orc_entry,*operacao_orc_combo,*cliente_orc_entry,*cliente_orc_name_entry,*cliente_orc_end_entry,*cliente_orc_tel_entry;

GtkWidget **codigo_orc_prod_box;
GtkWidget *orc_bnc_code_entry, *orc_bnc_nome_entry, *orc_bnc_psq_button;

GtkWidget **codigo_prod_orc_entry,
**descricao_prod_orc_entry,
**subgrp_prod_orc_entry, **subgrp_prod_orc_cod_entry,
**subgrp_prod_orc_button,
**qnt_prod_orc_entry,
**preco_prod_orc_entry,
**desconto_prod_orc_entry,
**total_prod_orc_entry;

GtkWidget *data_orc_entry;

//  BOXES
GtkWidget *caixa_orc_infos_c;/*,*caixa_orc_infos_b;*/
GtkWidget *codigo_orc_box,*operacao_orc_box,*cliente_orc_box,*itens_orc_box,*transp_orc_box,*outros_orc_box,
**linhas_prod_orc_box,
**linhas2_prod_orc_box,
**juncao_linha_prod_orc_box ;
GtkWidget *caixa_grande,*caixa_orc_infos_d,*caixa_orc_infos_e,*caixa_orc_infos;

//  CONTAINERS
GtkWidget *prod_scroll_window,*prod_scroll_box;
GtkWidget *img_botao_orc_mais,**img_botao_menos;

static int cont;
static int adicionar_linha_orc();
GtkWidget *observacoes_orc,*buffer_orc;

//Botoes
GtkWidget *botao_orc_mais,**botao_menos;

static int produto_inserido[MAX_PROD_ORC+1];
static int itens_qnt=1;
int id_vetor[MAX_PROD_ORC+1];

static int sinal_operacao_int=0;
static int operacao_orc_int=0;

int orc_transp_msg_cep=0;
int transp_verified=0;
GtkTextBuffer  **obs_prod_orc_buffer;
GtkWidget **obs_prod_orc_view, **obs_prod_orc_frame;
static gchar **obs_prod_orc_gchar;
