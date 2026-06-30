#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "personagem.h"
#include "mapa.h"
#include "menus.h"

void limpar_tela() {
  // Limpa a tela do terminal para dar a sensação de uma nova tela de menu
  // ou de uma nova cena do jogo, dependendo do sistema operacional.
  #ifdef _WIN32
    system("cls");
  #else
    system("clear");
  #endif
}

void limpar_buffer() {
  // Consome o restante da entrada do teclado para evitar que o próximo scanf
  // leia caracteres residuais deixados pelo usuário.
  while (getchar() != '\n');
}

void inicializar_jogador(PERSONAGEM *jogador) {
    jogador->nome[0]            = '\0';
    jogador->hp_max             = 20;
    jogador->hp                 = 20;
    jogador->mana_max           = 30;
    jogador->mana               = 30;
    jogador->ataque             = 8;
    jogador->defesa             = 3;
    jogador->num_itens          = 0;
    jogador->nivel              = 1;
    jogador->exp                = 0;
    jogador->arma_equipada      = -1;
    jogador->armadura_equipada  = -1;
}

void inicializar_mapa(MAPA *mapa) {
    mapa->grid = NULL;
    mapa->largura = 0;
    mapa->altura = 0;
    mapa->jogador_x = 3;
    mapa->jogador_y = 3;
    mapa->num_saidas = 0;
    mapa->mapa_atual[0] = '\0';
}

void iniciar_nova_partida(PERSONAGEM *jogador, MAPA *mapa, char *caminho, int slot) {
    inicializar_jogador(jogador);
    inicializar_mapa(mapa);

    sprintf(caminho, "data/saves/save%d.dat", slot);
    limpar_estados_slot(slot);
    remove(caminho);
    carregar_mapa_com_estado(mapa, "data/maps/mapa_norte.txt", slot);
}

int ler_opcao_menu(const char *mensagem) {
    int valor = 0;
    printf("%s", mensagem);
    if (scanf("%d", &valor) != 1) {
        limpar_buffer();
        printf("\n  Entrada inválida. Tente novamente.\n");
        getchar();
        return -1;
    }
    limpar_buffer();
    return valor;
}

// ============================================================
// salvar / carregar 
// ============================================================

static int escrever_dados_salvos(FILE *arquivo, PERSONAGEM *jogador, MAPA *mapa) {
    if (fwrite(jogador, sizeof(PERSONAGEM), 1, arquivo) != 1 ||
        fwrite(&mapa->largura, sizeof(int), 1, arquivo) != 1 ||
        fwrite(&mapa->altura, sizeof(int), 1, arquivo) != 1 ||
        fwrite(&mapa->jogador_x, sizeof(int), 1, arquivo) != 1 ||
        fwrite(&mapa->jogador_y, sizeof(int), 1, arquivo) != 1 ||
        fwrite(&mapa->num_saidas, sizeof(int), 1, arquivo) != 1 ||
        fwrite(mapa->saidas, sizeof(SAIDA), mapa->num_saidas, arquivo) != (size_t)mapa->num_saidas ||
        fwrite(mapa->mapa_atual, sizeof(char), 50, arquivo) != 50) {
        return 0;
    }

    for (int y = 0; y < mapa->altura; y++) {
        if (fwrite(mapa->grid[y], sizeof(CELULA), mapa->largura, arquivo) != (size_t)mapa->largura) {
            return 0;
        }
    }

    return 1;
}

static int ler_dados_salvos(FILE *arquivo, PERSONAGEM *jogador, MAPA *mapa) {
    if (fread(jogador, sizeof(PERSONAGEM), 1, arquivo) != 1 ||
        fread(&mapa->largura, sizeof(int), 1, arquivo) != 1 ||
        fread(&mapa->altura, sizeof(int), 1, arquivo) != 1 ||
        fread(&mapa->jogador_x, sizeof(int), 1, arquivo) != 1 ||
        fread(&mapa->jogador_y, sizeof(int), 1, arquivo) != 1 ||
        fread(&mapa->num_saidas, sizeof(int), 1, arquivo) != 1) {
        return 0;
    }

    if (fread(mapa->saidas, sizeof(SAIDA), mapa->num_saidas, arquivo) != (size_t)mapa->num_saidas ||
        fread(mapa->mapa_atual, sizeof(char), 50, arquivo) != 50) {
        return 0;
    }

    return 1;
}

static int alocar_grade_mapa(MAPA *mapa) {
    if (mapa->grid != NULL) {
        for (int y = 0; y < mapa->altura; y++) free(mapa->grid[y]);
        free(mapa->grid);
        mapa->grid = NULL;
    }

    mapa->grid = malloc(mapa->altura * sizeof(CELULA *));
    if (mapa->grid == NULL) {
        return 0;
    }

    for (int y = 0; y < mapa->altura; y++) {
        mapa->grid[y] = malloc(mapa->largura * sizeof(CELULA));
        if (mapa->grid[y] == NULL) {
            for (int i = 0; i < y; i++) free(mapa->grid[i]);
            free(mapa->grid);
            mapa->grid = NULL;
            return 0;
        }
    }

    return 1;
}

static int ler_grade_mapa(FILE *arquivo, MAPA *mapa) {
    for (int y = 0; y < mapa->altura; y++) {
        if (fread(mapa->grid[y], sizeof(CELULA), mapa->largura, arquivo) != (size_t)mapa->largura) {
            return 0;
        }
    }
    return 1;
}

void salvar_jogo(PERSONAGEM *jogador, MAPA *mapa, const char *caminho) {
    // Salva o estado completo do personagem e do mapa em um arquivo binário
    // para que a partida possa ser retomada depois.
    FILE *arquivo = fopen(caminho, "wb");
    if (arquivo == NULL) {
        printf("Erro ao salvar o progresso em '%s'.\n", caminho);
        return;
    }

    if (!escrever_dados_salvos(arquivo, jogador, mapa)) {
        printf("Erro ao gravar o save.\n");
        fclose(arquivo);
        return;
    }

    fclose(arquivo);
}

int carregar_jogo(PERSONAGEM *jogador, MAPA *mapa, const char *caminho) {
    // Carrega os dados salvos e reconstrói a estrutura do mapa e do
    // personagem no estado em que foram deixados.
    FILE *arquivo = fopen(caminho, "rb");
    if (arquivo == NULL) {
        printf("Save vazio ou arquivo inexistente.\n");
        return 0;
    }

    if (!ler_dados_salvos(arquivo, jogador, mapa)) {
        printf("Erro ao ler o save.\n");
        fclose(arquivo);
        return 0;
    }

    if (!alocar_grade_mapa(mapa)) {
        printf("Erro ao reservar memória para o mapa carregado.\n");
        fclose(arquivo);
        return 0;
    }

    if (!ler_grade_mapa(arquivo, mapa)) {
        printf("Erro ao ler a grade do mapa.\n");
        for (int y = 0; y < mapa->altura; y++) {
            if (mapa->grid[y] != NULL) free(mapa->grid[y]);
        }
        free(mapa->grid);
        mapa->grid = NULL;
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);
    return 1;
}

void limpar_estados_slot(int slot) {
    // Remove os arquivos de estado do slot escolhido para garantir que um
    // novo jogo comece do zero, sem reaproveitar dados antigos da sessão.
    char estado[100];
    char save[100];
    const char *mapas[] = {
        "mapa_norte.txt", "mapa_nordeste.txt", "mapa_centro_oeste.txt",
        "mapa_sudeste.txt", "mapa_sul.txt", "mapa_brasilia.txt"
    };

    sprintf(save, "data/saves/save%d.dat", slot);
    remove(save);

    for (int i = 0; i < 6; i++) {
        sprintf(estado, "data/saves/slot%d_%s.state", slot, mapas[i]);
        remove(estado);
    }
}

int extrair_slot_do_caminho(const char *caminho) {
    // Extrai o número do slot a partir do nome do arquivo de save, por
    // exemplo "save2.dat" vira o número 2.
    const char *nome = strrchr(caminho, '/');
    if (nome) nome++;
    else nome = caminho;
    int slot = 0;
    sscanf(nome, "save%d", &slot);
    return slot;
}

int ler_info_slot(int slot, int *nivel, char *regiao) {
    char caminho[50];
    snprintf(caminho, sizeof(caminho), "data/saves/save%d.dat", slot);

    FILE *arquivo = fopen(caminho, "rb");
    if (arquivo == NULL) {
        return 0;
    }

    PERSONAGEM jogador_temp;
    if (fread(&jogador_temp, sizeof(PERSONAGEM), 1, arquivo) != 1) {
        fclose(arquivo);
        return 0;
    }
    *nivel = jogador_temp.nivel;

    int num_saidas;
    fseek(arquivo, sizeof(int) * 4, SEEK_CUR); // largura, altura, jogador_x, jogador_y
    fread(&num_saidas, sizeof(int), 1, arquivo); // lê num_saidas de verdade
    fseek(arquivo, sizeof(SAIDA) * num_saidas, SEEK_CUR); // pula só as saídas que existem

    char mapa_atual[50];
    if (fread(mapa_atual, sizeof(char), 50, arquivo) != 50) {
        fclose(arquivo);
        return 0;
    }

    const char *nome_regiao = strrchr(mapa_atual, '_');
    if (nome_regiao) {
        nome_regiao++;
        strncpy(regiao, nome_regiao, 49);
        regiao[49] = '\0';
        char *ponto = strrchr(regiao, '.');
        if (ponto) *ponto = '\0';
        if (regiao[0] >= 'a' && regiao[0] <= 'z') regiao[0] -= 32;
    }
    return 1;
}

// ============================================================
// loop principal
// ============================================================

static int processar_inventario(PERSONAGEM *jogador) {
    imprimir_tela_inventario(jogador);
    if (jogador->num_itens == 0) {
        adicionar_mensagem("Você não tem itens para usar!");
        getchar();
        return 1;
    }

    char buf[TAM_MENSAGEM];
    snprintf(buf, sizeof(buf), "Selecione um item para usar (1-%d): ", jogador->num_itens);
    adicionar_mensagem(buf);
    
    int indice = 0;
    if (scanf("%d", &indice) != 1) {
        limpar_buffer();
        printf("Entrada inválida.\n");
        getchar();
        return 1;
    }

    getchar();
    usar_item(jogador, indice);
    return 1;
}

static int processar_pausa(PERSONAGEM *jogador, MAPA *mapa, const char *caminho) {
    if (tela_pause(jogador, mapa, caminho) == 1) {
        return MENU_SAIR;
    }
    return 1;
}

static void atualizar_posicao(char tecla, int *novo_x, int *novo_y) {
    if (tecla == 'w') {
        (*novo_y)--;
    } else if (tecla == 's') {
        (*novo_y)++;
    } else if (tecla == 'a') {
        (*novo_x)--;
    } else if (tecla == 'd') {
        (*novo_x)++;
    }
}

static int processar_movimento(MAPA *mapa, PERSONAGEM *jogador, const char *caminho, int novo_x, int novo_y) {
    if (mover_jogador(mapa, novo_x, novo_y) == 0) {
        adicionar_mensagem("Você não pode passar por aqui...");
        getchar();
        return 0;
    }

    int slot = extrair_slot_do_caminho(caminho);
    int resultado_celula = verificar_celula(mapa, jogador, caminho, slot);
    if (resultado_celula == CELULA_TRANSICAO) {
        return 1;
    }
    if (resultado_celula == CELULA_MORTE) {
        return MENU_GAME_OVER;
    }
    return 0;
}

int rodar_jogo (PERSONAGEM *jogador, MAPA *mapa, const char *caminho) {
    // Esse laço central controla o jogo enquanto o jogador estiver em partida.
    // Cada tecla representa uma ação, como mover, abrir inventário ou pausar.
    while (1) {
        limpar_tela();

        int novo_x = mapa->jogador_x;
        int novo_y = mapa->jogador_y;

        imprimir_mapa(mapa);
        imprimir_hud(jogador, mapa);

        char tecla = getchar();
        limpar_buffer();

        if (tecla == 'i') {
            processar_inventario(jogador);
            continue;
        }

        if (tecla == 'p') {
            int resultado_pausa = processar_pausa(jogador, mapa, caminho);
            if (resultado_pausa == MENU_SAIR) {
                return MENU_SAIR;
            }
            continue;
        }

        atualizar_posicao(tecla, &novo_x, &novo_y);

        int resultado_movimento = processar_movimento(mapa, jogador, caminho, novo_x, novo_y);
        if (resultado_movimento == MENU_GAME_OVER) {
            return MENU_GAME_OVER;
        }
        if (resultado_movimento == 1) {
            continue;
        }
    }
}

// ============================================================
// Buffer mensagens e HUD
// ============================================================

char mensagens[MAX_MENSAGENS][TAM_MENSAGEM] = {{0}};

void adicionar_mensagem(const char *msg) {
    for (int i = 0; i < MAX_MENSAGENS - 1; i++) {
        strcpy(mensagens[i], mensagens[i+1]);
    }
    strcpy(mensagens[MAX_MENSAGENS - 1], msg);
}

void imprimir_hud (PERSONAGEM *jogador, MAPA *mapa) {
    //Extrai nome da região do caminho salvo em mapa_atual
    const char *nome_regiao = strrchr(mapa->mapa_atual, '_');
    char regiao[30] = "Desconhecida";
    if (nome_regiao) {
        nome_regiao++;
        strncpy(regiao, nome_regiao, sizeof(regiao) - 1);
        char *ponto = strrchr(regiao, '.');
        if (ponto) *ponto = '\0';
        if (regiao[0] >= 'a' && regiao[0] <= 'z') regiao[0] -= 32;
    }
    
    char hp[20], xp[20], mana[20], reg[30], lvl[20];
    snprintf(hp,  sizeof(hp),  "HP: %d/%d", jogador->hp, jogador->hp_max);
    snprintf(xp,  sizeof(xp),  "XP: %d/%d", jogador->exp, jogador->nivel * 100);
    snprintf(lvl, sizeof(lvl), "Nivel: %d", jogador->nivel);
    snprintf(mana,sizeof(mana),"Mana: %d/%d", jogador->mana, jogador->mana_max);
    snprintf(reg, sizeof(reg), "Regiao: %s", regiao);

    printf("%-20s | %-20s | %s\n", hp, reg, mensagens[0]);
    printf("%-20s | %-20s | %s\n", xp, lvl, mensagens[1]);
    printf("%-20s | %-20s | %s\n", mana, "", mensagens[2]);
}