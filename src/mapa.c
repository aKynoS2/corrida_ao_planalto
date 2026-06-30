#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "mapa.h"
#include "simbolos.h"
#include "inimigos.h"
#include "itens.h"
#include "combate.h"
#include "personagem.h"

// ============================================================
// impressão do mapa
// ============================================================

void imprimir_mapa(MAPA *mapa) {
    // A câmera do mapa é ajustada para seguir o personagem e mostrar
    // apenas uma janela fixa do cenário em vez de todo o mapa de uma vez.
    int cam_x = mapa->jogador_x - (VIEWPORT_W / 2);
    int cam_y = mapa->jogador_y - (VIEWPORT_H / 2);
    char buffer[32768];
    int pos = 0;
    const char *cor_atual = "";

    if (cam_x + VIEWPORT_W > mapa->largura) cam_x = mapa->largura - VIEWPORT_W;
    if (cam_y + VIEWPORT_H > mapa->altura)  cam_y = mapa->altura - VIEWPORT_H;
    if (cam_x < 0) cam_x = 0;
    if (cam_y < 0) cam_y = 0;

    for (int y = cam_y; y < cam_y + VIEWPORT_H && y < mapa->altura; y++) {
        for (int x = cam_x; x < cam_x + VIEWPORT_W && x < mapa->largura; x++) {
            if (x == mapa->jogador_x && y == mapa->jogador_y) {
                pos += sprintf(buffer + pos, COR_CIANO_BRI "@" RESET);
                cor_atual = "";
            } else {
                const char *cor = cor_do_simbolo(mapa->grid[y][x].simbolo);
                if (cor != cor_atual) {
                    pos += sprintf(buffer + pos, "%s", cor);
                    cor_atual = cor;
                }
                pos += sprintf(buffer + pos, "%s", mapa->grid[y][x].simbolo);
            }
        }
        pos += sprintf(buffer + pos, RESET "\n");
        cor_atual = "";
    }

    printf("%s", buffer);
    fflush(stdout);
}

// ============================================================
// carregamento do mapa
// ============================================================

static void liberar_grade_mapa(MAPA *mapa) {
    if (mapa->grid == NULL) {
        return;
    }

    for (int i = 0; i < mapa->altura; i++) {
        free(mapa->grid[i]);
    }
    free(mapa->grid);
    mapa->grid = NULL;
}

static int alocar_grade_mapa(MAPA *mapa) {
    mapa->grid = (CELULA **)malloc(mapa->altura * sizeof(CELULA *));
    if (mapa->grid == NULL) {
        return 0;
    }

    for (int i = 0; i < mapa->altura; i++) {
        mapa->grid[i] = (CELULA *)malloc(mapa->largura * sizeof(CELULA));
        if (mapa->grid[i] == NULL) {
            for (int j = 0; j < i; j++) free(mapa->grid[j]);
            free(mapa->grid);
            mapa->grid = NULL;
            return 0;
        }
    }

    return 1;
}

static int ler_dimensoes_mapa(FILE *arquivo, int *largura, int *altura) {
    char linha[100];
    int altura_temp = 0;
    int largura_temp = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (linha[0] == '-') break;
        linha[strcspn(linha, "\n")] = '\0';
        linha[strcspn(linha, "\r")] = '\0';

        if (altura_temp == 0) {
            largura_temp = strlen(linha);
        }
        altura_temp++;
    }

    *largura = largura_temp;
    *altura = altura_temp;
    return 1;
}

static void aplicar_definicao_simbolo(CELULA *celula, const char *simbolo) {
    strcpy(celula->simbolo, simbolo);

    DEFINICAO_SIMBOLO *def = buscar_simbolo((char[2]){simbolo[0], '\0'});
    if (def != NULL) {
        celula->transitavel = def->transitavel;
        celula->tem_inimigo = def->tem_inimigo;
        celula->tem_bau = def->tem_bau;
        celula->tem_saida = def->tem_saida;
    } else {
        celula->transitavel = 0;
        celula->tem_inimigo = 0;
        celula->tem_bau = 0;
        celula->tem_saida = 0;
    }
}

static void carregar_grade_mapa(FILE *arquivo, MAPA *mapa) {
    char linha[100];
    int y = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (linha[0] == '-') break;
        linha[strcspn(linha, "\n")] = '\0';
        linha[strcspn(linha, "\r")] = '\0';

        for (int x = 0; x < mapa->largura; x++) {
            char tmp[2] = {linha[x], '\0'};
            aplicar_definicao_simbolo(&mapa->grid[y][x], tmp);
        }
        y++;
    }
}

static void carregar_saidas_mapa(FILE *arquivo, MAPA *mapa) {
    char linha[100];

    mapa->num_saidas = 0;
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (linha[0] == '\n' || linha[0] == '\r' || linha[0] == '\0') continue;
        char tipo;
        SAIDA s;
        if (sscanf(linha, "%c %d %d %49s %d %d",
                   &tipo, &s.x, &s.y, s.destino_arquivo,
                   &s.destino_x, &s.destino_y) == 6) {
            mapa->saidas[mapa->num_saidas] = s;
            mapa->num_saidas++;
        }
    }
}

static void montar_caminho_estado(char *destino, size_t tamanho, int slot, const char *nome_mapa) {
    snprintf(destino, tamanho, "data/saves/slot%d_%s.state", slot, nome_mapa);
}

static int carregar_estado_mapa(MAPA *mapa, int slot) {
    const char *nome_mapa = strrchr(mapa->mapa_atual, '/');
    if (nome_mapa) nome_mapa++;
    else nome_mapa = mapa->mapa_atual;

    char caminho_estado[100];
    montar_caminho_estado(caminho_estado, sizeof(caminho_estado), slot, nome_mapa);

    FILE *arquivo = fopen(caminho_estado, "rb");
    if (arquivo == NULL) {
        return 0;
    }

    int largura_salva, altura_salva;
    if (fread(&largura_salva, sizeof(int), 1, arquivo) != 1 ||
        fread(&altura_salva, sizeof(int), 1, arquivo) != 1) {
        fclose(arquivo);
        return 0;
    }

    if (largura_salva != mapa->largura || altura_salva != mapa->altura) {
        fclose(arquivo);
        return 0;
    }

    for (int i = 0; i < mapa->altura; i++) {
        if (fread(mapa->grid[i], sizeof(CELULA), mapa->largura, arquivo) != (size_t)mapa->largura) {
            fclose(arquivo);
            return 0;
        }
    }

    fclose(arquivo);
    return 1;
}

static int salvar_estado_mapa(MAPA *mapa, int slot) {
    const char *nome_mapa = strrchr(mapa->mapa_atual, '/');
    if (nome_mapa) nome_mapa++;
    else nome_mapa = mapa->mapa_atual;

    char caminho_estado[100];
    montar_caminho_estado(caminho_estado, sizeof(caminho_estado), slot, nome_mapa);

    FILE *arquivo = fopen(caminho_estado, "wb");
    if (arquivo == NULL) {
        return 0;
    }

    fwrite(&mapa->largura, sizeof(int), 1, arquivo);
    fwrite(&mapa->altura, sizeof(int), 1, arquivo);
    for (int i = 0; i < mapa->altura; i++) {
        fwrite(mapa->grid[i], sizeof(CELULA), mapa->largura, arquivo);
    }

    fclose(arquivo);
    return 1;
}

void carregar_mapa(MAPA *mapa, const char *caminho_arquivo) {
    // A leitura do arquivo de mapa é feita em duas etapas: primeiro para
    // descobrir as dimensões e depois para preencher a grade com os símbolos.
    FILE *arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo do mapa: %s\n", caminho_arquivo);
        mapa->largura = 0;
        mapa->altura = 0;
        liberar_grade_mapa(mapa);
        return;
    }

    int altura_temp = 0;
    int largura_temp = 0;
    ler_dimensoes_mapa(arquivo, &largura_temp, &altura_temp);

    mapa->largura = largura_temp;
    mapa->altura = altura_temp;
    liberar_grade_mapa(mapa);

    if (!alocar_grade_mapa(mapa)) {
        printf("Erro ao reservar memória para o mapa.\n");
        mapa->largura = 0;
        mapa->altura = 0;
        fclose(arquivo);
        return;
    }

    rewind(arquivo);
    carregar_grade_mapa(arquivo, mapa);
    carregar_saidas_mapa(arquivo, mapa);

    fclose(arquivo);
}

// ============================================================
// busca dos símbolos na tabela
// ============================================================

DEFINICAO_SIMBOLO *buscar_simbolo(char s[2]) {
    for (int i = 0; i < (int)NUM_SIMBOLOS; i++) {
        if (strcmp(tabela_simbolos[i].simbolo, s) == 0) {
            return &tabela_simbolos[i];
        }
    }
    return NULL;
}

// ============================================================
// carrega e salva state dos mapas
// ============================================================

void carregar_mapa_com_estado(MAPA *mapa, const char *caminho_txt, int slot) {
    // Ao carregar um mapa, também tenta restaurar o estado salvo do mesmo
    // mapa para manter itens, inimigos e células alteradas entre sessões.
    carregar_mapa(mapa, caminho_txt);
    strncpy(mapa->mapa_atual, caminho_txt, sizeof(mapa->mapa_atual) - 1);
    mapa->mapa_atual[sizeof(mapa->mapa_atual) - 1] = '\0';

    carregar_estado_mapa(mapa, slot);
}

void salvar_mapa_com_estado(MAPA *mapa, int slot) {
    if (!salvar_estado_mapa(mapa, slot)) {
        printf("Aviso: não foi possivel abrir arquivo de estado para salvar\n");
    }
}

// ============================================================
// cores em ANSI
// ============================================================

const char* cor_do_simbolo(char simbolo[2]) {
    if (strcmp(simbolo, "#") == 0) return COR_BRANCO;
    if (strcmp(simbolo, "%") == 0) return COR_VERDE;
    if (strcmp(simbolo, "&") == 0) return COR_CINZA;
    if (strcmp(simbolo, "^") == 0) return COR_CINZA_CLARO;
    if (strcmp(simbolo, "T") == 0) return COR_VERDE;
    if (strcmp(simbolo, ".") == 0) return COR_BRANCO;
    if (strcmp(simbolo, ",") == 0) return COR_VERDE_CLARO;
    if (strcmp(simbolo, "_") == 0) return COR_AMARELO;
    if (strcmp(simbolo, ":") == 0) return COR_AMARELO;
    if (strcmp(simbolo, "'") == 0) return COR_CINZA;
    if (strcmp(simbolo, "=") == 0) return COR_AZUL;
    if (strcmp(simbolo, "~") == 0) return COR_CIANO;
    if (strcmp(simbolo, "@") == 0) return COR_CIANO_BRI;
    if (strcmp(simbolo, "E") == 0) return COR_VERMELHO;
    if (strcmp(simbolo, "?") == 0) return COR_MAGENTA;
    if (strcmp(simbolo, ">") == 0) return COR_AMARELO_BRI;
    if (strcmp(simbolo, "<") == 0) return COR_AMARELO_BRI;
    if (strcmp(simbolo, "v") == 0) return COR_AMARELO_BRI;
    if (strcmp(simbolo, "*") == 0) return COR_AMARELO_BRI;
    if (strcmp(simbolo, "+") == 0) return COR_AMARELO;
    return RESET;
}

// ============================================================
// movimentação
// ============================================================

int mover_jogador(MAPA *mapa, int novo_x, int novo_y) {
    // Garante que o movimento fique dentro dos limites do mapa antes de
    // tentar acessar a célula alvo.
    if (novo_x < 0 || novo_y < 0 || novo_x >= mapa->largura || novo_y >= mapa->altura) {
        return 0;
    }

    if (mapa->grid[novo_y][novo_x].transitavel == 1) {
        mapa->jogador_x = novo_x;
        mapa->jogador_y = novo_y;
        return 1;
    }

    return 0;
}

// ============================================================
// validação das células
// ============================================================

int verificar_celula(MAPA *mapa, PERSONAGEM *jogador, const char *caminho, int slot) {
    int y = mapa->jogador_y;
    int x = mapa->jogador_x;

    if (x < 0 || y < 0 || x >= mapa->largura || y >= mapa->altura) {
        return CELULA_NORMAL;
    }

    // Cada célula pode ter efeito especial ao ser visitada. Depois de
    // processada, o estado da célula é alterado para evitar repetir a ação.
    if (mapa->grid[y][x].tem_inimigo == 1) {
        int indice = rand() % NUM_INIMIGOS;
        INIMIGO inimigo = catalogo_inimigos[indice];
        
        char buf[TAM_MENSAGEM];
        snprintf(buf, sizeof(buf), "Você encontrou um %s!", inimigo.nome);
        adicionar_mensagem(buf);
        
        getchar();
        int resultado_combate = combate(jogador, &inimigo);
        mapa->grid[y][x].tem_inimigo = 0;
        if (resultado_combate == MENU_GAME_OVER) {
            return CELULA_MORTE;
        }
        strcpy(mapa->grid[y][x].simbolo, ".");
        salvar_jogo(jogador, mapa, caminho);
    }

    if (mapa->grid[y][x].tem_bau == 1) {
        int indice_item = rand() % NUM_ITENS;
        ITEM item = catalogo_itens[indice_item];
        item.quantidade = 1;

        mapa->grid[y][x].tem_bau = 0;
        strcpy(mapa->grid[y][x].simbolo, ".");

        adicionar_item(jogador, item);
        salvar_jogo(jogador, mapa, caminho);
        fflush(stdout);
        getchar();
    }

    if (mapa->grid[y][x].tem_saida == 1) {
        char destino[50] = {0};
        int saida_x = 0, saida_y = 0;

        for (int i = 0; i < mapa->num_saidas; i++) {
            if (mapa->saidas[i].x == x && mapa->saidas[i].y == y) {
                strncpy(destino, mapa->saidas[i].destino_arquivo, sizeof(destino) - 1);
                destino[sizeof(destino) - 1] = '\0';
                saida_x = mapa->saidas[i].destino_x;
                saida_y = mapa->saidas[i].destino_y;
                break;
            }
        }

        salvar_mapa_com_estado(mapa, slot);
        carregar_mapa_com_estado(mapa, destino, slot);
        mapa->jogador_x = saida_x;
        mapa->jogador_y = saida_y;
        return CELULA_TRANSICAO;
    }

    return CELULA_NORMAL;
}