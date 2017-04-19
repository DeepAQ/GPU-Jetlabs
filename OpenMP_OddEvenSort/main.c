#include <stdio.h>
#include <time.h>

void swap(int *a, int n, int m) {
    int tmp = a[n];
    a[n] = a[m];
    a[m] = tmp;
}

void sort_first(int *a, int n, int thread_count) {
    int i;
    for (int phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {
#pragma omp parallel for num_threads(thread_count) default(none) shared(a, n) private(i)
            for (i = 1; i < n; i += 2) {
                if (a[i-1] > a[i]) {
                    swap(a, i-1, i);
                }
            }
        } else {
#pragma omp parallel for num_threads(thread_count) default(none) shared(a, n) private(i)
            for (i = 1; i < n-1; i += 2) {
                if (a[i] > a[i+1]) {
                    swap(a, i, i+1);
                }
            }
        }
    }
}

void sort_second(int *a, int n, int thread_count) {
    int i, phase;
#pragma omp parallel num_threads(thread_count) default(none) shared(a, n) private(i, phase)
    for (phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {
#pragma omp for
            for (i = 1; i < n; i += 2) {
                if (a[i-1] > a[i]) {
                    swap(a, i-1, i);
                }
            }
        } else {
#pragma omp for
            for (i = 1; i < n-1; i += 2) {
                if (a[i] > a[i+1]) {
                    swap(a, i, i+1);
                }
            }
        }
    }
}

void sort_serial(int *a, int n) {
    for (int phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {
            for (int i = 1; i < n; i += 2) {
                if (a[i-1] > a[i]) {
                    swap(a, i-1, i);
                }
            }
        } else {
            for (int i = 1; i < n-1; i += 2) {
                if (a[i] > a[i+1]) {
                    swap(a, i, i+1);
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int n, threads;
    printf("Input count of numbers: ");
    scanf("%d", &n);
    printf("Input thread count: ");
    scanf("%d", &threads);
    int a[n];

    for (int i = 0; i < n; i++) {
        a[i] = n - i;
    }
    clock_t start_time = clock();
    sort_first(a, n, threads);
    printf("Two `parallel for` directives: %lfs\n", (clock() - start_time) * 1.0 / CLOCKS_PER_SEC);

    for (int i = 0; i < n; i++) {
        a[i] = n - i;
    }
    start_time = clock();
    sort_second(a, n, threads);
    printf("Two `for` directives: %lfs\n", (clock() - start_time) * 1.0 / CLOCKS_PER_SEC);

    for (int i = 0; i < n; i++) {
        a[i] = n - i;
    }
    start_time = clock();
    sort_serial(a, n);
    printf("Serial: %lfs\n", (clock() - start_time) * 1.0 / CLOCKS_PER_SEC);

    return 0;
}
