#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DIM 2 // Duas dimensões: latitude e longitude

typedef struct cidade {
    int codigo_ibge;
    char nome[50];
    double latitude;
    double longitude;
} cidade;

typedef struct tnode {
    void *reg;
    struct tnode *esq;
    struct tnode *dir;
} tnode;

typedef struct tarv {
    tnode *raiz;
    int (*cmp)(void *, void *);
    int dim;
} tarv;

// Função para comparar duas cidades baseado em latitude e longitude
int cmp(void *t1, void *t2) {
    cidade *c1 = (cidade *)t1;
    cidade *c2 = (cidade *)t2;

    if (c1->latitude == c2->latitude) {
        return (c1->longitude - c2->longitude);
    } else {
        return (c1->latitude - c2->latitude);
    }
}

// Função para construir a k-d tree
void kd_constroi(tarv *parv, int (*cmp)(void *, void *), int dim) {
    parv->raiz = NULL;
    parv->cmp = cmp;
    parv->dim = dim;
}

// Função para alocar uma cidade
cidade *aloca_cidade(int codigo_ibge, char nome[], double latitude, double longitude) {
    cidade *pcidade;
    pcidade = malloc(sizeof(cidade));
    pcidade->codigo_ibge = codigo_ibge;
    strcpy(pcidade->nome, nome);
    pcidade->latitude = latitude;
    pcidade->longitude = longitude;
    return pcidade;
}


// Função para inserir um nó na k-d tree
int kd_insere_node(tarv *parv, tnode **ppnode, void *reg, int depth) {
    int cd = depth % parv->dim;

    if (*ppnode == NULL) {
        *ppnode = malloc(sizeof(tnode));
        (*ppnode)->reg = reg;
        (*ppnode)->esq = NULL;
        (*ppnode)->dir = NULL;
    } else if (parv->cmp((*ppnode)->reg, reg) > 0) { // Esquerda
        kd_insere_node(parv, &((*ppnode)->esq), reg, depth + 1);
    } else { // Direita
        kd_insere_node(parv, &((*ppnode)->dir), reg, depth + 1);
    }
}

// Função para inserir um nó na k-d tree
int kd_insere(tarv *parv, void *reg) {
    return kd_insere_node(parv, &parv->raiz, reg, 0);
}

// Função para buscar um nó na k-d tree
void *kd_busca_node(tarv *parv, tnode *pnode, void *reg, int depth) {
    int cd = depth % parv->dim;

    if (pnode == NULL) {
        return NULL;
    }

    if (parv->cmp(pnode->reg, reg) == 0) { // Encontrado
        return pnode->reg;
    }

    if (parv->cmp(pnode->reg, reg) > 0) { // Esquerda
        return kd_busca_node(parv, pnode->esq, reg, depth + 1);
    } else { // Direita
        return kd_busca_node(parv, pnode->dir, reg, depth + 1);
    }
}

// Função para buscar um nó na k-d tree
void *kd_busca(tarv *parv, void *reg) {
    return kd_busca_node(parv, parv->raiz, reg, 0);
}

int main() {
    tarv kd_tree;
    kd_constroi(&kd_tree, cmp, DIM);

    // Exemplo de inserção
    kd_insere(&kd_tree, aloca_cidade(5200050, "Abadia de Goiás", -16.7573, -49.4412));
    kd_insere(&kd_tree, aloca_cidade(3100104, "Abadia dos Dourados", -18.4831, -47.3916));

    // Exemplo de busca (supondo que queremos encontrar cidades próximas a uma certa latitude e longitude)
    double latitude_busca = -18.8;
    double longitude_busca = -47.4;
    cidade cidade_busca = { .latitude = latitude_busca, .longitude = longitude_busca };
    cidade *cidade_proxima = kd_busca(&kd_tree, &cidade_busca);

    if (cidade_proxima != NULL) {
        printf("Cidade próxima encontrada: %s, Latitude: %f, Longitude: %f\n", cidade_proxima->nome, cidade_proxima->latitude, cidade_proxima->longitude);
    } else {
        printf("Nenhuma cidade próxima encontrada.\n");
    }

    return 0;
}
