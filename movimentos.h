#ifndef MOVIMENTOS_H
#define MOVIMENTOS_H
#include "estruturas.h"

void removeAlvo(Estado *estado);
void devolveAlvo(Estado *estado, int pilha);
void realocaBloco(Estado *estado, int origem, int destino, int modo);


/*Estado removeAlvo(Estado antigo, int pilhas, int altura);
void removeAlvo2(Estado* estado, int pilhas, int altura);
void devolveAlvo(Estado* estado, int pilhaAlvo, int pilhas, int altura);
Estado realocaBloqueante(Estado antigo, int destino, int pilhas, int altura);
void realocaBloco(Estado* estado, int origem, int destino, int pilhas, int altura, int modo);
EstadoB removeAlvoB(EstadoB antigo, int pilhas, int altura);
EstadoB realocaBloqueanteB(EstadoB antigo, int destino, int pilhas, int altura);
int calculaLowerBound(EstadoB estado, int pilhas, int altura);*/
#endif