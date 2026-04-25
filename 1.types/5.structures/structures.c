#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    char name[50];
    int age;
    double salary;
} Employee;

typedef struct {
    int active : 1;
    int ready : 1;
    int error : 4;
} Flags;

typedef union {
    int i;
    float f;
    char str[4];
} Data;

typedef enum {
    MONDAY = 1,
    TUESDAY = 2,
    WEDNESDAY = 3,
    THURSDAY = 4,
    FRIDAY = 5,
    SATURDAY = 6,
    SUNDAY = 7
} Day;

void demonstrateStructBasics() {
    printf("=== Структуры: основы ===\n\n");

    Point p1 = {10, 20};
    Point p2 = {.x = 30, .y = 40};

    printf("p1 = (%d, %d)\n", p1.x, p1.y);
    printf("p2 = (%d, %d)\n\n", p2.x, p2.y);

    p1.x = 15;
    p1.y = 25;
    printf("После изменения: p1 = (%d, %d)\n\n", p1.x, p1.y);
}

void demonstrateStructPointer() {
    printf("=== Доступ через указатель ===\n\n");

    Point p = {100, 200};
    Point *ptr = &p;

    printf("Через точку: (%d, %d)\n", p.x, p.y);
    printf("Через стрелку: (%d, %d)\n", ptr->x, ptr->y);
    printf("Через (*ptr): (%d, %d)\n\n", (*ptr).x, (*ptr).y);

    ptr->x = 150;
    printf("После ptr->x = 150: (%d, %d)\n\n", p.x, p.y);
}

void demonstrateStructCopy() {
    printf("=== Копирование структур ===\n\n");

    Point p1 = {10, 20};
    Point p2;

    p2 = p1;  // копирует все поля
    printf("p1 = (%d, %d)\n", p1.x, p1.y);
    printf("p2 = p1 → (%d, %d)\n\n", p2.x, p2.y);

    p2.x = 99;
    printf("p2.x = 99:\n");
    printf("p1 = (%d, %d) — не изменилась\n", p1.x, p1.y);
    printf("p2 = (%d, %d)\n\n", p2.x, p2.y);
}

void demonstrateBitFields() {
    printf("=== Битовые поля ===\n\n");

    Flags f;
    f.active = 1;
    f.ready = 0;
    f.error = 3;

    printf("Flags: active=%d, ready=%d, error=%d\n", f.active, f.ready, f.error);
    printf("sizeof(Flags) = %zu байт\n\n", sizeof(Flags));
}

void demonstrateUnion() {
    printf("=== Union — объединение ===\n\n");

    Data d;
    printf("sizeof(Data) = %zu (максимум: %zu)\n\n", sizeof(Data), sizeof(int));

    d.i = 42;
    printf("d.i = %d → d.str = ", d.i);
    for (int i = 0; i < 4; i++) {
        printf("%02X ", (unsigned char)d.str[i]);
    }
    printf("\n");

    d.f = 3.14;
    printf("d.f = %.2f\n", d.f);
    printf("Как int: %d\n\n", d.i);
}

void demonstrateUnionIP() {
    printf("=== Union: IP-адрес ===\n\n");

    typedef union {
        unsigned int addr;
        unsigned char bytes[4];
    } IP;

    IP ip = {.addr = 0xC0A8010A};  // 192.168.1.10
    printf("0x%08X → %d.%d.%d.%d\n",
           ip.addr, ip.bytes[0], ip.bytes[1], ip.bytes[2], ip.bytes[3]);
    printf("\n");
}

void demonstrateEnum() {
    printf("=== Enum — перечисления ===\n\n");

    Day today = MONDAY;
    Day tomorrow = TUESDAY;

    printf("today = %d (%s)\n", today, "MONDAY");
    printf("tomorrow = %d (%s)\n", tomorrow, "TUESDAY");

    if (today == MONDAY) {
        printf("Сегодня понедельник!\n");
    }

    typedef enum {FALSE, TRUE} Bool;
    Bool flag = TRUE;
    printf("Bool flag = %d\n\n", flag);
}

void demonstrateStructInFunction() {
    printf("=== Структуры в функциях ===\n\n");

    void printEmployee(Employee *e) {
        printf("  %s, возраст %d, зарплата %.2f\n",
               e->name, e->age, e->salary);
    }

    Employee e1 = {"Ivan", 30, 50000.0};
    Employee e2 = {"Anna", 25, 45000.0};

    printEmployee(&e1);
    printEmployee(&e2);
    printf("\n");
}

void demonstrateSizeofStruct() {
    printf("=== Размер структуры ===\n\n");

    typedef struct {
        char a;
        int b;
    } Unpacked;

    typedef struct __attribute__((packed)) {
        char a;
        int b;
    } Packed;

    printf("Unpacked: %zu байт (с выравниванием)\n", sizeof(Unpacked));
    printf("Packed: %zu байт (без выравнивания)\n\n", sizeof(Packed));
}

int main() {
    demonstrateStructBasics();
    demonstrateStructPointer();
    demonstrateStructCopy();
    demonstrateBitFields();
    demonstrateUnion();
    demonstrateUnionIP();
    demonstrateEnum();
    demonstrateStructInFunction();
    demonstrateSizeofStruct();

    return 0;
}