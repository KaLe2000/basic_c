#include <stdio.h>

int getValByIndex(int* array, int size, int index);
int getIndexByVal(int* array, int size, int value);
void insertElement(int **array, int *size, int index, int value);
int binarySearch(int *arr, int size, int target);

int testsPassed = 0;
int testsFailed = 0;

void testGetValByIndex() {
    printf("Тест: getValByIndex\n");
    int array[] = {10, 20, 30, 40, 50};
    int result = getValByIndex(array, 5, 2);

    if (result == 30) {
        printf("  [PASS] array[2] = 30\n");
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось 30\n");
        testsFailed++;
    }
}

void testGetValByIndexOutOfBounds() {
    printf("Тест: getValByIndex выход за границы\n");
    int array[] = {10, 20, 30};
    int result = getValByIndex(array, 3, 10);

    if (result == -1) {
        printf("  [PASS] возвращает -1\n");
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось -1\n");
        testsFailed++;
    }
}

void testGetIndexByVal() {
    printf("Тест: getIndexByVal\n");
    int array[] = {10, 20, 30, 40, 50};
    int result = getIndexByVal(array, 5, 30);

    if (result == 2) {
        printf("  [PASS] index = 2\n");
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось 2\n");
        testsFailed++;
    }
}

void testGetIndexByValNotFound() {
    printf("Тест: getIndexByVal не найден\n");
    int array[] = {10, 20, 30};
    int result = getIndexByVal(array, 3, 100);

    if (result == -1) {
        printf("  [PASS] возвращает -1\n");
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось -1\n");
        testsFailed++;
    }
}

void testBinarySearch() {
    printf("Тест: binarySearch\n");
    int arr[] = {1, 3, 5, 7, 9};
    int result = binarySearch(arr, 5, 7);

    if (result == 3) {
        printf("  [PASS] index = 3\n");
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось 3\n");
        testsFailed++;
    }
}

void testBinarySearchNotFound() {
    printf("Тест: binarySearch не найден\n");
    int arr[] = {1, 3, 5, 7, 9};
    int result = binarySearch(arr, 5, 4);

    if (result == -1) {
        printf("  [PASS] возвращает -1\n");
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось -1\n");
        testsFailed++;
    }
}

int main() {
    printf("=== Тесты массивов ===\n\n");

    testGetValByIndex();
    testGetValByIndexOutOfBounds();
    testGetIndexByVal();
    testGetIndexByValNotFound();
    testBinarySearch();
    testBinarySearchNotFound();

    printf("\n=== Результаты ===\n");
    printf("Пройдено: %d\n", testsPassed);
    printf("Провалено: %d\n", testsFailed);

    return testsFailed > 0 ? 1 : 0;
}