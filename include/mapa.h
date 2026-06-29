#ifndef MAPA_H
#define MAPA_H

#include "simbolos.h"

#define VIEWPORT_W 40
#define VIEWPORT_H 20

#define CELULA_NORMAL    0
#define CELULA_TRANSICAO 1
#define CELULA_MORTE     2

typedef struct {
    int x, y;
    char destino_arquivo[50];
    int destino_x, destino_y;
} SAIDA;

typedef struct {
    char simbolo[2];
    int transitavel;
    int tem_inimigo;
    int tem_bau;
    int tem_saida;
} CELULA;

typedef struct {
    CELULA **grid;
    int largura;
    int altura;
    int jogador_x;
    int jogador_y;
    SAIDA saidas[10];
    int num_saidas;
    char mapa_atual[50];
} MAPA;

void imprimir_mapa(MAPA *mapa);
void carregar_mapa(MAPA *mapa, const char *caminho_arquivo);
DEFINICAO_SIMBOLO* buscar_simbolo(char s[2]);
void carregar_mapa_com_estado(MAPA *mapa, const char *caminho_txt, int slot);
void salvar_mapa_com_estado(MAPA *mapa, int slot);
const char* cor_do_simbolo(char simbolo[2]);
void mover_jogador(MAPA *mapa, int novo_x, int novo_y);
int verificar_celula(MAPA *mapa, PERSONAGEM *jogador, const char *caminho, int slot);

#endif // MAPA_H