#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "mapa.h"
#include "simbolos.h"

void imprimir_mapa(MAPA *mapa) {
    int cam_x = mapa->jogador_x - (VIEWPORT_W / 2);
    int cam_y = mapa->jogador_y - (VIEWPORT_H / 2);
    if (cam_x < 0) cam_x = 0;
    if (cam_y < 0) cam_y = 0;
    if (cam_x + VIEWPORT_W > mapa->largura) cam_x = mapa->largura - VIEWPORT_W;
    if (cam_y + VIEWPORT_H > mapa->altura)  cam_y = mapa->altura - VIEWPORT_H;

    for (int y = cam_y; y < cam_y + VIEWPORT_H; y++) {
        for (int x = cam_x; x < cam_x + VIEWPORT_W; x++) {
            if (x == mapa->jogador_x && y == mapa->jogador_y) {
                printf("@");
            } else {
                printf("%s", mapa->grid[y][x].simbolo);
            }
        }
        printf("\n");
    }
}

void carregar_mapa(MAPA *mapa, const char *caminho_arquivo) {
    char linha[100];
    int y = 0;
    mapa->altura = 0;

    FILE *arquivo = fopen (caminho_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir mapa...");
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        
        if (linha[0] == '-') break; // pula o "---"
        linha[strcspn(linha, "\n")] = '\0';
        if (y ==0) mapa->largura = strlen(linha);
        
        for (int x = 0; x < mapa->largura; x++) {
            char tmp[2] = {linha[x], '\0'};
            strcpy(mapa->grid[y][x].simbolo, tmp);
      
            DEFINICAO_SIMBOLO *def = buscar_simbolo(tmp);
            if (def != NULL) {
                mapa->grid[y][x].transitavel = def->transitavel;
                mapa->grid[y][x].tem_inimigo = def->tem_inimigo;
                mapa->grid[y][x].tem_bau    = def->tem_bau;
                mapa->grid[y][x].tem_saida = def->tem_saida;
            } else {
                // símbolo desconhecido — assume intransitável por segurança
                mapa->grid[y][x].transitavel = 0;
                mapa->grid[y][x].tem_inimigo = 0;
                mapa->grid[y][x].tem_bau = 0;
                mapa->grid[y][x].tem_saida = 0;
            }
        }
        y++;
        mapa->altura++;
    }
    
    mapa->num_saidas = 0;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {

    char tipo;
    SAIDA s;
    sscanf(linha, "%c %d %d %49s %d %d", &tipo, &s.x, &s.y, s.destino_arquivo, &s.destino_x, &s.destino_y);
    mapa->saidas[mapa->num_saidas] = s;
    mapa->num_saidas++;
    }
    fclose (arquivo);
}

// ============================================================
// Função de busca na tabela
// Retorna o ponteiro pra definição do símbolo, ou NULL se não encontrado
// ============================================================
DEFINICAO_SIMBOLO *buscar_simbolo(char s[5]) {
    for (int i = 0; i < (int)NUM_SIMBOLOS; i++) {
        if (strcmp(tabela_simbolos[i].simbolo, s) == 0) {
            return &tabela_simbolos[i];
        }
    }
    return NULL;
}

// ============================================================
// Carrega o mapa do .txt e depois tenta restaurar o estado
// do arquivo .dat se ele existir (inimigos mortos, baús abertos, etc)
// ============================================================
void carregar_mapa_com_estado(MAPA *mapa, const char *caminho_txt, int slot) {
    // Primeiro, carrega o mapa limpo do arquivo .txt
    carregar_mapa(mapa, caminho_txt);
    strcpy(mapa->mapa_atual, caminho_txt);
    
    // Extrai o nome do arquivo .txt (tipo "mapa_norte.txt")
    const char *nome_mapa = strrchr(caminho_txt, '/');
    if (nome_mapa) nome_mapa++;  // pula a barra
    else nome_mapa = caminho_txt;
    
    // Monta o caminho do arquivo de estado (tipo "data/saves/slot1_mapa_norte.txt.state")
    char caminho_estado[100];
    sprintf(caminho_estado, "data/saves/slot%d_%s.state", slot, nome_mapa);
    
    // Se o arquivo de estado existe, carrega o grid salvo (sobrescreve os inimigos/baús removidos)
    FILE *arquivo = fopen(caminho_estado, "rb");
    if (arquivo != NULL) {
        if (fread(mapa->grid, sizeof(mapa->grid), 1, arquivo) != 1) {
            printf("Aviso: erro ao ler estado do mapa\n");
        }
        fclose(arquivo);
    }
    // Se não existir, fica com o grid limpo carregado do .txt (primeira vez visitando)
}

// ============================================================
// Salva o estado do grid em arquivo .state
// Chamado toda vez que sai de um mapa (transição ou pausa+save)
// ============================================================
void salvar_mapa_com_estado(MAPA *mapa, int slot) {
    // Extrai o nome do arquivo .txt do campo mapa_atual
    const char *caminho_txt = mapa->mapa_atual;
    const char *nome_mapa = strrchr(caminho_txt, '/');
    if (nome_mapa) nome_mapa++;
    else nome_mapa = caminho_txt;
    
    // Monta o caminho do arquivo de estado
    char caminho_estado[100];
    sprintf(caminho_estado, "data/saves/slot%d_%s.state", slot, nome_mapa);
    
    FILE *arquivo = fopen(caminho_estado, "wb");
    if (arquivo != NULL) {
        if (fwrite(mapa->grid, sizeof(mapa->grid), 1, arquivo) != 1) {
            printf("Aviso: erro ao salvar estado do mapa\n");
        }
        fclose(arquivo);
    } else {
        printf("Aviso: não foi possível abrir arquivo de estado para salvar\n");
    }
}