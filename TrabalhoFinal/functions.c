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
        printf("Produto não encontrado.\n");
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
    Pedido *anterior = NULL, *atual = mesa->pedidos_abertos;
    while (atual != NULL && atual->numero != numero_pedido)
    {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual == NULL)
    {
        printf("Pedido não encontrado.\n");
        return;
    }
    if (anterior == NULL)
    {
        mesa->pedidos_abertos = atual->prox;
    }
    else
    {
        anterior->prox = atual->prox;
    }
    atual->prox = mesa->pedidos_fechados;
    mesa->pedidos_fechados = atual;

    Pedido *p = mesa->pedidos_fechados;
    while (p != NULL)
    {
        Pedido *q = p->prox;
        while (q != NULL)
        {
            if (p->numero > q->numero)
            {
                Pedido temp = *p;
                *p = *q;
                *q = temp;
                Pedido *temp_ptr = p->prox;
                p->prox = q->prox;
                q->prox = temp_ptr;
            }
            q = q->prox;
        }
        p = p->prox;
    }

    printf("Pedido pago e movido para pedidos fechados.\n");
}

void eliminar_pedido_aberto(Mesa *mesa, int numero_pedido, Produto *produtos, int total_produtos)
{
    Pedido *anterior = NULL, *atual = mesa->pedidos_abertos;
    while (atual != NULL && atual->numero != numero_pedido)
    {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual == NULL)
    {
        printf("Pedido não encontrado.\n");
        return;
    }
    for (int i = 0; i < atual->total_produtos; i++)
    {
        for (int j = 0; j < total_produtos; j++)
        {
            if (strcmp(produtos[j].nome, atual->produtos[i].nome) == 0)
            {
                produtos[j].quantidade += atual->quantidade[i];
                break;
            }
        }
    }
    if (anterior == NULL)
    {
        mesa->pedidos_abertos = atual->prox;
    }
    else
    {
        anterior->prox = atual->prox;
    }
    free(atual);
    printf("Pedido eliminado e quantidades repostas.\n");
}

void registrar_produto(Produto *produtos, int *total_produtos)
{
    printf("Nome do produto: ");
    scanf(" %[^\n]", produtos[*total_produtos].nome);
    printf("Preco do produto: ");
    scanf("%f", &produtos[*total_produtos].preco);
    printf("Quantidade disponivel: ");
    scanf("%d", &produtos[*total_produtos].quantidade);
    (*total_produtos)++;
    printf("Produto registrado.\n");

    // Abrir o arquivo "produtos.bin" para escrita em modo binário
    FILE *arquivo = fopen("produtos.bin", "ab");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo 'produtos.bin' para escrita.\n");
        return;
    }

    // Escrever o produto no arquivo binário
    fwrite(&produtos[*total_produtos - 1], sizeof(Produto), 1, arquivo);

    // Fechar o arquivo
    fclose(arquivo);
}

void listar_produtos(Produto *produtos, int total_produtos)
{
    FILE *arquivo = fopen("produtos.bin", "rb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo 'produtos.bin' para leitura.\n");
        return;
    }

    Produto produto;
    int contador = 1;
    while (fread(&produto, sizeof(Produto), 1, arquivo))
    {
        printf("Produto %d:\n", contador);
        printf("Nome: %s\n", produto.nome);
        printf("Preco: %.2f\n", produto.preco);
        printf("Quantidade disponivel: %d\n", produto.quantidade);
        printf("\n");
        contador++;
    }

    fclose(arquivo);
}

void editar_produto(Produto *produtos, int total_produtos, char *nome)
{
    FILE *arquivo_original = fopen("produtos.bin", "rb");
    if (arquivo_original == NULL)
    {
        printf("Erro ao abrir o arquivo 'produtos.bin' para leitura.\n");
        return;
    }

    FILE *arquivo_temporario = fopen("temporario.bin", "wb");
    if (arquivo_temporario == NULL)
    {
        printf("Erro ao abrir o arquivo 'temporario.bin' para escrita.\n");
        fclose(arquivo_original);
        return;
    }

    Produto produto;
    int encontrado = 0;
    while (fread(&produto, sizeof(Produto), 1, arquivo_original))
    {
        if (strcmp(produto.nome, nome) == 0)
        {
            encontrado = 1;
            printf("Novo preco do produto '%s': ", nome);
            scanf("%f", &produto.preco);
            printf("Nova quantidade disponivel do produto '%s': ", nome);
            scanf("%d", &produto.quantidade);
        }
        fwrite(&produto, sizeof(Produto), 1, arquivo_temporario);
    }

    fclose(arquivo_original);
    fclose(arquivo_temporario);

    if (!encontrado)
    {
        printf("Produto '%s' não encontrado.\n", nome);
        remove("temporario.bin");
        return;
    }

    if (remove("produtos.bin") != 0)
    {
        printf("Erro ao remover o arquivo 'produtos.bin'.\n");
        return;
    }

    if (rename("temporario.bin", "produtos.bin") != 0)
    {
        printf("Erro ao renomear o arquivo 'temporario.bin'.\n");
        return;
    }

    printf("Produto '%s' editado com sucesso.\n", nome);
}

void remover_produto(Produto *produtos, int *total_produtos, char *nome)
{
    FILE *arquivo_original = fopen("produtos.bin", "rb");
    if (arquivo_original == NULL)
    {
        printf("Erro ao abrir o arquivo 'produtos.bin' para leitura.\n");
        return;
    }

    FILE *arquivo_temporario = fopen("temporario.bin", "wb");
    if (arquivo_temporario == NULL)
    {
        printf("Erro ao abrir o arquivo 'temporario.bin' para escrita.\n");
        fclose(arquivo_original);
        return;
    }

    Produto produto;
    int removido = 0;
    while (fread(&produto, sizeof(Produto), 1, arquivo_original))
    {
        if (strcmp(produto.nome, nome) != 0)
        {
            fwrite(&produto, sizeof(Produto), 1, arquivo_temporario);
        }
        else
        {
            removido = 1;
        }
    }

    fclose(arquivo_original);
    fclose(arquivo_temporario);

    if (!removido)
    {
        printf("Produto '%s' não encontrado.\n", nome);
        remove("temporario.bin");
        return;
    }

    if (remove("produtos.bin") != 0)
    {
        printf("Erro ao remover o arquivo 'produtos.bin'.\n");
        return;
    }

    if (rename("temporario.bin", "produtos.bin") != 0)
    {
        printf("Erro ao renomear o arquivo 'temporario.bin'.\n");
        return;
    }

    printf("Produto '%s' removido com sucesso.\n", nome);
}

void tratar_notas_encomenda(Produto *produtos, int total_produtos)
{
    printf("Notas de encomenda:\n");
    for (int i = 0; i < total_produtos; i++)
    {
        if (produtos[i].quantidade < 10)
        {
            printf("Produto %s está com estoque baixo. Quantidade: %d\n", produtos[i].nome, produtos[i].quantidade);
        }
    }
}

void tratar_pedidos(Mesa *mesas, int total_mesas, Produto *produtos, int total_produtos, Funcionario *funcionarios, int total_funcionarios)
{
    char nome_mesa[50];
    char nome_produto[50];
    int quantidade, opcao, numero_pedido;

    printf("1. Adicionar produto a um pedido\n");
    printf("2. Pagar pedido\n");
    printf("3. Eliminar pedido aberto\n");
    printf("4. Criar novo pedido\n"); // Adicionando a opção para criar um novo pedido
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
        printf("Número do pedido: ");
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
        printf("Número do pedido: ");
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
        printf("Opção inválida.\n");
    }
}

void registrar_funcionario(Funcionario *funcionarios, int *total_funcionarios)
{
    printf("Nome do funcionario: ");
    scanf("%s", funcionarios[*total_funcionarios].nome);
    printf("Função: ");
    scanf("%s", funcionarios[*total_funcionarios].funcao);
    printf("Disponibilidade: ");
    scanf("%s", funcionarios[*total_funcionarios].disponibilidade);
    strcpy(funcionarios[*total_funcionarios].status, "ativo"); // Definindo status como ativo

    (*total_funcionarios)++;
    printf("Funcionário registrado com sucesso.\n");

    // Salvar os funcionários no arquivo binário após o registro
    salvar_funcionarios(funcionarios, *total_funcionarios, "funcionarios.bin");
}

void listar_funcionarios(Funcionario *funcionarios, int total_funcionarios)
{
    if (total_funcionarios == 0)
    {
        printf("Nenhum funcionario registrado.\n");
        return;
    }

    for (int i = 0; i < total_funcionarios; i++)
    {
        printf("Nome: %s, Funcao: %s, Disponibilidade: %s, Status: %s\n",
               funcionarios[i].nome,
               funcionarios[i].funcao,
               funcionarios[i].disponibilidade,
               funcionarios[i].status);
    }
}

void salvar_funcionarios(Funcionario *funcionarios, int total_funcionarios, const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }
    fwrite(&total_funcionarios, sizeof(int), 1, file);
    fwrite(funcionarios, sizeof(Funcionario), total_funcionarios, file);
    fclose(file);
    printf("Funcionarios salvos com sucesso.\n");
}

void carregar_funcionarios(Funcionario *funcionarios, int *total_funcionarios, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return;
    }
    fread(total_funcionarios, sizeof(int), 1, file);
    fread(funcionarios, sizeof(Funcionario), *total_funcionarios, file);
    fclose(file);
    printf("Funcionarios carregados com sucesso.\n");
}
void editar_funcionario(Funcionario *funcionarios, int total_funcionarios)
{
    char nome[50];
    printf("\nDigite o nome do funcionario a ser editado: ");
    scanf("%s", nome);

    for (int i = 0; i < total_funcionarios; i++)
    {
        if (strcmp(funcionarios[i].nome, nome) == 0)
        {
            printf("\nEscolha uma das opcoes para Editar o Funcionario \n\n(1)Nome \n(2)Funcao \n(3)Disponibilidade:\n(4)Alterar estado: \n");
            int opcao;
            scanf("%d", &opcao);

            switch (opcao)
            {
            case 1:
                printf("Novo Nome: ");
                scanf("%s", funcionarios[i].nome);
                break;
            case 2:
                printf("Nova Funcao: ");
                scanf("%s", funcionarios[i].funcao);
                break;
            case 3:
                printf("Nova Disponibilidade: ");
                scanf("%s", funcionarios[i].disponibilidade);
                break;
            case 4:
                printf("Novo Estado (ativo/inativo): ");
                scanf("%s", funcionarios[i].status);
                if (strcmp(funcionarios[i].status, "ativo") != 0 && strcmp(funcionarios[i].status, "inativo") != 0)
                {
                    printf("Estado inválido. O estado deve ser 'ativo' ou 'inativo'.\n");
                }
                break;

            default:
                printf("Opção invalida.\n");
            }

            salvar_funcionarios(funcionarios, total_funcionarios, "funcionarios.bin");
            printf("\nFuncionario editado com sucesso.\n");
            return;
        }
    }
    printf("Funcionario nao encontrado.\n");
}
void remover_funcionario(Funcionario *funcionarios, int *total_funcionarios)
{
    if (*total_funcionarios == 0)
    {
        printf("Nenhum funcionario para remover.\n");
        return;
    }
    else
    {
        char nome[50];
        printf("Digite o nome do funcionario a ser removido: ");
        scanf("%s", nome);

        for (int i = 0; i < *total_funcionarios; i++)
        {
            if (strcmp(funcionarios[i].nome, nome) == 0)
            {
                for (int j = i; j < *total_funcionarios - 1; j++)
                {
                    funcionarios[j] = funcionarios[j + 1];
                }
                (*total_funcionarios)--;
                salvar_funcionarios(funcionarios, *total_funcionarios, "funcionarios.bin");
                printf("Funcionario removido com sucesso.\n");
                return;
            }
        }
        printf("Funcionario nao encontrado.\n");
    }
}

void definir_estado_mesa(Mesa *mesa, char *estado)
{
    strcpy(mesa->estado, estado);
}

void adicionar_pedido_mesa(Mesa *mesa, Pedido *pedido)
{
    if (mesa->pedidos_abertos == NULL)
    {
        mesa->pedidos_abertos = pedido;
    }
    else
    {
        Pedido *temp = mesa->pedidos_abertos;
        while (temp->prox != NULL)
        {
            temp = temp->prox;
        }
        temp->prox = pedido;
    }
}

void listar_pedidos_mesa(Mesa *mesa)
{
    printf("Pedidos da mesa %s:\n", mesa->nome);
    Pedido *temp = mesa->pedidos_abertos;
    int contador = 1;
    while (temp != NULL)
    {
        printf("Pedido %d:\n", contador);
        temp = temp->prox;
        contador++;
    }
}
void listar_mesas(Mesa *mesas, int total_mesas)
{
    printf("Mesas disponiveis:\n");
    for (int i = 0; i < total_mesas; i++)
    {
        printf("%d. %s - %d lugares\n", i + 1, mesas[i].nome, mesas[i].lugares);
    }
}

void opcoes_mesas(Mesa *mesas, int total_mesas, int lugares_por_mesa) {
    int opcao;
    do {
        printf("\n--- Opcoes de Mesas ---\n");
        printf("1. Adicionar nova mesa\n");
        printf("2. Listar mesas existentes\n");
        printf("3. Verificar estado de uma mesa\n");
        printf("0. Voltar ao menu principal\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (total_mesas + 1 > 50) {
                    printf("Limite maximo de mesas atingido.\n");
                } else {
                    criar_mesas(mesas, &total_mesas, lugares_por_mesa);
                }
                break;
            case 2:
                // Listar mesas existentes
                listar_mesas(mesas, total_mesas);
                break;
            case 3:
                // Verificar estado de uma mesa
                verificar_estado_mesa(mesas, total_mesas);
                break;
            case 0:
                printf("Retornando ao menu principal...\n");
                break;
            default:
                printf("Opção invalida. Por favor, escolha uma opção valida.\n");
        }
    } while (opcao != 0);
}

    void verificar_estado_mesa(Mesa mesas[], int total_mesas)
    {
        printf("Estado das Mesas:\n");
        for (int i = 0; i < total_mesas; i++)
        {
            printf("Mesa %d: %s\n", i + 1, mesas[i].estado);
        }
    }

void criar_mesas(Mesa mesas[], int *num_mesas, int lugares_por_mesa) {
    if (*num_mesas + 1 <= 50) { // Check if there is space to add more tables
        printf("Quantos lugares deseja na mesa: ");
        scanf("%d", &lugares_por_mesa);

        strcpy(mesas[*num_mesas].nome, "Mesa ");
        sprintf(mesas[*num_mesas].nome + strlen(mesas[*num_mesas].nome), "%d", *num_mesas + 1);
        mesas[*num_mesas].lugares = lugares_por_mesa;
        strcpy(mesas[*num_mesas].tipo, "Normal");
        strcpy(mesas[*num_mesas].estado, "Livre");
        mesas[*num_mesas].pedidos_abertos = NULL;
        mesas[*num_mesas].pedidos_fechados = NULL;
        (*num_mesas)++;
        printf("Mesa criada com sucesso!\n");
    } else {
        printf("Limite de mesas atingido. Não é possível adicionar mais mesas.\n");
    }
}


void criar_novo_pedido(Mesa *mesas, int total_mesas, Produto *produtos, int total_produtos, Funcionario *funcionarios, int total_funcionarios) {
    
    char nome_mesa[50];
    int num_lugares;
    int num_produtos;
    int num_quantidades;
    int num_pedido = 0;

    // Pergunta ao usuário o nome da mesa e o número de lugares necessários
    printf("Nome da mesa: ");
    scanf("%s", nome_mesa);
    printf("Quantos lugares necessita? ");
    scanf("%d", &num_lugares);

    // Encontra a mesa pelo nome
    Mesa *mesa = NULL;
    for (int i = 0; i < total_mesas; i++) {
        if (strcmp(mesas[i].nome, nome_mesa) == 0) {
            mesa = &mesas[i];
            break;
        }
    }

    // Se a mesa não foi encontrada, exibe uma mensagem de erro e retorna
    if (mesa == NULL) {
        printf("Mesa não encontrada.\n");
        return;
    }

    // Verifica se há um funcionário disponível para atender a mesa
    int funcionario_disponivel = 0;
    for (int i = 0; i < total_funcionarios; i++) {
        if (strcmp(funcionarios[i].disponibilidade, "disponivel") == 0) {
            funcionario_disponivel = 1;
            break;
        }
    }

    // Se não houver funcionário disponível, exibe uma mensagem e retorna
    if (!funcionario_disponivel) {
        printf("Nenhum funcionário disponível para atender a mesa.\n");
        return;
    }

    // Cria um novo pedido
    Pedido *novo_pedido = (Pedido *)malloc(sizeof(Pedido));
    if (novo_pedido == NULL) {
        printf("Erro ao alocar memória para o novo pedido.\n");
        return;
    }

    // Preenche os detalhes do pedido
    novo_pedido->numero = ++num_pedido;
    strcpy(novo_pedido->mesa, mesa->nome);
    novo_pedido->funcionario = NULL;
    novo_pedido->total_produtos = 0;
    novo_pedido->valor_a_pagar = 0;
    novo_pedido->prox = NULL;

    // Pergunta ao usuário os produtos e suas quantidades
    printf("Quantos produtos deseja pedir? ");
    scanf("%d", &num_produtos);

    for (int i = 0; i < num_produtos; i++) {
        char nome_produto[50];
        printf("Nome do produto %d: ", i + 1);
        scanf("%s", nome_produto);

        // Encontra o produto pelo nome
        Produto *produto = NULL;
        for (int j = 0; j < total_produtos; j++) {
            if (strcmp(produtos[j].nome, nome_produto) == 0) {
                produto = &produtos[j];
                break;
            }
        }

        // Se o produto não foi encontrado, exibe uma mensagem de erro e continua
        if (produto == NULL) {
            printf("Produto '%s' não encontrado.\n", nome_produto);
            continue;
        }

        printf("Quantidade do produto %s: ", nome_produto);
        scanf("%d", &num_quantidades);

        // Verifica se a quantidade desejada está disponível
        if (num_quantidades > produto->quantidade) {
            printf("Quantidade insuficiente de %s.\n", produto->nome);
            continue;
        }

        // Adiciona o produto ao pedido
        novo_pedido->produtos[i] = *produto;
        novo_pedido->quantidade[i] = num_quantidades;
        novo_pedido->total_produtos++;
        novo_pedido->valor_a_pagar += produto->preco * num_quantidades;

        // Atualiza a quantidade disponível do produto
        produto->quantidade -= num_quantidades;
    }

    // Adiciona o pedido à lista de pedidos abertos da mesa
    if (mesa->pedidos_abertos == NULL) {
        mesa->pedidos_abertos = novo_pedido;
    } else {
        Pedido *temp = mesa->pedidos_abertos;
        while (temp->prox != NULL) {
            temp = temp->prox;
        }
        temp->prox = novo_pedido;
    }

    // Atribui um funcionário disponível ao pedido
    for (int i = 0; i < total_funcionarios; i++) {
        if (strcmp(funcionarios[i].disponibilidade, "disponivel") == 0) {
            novo_pedido->funcionario = &funcionarios[i];
            strcpy(funcionarios[i].disponibilidade, "ocupado");
            break;
        }
    }

    // Exibe uma mensagem de sucesso
    printf("Novo pedido registrado com sucesso.\n");

    // Salva os detalhes do pedido em um arquivo CSV
    salvar_detalhes_pedido_csv(novo_pedido);
}

void salvar_detalhes_pedido_csv(Pedido *pedido) {
    // Abre o arquivo CSV para escrita
    FILE *file = fopen("detalhes_pedidos.csv", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo 'detalhes_pedidos.csv' para escrita.\n");
        return;
    }

    // Escreve os detalhes do pedido no arquivo CSV
    fprintf(file, "%d, %s, %.2f\n", pedido->numero, pedido->mesa, pedido->valor_a_pagar);

    // Fecha o arquivo
    fclose(file);
}
