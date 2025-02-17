#include <stdio.h>
#define linhas 2
#define colunas 2
void ler_matriz(int v[linhas][colunas])
{
    for (int i = 0; i < linhas; i++)
            for (int j = 0; j < colunas; j++)
        {   
            printf("Introduza o valor da linha %d e da coluna %d: \n",i,j);
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
void calcular_media(int m3[linhas][colunas]){
    int soma = 0;
    float media;
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j)
        {
            soma += m3[i][j];
        }

        media = soma / (linhas * colunas);
        printf("%d",soma);
        printf("%f",media);
    }

    
}
int main()
{
    int mat1[linhas][colunas];
    ler_matriz(mat1);
    print_matriz(mat1);
    calcular_media(mat1);
    printf("teste");
}
