#include <stdio.h>
#include <string.h>
#include "../include/mapa.h"
#include "../include/simbolos.h"

void imprimir_mapa(MAPA *mapa) {
    int cam_x = mapa->jogador_x - (VIEWPORT_W / 2);
    int cam_y = mapa->jogador_y - (VIEWPORT_H / 2);
    if (cam_x < 0) cam_x = 0;
    if (cam_y < 0) cam_y = 0;
    if (cam_x + VIEWPORT_W > mapa->largura) cam_x = mapa->largura - VIEWPORT_W;
    if (cam_y + VIEWPORT_H > mapa->altura)  cam_y = mapa->altura - VIEWPORT_H;

    for (int y = cam_y; y < cam_y + VIEWPORT_H; y++) {
        for (int x = cam_x; x < cam_x + VIEWPORT_W; x++) {
            if (x == mapa->jogador_x && y == mapa->jogador_y) {
                printf("@");
            } else {
                printf("%s", mapa->grid[y][x].simbolo);
            }
        }
        printf("\n");
    }
}

// ============================================================
// Função de busca na tabela
// Retorna o ponteiro pra definição do símbolo, ou NULL se não encontrado
// ============================================================
// DEFINICAO_SIMBOLO* buscar_simbolo(char s) {
//     for (int i = 0; i < (int)NUM_SIMBOLOS; i++) {
//         if (tabela_simbolos[i].simbolo == s) {
//             return &tabela_simbolos[i];
//         }
//     }
//     return NULL;
// }