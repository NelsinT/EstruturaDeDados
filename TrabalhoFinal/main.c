#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    Funcionario funcionarios[100];
    Mesa mesas[50];
    Produto produtos[100];
    int total_funcionarios = 0, total_mesas = 0, total_produtos = 0;

    abrir_dia();
    carregar_funcionarios(funcionarios, &total_funcionarios, "funcionarios.bin");

    // Load products from file
    FILE *file = fopen("produtos.bin", "rb");
    if (file != NULL)
    {
        while (fread(&produtos[total_produtos], sizeof(Produto), 1, file))
        {
            total_produtos++;
        }
        fclose(file);
    }

    int lugares_por_mesa; // Declara a variável lugares_por_mesa antes de usá-la

    int opcao;
    do
    {
        printf("\n\t\t\tMenu:\n");
        printf("\t1. Registrar Funcionario\n");
        printf("\t2. Listar Funcionarios\n");
        printf("\t3. Editar Funcionarios\n");
        printf("\t4. Remover Funcionarios\n");
        printf("\t5. Tratar Pedidos\n");
        printf("\t6. Gerir Mesas\n");
        printf("\t7. Fechar o Dia\n");
        printf("\t8. Registrar Produto\n");
        printf("\t9. Listar Produtos\n");
        printf("\t10. Editar Produto\n");
        printf("\t11. Remover Produto\n");
        printf("\t12. Tratar Notas de Encomenda\n");
        printf("\t0. Sair\n");
        printf("\n\tEscolha uma opcao: \n ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            registrar_funcionario(funcionarios, &total_funcionarios);
            break;

        case 2:
            listar_funcionarios(funcionarios, total_funcionarios);
            break;

        case 3:
            editar_funcionario(funcionarios, total_funcionarios);
            break;

        case 4:
            remover_funcionario(funcionarios, &total_funcionarios);
            break;

        case 5:
         tratar_pedidos(mesas, total_mesas, produtos, total_produtos, funcionarios, total_funcionarios);
            break;

        case 6:
        opcoes_mesas(mesas, total_mesas, lugares_por_mesa);
            break;

        case 7:
            fechar_dia();
            break;

        case 8:
            registrar_produto(produtos, &total_produtos);
            break;

        case 9:
            listar_produtos(produtos, total_produtos);
            break;

        case 10:
        {
            char nome_produto[50];
            printf("Nome do produto a ser editado: ");
            scanf("%s", nome_produto);
            editar_produto(produtos, total_produtos, nome_produto);
        }
        break;

        case 11:
        {
            char nome_produto[50];
            printf("Nome do produto a ser removido: ");
            scanf("%s", nome_produto);
            remover_produto(produtos, &total_produtos, nome_produto);
        }
        break;

        case 12:
            tratar_notas_encomenda(produtos, total_produtos);
            break;

        case 0:
            printf("Saindo...\n");
            break;

        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
