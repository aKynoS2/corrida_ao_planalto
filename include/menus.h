#ifndef MENUS_H
#define MENUS_H

#include "personagem.h"
#include "mapa.h"

// ============================================================
// Menus e telas do jogo
// ============================================================
int menu_principal(char *caminho_out);
int menu_slot(int modo);
int menu_confirmar_sobrescrita(int slot);
int menu_confirmar_saida(void);
int tela_game_over(void);
int tela_pause(PERSONAGEM *jogador, MAPA *mapa, const char *caminho);

#endif // MENUS_H
