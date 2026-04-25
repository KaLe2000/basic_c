#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

int testsPassed = 0;
int testsFailed = 0;

void testBasicThread() {
    printf("Тест: создание потока\n");

    pthread_t thread;
    int result = 0;

    void* worker(void* arg) {
        int* val = (int*)arg;
        *val = 42;
        return NULL;
    }

    pthread_create(&thread, NULL, worker, &result);
    pthread_join(thread, NULL);

    if (result == 42) {
        printf("  [PASS] поток выполнился\n");
        testsPassed++;
    } else {
        printf("  [FAIL] результат = %d\n", result);
        testsFailed++;
    }
}

void testMutex() {
    printf("Тест: mutex\n");

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int counter = 0;

    void* worker(void* arg) {
        for (int i = 0; i < 1000; i++) {
            pthread_mutex_lock(&mutex);
            counter++;
            pthread_mutex_unlock(&mutex);
        }
        return NULL;
    }

    pthread_t t1, t2;
    pthread_create(&t1, NULL, worker, NULL);
    pthread_create(&t2, NULL, worker, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    if (counter == 2000) {
        printf("  [PASS] counter = %d\n", counter);
        testsPassed++;
    } else {
        printf("  [FAIL] counter = %d (expected 2000)\n", counter);
        testsFailed++;
    }
}

void testConditionVariable() {
    printf("Тест: condition variable\n");

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    int ready = 0;
    int received = 0;

    void* producer(void* arg) {
        pthread_mutex_lock(&mutex);
        ready = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    void* consumer(void* arg) {
        pthread_mutex_lock(&mutex);
        while (!ready) {
            pthread_cond_wait(&cond, &mutex);
        }
        received = 42;
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    pthread_t prod, cons;
    pthread_create(&cons, NULL, consumer, NULL);
    pthread_create(&prod, NULL, producer, NULL);
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    if (received == 42) {
        printf("  [PASS] signal получен\n");
        testsPassed++;
    } else {
        printf("  [FAIL] received = %d\n", received);
        testsFailed++;
    }
}

int main() {
    printf("=== Тесты потоков ===\n\n");

    testBasicThread();
    testMutex();
    testConditionVariable();

    printf("\n=== Результаты ===\n");
    printf("Пройдено: %d\n", testsPassed);
    printf("Провалено: %d\n", testsFailed);

    return testsFailed > 0 ? 1 : 0;
}