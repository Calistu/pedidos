struct _sessao * get_new_sessao_from_oper(int oper_code) {
    struct _sessao * sessao = malloc(sizeof(struct _sessao));

    sessao -> operador = get_operador(oper_code);
    if (!sessao -> operador)
        return NULL;

    sessao -> ult_ativ = g_date_time_new_now_local();
    sessao -> criacao = g_date_time_new_now_local();
    sessao -> expiracao = g_date_time_add(sessao -> ult_ativ, G_TIME_SPAN_MINUTE * SESSAO_EXP_MIN);
    sessao -> status_sessao = SESSAO_LOGADA;

    return sessao;
}

int limpar_sessao() {

    sessao_oper = criar_sessao_anon();
    sessao_set_nonemodules();

    return 0;
}

int fechar_sessao() {
    char query[MAX_QUERY_LEN];
    sprintf(query, "insert into wnd_logger(id_janela,nome_janela,estado,qnt_aberta,operador,tempo) values(%i,'%s',%i,%i,%i,NOW())",
        REG_CORRECT_FINAL,
        "Fazendo Logoff...",
        0, 0,
        sessao_oper -> operador -> code);
    if (mysql_query( & conectar, query)) {
        file_logger("Não foi possivel salvar status da sessão\n");
        file_logger(query);
        file_logger((char * ) mysql_error( & conectar));
    }

    limpar_sessao();
    limpar_applicacao();

    //variavel de encerramento ocorrida pelo proprio sistema (logoff)
    janelas_gerenciadas.principal.sys_close_wnd = 1;

    if (janelas_gerenciadas.principal.janela_pointer)
        gtk_widget_destroy(janelas_gerenciadas.principal.janela_pointer);

    if (init())
        return 1;

    janelas_gerenciadas.principal.sys_close_wnd = 0;

    return 0;
}

struct _sessao * criar_sessao_anon() {

    struct _sessao * sessao_oper = malloc(sizeof(struct _sessao));
    sessao_oper = malloc(sizeof(struct _sessao));
    sessao_oper -> operador = malloc(sizeof(struct _operador));

    ativar.ativo = 0;
    sessao_oper -> operador -> code = default_user_code;
    sessao_oper -> operador -> nome = strdup(login_bindings[LOGIN_BIND_ENTRAR]);
    sessao_oper -> operador -> senha = strdup("");
    sessao_oper -> operador -> nivel = 1;

    sessao_oper -> status_sessao = SESSAO_TESTE;
    sessao_oper -> criacao = g_date_time_new_now_local();
    sessao_oper -> ult_ativ = g_date_time_new_now_local();
    sessao_oper -> expiracao = g_date_time_add(sessao_oper -> ult_ativ, G_TIME_SPAN_MINUTE * S_ANON_EXP_MIN);
    sessao_set_nonemodules();

    return sessao_oper;
}
struct _sessao * criar_sessao_default() {

    struct _sessao * sessao_oper = malloc(sizeof(struct _sessao));
    sessao_oper -> operador = malloc(sizeof(struct _operador));

    sessao_oper -> operador -> code = default_user_code;
    sessao_oper -> operador -> nome = strdup("Default");
    sessao_oper -> operador -> nivel = NIVEL_GERENCIAL;

    sessao_oper -> status_sessao = SESSAO_LOGADA;
    sessao_oper -> criacao = g_date_time_new_now_local();
    sessao_oper -> ult_ativ = g_date_time_new_now_local();
    sessao_oper -> expiracao = g_date_time_add(sessao_oper -> ult_ativ, G_TIME_SPAN_MINUTE * SESSAO_EXP_MIN);
    return sessao_oper;

}

void sessao_set_allmodules() {
    ativar.cadastro = 1;
    ativar.compras = 1;
    ativar.faturamento = 1;
    ativar.estoque = 1;
    ativar.financeiro = 1;
    ativar.marketing = 1;
    ativar.relatorios = 1;
    ativar.tecnicos = 1;
    return;
}

void sessao_set_nonemodules() {
    ativar.cadastro = 0;
    ativar.compras = 0;
    ativar.faturamento = 0;
    ativar.estoque = 0;
    ativar.financeiro = 0;
    ativar.marketing = 0;
    ativar.relatorios = 0;
    ativar.tecnicos = 0;
    return;
}

int validar_sessao_criada() {
    if (!aplicacao_inicializada())
        return 0;

    if (janelas_gerenciadas.principal.sys_close_wnd)
        return 0;

    if (sessao_oper -> status_sessao == SESSAO_NULA) {
        popup(get_pop_parents_wnd(), "Sessão com erro, o incidente será reportado");
        autologger("!!!!!!!!!!!!!!!\nSistema utilizado sem uma sessao ativa\n!!!!!!!!!!!!!!!");
        encerrando();
        return 1;
    }

    if (!param_funcionalidades.expirar_sessao) {
        return 0;
    }
    
    GDateTime * atual = g_date_time_new_now_local();
    if (!atual) {
        file_logger("Erro ao consultar datetime atual em validar_sessao_criada()-> g_date_time_new_now_local()");
        return 1;
    }

    if (!sessao_oper -> ult_ativ) {
        file_logger("Erro ao consultar datetime de ultima atividade em validar_sessao_criada() -> ult_ativ -> struct sessao");
        return 1;
    }

    if (g_date_time_difference(atual, sessao_oper -> ult_ativ) > G_TIME_SPAN_MINUTE * SESSAO_MAX_INATIVIDADE) {
        gchar msg[100];
        sprintf(msg, "data atual = %s\n", g_date_time_format(atual, "%F %T"));
        file_logger(msg);
        sprintf(msg, "data da ultima atividade = %s\n", g_date_time_format(sessao_oper -> ult_ativ, "%F %T"));
        fechar_sessao();
        popup(janelas_gerenciadas.login.janela_pointer, "Sessão reiniciada por ausência de atividade!");
        autologger("Sessão reiniciada por ausência de atividade");
        return 1;
    }

    if (g_date_time_compare(atual, sessao_oper -> expiracao) >= 0) {
        gchar msg[100];
        sprintf(msg, "data atual = %s\n", g_date_time_format(atual, "%F %T"));
        file_logger(msg);
        sprintf(msg, "data expiracao = %s\n", g_date_time_format(sessao_oper -> expiracao, "%F %T"));
        file_logger(msg);
        fechar_sessao();
        popup(janelas_gerenciadas.login.janela_pointer, "Sessão expirada!");
        autologger("Sessão expirada");
        return 1;
    }

    return 0;
}