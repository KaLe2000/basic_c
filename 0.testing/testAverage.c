#include <stdio.h>

int averageSum(int* nums, int size);
int sumNums(int* nums, int size);

int testsPassed = 0;
int testsFailed = 0;

void testAverage() {
    printf("Тест: averageSum\n");
    int testArr[] = {1, 1, 1, 2, 2, 5};
    int result = averageSum(testArr, 6);

    if (result == 2) {
        printf("  [PASS] averageSum = 2\n");
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось 2, получили %d\n", result);
        testsFailed++;
    }
}

void testSum() {
    printf("Тест: sumNums\n");
    int testArr[] = {1, 1, 1, 1, 1, 5};
    int result = sumNums(testArr, 6);

    if (result == 10) {
        printf("  [PASS] sumNums = 10\n");
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось 10, получили %d\n", result);
        testsFailed++;
    }
}

int main() {
    printf("=== Тесты average ===\n\n");
    testAverage();
    testSum();

    printf("\n=== Результаты ===\n");
    printf("Пройдено: %d\n", testsPassed);
    printf("Провалено: %d\n", testsFailed);

    return testsFailed > 0 ? 1 : 0;
}