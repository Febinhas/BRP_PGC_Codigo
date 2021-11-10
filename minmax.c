/*
Autor: Matheus Fellone dos Santos
Universidade Federal do ABC
Código implementado para projeto de graduação da computação

Implementação método MinMax para o problema do BRP
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estruturas.h"
#include "movimentos.h"
#include "metodos.h"


/*Código do MinMax se encontra na classe metodos.c*/

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
        minMax(estado, solucao, vetorM);
        clock_t end = clock();

        LiberaEstado(estado);

        // printf("Elapsed: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
        printf("%d\n", solucao->realoc);
        //imprimir_sol(solucao);

        liberar_solucao(solucao->inicio);
        free(solucao);
    }
    return 0;
}