#include <stdio.h>
#include <omp.h>

void hello() {
    int my_rank = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    printf("Hello from thread %d of %d\n", my_rank, thread_count);
}

int main(int argc, char *argv[])
{
    int thread_num;
    printf("Input thread counts: ");
    scanf("%d", &thread_num);

#pragma omp parallel num_threads(thread_num)
    hello();

    return 0;
}
