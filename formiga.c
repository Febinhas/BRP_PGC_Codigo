/*
Autor: Matheus Fellone dos Santos
Universidade Federal do ABC
Código implementado para projeto de graduação da computação

Implementação Algoritmo Colônia de Formigas para o problema do BRP
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estruturas.h"
#include "movimentos.h"
#include "metodos.h"

typedef struct candidato
{
    int destino;
    double valorCandidato;
    double prob;
    struct candidato *prox;
} Candidato;

typedef struct listaCandidato
{
    Candidato *inicio; 
} listaCandidato;

listaCandidato *criar_listaCandidatos()
{
    listaCandidato *lista = malloc(sizeof(listaCandidato));
    lista->inicio = NULL;
    return lista;
}

double ****AlocaMatriz4Dim(int m, int n, int o, int p)
{
    double ****M;
    int i, j, k;

    M = (double ****)malloc(sizeof(double ***) * m);
    if (M == NULL)
    {
        printf("Memoria insuficiente.\n");
        return NULL;
    }
    for (i = 0; i < m; i++)
    {
        M[i] = (double ***)malloc(sizeof(double**) * n);
        if (M[i] == NULL)
        {
            printf("Memoria insuficiente.\n");
            return NULL;
        }
        for (j = 0; j < n; j++)
        {
            M[i][j] = (double **)malloc(sizeof(double *) * o);
            if (M[i][j] == NULL)
            {
                printf("Memoria insuficiente.\n");
                return NULL;
            }

            for (k = 0; k < o; k++)
            {
                M[i][j][k] = (double *)malloc(sizeof(double) * p);
                if (M[i][j][k] == NULL)
                {
                    printf("Memoria insuficiente.\n");
                    return NULL;
                }
            }
        }
    }

    return M;
}

void LiberaMatriz4Dim(int ****M, int m, int n, int o)
{
    int i, j, k;
    for (i = 0; i < m; i++){
        for (j = 0; j < n; j++){
            for (k = 0; k < o; k++)
                free(M[i][j][k]);
            free(M[i][j]);
        }
        free(M[i]);
    }      
    free(M);
}

double calculaF(int diff) {
    double f;
    f = 1 / ((double) 1 + diff);
    return f;
}

//Adiciona realocações canditadas a lista
void adicionaCandidato(listaCandidato *canditados, Estado *estado, double ****feromonio, int pilha, int *vetorM){
    int diff, minP, bloq;
    double valorF;
    double valorG;
    double valorFeromonio;
    int realocado = primeiroBloqueanteF(estado);
    minP = calcularMinPF(estado, pilha);
    int minP2 = calcularMinPF2(estado, pilha);
    valorFeromonio = feromonio[realocado-1][minP2][vetorM[realocado-1]][estado->alvo-1];
    

    
    if (minP > realocado)
    {
        diff = minP - realocado;
    }
    else
    {
        diff = (2 * estado->qtdConteiners) + 1 - minP;
    }
    valorF = calculaF(diff);
    valorG = valorF * valorFeromonio;

    //ALOCAR E ADICIONAR O CANDIDATO NO INICIO
    Candidato *novo = malloc(sizeof(Candidato));
    if (novo == NULL)
        return;
    novo->destino = pilha;
    novo->valorCandidato = valorG;

    Candidato *inicio = canditados->inicio;
    canditados->inicio = novo;
    novo->prox = inicio;

    return;
}

void liberar_candidatos(Candidato *inicio)
{
    if (inicio == NULL)
        return;
    liberar_candidatos(inicio->prox);
    free(inicio);
    return;
}

void ColoniaDeFormigas(Estado *estado, Solucao *sol, clock_t start)
{
    
    int iteracoes;
    int numFormigas = 10;
    iteracoes = 0;
    Solucao *novaSolucao;
    int i, j, k, l;
    Estado *estadoAtual = CriaEstado(estado->qtdPilhas, estado->alturaMax);
    copiaEstado(estado, estadoAtual);
    //feromonio[blocoRealocado][pilhaDestino][RealocaçõesDoBloco][Alvo]
    int feromonioMinParameter = estado->qtdConteiners + estado->qtdPilhas+1;
    double ****feromonio = AlocaMatriz4Dim(estado->qtdConteiners, feromonioMinParameter, 10, estado->qtdConteiners);
    //vetor que controla quantas vezes cada conteiner foi realocado
    int *vetorM = aloca_vetor(estado->qtdConteiners);
    for (int i = 0; i < estado->qtdConteiners; i++)
    {
        vetorM[i] = 0;
    }
    minMax(estadoAtual, sol, vetorM);
    
    calculaLowerBound(estado);
    //calculaLowerBound2(estado);
    //calculaLowerBound3(estado);
    
    //Valor inicial da Matriz de ferômonio
    double valSol = (double) 1 / (sol->realoc + estado->lowerBound + 1);
    
    //Define valor inicial da Matriz de feromonio
    for (i = 0; i < estado->qtdConteiners; i++)
    {
        for (j = 0; j < feromonioMinParameter; j++)
        {
            for (k = 0; k < 10; k++)
            {
                for (l = 0; l < estado->qtdConteiners; l++)
                {
                    feromonio[i][j][k][l] = ((double)1 / estado->alturaMax) * valSol;
                }
            }
        }
    }
    double somaDeG;
    listaCandidato *candidatos = criar_listaCandidatos();
    Candidato *candidatoAtual, *candidatoSelecionado;
    double q, select, probTotal;
    double exploracao = 0.9;
    double local = 0.9;
    double global = 0.1;
    int realocado, destino, solucaoValida;
    novaSolucao = criar_solucao();

    int iterSemMudanca = 0;
    while (iteracoes < 5000)
    {
        if ((double)(clock() - start) / CLOCKS_PER_SEC > 600)
            return -1;
        iteracoes++;
        for (int i = 0; i < numFormigas; i++) {
            for (int i = 0; i < estado->qtdConteiners; i++)
            {
                vetorM[i] = 0;
            }
            copiaEstado(estado, estadoAtual);
            solucaoValida = 1;
            while (!estado_vazio(estadoAtual))
            {
                
                while(estadoAtual->qtdBloqueantes != 0) {
                    somaDeG = 0;
                    realocado = primeiroBloqueanteF(estadoAtual);
                    liberar_candidatos(candidatos->inicio);
                    candidatos->inicio = NULL;
                    //Selecionar lista dos possiveis candidatos para a realocação do bloqueante, ja calculandos a prob e o G para regra de transição
                    for (int i = 0; i < estado->qtdPilhas; i++)
                    {
                        if (i != estadoAtual->pilhaAlvo && estadoAtual->compartimento[i][estado->alturaMax - 1] == 0)
                        {
                            adicionaCandidato(candidatos, estadoAtual, feromonio, i, vetorM);
                            somaDeG = somaDeG + candidatos->inicio->valorCandidato;
                        }
                    }
                    
                    candidatoAtual = candidatos->inicio;
                    while (candidatoAtual != NULL)
                    {
                        candidatoAtual->prob = candidatoAtual->valorCandidato / somaDeG;
                        candidatoAtual = candidatoAtual->prox;
                    }
                    //Regra de transição ---------------------------------------------------------------
                    q = rand() % 101;
                    q = q / 100;
                   //Select - Determinístico
                    if(q < exploracao) {
                        candidatoSelecionado = candidatos->inicio;
                        candidatoAtual = candidatoSelecionado->prox;
                        while(candidatoAtual != NULL) {
                            if(candidatoAtual->valorCandidato>candidatoSelecionado->valorCandidato) {
                                candidatoSelecionado = candidatoAtual;  
                            }
                            candidatoAtual = candidatoAtual->prox;
                        }
                    }
                    //Select - Probabilistico
                    else {
                        select = rand() % 101;
                        select = select / 100;
                        probTotal = 0;
                        candidatoSelecionado = candidatos->inicio;
                        while (candidatoSelecionado->prox != NULL)
                        {
                            if (select >= probTotal && select < probTotal + candidatoSelecionado->prob)
                            {
                                break;
                            }
                            probTotal = probTotal+candidatoSelecionado->prob;
                            candidatoSelecionado = candidatoSelecionado->prox;
                        }
                        
                    }
                    //----------------------------------------------------------------------------------------W
                    destino = candidatoSelecionado->destino;
                    vetorM[realocado - 1]++;
                    inserirMovimento(novaSolucao, realocado, destino + 1, vetorM[realocado - 1], estadoAtual->alvo);
                    realocaBloco(estadoAtual, estadoAtual->pilhaAlvo, destino, 1);
                    calculaLowerBound(estadoAtual);
                    //calculaLowerBound2(estado);
                    //calculaLowerBound3(estado);
                    novaSolucao->realoc++;
                    //Verificar se solução parcial ja é pior que a melhor solução encontrada
                    if(novaSolucao->realoc+estadoAtual->lowerBound > sol->realoc){
                        solucaoValida = 0;
                        break;
                    }
                }
                if(solucaoValida == 0)
                    break;
                vetorM[estadoAtual->alvo - 1]++;
                inserirMovimento(novaSolucao, estadoAtual->alvo, 0, vetorM[estadoAtual->alvo - 1], estadoAtual->alvo);
                removeAlvo(estadoAtual);
            }
            //Atualização local Matriz de feromonio
            Move *atualMovimento = novaSolucao->inicio;
            while(atualMovimento !=NULL) {
                if (atualMovimento->destino != 0) {
                    feromonio[atualMovimento->bloco - 1][calcularMinPF2(estado, atualMovimento->destino - 1)][atualMovimento->numRealoc][atualMovimento->alvo - 1] *= local;
                }
                atualMovimento = atualMovimento->proximo;
            }
            if(solucaoValida != 0) {
                //Verificar se a nova solução é a melhor solução encontrada
                if(novaSolucao->realoc < sol->realoc) {
                    liberar_solucao(sol->inicio);
                    sol->inicio = novaSolucao->inicio;
                    sol->realoc = novaSolucao->realoc;
                    iterSemMudanca = 0;
                }
                else {
                    iterSemMudanca++;
                }
            }
            novaSolucao->inicio = NULL;
            novaSolucao->realoc = 0;
        }
        if(iterSemMudanca > 100) {
            for (int a = 0; a < estado->qtdConteiners; a++)
            {
                for (int b = 0; b < feromonioMinParameter; b++)
                {
                    for (int c = 0; c < 10; c++)
                    {
                        for (int d = 0; d < estado->qtdConteiners; d++)
                        {
                            feromonio[a][b][c][d] = ((double)1 / estado->alturaMax) * valSol;
                        }
                    }
                }
            }
        }
        //Atualização global Matriz de feromonio
        Move *atualMovimento = sol->inicio;
        while (atualMovimento != NULL)
        {
            if(atualMovimento->destino != 0) {
                double delta = 1 / ((double)(sol->realoc) + estado->lowerBound + 1);
                double fero = feromonio[atualMovimento->bloco - 1][calcularMinPF2(estado, atualMovimento->destino - 1)][atualMovimento->numRealoc][atualMovimento->alvo - 1];
                feromonio[atualMovimento->bloco - 1][calcularMinPF2(estado, atualMovimento->destino - 1)][atualMovimento->numRealoc][atualMovimento->alvo - 1] = (1 - global) * fero + global * delta;
            }
            atualMovimento = atualMovimento->proximo;
        }
    }
    
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
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

        fclose(file);
        int *vetorM = aloca_vetor(conteiners);
        for (int i = 0; i < conteiners; i++)
        {
            vetorM[i] = 0;
        }

        for (i = alturaAlvo + 1, j = 0; estado->compartimento[estado->pilhaAlvo][i] != 0; i++, j++)
        {
            estado->bloqueanteAlvo[j] = estado->compartimento[estado->pilhaAlvo][i];
            estado->qtdBloqueantes++;
        }

        clock_t start = clock();
        ColoniaDeFormigas(estado, solucao, start);
        clock_t end = clock();

        LiberaEstado(estado);
        if(realoc != -1)
            printf("Elapsed: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
            printf("Realoc: %d\n", solucao->realoc);
        else
            printf("TIMEOUT");
        liberar_solucao(solucao->inicio);
        free(solucao);
    }
    return 0;
}