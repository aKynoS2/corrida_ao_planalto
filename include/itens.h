#ifndef ITENS_H
#define ITENS_H

#include "personagem.h"

// Índices do catálogo
#define ITEM_SANTINHO          0
#define ITEM_HORARIO_ELEITORAL 1
#define ITEM_DOSSIE            2
#define ITEM_CAIXA_2           3
#define ITEM_APOIO_POPULAR     4

// Campos: nome, tipo, valor, quantidade
static ITEM catalogo_itens[] = {
    {"Santinho",            2, 10, 0}, // Poção de HP leve
    {"Horario Eleitoral",   0, 5,  0},  // Buff de ataque temporário
    {"Dossie",              2, 0,  0},  // Remove imunidade parlamentar
    {"Caixa 2",             0, 15, 0}, // Poderoso, mas gera flag de escândalo
    {"Apoio Popular",       1, 3,  0},  // Armadura que escala com nível
};

#define NUM_ITENS 5

#endif // ITENS_H