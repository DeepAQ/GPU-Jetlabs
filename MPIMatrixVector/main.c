#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int my_rank, comm_sz;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    int m, n;
    if (my_rank == 0) {
        printf("Input matrix size m, n: ");
        fflush(stdout);
        scanf("%d%d", &m, &n);
    }
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int matrix[m][n];
    int vector[n];
    if (my_rank == 0) {
        printf("Input a %d x %d matrix:\n", m, n);
        fflush(stdout);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                scanf("%d", &matrix[i][j]);
            }
        }
        printf("Input a %d x 1 vector:\n", n);
        fflush(stdout);
        for (int i = 0; i < n; i++) {
            scanf("%d", &vector[i]);
        }
    }
    MPI_Bcast(matrix, m * n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector, n, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Process %d begin calculating\n", my_rank);
    fflush(stdout);
    int group_size = m / comm_sz;
    if (group_size == 0) {
        group_size = 1;
    }
    int local_begin = my_rank * group_size;
    int local_end = (my_rank + 1) * group_size;
    if (my_rank == comm_sz - 1) {
        local_end = n;
    }
    int local_result[m];
    for (int i = 0; i < m; i++) {
        local_result[i] = 0;
        if (my_rank + 1 <= m && i >= local_begin && i < local_end) {
            for (int j = 0; j < n; j++) {
                local_result[i] += matrix[i][j] * vector[j];
            }
        }
    }
    printf("Process %d finished calculating\n", my_rank);
    fflush(stdout);
    int total_result[m];
    MPI_Reduce(local_result, total_result, m, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (my_rank == 0) {
        printf("Result:\n");
        for (int i = 0; i < m; i++) {
            printf("%d ", total_result[i]);
        }
    }
    MPI_Finalize();
    return 0;
}
