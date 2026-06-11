#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../include/personagem.h"
#include "../include/mapa.h"
#include "../include/combate.h"
#include "../include/utils.h"
#include "../include/inimigos.h"
#include "../include/itens.h"

int main () {
  MAPA mapa;
  mapa.largura = 20;
  mapa.altura = 20;
  mapa.jogador_x = 8;
  mapa.jogador_y = 10;

  PERSONAGEM jogador;
  jogador.hp_max = 20;
  jogador.hp = 20;
  jogador.ataque = 8;
  jogador.defesa = 3;
  jogador.num_itens = 0;

  char layout[20][21] = {
    "####################",
    "#..................#",
    "#..................#",
    "#..###...###.......#",
    "#..#.......#.......#",
    "#..#...E...#.......#",
    "#..###...###.......#",
    "#..................#",
    "#....E.............#",
    "#..................#",
    "#.......@..........#",
    "#..................#",
    "#.........E........#",
    "#..................#",
    "#..###...###.......#",
    "#..#.......#.......#", 
    "#..#.......#...B...#",
    "#..###...###.......#",
    "#..................#",
    "####################"
  };

  srand(time(NULL));

  for (int y = 0; y < mapa.altura; y++) {
    for (int x = 0; x < mapa.largura; x++) {
      char tmp[2] = { layout[y][x], '\0' };
      strcpy(mapa.grid[y][x].simbolo, tmp);
      
      // Define se a célula é transitável ou não
      if (layout[y][x] == '#') {
        mapa.grid[y][x].transitavel = 0;
      } else {
        mapa.grid[y][x].transitavel = 1;
      }
      
      // Define se a célula tem inimigo ou não
      if (layout[y][x] == 'E') {
        mapa.grid[y][x].tem_inimigo = 1;
      } else {
        mapa.grid[y][x].tem_inimigo = 0;
      }

      // Define se a célula tem baú ou não
      if (layout[y][x] == 'B') {
        mapa.grid[y][x].tem_bau = 1;
      } else {
        mapa.grid[y][x].tem_bau = 0;
      }
    }
  }

  while (1) {
    limpar_tela();
    
    // Variáveis para nova posição do jogador
    int novo_x = mapa.jogador_x;
    int novo_y = mapa.jogador_y;

    imprimir_mapa(&mapa);

    // Marca a posição atual do jogador como transitável antes de mover
    strcpy(mapa.grid[mapa.jogador_y][mapa.jogador_x].simbolo, ".");

    char tecla = getchar(); // Lê a tecla pressionada
    
    // Atualiza a nova posição com base na tecla pressionada
    if (tecla == 'w') novo_y--;
    else if (tecla == 's') novo_y++;
    else if (tecla == 'a') novo_x--;
    else if (tecla == 'd') novo_x++;
    else if (tecla == 'i') {
      mostrar_inventario(&jogador);
      if (jogador.num_itens == 0) {
        printf("Você não tem itens para usar!\n");
        getchar(); // Aguarda o jogador pressionar Enter para continuar
        continue; // Volta para o início do loop sem mover o jogador
      }
      printf("Selecione um item para usar (1-%d): ", jogador.num_itens);
      int indice;
      scanf("%d", &indice);
      getchar(); // Limpa o buffer do teclado
      usar_item(&jogador, indice);
      continue; // Volta para o início do loop sem mover o jogador
    }

    // Verifica se a nova posição é transitável antes de mover o jogador
    if (mapa.grid[novo_y][novo_x].transitavel == 1) {
      mapa.jogador_x = novo_x;
      mapa.jogador_y = novo_y;
    }

    // Verifica se o jogador encontrou um inimigo
    if (mapa.grid[mapa.jogador_y][mapa.jogador_x].tem_inimigo == 1) {
      int indice = rand() % NUM_INIMIGOS;
      INIMIGO inimigo = catalogo_inimigos[indice];
      printf("Você encontrou um %s!", inimigo.nome);
      getchar();
      combate (&jogador, &inimigo);
      mapa.grid[mapa.jogador_y][mapa.jogador_x].tem_inimigo = 0;
    }

    // Verifica se o jogador encontrou um baú
    if (mapa.grid[mapa.jogador_y][mapa.jogador_x].tem_bau == 1) {
      printf("Você encontrou um baú! Abrindo...\n");
      ITEM item = catalogo_itens[rand() % NUM_ITENS];
      item.quantidade = 1;
      adicionar_item(&jogador, item);
      mapa.grid[mapa.jogador_y][mapa.jogador_x].tem_bau = 0;
      limpar_buffer();
      getchar(); // Aguarda o jogador pressionar Enter para continuar
    }

    // Marca a nova posição do jogador no mapa
    strcpy(mapa.grid[mapa.jogador_y][mapa.jogador_x].simbolo, "@");
  }
}