#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <math.h>

int testsPassed = 0;
int testsFailed = 0;

void testCharRange() {
    printf("Тест: диапазон char\n");
    if (CHAR_MIN == -128 && CHAR_MAX == 127) {
        printf("  [PASS] signed char: %d ... %d\n", CHAR_MIN, CHAR_MAX);
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось -128 ... 127\n");
        testsFailed++;
    }
}

void testCharAsNumber() {
    printf("Тест: char как число\n");
    char ch = 'A';
    if (ch == 65) {
        printf("  [PASS] 'A' = %d\n", ch);
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось 65\n");
        testsFailed++;
    }
}

void testCharDigitConversion() {
    printf("Тест: преобразовани�� символа цифры в число\n");
    char digit = '7';
    int value = digit - '0';
    if (value == 7) {
        printf("  [PASS] '%c' - '0' = %d\n", digit, value);
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось 7\n");
        testsFailed++;
    }
}

void testIntRange() {
    printf("Тест: диапазон int\n");
    if (INT_MIN == -2147483648 && INT_MAX == 2147483647) {
        printf("  [PASS] int: %d ... %d\n", INT_MIN, INT_MAX);
        testsPassed++;
    } else {
        printf("  [FAIL] неверный диапазон int\n");
        testsFailed++;
    }
}

void testUnsignedIntRange() {
    printf("Тест: диапазон unsigned int\n");
    if (UINT_MAX == 4294967295) {
        printf("  [PASS] unsigned int: 0 ... %u\n", UINT_MAX);
        testsPassed++;
    } else {
        printf("  [FAIL] неверный диапазон unsigned int\n");
        testsFailed++;
    }
}

void testFloatPrecision() {
    printf("Тест: точность float\n");
    float f1 = 1.0f;
    float f2 = 0.1f;
    float sum = f1 + f2;

    if (fabs(sum - 1.1f) < 0.0001f) {
        printf("  [PASS] 1.0f + 0.1f ≈ 1.1f\n");
        testsPassed++;
    } else {
        printf("  [FAIL] точность float нарушена\n");
        testsFailed++;
    }
}

void testDoublePrecision() {
    printf("Тест: точность double\n");
    double d1 = 1.0;
    double d2 = 0.1;
    double sum = d1 + d2;

    if (fabs(sum - 1.1) < 0.0000000001) {
        printf("  [PASS] 1.0 + 0.1 ≈ 1.1 (высокая точность)\n");
        testsPassed++;
    } else {
        printf("  [FAIL] точность double нарушена\n");
        testsFailed++;
    }
}

void testIntegerDivision() {
    printf("Тест: целочисленное деление\n");
    int result = 7 / 3;
    if (result == 2) {
        printf("  [PASS] 7 / 3 = %d (отбрасывание)\n", result);
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось 2\n");
        testsFailed++;
    }
}

void testFloatDivision() {
    printf("Тест: вещественное деление\n");
    int a = 7, b = 3;
    double result = (double)a / b;
    if (fabs(result - 2.333333) < 0.000001) {
        printf("  [PASS] (double)7 / 3 = %.6f\n", result);
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось ~2.333333\n");
        testsFailed++;
    }
}

void testTypeCasting() {
    printf("Тест: явное преобразование типов\n");
    double pi = 3.14159;
    int truncated = (int)pi;
    if (truncated == 3) {
        printf("  [PASS] (int)3.14159 = %d\n", truncated);
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось 3\n");
        testsFailed++;
    }
}

void testLongLongRange() {
    printf("Тест: диапазон long long\n");
    long long max = LLONG_MAX;
    long long min = LLONG_MIN;
    if (max > 0 && min < 0) {
        printf("  [PASS] long long поддерживает большие значения\n");
        testsPassed++;
    } else {
        printf("  [FAIL] неверный диапазон long long\n");
        testsFailed++;
    }
}

int main() {
    printf("=== Тесты примитивных типов ===\n\n");

    testCharRange();
    testCharAsNumber();
    testCharDigitConversion();
    testIntRange();
    testUnsignedIntRange();
    testFloatPrecision();
    testDoublePrecision();
    testIntegerDivision();
    testFloatDivision();
    testTypeCasting();
    testLongLongRange();

    printf("\n=== Результаты ===\n");
    printf("Пройдено: %d\n", testsPassed);
    printf("Провалено: %d\n", testsFailed);

    return testsFailed > 0 ? 1 : 0;
}