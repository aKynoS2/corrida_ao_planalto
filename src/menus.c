#include <stdio.h>
#include <string.h>
#include "menus.h"
#include "utils.h"
#include "mapa.h"
#include "personagem.h"

// ============================================================
// ASCII art
// ============================================================

static void imprimir_logo_grande(void);
static void imprimir_logo_pequeno(void);

static void imprimir_cabecalho(const char *titulo, int usar_logo_grande) {
    if (usar_logo_grande) {
        imprimir_logo_grande();
    } else {
        imprimir_logo_pequeno();
    }

    printf(COR_CINZA "  AO PLANALTO" RESET " — uma campanha sem retorno\n");
    printf(COR_CINZA "\n  -----------------------------------------\n\n" RESET);

    if (titulo != NULL) {
        printf("  %s\n", titulo);
    }
}

static void imprimir_logo_grande() {
    printf(COR_AMARELO_BRI
    " _______  _______  _______  _______ _________ ______   _______ \n"
    "(  ____ \\(  ___  )(  ____ )(  ____ )\\__   __/(  __  \\ (  ___  )\n"
    "| (    \\/| (   ) || (    )|| (    )|   ) (   | (  \\  )| (   ) |\n"
    "| |      | |   | || (____)|| (____)|   | |   | |   ) || (___) |\n"
    "| |      | |   | ||     __)|     __)   | |   | |   | ||  ___  |\n"
    "| |      | |   | || (\\ (   | (\\ (      | |   | |   ) || (   ) |\n"
    "| (____/\\| (___) || ) \\__| ) \\_____) (___| (__/  )| )   ( |\n"
    "(_______/(_______)|/   \\__/|/   \\__/\\_______/(______/ |/     \\|\n"
    RESET);
}

static void imprimir_logo_pequeno() {
    printf(COR_AMARELO_BRI
    "                           _     __       \n"
    "     _________  __________(_)___/ /___ _\n"
    "    / ___/ __ \\/ ___/ ___/ / __  / __ `/\n"
    "   / /__/ /_/ / /  / /  / / /_/ / /_/ / \n"
    "   \\___/\\____/_/  /_/  /_/\\__,_/\\__,_/  \n"
    RESET);
}

// ============================================================
// menu_principal — controla o menu inicial do jogo
// ============================================================
int menu_principal(char *caminho_out) {
    while (1) {
        limpar_tela();
        imprimir_cabecalho(NULL, 1);
        printf("  " COR_AMARELO_BRI "[1]" RESET " Novo Jogo\n");
        printf("  " COR_AMARELO_BRI "[2]" RESET " Carregar Jogo\n");
        printf("  " COR_AMARELO_BRI "[3]" RESET " Sair\n");
        printf(COR_CINZA "\n  -----------------------------------------\n" RESET);

        int opcao = ler_opcao_menu("\n  Escolha: ");
        if (opcao == -1) continue;

        if (opcao == 3) return MENU_SAIR;

        if (opcao == 1 || opcao == 2) {
            int slot = menu_slot(opcao);
            if (slot == 0) continue;
            snprintf(caminho_out, 50, "data/saves/save%d.dat", slot);
            return (opcao == 1) ? MENU_NOVO_JOGO : MENU_CARREGAR;
        }
    }
}

// ============================================================
// menu_slot — permite escolher um slot de save entre 1 e 3
// ============================================================
int menu_slot(int modo) {
    limpar_tela();
    imprimir_cabecalho(NULL, 1);

    if (modo == 1)
        printf("  " COR_AMARELO_BRI "NOVO JOGO" RESET " — escolha um slot\n\n");
    else
        printf("  " COR_AMARELO_BRI "CARREGAR" RESET " — escolha um slot\n\n");

    char label1[100], label2[100], label3[100];
    int nivel;
    char regiao[50];

    if (ler_info_slot(1, &nivel, regiao))
        snprintf(label1, sizeof(label1),
        "  " COR_AMARELO_BRI "[1]" RESET "Slot 1 — Nível %d | %s", nivel, regiao);
    else
        snprintf(label1, sizeof(label1),
        "  " COR_AMARELO_BRI "[1]" RESET "Slot 1 — Vazio");

    if (ler_info_slot(2, &nivel, regiao))
        snprintf(label2, sizeof(label2), 
        "  " COR_AMARELO_BRI "[2]" RESET "Slot 2 — Nível %d | %s", nivel, regiao);
    else
        snprintf(label2, sizeof(label2), 
        "  " COR_AMARELO_BRI "[2]" RESET "Slot 2 — Vazio");
    
    if (ler_info_slot(3, &nivel, regiao))
        snprintf(label3, sizeof(label3), 
        "  " COR_AMARELO_BRI "[3]" RESET "Slot 3 — Nível %d | %s", nivel, regiao);
    else
        snprintf(label3, sizeof(label3), 
        "  " COR_AMARELO_BRI "[3]" RESET "Slot 3 — Vazio");
    
    printf("%s\n", label1);
    printf("%s\n", label2);
    printf("%s\n", label3);
    printf("  " COR_CINZA "[0]" RESET " Voltar\n");
    printf(COR_CINZA "\n  -----------------------------------------\n" RESET);
    int slot = ler_opcao_menu("\n  Escolha: ");
    if (slot == -1) return 0;

    if (slot < 0 || slot > 3) return 0;
    return slot;
}

// ============================================================
// menu_confirmar_sobrescrita — pergunta antes de sobrescrever um save
// ============================================================
int menu_confirmar_sobrescrita(int slot) {
    limpar_tela();
    imprimir_cabecalho(NULL, 1);
    printf("  " COR_VERMELHO "Slot %d já possui um save salvo." RESET "\n", slot);
    printf("  Deseja sobrescrever?\n\n");
    printf("  " COR_AMARELO_BRI "[1]" RESET " Sim, sobrescrever\n");
    printf("  " COR_CINZA "[2]" RESET " Cancelar\n");
    printf(COR_CINZA "\n  -----------------------------------------\n" RESET);
    int confirma = ler_opcao_menu("\n  Escolha: ");
    if (confirma == -1) return 0;
    return (confirma == 1) ? 1 : 0;
}

int menu_confirmar_saida(void) {
    limpar_tela();
    imprimir_cabecalho(NULL, 1);
    printf("  " COR_VERMELHO "Deseja realmente sair do jogo?" RESET "\n\n");
    printf("  " COR_AMARELO_BRI "[1]" RESET " Sim, sair\n");
    printf("  " COR_CINZA "[2]" RESET " Cancelar\n");
    printf(COR_CINZA "\n  -----------------------------------------\n" RESET);
    int confirma = ler_opcao_menu("\n  Escolha: ");
    if (confirma == -1) return 0;
    return (confirma == 1) ? 1 : 0;
}

int tela_game_over(void) {
    limpar_tela();
    imprimir_cabecalho(NULL, 1);
    printf("  " COR_VERMELHO "GAME OVER" RESET "\n\n");
    printf("  Você foi derrotado.\n");
    printf("\n  [1] Voltar ao menu principal\n");
    printf("  [2] Tentar novamente\n");
    printf(COR_CINZA "\n  -----------------------------------------\n" RESET);

    int opcao = ler_opcao_menu("\n  Escolha: ");
    if (opcao == 2) {
        return MENU_NOVO_JOGO;
    }
    return 0;
}

// ============================================================
// tela_pause — exibe o menu de pausa durante a partida
// ============================================================
int tela_pause(PERSONAGEM *jogador, MAPA *mapa, const char *caminho) {
    limpar_tela();
    imprimir_cabecalho(NULL, 0);

    const char *nome_regiao = strrchr(mapa->mapa_atual, '_');
    char regiao[30] = "Desconhecida";
    if (nome_regiao) {
        nome_regiao++;
        strncpy(regiao, nome_regiao, sizeof(regiao) - 1);
        char *ponto = strrchr(regiao, '.');
        if (ponto) *ponto = '\0';
        if (regiao[0] >= 'a' && regiao[0] <= 'z') regiao[0] -= 32;
    }

    printf(COR_CINZA "\n  -----------------------------------------\n" RESET);
    printf("  Nivel " COR_AMARELO_BRI "%d" RESET
           "  ·  HP " COR_VERDE "%d" RESET "/" COR_CINZA "%d" RESET
           "  ·  " COR_CIANO "%s" RESET "\n",
           jogador->nivel, jogador->hp, jogador->hp_max, regiao);
    printf(COR_CINZA "  -----------------------------------------\n\n" RESET);

    printf("  " COR_AMARELO_BRI "[1]" RESET " Continuar\n");
    printf("  " COR_AMARELO_BRI "[2]" RESET " Salvar\n");
    printf("  " COR_AMARELO_BRI "[3]" RESET " Salvar e sair\n");
    printf(COR_CINZA "\n  -----------------------------------------\n" RESET);
    int opcao = ler_opcao_menu("\n  Escolha: ");
    if (opcao == -1) return 0;

    int slot = extrair_slot_do_caminho(caminho);

    switch (opcao) {
        case 2:
            salvar_jogo(jogador, mapa, caminho);
            salvar_mapa_com_estado(mapa, slot);
            printf(COR_VERDE "\n  Jogo salvo.\n" RESET);
            getchar();
            return 0;
        case 3:
            salvar_jogo(jogador, mapa, caminho);
            salvar_mapa_com_estado(mapa, slot);
            return 1;
        default:
            return 0;
    }
}
