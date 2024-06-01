#ifndef HEADERS_H
#define HEADERS_H

typedef struct {
    char nome[50];
    char funcao[20];
    char disponibilidade[10];
    char status[10];
} Funcionario;

typedef struct {
    char nome[50];
    float preco;
    int quantidade;
} Produto;

typedef struct Pedido {
    int numero;
    char mesa[5];
    Funcionario *funcionario;
    Produto produtos[50];
    int quantidade[50];
    int total_produtos;
    float valor_a_pagar;
    struct Pedido *prox;
} Pedido;

typedef struct {
    char nome[5];
    int lugares;
    char localizacao[20];
    char estado[10];
    Pedido *pedidos_abertos;
    Pedido *pedidos_fechados;
} Mesa;

void abrir_dia();
void fechar_dia();
void adicionar_produto_pedido(Mesa *mesa, Produto produto, int quantidade);
void pagar_pedido(Mesa *mesa, int numero_pedido);
void eliminar_pedido_aberto(Mesa *mesa, int numero_pedido);
void registrar_funcionario(Funcionario *funcionarios, int *total_funcionarios);
void listar_funcionarios(Funcionario *funcionarios, int total_funcionarios);
void editar_funcionario(Funcionario *funcionarios, int total_funcionarios);
void remover_funcionario(Funcionario *funcionarios, int *total_funcionarios);
void inativar_funcionario(Funcionario *funcionarios, int total_funcionarios);
void salvar_funcionarios(Funcionario *funcionarios, int total_funcionarios, const char *filename);
void carregar_funcionarios(Funcionario *funcionarios, int *total_funcionarios, const char *filename);

#endif 
