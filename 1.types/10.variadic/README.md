# Variadic Functions — Функции с переменным числом аргументов

## stdarg.h

```c
#include <stdarg.h>

int sum(int count, ...);  // объявление с ...

va_list args;             // объект для итерации
va_start(args, count);    // начать с последнего именованного
va_arg(args, int);       // получить следующий int
va_end(args);           // завершить
```

## Базовый пример

```c
#include <stdio.h>
#include <stdarg.h>

int sum(int count, ...) {
    int total = 0;
    va_list args;

    va_start(args, count);
    for (int i = 0; i < count; i++) {
        total += va_arg(args, int);
    }
    va_end(args);

    return total;
}

int main() {
    printf("%d\n", sum(3, 1, 2, 3));     // 6
    printf("%d\n", sum(5, 1, 2, 3, 4, 5)); // 15
}
```

## printf-lite

```c
#include <stdio.h>
#include <stdarg.h>

void printAll(int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        if (i > 0) printf(", ");
        printf("%d", va_arg(args, int));
    }
    va_end(args);
    printf("\n");
}

int main() {
    printAll(3, 1, 2, 3);      // 1, 2, 3
    printAll(5, 10, 20, 30, 40, 50); // 10, 20, 30, 40, 50
}
```

## va_copy

```c
void printTwice(int count, ...) {
    va_list args1, args2;
    va_start(args1, count);
    va_copy(args2, args1);  // копия для второго прохода

    for (int i = 0; i < count; i++) {
        printf("%d ", va_arg(args1, int));
    }
    printf("\n");

    for (int i = 0; i < count; i++) {
        printf("%d ", va_arg(args2, int));
    }
    va_end(args1);
    va_end(args2);
}
```

## Строки разных типов

```c
#include <stdio.h>
#include <stdarg.h>

void printValues(int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        if (i > 0) printf(" | ");

        // Упрощённый пример: чередуем int и char*
        if (i % 2 == 0) {
            printf("%d", va_arg(args, int));
        } else {
            printf("%s", va_arg(args, char*));
        }
    }
    va_end(args);
    printf("\n");
}

int main() {
    printValues(4, 42, "hello", 100, "world");
}
```

## Error handling

```c
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

typedef enum {
    ERR_OK = 0,
    ERR_INVALID_ARGS,
    ERR_MALLOC,
} error_t;

error_t sum(int count, int *result, ...) {
    if (result == NULL) {
        return ERR_INVALID_ARGS;
    }

    va_list args;
    va_start(args, result);
    *result = 0;

    for (int i = 0; i < count; i++) {
        *result += va_arg(args, int);
    }

    va_end(args);
    return ERR_OK;
}

int main() {
    int result;
    error_t err = sum(3, &result, 1, 2, 3);

    if (err != ERR_OK) {
        fprintf(stderr, "Error: %d\n", err);
        return 1;
    }
    printf("Sum: %d\n", result);
}
```

## va_list как параметр

```c
#include <stdarg.h>

int vsum(int count, va_list args) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += va_arg(args, int);
    }
    return total;
}

int sum(int count, ...) {
    va_list args;
    va_start(args, count);
    int result = vsum(count, args);
    va_end(args);
    return result;
}

int min(int count, ...) {
    va_list args;
    va_start(args, count);
    int min_val = va_arg(args, int);

    for (int i = 1; i < count; i++) {
        int val = va_arg(args, int);
        if (val < min_val) min_val = val;
    }

    va_end(args);
    return min_val;
}
```

## Format string parsing (simplified)

```c
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void debugPrint(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'd':
                    printf("%d", va_arg(args, int));
                    break;
                case 'f':
                    printf("%f", va_arg(args, double));
                    break;
                case 's':
                    printf("%s", va_arg(args, char*));
                    break;
                case '%':
                    printf("%%");
                    break;
                default:
                    printf("%%%c", *fmt);
            }
        } else {
            putchar(*fmt);
        }
        fmt++;
    }

    va_end(args);
    putchar('\n');
}

int main() {
    debugPrint("int=%d, str=%s, float=%f", 42, "hello", 3.14);
}
```

## Подводные камни

### 1. Типизация
```c
// Нет проверки типов! Компилятор не предупредит
sum(3, 1.5, 2.5, 3.5);  // UB: double вместо int
```

### 2. Побочные эффекты
```c
// UB: va_arg может вычисляться несколько раз
printf("%d", sum(3, x++, x++, x++));
```

### 3. va_copy после va_end
```c
va_end(args);
va_copy(args2, args);  // UB: args уже завершён
```

### 4. Alignment
```c
// Типы < int передаются как int
// Типы < double передаются как double
// Остальные — как есть
```

## va_start после определения

```c
// va_start требует последний именованный параметр
int sum(...) {  // ошибка: нужен хотя бы один именованный
    va_list args;
    va_start(args, count);  // count не определён
}
```

## Практический min/max

```c
#include <stdarg.h>

int varmin(int first, ...) {
    int min_val = first;
    va_list args;
    va_start(args, first);

    int val;
    while ((val = va_arg(args, int)) != 0) {  // 0 = sentinel
        if (val < min_val) min_val = val;
    }

    va_end(args);
    return min_val;
}

int main() {
    printf("%d\n", varmin(5, 3, 7, 2, 9, 0));  // 2
}
```