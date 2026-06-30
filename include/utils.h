#ifndef UTILS_H
#define UTILS_H

#include "personagem.h"
#include "mapa.h"

// ============================================================
// Definição buffer mensagens
// ============================================================
#define MAX_MENSAGENS 3
#define TAM_MENSAGEM 80

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
#define MENU_GAME_OVER  4

// ============================================================
// Utilitários
// ============================================================
void limpar_tela();
void limpar_buffer();
int ler_opcao_menu(const char *mensagem);
int  extrair_slot_do_caminho(const char *caminho);

// ============================================================
// Save / Load
// ============================================================
void salvar_jogo(PERSONAGEM *jogador, MAPA *mapa, const char *caminho);
int  carregar_jogo(PERSONAGEM *jogador, MAPA *mapa, const char *caminho);
void limpar_estados_slot(int slot);
void inicializar_jogador(PERSONAGEM *jogador);
void inicializar_mapa(MAPA *mapa);
void iniciar_nova_partida(PERSONAGEM *jogador, MAPA *mapa, char *caminho, int slot);
int ler_info_slot(int slot, int *nivel, char *regiao);

// ============================================================
// Loop principal
// ============================================================
int rodar_jogo (PERSONAGEM *jogador, MAPA *mapa, const char *caminho);

// ============================================================
// Buffer mensagens e HUD
// ============================================================
extern char mensagens[MAX_MENSAGENS][TAM_MENSAGEM];
void adicionar_mensagem(const char *msg);
void imprimir_hud (PERSONAGEM *jogador, MAPA *mapa);

#endif // UTILS_H