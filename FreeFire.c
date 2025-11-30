/*
 * estrutura_linear_completa.c
 * Programa demonstrativo em C que implementa:
 *  - Lista estática (vetor)
 *  - Lista dinâmica (realloc)
 *  - Lista encadeada (simples)
 *  - Operações: inserção, remoção, percurso, busca sequencial
 *  - Busca binária (iterativa e recursiva)
 *  - Métodos de ordenação: Bubble, Selection, Insertion, Quick, Merge
 *
 * O programa também instrumenta comparações e mede tempo (clock) para
 * comparar algoritmos. É um arquivo único para estudo e testes.
 *
 * Compile: gcc -O2 -std=c11 -o estrutura_linear_completa estrutura_linear_completa.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* =============================== */
/* ===== Lista Estática (Vetor) == */
/* =============================== */

#define MAX_STATIC 1000

typedef struct {
    int dados[MAX_STATIC];
    int tamanho;
} ListaEstatica;

void listaE_init(ListaEstatica *l) {
    l->tamanho = 0;
}

int listaE_inserirFim(ListaEstatica *l, int valor) {
    if (l->tamanho >= MAX_STATIC) return 0; // falha
    l->dados[l->tamanho++] = valor;
    return 1;
}

int listaE_removerPos(ListaEstatica *l, int pos) {
    if (pos < 0 || pos >= l->tamanho) return 0;
    for (int i = pos; i < l->tamanho - 1; i++)
        l->dados[i] = l->dados[i+1];
    l->tamanho--;
    return 1;
}

int listaE_buscaSeq(ListaEstatica *l, int valor) {
    for (int i = 0; i < l->tamanho; i++)
        if (l->dados[i] == valor) return i;
    return -1;
}

void listaE_print(ListaEstatica *l) {
    printf("[ ");
    for (int i = 0; i < l->tamanho; i++) printf("%d ", l->dados[i]);
    printf("]\n");
}

/* =============================== */
/* ===== Lista Dinâmica (realloc) */
/* =============================== */

typedef struct {
    int *dados;
    int tamanho;
    int capacidade;
} ListaDinamica;

ListaDinamica listaD_criar(int capInicial) {
    ListaDinamica l;
    l.dados = malloc(sizeof(int) * capInicial);
    l.tamanho = 0;
    l.capacidade = capInicial;
    return l;
}

void listaD_free(ListaDinamica *l) {
    free(l->dados);
    l->dados = NULL;
    l->tamanho = 0;
    l->capacidade = 0;
}

void listaD_expandirSeNecessario(ListaDinamica *l) {
    if (l->tamanho >= l->capacidade) {
        l->capacidade *= 2;
        l->dados = realloc(l->dados, sizeof(int) * l->capacidade);
        if (!l->dados) {
            perror("realloc falhou");
            exit(EXIT_FAILURE);
        }
    }
}

void listaD_inserirFim(ListaDinamica *l, int valor) {
    listaD_expandirSeNecessario(l);
    l->dados[l->tamanho++] = valor;
}

int listaD_removerPos(ListaDinamica *l, int pos) {
    if (pos < 0 || pos >= l->tamanho) return 0;
    for (int i = pos; i < l->tamanho-1; i++) l->dados[i] = l->dados[i+1];
    l->tamanho--;
    return 1;
}

int listaD_buscaSeq(ListaDinamica *l, int valor) {
    for (int i = 0; i < l->tamanho; i++)
        if (l->dados[i] == valor) return i;
    return -1;
}

void listaD_print(ListaDinamica *l) {
    printf("[ ");
    for (int i = 0; i < l->tamanho; i++) printf("%d ", l->dados[i]);
    printf("]\n");
}

/* =============================== */
/* ===== Lista Encadeada (simples) */
/* =============================== */

typedef struct No {
    int valor;
    struct No *prox;
} No;

No* listaL_criar() {
    return NULL;
}

No* listaL_inserirInicio(No *head, int valor) {
    No *novo = malloc(sizeof(No));
    novo->valor = valor;
    novo->prox = head;
    return novo;
}

No* listaL_inserirFim(No *head, int valor) {
    No *novo = malloc(sizeof(No));
    novo->valor = valor;
    novo->prox = NULL;
    if (!head) return novo;
    No *p = head;
    while (p->prox) p = p->prox;
    p->prox = novo;
    return head;
}

No* listaL_removerValor(No *head, int valor) {
    No *ant = NULL;
    No *p = head;
    while (p && p->valor != valor) {
        ant = p; p = p->prox;
    }
    if (!p) return head; // não achou
    if (!ant) { // removeu head
        No *tmp = head->prox;
        free(head);
        return tmp;
    }
    ant->prox = p->prox;
    free(p);
    return head;
}

int listaL_buscaSeq(No *head, int valor) {
    int idx = 0;
    while (head) {
        if (head->valor == valor) return idx;
        head = head->prox;
        idx++;
    }
    return -1;
}

void listaL_percorrer(No *head) {
    printf("[ ");
    while (head) {
        printf("%d ", head->valor);
        head = head->prox;
    }
    printf("]\n");
}

void listaL_free(No *head) {
    while (head) {
        No *tmp = head->prox;
        free(head);
        head = tmp;
    }
}

/* =============================== */
/* ===== Busca Binária (vetor) === */
/* =============================== */

int buscaBinIterativa(int v[], int n, int x) {
    int ini = 0, fim = n-1;
    while (ini <= fim) {
        int meio = ini + (fim - ini) / 2;
        if (v[meio] == x) return meio;
        if (v[meio] < x) ini = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

int buscaBinRecursiva(int v[], int ini, int fim, int x) {
    if (ini > fim) return -1;
    int meio = ini + (fim - ini) / 2;
    if (v[meio] == x) return meio;
    if (v[meio] < x) return buscaBinRecursiva(v, meio+1, fim, x);
    return buscaBinRecursiva(v, ini, meio-1, x);
}

/* =============================== */
/* ===== Instrumentação das ordena */
/* =============================== */

// contadores globais de comparações
unsigned long long comps;

void resetComps() { comps = 0; }

/* ----- Bubble Sort ----- */
void bubbleSort(int v[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            comps++; // comparação v[j] > v[j+1]
            if (v[j] > v[j+1]) {
                int tmp = v[j]; v[j] = v[j+1]; v[j+1] = tmp;
            }
        }
    }
}

/* ----- Selection Sort ----- */
void selectionSort(int v[], int n) {
    for (int i = 0; i < n-1; i++) {
        int min = i;
        for (int j = i+1; j < n; j++) {
            comps++; // comparação v[j] < v[min]
            if (v[j] < v[min]) min = j;
        }
        int tmp = v[i]; v[i] = v[min]; v[min] = tmp;
    }
}

/* ----- Insertion Sort ----- */
void insertionSort(int v[], int n) {
    for (int i = 1; i < n; i++) {
        int chave = v[i];
        int j = i - 1;
        while (j >= 0) {
            comps++; // comparação v[j] > chave
            if (v[j] > chave) {
                v[j+1] = v[j];
                j--;
            } else break;
        }
        v[j+1] = chave;
    }
}

/* ----- Quick Sort ----- */
int partition_quick(int v[], int ini, int fim) {
    int pivo = v[fim];
    int i = ini - 1;
    for (int j = ini; j < fim; j++) {
        comps++; // v[j] <= pivo
        if (v[j] <= pivo) {
            i++;
            int tmp = v[i]; v[i] = v[j]; v[j] = tmp;
        }
    }
    int tmp = v[i+1]; v[i+1] = v[fim];