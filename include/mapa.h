#ifndef MAPA_H
#define MAPA_H

#define VIEWPORT_W 20
#define VIEWPORT_H 20

typedef struct {
    char simbolo[5];
    int transitavel;
    int tem_inimigo;
    int tem_bau;
    int tem_saida;
} CELULA;

typedef struct {
    CELULA grid[20][20];
    int largura;
    int altura;
    int jogador_x;
    int jogador_y;
} MAPA;

void imprimir_mapa(MAPA *mapa);
void carregar_mapa(MAPA *mapa, const char *caminho_arquivo);
// DEFINICAO_SIMBOLO* buscar_simbolo(char s);

#endif // MAPA_H