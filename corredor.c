/*
Autor: Matheus Fellone dos Santos
Universidade Federal do ABC
Código implementado para projeto de graduação da computação

Implementação método corredor para o problema do BRP
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estruturas.h"
#include "movimentos.h"


int buscaCorredor(Estado *estado, int alfa, int lambda, clock_t start)
{
    int valor;
    if ((double)(clock() - start) / CLOCKS_PER_SEC > 600)
        return -1;
    //Se alvo é o ultimo conteiner a ser retirado
    if (estado->alvo == estado->qtdConteiners)
    {
        return 0;
    }
    //Se alvo esta livre
    if (estado->qtdBloqueantes == 0)
    {
        int pilhaAlvo = estado->pilhaAlvo;
        removeAlvo(estado);
        valor = buscaCorredor(estado, alfa, lambda, start);
        devolveAlvo(estado, pilhaAlvo);

        return valor;
    }
    else
    {
        int min = 0;
        int atual, i, j;
        int minPilha, maxPilha;
        int livres = 0;
        int viavel = 0;
        int novoAlfa = alfa, novoLambda = lambda;

        do
        {

            minPilha = ((estado->pilhaAlvo - novoAlfa) < 0) ? 0 : (estado->pilhaAlvo - novoAlfa);
            maxPilha = ((estado->pilhaAlvo + novoAlfa) > estado->qtdPilhas - 1) ? estado->qtdPilhas - 1 : (estado->pilhaAlvo + novoAlfa);
            novoLambda = (novoLambda > estado->alturaMax) ? estado->alturaMax : novoLambda;

            //conta espaços livres dentro do corredor
            livres = 0;
            for (i = minPilha; i <= maxPilha; i++)
            {
                if (i != estado->pilhaAlvo)
                {
                    for (j = estado->alturaPilha[i]; j < novoLambda; j++)
                    {
                        livres++;
                    }
                }
            }
            //Se não tiver livres o suficiente aumenta um parametro aleatorio do corredor
            if (livres >= estado->qtdBloqueantes)
                viavel = 1;
            else
            {
                if (rand() % 2)
                    novoAlfa++;
                else
                    novoLambda++;
            }

        } while (!viavel);

        //Verifica todas as realocações possíveis dentro do corredor
        for (i = minPilha; i <= maxPilha; i++)
        {
            if ((double)(clock() - start) / CLOCKS_PER_SEC > 600)
                return -1;
        
            if (i != estado->pilhaAlvo && estado->compartimento[i][novoLambda - 1] == 0)
            {
                realocaBloco(estado, estado->pilhaAlvo, i, 1);
                atual = buscaCorredor(estado, alfa, lambda, start);
                realocaBloco(estado, i, estado->pilhaAlvo, 0);
                if (estado->qtdConteiners - estado->alvo + 1 < estado->qtdPilhas && estado->compartimento[i][0] == 0)
                {
                    min = atual;
                    break;
                }
                if (atual < min || min == 0)
                {
                    min = atual;
                }
                
            }
        }
        return 1 + min;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int pilhas, altura, alturaAlvo, i, j;
    int conteiners = 0;
    int timeout = 0;
    int realoc = 0;
    int alfa;
    int lambda;
    Estado *estado;

    FILE *file, *file2;
    file = fopen(argv[1], "r");
    file2 = fopen(argv[2], "r");
    if (file == NULL || file2 == NULL)
    {
        printf("Error: can't open file to read\n");
    }
    else
    {

        fscanf(file, "%d", &pilhas);
        fscanf(file, "%d", &altura);
        fscanf(file2, "%d", &alfa);
        fscanf(file2, "%d", &lambda);

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
        fclose(file2);

        for (i = alturaAlvo + 1, j = 0; estado->compartimento[estado->pilhaAlvo][i] != 0; i++, j++)
        {
            estado->bloqueanteAlvo[j] = estado->compartimento[estado->pilhaAlvo][i];
            estado->qtdBloqueantes++;
        }

        clock_t start = clock();
        realoc = buscaCorredor(estado, alfa, lambda, start);
        clock_t end = clock();

        LiberaEstado(estado);

        if(realoc == -1) {
            printf("TIMEOUT\n");
        }
        else {
            printf("Elapsed: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
            printf("Relocations: %d\n", realoc);
        }
        
    }
    return 0;
}
