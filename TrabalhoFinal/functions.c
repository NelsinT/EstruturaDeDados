#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void abrir_dia() {
    printf("\t\t\t\t\t\t\tDia aberto..\n\n");
}

void fechar_dia() {
    FILE *file = fopen("vendas_do_dia.csv", "w");
    if (file == NULL) {
        printf("Erro ao criar o ficheiro de vendas.\n");
        return;
    }
    fprintf(file, "Exemplo de dados de vendas\n");
    fclose(file);
    printf("Dia fechado. Dados salvos e quantidades atualizadas.\n");
}

void adicionar_produto_pedido(Mesa *mesa, Produto produto, int quantidade) {
    if (strcmp(mesa->estado, "livre") == 0) {
        Pedido *novo_pedido = (Pedido *)malloc(sizeof(Pedido));
        novo_pedido->numero = 1; 
        strcpy(novo_pedido->mesa, mesa->nome);
        novo_pedido->funcionario = NULL; 
        novo_pedido->produtos[0] = produto;
        novo_pedido->quantidade[0] = quantidade;
        novo_pedido->total_produtos = 1;
        novo_pedido->valor_a_pagar = produto.preco * quantidade;
        novo_pedido->prox = NULL;
        mesa->pedidos_abertos = novo_pedido;
        strcpy(mesa->estado, "ocupada");
    } else {
        Pedido *pedido = mesa->pedidos_abertos;
        while (pedido->prox != NULL) {
            pedido = pedido->prox;
        }
        pedido->produtos[pedido->total_produtos] = produto;
        pedido->quantidade[pedido->total_produtos] = quantidade;
        pedido->total_produtos++;
        pedido->valor_a_pagar += produto.preco * quantidade;
    }
    produto.quantidade -= quantidade;
    printf("Produto adicionado ao pedido.\n");
}

void pagar_pedido(Mesa *mesa, int numero_pedido) {
    Pedido *pedido = mesa->pedidos_abertos;
    Pedido *anterior = NULL;

    while (pedido != NULL && pedido->numero != numero_pedido) {
        anterior = pedido;
        pedido = pedido->prox;
    }

    if (pedido != NULL) {
        if (anterior != NULL) {
            anterior->prox = pedido->prox;
        } else {
            mesa->pedidos_abertos = pedido->prox;
        }
        pedido->prox = mesa->pedidos_fechados;
        mesa->pedidos_fechados = pedido;
        printf("Pedido %d pago e fechado.\n", numero_pedido);
    } else {
        printf("Pedido %d nao encontrado.\n", numero_pedido);
    }
}

void eliminar_pedido_aberto(Mesa *mesa, int numero_pedido) {
    Pedido *pedido = mesa->pedidos_abertos;
    Pedido *anterior = NULL;

    while (pedido != NULL && pedido->numero != numero_pedido) {
        anterior = pedido;
        pedido = pedido->prox;
    }

    if (pedido != NULL) {
        if (anterior != NULL) {
            anterior->prox = pedido->prox;
        } else {
            mesa->pedidos_abertos = pedido->prox;
        }

        for (int i = 0; i < pedido->total_produtos; i++) {
            pedido->produtos[i].quantidade += pedido->quantidade[i];
        }

        free(pedido);
        printf("Pedido %d eliminado e quantidades repostas.\n", numero_pedido);
    } else {
        printf("Pedido %d não encontrado.\n", numero_pedido);
    }
}

void registrar_funcionario(Funcionario *funcionarios, int *total_funcionarios) {
    printf("Nome do funcionário: ");
    scanf("%s", funcionarios[*total_funcionarios].nome);
    printf("Função: ");
    scanf("%s", funcionarios[*total_funcionarios].funcao);
    printf("Disponibilidade: ");
    scanf("%s", funcionarios[*total_funcionarios].disponibilidade);
    strcpy(funcionarios[*total_funcionarios].status, "ativo");  // Definindo status como ativo

    (*total_funcionarios)++;
    printf("Funcionário registrado com sucesso.\n");

    // Salvar os funcionários no arquivo binário após o registro
    salvar_funcionarios(funcionarios, *total_funcionarios, "funcionarios.bin");
}

void listar_funcionarios(Funcionario *funcionarios, int total_funcionarios) {
    if (total_funcionarios == 0) {
        printf("Nenhum funcionário registrado.\n");
        return;
    }

    for (int i = 0; i < total_funcionarios; i++) {
        printf("Nome: %s, Função: %s, Disponibilidade: %s, Status: %s\n",
               funcionarios[i].nome,
               funcionarios[i].funcao,
               funcionarios[i].disponibilidade,
               funcionarios[i].status);
    }
}


void salvar_funcionarios(Funcionario *funcionarios, int total_funcionarios, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }
    fwrite(&total_funcionarios, sizeof(int), 1, file);
    fwrite(funcionarios, sizeof(Funcionario), total_funcionarios, file);
    fclose(file);
    printf("Funcionários salvos com sucesso.\n");
}

void carregar_funcionarios(Funcionario *funcionarios, int *total_funcionarios, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return;
    }
    fread(total_funcionarios, sizeof(int), 1, file);
    fread(funcionarios, sizeof(Funcionario), *total_funcionarios, file);
    fclose(file);
    printf("Funcionários carregados com sucesso.\n");
}
void editar_funcionario(Funcionario *funcionarios, int total_funcionarios) {
    char nome[50];
    printf("Digite o nome do funcionário a ser editado: ");
    scanf("%s", nome);

    for (int i = 0; i < total_funcionarios; i++) {
        if (strcmp(funcionarios[i].nome, nome) == 0) {
            printf("Editar (1)Nome (2)Função (3)Disponibilidade: ");
            int opcao;
            scanf("%d", &opcao);

            switch (opcao) {
                case 1:
                    printf("Novo Nome: ");
                    scanf("%s", funcionarios[i].nome);
                    break;
                case 2:
                    printf("Nova Função: ");
                    scanf("%s", funcionarios[i].funcao);
                    break;
                case 3:
                    printf("Nova Disponibilidade: ");
                    scanf("%s", funcionarios[i].disponibilidade);
                    break;
                default:
                    printf("Opção inválida.\n");
            }

            salvar_funcionarios(funcionarios, total_funcionarios, "funcionarios.bin");
            printf("Funcionário editado com sucesso.\n");
            return;
        }
    }
    printf("Funcionário não encontrado.\n");
}
void remover_funcionario(Funcionario *funcionarios, int *total_funcionarios) {
    char nome[50];
    printf("Digite o nome do funcionário a ser removido: ");
    scanf("%s", nome);

    for (int i = 0; i < *total_funcionarios; i++) {
        if (strcmp(funcionarios[i].nome, nome) == 0) {
            for (int j = i; j < *total_funcionarios - 1; j++) {
                funcionarios[j] = funcionarios[j + 1];
            }
            (*total_funcionarios)--;
            salvar_funcionarios(funcionarios, *total_funcionarios, "funcionarios.bin");
            printf("Funcionário removido com sucesso.\n");
            return;
        }
    }
    printf("Funcionário não encontrado.\n");
}
void inativar_funcionario(Funcionario *funcionarios, int total_funcionarios) {
    char nome[50];
    printf("Digite o nome do funcionário a ser inativado: ");
    scanf("%s", nome);

    for (int i = 0; i < total_funcionarios; i++) {
        if (strcmp(funcionarios[i].nome, nome) == 0) {
            strcpy(funcionarios[i].status, "inativo");
            salvar_funcionarios(funcionarios, total_funcionarios, "funcionarios.bin");
            printf("Funcionário inativado com sucesso.\n");
            return;
        }
    }
    printf("Funcionário não encontrado.\n");
}