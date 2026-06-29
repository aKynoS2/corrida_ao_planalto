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

void imprimir_mapa(MAPA *mapa) {
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

void carregar_mapa(MAPA *mapa, const char *caminho_arquivo) {
    FILE *arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir mapa...");
        return;
    }

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

    mapa->largura = largura_temp;
    mapa->altura = altura_temp;

    if (mapa->grid != NULL) {
        for (int i = 0; i < mapa->altura; i++) {
            free(mapa->grid[i]);
        }
        free(mapa->grid);
        mapa->grid = NULL;
    }

    mapa->grid = (CELULA **)malloc(mapa->altura * sizeof(CELULA *));
    for (int i = 0; i < mapa->altura; i++) {
        mapa->grid[i] = (CELULA *)malloc(mapa->largura * sizeof(CELULA));
    }

    rewind(arquivo);

    int y = 0;
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (linha[0] == '-') break;
        linha[strcspn(linha, "\n")] = '\0';
        linha[strcspn(linha, "\r")] = '\0';

        for (int x = 0; x < mapa->largura; x++) {
            char tmp[2] = {linha[x], '\0'};
            strcpy(mapa->grid[y][x].simbolo, tmp);

            DEFINICAO_SIMBOLO *def = buscar_simbolo(tmp);
            if (def != NULL) {
                mapa->grid[y][x].transitavel = def->transitavel;
                mapa->grid[y][x].tem_inimigo = def->tem_inimigo;
                mapa->grid[y][x].tem_bau = def->tem_bau;
                mapa->grid[y][x].tem_saida = def->tem_saida;
            } else {
                mapa->grid[y][x].transitavel = 0;
                mapa->grid[y][x].tem_inimigo = 0;
                mapa->grid[y][x].tem_bau = 0;
                mapa->grid[y][x].tem_saida = 0;
            }
        }
        y++;
    }

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

    fclose(arquivo);
}

DEFINICAO_SIMBOLO *buscar_simbolo(char s[2]) {
    for (int i = 0; i < (int)NUM_SIMBOLOS; i++) {
        if (strcmp(tabela_simbolos[i].simbolo, s) == 0) {
            return &tabela_simbolos[i];
        }
    }
    return NULL;
}

void carregar_mapa_com_estado(MAPA *mapa, const char *caminho_txt, int slot) {
    carregar_mapa(mapa, caminho_txt);
    strcpy(mapa->mapa_atual, caminho_txt);

    const char *nome_mapa = strrchr(caminho_txt, '/');
    if (nome_mapa) nome_mapa++;
    else nome_mapa = caminho_txt;

    char caminho_estado[100];
    sprintf(caminho_estado, "data/saves/slot%d_%s.state", slot, nome_mapa);

    FILE *arquivo = fopen(caminho_estado, "rb");
    if (arquivo != NULL) {
        int largura_salva, altura_salva;

        if (fread(&largura_salva, sizeof(int), 1, arquivo) != 1) {
            fclose(arquivo); return;
        }
        if (fread(&altura_salva, sizeof(int), 1, arquivo) != 1) {
            fclose(arquivo); return;
        }
        if (largura_salva != mapa->largura || altura_salva != mapa->altura) {
            fclose(arquivo); return;
        }
        for (int i = 0; i < mapa->altura; i++) {
            if (fread(mapa->grid[i], sizeof(CELULA), mapa->largura, arquivo) != (size_t)mapa->largura) {
                fclose(arquivo); return;
            }
        }
        fclose(arquivo);
    }
}

void salvar_mapa_com_estado(MAPA *mapa, int slot) {
    const char *caminho_txt = mapa->mapa_atual;
    const char *nome_mapa = strrchr(caminho_txt, '/');
    if (nome_mapa) nome_mapa++;
    else nome_mapa = caminho_txt;

    char caminho_estado[100];
    sprintf(caminho_estado, "data/saves/slot%d_%s.state", slot, nome_mapa);

    FILE *arquivo = fopen(caminho_estado, "wb");
    if (arquivo == NULL) {
        printf("Aviso: nao foi possivel abrir arquivo de estado para salvar\n");
        return;
    }

    fwrite(&mapa->largura, sizeof(int), 1, arquivo);
    fwrite(&mapa->altura,  sizeof(int), 1, arquivo);
    for (int i = 0; i < mapa->altura; i++) {
        fwrite(mapa->grid[i], sizeof(CELULA), mapa->largura, arquivo);
    }
    fclose(arquivo);
}

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

void mover_jogador(MAPA *mapa, int novo_x, int novo_y) {
    if (mapa->grid[novo_y][novo_x].transitavel == 1) {
        mapa->jogador_x = novo_x;
        mapa->jogador_y = novo_y;
    }
}

int verificar_celula(MAPA *mapa, PERSONAGEM *jogador, const char *caminho, int slot) {
    int y = mapa->jogador_y;
    int x = mapa->jogador_x;

    if (mapa->grid[y][x].tem_inimigo == 1) {
        int indice = rand() % NUM_INIMIGOS;
        INIMIGO inimigo = catalogo_inimigos[indice];
        printf("Voce encontrou um %s!\n", inimigo.nome);
        getchar();
        combate(jogador, &inimigo);
        mapa->grid[y][x].tem_inimigo = 0;
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

        printf("Voce encontrou: %s!\n", item.nome);
        printf("Pressione Enter para continuar...\n");
        fflush(stdout);
        getchar();
    }

    if (mapa->grid[y][x].tem_saida == 1) {
        char destino[50] = {0};
        int saida_x = 0, saida_y = 0;

        for (int i = 0; i < mapa->num_saidas; i++) {
            if (mapa->saidas[i].x == x && mapa->saidas[i].y == y) {
                strcpy(destino, mapa->saidas[i].destino_arquivo);
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