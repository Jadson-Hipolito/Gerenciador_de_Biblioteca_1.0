#include "modulo_livro.h"
#include "modulo_cliente.h"
#include "modulo_comparacao.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LIVROS 5000
#define max_cpf 12

struct livro biblioteca[MAX_LIVROS];
int numLivros = 0;

void emprestimo_livro() {
    char cpf[12];
    char isbn[13];

    // Obter CPF e ISBN do usuário
    printf("CPF do cliente ao qual irá retirar o livro: ");
    scanf("%s", cpf);
    printf("ISBN do livro ao qual irá retirar: ");
    scanf("%s", isbn);

    // Verificar se o cliente existe
    FILE *clientes_file = fopen("clientes.txt", "r");
    if (clientes_file == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    int cliente_encontrado = 0;
    cliente cliente;
    while (fscanf(clientes_file, "%s %s %s %s %d %d", cliente.nome, cliente.endereco, cliente.telefone, cliente.cpf, &cliente.id, &cliente.ativ) == 6) {
        if (strcmp(cliente.cpf, cpf) == 0) {
            cliente_encontrado = 1;
            break;
        }
    }

    fclose(clientes_file);

    if (!cliente_encontrado) {
        printf("Cliente com CPF %s não encontrado.\n", cpf);
        return;
    }

    // Verificar se o livro existe
    FILE *biblioteca_file = fopen("biblioteca.txt", "r");
    if (biblioteca_file == NULL) {
        printf("Erro ao abrir o arquivo da biblioteca.\n");
        return;
    }
    struct livro livro;
    int livro_encontrado = 0;
    while (fscanf(biblioteca_file, "%s %s %d %d %d %s", livro.titulo, livro.autor, &livro.quant, &livro.quant_disp, &livro.local, livro.isbn) == 6) {
        if (strcmp(livro.isbn, isbn) == 0) {
            livro_encontrado = 1;
            break;
        }
    }

    fclose(biblioteca_file);

    if (!livro_encontrado) {
        printf("Livro com ISBN %s não encontrado.\n", isbn);
        return;
    }

    // Verificar se o livro está disponível
    if (livro.quant_disp <= 0) {
        printf("O livro não está disponível para empréstimo.\n");
        return;
    }

    // Alocar memória para a lista de CPFs
    livro.lista_cpfs = malloc(MAX_CPFS * sizeof(char *));
    for (int i = 0; i < MAX_CPFS; i++) {
        livro.lista_cpfs[i] = malloc((max_cpf) * sizeof(char));
    }

    // Adicionar o CPF à lista_cpfs
    strcpy(livro.lista_cpfs[MAX_CPFS - 1], cliente.cpf);

    // Atualizar o arquivo da biblioteca com as modificações
    FILE *biblioteca_atualizada = fopen("biblioteca.txt", "w");
    if (biblioteca_atualizada == NULL) {
        printf("Erro ao abrir o arquivo da biblioteca para atualização.\n");
        return;
    }

    fprintf(biblioteca_atualizada, "%s %s %d %d %d %s\n", livro.titulo, livro.autor, livro.quant, livro.quant_disp, livro.local, livro.isbn);

    fclose(biblioteca_atualizada);

    printf("Empréstimo realizado com sucesso.\n");
}

void devolucao_livro() {
    char cpf[12];
    char isbn[13];

    // Obter CPF e ISBN do usuário
    printf("CPF do cliente que irá devolver o livro: ");
    scanf("%s", cpf);
    printf("ISBN do livro a ser devolvido: ");
    scanf("%s", isbn);

    // Verificar se o cliente existe
    FILE *clientes_file = fopen("clientes.txt", "r");
    if (clientes_file == NULL) {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    int cliente_encontrado = 0;
    cliente cliente;
    while (fscanf(clientes_file, "%s %s %s %s %d %d", cliente.nome, cliente.endereco, cliente.telefone, cliente.cpf, &cliente.id, &cliente.ativ) == 6) {
        if (strcmp(cliente.cpf, cpf) == 0) {
            cliente_encontrado = 1;
            break;
        }
    }

    fclose(clientes_file);

    if (!cliente_encontrado) {
        printf("Cliente com CPF %s não encontrado.\n", cpf);
        return;
    }

    // Verificar se o livro existe
    FILE *biblioteca_file = fopen("biblioteca.txt", "r");
    if (biblioteca_file == NULL) {
        printf("Erro ao abrir o arquivo da biblioteca.\n");
        return;
    }

    struct livro livro;
    int livro_encontrado = 0;
    while (fscanf(biblioteca_file, "%s %s %d %d %d %s", livro.titulo, livro.autor, &livro.quant, &livro.quant_disp, &livro.local, livro.isbn) == 6) {
        if (strcmp(livro.isbn, isbn) == 0) {
            livro_encontrado = 1;
            break;
        }
    }

    fclose(biblioteca_file);

    if (!livro_encontrado) {
        printf("Livro com ISBN %s não encontrado.\n", isbn);
        return;
    }

    // Verificar se o livro foi retirado pelo cliente
    int cliente_tem_livro = 0;
    for (int i = 0; i < MAX_CPFS; i++) {
        if (livro.lista_cpfs[i] != NULL && strcmp(livro.lista_cpfs[i], cliente.cpf) == 0) {
            cliente_tem_livro = 1;
            // Liberar o espaço alocado para o CPF na lista
            free(livro.lista_cpfs[i]);
            livro.lista_cpfs[i] = NULL;
            break;
        }
    }

    if (!cliente_tem_livro) {
        printf("O cliente com CPF %s não retirou este livro.\n", cpf);
        return;
    }

    // Atualizar o arquivo da biblioteca com as modificações
    FILE *biblioteca_atualizada = fopen("biblioteca.txt", "w");
    if (biblioteca_atualizada == NULL) {
        printf("Erro ao abrir o arquivo da biblioteca para atualização.\n");
        return;
    }

    fprintf(biblioteca_atualizada, "%s %s %d %d %d %s\n", livro.titulo, livro.autor, livro.quant, livro.quant_disp + 1, livro.local, livro.isbn);

    fclose(biblioteca_atualizada);

    printf("Devolução realizada com sucesso.\n");
}

void editor_livro() {
    char isbn[14];
    int mudar;
    int novo_local;

    printf("\n=====================================\n");
    printf("=== Editor de Livros ===\n");
    printf("=====================================\n\n");

    // Pedir ISBN e verificar se existe
    printf("Digite o ISBN do livro: ");
    scanf("%s", isbn);

    int livro_encontrado = 0;
    for (int i = 0; i < numLivros; i++) {
        if (strcmp(biblioteca[i].isbn, isbn) == 0) {
            livro_encontrado = 1;
            break;
        }
    }

    if (!livro_encontrado) {
        printf("Livro com ISBN %s não encontrado.\n", isbn);
        return;
    }

    // Se o livro foi encontrado, continuar com as opções de edição
    printf("1 - Mudar Local do livro\n");
    printf("2 - Mudar quantidade de unidades\n");
    printf("0 - Retornar para o menu principal\n");
    printf("=====================================\n");
    printf("Escolha uma opção: ");
    scanf("%d", &mudar);

    switch (mudar) {
        case 1:
            printf("Novo local do livro: ");
            scanf("%d", &novo_local);

            for (int i = 0; i < numLivros; i++) {
                if (strcmp(biblioteca[i].isbn, isbn) == 0) {
                    biblioteca[i].local = novo_local;
                }
            }
            break;

        case 2:
            printf("Nova quantidade de unidades: ");
            scanf("%d", &novo_local);

            for (int i = 0; i < numLivros; i++) {
                if (strcmp(biblioteca[i].isbn, isbn) == 0) {
                    biblioteca[i].quant = novo_local;
                    biblioteca[i].quant_disp = novo_local;
                }
            }
            break;

        case 0:
            printf("Retornando para o menu principal.\n");
            break;

        default:
            printf("Opção inválida. Tente novamente.\n");
    }

    // Atualizar o arquivo da biblioteca com as modificações
    FILE *biblioteca_atualizada = fopen("biblioteca.txt", "w");
    if (biblioteca_atualizada == NULL) {
        printf("Erro ao abrir o arquivo da biblioteca para atualização.\n");
        return;
    }

    for (int i = 0; i < numLivros; i++) {
        fprintf(biblioteca_atualizada, "%s %s %d %d %d %s\n", biblioteca[i].titulo, biblioteca[i].autor,
                biblioteca[i].quant, biblioteca[i].quant_disp, biblioteca[i].local, biblioteca[i].isbn);
    }

    fclose(biblioteca_atualizada);
}

int menu_livro() {
  int escolha = 1;
  char titulo[100];
  while (escolha != 0) {
    printf("\n=====================================\n");
    printf("=====================================\n");
    printf("=============== Livro ===============\n\n");
    printf("1 - Registrar Livros Novos\n");
    printf("2 - Editar Registro de Livro\n");
    printf("3 - Alugar Livro\n");
    printf("4 - Devolver Livro\n");
    printf("5 - Listar livros\n");
    printf("6 - Listar livros por autor\n");
    printf("0 - Retornar para o menu principal\n");
    printf("=====================================\n");
    printf("Escolha uma opção: ");
    scanf("%d", &escolha);

    switch (escolha) {
    case 1:
      registrar_livro();
      break;
    case 2:
      editor_livro(titulo);
      break;
    case 3:
      emprestimo_livro();
      break;
    case 4:
      devolucao_livro();
      break;
    case 5:
      listar_livros();
      break;
    case 6:
      listar_livros_por_autor();
      break;
    case 0:
      printf("Retornando para o menu principal.\n");
      break;
    default:
      printf("Opção inválida. Tente novamente.\n");
    }
  }
  return 0;
}


void listar_livros() {
    FILE *arquivo;
    arquivo = fopen("biblioteca.txt", "r"); // Abre o arquivo em modo de leitura

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    printf("\n === Lista de Livros ===\n");

    char linha[100];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        printf("%s", linha); // Exibe cada linha do arquivo
    }

    fclose(arquivo);

    printf("Pressione Enter para sair...\n");
    while (getchar() != '\n');  // Limpa o buffer de entrada
    getchar(); // Aguarda Enter
}

void listar_livros_por_autor() {
    FILE *arquivo;
    arquivo = fopen("biblioteca.txt", "r"); // Abre o arquivo em modo de leitura

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char autor[150];
    printf("\nDigite o nome do autor: ");
    scanf(" %[^\n]", autor);

    printf("\n === Lista de Livros do Autor %s ===\n", autor);

    int encontrados = 0;
    int tempNumLivros = 0;  // Variável temporária para controlar a leitura dos livros

    while (tempNumLivros < MAX_LIVROS &&
           fscanf(arquivo, "%s %s %d %d %s",
                  biblioteca[tempNumLivros].titulo, biblioteca[tempNumLivros].autor,
                  &biblioteca[tempNumLivros].quant, &biblioteca[tempNumLivros].local, biblioteca[tempNumLivros].isbn) == 5) {

        if (strcmp(biblioteca[tempNumLivros].autor, autor) == 0) {
            printf("Título: %s, Quantidade: %d, Local: %d, ISBN: %s\n",
                   biblioteca[tempNumLivros].titulo, biblioteca[tempNumLivros].quant,
                   biblioteca[tempNumLivros].local, biblioteca[tempNumLivros].isbn);
            encontrados++;
        }

        tempNumLivros++;
    }

    fclose(arquivo);

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("Pressione Enter para sair...\n");
    getchar();

    if (encontrados == 0) {
        printf("Nenhum livro encontrado para o autor %s.\n", autor);
    }
}

void registrar_livro() {
    FILE *file;
    file = fopen("biblioteca.txt", "a");  // Open file in append mode

    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    if (numLivros < MAX_LIVROS) {
        printf("\n === Registrar Livro ===\n");

        printf("Nome do livro: ");
        scanf(" %[^\n]", biblioteca[numLivros].titulo);
        fprintf(file, "Nome do livro: %s\n", biblioteca[numLivros].titulo);

        printf("Autor do livro: ");
        scanf(" %[^\n]", biblioteca[numLivros].autor);
        fprintf(file, "Autor do livro: %s\n", biblioteca[numLivros].autor);

        printf("Quantidade de cópias do livro: ");
        scanf("%d", &biblioteca[numLivros].quant);
        fprintf(file, "Quantidade de cópias do livro: %d\n", biblioteca[numLivros].quant);

        biblioteca[numLivros].quant_disp = biblioteca[numLivros].quant;
        fprintf(file, "Quantidade de cópias do livro: %d\n", biblioteca[numLivros].quant_disp);

        fprintf(file, "Lista de CPFs: %d\n", biblioteca[numLivros].quant_disp);

        printf("Número do local: ");
        scanf("%d", &biblioteca[numLivros].local);
        fprintf(file, "Número do local: %d\n", biblioteca[numLivros].local);

        printf("ISBN: ");
        scanf(" %19[^\n]%*c", biblioteca[numLivros].isbn);
        fprintf(file, "ISBN: %.19s\n", biblioteca[numLivros].isbn);

        numLivros++;
    } else {
        printf("A biblioteca está cheia. Não é possível adicionar mais livros.\n");
    }

    fclose(file);  // Close the file
}

bool validar_isbn(const char *isbn) {
  if (isbn == NULL) {
    return false;
  }
  if (strlen(isbn) != 13) {
    return false; 
  }
  for (int i = 0; isbn[i] != '\0'; i++) {
    if (!isdigit(isbn[i]) && !(i == 12 && isbn[i] == 'X')) {
      return false; // ISBN inválido se contiver caracteres não permitidos
    }
  }

  // Calcula o dígito de verificação
  int soma = 0;
  for (int i = 0; i < 12; i++) {
    int digito = isbn[i] - '0';
    soma += (i % 2 == 0) ? digito : digito * 3;
  }
  int digitoVerificador = (10 - (soma % 10)) % 10;

  // Verifica o dígito de verificação
  if (isbn[12] != (digitoVerificador + '0')) {
    return false; // ISBN inválido se o dígito de verificação estiver errado
  }

  return true; // ISBN válido
}
