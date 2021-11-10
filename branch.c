/*
Autor: Matheus Fellone dos Santos
Universidade Federal do ABC
Código implementado para projeto de graduação da computação

Implementação algoritmo branch and bound para o problema do BRP
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estruturas.h"
#include "movimentos.h"
#include "metodos.h"

void copiaEstado(Estado *estado, Estado *novoEstado)
{
    int i;
    int qtdPilhas = novoEstado->qtdPilhas;
    int alturaMax = novoEstado->alturaMax;
    novoEstado->qtdBloqueantes = estado->qtdBloqueantes;
    novoEstado->qtdConteiners = estado->qtdConteiners;
    novoEstado->pilhaAlvo = estado->pilhaAlvo;
    novoEstado->alvo = estado->alvo;
    novoEstado->lowerBound = estado->lowerBound;

    for (i = 0; i < alturaMax - 1; i++)
    {
        novoEstado->bloqueanteAlvo[i] = estado->bloqueanteAlvo[i];
    }

    for (i = 0; i < qtdPilhas; i++)
    {
        novoEstado->alturaPilha[i] = estado->alturaPilha[i];
    }

    int j;
    i = 0;
    while (i < qtdPilhas)
    {
        j = 0;
        while (j < alturaMax)
        {
            novoEstado->compartimento[i][j] = estado->compartimento[i][j];
            j++;
        }
        i++;
    }
}
void inserirFilhos(listaFilhos *filhos, Estado *estado)
{
    Filho *novo = malloc(sizeof(Filho));
    if (novo == NULL)
        return;

    novo->filho = estado;
    novo->proximo = NULL;

    if (filhos->inicio == NULL)
    {
        filhos->inicio = novo;
        return;
    }

    if ((novo->filho)->lowerBound <= (filhos->inicio->filho)->lowerBound)
    {
        Filho *inicio = filhos->inicio;
        novo->proximo = inicio;
        filhos->inicio = novo;
        return;
    }

    Filho *anterior = filhos->inicio;
    Filho *atual = filhos->inicio->proximo;
    while (anterior != NULL)
    {
        if (atual == NULL || ((novo->filho)->lowerBound <= (atual->filho)->lowerBound))
        {
            anterior->proximo = novo;
            novo->proximo = atual;
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    return;
}

void geraFilhos(Estado* estado, listaFilhos *filhos)
{
    Estado* novoEstado;
    //Adiciona estado a árvore quando não há bloqueantes
    if (estado->qtdBloqueantes == 0)
    {
        novoEstado = CriaFilho(estado);
        //Insere filho e calcula realocações confirmadas
        inserirFilhos(filhos, novoEstado);
        return;
    }
    for (int i = 0; i < estado->qtdPilhas; i++)
    {
        if (i != estado->pilhaAlvo && estado->compartimento[i][estado->alturaMax - 1] == 0)
        {
            realocaBloco(estado, estado->pilhaAlvo, i, 1);
            geraFilhos(estado, filhos);
            realocaBloco(estado, i, estado->pilhaAlvo, 0);
        }
    }
    return;
}

int branchBound(Estado *estado, int mov, int melhorSol, clock_t start)
{
    listaFilhos *proximos_estados = criar_filhos();
    if ((double)(clock() - start) / CLOCKS_PER_SEC > 600)
        return -1;
    if (estado_vazio(estado))
    {
        if (mov  < melhorSol)
        {
            melhorSol = mov ;
            
        }
        return melhorSol;
    }
    //Verifica se o lower bound do né atual ja é maior que a melhor solução encontrada
    if ((mov + estado->lowerBound) >= melhorSol)
        return melhorSol;
    mov = mov + estado->qtdBloqueantes;
    geraFilhos(estado, proximos_estados);
    Filho *atual = proximos_estados->inicio;
    while (atual != NULL)
    {
        melhorSol = branchBound(atual->filho, mov, melhorSol, start);
        atual = atual->proximo;
    }
    liberar_filhos(proximos_estados->inicio);
    free(proximos_estados);
    return melhorSol;
}

int main(int argc, char *argv[])
{

    int pilhas, altura, alturaAlvo, i, j;
    int conteiners = 0;
    int realoc = 0;
    Estado *estado;
    Solucao *solucao = criar_solucao();

    FILE *file;
    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Error: can't open file to read\n");
    }
    else
    {

        fscanf(file, "%d", &pilhas);
        fscanf(file, "%d", &altura);

        estado = CriaEstado(pilhas, altura);

        for (i = 0; i < pilhas; i++)
        {
            for (j = 0; j < altura; j++)
            {

                fscanf(file, "%d", &estado->compartimento[i][j]);

                if (estado->compartimento[i][j] != 0)
                {
                    conteiners++;
                    estado->alturaPilha[i]++;

                    if (estado->compartimento[i][j] == 1)
                    {
                        estado->pilhaAlvo = i;
                        alturaAlvo = j;
                    }
                }
            }
        }

        estado->qtdConteiners = conteiners;
        estado->alvo = 1;
        int *vetorM = aloca_vetor(conteiners);
        for (int i = 0; i < conteiners; i++)
        {
            vetorM[i] = 0;
        }

        fclose(file);

        for (i = alturaAlvo + 1, j = 0; estado->compartimento[estado->pilhaAlvo][i] != 0; i++, j++)
        {
            estado->bloqueanteAlvo[j] = estado->compartimento[estado->pilhaAlvo][i];
            estado->qtdBloqueantes++;
        }
        Estado *estadoAtual = CriaEstado(estado->qtdPilhas, estado->alturaMax);
        copiaEstado(estado, estadoAtual);
        minMax(estadoAtual, solucao, vetorM);

        clock_t start = clock();
        
        realoc = branchBound(estado, 0, solucao->realoc, start);
        clock_t end = clock();

        LiberaEstado(estado);
        LiberaEstado(estadoAtual);

        if(realoc != -1)
            printf("Elapsed: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
            printf("%d\n", realoc);
        else
            printf("TIMEOUT");
    }
    return 0;
}