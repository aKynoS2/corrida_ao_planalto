#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "personagem.h"
#include "mapa.h"

void limpar_tela () {
  #ifdef _WIN32
    system("cls");
  #else
    system("clear");
  #endif
}

void limpar_buffer() {
  while (getchar() != '\n');
}

// ============================================================
// Extrai o número do slot do caminho tipo "data/saves/save1.dat"
// Retorna 1 se for save1.dat, 2 se for save2.dat, etc
// ============================================================
int extrair_slot_do_caminho(const char *caminho) {
    // Extrai o nome do arquivo (tipo "save1.dat")
    const char *nome = strrchr(caminho, '/');
    if (nome) nome++;
    else nome = caminho;
    
    // Procura por "save" e pega o número depois
    int slot = 0;
    sscanf(nome, "save%d", &slot);
    return slot;
}

void salvar_jogo(PERSONAGEM *jogador, MAPA *mapa, const char *caminho) {
  FILE *arquivo = fopen(caminho, "wb");
  if (arquivo == NULL) {
    printf("Save vazio...");
  } else {
    fwrite(jogador, sizeof(PERSONAGEM), 1, arquivo);
    fwrite(mapa, sizeof(MAPA), 1, arquivo);
    fclose(arquivo);
  }
}

int carregar_jogo(PERSONAGEM *jogador, MAPA *mapa, const char *caminho) {
  FILE *arquivo = fopen(caminho, "rb");
  if (arquivo == NULL) {
    printf("Save vazio..."); 
    return 0;
  } else {
    fread(jogador, sizeof(PERSONAGEM), 1, arquivo);
    fread(mapa, sizeof(MAPA), 1, arquivo);
    fclose(arquivo);
    return 1;
  }
}

int tela_pause(PERSONAGEM *jogador, MAPA *mapa, const char *caminho) {
  int opcao;

  limpar_tela();
  printf("=== PAUSA ===\n");
  printf("1 - Continuar\n");
  printf("2 - Salvar\n");
  printf("3 - Salvar e sair\n");
  scanf("%d", &opcao);
  limpar_buffer();

  int slot = extrair_slot_do_caminho(caminho);

  switch (opcao) {
    case 1:
      return 0;
    case 2:
      salvar_jogo(jogador, mapa, caminho);
      salvar_mapa_com_estado(mapa, slot);
      return 0;
    case 3:
      salvar_jogo(jogador, mapa, caminho);
      salvar_mapa_com_estado(mapa, slot);
      return 1;
    default:
      return 0;
  }
}

void limpar_estados_slot(int slot) {
    char estado[100];
    const char *mapas[] = {
      "mapa_norte.txt", 
      "mapa_nordeste.txt", 
      "mapa_centro_oeste.txt",
      "mapa_sudeste.txt",
      "mapa_sul.txt",
      "mapa_brasilia.txt"
    };
    for (int i = 0; i < 6; i++) {
        sprintf(estado, "data/saves/slot%d_%s.state", slot, mapas[i]);
        remove(estado);
    }
}