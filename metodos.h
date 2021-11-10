#ifndef METODOS_H
#define METODOS_H

#include "estruturas.h"
#include "movimentos.h"

int primeiroBloqueante(Estado estado, int altura);
int primeiroBloqueanteF(Estado *estado);
int calcularMinP(Estado estado, int pilha, int altura, int containers);
int calcularMinPF(Estado *estado, int pilha);
int calcularMinPF2(Estado *estado, int pilha);
int acharDestino(Estado *estado);
void minMax(Estado *estado, Solucao *sol, int *vetorM);
int estado_vazio(Estado *estado);
void calculaLowerBound(Estado *estado);
void calculaLowerBound2(Estado *estado);
void calculaLowerBound3(Estado *estado);
void copiaEstado(Estado *estado, Estado *novoEstado);
#endif