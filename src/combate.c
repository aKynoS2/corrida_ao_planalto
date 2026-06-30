#include <stdio.h>
#include <string.h>
#include "../include/personagem.h"
#include "../include/combate.h"
#include "../include/mapa.h"
#include "../include/utils.h"

// ============================================================
// loop de combate
// ============================================================

static int executar_turno_jogador(PERSONAGEM *jogador, INIMIGO *inimigo) {
    int acao;
    if (scanf("%d", &acao) != 1) {
        limpar_buffer();
        adicionar_mensagem("Entrada inválida. Tente novamente.");
        getchar();
        return 0;
    }
    limpar_buffer();

    switch (acao) {
        case 1: {
            int dano = jogador->ataque - inimigo->defesa;
            if (dano < 0) dano = 0;
                inimigo->hp -= dano;
        
                char buf[TAM_MENSAGEM];
                snprintf(buf, sizeof(buf), "Você causou %d de dano!", dano);
                adicionar_mensagem(buf);
                return 1;
                break;
        }
            
        case 2: {
            imprimir_tela_inventario(jogador);
            
            char buf[TAM_MENSAGEM];
            snprintf(buf, sizeof(buf), "Selecione um item para usar (1-%d): ", jogador->num_itens);
            adicionar_mensagem(buf);
    
            int indice = 0;
            if (scanf("%d", &indice) != 1) {
                limpar_buffer();
                adicionar_mensagem("Entrada inválida...");
                getchar();
                return 1;
            }
            usar_item(jogador, indice);
            break;
        }
            
        case 3: {
            adicionar_mensagem("Você fugiu!");
            getchar();
            return -1;
        }

        default: {
          adicionar_mensagem("Opção inválida...");
          return 0;
        }
    }
    return 0;
}

static void executar_turno_inimigo(PERSONAGEM *jogador, INIMIGO *inimigo) {
    int dano = inimigo->ataque - jogador->defesa;
    if (dano < 0) dano = 0;
    jogador->hp -= dano;

    char buf[TAM_MENSAGEM];
    snprintf(buf, sizeof(buf), "O inimigo atacou! Você perdeu %d de HP.", dano);
    adicionar_mensagem(buf);
}

static int finalizar_combate(PERSONAGEM *jogador, INIMIGO *inimigo) {
    if (jogador->hp <= 0) {
        limpar_tela();
        printf(COR_VERMELHO "\n  Você morreu!\n" RESET);
        printf("  Pressione Enter para continuar...\n");
        getchar();
        return MENU_GAME_OVER;
    }

    if (inimigo->hp <= 0) {
        adicionar_mensagem("Você venceu!");
        jogador->exp += inimigo->xp_recompensa;

        char buf[TAM_MENSAGEM];
        snprintf(buf, sizeof(buf), "Você ganhou %d de experiência!", inimigo->xp_recompensa);
        adicionar_mensagem(buf);

        verificar_level_up(jogador);
        getchar();
        return 0;
    }

    return 0;
}

void imprimir_tela_combate(PERSONAGEM *jogador, INIMIGO *inimigo) {
    limpar_tela();

    printf("=========================================\n");
    printf("      COMBATE — %s\n", inimigo->nome);
    printf("=========================================\n");
    printf("\n");
    printf("  HP Inimigo: %d/%d\n", inimigo->hp, inimigo->hp_max);
    printf("\n");
    printf("-----------------------------------------\n");
    printf("\n");
    printf("  HP: %d/%d | Ataque: %d | Defesa: %d\n", 
    jogador->hp, jogador->hp_max, jogador->ataque, jogador->defesa);
    printf("\n");
    printf("=========================================\n");
    printf("  > %s\n", mensagens[0]);
    printf("  > %s\n", mensagens[1]);
    printf("  > %s\n", mensagens[2]);
    printf("=========================================\n");
    printf("\n");
    printf("  1. Atacar  2. Usar item  3. Fugir\n");
    printf("\n");
    printf("-----------------------------------------\n");
    printf("\n");
    printf("  Escolha: ");
}

int combate(PERSONAGEM *jogador, INIMIGO *inimigo) {
    // O combate segue o padrão turno a turno: o jogador escolhe uma ação,
    // depois o inimigo reage, até que um dos dois fique sem vida.
    while (jogador->hp > 0 && inimigo->hp > 0) {

        imprimir_tela_combate(jogador, inimigo);
        int resultado_turno = executar_turno_jogador(jogador, inimigo);
        if (resultado_turno == -1) {
            break;
        }

        if (resultado_turno == 0) {
            continue;
        }

        executar_turno_inimigo(jogador, inimigo);
    }

    return finalizar_combate(jogador, inimigo);
}
