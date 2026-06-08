#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    int pid;
    char nome[50];
    int prioridade;
} ProcessoFila;

typedef struct {
    ProcessoFila dados[MAX];
    int qtd;
} FilaPrioridade;

typedef struct {
    int pid;
    char motivo[100];
} ProcessoEncerrado;

typedef struct {
    ProcessoEncerrado dados[MAX];
    int topo;
} Pilha;

typedef struct ProcessoAtivo {
    int pid;
    char nome[50];
    char estado[30];
    struct ProcessoAtivo *prox;
} ProcessoAtivo;

void inicializarFila(FilaPrioridade *f) {
    f->qtd = 0;
}

int filaVazia(FilaPrioridade *f) {
    return f->qtd == 0;
}

void adicionarProcesso(FilaPrioridade *f, int pid, char nome[], int prioridade) {
    if (f->qtd == MAX) {
        printf("Fila de processos cheia.\n");
        return;
    }

    f->dados[f->qtd].pid = pid;
    strcpy(f->dados[f->qtd].nome, nome);
    f->dados[f->qtd].prioridade = prioridade;
    f->qtd++;
}

void executarProcesso(FilaPrioridade *f) {
    if (filaVazia(f)) {
        printf("Nenhum processo aguardando CPU.\n");
        return;
    }

    int maior = 0;

    for (int i = 1; i < f->qtd; i++) {
        if (f->dados[i].prioridade > f->dados[maior].prioridade) {
            maior = i;
        }
    }

    printf("Executando processo: PID %d | %s | Prioridade %d\n",
           f->dados[maior].pid,
           f->dados[maior].nome,
           f->dados[maior].prioridade);

    for (int i = maior; i < f->qtd - 1; i++) {
        f->dados[i] = f->dados[i + 1];
    }

    f->qtd--;
}

void listarFila(FilaPrioridade *f) {
    if (filaVazia(f)) {
        printf("Nenhum processo na fila.\n");
        return;
    }

    printf("\nProcessos aguardando CPU:\n");
    for (int i = 0; i < f->qtd; i++) {
        printf("PID %d | %s | Prioridade %d\n",
               f->dados[i].pid,
               f->dados[i].nome,
               f->dados[i].prioridade);
    }
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void registrarEncerramento(Pilha *p, int pid, char motivo[]) {
    if (p->topo == MAX - 1) {
        printf("Histórico cheio.\n");
        return;
    }

    p->topo++;
    p->dados[p->topo].pid = pid;
    strcpy(p->dados[p->topo].motivo, motivo);
}

void desfazerEncerramento(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Nenhum encerramento registrado.\n");
        return;
    }

    printf("Removendo do histórico: PID %d | Motivo: %s\n",
           p->dados[p->topo].pid,
           p->dados[p->topo].motivo);

    p->topo--;
}

void mostrarHistorico(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Histórico vazio.\n");
        return;
    }

    printf("\nHistórico de processos encerrados:\n");
    for (int i = p->topo; i >= 0; i--) {
        printf("PID %d | Motivo: %s\n",
               p->dados[i].pid,
               p->dados[i].motivo);
    }
}

ProcessoAtivo* adicionarAtivo(ProcessoAtivo *lista, int pid, char nome[], char estado[]) {
    ProcessoAtivo *novo = malloc(sizeof(ProcessoAtivo));

    novo->pid = pid;
    strcpy(novo->nome, nome);
    strcpy(novo->estado, estado);
    novo->prox = lista;

    return novo;
}

void buscarAtivo(ProcessoAtivo *lista, int pid) {
    while (lista != NULL) {
        if (lista->pid == pid) {
            printf("Processo ativo encontrado: PID %d | %s | Estado: %s\n",
                   lista->pid,
                   lista->nome,
                   lista->estado);
            return;
        }

        lista = lista->prox;
    }

    printf("Processo ativo não encontrado.\n");
}

ProcessoAtivo* removerAtivo(ProcessoAtivo *lista, int pid) {
    ProcessoAtivo *atual = lista;
    ProcessoAtivo *anterior = NULL;

    while (atual != NULL) {
        if (atual->pid == pid) {
            if (anterior == NULL) {
                lista = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }

            free(atual);
            printf("Processo ativo removido.\n");
            return lista;
        }

        anterior = atual;
        atual = atual->prox;
    }

    printf("Processo ativo não encontrado.\n");
    return lista;
}

void listarAtivos(ProcessoAtivo *lista) {
    if (lista == NULL) {
        printf("Nenhum processo ativo.\n");
        return;
    }

    printf("\nProcessos ativos:\n");
    while (lista != NULL) {
        printf("PID %d | %s | Estado: %s\n",
               lista->pid,
               lista->nome,
               lista->estado);
        lista = lista->prox;
    }
}

int main() {
    FilaPrioridade fila;
    Pilha pilha;
    ProcessoAtivo *ativos = NULL;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    adicionarProcesso(&fila, 1, "antivirus", 10);
    adicionarProcesso(&fila, 2, "navegador", 3);
    adicionarProcesso(&fila, 3, "editor", 2);

    listarFila(&fila);

    printf("\nExecutando processo mais prioritário:\n");
    executarProcesso(&fila);

    listarFila(&fila);

    ativos = adicionarAtivo(ativos, 1, "antivirus", "executando");
    ativos = adicionarAtivo(ativos, 2, "navegador", "aguardando");
    ativos = adicionarAtivo(ativos, 3, "editor", "aguardando");

    listarAtivos(ativos);

    buscarAtivo(ativos, 2);

    ativos = removerAtivo(ativos, 3);

    listarAtivos(ativos);

    registrarEncerramento(&pilha, 3, "finalizado pelo usuario");
    registrarEncerramento(&pilha, 2, "erro de memoria");

    mostrarHistorico(&pilha);

    desfazerEncerramento(&pilha);

    mostrarHistorico(&pilha);

    return 0;
}
