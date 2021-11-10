#include <stdlib.h>
#include <stdio.h>
#include "estruturas.h"
#include "movimentos.h"
#include "metodos.h"

int primeiroBloqueante(Estado estado, int altura) {

    for (int i = 0; i < altura - 1; i++)
    {
        if (estado.bloqueanteAlvo[i] == 0)
        {
            return estado.bloqueanteAlvo[i - 1];
        }
        if (i == altura - 2)
        {
            return estado.bloqueanteAlvo[i];
          }
    }
}
int primeiroBloqueanteF(Estado *estado)
{
    for (int i = 0; i < estado->alturaMax - 1; i++)
    {
        if (estado->bloqueanteAlvo[i] == 0)
        {
            return estado->bloqueanteAlvo[i - 1];
        }
        if (i == estado->alturaMax - 2)
        {
            return estado->bloqueanteAlvo[i];
        }
    }
}

int calcularMinP(Estado estado, int pilha, int altura, int containers)

{
    int min = 0;
    for (int i = 0; i < altura && estado.compartimento[pilha][i] != 0; i++)
    {
        if (min == 0 || estado.compartimento[pilha][i] < min)
        {
            min = estado.compartimento[pilha][i];
        }
    }
    if (min == 0)
        min = containers + 1;

    return min;
}
int calcularMinPF(Estado *estado, int pilha)
{
    int min = 0;
    for (int i = 0; i < estado->alturaMax && estado->compartimento[pilha][i] != 0; i++)
    {
        if (min == 0 || estado->compartimento[pilha][i] < min)
        {
            min = estado->compartimento[pilha][i];
        }
    }
    if (min == 0)
        min = estado->qtdConteiners + 1;

    return min;
}

int calcularMinPF2(Estado *estado, int pilha)
{
    int min = 0;
    for (int i = 0; i < estado->alturaMax && estado->compartimento[pilha][i] != 0; i++)
    {
        if (min == 0 || estado->compartimento[pilha][i] < min)
        {
            min = estado->compartimento[pilha][i];
        }
    }
    if (min == 0)
        min = estado->qtdConteiners + pilha+1;

    return min;
}

int acharDestino(Estado *estado)
{
    int diff = 0;
    int diffMin = 0;
    int destino;
    int bloq;
    int min;
    for (int j = 0; estado->bloqueanteAlvo[j] != 0; j++)
    {
        bloq = estado->bloqueanteAlvo[j];
    }
    for (int i = 0; i < estado->qtdPilhas; i++)
    {
        if (i != estado->pilhaAlvo && estado->compartimento[i][estado->alturaMax - 1] == 0)
        {
            min = calcularMinPF(estado, i);
            if (min > bloq)
            {
                diff = min - bloq;
            }
            else
            {
                diff = (2 * estado->qtdConteiners) + 1 - min;
            }
            if (diffMin == 0 || diff < diffMin)
            {
                diffMin = diff;
                destino = i;
            }
        }
    }

    return destino;
}

//Metodo MinMax
void minMax(Estado *estado, Solucao *sol, int *vetorM)
{
    int valor;
    if (estado->alvo == estado->qtdConteiners)
    {
        vetorM[estado->alvo - 1]++;
        inserirMovimento(sol, estado->alvo, 0, vetorM[estado->alvo - 1], estado->alvo);
        return;
    }
    if (estado->bloqueanteAlvo[0] == 0)
    {
        vetorM[estado->alvo - 1]++;
        inserirMovimento(sol, estado->alvo, 0, vetorM[estado->alvo - 1], estado->alvo);
        removeAlvo(estado);
        minMax(estado, sol, vetorM);
        return;
    }
    else
    {
        int realocado;
        int destino = acharDestino(estado);
        realocado = primeiroBloqueanteF(estado);
        inserirMovimento(sol, realocado, destino + 1, vetorM[realocado - 1], estado->alvo);
        vetorM[realocado - 1]++;
        realocaBloco(estado, estado->pilhaAlvo, destino, 1);
        minMax(estado, sol, vetorM);
        sol->realoc++;
        return;
    }
}

int estado_vazio(Estado *estado)
{
    for (int i = 0; i < estado->qtdPilhas; i++)
    {
        for (int j = 0; j < estado->alturaMax; j++)
        {
            if (estado->compartimento[i][j] != 0)
                return 0;
        }
    }
    return 1;
}

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

void calculaLowerBound(Estado* estado)
{
    int menor;
    int lowerBound = 0;
    for (int i = 0; i < estado->qtdPilhas; i++)
    {
        menor = estado->compartimento[i][0];
        for (int j = 1; j < estado->alturaMax && estado->compartimento[i][j] != 0; j++)
        {
            if (estado->compartimento[i][j] > menor)
                lowerBound++;
            else
                menor = estado->compartimento[i][j];
        }
    }

    estado->lowerBound = lowerBound;
    return;
}

void calculaLowerBound2(Estado* estado) {
    calculaLowerBound(estado);
    //estado->lowerBound - calculado no calcula
    int adiciona = 0;
    for (int j = 0; j < estado->qtdBloqueantes; j++) {
        for (int i = 0; i < estado->qtdPilhas; i++)
        {
            if (i != estado->pilhaAlvo && estado->compartimento[i][estado->alturaMax - 1] == 0)
            {
                if(calcularMinPF(estado, i) > estado->bloqueanteAlvo[j]) {
                    adiciona = 1;
                    break;
                }
            }
        }
        if(adiciona != 1) {
            estado->lowerBound += 1;
        }
    }  
}

void novoEstadoLowerBound(Estado *estado) {
    int min = 0;
    int minPilha;
    while (estado->qtdBloqueantes != 0)
    {
        estado->bloqueanteAlvo[estado->qtdBloqueantes - 1] = 0;
        estado->qtdBloqueantes--;
    }
    int i = estado->pilhaAlvo;
    int bloco = estado->compartimento[i][estado->alturaPilha[i] - 1];
    estado->compartimento[i][estado->alturaPilha[i] - 1] = 0;
    while(bloco != estado->alvo) {
        estado->compartimento[i][estado->alturaPilha[i] - 1] = 0;
        estado->alturaPilha[i]--;
        bloco = estado->compartimento[i][estado->alturaPilha[i] - 1];
        
    }
    estado->compartimento[i][estado->alturaPilha[i] - 1] = 0;
    estado->alturaPilha[i]--;

    for (int j = 0; j < estado->qtdPilhas; j++) {
        minPilha = calcularMinPF(estado, j);
        if (minPilha < min || min == 0)
        {
            min = minPilha;
            estado->pilhaAlvo = j;
            estado->alvo = min;
        }
    }
    int achou = 0;
    for (int j = 0; j < estado->alturaPilha[estado->pilhaAlvo]; j++) {
        if (achou)
        {
            estado->bloqueanteAlvo[estado->qtdBloqueantes] = estado->compartimento[estado->pilhaAlvo][j];
            estado->qtdBloqueantes++;
            continue;
        }
        if(estado->compartimento[estado->pilhaAlvo][j] == estado->alvo) {
            achou = 1;
            continue;
        }        
    }
}
int lowerBound3Aux(Estado *estado) {
    if(estado_vazio(estado)) {
        return 0;
    }
    novoEstadoLowerBound(estado);
    int adiciona = 0;
    int somaLower = 0;
    for (int j = 0; j < estado->qtdBloqueantes; j++)
    {
        for (int i = 0; i < estado->qtdPilhas; i++)
        {
            if (i != estado->pilhaAlvo && estado->compartimento[i][estado->alturaMax - 1] == 0)
            {
                if (calcularMinPF(estado, i) > estado->bloqueanteAlvo[j])
                {
                    adiciona = 1;
                    break;
                }
            }
        }
        if (adiciona != 1)
        {
            somaLower++;
        }
    }

    return somaLower + lowerBound3Aux(estado);
} 

void calculaLowerBound3(Estado *estado)
{
    calculaLowerBound2(estado);
    Estado *estadoAtual = CriaEstado(estado->qtdPilhas, estado->alturaMax);
    copiaEstado(estado, estadoAtual);
    lowerBound3Aux(estadoAtual);
    LiberaEstado(estadoAtual);
}
