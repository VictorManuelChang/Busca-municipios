#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include"../include/libfacom.h"

typedef struct {
   int codigo_ibge;
   double latitude;
   double longitude;
} tmunicipio;

int cmp(void *t1, void *t2){
    tmunicipio *m1 = (tmunicipio *)t1;
    tmunicipio *m2 = (tmunicipio *)t2;

    if (m1->latitude == m2->latitude) {
        return (m1->longitude - m2->longitude);
    } else {
        return (m1->latitude - m2->latitude);
    }
}


void abb_constroi(tarv *parv, int (*cmp)(void *, void *)){
    parv->raiz = NULL;
    parv->cmp  = cmp;
}

tmunicipio * aloca_municipio(int codigo_ibge, double latitude, double longitude) {
    tmunicipio * pmunicipio;
    pmunicipio = malloc(sizeof(tmunicipio));
    pmunicipio->codigo_ibge = codigo_ibge;
    pmunicipio->latitude = latitude;
    pmunicipio->longitude = longitude;
    return pmunicipio;
}

int abb_insere_node(tarv * parv, tnode ** ppnode, void *reg, int profundidade){
    if (*ppnode == NULL){
         *ppnode = malloc(sizeof(tnode));
         (*ppnode)->reg = reg;
         (*ppnode)->esq = NULL;
         (*ppnode)->dir = NULL;
    } else {
        int camada = profundidade % 2; 

        if (camada == 0) { 
            if (((tmunicipio *)reg)->latitude < ((tmunicipio *)(*ppnode)->reg)->latitude) {
                abb_insere_node(parv, &((*ppnode)->esq), reg, profundidade + 1);
            } else {
                abb_insere_node(parv, &((*ppnode)->dir), reg, profundidade + 1);
            }
        } else { 
            if (((tmunicipio *)reg)->longitude < ((tmunicipio *)(*ppnode)->reg)->longitude) {
                abb_insere_node(parv, &((*ppnode)->esq), reg, profundidade + 1);
            } else {
                abb_insere_node(parv, &((*ppnode)->dir), reg, profundidade + 1);
            }
        }
    }
    return 1; 
}

int abb_insere(tarv * parv,  void * reg){
    return abb_insere_node(parv, &parv->raiz, reg, 0); 
}

void * abb_busca_node(tarv * parv, tnode * pnode, void *reg, int profundidade){
    if (pnode == NULL){
        return NULL;
    }

    int camada = profundidade % 2; 

    if (parv->cmp(pnode->reg, reg) == 0) { 
        return pnode->reg;
    }

    if (camada == 0) { 
        if (((tmunicipio *)reg)->latitude < ((tmunicipio *)(pnode->reg))->latitude) {
            return abb_busca_node(parv, pnode->esq, reg, profundidade + 1);
        } else {
            return abb_busca_node(parv, pnode->dir, reg, profundidade + 1);
        }
    } else { 
        if (((tmunicipio *)reg)->longitude < ((tmunicipio *)(pnode->reg))->longitude) {
            return abb_busca_node(parv, pnode->esq, reg, profundidade + 1);
        } else {
            return abb_busca_node(parv, pnode->dir, reg, profundidade + 1);
        }
    }
}

void * abb_busca(tarv * parv,  void * reg){
    return abb_busca_node(parv, parv->raiz, reg, 0); 
}




int main() {
    
    tarv arvore;
    abb_constroi(&arvore, cmp);

   
    tmunicipio *municipio1 = aloca_municipio(12345, -22.876, -45.098);
    tmunicipio *municipio2 = aloca_municipio(67890, -23.456, -46.789);
    tmunicipio *municipio3 = aloca_municipio(24680, -24.135, -47.632);

    abb_insere(&arvore, municipio1);
    abb_insere(&arvore, municipio2);
    abb_insere(&arvore, municipio3);

   
    printf("Municípios inseridos:\n");

    tmunicipio *resultado1 = abb_busca(&arvore, municipio1);
    if (resultado1 != NULL)
        printf("Código IBGE: %d, Latitude: %lf, Longitude: %lf\n", resultado1->codigo_ibge, resultado1->latitude, resultado1->longitude);

    tmunicipio *resultado2 = abb_busca(&arvore, municipio2);
    if (resultado2 != NULL)
        printf("Código IBGE: %d, Latitude: %lf, Longitude: %lf\n", resultado2->codigo_ibge, resultado2->latitude, resultado2->longitude);

    tmunicipio *resultado3 = abb_busca(&arvore, municipio3);
    if (resultado3 != NULL)
        printf("Código IBGE: %d, Latitude: %lf, Longitude: %lf\n", resultado3->codigo_ibge, resultado3->latitude, resultado3->longitude);

    
    free(municipio1);
    free(municipio2);
    free(municipio3);

    return 0;
}
