// modulo_comparacao.h

#ifndef MODULO_COMPARACAO_H
#define MODULO_COMPARACAO_H

typedef struct {
    char cpf[12];
} CPF;

extern int compararPorCPF(const void *a, const void *b);

#endif