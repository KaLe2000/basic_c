#include <stdio.h>
#include <stdlib.h>

int testsPassed = 0;
int testsFailed = 0;

void testMalloc() {
    printf("Тест: malloc\n");
    int *ptr = (int*)malloc(sizeof(int));
    if (ptr != NULL) {
        *ptr = 42;
        if (*ptr == 42) {
            printf("  [PASS] malloc выделил память\n");
            testsPassed++;
        } else {
            printf("  [FAIL] запись не удалась\n");
            testsFailed++;
        }
        free(ptr);
    } else {
        printf("  [FAIL] malloc вернул NULL\n");
        testsFailed++;
    }
}

void testCallocZeros() {
    printf("Тест: calloc обнуляет\n");
    int *arr = (int*)calloc(5, sizeof(int));
    if (arr != NULL) {
        int allZeros = 1;
        for (int i = 0; i < 5; i++) {
            if (arr[i] != 0) {
                allZeros = 0;
                break;
            }
        }
        if (allZeros) {
            printf("  [PASS] все элементы = 0\n");
            testsPassed++;
        } else {
            printf("  [FAIL] элементы не обнулены\n");
            testsFailed++;
        }
        free(arr);
    } else {
        printf("  [FAIL] calloc вернул NULL\n");
        testsFailed++;
    }
}

void testRealloc() {
    printf("Тест: realloc\n");
    int *arr = (int*)malloc(3 * sizeof(int));
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;

    int *newArr = (int*)realloc(arr, 5 * sizeof(int));
    if (newArr != NULL) {
        if (newArr[0] == 10 && newArr[1] == 20 && newArr[2] == 30) {
            newArr[3] = 40;
            newArr[4] = 50;
            if (newArr[3] == 40 && newArr[4] == 50) {
                printf("  [PASS] расширение работает\n");
                testsPassed++;
            } else {
                printf("  [FAIL] новые данные не записались\n");
                testsFailed++;
            }
            free(newArr);
        } else {
            printf("  [FAIL] данные не сохранены\n");
            testsFailed++;
            free(arr);
        }
    } else {
        printf("  [FAIL] realloc вернул NULL\n");
        testsFailed++;
        free(arr);
    }
}

void testNullCheck() {
    printf("Тест: проверка на NULL\n");
    size_t huge = (size_t)-1;
    void *ptr = malloc(huge);

    if (ptr == NULL) {
        printf("  [PASS] большой malloc вернул NULL\n");
        testsPassed++;
    } else {
        free(ptr);
        printf("  [FAIL] должен вернуть NULL\n");
        testsFailed++;
    }
}

void testFreeDoesNotNullify() {
    printf("Тест: free НЕ обнуляет указатель\n");
    int *ptr = (int*)malloc(sizeof(int));
    free(ptr);

    printf("  [INFO] free() сам НЕ обнуляет ptr\n");
    printf("  [INFO] ptr все ещё = %p после free\n", (void*)ptr);
    printf("  [INFO] Программист должен: ptr = NULL;\n");
    ptr = NULL;
    if (ptr == NULL) {
        printf("  [PASS] после ptr = NULL\n");
        testsPassed++;
    } else {
        printf("  [FAIL] ptr не NULL\n");
        testsFailed++;
    }
}

void testReallocShrink() {
    printf("Тест: realloc shrink\n");
    int *arr = (int*)malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++) {
        arr[i] = i + 1;
    }

    int *small = (int*)realloc(arr, 3 * sizeof(int));
    if (small != NULL) {
        if (small[0] == 1 && small[2] == 3) {
            printf("  [PASS] shrink сохраняет данные\n");
            testsPassed++;
        } else {
            printf("  [FAIL] данные потеряны\n");
            testsFailed++;
        }
        free(small);
    } else {
        printf("  [FAIL] realloc вернул NULL\n");
        testsFailed++;
        free(arr);
    }
}

void testMemorySize() {
    printf("Тест: размер памяти\n");
    size_t count = 100;
    int *arr = (int*)malloc(count * sizeof(int));

    if (arr != NULL) {
        arr[99] = 42;
        if (arr[99] == 42) {
            printf("  [PASS] sizeof(int) корректен\n");
            testsPassed++;
        } else {
            printf("  [FAIL] запись за границей\n");
            testsFailed++;
        }
        free(arr);
    } else {
        printf("  [FAIL] malloc вернул NULL\n");
        testsFailed++;
    }
}

int main() {
    printf("=== Тесты управления памятью ===\n\n");

    testMalloc();
    testCallocZeros();
    testRealloc();
    testNullCheck();
    testFreeDoesNotNullify();
    testReallocShrink();
    testMemorySize();

    printf("\n=== Результаты ===\n");
    printf("Пройдено: %d\n", testsPassed);
    printf("Провалено: %d\n", testsFailed);

    return testsFailed > 0 ? 1 : 0;
}