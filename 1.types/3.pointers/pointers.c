#include <stdio.h>
#include <stdlib.h>

void demonstrateBasics() {
    printf("=== Основы указателей ===\n\n");

    int value = 42;
    int *ptr = &value;

    printf("value = %d\n", value);
    printf("ptr (адрес) = %p\n", (void*)ptr);
    printf("*ptr (значение) = %d\n\n", *ptr);

    *ptr = 100;
    printf("После *ptr = 100:\n");
    printf("value = %d\n\n", value);
}

void demonstratePointerArithmetic() {
    printf("=== Арифметика указателей ===\n\n");

    int arr[] = {10, 20, 30, 40, 50};
    int *p = arr;

    printf("Массив: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    printf("p = arr (указывает на arr[0]): %d\n", *p);
    printf("p + 1 (указывает на arr[1]): %d\n", *(p + 1));
    printf("*(p + 2): %d\n", *(p + 2));

    p++;
    printf("\nПосле p++:\n");
    printf("*p = %d (теперь указывает на arr[1])\n\n", *p);

    printf("Индексация через указатель:\n");
    printf("p[0] = %d, p[1] = %d, p[2] = %d\n\n", p[0], p[1], p[2]);

    printf("Разность указателей:\n");
    int *p1 = &arr[0];
    int *p2 = &arr[4];
    printf("&arr[4] - &arr[0] = %td\n\n", p2 - p1);
}

void demonstrateVoidPointer() {
    printf("=== void* указатель ===\n\n");

    int intVal = 42;
    double doubleVal = 3.14;
    char charVal = 'A';

    void *vptr = &intVal;
    printf("void* на int: %d\n", *(int*)vptr);

    vptr = &doubleVal;
    printf("void* на double: %.2f\n", *(double*)vptr);

    vptr = &charVal;
    printf("void* на char: '%c'\n\n", *(char*)vptr);

    printf("malloc возвращает void*:\n");
    int *arr = (int*)malloc(5 * sizeof(int));
    for (int i = 0; i < 5; i++) {
        arr[i] = (i + 1) * 10;
        printf("arr[%d] = %d\n", i, arr[i]);
    }
    free(arr);
    printf("\n");
}

void demonstratePointerToPointer() {
    printf("=== Указатель на указатель ===\n\n");

    int value = 10;
    int *ptr = &value;
    int **pptr = &ptr;

    printf("value = %d\n", value);
    printf("*ptr = %d\n", *ptr);
    printf("**pptr = %d\n\n", **pptr);

    **pptr = 20;
    printf("После **pptr = 20:\n");
    printf("value = %d\n\n", value);

    printf("Модификация указателя через функцию:\n");
    int *dynamicArr = NULL;
    int **ppArr = &dynamicArr;

    *ppArr = (int*)malloc(3 * sizeof(int));
    (*ppArr)[0] = 100;
    (*ppArr)[1] = 200;
    (*ppArr)[2] = 300;

    printf("dynamicArr: %d, %d, %d\n", dynamicArr[0], dynamicArr[1], dynamicArr[2]);
    free(dynamicArr);
    printf("\n");
}

void allocateIntArray(int **ptr, size_t size) {
    *ptr = (int*)malloc(size * sizeof(int));
}

void demonstrateModifyPointerInFunction() {
    printf("=== Изменение указателя в функции ===\n\n");

    int *arr = NULL;
    allocateIntArray(&arr, 5);

    for (int i = 0; i < 5; i++) {
        arr[i] = i * 10;
    }

    printf("Выделенный массив: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    arr = NULL;
    printf("После free: arr = %p\n\n", (void*)arr);
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void demonstrateSwapViaPointers() {
    printf("=== Обмен через указатели ===\n\n");

    int x = 10, y = 20;
    printf("До swap: x = %d, y = %d\n", x, y);

    swap(&x, &y);
    printf("После swap: x = %d, y = %d\n\n", x, y);
}

void demonstrateConstPointers() {
    printf("=== Константные указатели ===\n\n");

    const int x = 10;
    const int *ptr1 = &x;
    printf("const int *ptr1 = &x: нельзя *ptr1 = 20\n");

    int arr[] = {1, 2, 3};
    int *const ptr2 = arr;
    printf("int *const ptr2 = arr: нельзя ptr2++\n");
    (*ptr2)++;  // но можно изменять данные
    printf("После (*ptr2)++: arr[0] = %d\n\n", arr[0]);

    const int *const ptr3 = &x;
    printf("const int *const ptr3: нельзя менять ничего\n");
}

void demonstrateFunctionPointers() {
    printf("=== Указатели на функции ===\n\n");

    int add(int a, int b) { return a + b; }
    int multiply(int a, int b) { return a * b; }

    int (*operations[2])(int, int) = {add, multiply};

    printf("operations[0](2, 3) = %d\n", operations[0](2, 3));
    printf("operations[1](2, 3) = %d\n", operations[1](2, 3));
}

int main() {
    demonstrateBasics();
    demonstratePointerArithmetic();
    demonstrateVoidPointer();
    demonstratePointerToPointer();
    demonstrateModifyPointerInFunction();
    demonstrateSwapViaPointers();
    demonstrateConstPointers();
    demonstrateFunctionPointers();

    return 0;
}