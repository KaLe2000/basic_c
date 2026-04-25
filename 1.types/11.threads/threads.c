#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 1000

typedef struct {
    int* data;
    size_t start;
    size_t end;
    long long sum;
} Task;

void* sumRange(void* arg) {
    Task* task = (Task*)arg;
    long long sum = 0;

    for (size_t i = task->start; i < task->end; i++) {
        sum += task->data[i];
    }

    task->sum = sum;
    printf("Thread: sum[%zu-%zu] = %lld\n", task->start, task->end, sum);

    return (void*)sum;
}

int main() {
    int data[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) {
        data[i] = i + 1;
    }

    pthread_t threads[NUM_THREADS];
    Task tasks[NUM_THREADS];
    size_t chunk = ARRAY_SIZE / NUM_THREADS;

    printf("=== Параллельное суммирование ===\n");
    printf("Массив: %d элементов, %d потоков\n\n", ARRAY_SIZE, NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        tasks[i].data = data;
        tasks[i].start = i * chunk;
        tasks[i].end = (i == NUM_THREADS - 1) ? ARRAY_SIZE : (i + 1) * chunk;
        tasks[i].sum = 0;

        pthread_create(&threads[i], NULL, sumRange, &tasks[i]);
    }

    long long total = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        void* result;
        pthread_join(threads[i], &result);
        total += (long long)result;
    }

    printf("\nTotal sum: %lld\n", total);
    printf("Expected: %lld\n", (long long)ARRAY_SIZE * (ARRAY_SIZE + 1) / 2);

    return 0;
}