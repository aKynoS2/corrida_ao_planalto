#include <stdio.h>
#include <string.h>
#include "../include/personagem.h"
#include "../include/mapa.h"
#include "../include/combate.h"
#include "../include/utils.h"

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

  INIMIGO inimigo;
  inimigo.hp_max = 20;
  inimigo.hp = 20;
  inimigo.ataque = 5;
  inimigo.defesa = 3;
  inimigo.xp_recompensa = 50;

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

  for (int y = 0; y < mapa.altura; y++) {
    for (int x = 0; x < mapa.largura; x++) {
      mapa.grid[y][x].simbolo = layout[y][x];
      
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
    mapa.grid[mapa.jogador_y][mapa.jogador_x].simbolo = '.';

    char tecla = getchar(); // Lê a tecla pressionada
    
    // Atualiza a nova posição com base na tecla pressionada
    if (tecla == 'w') novo_y--;
    else if (tecla == 's') novo_y++;
    else if (tecla == 'a') novo_x--;
    else if (tecla == 'd') novo_x++;

    // Verifica se a nova posição é transitável antes de mover o jogador
    if (mapa.grid[novo_y][novo_x].transitavel == 1) {
      mapa.jogador_x = novo_x;
      mapa.jogador_y = novo_y;
    }

    // Verifica se o jogador encontrou um inimigo
    if (mapa.grid[mapa.jogador_y][mapa.jogador_x].tem_inimigo == 1) {
      printf("Você encontrou um inimigo! Iniciando combate...\n");
      combate (&jogador, &inimigo);
      mapa.grid[mapa.jogador_y][mapa.jogador_x].tem_inimigo = 0;
    }

    // Verifica se o jogador encontrou um baú
    if (mapa.grid[mapa.jogador_y][mapa.jogador_x].tem_bau == 1) {
      printf("Você encontrou um baú! Abrindo...\n");

      getchar(); // Aguarda o jogador pressionar Enter para continuar
    }

    // Marca a nova posição do jogador no mapa
    mapa.grid[mapa.jogador_y][mapa.jogador_x].simbolo = '@';
  }
}