#include <stdio.h>
#include <string.h>
#include "../include/personagem.h"
#include "../include/combate.h"
#include "../include/mapa.h"
#include "../include/utils.h"

void combate (PERSONAGEM* jogador, INIMIGO* inimigo) {
  int acao;

  while (jogador -> hp > 0 && inimigo -> hp > 0) {
    //Turno do jogador
    printf("1. Atacar\n");
    printf("2. Fugir\n");
    scanf("%d", &acao);

    if (acao == 1) {
      int dano = jogador->ataque - inimigo->defesa;
      if (dano < 0) dano = 0;
      inimigo->hp -= dano;
      printf("Você causou %d de dano!\n", dano);
    } else {
      printf ("Você fugiu!\n"); 
      break;
    }

    //Turno do inimigo
    int dano = inimigo->ataque - jogador->defesa;
    if (dano < 0) dano = 0;
    jogador->hp -= dano;
    printf("O inimigo atacou! Você perdeu %d de HP.\n", dano);
  }

  if (jogador->hp <= 0) {
    printf("Você morreu!\n");
    getchar();
  }
  else if (inimigo->hp <= 0) {
    printf("Você venceu!\n");
    jogador->exp += inimigo->xp_recompensa;
    printf("Você ganhou %d de experiência!", inimigo->xp_recompensa);
    verificar_level_up(jogador);
    limpar_buffer();
    getchar();
  }
}
