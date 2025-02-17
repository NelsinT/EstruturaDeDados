#include <stdio.h>
#define linhas 2
#define colunas 2
void ler_matriz(int v[linhas][colunas])
{
    for (int i = 0; i < linhas; i++)
        for (int j = 0; j < colunas; j++)
        {
            printf("Introduza o valor da linha %d e da coluna %d: \n", i, j);
            scanf("%d", &v[i][j]);
        }
}

void print_matriz(int m3[linhas][colunas])
{

    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            printf("%d", m3[i][j]);
        }
        printf("\n");
    }
}
void calcular_media(int m3[linhas][colunas])
{
    int soma = 0;
    int soma_coluna = 0;
    float media;
    float media_coluna;
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {

            soma += m3[i][j];
        }

        media = soma / (linhas * colunas);
    }
    printf("a soma e  %d\n ", soma);
    printf("A media e : %f \n", media);
}
void calcular_media_coluna(int m3[linhas][colunas])
{
    int soma = 0;
    float media_coluna = 0;
    int c = 1;
    for (int i = 0; i < linhas; i++)

    {
        soma += m3[i][c];
    }
    media_coluna = soma / (linhas);
    printf("A media da coluna e  : %f",media_coluna);
}

void calcular_media_linha(int m3[linhas][colunas])
{
    int soma = 0;
    float media_linha = 0;
    int c = 1;
    for (int j = 0; j < colunas; j++)

    {
        soma += m3[c][j];
    }
    media_linha= soma / (colunas);
    printf("A media da linha e  : %f",media_linha);
}

int main()
{
    int mat1[linhas][colunas];
    ler_matriz(mat1);
    print_matriz(mat1);
    calcular_media(mat1);
    calcular_media_coluna(mat1);
    calcular_media_linha(mat1);
}
