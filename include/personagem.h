#ifndef PERSONAGEM_H
#define PERSONAGEM_H

// ============================================================
// structs de item, personagem e inimigo
// ============================================================

typedef struct {
  char nome[50];
  int tipo;     // 0: arma, 1: armadura, 2: poção
  int valor;    // para armas: dano, para armaduras: defesa, para poções: efeito
  int quantidade;
} ITEM;

typedef struct {
  char nome[50];
  int hp, hp_max;
  int mana, mana_max;
  int ataque;
  int defesa;
  int nivel;
  int exp;
  ITEM inventario[20];
  int num_itens;
  int arma_equipada;    // índice do item equipado, -1 se nenhum
  int armadura_equipada; // índice do item equipado, -1 se nenhum
} PERSONAGEM;

typedef struct {
  char nome[50];
  int hp, hp_max;
  int ataque;
  int defesa;
  int xp_recompensa;
  int mana_drain;    // pro Lobista
  int tipo;          // pra comportamentos especiais
} INIMIGO;

// ============================================================
// verificação lvl
// ============================================================

void verificar_level_up(PERSONAGEM* jogador);

// ============================================================
// inventário
// ============================================================

void adicionar_item(PERSONAGEM* jogador, ITEM item);
void usar_item(PERSONAGEM* jogador, int indice);
void imprimir_tela_inventario(PERSONAGEM *jogador);

#endif // PERSONAGEM_H