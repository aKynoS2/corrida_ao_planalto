#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "personagem.h"
#include "mapa.h"

void limpar_tela() {
  #ifdef _WIN32
    system("cls");
  #else
    system("clear");
  #endif
}

void limpar_buffer() {
  while (getchar() != '\n');
}

int extrair_slot_do_caminho(const char *caminho) {
    const char *nome = strrchr(caminho, '/');
    if (nome) nome++;
    else nome = caminho;
    int slot = 0;
    sscanf(nome, "save%d", &slot);
    return slot;
}

// ============================================================
// ASCII art
// ============================================================

static void imprimir_logo_grande() {
    printf(COR_AMARELO_BRI
    " _______  _______  _______  _______ _________ ______   _______ \n"
    "(  ____ \\(  ___  )(  ____ )(  ____ )\\__   __/(  __  \\ (  ___  )\n"
    "| (    \\/| (   ) || (    )|| (    )|   ) (   | (  \\  )| (   ) |\n"
    "| |      | |   | || (____)|| (____)|   | |   | |   ) || (___) |\n"
    "| |      | |   | ||     __)|     __)   | |   | |   | ||  ___  |\n"
    "| |      | |   | || (\\ (   | (\\ (      | |   | |   ) || (   ) |\n"
    "| (____/\\| (___) || ) \\ \\__| ) \\ \\_____) (___| (__/  )| )   ( |\n"
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
// menu_principal — retorna slot escolhido (novo jogo) ou -slot (carregar)
// ============================================================
int menu_principal(char *caminho_out) {
    while (1) {
        limpar_tela();
        imprimir_logo_grande();
        printf(COR_CINZA "  AO PLANALTO" RESET " — uma campanha sem retorno\n");
        printf(COR_CINZA "\n  -----------------------------------------\n\n" RESET);
        printf("  " COR_AMARELO_BRI "[1]" RESET " Novo Jogo\n");
        printf("  " COR_AMARELO_BRI "[2]" RESET " Carregar Jogo\n");
        printf("  " COR_AMARELO_BRI "[3]" RESET " Sair\n");
        printf(COR_CINZA "\n  -----------------------------------------\n" RESET);
        printf("\n  Escolha: ");

        int opcao;
        scanf("%d", &opcao);
        limpar_buffer();

        if (opcao == 3) return MENU_SAIR;

        if (opcao == 1 || opcao == 2) {
            int slot = menu_slot(opcao);
            if (slot == 0) continue;
            sprintf(caminho_out, "data/saves/save%d.dat", slot);
            return (opcao == 1) ? MENU_NOVO_JOGO : MENU_CARREGAR;
        }
    }
}

// ============================================================
// menu_slot — seleciona slot 1-3, retorna 0 se voltar
// ============================================================
int menu_slot(int modo) {
    limpar_tela();
    imprimir_logo_grande();
    printf(COR_CINZA "  AO PLANALTO" RESET " — uma campanha sem retorno\n");
    printf(COR_CINZA "\n  -----------------------------------------\n\n" RESET);

    if (modo == 1)
        printf("  " COR_AMARELO_BRI "NOVO JOGO" RESET " — escolha um slot\n\n");
    else
        printf("  " COR_AMARELO_BRI "CARREGAR" RESET " — escolha um slot\n\n");

    printf("  " COR_AMARELO_BRI "[1]" RESET " Slot 1\n");
    printf("  " COR_AMARELO_BRI "[2]" RESET " Slot 2\n");
    printf("  " COR_AMARELO_BRI "[3]" RESET " Slot 3\n");
    printf("  " COR_CINZA "[0]" RESET " Voltar\n");
    printf(COR_CINZA "\n  -----------------------------------------\n" RESET);
    printf("\n  Escolha: ");

    int slot;
    scanf("%d", &slot);
    limpar_buffer();

    if (slot < 0 || slot > 3) return 0;
    return slot;
}

// ============================================================
// menu_confirmar_sobrescrita — retorna 1 se confirmou, 0 se cancelou
// ============================================================
int menu_confirmar_sobrescrita(int slot) {
    limpar_tela();
    imprimir_logo_grande();
    printf(COR_CINZA "  AO PLANALTO" RESET " — uma campanha sem retorno\n");
    printf(COR_CINZA "\n  -----------------------------------------\n\n" RESET);
    printf("  " COR_VERMELHO "Slot %d já possui um save salvo." RESET "\n", slot);
    printf("  Deseja sobrescrever?\n\n");
    printf("  " COR_AMARELO_BRI "[1]" RESET " Sim, sobrescrever\n");
    printf("  " COR_CINZA "[2]" RESET " Cancelar\n");
    printf(COR_CINZA "\n  -----------------------------------------\n" RESET);
    printf("\n  Escolha: ");

    int confirma;
    scanf("%d", &confirma);
    limpar_buffer();
    return (confirma == 1) ? 1 : 0;
}

// ============================================================
// tela_pause — retorna 0 para continuar, 1 para sair ao menu
// ============================================================
int tela_pause(PERSONAGEM *jogador, MAPA *mapa, const char *caminho) {
    limpar_tela();
    imprimir_logo_pequeno();
    printf(COR_CINZA "  AO PLANALTO" RESET " — uma campanha sem retorno\n");

    // Extrai nome da região do caminho do mapa atual
    const char *nome_regiao = strrchr(mapa->mapa_atual, '_');
    char regiao[30] = "Desconhecida";
    if (nome_regiao) {
        nome_regiao++;
        strncpy(regiao, nome_regiao, sizeof(regiao) - 1);
        // Remove ".txt"
        char *ponto = strrchr(regiao, '.');
        if (ponto) *ponto = '\0';
        // Capitaliza primeira letra
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
    printf("\n  Escolha: ");

    int opcao;
    scanf("%d", &opcao);
    limpar_buffer();

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

// ============================================================
// salvar / carregar / utilitários
// ============================================================

void salvar_jogo(PERSONAGEM *jogador, MAPA *mapa, const char *caminho) {
    FILE *arquivo = fopen(caminho, "wb");
    if (arquivo == NULL) { printf("Erro ao salvar...\n"); return; }
    fwrite(jogador, sizeof(PERSONAGEM), 1, arquivo);
    fwrite(&mapa->largura,    sizeof(int),  1, arquivo);
    fwrite(&mapa->altura,     sizeof(int),  1, arquivo);
    fwrite(&mapa->jogador_x,  sizeof(int),  1, arquivo);
    fwrite(&mapa->jogador_y,  sizeof(int),  1, arquivo);
    fwrite(&mapa->num_saidas, sizeof(int),  1, arquivo);
    fwrite(mapa->saidas,      sizeof(SAIDA), mapa->num_saidas, arquivo);
    fwrite(mapa->mapa_atual,  sizeof(char), 50, arquivo);
    for (int y = 0; y < mapa->altura; y++)
        fwrite(mapa->grid[y], sizeof(CELULA), mapa->largura, arquivo);
    fclose(arquivo);
}

int carregar_jogo(PERSONAGEM *jogador, MAPA *mapa, const char *caminho) {
    FILE *arquivo = fopen(caminho, "rb");
    if (arquivo == NULL) { printf("Save vazio...\n"); return 0; }
    fread(jogador,           sizeof(PERSONAGEM), 1, arquivo);
    fread(&mapa->largura,    sizeof(int), 1, arquivo);
    fread(&mapa->altura,     sizeof(int), 1, arquivo);
    fread(&mapa->jogador_x,  sizeof(int), 1, arquivo);
    fread(&mapa->jogador_y,  sizeof(int), 1, arquivo);
    fread(&mapa->num_saidas, sizeof(int), 1, arquivo);
    fread(mapa->saidas,      sizeof(SAIDA), mapa->num_saidas, arquivo);
    fread(mapa->mapa_atual,  sizeof(char), 50, arquivo);
    if (mapa->grid != NULL) {
        for (int y = 0; y < mapa->altura; y++) free(mapa->grid[y]);
        free(mapa->grid);
        mapa->grid = NULL;
    }
    mapa->grid = malloc(mapa->altura * sizeof(CELULA *));
    for (int y = 0; y < mapa->altura; y++) {
        mapa->grid[y] = malloc(mapa->largura * sizeof(CELULA));
        fread(mapa->grid[y], sizeof(CELULA), mapa->largura, arquivo);
    }
    fclose(arquivo);
    return 1;
}

void limpar_estados_slot(int slot) {
    char estado[100];
    const char *mapas[] = {
        "mapa_norte.txt", "mapa_nordeste.txt", "mapa_centro_oeste.txt",
        "mapa_sudeste.txt", "mapa_sul.txt", "mapa_brasilia.txt"
    };
    for (int i = 0; i < 6; i++) {
        sprintf(estado, "data/saves/slot%d_%s.state", slot, mapas[i]);
        remove(estado);
    }
}

int rodar_jogo (PERSONAGEM *jogador, MAPA *mapa, const char *caminho) {
    while (1) {
        limpar_tela();

        int novo_x = mapa->jogador_x;
        int novo_y = mapa->jogador_y;

        imprimir_mapa(mapa);

        char tecla = getchar();
        limpar_buffer();

        if      (tecla == 'w') novo_y--;
        else if (tecla == 's') novo_y++;
        else if (tecla == 'a') novo_x--;
        else if (tecla == 'd') novo_x++;
        else if (tecla == 'i') {
            mostrar_inventario(jogador);
            if (jogador->num_itens == 0) {
                printf("Voce nao tem itens para usar!\n");
                getchar();
                continue;
            }
            printf("Selecione um item para usar (1-%d): ", jogador->num_itens);
            int indice;
            scanf("%d", &indice);
            getchar();
            usar_item(jogador, indice);
            continue;
        }
        else if (tecla == 'p') {
            if (tela_pause(jogador, mapa, caminho) == 1) return MENU_SAIR;
            continue;
        }

        mover_jogador(mapa, novo_x, novo_y);

        int slot = extrair_slot_do_caminho(caminho);
        if (verificar_celula(mapa, jogador, caminho, slot) == CELULA_TRANSICAO)
            continue;
    }
}
