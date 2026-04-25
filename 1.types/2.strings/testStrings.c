#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int testsPassed = 0;
int testsFailed = 0;

void testStrlen() {
    printf("Тест: strlen\n");
    if (strlen("Hello") == 5) {
        printf("  [PASS] strlen(\"Hello\") = 5\n");
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось 5\n");
        testsFailed++;
    }
}

void testStrcpy() {
    printf("Тест: strcpy\n");
    char buf[20];
    strcpy(buf, "Test");
    if (strcmp(buf, "Test") == 0) {
        printf("  [PASS] strcpy работает корректно\n");
        testsPassed++;
    } else {
        printf("  [FAIL] копирование не удалось\n");
        testsFailed++;
    }
}

void testStrcat() {
    printf("Тест: strcat\n");
    char buf[20] = "Hello";
    strcat(buf, " World");
    if (strcmp(buf, "Hello World") == 0) {
        printf("  [PASS] strcat: \"%s\"\n", buf);
        testsPassed++;
    } else {
        printf("  [FAIL] конкатенация не удалась\n");
        testsFailed++;
    }
}

void testStrcmp() {
    printf("Тест: strcmp\n");
    if (strcmp("abc", "abc") == 0 &&
        strcmp("abc", "abd") < 0 &&
        strcmp("abd", "abc") > 0) {
        printf("  [PASS] strcmp возвращает правильные значения\n");
        testsPassed++;
    } else {
        printf("  [FAIL] неверные результаты сравнения\n");
        testsFailed++;
    }
}

void testStrncmp() {
    printf("Тест: strncmp\n");
    if (strncmp("Hello", "Helicopter", 3) == 0) {
        printf("  [PASS] strncmp(\"Hello\", \"Helicopter\", 3) = 0\n");
        testsPassed++;
    } else {
        printf("  [FAIL] сравнение первых 3 символов\n");
        testsFailed++;
    }
}

void testStrchr() {
    printf("Тест: strchr\n");
    char *found = strchr("Hello", 'l');
    if (found != NULL && *found == 'l') {
        printf("  [PASS] strchr нашел 'l'\n");
        testsPassed++;
    } else {
        printf("  [FAIL] символ не найден\n");
        testsFailed++;
    }

    found = strchr("Hello", 'z');
    if (found == NULL) {
        printf("  [PASS] strchr для отсутствующего символа = NULL\n");
        testsPassed++;
    } else {
        printf("  [FAIL] должен быть NULL\n");
        testsFailed++;
    }
}

void testStrstr() {
    printf("Тест: strstr\n");
    char *found = strstr("Hello World", "World");
    if (found != NULL && strcmp(found, "World") == 0) {
        printf("  [PASS] strstr нашел подстроку\n");
        testsPassed++;
    } else {
        printf("  [FAIL] подстрока не найдена\n");
        testsFailed++;
    }
}

void testStrtol() {
    printf("Тест: strtol\n");
    char *end;
    long value = strtol("  123abc", &end, 10);
    if (value == 123 && strcmp(end, "abc") == 0) {
        printf("  [PASS] strtol: %ld, остаток: \"%s\"\n", value, end);
        testsPassed++;
    } else {
        printf("  [FAIL] преобразование или остаток\n");
        testsFailed++;
    }
}

void testAtoi() {
    printf("Тест: atoi\n");
    if (atoi("42") == 42 && atoi("-10") == -10) {
        printf("  [PASS] atoi положительных и отрицательных\n");
        testsPassed++;
    } else {
        printf("  [FAIL] неверное преобразование\n");
        testsFailed++;
    }
}

void testAtof() {
    printf("Тест: atof\n");
    double value = atof("3.14");
    if (value > 3.13 && value < 3.15) {
        printf("  [PASS] atof(\"3.14\") = %f\n", value);
        testsPassed++;
    } else {
        printf("  [FAIL] неверное преобразование\n");
        testsFailed++;
    }
}

void testStringTerminator() {
    printf("Тест: нуль-символ '\\0'\n");
    char s[] = "Hi";
    if (s[2] == '\0' && strlen(s) == 2) {
        printf("  [PASS] строка корректно завершается '\\0'\n");
        testsPassed++;
    } else {
        printf("  [FAIL] отсутствует '\\0'\n");
        testsFailed++;
    }
}

int main() {
    printf("=== Тесты строк ===\n\n");

    testStrlen();
    testStrcpy();
    testStrcat();
    testStrcmp();
    testStrncmp();
    testStrchr();
    testStrstr();
    testStrtol();
    testAtoi();
    testAtof();
    testStringTerminator();

    printf("\n=== Результаты ===\n");
    printf("Пройдено: %d\n", testsPassed);
    printf("Провалено: %d\n", testsFailed);

    return testsFailed > 0 ? 1 : 0;
}