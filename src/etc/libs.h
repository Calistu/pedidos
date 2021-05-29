#ifndef PEDIDOS_INCLUDES_HEADER
#define PEDIDOS_INCLUDES_HEADER

// comentar na compilação da versão oficial !!
#include "debug_version.h"

#include "./basic_headers.h"
#include "./Pedidos.h"
#include "./init.h"
#include "./version.h"
#include "./desktop.h"
#include "./defines.h"
#include "./receber_confs.h"
#include "./xml_tools.h"
#include "./sql_tools.h"
#include "./tools.h"
#include "./logger.h"
#include "./popups.h"
#include "./maquinas.h"
#include "./temas.h"
#include "./impressao.h"
#include "./vars.h"

#include "./stoi.h"
#include "./css.h"
#include "./http.h"
#include "./criticas.h"
#include "./barra_icones.h"
#include "./monitor_size.h"
#include "./analisa_campos.h"
#include "./mp_popup.h"
#include "./debugger.h"
#include "./sobre.h"
#include "./sessao.h"
#include "./operadores.h"
#include "./fechar.h"
#include "./pega_data.h"

#include "../Menu/iconview.h"
#include "../Menu/abas.h"

#include "../Canones/cad_oper/Cadastro_Operadores.h"
#include "../Canones/gerenciador_janelas/gerenciador_janelas.h"
#include "../Canones/cad_ceps/cadastro_ceps.h"

#include "../Parametros/parametros.h"

#include "../Terceiros/cadastro_terceiros.h"

#include "../Produtos/cadastro_produtos.h"
#include "../Produtos/precos/precos.h"

#include "../Produtos/grupos/cad_grupo.h"

#include "../pesquisadores/pesquisadores.h"
#include "../pesquisadores/grupos.h"
#include "../pesquisadores/datas.h"
#include "../pesquisadores/subgrupos.h"
#include "../pesquisadores/subgrupos_todos.h"
#include "../pesquisadores/terceiros.h"
#include "../pesquisadores/unidades.h"
#include "../pesquisadores/estoques.h"
#include "../pesquisadores/produtos.h"
#include "../pesquisadores/ncms.h"
#include "../pesquisadores/orcamentos.h"
#include "../pesquisadores/pedidos.h"
#include "../pesquisadores/ceps.h"
#include "../pesquisadores/movimentos_ent_estoque.h"
#include "../pesquisadores/movimentos_said_estoque.h"
#include "../pesquisadores/relatorios.h"
#include "../pesquisadores/cond_pag.h"
#include "../pesquisadores/operadores.h"
#include "../pesquisadores/requisicoes.h"
#include "../pesquisadores/ord_cmp.h"
#include "../pesquisadores/fin_rec.h"
#include "../pesquisadores/fin_pag.h"
#include "../pesquisadores/cidades.h"
#include "../pesquisadores/bancos.h"
#include "../pesquisadores/cotacoes.h"
#include "../pesquisadores/contratos.h"
#include "../pesquisadores/serv_transporte.h"
#include "../pesquisadores/mail_distribuicoes.h"
#include "../pesquisadores/precos.h"
#include "../pesquisadores/modelos_email.h"
#include "../pesquisadores/cheques.h"


#include "../Compras/cadastro_compras.h"
#include "../Compras/entradas/entradas.h"
#include "../Compras/ordens/ordem.h"
#include "../Compras/cotacoes/cotacao.h"
#include "../Compras/solicitacoes/solicitacao.h"

#include "../Unidades/cadastro_unidades.h"

#include "../Estoque/saldos/saldos.h"
#include "../Estoque/entradas/entradas.h"
#include "../Estoque/saidas/saidas.h"
#include "../Estoque/cadastro/estoque.h"

#include "../Vendas/FormaPagamento/forma_pagamento.h"

#include "../Vendas/condicoes_pag/cad_pag.h"
#include "../Vendas/condicoes_pag/cancelar.h"
#include "../Vendas/condicoes_pag/concluir.h"
#include "../Vendas/condicoes_pag/alterar.h"
#include "../Vendas/condicoes_pag/excluir.h"

#include "../Vendas/pedidos/pedidos.h"
#include "../Vendas/pedidos/emitir.h"
#include "../Vendas/pedidos/envia_email.h"
#include "../Vendas/pedidos/cancelar.h"
#include "../Vendas/pedidos/excluir.h"
#include "../Vendas/pedidos/gerar.h"
#include "../Vendas/pedidos/status.h"

#include "../Vendas/orcamentos/orcamentos.h"
#include "../Vendas/orcamentos/print_operation.h"
#include "../Vendas/orcamentos/gerar_totais.h"
#include "../Vendas/orcamentos/janela/adicionar_linha.h"
#include "../Vendas/orcamentos/janela/tirar_linha.h"
#include "../Vendas/orcamentos/campos/entrega_campos.h"

#include "../Emissor/CFe/rec_infos_cupom.h"
#include "../Emissor/CFe/cfe.h"
#include "../Emissor/CFe/bemasat.h"

#include "../Emissor/ConsultaContrib/consulta_contrib.h"

#include "../Vendas/pdv/pdv_princ_wnd.h"
#include "../Vendas/pdv/caixa.h"

#include "./GerarDocs/Orcs2/header.h"
#include "./GerarDocs/Orcs2/cliente.h"
#include "./GerarDocs/Orcs2/itens.h"
#include "./GerarDocs/Orcs2/transporte.h"
#include "./GerarDocs/Orcs2/pagamentos.h"
#include "./GerarDocs/Orcs2/observacao.h"
#include "./GerarDocs/Orcs2/gerar.h"

#include "./GerarDocs/OrdensCompra/gerar.h"

//#include "../Transportes0/cad_trsp.h"
#include "../Transportes/cad_trsp.h"

#include "../Financeiro/geral.h"
#include "../Financeiro/titulos/funcoes.h"
#include "../Financeiro/titulos/get_widget.h"
#include "../Financeiro/titulos/update_widget.h"
#include "../Financeiro/bancos/cad_bancos.h"
#include "../Financeiro/cheques/cheque.h"
#include "../Financeiro/titulos/receber/cadastro_titulos.h"
#include "../Financeiro/titulos/pagar/cadastro_titulos.h"
#include "../Financeiro/baixas/receber/cadastro_baixas.h"
#include "../Financeiro/baixas/pagar/cadastro_baixas.h"
#include "../Financeiro/titulos/pagar/contas_fixas/contas_fixas.h"
#include "../Financeiro/titulos/etc/importacao/importador.h"
#include "../Financeiro/titulos/etc/importacao/iniciar.h"

#include "../Relatorios/relatorios.h"
#include "../Relatorios/relatorios/produtos/produtos.h"
#include "../Relatorios/relatorios/terceiros/terceiros.h"
#include "../Relatorios/relatorios/movimentos/movimentos.h"
#include "../Relatorios/relatorios/orc_prod/orc_prods.h"

#include "../Relatorios/pesquisar/relat_ptabs.h"

#include "../Relatorios/fixados/iconview.h"
#include "../Relatorios/fixados/produtos/principal.h"
#include "../Relatorios/fixados/vendas/principal.h"
#include "../Relatorios/fixados/estoques/principal.h"
#include "../Relatorios/fixados/financeiro/receber/principal.h"
#include "../Relatorios/fixados/financeiro/pagar/principal.h"

#include "../Marketing/analises/analisar.h"
#include "../Marketing/emails/enviar/envio_emails.h"
#include "../Marketing/emails/modelar/modela_email.h"
#include "../Marketing/distribuicao/cad_distribuicao.h"

#include "../Suporte/suporte.h"
#include "../Suporte/adicionar.h"
#include "../Suporte/atualiza.h"
#include "../Suporte/email.h"

#include "../Integracoes/woocommerce/produtos.h"
#include "../Integracoes/woocommerce/clientes.h"

#include "../Tecnico/cad_empresa/cad_empresa.h"
#include "../Tecnico/debugger/logs.h"
#include "../Tecnico/contratos/contratos.h"

#include "../Estoque/cadastro/conclui.h"
#include "../Estoque/cadastro/cancela.h"
#include "../Estoque/cadastro/altera.h"
#include "../Estoque/cadastro/exclui.h"

#include "../Gestao/enviar_email.h"
#include "../Gestao/Notificacao/notificacao.h"

#include "../Kanban/kanban.h"

#endif
