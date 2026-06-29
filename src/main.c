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

#ifdef _WIN32
    #include <windows.h>
#endif

int main() {
    #ifdef _WIN32
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
    #endif

    MAPA mapa;
    mapa.grid = NULL;
    mapa.jogador_x = 3;
    mapa.jogador_y = 3;

    PERSONAGEM jogador;
    jogador.nome[0] = '\0';
    jogador.hp_max    = 20;
    jogador.hp        = 20;
    jogador.mana_max  = 30;
    jogador.mana      = 30;
    jogador.ataque    = 8;
    jogador.defesa    = 3;
    jogador.num_itens = 0;
    jogador.nivel     = 1;
    jogador.exp       = 0;

    char caminho[50];

    srand(time(NULL));

    while (1) {
        int resultado = menu_principal(caminho);
    
        if (resultado == MENU_SAIR) return 0;
    
        if (resultado == MENU_NOVO_JOGO) {
            int slot = extrair_slot_do_caminho(caminho);
            FILE *teste = fopen(caminho, "rb");
            if (teste != NULL) {
                fclose(teste);
                if (menu_confirmar_sobrescrita(slot) != 1) continue;
                limpar_estados_slot(slot);
            }
            carregar_mapa_com_estado(&mapa, "data/maps/mapa_norte.txt", slot);
        }
    
        if (resultado == MENU_CARREGAR) {
            if (carregar_jogo(&jogador, &mapa, caminho) == 0) {
                printf(COR_VERMELHO "\n  Save vazio, escolha outro slot.\n" RESET);
                getchar();
                continue;
            }
        }
    
        rodar_jogo(&jogador, &mapa, caminho);
        // rodar_jogo retorna quando jogador volta pro menu — loop continua
    }
}