#include <stdio.h>
#include "../include/personagem.h"

void verificar_level_up(PERSONAGEM* jogador) {
  if (jogador->exp >= jogador->nivel * 100) {
    jogador->exp -= jogador->nivel * 100;
    jogador->nivel += 1;
    jogador->hp_max += 5;
    jogador->hp = jogador->hp_max; // cura completo ao upar
    jogador->ataque += 1;
    jogador->defesa += 1;

    printf("Você subiu para o nível %d!\n", jogador->nivel);
  }
}

void adicionar_item(PERSONAGEM* jogador, ITEM item) {
  if (jogador->num_itens < 20) {
    jogador->inventario[jogador->num_itens] = item;
    jogador->num_itens++;
    printf("Você adquiriu: %s\n", item.nome);
  } else {
    printf("Seu inventário está cheio!\n");
  }
}

void mostrar_inventario(PERSONAGEM* jogador) {
  if (jogador->num_itens != 0) {
    printf("Inventário:\n");
    for (int i = 0; i < jogador->num_itens; i++) {
      ITEM item = jogador->inventario[i];
      printf("%d  |   %s    |     Tipo: %d    |   Valor: %d   |   Quantidade: %d\n", 
             i + 1, item.nome, item.tipo, item.valor, item.quantidade);
    }
  } 
}

void usar_item(PERSONAGEM* jogador, int indice) {
    if (indice < 1 || indice > jogador->num_itens) {
        printf("Índice inválido!\n");
        return;
    }
    ITEM* item = &jogador->inventario[indice - 1];
    if (item->quantidade > 0) {
        if (item->tipo == 2) {
            jogador->hp += item->valor;
            if (jogador->hp > jogador->hp_max) {
                jogador->hp = jogador->hp_max;
            }
            printf("Você usou %s e recuperou %d HP!\n", item->nome, item->valor);
        }
        item->quantidade--;
        if (item->quantidade == 0) {
            for (int i = indice - 1; i < jogador->num_itens - 1; i++) {
                jogador->inventario[i] = jogador->inventario[i + 1];
            }
            jogador->num_itens--;
        }
    } else {
        printf("Você não tem mais %s!\n", item->nome);
    }
}
