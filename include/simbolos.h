#ifndef SIMBOLOS_H
#define SIMBOLOS_H

#include "mapa.h"
#include <stdlib.h>

// ============================================================
// Definição dos símbolos disponíveis
// ============================================================

// Paredes e estruturas
#define SIM_PAREDE          "#"
#define SIM_PAREDE_VEG      "%"
#define SIM_ESTRUTURA       "&"
#define SIM_MONTANHA        "^"
#define SIM_ARVORE          "T"

// Chão
#define SIM_CHAO            "."
#define SIM_GRAMA           ","
#define SIM_ESTRADA         "_"
#define SIM_TERRA_SECA      ":"
#define SIM_PEDRA_SOLTA     "'"

// Água
#define SIM_AGUA            "="
#define SIM_RASO            "~"

// Entidades
#define SIM_JOGADOR         "@"
#define SIM_INIMIGO         "E"
#define SIM_BAU             "?"

// Saídas
#define SIM_SAIDA_DIR       ">"
#define SIM_SAIDA_ESQ       "<"
#define SIM_SAIDA_BAIXO     "v"
#define SIM_SAIDA_CIMA      "*"

// Ponto de interesse
#define SIM_CIDADE          "+"

// ============================================================
// Tabela de comportamento dos símbolos
// ============================================================

typedef struct {
    char simbolo[2];
    int transitavel;
    int tem_inimigo;
    int tem_bau;
    int tem_saida;
} DEFINICAO_SIMBOLO;

static DEFINICAO_SIMBOLO tabela_simbolos[] = {
    // simbolo          transitavel  tem_inimigo  tem_bau  tem_saida
    { SIM_PAREDE,           0,           0,          0,       0 },
    { SIM_PAREDE_VEG,       0,           0,          0,       0 },
    { SIM_ESTRUTURA,        0,           0,          0,       0 },
    { SIM_MONTANHA,         0,           0,          0,       0 },
    { SIM_ARVORE,           0,           0,          0,       0 },
    { SIM_CHAO,             1,           0,          0,       0 },
    { SIM_GRAMA,            1,           0,          0,       0 },
    { SIM_ESTRADA,          1,           0,          0,       0 },
    { SIM_TERRA_SECA,       1,           0,          0,       0 },
    { SIM_PEDRA_SOLTA,      1,           0,          0,       0 },
    { SIM_AGUA,             0,           0,          0,       0 },
    { SIM_RASO,             1,           0,          0,       0 },
    { SIM_INIMIGO,          1,           1,          0,       0 },
    { SIM_BAU,              1,           0,          1,       0 },
    { SIM_SAIDA_DIR,        1,           0,          0,       1 },
    { SIM_SAIDA_ESQ,        1,           0,          0,       1 },
    { SIM_SAIDA_BAIXO,      1,           0,          0,       1 },
    { SIM_SAIDA_CIMA,       1,           0,          0,       1 },
    { SIM_CIDADE,           1,           0,          0,       0 },
};

#define NUM_SIMBOLOS (sizeof(tabela_simbolos) / sizeof(tabela_simbolos[0]))

#endif // SIMBOLOS_H