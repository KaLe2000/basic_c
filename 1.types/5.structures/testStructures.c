#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int testsPassed = 0;
int testsFailed = 0;

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int id;
    char name[20];
} User;

void testStructInit() {
    printf("Тест: инициализация структуры\n");
    Point p = {10, 20};

    if (p.x == 10 && p.y == 20) {
        printf("  [PASS] Point{%d, %d}\n", p.x, p.y);
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось {10, 20}\n");
        testsFailed++;
    }
}

void testStructCopy() {
    printf("Тест: копирование структуры\n");
    Point p1 = {10, 20};
    Point p2 = p1;

    p2.x = 99;
    if (p1.x == 10 && p2.x == 99) {
        printf("  [PASS] независимое копирование\n");
        testsPassed++;
    } else {
        printf("  [FAIL] структуры связаны\n");
        testsFailed++;
    }
}

void testStructPointerAccess() {
    printf("Тест: доступ через указатель\n");
    Point p = {10, 20};
    Point *ptr = &p;

    if (ptr->x == 10 && ptr->y == 20) {
        printf("  [PASS] ptr->x = %d, ptr->y = %d\n", ptr->x, ptr->y);
        testsPassed++;
    } else {
        printf("  [FAIL] неверные значения\n");
        testsFailed++;
    }
}

void testNamedInit() {
    printf("Тест: именованная инициализация\n");
    Point p = {.y = 20, .x = 10};

    if (p.x == 10 && p.y == 20) {
        printf("  [PASS] {.x = 10, .y = 20}\n");
        testsPassed++;
    } else {
        printf("  [FAIL] порядок нарушен\n");
        testsFailed++;
    }
}

void testStructArray() {
    printf("Тест: массив структур\n");
    Point arr[3] = {{0, 0}, {1, 1}, {2, 2}};

    if (arr[1].x == 1 && arr[2].y == 2) {
        printf("  [PASS] arr[1]{%d,%d}, arr[2]{%d,%d}\n",
               arr[1].x, arr[1].y, arr[2].x, arr[2].y);
        testsPassed++;
    } else {
        printf("  [FAIL] массив структур\n");
        testsFailed++;
    }
}

void testEnumValues() {
    printf("Тест: enum значения\n");
    typedef enum {A = 1, B, C, D = 10, E, F} Letters;

    if (A == 1 && B == 2 && C == 3 && E == 11) {
        printf("  [PASS] A=%d, B=%d, C=%d, E=%d\n", A, B, C, E);
        testsPassed++;
    } else {
        printf("  [FAIL] неверные значения\n");
        testsFailed++;
    }
}

void testUnionOverwrite() {
    printf("Тест: union перезаписывает\n");
    typedef union {
        int i;
        float f;
    } Data;

    Data d;
    d.i = 42;
    d.f = 3.14f;

    if (d.f > 3.1f && d.f < 3.2f) {
        printf("  [PASS] d.f = %.2f (перезаписало int)\n", d.f);
        testsPassed++;
    } else {
        printf("  [FAIL] union не работает\n");
        testsFailed++;
    }
}

void testStructSize() {
    printf("Тест: sizeof структуры\n");
    typedef struct {
        char a;
        int b;
    } S;

    size_t size = sizeof(S);
    if (size >= 4) {
        printf("  [PASS] sizeof(S) = %zu\n", size);
        testsPassed++;
    } else {
        printf("  [FAIL] размер слишком мал\n");
        testsFailed++;
    }
}

void testBitField() {
    printf("Тест: битовые поля\n");
    typedef struct {
        unsigned int flag : 1;
        unsigned int mode : 3;
    } Flags;

    Flags f;
    f.flag = 1;
    f.mode = 5;

    if (f.flag == 1 && f.mode == 5) {
        printf("  [PASS] flag=%d, mode=%d\n", f.flag, f.mode);
        testsPassed++;
    } else {
        printf("  [FAIL] битовые поля\n");
        testsFailed++;
    }
}

int main() {
    printf("=== Тесты структур ===\n\n");

    testStructInit();
    testStructCopy();
    testStructPointerAccess();
    testNamedInit();
    testStructArray();
    testEnumValues();
    testUnionOverwrite();
    testStructSize();
    testBitField();

    printf("\n=== Результаты ===\n");
    printf("Пройдено: %d\n", testsPassed);
    printf("Провалено: %d\n", testsFailed);

    return testsFailed > 0 ? 1 : 0;
}