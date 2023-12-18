#include "modulo_cliente.h"
#include "modulo_comparacao.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define tamanho_max 100
cliente clientes[tamanho_max];
int num_clientes = 0;

void menu_cliente(void) {
  int mudar = 1;
  while (mudar != 0) 
  {int novo_local;
   printf("\n=====================================\n");
   printf("=== Menu Cliente ===\n");
   printf("=====================================\n\n");
   printf("1 - Cadastrar Cliente\n");
   printf("2 - Listar Clientes\n");
   printf("3 - Editar Registro de Cliente\n");
   printf("0 - Retornar para o menu principal\n");
   printf("=====================================\n");
   printf("Escolha uma opção: ");
   scanf("%d", &mudar);

   switch (mudar) {
   case 1:
     cadastrar_cliente(clientes, &num_clientes);
     break;
   case 2:
     listar_clientes(clientes, &num_clientes);
     break;
   case 3:
     editar_cliente(clientes, &num_clientes);
     break;
   case 0:
     printf("Programa finalizado.\n");
     break;
   default:
     printf("Opção inválida. Tente novamente.\n");
   }
}}

void cadastrar_cliente(cliente *clientes, int *num_clientes) {
  if (*num_clientes < tamanho_max) {
    FILE *arquivo = fopen("clientes.txt", "a");

    if (arquivo != NULL) {
      printf("Digite o nome do cliente: ");
      scanf("%[^\n]", clientes[*num_clientes].nome);

      printf("Digite o endereço do cliente: ");
      scanf(" %[^\n]", clientes[*num_clientes].endereco);

      printf("Digite o telefone do cliente: ");
      scanf(" %[^\n]", clientes[*num_clientes].telefone);

      printf("Digite o cpf do cliente: ");
      scanf(" %[^\n]", clientes[*num_clientes].cpf);

      clientes[*num_clientes].ativ = 1;

      clientes[*num_clientes].id = *num_clientes + 1;

      fprintf(arquivo,
              "Nome: %s\nEndereço: %s\nTelefone: %s\nCPF: %s\nID: %d\nAtiv: %d\n\n",
              clientes[*num_clientes].nome, clientes[*num_clientes].endereco,
              clientes[*num_clientes].telefone, clientes[*num_clientes].cpf, clientes[*num_clientes].id, clientes[*num_clientes].ativ);

      (*num_clientes)++;
      fclose(arquivo);
    } else {
      printf("Erro ao abrir o arquivo.\n");
    }
  } else {
    printf("Limite máximo de clientes atingido.\n");
  }
}

void listar_clientes() {
    FILE *arquivo;
    arquivo = fopen("clientes.txt", "r"); // Abre o arquivo em modo de leitura

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    cliente clientes[200];
    int numClientes = 0;

    printf("Lista de Clientes em Ordem Crescente pelo CPF:\n");

    char linha[100];
    while (fgets(linha, sizeof(linha), arquivo) != NULL && numClientes < 200) {
        sscanf(linha, "%s %s", clientes[numClientes].nome, clientes[numClientes].cpf);
        numClientes++;
    }

    fclose(arquivo);
    qsort(clientes, numClientes, sizeof(cliente), compararPorCPF);
    
    for (int i = 0; i < numClientes; i++) {
      if (clientes[i].ativ == 1) {
        printf("Nome: %s, CPF: %s\n", clientes[i].nome, clientes[i].cpf);
      }
}
}

void editar_cliente() {
  FILE *arquivo;
  arquivo = fopen("clientes.txt", "r+");

  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return;
  }

  char cpf[15]; // Suponha que um CPF tenha até 14 dígitos
  char confir;
  printf("Digite o CPF do cliente ao qual deseja pesquisar: ");
  scanf("%14s", cpf);

  char linha[1013]; // Suponha que cada linha do arquivo tenha no máximo 100 caracteres
  int encontrado = 0;
  long int posicao = ftell(arquivo); // Para controlar a posição do arquivo

  while (fgets(linha, sizeof(linha), arquivo) != NULL) {
    if (strstr(linha, cpf) != NULL) {
      encontrado = 1;
      break;

    posicao = ftell(arquivo);
  }

  if (!encontrado) {
    printf("Cliente não encontrado.\n");
    fclose(arquivo);
    return;
  }

  printf("Informações atuais do cliente:\n");
  printf("%s", linha);

  int continuar = 1;
  while (continuar) {
    printf("\n=====================================\n");
    printf("=== Editor de Cliente ===\n");
    printf("=====================================\n");
    printf("1 - Mudar Nome do Cliente\n");
    printf("2 - Mudar Endereço do Cliente\n");
    printf("3 - Mudar Telefone do Cliente\n");
    printf("4 - Deletar Registro do Cliente\n");
    printf("0 - Retornar para o menu principal\n");
    printf("=====================================\n");
    printf("Escolha uma opção: ");
    int mudar;
    scanf("%d", &mudar);

    char novoValor[1000];
    switch (mudar) {
      case 1:
        printf("Nome: ");
        scanf("%s", novoValor);
        snprintf(linha, sizeof(linha), "Nome: %s\n", novoValor);
        break;
      case 2:
        printf("Endereço: ");
        scanf("%s", novoValor);
        snprintf(linha, sizeof(linha), "Endereço: %s\n", novoValor);
        break;
      case 3:
        printf("Telefone: ");
        scanf("%s", novoValor);
        snprintf(linha, sizeof(linha), "Telefone: %s\n", novoValor);
        break;
      case 4:
        printf("Tem certeza que deseja deletar o registro deste cliente? Digite 0 "
               "para cancelar ou 1 para confirmar: ");
        scanf(" %c", &confir);
        if (confir == '1') {
          fseek(arquivo, posicao, SEEK_SET);
          fprintf(arquivo, "Atividade: 0\n");
        }
        break;
      case 0:
        printf("Retornando para o menu principal.\n");
        continuar = 0;
        break;
      default:
        printf("Opção inválida. Tente novamente.\n");
    }
  }

  // Fecha o arquivo após as operações
  fclose(arquivo);
}
}