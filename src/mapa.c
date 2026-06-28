#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "mapa.h"
#include "simbolos.h"

void imprimir_mapa(MAPA *mapa) {
    int cam_x = mapa->jogador_x - (VIEWPORT_W / 2);
    int cam_y = mapa->jogador_y - (VIEWPORT_H / 2);
    // Clamp de borda inferior (não passar do fim do mapa)
    if (cam_x + VIEWPORT_W > mapa->largura) cam_x = mapa->largura - VIEWPORT_W;
    if (cam_y + VIEWPORT_H > mapa->altura)  cam_y = mapa->altura - VIEWPORT_H;
    // Clamp de borda superior (depois, para cobrir mapas menores que o viewport)
    if (cam_x < 0) cam_x = 0;
    if (cam_y < 0) cam_y = 0;

    for (int y = cam_y; y < cam_y + VIEWPORT_H && y < mapa->altura; y++) {
        for (int x = cam_x; x < cam_x + VIEWPORT_W && x < mapa->largura; x++) {
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
    FILE *arquivo = fopen (caminho_arquivo, "r");
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
        char tipo;
        SAIDA s;
        sscanf(linha, "%c %d %d %49s %d %d", &tipo, &s.x, &s.y, s.destino_arquivo, &s.destino_x, &s.destino_y);
        mapa->saidas[mapa->num_saidas] = s;
        mapa->num_saidas++;
    }
    
    fclose (arquivo);
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
        
        // PASSO 1: Lê largura e altura
        if (fread(&largura_salva, sizeof(int), 1, arquivo) != 1) {
            printf("Aviso: erro ao ler largura do estado\n");
            fclose(arquivo);
            return;
        }
        if (fread(&altura_salva, sizeof(int), 1, arquivo) != 1) {
            printf("Aviso: erro ao ler altura do estado\n");
            fclose(arquivo);
            return;
        }
        
        // PASSO 2: Valida se tamanhos batem
        if (largura_salva != mapa->largura || altura_salva != mapa->altura) {
            printf("Aviso: tamanho do mapa salvo não bate com o arquivo .txt\n");
            fclose(arquivo);
            return;
        }
        
        // PASSO 3: Lê o grid linha por linha
        for (int i = 0; i < mapa->altura; i++) {
            if (fread(mapa->grid[i], sizeof(CELULA), mapa->largura, arquivo) != (size_t)mapa->largura) {
                printf("Aviso: erro ao ler linha %d do grid\n", i);
                fclose(arquivo);
                return;
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
    if (arquivo != NULL) {
        // PASSO 1: Salva largura e altura
        if (fwrite(&mapa->largura, sizeof(int), 1, arquivo) != 1) {
            printf("Aviso: erro ao salvar largura\n");
            fclose(arquivo);
            return;
        }
        if (fwrite(&mapa->altura, sizeof(int), 1, arquivo) != 1) {
            printf("Aviso: erro ao salvar altura\n");
            fclose(arquivo);
            return;
        }
        
        // PASSO 2: Salva o grid linha por linha
        for (int i = 0; i < mapa->altura; i++) {
            if (fwrite(mapa->grid[i], sizeof(CELULA), mapa->largura, arquivo) != (size_t)mapa->largura) {
                printf("Aviso: erro ao salvar linha %d do grid\n", i);
                fclose(arquivo);
                return;
            }
        }
        
        fclose(arquivo);
    } else {
        printf("Aviso: não foi possível abrir arquivo de estado para salvar\n");
    }
}