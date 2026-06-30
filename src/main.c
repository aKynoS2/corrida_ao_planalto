#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "personagem.h"
#include "mapa.h"
#include "combate.h"
#include "utils.h"
#include "inimigos.h"
#include "itens.h"
#include "menus.h"

#ifdef _WIN32
    #include <windows.h>
#endif

static int tratar_menu_principal(char *caminho, PERSONAGEM *jogador, MAPA *mapa) {
    int resultado = menu_principal(caminho);

    if (resultado == MENU_SAIR) {
        if (menu_confirmar_saida() != 1) return MENU_SAIR;
        return MENU_SAIR;
    }

    if (resultado == MENU_GAME_OVER) {
        tela_game_over();
        return MENU_GAME_OVER;
    }

    if (resultado == MENU_NOVO_JOGO) {
        int slot = extrair_slot_do_caminho(caminho);
        FILE *teste = fopen(caminho, "rb");
        if (teste != NULL) {
            fclose(teste);
            if (menu_confirmar_sobrescrita(slot) != 1) return MENU_NOVO_JOGO;
        }
        iniciar_nova_partida(jogador, mapa, caminho, slot);
        return MENU_NOVO_JOGO;
    }

    if (resultado == MENU_CARREGAR) {
        if (carregar_jogo(jogador, mapa, caminho) == 0) {
            printf(COR_VERMELHO "\n  Save vazio, escolha outro slot.\n" RESET);
            getchar();
            return MENU_CARREGAR;
        }
        return MENU_CARREGAR;
    }

    return MENU_SAIR;
}

static int tratar_resultado_jogo(char *caminho, PERSONAGEM *jogador, MAPA *mapa) {
    int resultado_jogo = rodar_jogo(jogador, mapa, caminho);

    if (resultado_jogo == MENU_GAME_OVER) {
        int opcao_game_over = tela_game_over();
        if (opcao_game_over == MENU_NOVO_JOGO) {
            int slot = extrair_slot_do_caminho(caminho);
            iniciar_nova_partida(jogador, mapa, caminho, slot);
            return MENU_NOVO_JOGO;
        }
        return MENU_SAIR;
    }

    if (resultado_jogo == MENU_SAIR) {
        return MENU_SAIR;
    }

    return MENU_NOVO_JOGO;
}

int main() {
    
    // ============================================================
    // define terminal do windows para UTF-8
    // ============================================================

    #ifdef _WIN32
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
    #endif

    // ============================================================
    // inicialização de variáveis
    // ============================================================
    
    MAPA mapa;
    PERSONAGEM jogador;
    char caminho[50] = {0};

    inicializar_mapa(&mapa);
    inicializar_jogador(&jogador);

    // ============================================================
    // gera seed aleatória
    // ============================================================
    
    srand(time(NULL));

    // ============================================================
    // loop principal 
    // ============================================================
    
    // O fluxo do jogo é mantido em um loop infinito para permitir
    // voltar ao menu após terminar uma partida, carregar um save ou
    // iniciar um novo jogo sem reiniciar o programa.
    while (1) {
        int resultado_menu = tratar_menu_principal(caminho, &jogador, &mapa);

        if (resultado_menu == MENU_SAIR) {
            return 0;
        }

        if (resultado_menu == MENU_GAME_OVER) {
            continue;
        }

        while (1) {
            int resultado_jogo = tratar_resultado_jogo(caminho, &jogador, &mapa);
            if (resultado_jogo == MENU_NOVO_JOGO || resultado_jogo == MENU_CARREGAR) {
                continue;
            }
            break;
        }
    }
}