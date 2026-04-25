#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

int testsPassed = 0;
int testsFailed = 0;

void testBasicPointer() {
    printf("Тест: базовый указатель\n");
    int value = 42;
    int *ptr = &value;

    if (*ptr == 42) {
        printf("  [PASS] *ptr == 42\n");
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось 42\n");
        testsFailed++;
    }
}

void testModifyViaPointer() {
    printf("Тест: изменение через указатель\n");
    int value = 10;
    int *ptr = &value;

    *ptr = 20;
    if (value == 20) {
        printf("  [PASS] значение изменено через *ptr\n");
        testsPassed++;
    } else {
        printf("  [FAIL] значение не изменилось\n");
        testsFailed++;
    }
}

void testPointerArithmetic() {
    printf("Тест: арифметика указателей\n");
    int arr[] = {100, 200, 300};
    int *p = arr;

    if (*(p + 1) == 200 && *(p + 2) == 300) {
        printf("  [PASS] *(p + 1) = 200, *(p + 2) = 300\n");
        testsPassed++;
    } else {
        printf("  [FAIL] неверные значения\n");
        testsFailed++;
    }
}

void testPointerDifference() {
    printf("Тест: разность указателей\n");
    int arr[5] = {0};
    ptrdiff_t diff = &arr[4] - &arr[0];

    if (diff == 4) {
        printf("  [PASS] разность = 4\n");
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось 4\n");
        testsFailed++;
    }
}

void testPointerToPointer() {
    printf("Тест: указатель на указатель\n");
    int value = 10;
    int *ptr = &value;
    int **pptr = &ptr;

    **pptr = 20;
    if (value == 20) {
        printf("  [PASS] **pptr изменяет value\n");
        testsPassed++;
    } else {
        printf("  [FAIL] значение не изменилось\n");
        testsFailed++;
    }
}

void testNullPointer() {
    printf("Тест: NULL указатель\n");
    int *ptr = NULL;

    if (ptr == NULL) {
        printf("  [PASS] ptr == NULL\n");
        testsPassed++;
    } else {
        printf("  [FAIL] ptr не NULL\n");
        testsFailed++;
    }
}

void testVoidPointer() {
    printf("Тест: void* указатель\n");
    int intVal = 42;
    void *vptr = &intVal;

    if (*(int*)vptr == 42) {
        printf("  [PASS] void* приведён к int*\n");
        testsPassed++;
    } else {
        printf("  [FAIL] неверное значение\n");
        testsFailed++;
    }
}

void testSwapViaPointers() {
    printf("Тест: обмен через указатели\n");
    int a = 5, b = 10;

    int temp = a;
    a = b;
    b = temp;

    if (a == 10 && b == 5) {
        printf("  [PASS] swap: a=%d, b=%d\n", a, b);
        testsPassed++;
    } else {
        printf("  [FAIL] swap не удался\n");
        testsFailed++;
    }
}

void testArrayAsPointer() {
    printf("Тест: массив как указатель\n");
    int arr[] = {1, 2, 3, 4, 5};
    int *p = arr;

    if (p[2] == 3 && *(p + 4) == 5) {
        printf("  [PASS] p[2]=3, *(p+4)=5\n");
        testsPassed++;
    } else {
        printf("  [FAIL] неверные значения\n");
        testsFailed++;
    }
}

void testConstPointer() {
    printf("Тест: константный указатель\n");
    const int x = 100;
    const int *ptr = &x;

    if (*ptr == 100) {
        printf("  [PASS] *ptr доступен для чтения\n");
        testsPassed++;
    } else {
        printf("  [FAIL] неверное значение\n");
        testsFailed++;
    }
}

int main() {
    printf("=== Тесты указателей ===\n\n");

    testBasicPointer();
    testModifyViaPointer();
    testPointerArithmetic();
    testPointerDifference();
    testPointerToPointer();
    testNullPointer();
    testVoidPointer();
    testSwapViaPointers();
    testArrayAsPointer();
    testConstPointer();

    printf("\n=== Результаты ===\n");
    printf("Пройдено: %d\n", testsPassed);
    printf("Провалено: %d\n", testsFailed);

    return testsFailed > 0 ? 1 : 0;
}