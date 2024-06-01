#include "headers.h"
#include <stdio.h>

int main() {
    Funcionario funcionarios[100];
    int total_funcionarios = 0;

    abrir_dia();
    carregar_funcionarios(funcionarios, &total_funcionarios, "funcionarios.bin");

    int opcao;
    do {
        printf("\n\t\t\tMenu:\n");
        printf("\t1. Registrar Funcionario\n");
        printf("\t2. Listar Funcionarios\n");
        printf("\t3. Editar Funcionarios\n");
        printf("\t4. Pagar Pedido\n");
        printf("\t5. Eliminar Pedido Aberto\n");
        printf("\t6. Fechar o Dia\n");
        printf("\t0. Sair\n");
        printf("\n\tEscolha uma opcao: \n ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                registrar_funcionario(funcionarios, &total_funcionarios);
                break;

            case 2:
                listar_funcionarios(funcionarios, total_funcionarios);
                break;
                
            case 3: {
                int subopcao;
                do {
                    printf("\n\t\t\t Painel Funcionarios:\n");
                    printf("\t1. Editar Nome\n");
                    printf("\t2. Editar Funcao\n");
                    printf("\t3. Editar Disponibilidade\n");
                    printf("\t4. Remover Funcionario\n");
                    printf("\t5. Inativar Funcionario\n");
                    printf("\t0. Sair\n");
                    printf("\n\tEscolha uma opcao: \n ");
                    scanf("%d", &subopcao);

                    switch (subopcao) {
                        case 1:
                        case 2:
                        case 3:
                            editar_funcionario(funcionarios, total_funcionarios);
                            break;
                        case 4:
                            remover_funcionario(funcionarios, &total_funcionarios);
                            break;
                        case 5:
                            inativar_funcionario(funcionarios, total_funcionarios);
                            break;
                        case 0:
                            break;
                        default:
                            printf("Opção inválida. Tente novamente.\n");
                    }
                } while (subopcao != 0);
                break;
            }
            
            case 4:
                // Implementar pagamento de pedido
                break;
                
            case 5:
                // Implementar eliminação de pedido aberto
                break;
                
            case 6:
                fechar_dia();
                break;
                
            case 0:
                printf("Saindo...\n");
                break;
                
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
