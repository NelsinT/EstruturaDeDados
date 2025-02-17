#include <stdio.h>

#define linhas 5
#define colunas 5

void matriz_1(int m1[linhas][colunas])
{
    int i, j;
    for (i = 0; i < linhas; i++)
        for (j = 0; j < colunas; j++)
            m1[i][j] = j;
}
void matriz_2(int m2[][colunas])
{
    int i, j;
    for (i = 0; i < linhas; i++)
        for (j = 0; j < colunas; j++)
            m2[i][j] = i;
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

// subprograma que escreve uma matriz
int main()
{
    int mat1[linhas][colunas], mat2[linhas][colunas];
    matriz_1(mat1);
    print_matriz(mat1);
    matriz_2(mat2);
    print_matriz(mat2);

    return 0;
}