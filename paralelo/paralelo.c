#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Función para inicializar la matriz con valores aleatorios
void init_matrix(int *matrix, int N)
{
    for (int i = 0; i < N * N; i++)
    {
        matrix[i] = rand() % 10;
    }
}

// Función para multiplicar matrices
void multiply_matrices(int *A, int *B, int *C, int N, int start_row, int end_row)
{
    for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < N; j++)
        {
            C[i * N + j] = 0;
            for (int k = 0; k < N; k++)
            {
                C[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int rank, size;
    int N;

    if (argc != 2)
    {
        if (rank == 0)
            printf("Uso: %s <tamaño de la matriz N>\n", argv[0]);
        return -1;
    }

    N = atoi(argv[1]);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *A = NULL;
    int *B = NULL;
    int *C = NULL;
    int *local_A = NULL;
    int *local_C = NULL;

    int rows_per_process = N / size;
    int extra_rows = N % size;
    int start_row, end_row;

    if (rank < extra_rows)
    {
        start_row = rank * (rows_per_process + 1);
        end_row = start_row + rows_per_process + 1;
    }
    else
    {
        start_row = rank * rows_per_process + extra_rows;
        end_row = start_row + rows_per_process;
    }

    local_A = (int *)malloc((end_row - start_row) * N * sizeof(int));
    local_C = (int *)malloc((end_row - start_row) * N * sizeof(int));

    if (rank == 0)
    {
        A = (int *)malloc(N * N * sizeof(int));
        B = (int *)malloc(N * N * sizeof(int));
        C = (int *)malloc(N * N * sizeof(int));
        srand(time(NULL));
        init_matrix(A, N);
        init_matrix(B, N);
    }

    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(A, send_counts, displs, MPI_INT, local_A, (end_row - start_row) * N, MPI_INT, 0, MPI_COMM_WORLD);

    multiply_matrices(local_A, B, local_C, N, 0, end_row - start_row);

    MPI_Gatherv(local_C, (end_row - start_row) * N, MPI_INT, C, recv_counts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        // Imprimir matrices (opcional)
        // print_matrix(A, N);
        // printf("\n");
        // print_matrix(B, N);
        // printf("\n");
        // print_matrix(C, N);

        free(A);
        free(B);
        free(C);
    }

    free(local_A);
    free(local_C);

    MPI_Finalize();

    return 0;
}