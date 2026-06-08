#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    int ticket;
    char descricao[100];
} Chamado;

typedef struct {
    Chamado dados[MAX];
    int inicio;
    int fim;
} Fila;

typedef struct {
    char acao[100];
} Log;

typedef struct {
    Log dados[MAX];
    int topo;
} Pilha;

typedef struct Equipamento {
    int patrimonio;
    char tipo[50];
    char localizacao[50];
    struct Equipamento *prox;
} Equipamento;

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
}

int filaVazia(Fila *f) {
    return f->inicio == f->fim;
}

void abrirChamado(Fila *f, int ticket, char descricao[]) {
    if (f->fim == MAX) {
        printf("Fila de chamados cheia.\n");
        return;
    }

    f->dados[f->fim].ticket = ticket;
    strcpy(f->dados[f->fim].descricao, descricao);
    f->fim++;
}

void atenderProximoChamado(Fila *f) {
    if (filaVazia(f)) {
        printf("Nenhum chamado pendente.\n");
        return;
    }

    printf("Atendendo Ticket %d - %s\n",
           f->dados[f->inicio].ticket,
           f->dados[f->inicio].descricao);

    f->inicio++;
}

void listarChamados(Fila *f) {
    if (filaVazia(f)) {
        printf("Nenhum chamado pendente.\n");
        return;
    }

    printf("\nChamados pendentes:\n");
    for (int i = f->inicio; i < f->fim; i++) {
        printf("Ticket %d - %s\n",
               f->dados[i].ticket,
               f->dados[i].descricao);
    }
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void registrarAcao(Pilha *p, char acao[]) {
    if (p->topo == MAX - 1) {
        printf("Histórico de logs cheio.\n");
        return;
    }

    p->topo++;
    strcpy(p->dados[p->topo].acao, acao);
}

void desfazerUltimaAcao(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Nenhuma ação para desfazer.\n");
        return;
    }

    printf("Ação desfeita: %s\n", p->dados[p->topo].acao);
    p->topo--;
}

void mostrarHistorico(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Histórico vazio.\n");
        return;
    }

    printf("\nHistórico de ações:\n");
    for (int i = p->topo; i >= 0; i--) {
        printf("%s\n", p->dados[i].acao);
    }
}

Equipamento* adicionarEquipamento(Equipamento *lista, int patrimonio, char tipo[], char localizacao[]) {
    Equipamento *novo = malloc(sizeof(Equipamento));

    novo->patrimonio = patrimonio;
    strcpy(novo->tipo, tipo);
    strcpy(novo->localizacao, localizacao);
    novo->prox = lista;

    return novo;
}

void buscarEquipamento(Equipamento *lista, int patrimonio) {
    while (lista != NULL) {
        if (lista->patrimonio == patrimonio) {
            printf("Equipamento encontrado: Patrimônio %d | Tipo: %s | Localização: %s\n",
                   lista->patrimonio,
                   lista->tipo,
                   lista->localizacao);
            return;
        }

        lista = lista->prox;
    }

    printf("Equipamento não encontrado.\n");
}

Equipamento* removerEquipamento(Equipamento *lista, int patrimonio) {
    Equipamento *atual = lista;
    Equipamento *anterior = NULL;

    while (atual != NULL) {
        if (atual->patrimonio == patrimonio) {
            if (anterior == NULL) {
                lista = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }

            free(atual);
            printf("Equipamento removido.\n");
            return lista;
        }

        anterior = atual;
        atual = atual->prox;
    }

    printf("Equipamento não encontrado.\n");
    return lista;
}

void listarEquipamentos(Equipamento *lista) {
    if (lista == NULL) {
        printf("Nenhum equipamento cadastrado.\n");
        return;
    }

    printf("\nLista de equipamentos:\n");
    while (lista != NULL) {
        printf("Patrimônio: %d | Tipo: %s | Localização: %s\n",
               lista->patrimonio,
               lista->tipo,
               lista->localizacao);
        lista = lista->prox;
    }
}

int main() {
    Fila fila;
    Pilha pilha;
    Equipamento *equipamentos = NULL;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    abrirChamado(&fila, 101, "Internet caiu");
    abrirChamado(&fila, 102, "Impressora nao funciona");
    abrirChamado(&fila, 103, "Computador nao liga");

    listarChamados(&fila);

    printf("\nAtendendo chamado:\n");
    atenderProximoChamado(&fila);

    listarChamados(&fila);

    registrarAcao(&pilha, "Reiniciou servidor");
    registrarAcao(&pilha, "Alterou IP");
    registrarAcao(&pilha, "Resetou switch");

    mostrarHistorico(&pilha);

    desfazerUltimaAcao(&pilha);

    mostrarHistorico(&pilha);

    equipamentos = adicionarEquipamento(equipamentos, 1001, "Computador", "Sala 1");
    equipamentos = adicionarEquipamento(equipamentos, 1002, "Impressora", "Recepcao");
    equipamentos = adicionarEquipamento(equipamentos, 1003, "Switch", "Rack principal");

    listarEquipamentos(equipamentos);

    buscarEquipamento(equipamentos, 1002);

    equipamentos = removerEquipamento(equipamentos, 1002);

    listarEquipamentos(equipamentos);

    return 0;
}
