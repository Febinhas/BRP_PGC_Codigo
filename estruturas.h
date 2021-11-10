#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

typedef struct state
{
    int qtdPilhas;
    int alturaMax;
    int qtdConteiners;
    int **compartimento;
    int *alturaPilha;
    int alvo;
    int pilhaAlvo;
    int *bloqueanteAlvo;
    int qtdBloqueantes;
    int lowerBound;
} Estado;

typedef struct move
{
    int bloco;
    int destino; // destino 0 para retirar o bloco
    int numRealoc;
    int alvo;
    struct move *proximo;
} Move;

typedef struct solucao
{
    Move *inicio, *fim;
    int realoc;
} Solucao;

typedef struct filhos
{
    Estado *filho;
    struct filhos *proximo;
} Filho;

typedef struct listaFilhos
{
    Filho *inicio, *fim;
} listaFilhos;

Estado *CriaEstado(int qtdPilhas, int alturaMax);
int *aloca_vetor(int n);
void ImprimeEstado(Estado *estado);
void LiberaEstado(Estado *estado);
Solucao *criar_solucao();
listaFilhos *criar_filhos();
void inserirMovimento(Solucao *sol, int bloco, int destino, int numRealoc, int alvo);
void imprimir_sol(Solucao *solucao);
void liberar_solucao(Move *inicio);
void liberar_filhos(Filho *inicio);
Estado *CriaFilho(Estado *estadoPai);
#endif