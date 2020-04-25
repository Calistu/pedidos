MYSQL_ROW campos;
GtkWidget *filas;
#include "campos/vars/campos_ter.h"
#include "campos/vars/vinculo_precos.h"
#include "campos/vars/listagem_ter.h"
#include "campos/codigo.c"
#include "campos/cnpj.c"
#include "campos/cpf.c"
#include "campos/escolher_doc.c"
#include "campos/nome.c"
#include "campos/cep.c"
#include "campos/endereco.c"
#include "campos/tipo.c"
#include "campos/contatos.c"
#include "campos/observacoes.c"
#include "campos/email.c"
#include "campos/precos.c"
#include "campos/prazo.c"
#include "campos/numrua.c"
#include "campos/cidade.c"

#define CAD_QUERY_TER "insert into terceiros(razao,doc,tipo_doc,tipoc,tipo,prazo,cep,endereco,numrua,tiporua,celular,contatoc,telefone,contatot,email,contatoe,obs) values('%s',  '%s',  %i,  '%s',  %i, '%s', '%s',  '%s',  %s, %i, '%s',  '%s',  '%s', '%s',  '%s',  '%s' , '%s');"

#define CAD_QUERY_TER_VAR nomes_ter, doc_ter, tipo_doc, tipo_ter, terci_tipo, prazo_ter, cep_ter, endereco_ter, address_num, tipo_log, celular_ter, contatoc_ter, telefone_ter, contatot_ter, email_ter, contatoe_ter, observacoes_ter

#define UPDATE_QUERY_TER "update terceiros set razao = '%s', doc = '%s', tipo_doc = %i, tipoc = '%s', tipo = %i, prazo = '%s', cep = '%s', endereco = '%s', numrua = %s,tiporua = %i, telefone = '%s',contatot = '%s',celular = '%s',contatoc = '%s', email = '%s',obs = '%s' where code = %s;"
#define UPDATE_QUERY_TER_VAR nomes_ter, doc_ter, tipo_doc, tipo_ter, terci_tipo, prazo_ter, cep_ter,endereco_ter, address_num, tipo_log,telefone_ter,contatot_ter,celular_ter,contatoc_ter,email_ter,observacoes_ter,codigos_ter
