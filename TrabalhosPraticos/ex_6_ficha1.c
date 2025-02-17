#include <stdio.h>
#define linhas 5
#define colunas 3
void ler_matriz(int v[linhas][colunas])
{
    for (int i = 0; i < linhas; i++)
            for (int j = 0; j < colunas; j++)
        {
            scanf("%d", &v[linhas][colunas]);
        }
}

int main()
{
    int mat1[linhas][colunas];
    ler_matriz(mat1);
    
}
