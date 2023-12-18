// modulo_comparacao.c

#include <string.h>
#include "modulo_comparacao.h"

int compararPorCPF(const void *a, const void *b) {
    return strcmp(((CPF *)a)->cpf, ((CPF *)b)->cpf);
}