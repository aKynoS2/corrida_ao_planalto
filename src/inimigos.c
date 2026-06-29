#include "inimigos.h"

// Campos: nome, hp, hp_max, ataque, defesa, xp_recompensa, mana_drain, tipo
INIMIGO catalogo_inimigos[NUM_INIMIGOS] = {
    {"Lobista",                      15, 15, 4, 2, 25, 3, 0},
    {"Bot de Rede Social",           10, 10, 6, 1, 20, 0, 1},
    {"Cabo Eleitoral Corrompido",    12, 12, 4, 3, 22, 0, 2},
    {"Jornalista Sensacionalista",   18, 18, 7, 1, 35, 0, 3},
    {"Politico Veterano",            25, 25, 5, 6, 50, 0, 4},
};