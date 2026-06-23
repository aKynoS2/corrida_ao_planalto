#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "personagem.h"
#include "mapa.h"
#include "combate.h"
#include "utils.h"
#include "inimigos.h"
#include "itens.h"

int main () {
  MAPA mapa;
  mapa.jogador_x = 8;
  mapa.jogador_y = 10;
  PERSONAGEM jogador;
  jogador.hp_max = 20;
  jogador.hp = 20;
  jogador.mana_max = 30;
  jogador.mana = 30;
  jogador.ataque = 8;
  jogador.defesa = 3;
  jogador.num_itens = 0;
  jogador.nivel = 1;
  jogador.exp = 0;

  char caminho[50];

  srand(time(NULL));

  menu:
  while (1) {
    int opcao;

    limpar_tela();
    printf("1 - Novo Jogo\n");
    printf("2 - Carregar jogo\n");
    printf("3 - Sair\n");
    scanf("%d", &opcao);

    switch(opcao) {
      case 1: {
        int slot;
        printf("Qual slot? (1-3): ");
        scanf("%d", &slot);
        limpar_buffer();
        sprintf(caminho, "data/saves/save%d.dat", slot);

        FILE *teste = fopen(caminho, "rb");
        if (teste != NULL) {
          fclose(teste);
          printf("Esse slot já tem um save. Sobrescrever? (1-sim / 2-nao): ");
          int confirma;
          scanf("%d", &confirma);
          limpar_buffer();
          if (confirma != 1) {
            continue;
          }
          limpar_estados_slot(slot);
        }
        // Carrega o mapa com estado (se existir arquivo de estado salvo)
        carregar_mapa_com_estado(&mapa, "data/maps/mapa_norte.txt", slot);
        goto jogo;
      }

      case 2: {
        int slot;
        printf("Qual slot? (1-3): ");
        scanf("%d", &slot);
        sprintf(caminho, "data/saves/save%d.dat", slot);
        if (carregar_jogo(&jogador, &mapa, caminho) == 0) {
          printf("Save vazio, escolha outra opção.\n");
          getchar();
          break; // volta pro menu, não dá goto
        }
        goto jogo;
      }
      case 3: return 0;
    }
  }

  jogo:
  while (1) {
    limpar_tela();
    
    // Variáveis para nova posição do jogador
    int novo_x = mapa.jogador_x;
    int novo_y = mapa.jogador_y;

    imprimir_mapa(&mapa);

    // Marca a posição atual do jogador como transitável antes de mover
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
    else if (tecla == 'p') {
      int opcao = tela_pause(&jogador, &mapa, caminho);
      if (opcao == 1) goto menu;
      continue;
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
      strcpy(mapa.grid[mapa.jogador_y][mapa.jogador_x].simbolo, ".");
      salvar_jogo(&jogador, &mapa, caminho);
    }

    // Verifica se o jogador encontrou um baú
    if (mapa.grid[mapa.jogador_y][mapa.jogador_x].tem_bau == 1) {
      int indice_item = rand() % NUM_ITENS;
      ITEM item = catalogo_itens[indice_item];
      item.quantidade = 1;
    
      mapa.grid[mapa.jogador_y][mapa.jogador_x].tem_bau = 0;
      strcpy(mapa.grid[mapa.jogador_y][mapa.jogador_x].simbolo, ".");
    
      adicionar_item(&jogador, item);
      salvar_jogo(&jogador, &mapa, caminho);
    
      limpar_tela();
      printf("🎁 Você encontrou: %s!\n", item.nome);
      printf("Pressione Enter para continuar...\n");
      getchar();
    
      continue;
    }

    if (mapa.grid[mapa.jogador_y][mapa.jogador_x].tem_saida == 1) {
      char destino[50] = {0};
      int saida_x = 0, saida_y = 0;
      
      for (int i = 0; i < mapa.num_saidas; i++) {
        if ((mapa.saidas[i].x == mapa.jogador_x) && (mapa.saidas[i].y == mapa.jogador_y)) {
          strcpy(destino, mapa.saidas[i].destino_arquivo);
          saida_x = mapa.saidas[i].destino_x;
          saida_y = mapa.saidas[i].destino_y;
          break;
        }
      }

      int slot = extrair_slot_do_caminho(caminho);
      salvar_mapa_com_estado(&mapa, slot);
      carregar_mapa_com_estado(&mapa, destino, slot);
      mapa.jogador_y = saida_y;
      mapa.jogador_x = saida_x;
      continue;
    }
  }
}