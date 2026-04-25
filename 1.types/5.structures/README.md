# Структуры

Структура — это пользовательский тип данных, объединяющий поля разных типов.

## struct — базовый синтаксис

```c
struct Point {
    int x;
    int y;
};

struct Point p1 = {10, 20};        // инициализация
struct Point p2 = {.x = 10, .y = 20};  // именованная инициализация
```

## Доступ к полям

```c
struct Point p = {10, 20};

p.x = 15;           // через точку (значение)
struct Point *ptr = &p;
ptr->x = 15;        // через стрелку (указатель)
(*ptr).x = 15;      // эквивалентно
```

## typedef — псевдоним типа

```c
typedef struct Point {
    int x;
    int y;
} Point;

Point p = {10, 20};  // без "struct"
```

## Вложенные структуры

```c
struct Rectangle {
    struct Point topLeft;
    struct Point bottomRight;
};

struct Rectangle r = {{0, 0}, {100, 50}};
```

## Указатели в структурах

```c
typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node n1 = {10, NULL};
Node n2 = {20, NULL};
n1.next = &n2;
```

## Размер структуры

```c
struct Example {
    char a;     // 1 байт
    int b;      // выравнивание → фактически 4 байта
};
// sizeof(struct Example) = 8, не 5
```

### packed attribute (GNU C)
```c
struct __attribute__((packed)) Small {
    char a;
    int b;
};
// sizeof = 5
```

## Структуры и функции

### Передача по значению
```c
void printPoint(Point p) {
    printf("(%d, %d)\n", p.x, p.y);
}
```

### Передача по указателю
```c
void movePoint(Point *p, int dx, int dy) {
    p->x += dx;
    p->y += dy;
}
```

### Возврат структуры
```c
Point createPoint(int x, int y) {
    Point p = {x, y};
    return p;
}
```

## Битовые поля

Экономия памяти: поле занимает указанное количество бит:

```c
struct Flags {
    unsigned int active : 1;   // 1 бит (0 или 1)
    unsigned int ready : 1;    // 1 бит
    unsigned int error : 4;    // 4 бита (0-15)
};

struct Flags f = {.active = 1, .ready = 0, .error = 2};
```

**Подводный камень**: порядок бит зависит от реализации.

## Union — объединение

Все поля используют одну область памяти:

```c
union Data {
    int i;
    float f;
    char str[4];
};

union Data d;
d.i = 42;       // перезаписываем int
printf("%d", d.i);  // 42
d.f = 3.14;    // перезаписываем
printf("%f", d.f);  // 3.14
```

### Применение: экономия памяти или интерпретация данных
```c
union IP {
    unsigned int addr;
    unsigned char bytes[4];
};

union IP ip = {.addr = 0xC0A8010A};  // 192.168.1.10
printf("%d.%d.%d.%d", ip.bytes[0], ip.bytes[1], 
       ip.bytes[2], ip.bytes[3]);
```

## enum — перечисления

Именованные целочисленные константы:

```c
enum Day {MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY};
enum Day today = MONDAY;

if (today == MONDAY) { }
```

### Явные значения
```c
enum Status {ERROR = -1, OK = 0, PENDING = 1};
```

### typedef для enum
```c
typedef enum {FALSE, TRUE} Bool;
Bool flag = TRUE;
```

## Подводные камни

### 1. Присваивание структур
```c
Point p1, p2;
p1 = p2;  // копирует все поля
```

### 2. Сравнение структур
```c
// нельзя: p1 == p2 — только поэлементно
if (p1.x == p2.x && p1.y == p2.y) { }
```

### 3. Гибкие массивы (C99)
```c
struct Dynamic {
    int size;
    int data[];  // гибкий член массива
};
```

### 4. Выравнивание памяти
```c
struct {
    char a;    // +0 байт
    int b;     // выравнивается до +4
} s;
// sizeof больше суммы размеров
```

### 5. Указатель на union после free структуры
```c
struct Node *node = malloc(sizeof(struct Node));
free(node);
// node->next использовать нельзя
```