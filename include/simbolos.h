#ifndef SIMBOLOS_H
#define SIMBOLOS_H

#include <stdlib.h>

// ============================================================
// Definição dos símbolos disponíveis
// ============================================================

#define SIM_PAREDE          "#"
#define SIM_PAREDE_VEG      "%"
#define SIM_ESTRUTURA       "&"
#define SIM_MONTANHA        "^"
#define SIM_ARVORE          "T"
#define SIM_CHAO            "."
#define SIM_GRAMA           ","
#define SIM_ESTRADA         "_"
#define SIM_TERRA_SECA      ":"
#define SIM_PEDRA_SOLTA     "'"
#define SIM_AGUA            "="
#define SIM_RASO            "~"
#define SIM_JOGADOR         "@"
#define SIM_INIMIGO         "E"
#define SIM_BAU             "?"
#define SIM_SAIDA_DIR       ">"
#define SIM_SAIDA_ESQ       "<"
#define SIM_SAIDA_BAIXO     "v"
#define SIM_SAIDA_CIMA      "*"
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

#define NUM_SIMBOLOS 20

extern DEFINICAO_SIMBOLO tabela_simbolos[NUM_SIMBOLOS];

#endif // SIMBOLOS_H