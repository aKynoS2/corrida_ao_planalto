#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/utils.h"

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