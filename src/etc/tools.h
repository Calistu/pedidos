#define RANDOM_STRING_SIZE 10
static int logging = 0;
static int erro_logger=0;

GtkWidget *print_janela;

GtkWidget *msg_abrir_orc_window;

static MYSQL conectar;
static int primeira_conexao=0;

static char **tipo_ter_list;

void passar_campo(GtkWidget *widget,gpointer widget2);

int is_texto(char *texto);

char *text_to_html(char *texto);

char *formatar_data(char *data);

int file_logger(char *string);

void receber_psq_code_space(GtkTreeView *treeview, GtkTreePath *path,  GtkTreeViewColumn *column, GtkWidget *window);

void auto_vmover_scroll(GtkWidget *widget, GdkRectangle *null, GtkWidget *scroll_window);

void auto_hmover_scroll(GtkWidget *widget, GdkRectangle *null, GtkWidget *scroll_window);

void button_mover_scroll(GtkButton *button, GtkWidget *scroll_window);

char *get_elem_from_path(char *path);

void get_filename_to_entry(GtkFileChooserButton *fchooser, GtkEntry *entry);

void encerrando();

int close_window_callback(GtkWidget *widget,gpointer *ponteiro);

char *randomizar_string();

int autologger(char *string);

MYSQL_RES *consultar(char *query);

int enviar_query(char *query);

int tasker(char *table);

char *infos(int pos);

int configurar_parametros();


char *tipo_ter_num_to_str(int num);

GtkWidget *popup_fechar;

char *ped_status_from_int(int code);

char *status_tit_str(int status);

int aviso_de_baixa_fin();

int cep_nao_existente_fun(gchar *cep);
