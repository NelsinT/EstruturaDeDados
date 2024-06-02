#include "headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void abrir_dia()
{
    printf("Dia aberto.\n");
}

void fechar_dia()
{
    printf("Dia fechado.\n");
    exit(0);
}

void adicionar_produto_pedido(Mesa *mesa, Produto *produtos, int total_produtos, char *nome_produto, int quantidade)
{
    Produto *produto = NULL;
    for (int i = 0; i < total_produtos; i++)
    {
        if (strcmp(produtos[i].nome, nome_produto) == 0)
        {
            produto = &produtos[i];
            break;
        }
    }
    if (produto == NULL)
    {
        printf("Produto nao encontrado.\n");
        return;
    }
    if (produto->quantidade < quantidade)
    {
        printf("Quantidade insuficiente de %s.\n", produto->nome);
        return;
    }

    if (mesa->pedidos_abertos == NULL)
    {
        Pedido *novo_pedido = (Pedido *)malloc(sizeof(Pedido));
        novo_pedido->numero = 1;
        strcpy(novo_pedido->mesa, mesa->nome);
        novo_pedido->funcionario = NULL;
        novo_pedido->produtos[0] = *produto;
        novo_pedido->quantidade[0] = quantidade;
        novo_pedido->total_produtos = 1;
        novo_pedido->valor_a_pagar = produto->preco * quantidade;
        novo_pedido->prox = NULL;
        mesa->pedidos_abertos = novo_pedido;
        strcpy(mesa->estado, "ocupada");
    }
    else
    {
        Pedido *pedido = mesa->pedidos_abertos;
        int i;
        for (i = 0; i < pedido->total_produtos; i++)
        {
            if (strcmp(pedido->produtos[i].nome, nome_produto) == 0)
            {
                pedido->quantidade[i] += quantidade;
                break;
            }
        }
        if (i == pedido->total_produtos)
        {
            pedido->produtos[pedido->total_produtos] = *produto;
            pedido->quantidade[pedido->total_produtos] = quantidade;
            pedido->total_produtos++;
        }
        pedido->valor_a_pagar += produto->preco * quantidade;
    }
    produto->quantidade -= quantidade;
    printf("Produto adicionado ao pedido.\n");
}

void pagar_pedido(Mesa *mesa, int numero_pedido)
{
    if (mesa->pedidos_abertos == NULL)
    {
        printf("Nao ha pedidos abertos para esta mesa.\n");
        return;
    }

    Pedido *pedido = mesa->pedidos_abertos;
    mesa->pedidos_abertos = NULL;
    strcpy(mesa->estado, "disponivel");

    if (mesa->pedidos_fechados == NULL)
    {
        mesa->pedidos_fechados = pedido;
    }
    else
    {
        Pedido *aux = mesa->pedidos_fechados;
        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = pedido;
    }
    printf("Pedido pago e fechado.\n");

    salvar_pedidos_csv(mesa, 1, "pedidos.csv");
}

void eliminar_pedido_aberto(Mesa *mesa, int numero_pedido, Produto *produtos, int total_produtos)
{
    if (mesa->pedidos_abertos == NULL)
    {
        printf("Nao ha pedidos abertos para esta mesa.\n");
        return;
    }

    Pedido *pedido = mesa->pedidos_abertos;
    for (int i = 0; i < pedido->total_produtos; i++)
    {
        for (int j = 0; j < total_produtos; j++)
        {
            if (strcmp(pedido->produtos[i].nome, produtos[j].nome) == 0)
            {
                produtos[j].quantidade += pedido->quantidade[i];
                break;
            }
        }
    }

    mesa->pedidos_abertos = NULL;
    strcpy(mesa->estado, "disponivel");
    free(pedido);

    printf("Pedido eliminado.\n");
}

void registrar_produto(Produto *produtos, int *total_produtos)
{
    Produto novo_produto;
    printf("Nome do produto: ");
    scanf("%s", novo_produto.nome);
    printf("Preco: ");
    scanf("%f", &novo_produto.preco);
    printf("Quantidade: ");
    scanf("%d", &novo_produto.quantidade);

    produtos[*total_produtos] = novo_produto;
    (*total_produtos)++;

    FILE *file = fopen("produtos.bin", "wb");
    fwrite(produtos, sizeof(Produto), *total_produtos, file);
    fclose(file);

    printf("Produto registrado com sucesso.\n");
}

void listar_produtos(Produto *produtos, int total_produtos)
{
    printf("Lista de Produtos:\n");
    for (int i = 0; i < total_produtos; i++)
    {
        printf("Nome: %s, Preco: %.2f, Quantidade: %d\n",
               produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }
}

void editar_produto(Produto *produtos, int total_produtos, char *nome)
{
    Produto *produto = NULL;
    for (int i = 0; i < total_produtos; i++)
    {
        if (strcmp(produtos[i].nome, nome) == 0)
        {
            produto = &produtos[i];
            break;
        }
    }
    if (produto == NULL)
    {
        printf("Produto nao encontrado.\n");
        return;
    }

    printf("Novo preco: ");
    scanf("%f", &produto->preco);
    printf("Nova quantidade: ");
    scanf("%d", &produto->quantidade);

    FILE *file = fopen("produtos.bin", "wb");
    fwrite(produtos, sizeof(Produto), total_produtos, file);
    fclose(file);

    printf("Produto editado com sucesso.\n");
}

void remover_produto(Produto *produtos, int *total_produtos, char *nome)
{
    int index = -1;
    for (int i = 0; i < *total_produtos; i++)
    {
        if (strcmp(produtos[i].nome, nome) == 0)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        printf("Produto nao encontrado.\n");
        return;
    }

    for (int i = index; i < *total_produtos - 1; i++)
    {
        produtos[i] = produtos[i + 1];
    }
    (*total_produtos)--;

    FILE *file = fopen("produtos.bin", "wb");
    fwrite(produtos, sizeof(Produto), *total_produtos, file);
    fclose(file);

    printf("Produto removido com sucesso.\n");
}
void tratar_notas_encomenda(Produto *produtos, int total_produtos)
{
    FILE *file = fopen("notas_encomenda.txt", "a");
    if (!file)
    {
        printf("Erro ao abrir o arquivo de notas de encomenda.\n");
        return;
    }

    int produtos_inseridos = 0;

    for (int i = 0; i < total_produtos; i++)
    {
        if (produtos[i].quantidade < 5)
        {
            fprintf(file, "Produto: %s, Quantidade: %d\n", produtos[i].nome, produtos[i].quantidade);
            produtos[i].quantidade = 0;
            produtos_inseridos++;
        }
    }

    fclose(file);

    if (produtos_inseridos > 0)
    {
        printf("%d produtos foram adicionados nas notas de encomenda.\n", produtos_inseridos);
    }
    else
    {
        printf("Nenhum produto precisa ser adicionado nas notas de encomenda.\n");
    }
}

void registrar_funcionario(Funcionario *funcionarios, int *total_funcionarios)
{
    Funcionario novo_funcionario;
    printf("Nome do funcionario: ");
    scanf("%s", novo_funcionario.nome);
    printf("Funcao: ");
    scanf("%s", novo_funcionario.funcao);
    printf("Disponibilidade: ");
    scanf("%s", novo_funcionario.disponibilidade);
    strcpy(novo_funcionario.status, "ativo");

    funcionarios[*total_funcionarios] = novo_funcionario;
    (*total_funcionarios)++;

    salvar_funcionarios(funcionarios, *total_funcionarios, "funcionarios.bin");
    printf("Funcionario registrado com sucesso.\n");
}

void listar_funcionarios(Funcionario *funcionarios, int total_funcionarios)
{
    printf("Lista de Funcionarios:\n");
    for (int i = 0; i < total_funcionarios; i++)
    {
        printf("Nome: %s, Funcao: %s, Disponibilidade: %s, Status: %s\n",
               funcionarios[i].nome, funcionarios[i].funcao, funcionarios[i].disponibilidade, funcionarios[i].status);
    }
}

void salvar_funcionarios(Funcionario *funcionarios, int total_funcionarios, const char *filename)
{
    FILE *file = fopen(filename, "wb");
    fwrite(funcionarios, sizeof(Funcionario), total_funcionarios, file);
    fclose(file);
}

void carregar_funcionarios(Funcionario *funcionarios, int *total_funcionarios, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file != NULL)
    {
        while (fread(&funcionarios[*total_funcionarios], sizeof(Funcionario), 1, file))
        {
            (*total_funcionarios)++;
        }
        fclose(file);
    }
}

void editar_funcionario(Funcionario *funcionarios, int total_funcionarios)
{
    char nome[50];
    printf("Nome do funcionario a ser editado: ");
    scanf("%s", nome);

    Funcionario *funcionario = NULL;
    for (int i = 0; i < total_funcionarios; i++)
    {
        if (strcmp(funcionarios[i].nome, nome) == 0)
        {
            funcionario = &funcionarios[i];
            break;
        }
    }
    if (funcionario == NULL)
    {
        printf("Funcionario nao encontrado.\n");
        return;
    }

    printf("Nova funcao: ");
    scanf("%s", funcionario->funcao);
    printf("Nova disponibilidade: ");
    scanf("%s", funcionario->disponibilidade);
    printf("Novo status: ");
    scanf("%s", funcionario->status);

    salvar_funcionarios(funcionarios, total_funcionarios, "funcionarios.bin");
    printf("Funcionario editado com sucesso.\n");
}

void remover_funcionario(Funcionario *funcionarios, int *total_funcionarios)
{
    char nome[50];
    printf("Nome do funcionario a ser removido: ");
    scanf("%s", nome);

    int index = -1;
    for (int i = 0; i < *total_funcionarios; i++)
    {
        if (strcmp(funcionarios[i].nome, nome) == 0)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
    {
        printf("Funcionario nao encontrado.\n");
        return;
    }

    for (int i = index; i < *total_funcionarios - 1; i++)
    {
        funcionarios[i] = funcionarios[i + 1];
    }
    (*total_funcionarios)--;

    salvar_funcionarios(funcionarios, *total_funcionarios, "funcionarios.bin");
    printf("Funcionario removido com sucesso.\n");
}

void tratar_pedidos(Mesa *mesas, int total_mesas, Produto *produtos, int total_produtos, Funcionario *funcionarios, int total_funcionarios)
{
    char nome_mesa[50];
    char nome_produto[50];
    int quantidade, opcao, numero_pedido;

    printf("1. Adicionar produto a um pedido\n");
    printf("2. Pagar pedido\n");
    printf("3. Eliminar pedido aberto\n");
    printf("4. Criar novo pedido\n");
    scanf("%d", &opcao);

    switch (opcao)
    {
    case 1:
        printf("Nome da mesa: ");
        scanf("%s", nome_mesa);
        printf("Nome do produto: ");
        scanf("%s", nome_produto);
        printf("Quantidade: ");
        scanf("%d", &quantidade);
        for (int i = 0; i < total_mesas; i++)
        {
            if (strcmp(mesas[i].nome, nome_mesa) == 0)
            {
                adicionar_produto_pedido(&mesas[i], produtos, total_produtos, nome_produto, quantidade);
                break;
            }
        }
        break;
    case 2:
        printf("Nome da mesa: ");
        scanf("%s", nome_mesa);
        printf("Numero do pedido: ");
        scanf("%d", &numero_pedido);
        for (int i = 0; i < total_mesas; i++)
        {
            if (strcmp(mesas[i].nome, nome_mesa) == 0)
            {
                pagar_pedido(&mesas[i], numero_pedido);
                break;
            }
        }
        break;
    case 3:
        printf("Nome da mesa: ");
        scanf("%s", nome_mesa);
        printf("Numero do pedido: ");
        scanf("%d", &numero_pedido);
        for (int i = 0; i < total_mesas; i++)
        {
            if (strcmp(mesas[i].nome, nome_mesa) == 0)
            {
                eliminar_pedido_aberto(&mesas[i], numero_pedido, produtos, total_produtos);
                break;
            }
        }
        break;
    case 4:
        criar_novo_pedido(mesas, total_mesas, produtos, total_produtos, funcionarios, total_funcionarios);

    default:
        printf("Opcao invalida.\n");
    }
}
void listar_mesas(Mesa *mesas, int total_mesas)
{
    printf("Lista de Mesas:\n");
    for (int i = 0; i < total_mesas; i++)
    {
        printf("Numero: %d, Nome: %s, Estado: %s\n",
               mesas[i].numero, mesas[i].nome, mesas[i].estado);
    }
}

void criar_mesas(Mesa *mesas, int *num_mesas, int lugares_por_mesa)
{
    Mesa nova_mesa;
    nova_mesa.numero = *num_mesas + 1;
    printf("Nome da mesa: ");
    scanf("%s", nova_mesa.nome);
    strcpy(nova_mesa.estado, "disponivel");
    nova_mesa.pedidos_abertos = NULL;
    nova_mesa.pedidos_fechados = NULL;

    mesas[*num_mesas] = nova_mesa;
    (*num_mesas)++;

    FILE *file = fopen("mesas.bin", "wb");
    fwrite(mesas, sizeof(Mesa), *num_mesas, file);
    fclose(file);

    printf("Mesa criada com sucesso.\n");
}

void opcoes_mesas(Mesa mesas[], int *total_mesas, int *lugares_por_mesa)
{
    int opcao;
    do
    {
        printf("\n\t\t\tGerir Mesas:\n");
        printf("\t1. Listar Mesas\n");
        printf("\t2. Criar Mesa\n");
        printf("\t3. Verificar Estado da Mesa\n");
        printf("\t0. Voltar ao Menu Principal\n");
        printf("\n\tEscolha uma opcao: \n ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            listar_mesas(mesas, *total_mesas);
            break;

        case 2:
            criar_mesas(mesas, total_mesas, *lugares_por_mesa);
            break;

        case 3:
            verificar_estado_mesa(mesas, *total_mesas);
            break;

        case 0:
            break;

        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);
}

void verificar_estado_mesa(Mesa mesas[], int total_mesas)
{
    char nome_mesa[50];
    printf("Nome da mesa a ser verificada: ");
    scanf("%s", nome_mesa);

    Mesa *mesa = NULL;
    for (int i = 0; i < total_mesas; i++)
    {
        if (strcmp(mesas[i].nome, nome_mesa) == 0)
        {
            mesa = &mesas[i];
            break;
        }
    }
    if (mesa == NULL)
    {
        printf("Mesa nao encontrada.\n");
        return;
    }

    printf("Mesa: %s, Lugares: %d, Tipo: %s, Estado: %s\n",
           mesa->nome, mesa->estado);
}

void criar_novo_pedido(Mesa *mesas, int total_mesas, Produto *produtos, int total_produtos, Funcionario *funcionarios, int total_funcionarios)
{
    printf("Mesas disponiveis:\n");
    for (int i = 0; i < total_mesas; i++)
    {
        if (strcmp(mesas[i].estado, "disponivel") == 0)
        {
            printf("Numero: %d, Nome: %s\n", mesas[i].numero, mesas[i].nome);
        }
    }

    int numero_mesa;
    printf("Numero da mesa: ");
    scanf("%d", &numero_mesa);

    Mesa *mesa_selecionada = NULL;
    for (int i = 0; i < total_mesas; i++)
    {
        if (mesas[i].numero == numero_mesa)
        {
            mesa_selecionada = &mesas[i];
            break;
        }
    }

    if (mesa_selecionada == NULL || strcmp(mesa_selecionada->estado, "ocupada") == 0)
    {
        printf("Mesa invalida ou ocupada.\n");
        return;
    }

    printf("Produtos disponiveis:\n");
    for (int i = 0; i < total_produtos; i++)
    {
        printf("Nome: %s, Preco: %.2f, Quantidade: %d\n",
               produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }

    char nome_produto[50];
    int quantidade;
    printf("Nome do produto: ");
    scanf("%s", nome_produto);
    printf("Quantidade: ");
    scanf("%d", &quantidade);

    adicionar_produto_pedido(mesa_selecionada, produtos, total_produtos, nome_produto, quantidade);
}

void salvar_pedidos_csv(Mesa *mesas, int total_mesas, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("Erro ao abrir o arquivo %s para escrita.\n", filename);
        return;
    }

    fprintf(file, "Numero Pedido;Nome Mesa;Produto;Quantidade;Preco;Valor a Pagar\n");

    for (int i = 0; i < total_mesas; i++)
    {
        Mesa *mesa = &mesas[i];
        Pedido *pedido = mesa->pedidos_fechados;
        while (pedido != NULL)
        {
            for (int j = 0; j < pedido->total_produtos; j++)
            {
                fprintf(file, "%d,%s,%s,%d,%.2f,%.2f\n",
                        pedido->numero,
                        mesa->nome,
                        pedido->produtos[j].nome,
                        pedido->quantidade[j],
                        pedido->produtos[j].preco,
                        pedido->valor_a_pagar);
            }
            pedido = pedido->prox;
        }
    }

    fclose(file);
    printf("Pedidos salvos em %s com sucesso.\n", filename);
}