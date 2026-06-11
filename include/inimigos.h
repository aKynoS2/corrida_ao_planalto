#ifndef INIMIGOS_H
#define INIMIGOS_H

#include "personagem.h"

// Índices do catálogo
#define INIMIGO_LOBISTA                   0
#define INIMIGO_BOT_REDE_SOCIAL           1
#define INIMIGO_CABO_ELEITORAL            2
#define INIMIGO_JORNALISTA_SENSACIONAL    3
#define INIMIGO_POLITICO_VETERANO         4

// Campos: nome, hp, hp_max, ataque, defesa, xp_recompensa, mana_drain, tipo
static INIMIGO catalogo_inimigos[] = {
    {"Lobista",                      15, 15, 4, 2, 25, 3, 0},
    {"Bot de Rede Social",           10, 10, 6, 1, 20, 0, 1},
    {"Cabo Eleitoral Corrompido",    12, 12, 4, 3, 22, 0, 2},
    {"Jornalista Sensacionalista",   18, 18, 7, 1, 35, 0, 3},
    {"Politico Veterano",            25, 25, 5, 6, 50, 0, 4},
};

#define NUM_INIMIGOS 5

#endif // INIMIGOS_H