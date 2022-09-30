#include <stdio.h>
#include <stdlib.h>

typedef struct no{

    int valor;
    struct no *esquerdo, *direito;
    short altura;

}No;

// Função que Criar um Nó.
No* novo_no(int valor){

    No *novo = malloc(sizeof(No));

    if(novo != NULL){

        novo->valor = valor;
        novo->esquerdo = NULL;
        novo->direito = NULL;
        novo->altura = 0;
    }
    else{

        printf("\n\tERRO ao alocar nó em novo_no!\n");
    }

    return novo;
}

// Função que Retorna o Maior Valor.
short maior(short a, short b){

    return (a > b) ? a : b;
}

// Função que Retorna a Altura de um Nó ou -1 caso ele seja NULL.
short altura_do_no(No *no){

    if(no == NULL){

        return -1;
    }
    else{

        return no->altura;
    }
}

// Função que Calcula e Retorna o Fator de Balanceamento de um Nó.
short fator_de_balanceamento(No *no){

    if(no){

        return (altura_do_no(no->esquerdo) - altura_do_no(no->direito));
    }
    else{

        return 0;
    }
}

// --------------------------- ROTAÇÕES ---------------------------

// Função para a Rotação à Esquerda.
No* rotacao_esquerda(No *r){

    No *y, *f;

    y = r->direito;
    f = y->esquerdo;

    y->esquerdo = r;
    r->direito = f;

    r->altura = maior(altura_do_no(r->esquerdo), altura_do_no(r->direito)) + 1;
    y->altura = maior(altura_do_no(y->esquerdo), altura_do_no(y->direito)) + 1;

    return y;
}

// Função para a Rotação à Direita
No* rotacao_direita(No *r){

    No *y, *f;

    y = r->esquerdo;
    f = y->direito;

    y->direito = r;
    r->esquerdo = f;

    r->altura = maior(altura_do_no(r->esquerdo), altura_do_no(r->direito)) + 1;
    y->altura = maior(altura_do_no(y->esquerdo), altura_do_no(y->direito)) + 1;

    return y;
}

// Função para Fazer a Rotação Esquerda Direita.
No* rotacao_esquerda_direita(No *r){

    r->esquerdo = rotacao_esquerda(r->esquerdo);

    return rotacao_direita(r);
}

// Função para Fazer a Rotação Direita Esquerda.
No* rotacao_direita_esquerda(No *r){

    r->direito = rotacao_direita(r->direito);

    return rotacao_esquerda(r);
}

/*
    Função para realizar o Balanceamento da Árvore após uma Inserção ou Remoção
    Recebe o Nó que está Desbalanceado e Retorna a Nova Raiz após o Balanceamento.
*/
No* balancear(No *raiz){

    short fb = fator_de_balanceamento(raiz);

    // Rotação à Esquerda.
    if(fb < -1 && fator_de_balanceamento(raiz->direito) <= 0)
        raiz = rotacao_esquerda(raiz);

    // Rotação à Direita.
    else if(fb > 1 && fator_de_balanceamento(raiz->esquerdo) >= 0)
        raiz = rotacao_direita(raiz);

    // Rotação Dupla à Esquerda.
    else if(fb > 1 && fator_de_balanceamento(raiz->esquerdo) < 0)
        raiz = rotacao_esquerda_direita(raiz);

    // Rotação Dupla à Direita
    else if(fb < -1 && fator_de_balanceamento(raiz->direito) > 0)
        raiz = rotacao_direita_esquerda(raiz);

    return raiz;
}

// Função que Inserir um Novo Nó na Árvore e Balancear se Necessario.
No* inserir(No *raiz, int valor){

    if(raiz == NULL){

        return novo_no(valor);
    }
    else{

        if(valor < raiz->valor){

            raiz->esquerdo = inserir(raiz->esquerdo, valor);
        }
        else if(valor > raiz->valor){

            raiz->direito = inserir(raiz->direito, valor);
        }
        else{

            printf("\n\tInsercao Nao Realizada!\n\tO Elemento %d ja Existe!\n", valor);
        }
    }

    // Recalcula a Altura de todos os Nós entre a Raiz e o Novo Nó Inserido.
    raiz->altura = maior(altura_do_no(raiz->esquerdo), altura_do_no(raiz->direito)) + 1;

    // verifica a Necessidade de Rebalancear a Árvore.
    raiz = balancear(raiz);

    return raiz;
}

// Função para Remover um Nó da Arvore e Balancear se Necessario.
No* remover(No *raiz, int chave) {

    if(raiz == NULL){

        printf("\n\tValor nao encontrado!\n");

        return NULL;
    }
    else{

        if(raiz->valor == chave) {

            if(raiz->esquerdo == NULL && raiz->direito == NULL){

                free(raiz);

                printf("\n\tElemento folha removido: %d !\n", chave);

                return NULL;
            }
            else{

                if(raiz->esquerdo != NULL && raiz->direito != NULL){

                    No *aux = raiz->esquerdo;

                    while(aux->direito != NULL){

                        aux = aux->direito;
                    }

                    raiz->valor = aux->valor;

                    aux->valor = chave;

                    printf("\n\tElemento trocado: %d !\n", chave);

                    raiz->esquerdo = remover(raiz->esquerdo, chave);

                    return raiz;
                }
                else{

                    No *aux;

                    if(raiz->esquerdo != NULL){

                        aux = raiz->esquerdo;
                    }
                    else{
                        aux = raiz->direito;
                    }

                    free(raiz);

                    printf("\n\tElemento com 1 filho removido: %d !\n", chave);

                    return aux;
                }
            }
        }
        else{

            if(chave < raiz->valor){

                raiz->esquerdo = remover(raiz->esquerdo, chave);
            }
            else{

                raiz->direito = remover(raiz->direito, chave);
            }
        }

        // Recalcula a Altura de todos os Nós entre a Raiz e o Novo Nó Inserido.
        raiz->altura = maior(altura_do_no(raiz->esquerdo), altura_do_no(raiz->direito)) + 1;

        // Verifica a Necessidade de Rebalancear a Árvore.
        raiz = balancear(raiz);

        return raiz;
    }
}

// Função para Imprimir a Árvore.
void imprimir(No *raiz, int nivel){

    int i;

    if(raiz != NULL){

        imprimir(raiz->direito, nivel + 1);

        printf("\n\n");

        for(i = 0; i < nivel; i++){

            printf("\t");
        }

        printf("%d", raiz->valor);

        imprimir(raiz->esquerdo, nivel + 1);
    }
}

int main(){

    No *raiz = NULL;
    int opcao, valor;

    do{
        printf("\n\n\t0 - Sair\n\t1 - Inserir\n\t2 - Remover\n\t3 - Imprimir\n\n");
        scanf("%d", &opcao);

        switch(opcao){
        case 0:

            printf("\n\tSAINDO...\n");

            break;
        case 1:

            printf("\n\tDigite o Valor a ser Inserido: ");
            scanf("%d", &valor);

            raiz = inserir(raiz, valor);

            break;
        case 2:

            printf("\n\tDigite o Valor a ser Removido: ");
            scanf("%d", &valor);

            raiz = remover(raiz, valor);

            break;
        case 3:

            printf("\n");

            imprimir(raiz, 1);

            printf("\n");

            break;
        default:

            printf("\n\tOpcao Invalida!\n");

            break;
        }

    }while(opcao != 0);

    return 0;
}
