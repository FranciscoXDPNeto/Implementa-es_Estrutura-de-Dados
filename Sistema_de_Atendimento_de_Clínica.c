#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct {
    char nome[50];
    int idade;
} PacienteFila;

typedef struct {
    PacienteFila dados[MAX];
    int inicio;
    int fim;
} Fila;

typedef struct {
    char nome[50];
    char especialidade[50];
} Atendimento;

typedef struct {
    Atendimento dados[MAX];
    int topo;
} Pilha;

typedef struct Paciente {
    char nome[50];
    char cpf[20];
    char telefone[20];
    struct Paciente *prox;
} Paciente;

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
}

int filaVazia(Fila *f) {
    return f->inicio == f->fim;
}

int filaCheia(Fila *f) {
    return f->fim == MAX;
}

void cadastrarNaFila(Fila *f, char nome[], int idade) {
    if (filaCheia(f)) {
        printf("Fila cheia.\n");
        return;
    }

    strcpy(f->dados[f->fim].nome, nome);
    f->dados[f->fim].idade = idade;
    f->fim++;
}

void chamarProximo(Fila *f) {
    if (filaVazia(f)) {
        printf("Fila vazia.\n");
        return;
    }

    printf("Chamando: %s, %d anos\n",
           f->dados[f->inicio].nome,
           f->dados[f->inicio].idade);

    f->inicio++;
}

void exibirFila(Fila *f) {
    if (filaVazia(f)) {
        printf("Fila de espera vazia.\n");
        return;
    }

    printf("\nFila de espera:\n");
    for (int i = f->inicio; i < f->fim; i++) {
        printf("%s, %d anos\n", f->dados[i].nome, f->dados[i].idade);
    }
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX - 1;
}

void registrarAtendimento(Pilha *p, char nome[], char especialidade[]) {
    if (pilhaCheia(p)) {
        printf("Histórico cheio.\n");
        return;
    }

    p->topo++;
    strcpy(p->dados[p->topo].nome, nome);
    strcpy(p->dados[p->topo].especialidade, especialidade);
}

void desfazerUltimoRegistro(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Histórico vazio.\n");
        return;
    }

    printf("Registro desfeito: %s - %s\n",
           p->dados[p->topo].nome,
           p->dados[p->topo].especialidade);

    p->topo--;
}

void exibirHistorico(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("Histórico vazio.\n");
        return;
    }

    printf("\nHistórico de atendimentos:\n");
    for (int i = p->topo; i >= 0; i--) {
        printf("%s - %s\n", p->dados[i].nome, p->dados[i].especialidade);
    }
}

Paciente* cadastrarPaciente(Paciente *lista, char nome[], char cpf[], char telefone[]) {
    Paciente *novo = malloc(sizeof(Paciente));

    strcpy(novo->nome, nome);
    strcpy(novo->cpf, cpf);
    strcpy(novo->telefone, telefone);
    novo->prox = lista;

    return novo;
}

void buscarPaciente(Paciente *lista, char cpf[]) {
    while (lista != NULL) {
        if (strcmp(lista->cpf, cpf) == 0) {
            printf("Paciente encontrado: %s | CPF: %s | Telefone: %s\n",
                   lista->nome, lista->cpf, lista->telefone);
            return;
        }
        lista = lista->prox;
    }

    printf("Paciente não encontrado.\n");
}

Paciente* removerPaciente(Paciente *lista, char cpf[]) {
    Paciente *atual = lista;
    Paciente *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->cpf, cpf) == 0) {
            if (anterior == NULL) {
                lista = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }

            free(atual);
            printf("Cadastro removido.\n");
            return lista;
        }

        anterior = atual;
        atual = atual->prox;
    }

    printf("Paciente não encontrado.\n");
    return lista;
}

void listarPacientes(Paciente *lista) {
    if (lista == NULL) {
        printf("Nenhum paciente cadastrado.\n");
        return;
    }

    printf("\nLista de pacientes cadastrados:\n");
    while (lista != NULL) {
        printf("%s | CPF: %s | Telefone: %s\n",
               lista->nome, lista->cpf, lista->telefone);
        lista = lista->prox;
    }
}

int main() {
    Fila fila;
    Pilha pilha;
    Paciente *cadastro = NULL;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    cadastrarNaFila(&fila, "Joao", 25);
    cadastrarNaFila(&fila, "Maria", 40);
    cadastrarNaFila(&fila, "Carlos", 31);
    cadastrarNaFila(&fila, "Ana", 28);

    exibirFila(&fila);

    printf("\nChamando dois pacientes:\n");
    chamarProximo(&fila);
    chamarProximo(&fila);

    exibirFila(&fila);

    registrarAtendimento(&pilha, "Joao", "Clinico Geral");
    registrarAtendimento(&pilha, "Maria", "Cardiologia");

    exibirHistorico(&pilha);

    desfazerUltimoRegistro(&pilha);

    exibirHistorico(&pilha);

    cadastro = cadastrarPaciente(cadastro, "Joao", "11111111111", "99999-1111");
    cadastro = cadastrarPaciente(cadastro, "Maria", "22222222222", "99999-2222");
    cadastro = cadastrarPaciente(cadastro, "Carlos", "33333333333", "99999-3333");

    listarPacientes(cadastro);

    buscarPaciente(cadastro, "22222222222");

    cadastro = removerPaciente(cadastro, "22222222222");

    listarPacientes(cadastro);

    return 0;
}
