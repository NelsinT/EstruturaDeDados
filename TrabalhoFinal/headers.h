#ifndef HEADERS_H
#define HEADERS_H

typedef struct {
    char nome[50];
    float preco;
    int quantidade;
} Produto;

typedef struct Pedido {
    int numero;
    char mesa[50];
    struct Funcionario *funcionario;
    Produto produtos[50];
    int quantidade[50];
    int total_produtos;
    float valor_a_pagar;
    struct Pedido *prox;
} Pedido;

typedef struct {
    char nome[50];
    Pedido *pedidos_abertos;
    Pedido *pedidos_fechados;
    char estado[20];
} Mesa;

typedef struct {
    char nome[50];
    char funcao[50];
    char disponibilidade[50];
    char status[20];
} Funcionario;

void abrir_dia();
void fechar_dia();
void adicionar_produto_pedido(Mesa *mesa, Produto *produtos, int total_produtos, char *nome_produto, int quantidade);
void pagar_pedido(Mesa *mesa, int numero_pedido);
void eliminar_pedido_aberto(Mesa *mesa, int numero_pedido, Produto *produtos, int total_produtos);
void registrar_produto(Produto *produtos, int *total_produtos);
void listar_produtos(Produto *produtos, int total_produtos);
void editar_produto(Produto *produtos, int total_produtos, char *nome);
void remover_produto(Produto *produtos, int *total_produtos, char *nome);
void tratar_notas_encomenda(Produto *produtos, int total_produtos);
void registrar_funcionario(Funcionario *funcionarios, int *total_funcionarios);
void listar_funcionarios(Funcionario *funcionarios, int total_funcionarios);
void salvar_funcionarios(Funcionario *funcionarios, int total_funcionarios, const char *filename);
void carregar_funcionarios(Funcionario *funcionarios, int *total_funcionarios, const char *filename);
void editar_funcionario(Funcionario *funcionarios, int total_funcionarios);
void remover_funcionario(Funcionario *funcionarios, int *total_funcionarios);
void tratar_pedidos(Mesa *mesas, int total_mesas, Produto *produtos, int total_produtos, Funcionario *funcionarios, int total_funcionarios);

#endif