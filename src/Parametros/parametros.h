//combo_box de terceiros
static gchar **nomes_temas;
static int temas_qnt=0;
GtkWidget **campos_de_critica;

GtkWidget *tecn_param_nav_path1_entry, *tecn_param_nav_path2_entry, *tecn_param_nav_choose1_radio, *tecn_param_nav_choose2_radio;
GtkWidget *tecn_param_imp_path1_entry, *tecn_param_imp_path2_entry, *tecn_param_imp_path3_entry;

//quantidade de flags/aba
const int orc_critic_campos_qnt=3;

int parametrizar();

int temas();
static int orc_prod_saldo_limite=0;

static GtkWidget *janela_init,
*janela_keep_above,
*tema_combo_box,
*est_orc_padrao;


#define CAMPOS_QNT 25
int vet_erro[CAMPOS_QNT+1];

#define COD_ERR 0
#define DOC_ERR 1
#define INSC_ERR 2
#define NOM_ERR 3
#define END_ERR 4
#define UF_ERR 5
#define CID_ERR 6
#define TIP_ERR 7
#define CEL_ERR 8
#define CNC_ERR 9
#define TEL_ERR 10
#define CNT_ERR 11
#define EML_ERR 12
#define CNE_ERR 13
#define OBS_ERR 14
#define PRC_ERR 15
#define VIN_ERR 16
#define PES_ERR 17
#define FOR_ERR 18
#define UND_ERR 19
#define GRP_ERR 20
#define MRC_ERR 21
#define CEP_ERR 22
#define PRD_ERR 23
#define CLI_ERR 24
#define PRAZ_ERR 25
#define QNT_ERR 26

static struct {
	int navegador_pdr;
	gchar navegador_path1[MAX_PATH_LEN];
	gchar navegador_path2[MAX_PATH_LEN];
}navegadores;

static struct {
	gchar imp_path1[MAX_PATH_LEN];
	gchar imp_path2[MAX_PATH_LEN];
	gchar imp_path3[MAX_PATH_LEN];
}impressoras;

static struct {
		int est_orc_padrao;
}orc_params;