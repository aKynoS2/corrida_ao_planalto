#ifndef SIMBOLOS_H
#define SIMBOLOS_H

#include "mapa.h"
#include <stdlib.h>

// ============================================================
// Definição dos símbolos disponíveis
// ============================================================

// Paredes e estruturas
#define SIM_PAREDE_SOLIDA    '█'
#define SIM_PAREDE_GROSSA    '▓'
#define SIM_PAREDE_MEDIA     '▒'
#define SIM_PAREDE_FINA      '░'
#define SIM_PAREDE_H         '═'   // horizontal (estilo caixa)
#define SIM_PAREDE_V         '║'   // vertical   (estilo caixa)
#define SIM_CANTO_TL         '╔'
#define SIM_CANTO_TR         '╗'
#define SIM_CANTO_BL         '╚'
#define SIM_CANTO_BR         '╝'
#define SIM_PAREDE_SIMPLES   '#'   // fallback ASCII

// Chão
#define SIM_CHAO             '.'
#define SIM_CHAO_PONTO       '·'
#define SIM_CHAO_BOLA        '•'

// Natureza
#define SIM_ARVORE_1         '♣'   // floresta densa
#define SIM_ARVORE_2         '♠'   // floresta esparsa
#define SIM_MONTANHA         '▲'
#define SIM_AGUA             '~'
#define SIM_AGUA_ONDAS       '≈'

// Entidades
#define SIM_JOGADOR          '@'
#define SIM_INIMIGO          '☠'
#define SIM_BAU              '■'
#define SIM_BAU_PEQUENO      '▪'

// Saídas e pontos de interesse
#define SIM_SAIDA            '◉'
#define SIM_SAIDA_FECHADA    '○'
#define SIM_ENTRADA          '◈'

// ============================================================
// Tabela de comportamento dos símbolos
// ============================================================

typedef struct {
    char simbolo;
    int transitavel;
    int tem_inimigo;
    int tem_bau;
} DEFINICAO_SIMBOLO;

static DEFINICAO_SIMBOLO tabela_simbolos[] = {
    // simbolo            transitavel  tem_inimigo  tem_bau
    { SIM_PAREDE_SOLIDA,      0,           0,          0 },
    { SIM_PAREDE_GROSSA,      0,           0,          0 },
    { SIM_PAREDE_MEDIA,       0,           0,          0 },
    { SIM_PAREDE_FINA,        1,           0,          0 },  // decorativo, transitável
    { SIM_PAREDE_H,           0,           0,          0 },
    { SIM_PAREDE_V,           0,           0,          0 },
    { SIM_CANTO_TL,           0,           0,          0 },
    { SIM_CANTO_TR,           0,           0,          0 },
    { SIM_CANTO_BL,           0,           0,          0 },
    { SIM_CANTO_BR,           0,           0,          0 },
    { SIM_PAREDE_SIMPLES,     0,           0,          0 },
    { SIM_CHAO,               1,           0,          0 },
    { SIM_CHAO_PONTO,         1,           0,          0 },
    { SIM_CHAO_BOLA,          1,           0,          0 },
    { SIM_ARVORE_1,           0,           0,          0 },
    { SIM_ARVORE_2,           0,           0,          0 },
    { SIM_MONTANHA,           0,           0,          0 },
    { SIM_AGUA,               0,           0,          0 },
    { SIM_AGUA_ONDAS,         0,           0,          0 },
    { SIM_INIMIGO,            1,           1,          0 },
    { SIM_BAU,                1,           0,          1 },
    { SIM_BAU_PEQUENO,        1,           0,          1 },
    { SIM_SAIDA,              1,           0,          0 },
    { SIM_SAIDA_FECHADA,      0,           0,          0 },
    { SIM_ENTRADA,            1,           0,          0 },
};

#define NUM_SIMBOLOS (sizeof(tabela_simbolos) / sizeof(tabela_simbolos[0]))

#endif // SIMBOLOS_H