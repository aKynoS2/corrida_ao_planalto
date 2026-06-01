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
