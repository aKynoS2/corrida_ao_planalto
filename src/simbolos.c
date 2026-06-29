#include "simbolos.h"

DEFINICAO_SIMBOLO tabela_simbolos[NUM_SIMBOLOS] = {
    { SIM_PAREDE,       0, 0, 0, 0 },
    { SIM_PAREDE_VEG,   0, 0, 0, 0 },
    { SIM_ESTRUTURA,    0, 0, 0, 0 },
    { SIM_MONTANHA,     0, 0, 0, 0 },
    { SIM_ARVORE,       0, 0, 0, 0 },
    { SIM_CHAO,         1, 0, 0, 0 },
    { SIM_GRAMA,        1, 0, 0, 0 },
    { SIM_ESTRADA,      1, 0, 0, 0 },
    { SIM_TERRA_SECA,   1, 0, 0, 0 },
    { SIM_PEDRA_SOLTA,  1, 0, 0, 0 },
    { SIM_AGUA,         0, 0, 0, 0 },
    { SIM_RASO,         1, 0, 0, 0 },
    { SIM_INIMIGO,      1, 1, 0, 0 },
    { SIM_BAU,          1, 0, 1, 0 },
    { SIM_SAIDA_DIR,    1, 0, 0, 1 },
    { SIM_SAIDA_ESQ,    1, 0, 0, 1 },
    { SIM_SAIDA_BAIXO,  1, 0, 0, 1 },
    { SIM_SAIDA_CIMA,   1, 0, 0, 1 },
    { SIM_CIDADE,       1, 0, 0, 0 },
};