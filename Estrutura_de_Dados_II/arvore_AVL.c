//
// Created by Caio on 30/11/2025.
//
/*
* Implemente uma árvore AVL para armazenar livros encantados, onde a chave é o número mágico do livro. Após cada inserção, o sistema deve:
•	Mostrar a altura da árvore;
•	Mostrar o fator de balanceamento de cada nó;
•	Indicar se ocorreu rotação simples ou dupla.
Teste com as chaves: 40, 20, 10, 25, 30, 22.
 */
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stddef.h>

// Estruturas Principais
typedef struct AVLNode {
    int key;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

typedef struct {
    AVLNode *root;
    size_t size;
}AVLTree;

// Funções Úteis
static int max(int a, int b){
    return a>b ? a : b;
}

static int height(AVLNode *n){
    return n ? n->height : 0;
}

static int balance_factor(AVLNode *n){
    return n ? height(n->left) - height(n->right) : 0;
}


//Funções para a Árvore
static AVLNode* make_node(int key){
    AVLNode *n = (AVLNode*) malloc (sizeof(AVLNode));
    if(!n) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    n->key = key;
    n->height = 1;
    n->left = n->right = NULL;
    return n;
}

static void free_nodes(AVLNode *n){
    if(!n) return;
    free_nodes(n->left);
    free_nodes(n->right);
    free(n);
}

// Funções de Rotação
static AVLNode* rotate_right(AVLNode *y){
    AVLNode *x = y->left;
    AVLNode *aux = x->right;

    x->right = y;
    y->left = aux;

    y->height = 1+max(height(y->left), height(y->right));
    x->height = 1+max(height(x->left), height(x->right));
    return x;
}

static AVLNode* rotate_left(AVLNode *x){
    AVLNode *y = x->right;
    AVLNode *aux = y->left;

    y->left = x;
    x-> right = aux;

    x->height = 1+max(height(x->left), height(x->right));
    y->height = 1+max(height(y->left), height(y->right));
    return y;
}

//(RE)Balanceamento
static AVLNode* balance(AVLNode *n){
    if(!n) return NULL;
    n->height = 1+max(height(n->left), height(n->right));
    int bf = balance_factor(n);

    //Esquerda - esquerda
    if(bf>1 && balance_factor(n->left)>=0)
        return rotate_right(n);

    //Esquerda - direita
    if(bf>1 && balance_factor(n->left) < 0){
        n->left = rotate_left(n->left);
        return rotate_right(n);
    }

    //Direita - direita
    if(bf < -1 && balance_factor(n->right)<=0)
        return rotate_left(n);

    //Direita - esquerda
    if(bf < -1 && balance_factor(n->right) > 0){
        n->right = rotate_right(n->right);
        return rotate_left(n);
    }
    return n;
}

// Inserção
static AVLNode* insert_rec(AVLNode *node, int key, bool *inserted){
    if(!node){
        *inserted = true;
        return make_node(key);
    }
    if(key < node->key)
        node->left = insert_rec(node->left,key, inserted);
    else if(key > node->key)
        node->right = insert_rec(node->right,key, inserted);
    else return node;
    /*
     *
     */
    return balance(node);

}

//Remoção
static AVLNode* minNode(AVLNode* n){
    while (n && n->left) n = n->left;
    return n;
}

static AVLNode* remove_rec(AVLNode *node, int key, bool *removed){
    if(!node) return NULL;

    if(key < node->key){
        node->left = remove_rec(node->left, key, removed);
    }
    else if(key > node->key){
        node->right = remove_rec(node->right, key, removed);
    } else{
        *removed = true;
        //caso 1 e 2: 0 ou 1 filho apenas
        if(!node->left || !node->right){
            AVLNode *tmp = node->left ? node->left : node->right;
            if(!tmp){
                //CASO 1: 0 filhos
                free(node);
                return NULL;
            } else {
                //Caso 2: 1 filho
                *node = *tmp;
            }
        } else {
            // Caso 3: 2 filhos
            AVLNode *succ = minNode(node->right);
            node->key = succ->key;
            node->right = remove_rec(node->right, succ->key, removed);
        }
    }
    return balance(node);
}

//Busca
static bool search_rec(AVLNode *n, int key){
    if(!n) return false;
    if(key == n->key) return true;
    return key < n->key ? search_rec(n->left,key) : search_rec(n->right,key);
}

//Impressões
static void in_order_print_rec(AVLNode *n){
    if(!n) return;
    in_order_print_rec(n->left);
    printf("%d ",n->key);
    in_order_print_rec(n->right);
}

static void pre_order_print_rec(AVLNode *n){
    if(!n) return;
    printf("%d ",n->key);
    pre_order_print_rec(n->left);
    pre_order_print_rec(n->right);
}

static void post_order_print_rec(AVLNode *n){
    if(!n) return;
    post_order_print_rec(n->left);
    post_order_print_rec(n->right);
    printf("%d ",n->key);
}

static void print_rec(AVLNode *n, int depth, char branch){
    if(!n) return;
    print_rec(n->right, depth+1, '/');
    for(int i = 0;i<depth;i++)printf("\t");
    if(depth) printf("%c- ",branch);
    printf("%d (h=%d)\n",n->key, n->height);
    print_rec(n->left, depth+1, '\\');
}

//Funções Públicas

void avl_print_inorder(const AVLTree *t){
    in_order_print_rec(t?t->root:NULL);
}

void avl_print_preorder(const AVLTree *t){
    pre_order_print_rec(t?t->root:NULL);
}

void avl_print_postorder(const AVLTree *t){
    post_order_print_rec(t?t->root:NULL);
}

void avl_print(const AVLTree *t){
    if(!t||!t->root){
        printf("(arvore vazia)\n");
        return;
    }
    print_rec(t->root,0,'*');
}

AVLTree* avl_create(void){
    AVLTree *t = (AVLTree*) malloc(sizeof(AVLTree));
    if(!t){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    t->root = NULL;
    t->size = 0;
    return t;
}

void avl_destroy(AVLTree *t){
    if(!t) return;
    free_nodes(t->root);
    free(t);
}

bool avl_insert(AVLTree *t, int key){
    bool inserted = false;
    t->root = insert_rec(t->root,key,&inserted);
    if(inserted) t->size++;
    return inserted;
}

bool avl_remove(AVLTree *t, int key){
    bool removed = false;
    t->root = remove_rec(t->root, key, &removed);
    if(removed) t->size--;
    return removed;
}

bool avl_search(const AVLTree *t, int key){
    return search_rec(t?t->root:NULL, key);
}

size_t avl_size(const AVLTree *t){
    return t?t->size:0;
}

int main(void){
    AVLTree *t = avl_create();
    int valores[] = {10, 5, 4, 12, 17, 20, 3, 25, 15};
    int n = (int) (sizeof(valores)/sizeof(valores[0]));
    for(int i=0 ;i<n;i++){
        avl_insert(t,valores[i]);
    }
    printf("Arvore apos insercoes: \n");
    avl_print(t);

    printf("\nImpressao em ordem: ");
    avl_print_inorder(t);
    printf("\nImpressao Pre ordem: ");
    avl_print_preorder(t);
    printf("\nImpressao Pos ordem: ");
    avl_print_postorder(t);

    int valor1 = 10;
    int valor2 = 22;
    printf("\n\nValor %d existe?\n%s",valor1,avl_search(t,valor1)?"Sim!\n":"Nao!!\n");
    printf("\nValor %d existe?\n%s",valor2,avl_search(t,valor2)?"Sim!\n":"Nao!!\n");
    printf("\n\nTamanho Final da Arvore: %d",avl_size(t));
    int remocoes[] = {5, 15, 12};
    int m = (int) (sizeof(remocoes)/sizeof(remocoes[0]));
    for(int i=0 ;i<m;i++){
        printf("\n\nRemovendo o %d...\n",remocoes[i]);
        avl_remove(t,remocoes[i]);
        avl_print(t);
    }

    printf("\n\nTamanho Final da Arvore: %d\n",avl_size(t));
    avl_destroy(t);
    printf("\nArvore destruida!!\n");
    t = avl_create();
    printf("\nTamanho Final da Arvore: %d\n",avl_size(t));
}
