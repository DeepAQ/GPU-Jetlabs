#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int my_rank, comm_sz;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
//    if (my_rank > 0) {
//        int a = my_rank;
//        MPI_Send(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
//    } else {
//        int total = 0, a = 0;
//        for (int i = 1; i < comm_sz; i++) {
//            printf("Receiving from process %d\n", i);
//            MPI_Recv(&a, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//            printf("Received a=%d from process %d\n", a, i);
//            total += a;
//            printf("Total = %d\n", total);
//        }
//    }
    int total;
    MPI_Reduce(&my_rank, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    printf("Process %d, Total = %d\n", my_rank, total);
    MPI_Finalize();
    printf("End\n");
    return 0;
}
