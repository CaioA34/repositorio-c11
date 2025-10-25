#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

typedef int elemento_t;

typedef struct nodo_ab{
    elemento_t valor;
    struct nodo_ab *fesq;
    struct nodo_ab *fdir;
} no_ab;

// apontador para o t i p o de no da a r v o r e b i n a r i a
typedef no_ab *apontador ;

typedef apontador arv_bin_t;

void criar_arv_bin(arv_bin_t *A){
    *A = NULL;
}

bool arv_vazia(arv_bin_t A){
    return (A == NULL);
}

apontador criar_no(elemento_t val){
    apontador p = (apontador) malloc (sizeof(no_ab));
    if(!p){
        fprintf(stderr,"Erro: malloc\n");
        exit(EXIT_FAILURE);
    }
    p->valor = val;
    p->fesq = NULL;
    p->fdir = NULL;
    return p;
}

void atribui_no(apontador q, elemento_t val){
    if(q) q->valor = val;
}

elemento_t valor_raiz(arv_bin_t A){
    if(!A) {
        fprintf(stderr,"Erro: valor_raiz em arvore vazia\n");
        exit(EXIT_FAILURE);
    }
    return A->valor;
}

void atrib_esq(arv_bin_t A, apontador p){
    if(A) A->fesq = p;
}

void atrib_dir(arv_bin_t A, apontador p){
    if(A) A->fdir = p;
}

apontador esq(arv_bin_t A){
    return A ? A->fesq : NULL;
}

apontador dir(arv_bin_t A){
    return A ? A->fdir : NULL;
}

elemento_t remover_no (apontador q){
    if(!q){
        fprintf(stderr,"ERRO: Remocao recebeu NULL");
        return 0;
    }
    elemento_t v = q->valor;
    free(q);
    return v;
}

apontador pai(arv_bin_t A, apontador p){
    if(!A || !p) return NULL;
    if(A->fesq==p || A->fdir == p) return A;
    apontador q = pai(A->fesq, p);
    if(q) return p;
    return pai(A->fdir, p);
}

void cam_prefixado(arv_bin_t A){
    if(!A) return;
    printf("%d ",A->valor);
    cam_prefixado(A->fesq);
    cam_prefixado(A->fdir);
}

void cam_central(arv_bin_t A){
    if(!A) return;
    cam_central(A->fesq);
    printf("%d ",A->valor);
    cam_central(A->fdir);
}

void cam_posfixado(arv_bin_t A){
    if(!A) return;
    cam_posfixado(A->fesq);
    cam_posfixado(A->fdir);
    printf("%d ",A->valor);
}

void libera_arvore(arv_bin_t *pA){
    if(!pA || !*pA) return;
    libera_arvore(&((*pA)->fesq));
    libera_arvore(&((*pA)->fdir));
    free(*pA);
    *pA = NULL;
}

void imprime_estrutura(arv_bin_t A, int nivel){
    if(!A) return;
    imprime_estrutura(A->fdir, nivel+1);
    for(int i=0 ;i<nivel; ++i)
        printf("\t");
    printf("%d\n",A->valor);
    imprime_estrutura(A->fesq, nivel+1);
}

apontador busca_binaria(arv_bin_t A, elemento_t v){
    if(!A) return NULL;
    if(v == A->valor) return A;
    else if(v < A->valor) return busca_binaria(A->fesq, v);
    else return busca_binaria(A->fdir, v);
}

apontador insere_bin(arv_bin_t *pA, elemento_t v){
    if(!*pA){
        *pA = criar_no(v);
        return *pA;
    }
    if(v < (*pA)->valor) return insere_bin(&((*pA)->fesq),v );
    else insere_bin(&((*pA)->fdir),v );
}

apontador insere_binario_SR(arv_bin_t *pA, elemento_t v){
    apontador existe = busca_binaria(*pA, v);
    if(existe) return existe;
    return insere_bin(pA, v);
}

apontador menor_maior(apontador q){
    if(!q || !q->fdir) return NULL;
    apontador p = q->fdir;
    while(p->fesq) p = p->fesq;
    return p;
}

arv_bin_t remove_binario(arv_bin_t *pA, apontador q){
    if(!pA || !*pA || !q) return *pA;
    apontador p = pai(*pA, q);

    //caso 1 e 2: até 2 filhos - se um dos lados é NULL, o filho sobe!
    if(!q->fesq || !q->fdir){
        apontador filho = q->fesq ? q->fesq : q->fdir;
        if(!p) {
            *pA = filho;
        } else if(p->fesq == q){
            p->fesq = filho;
        } else {
            p->fdir = filho;
        }
        remover_no(q);
        return*pA;
    }
    //caso 3: dois filhos - copia o sucessor
    apontador suc = menor_maior(q);
    if(!suc) {
        suc = q->fesq;
        while(suc->fdir) suc = suc->fdir;
    }
    atribui_no(q, suc->valor);
    return remove_binario(pA, suc);
}


int main(){
    arv_bin_t T;
    criar_arv_bin(&T);
    int valores[] = {9,7,15,3,1,10,19,14,2,5};
    int n = (int)(sizeof(valores)/sizeof(valores[0]));
    for(int i=0;i<n;++i){
        insere_binario_SR(&T,valores[i]);
    }
    printf("-- BST construida --\n");
    imprime_estrutura(T,0);

    printf("Caminhamento - Pre-Ordem:\n");
    cam_prefixado(T);

    printf("\nCaminhamento - Pos-Ordem:\n");
    cam_posfixado(T);
    printf("\nCaminhamento - Central:\n");
    cam_central(T);
    printf("\n");

    printf("\nBusca e Remocao\n");
    int alvo = 9;
    apontador q = busca_binaria(T,alvo);
    if(q) {
        printf("Valor %d encontrado!! \n",alvo);
        remove_binario(&T,q);
        printf("Valor %d deletado!! \n",alvo);
        imprime_estrutura(T,0);
    }
}