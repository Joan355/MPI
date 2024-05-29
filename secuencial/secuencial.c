
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Función para inicializar la matriz con valores aleatorios
void init_matrix(int **matrix, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }
}

// Función para multiplicar matrices
void multiply_matrices(int **A, int **B, int **C, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Función para imprimir la matriz
void print_matrix(int **matrix, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s <tamaño de la matriz N>\n", argv[0]);
        return -1;
    }

    int N = atoi(argv[1]);

    // Inicializar matrices
    int **A = (int **)malloc(N * sizeof(int *));
    int **B = (int **)malloc(N * sizeof(int *));
    int **C = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
    {
        A[i] = (int *)malloc(N * sizeof(int));
        B[i] = (int *)malloc(N * sizeof(int));
        C[i] = (int *)malloc(N * sizeof(int));
    }

    srand(time(NULL));
    init_matrix(A, N);
    init_matrix(B, N);

    // Multiplicar matrices
    multiply_matrices(A, B, C, N);

    // Imprimir matrices (opcional)
    // print_matrix(A, N);
    // printf("\n");
    // print_matrix(B, N);
    // printf("\n");
    // print_matrix(C, N);

    // Liberar memoria
    for (int i = 0; i < N; i++)
    {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
