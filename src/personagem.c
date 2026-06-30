#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "personagem.h"

// ============================================================
// verificação de nível
// ============================================================

void verificar_level_up(PERSONAGEM* jogador) {
    // O level-up ocorre quando a experiência alcança um limite baseado no
    // nível atual, então a barra é resetada e os atributos aumentam.
    if (jogador->exp >= jogador->nivel * 100) {
        jogador->exp -= jogador->nivel * 100;
        jogador->nivel += 1;
        jogador->hp_max += 5;
        jogador->hp = jogador->hp_max; // cura completo ao upar
        jogador->ataque += 1;
        jogador->defesa += 1;

        char buf[TAM_MENSAGEM];
        snprintf(buf, sizeof(buf), "Você subiu para o nível %d!", jogador->nivel);
        adicionar_mensagem(buf);
    }
}

// ============================================================
// inventário 
// ============================================================

static void remover_item_em_indice(PERSONAGEM *jogador, int indice) {
    for (int i = indice - 1; i < jogador->num_itens - 1; i++) {
        jogador->inventario[i] = jogador->inventario[i + 1];
    }
    jogador->num_itens--;
}

void adicionar_item(PERSONAGEM *jogador, ITEM item) {
    if (jogador->num_itens < 20) {
        jogador->inventario[jogador->num_itens] = item;
        jogador->num_itens++;
        
        char buf[TAM_MENSAGEM];
        snprintf(buf, sizeof(buf), "Você adquiriu: %s", item.nome);
        adicionar_mensagem(buf);
    } else {
        adicionar_mensagem("Seu inventário está cheio!");
    }
}

void imprimir_tela_inventario(PERSONAGEM *jogador) {
    char tipo_item[15] = {""};
    
    limpar_tela();
    
    printf("=========================================\n");
    printf("              INVENTÁRIO                 \n");
    printf("=========================================\n");
    printf("%-3s| %-18s| %-10s| %s\n", "#", "Nome", "Tipo", "Val");
    printf("-----------------------------------------\n");
    for (int i = 0; i < jogador->num_itens; i++) {
        switch (jogador->inventario[i].tipo) {
            case 0: {
                strcpy(tipo_item, "Arma");
                break;
            }

            case 1: {
                strcpy(tipo_item, "Armadura");
                break;
            }

            case 2: {
                strcpy(tipo_item, "Pocao");
                break;
            }
        }

        printf("%-3d| %-18s| %-10s| %d\n",
        i+1, jogador->inventario[i].nome, tipo_item, jogador->inventario[i].valor);
    }
    printf("-----------------------------------------\n");
    printf("  HP: %d/%d | Ataque: %d | Defesa: %d\n", 
    jogador->hp, jogador->hp_max, jogador->ataque, jogador->defesa);
    printf("=========================================\n");
    printf("  Selecione um item (0 para fechar):     \n");
}

void usar_item(PERSONAGEM *jogador, int indice) {
    // O inventário é tratado como uma lista compactada: ao consumir o item,
    // a posição é removida e os demais elementos deslocam-se para ocupar o espaço.
    char buf[TAM_MENSAGEM];
    
    if (indice < 1 || indice > jogador->num_itens) {
        printf("Índice inválido!\n");
        return;
    }

    ITEM *item = &jogador->inventario[indice - 1];
    if (item->quantidade <= 0) {
        
        snprintf(buf, sizeof(buf), "Você não tem mais %s!", item->nome);
        adicionar_mensagem(buf);

        return;
    }

    switch (item->tipo) {
        case 0: {
            if (jogador->arma_equipada >= 0) {
                jogador->ataque -= jogador->inventario[jogador->arma_equipada].valor;
            }
            jogador->ataque += item->valor;
            jogador->arma_equipada = indice - 1;
        
            snprintf(buf, sizeof(buf), "Você equipou %s e aumentou seu ataque em %d!", item->nome, item->valor);
            adicionar_mensagem(buf);
            break;
        }

        case 1: {
            if (jogador->armadura_equipada >= 0) {
                jogador->defesa -= jogador->inventario[jogador->armadura_equipada].valor;
            }
            jogador->defesa += item->valor;
            jogador->armadura_equipada = indice - 1;
        
            snprintf(buf, sizeof(buf), "Você equipou %s e aumentou sua defesa em %d!", item->nome, item->valor);
            adicionar_mensagem(buf);
            break;
        }

        case 2: {

            jogador->hp += item->valor;
            if (jogador->hp > jogador->hp_max) {
                jogador->hp = jogador->hp_max;
            }

            snprintf(buf, sizeof(buf), "Você usou %s e recuperou %d HP!", item->nome, item->valor);
            adicionar_mensagem(buf);

            item->quantidade--;
            if (item->quantidade == 0) {
                remover_item_em_indice(jogador, indice);
            }
            break;
        }
    }
}
