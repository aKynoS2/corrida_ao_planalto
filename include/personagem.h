#ifndef PERSONAGEM_H
#define PERSONAGEM_H

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

void verificar_level_up(PERSONAGEM* jogador);
void adicionar_item(PERSONAGEM* jogador, ITEM item);
void mostrar_inventario(PERSONAGEM* jogador);
void usar_item(PERSONAGEM* jogador, int indice);

#endif // PERSONAGEM_H