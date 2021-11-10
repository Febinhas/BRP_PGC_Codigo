#include "movimentos.h"
#include <stdio.h>
#include "estruturas.h"
#include <stdlib.h>

void removeAlvo(Estado *estado)
{
    /* Assume que o alvo está no topo da pilha dele (portanto, não tem bloqueantes) */
    
    int i, j, achou;

    estado->compartimento[estado->pilhaAlvo][estado->alturaPilha[estado->pilhaAlvo] - 1] = 0;
    estado->alturaPilha[estado->pilhaAlvo]--;

    estado->alvo = estado->alvo + 1;
    /* Vamos encontrar a pilha do novo alvo */
    achou = 0;
    i = 0;
    while (i < estado->qtdPilhas && !achou)
    {
        j = 0;
        while (j < estado->alturaPilha[i] && !achou)
        {
            if (estado->compartimento[i][j] == estado->alvo)
                achou = 1;
            j++;
        }
        i++;
    }
    estado->pilhaAlvo = i - 1;
    for (i = j, j = 0; estado->compartimento[estado->pilhaAlvo][i] != 0 && i<estado->alturaMax; i++, j++)
    {
        estado->bloqueanteAlvo[j] = estado->compartimento[estado->pilhaAlvo][i];
        estado->qtdBloqueantes++;
    }
}

void devolveAlvo(Estado *estado, int pilha)
{
    estado->alvo = estado->alvo - 1;
    estado->pilhaAlvo = pilha;

    estado->qtdBloqueantes = 0;

    estado->compartimento[pilha][estado->alturaPilha[pilha]] = estado->alvo;
    estado->alturaPilha[pilha]++;
}

void realocaBloco(Estado *estado, int origem, int destino, int modo)
{
    /* Essa função já assume que a pilha de destino tem espaço para a realocação.
     * A variável modo indica se devemos colocar ou remover o bloco do conjunto de bloqueantes */

    int bloco;

    bloco = estado->compartimento[origem][estado->alturaPilha[origem] - 1];
    estado->compartimento[origem][estado->alturaPilha[origem] - 1] = 0;
    estado->alturaPilha[origem]--;

    estado->compartimento[destino][estado->alturaPilha[destino]] = bloco;
    estado->alturaPilha[destino]++;

    /*  Se modo == 1, tirar o bloco do conjunto de bloqueantes */
    if (modo == 1)
    {
        estado->qtdBloqueantes--;
        estado->bloqueanteAlvo[estado->qtdBloqueantes] = 0;
    }
    else
    {
        estado->bloqueanteAlvo[estado->qtdBloqueantes] = bloco;
        estado->qtdBloqueantes++;
    }
}
/*
Estado removeAlvo(Estado antigo, int pilhas, int altura)
{
    Estado novo;
    novo.estado = AlocaMatriz(pilhas, altura);
    novo.bloqueantes = AlocaVetor(altura - 1);
    for (int i = 0; i < altura - 1; i++)
    {
        novo.bloqueantes[i] = 0;
    }

    novo.alvo = antigo.alvo + 1;
    for (int i = 0; i < pilhas; i++)
    {
        for (int j = 0; j < altura; j++)
        {
            novo.estado[i][j] = antigo.estado[i][j];
            if (antigo.estado[i][j] == antigo.alvo)
            {
                novo.estado[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < pilhas; i++)
    {
        for (int j = 0; j < altura; j++)
        {
            if (novo.estado[i][j] == novo.alvo)
            {
                novo.pilha = i; //atualiza pilha do novo alvo
                for (int k = j + 1, l = 0; (novo.estado[i][k] != 0 && k < altura); k++, l++)
                {
                    novo.bloqueantes[l] = novo.estado[i][k];
                }
            }
        }
    }

    return novo;
}

//Não cria novo estado
void removeAlvo2(Estado* estado, int pilhas, int altura)
{
    for (int i = 0; i < altura - 1; i++)
    {
        estado->bloqueantes[i] = 0;
    }
    for (int i = 0; i < altura; i++)
    {
         if (estado->estado[estado->pilha][i] == estado->alvo)
        {
             estado->estado[estado->pilha][i] = 0;
        }
    }

    estado->alvo = estado->alvo + 1;
    int stop = 0;

    for (int i = 0; i < pilhas; i++)
    {
        if(stop == 1)
            break;
        for (int j = 0; j < altura; j++)
        {
            if (estado->estado[i][j] == estado->alvo)
            {
                estado->pilha = i; //atualiza pilha do novo alvo
                for (int k = j + 1, l = 0; (estado->estado[i][k] != 0 && k < altura); k++, l++)
                {
                    estado->bloqueantes[l] = estado->estado[i][k];
                }
                break;
            }
        }
    }

}

void devolveAlvo(Estado* estado, int pilhaAlvo, int pilhas, int altura){
    estado->alvo = estado->alvo - 1;
    estado->pilha = pilhaAlvo;
    for (int i = 0; i < altura - 1; i++)
    {
        estado->bloqueantes[i] = 0;
    }
    for (int j = 0; j < altura; j++) {
        if (estado->estado[pilhaAlvo][j] == 0)
        {
            estado->estado[pilhaAlvo][j] = estado->alvo;
            break;
        }
    }
}

Estado realocaBloqueante(Estado antigo, int destino, int pilhas, int altura)
{
    Estado novo;
    novo.alvo = antigo.alvo;
    novo.pilha = antigo.pilha;
    novo.estado = AlocaMatriz(pilhas, altura);
    novo.bloqueantes = AlocaVetor(altura - 1);

    int realocado = 0;

    for (int i = 0; i < altura - 1; i++)
    {
        novo.bloqueantes[i] = antigo.bloqueantes[i];
    }

    for (int i = 0; i < altura - 1; i++)
    {
        if (novo.bloqueantes[i] == 0)
        {
            realocado = novo.bloqueantes[i - 1];
            novo.bloqueantes[i - 1] = 0;
            break;
        }
        if (i == altura - 2)
        {
            realocado = novo.bloqueantes[i];
            novo.bloqueantes[i] = 0;
            break;
        }
    }

    for (int i = 0; i < pilhas; i++)
    {
        for (int j = 0; j < altura; j++)
        {
            if (antigo.estado[i][j] == realocado)
            {
                novo.estado[i][j] = 0;
            }
            else
            {
                novo.estado[i][j] = antigo.estado[i][j];
            }
        }
    }
    for (int i = 0; i < altura; i++)
    {
        if (novo.estado[destino][i] == 0)
        {
            novo.estado[destino][i] = realocado;
            break;
        }
    }

    return novo;
}

//Não cria novo estado, 1 para realocar bloqueante, 0 para retornar
void realocaBloco(Estado* estado, int origem, int destino, int pilhas, int altura, int modo)
{
    int realocado = 0;
    //remove da origem
    for (int i = 0; i < altura; i++)
    {
        if (estado->estado[origem][i] == 0)
        {
            realocado = estado->estado[origem][i - 1];
            estado->estado[origem][i-1] = 0;
            break;
        }
        if (i == altura - 1)
        {
            realocado = estado->estado[origem][i] = realocado;
            estado->estado[origem][i] = 0;
            break;
        }
    }
    for (int j = 0; j < altura; j++)
    {
        if (estado->estado[destino][j] == 0)
        {
            estado->estado[destino][j] = realocado;
            break;
        }
    }

    if(modo == 1) {
        for (int k = 0; k < altura - 1; k++)
        {
            if (estado->bloqueantes[k] == realocado)
            {
                estado->bloqueantes[k] = 0;
                break;
            }
        }   
    }
    else {
        for (int k = 0; k < altura - 1; k++)
        {
            if (estado->bloqueantes[k] == 0)
            {
                estado->bloqueantes[k] = realocado;
                break;
            }
        }
    }

}

EstadoB removeAlvoB(EstadoB antigo, int pilhas, int altura) //retira alvo quando livre
{
    EstadoB novo;
    novo.estado = AlocaMatriz(pilhas, altura);
    novo.bloqueantes = AlocaVetor(altura - 1);
    for (int i = 0; i < altura - 1; i++)
    {
        novo.bloqueantes[i] = 0;
    }
    novo.alvo = antigo.alvo + 1;
    for (int i = 0; i < pilhas; i++)
    {
        for (int j = 0; j < altura; j++)
        {
            novo.estado[i][j] = antigo.estado[i][j];
            if (antigo.estado[i][j] == antigo.alvo)
            {
                novo.estado[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < pilhas; i++)
    {
        for (int j = 0; j < altura; j++)
        {
            if (novo.estado[i][j] == novo.alvo)
            {
                novo.pilha = i; //atualiza pilha do novo alvo
                for (int k = j + 1, l = 0; (novo.estado[i][k] != 0 && k < altura); k++, l++)
                {
                    novo.bloqueantes[l] = novo.estado[i][k];
                }
            }
        }
    }
    novo.lowerBound = calculaLowerBound(novo, pilhas, altura);
    return novo;
}

EstadoB realocaBloqueanteB(EstadoB antigo, int destino, int pilhas, int altura)
{
    EstadoB novo;
    novo.estado = AlocaMatriz(pilhas, altura);
    novo.bloqueantes = AlocaVetor(altura - 1);
    novo.alvo = antigo.alvo;
    novo.pilha = antigo.pilha;
    int realocado = 0;

    for (int i = 0; i < altura - 1; i++)
    {
        novo.bloqueantes[i] = antigo.bloqueantes[i];
    }

    for (int i = 0; i < altura - 1; i++)
    {
        if (novo.bloqueantes[i] == 0)
        {
            realocado = novo.bloqueantes[i - 1];
            novo.bloqueantes[i - 1] = 0;
            break;
        }
        if (i == altura - 2)
        {
            realocado = novo.bloqueantes[i];
            novo.bloqueantes[i] = 0;
            break;
        }
    }
    for (int i = 0; i < pilhas; i++)
    {
        for (int j = 0; j < altura; j++)
        {
            if (antigo.estado[i][j] == realocado)
            {
                novo.estado[i][j] = 0;
            }
            else
            {
                novo.estado[i][j] = antigo.estado[i][j];
            }
        }
    }
    for (int i = 0; i < altura; i++)
    {
        if (novo.estado[destino][i] == 0)
        {
            novo.estado[destino][i] = realocado;
            break;
        }
    }
    return novo;
}

int calculaLowerBound(EstadoB estado, int pilhas, int altura)
{
    int menor;
    int lowerBound = 0;
    for (int i = 0; i < pilhas; i++)
    {
        menor = estado.estado[i][0];
        for (int j = 1; j < altura && estado.estado[i][0] != 0; j++)
        {
            if (estado.estado[i][j] > menor)
                lowerBound++;
            else
                menor = estado.estado[i][0];
        }
    }

    return lowerBound;
}
*/
