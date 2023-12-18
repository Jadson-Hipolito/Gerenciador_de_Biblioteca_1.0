#ifndef MODULO_LIVRO_H
#define MODULO_LIVRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CPFS 100
#define MAX_DIGITOS_CPF 12
#define MAX_TAMANHO_ISBN 20

struct livro {
    char titulo[300];
    char autor[150];
    int quant;
    int quant_disp;
    char **lista_cpfs;
    int local;
    char isbn[MAX_TAMANHO_ISBN];
};

extern struct livro biblioteca[5000];
extern int numLivros;

void editor_livro();
void emprestimo_livro();
void registrar_livro();
int menu_livro();
bool validar_isbn(const char *isbn);
void devolucao_livro();

#endif // MODULO_LIVRO_H
