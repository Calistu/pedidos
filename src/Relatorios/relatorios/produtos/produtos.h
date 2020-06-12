struct 
{
	
	char campos[MAX_RELAT_CAMPOS*MAX_QUERY_ELM_LEN+MAX_QUERY_ELM_LEN];
	char campo_query[MAX_RELAT_CAMPOS][MAX_QUERY_ELM_LEN];
	int campos_qnt;
}prod_query;
static GtkWidget *relat_prod_code_entry,
*relat_prod_nome_entry,
*relat_prod_query_entry;

static GtkWidget *psq_relat_prod_code;

static gchar *relat_prod_codigo_gchar, *relat_prod_query_gchar;

int relat_prod_codigo_fun();
int relat_prod_query_fun();
int relat_prod_gerar_fun();

#include "campos/codigo.c"
#include "campos/query.c"
#include "gerar.c"