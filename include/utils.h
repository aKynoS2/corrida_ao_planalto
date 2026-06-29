#ifndef UTILS_H
#define UTILS_H

#include "personagem.h"
#include "mapa.h"

// ============================================================
// Cores ANSI
// ============================================================
#define COR_VERMELHO      "\033[31m"
#define COR_VERDE         "\033[32m"
#define COR_AMARELO       "\033[33m"
#define COR_AZUL          "\033[34m"
#define COR_MAGENTA       "\033[35m"
#define COR_CIANO         "\033[36m"
#define COR_BRANCO        "\033[37m"
#define COR_CINZA         "\033[90m"
#define COR_CINZA_CLARO   "\033[37m"
#define COR_VERDE_CLARO   "\033[92m"
#define COR_AMARELO_BRI   "\033[93m"
#define COR_AZUL_BRI      "\033[94m"
#define COR_CIANO_BRI     "\033[96m"
#define RESET             "\033[0m"

// ============================================================
// Retornos do menu principal
// ============================================================
#define MENU_NOVO_JOGO  1
#define MENU_CARREGAR   2
#define MENU_SAIR       3

// ============================================================
// Utilitários
// ============================================================
void limpar_tela();
void limpar_buffer();
int  extrair_slot_do_caminho(const char *caminho);

// ============================================================
// Menus
// ============================================================
int menu_principal(char *caminho_out);
int menu_slot(int modo);
int menu_confirmar_sobrescrita(int slot);
int tela_pause(PERSONAGEM *jogador, MAPA *mapa, const char *caminho);

// ============================================================
// Save / Load
// ============================================================
void salvar_jogo(PERSONAGEM *jogador, MAPA *mapa, const char *caminho);
int  carregar_jogo(PERSONAGEM *jogador, MAPA *mapa, const char *caminho);
void limpar_estados_slot(int slot);

// ============================================================
// Loop principal
// ============================================================
int rodar_jogo (PERSONAGEM *jogador, MAPA *mapa, const char *caminho);

#endif // UTILS_H