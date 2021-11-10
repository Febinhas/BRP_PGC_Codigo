#include "estruturas.h"
#include "movimentos.h"
#include "metodos.h"
#include <stdio.h>
#include <stdlib.h>

int** aloca_matriz(int m, int n) {
    int **M;
    int i;

    M = (int **) malloc(sizeof(int*) * m);
    if (M == NULL) {
        printf("Memoria insuficiente.\n");
        exit(1);
    }
    for (i = 0; i < m; i++) {
        M[i] = (int *) malloc(sizeof(int) * n);
        if (M[i] == NULL) {
            printf("Memoria insuficiente.\n");
            exit(1);
        }
    }
    return M;
}

int* aloca_vetor(int n) {
    int *V;
    V = (int *) malloc(sizeof(int) * n);
    if (V == NULL) {
        printf("Memoria insuficiente.\n");
        exit(1);
    }

    return V;
}

void libera_matriz(int **M, int m) {
    int i;
    for (i = 0; i < m; i++)
        free(M[i]);
    free(M);
}

void imprime_compartimento(int **comp, int pilhas, int altura) {
    int i, j;
    for (i = altura-1; i >= 0; i--) {
        for (j = 0; j < pilhas; j++) {
            printf(" %2d ", comp[j][i]);
        }
        printf("\n");
    }
}

void ImprimeEstado(Estado *estado)
{
    int i;
    printf("===============================\n");
    printf("Alvo atual: %d\n", estado->alvo);
    printf("Compartimento:\n");
    imprime_compartimento(estado->compartimento, estado->qtdPilhas, estado->alturaMax);
    printf("LowerBound: %d\n", estado->lowerBound);
    printf("Qtd bloqueantes alvo: %d\n", estado->qtdBloqueantes);
    for (i = 0; i < estado->qtdBloqueantes; i++)
        printf(" %d ", estado->bloqueanteAlvo[i]);
    printf("\n");
    printf("===============================\n");
}

void LiberaEstado(Estado *estado)
{
    libera_matriz(estado->compartimento, estado->qtdPilhas);
    free(estado->alturaPilha);
    free(estado->bloqueanteAlvo);
    free(estado);
}

Estado *CriaEstado(int qtdPilhas, int alturaMax)
{
    int i;

    Estado *estado = malloc(sizeof(Estado));
    estado->qtdPilhas = qtdPilhas;
    estado->alturaMax = alturaMax;

    estado->compartimento = aloca_matriz(qtdPilhas, alturaMax);
    estado->bloqueanteAlvo = aloca_vetor(alturaMax - 1);
    estado->alturaPilha = aloca_vetor(qtdPilhas);

    estado->qtdBloqueantes = 0;
    estado->qtdConteiners = 0;
    estado->lowerBound = 0;

    for (i = 0; i < alturaMax - 1; i++)
    {
        estado->bloqueanteAlvo[i] = 0;
    }
    for (i = 0; i < qtdPilhas; i++)
    {
        estado->alturaPilha[i] = 0;
    }

    return estado;
}

Solucao *criar_solucao()
{
    Solucao *sol = malloc(sizeof(Solucao));
    sol->inicio = NULL;
    sol->realoc = 0;
    return sol;
}

void imprimir_sol(Solucao *solucao)
{
    Move *inicio = solucao->inicio;
    Move *atual, *anterior = NULL;
    for (atual = inicio; atual != NULL; atual = atual->proximo)
    {
        printf("/ %d %d  ", atual->bloco, atual->destino);
    }
    printf("\n");
}

listaFilhos *criar_filhos()
{
    listaFilhos *filhos = malloc(sizeof(listaFilhos));
    filhos->inicio = NULL;
    return filhos;
}

void inserirMovimento(Solucao *sol, int bloco, int destino, int numRealoc, int alvo)
{
    Move *novo = malloc(sizeof(Move));
    if (novo == NULL)
        return;

    novo->bloco = bloco;
    novo->destino = destino;
    novo->alvo = alvo;
    novo->numRealoc = numRealoc;
    Move *inicio = sol->inicio;

    sol->inicio = novo;
    novo->proximo = inicio;
    //inserir no final

    return;
}

void liberar_solucao(Move *inicio)
{
    if (inicio == NULL)
        return;
    liberar_solucao(inicio->proximo);
    free(inicio);
    return;
}

void liberar_filhos(Filho *inicio)
{
    if (inicio == NULL)
        return;
    liberar_filhos(inicio->proximo);
    LiberaEstado(inicio->filho);
    return;
}

Estado *CriaFilho(Estado* estadoPai) {
    int i;
    int qtdPilhas = estadoPai->qtdPilhas;
    int alturaMax = estadoPai->alturaMax;
    Estado *estado = malloc(sizeof(Estado));
    estado->qtdPilhas = qtdPilhas;
    estado->alturaMax = alturaMax;

    estado->compartimento = aloca_matriz(qtdPilhas, alturaMax);
    estado->bloqueanteAlvo = aloca_vetor(alturaMax - 1);
    estado->alturaPilha = aloca_vetor(qtdPilhas);

    estado->qtdBloqueantes = estadoPai->qtdBloqueantes;
    estado->qtdConteiners = estadoPai->qtdConteiners;
    estado->pilhaAlvo = estadoPai->pilhaAlvo;
    estado->alvo = estadoPai->alvo;
    estado->lowerBound = 0;

    for (i = 0; i < alturaMax - 1; i++)
    {
        estado->bloqueanteAlvo[i] = estadoPai->bloqueanteAlvo[i];
    }
 
    for (i = 0; i < qtdPilhas; i++)
    {
        estado->alturaPilha[i] = estadoPai->alturaPilha[i];
    }
 
    int j;
    i = 0;
    while (i < qtdPilhas)
    {
        j = 0;
        while (j < alturaMax)
        {
            estado->compartimento[i][j] = estadoPai->compartimento[i][j];
            j++;
        }
        i++;
    }
    removeAlvo(estado);
    calculaLowerBound(estado);
    return estado;
}
