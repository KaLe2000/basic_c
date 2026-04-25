# Перечисления (enum)

Перечисление — это тип данных, определяющий набор именованных целочисленных констант.

## Базовый синтаксис

```c
enum Color {RED, GREEN, BLUE};

enum Color c = RED;
```

## Значения по умолчанию

Первый элемент = 0, каждый следующий = предыдущий + 1:

```c
enum Day {MON, TUE, WED, THU, FRI, SAT, SUN};
// MON=0, TUE=1, WED=2, THU=3, FRI=4, SAT=5, SUN=6
```

## Явные значения

```c
enum Error {OK = 0, ERR_IO = 1, ERR_MEM = 2, ERR_NET = 3};
```

## Частичные значения

```c
enum Flags {A = 1, B, C, D = 10, E, F};
// A=1, B=2, C=3, D=10, E=11, F=12
```

## typedef для удобства

```c
typedef enum {FALSE, TRUE} Bool;
Bool flag = TRUE;
```

## switch с enum

```c
enum Op {ADD, SUB, MUL, DIV};

int calc(int a, int b, enum Op op) {
    switch (op) {
        case ADD: return a + b;
        case SUB: return a - b;
        case MUL: return a * b;
        case DIV: return b != 0 ? a / b : 0;
    }
    return 0;
}
```

## Подводные камни

### 1. Нет проверки типа
```c
enum Color c = 100;  // компилятор не ругнётся
```

### 2. Имена конфликтуют
```c
enum A {X = 1};
enum B {X = 100};  // переопределение X
```

### 3. Небезопасно для ввода
```c
int input;
scanf("%d", &input);
enum Color c = input;  // нет валидации
```

## Практическое применение

### Флаги состояния
```c
typedef enum {
    STATUS_INIT    = 0,
    STATUS_LOADING = 1,
    STATUS_READY   = 2,
    STATUS_ERROR    = -1
} Status;
```

### Выбор режима
```c
typedef enum {
    MODE_TEXT,
    MODE_BINARY,
    MODE_JSON
} FileMode;
```