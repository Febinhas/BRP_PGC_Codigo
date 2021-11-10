/*
Autor: Matheus Fellone dos Santos
Universidade Federal do ABC
Código implementado para projeto de graduação da computação

Implementação método por pragramação dinâmica para o problema do BRP
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estruturas.h"
#include "movimentos.h"

int buscaCompleta(Estado *estado)
{
    int valor;
    if (estado->alvo == estado->qtdConteiners)
    {
        return 0;
    }
    if (estado->qtdBloqueantes == 0)
    {
        int pilhaAlvo = estado->pilhaAlvo;
        removeAlvo(estado);
        valor = buscaCompleta(estado);
        devolveAlvo(estado, pilhaAlvo);

        return valor;
    }
    else
    {
        int min = 0;
        int atual;
        int dest; 

        for (int i = 0; i < estado->qtdPilhas; i++)
        {
            if (i != estado->pilhaAlvo && estado->compartimento[i][estado->alturaMax - 1] == 0)
            {
                realocaBloco(estado, estado->pilhaAlvo, i, 1);
                atual = buscaCompleta(estado);
                realocaBloco(estado, i, estado->pilhaAlvo, 0);
                if (atual < min || min == 0)
                {
                    min = atual;
                }
            }
        }
        return (1 + min);
    }
    return 0;
}

int main(int argc, char *argv[])
{

    int pilhas, altura, alturaAlvo, i, j;
    int conteiners = 0;
    int realoc = 0;
    Estado *estado;

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

        for (i = alturaAlvo + 1, j = 0; estado->compartimento[estado->pilhaAlvo][i] != 0; i++, j++)
        {
            estado->bloqueanteAlvo[j] = estado->compartimento[estado->pilhaAlvo][i];
            estado->qtdBloqueantes++;
        }

        clock_t start = clock();
        realoc = buscaCompleta(estado);
        clock_t end = clock();

        LiberaEstado(estado);

        printf("Elapsed: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
        printf("Relocations: %d\n", realoc);
    }
    return 0;
}