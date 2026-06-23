#ifndef UTILS_H
#define UTILS_H

#include "personagem.h"
#include "mapa.h"

#define COR_VERMELHO "\033[31m"
#define COR_VERDE    "\033[32m"
#define COR_AMARELO  "\033[33m"
#define COR_AZUL     "\033[34m"
#define COR_MAGENTA  "\033[35m"
#define COR_CIANO    "\033[36m"
#define COR_BRANCO   "\033[37m"
#define RESET        "\033[0m"

void limpar_tela();
void limpar_buffer();
int extrair_slot_do_caminho(const char *caminho);
void salvar_jogo(PERSONAGEM *jogador, MAPA *mapa, const char *caminho);
int carregar_jogo(PERSONAGEM *jogador, MAPA *mapa, const char *caminho);
int tela_pause(PERSONAGEM *jogador, MAPA *mapa, const char *caminho);
void limpar_estados_slot(int slot);

#endif //UTILS_H