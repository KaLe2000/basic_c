# Preprocessor — Директивы препроцессора

Препроцессор обрабатывает код до компиляции: подстановки, условия, макросы.

## Директивы

### #include
```c
#include <stdio.h>       // системный header
#include "local.h"       // локальный header
#include "path/file.h"    // конкретный путь
```

### #define — Константы
```c
#define MAX_SIZE 100
#define BUFFER_SIZE 1024
#define VERSION "1.0.0"

int arr[MAX_SIZE];  // подставляется на этапе препроцессора
```

### #define — Макросы
```c
// Простой макрос
#define SQUARE(x) ((x) * (x))

// Макрос с несколькими аргументами
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Макрос-оператор (скобки ОБЯЗАТЕЛЬНЫ)
#define SQUARE(x) ((x) * (x))
SQUARE(2)      // = 4
SQUARE(1 + 1) // = ((1 + 1) * (1 + 1)) = 4 ✅
#define SQUARE(x) (x * x)
SQUARE(1 + 1) // = (1 + 1 * 1 + 1) = 2 ❌
```

### #define — Multi-statement macros
```c
#define SAFE_FREE(ptr) \
    do { \
        free(ptr); \
        (ptr) = NULL; \
    } while (0)

// Использование
SAFE_FREE(arr);
```

### #define — Функции vs Macros
```c
// Макрос: подставляется inline, нет накладных расходов на вызов
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Inline функция: type-safe, отладка
static inline int max(int a, int b) {
    return a > b ? a : b;
}
```

## Include Guards

```c
#ifndef MODULE_NAME_H
#define MODULE_NAME_H

// код

#endif // MODULE_NAME_H
```

### Pragma once (C99)
```c
#pragma once  // альтернатива (не стандарт ISO, но поддерживается)

// Но include guards переносимее
```

## Условная компиляция

### #ifdef / #ifndef
```c
#define DEBUG 1

#ifdef DEBUG
    printf("Debug: x = %d\n", x);  // компилируется только если DEBUG определён
#endif

#ifndef RELEASE
    log_info();  // компилируется если RELEASE НЕ определён
#endif
```

### #if с выражениями
```c
#if SIZEOF_LONG == 8
    typedef long native_t;  // на 64-битных системах
#else
    typedef int native_t;
#endif
```

### #elif
```c
#if defined(LINUX)
    #include "linux.h"
#elif defined(WINDOWS)
    #include "windows.h"
#else
    #error "Unsupported platform"
#endif
```

## predefined macros

```c
__FILE__   // имя файла: "main.c"
__LINE__  // номер строки: 42
__DATE__  // дата компиляции: "Jan 15 2024"
__TIME__  // время компиляции: "12:30:00"
__STDC__  // стандарт C: 1 (C89), 199901L (C99), 201112L (C11)
__STDC_VERSION__
__func__  // имя текущей функции (C99)
```

```c
printf("File: %s, Line: %d\n", __FILE__, __LINE__);
printf("Compiled: %s at %s\n", __DATE__, __TIME__);
```

## Token-pasting

### ## — объединение токенов
```c
#define CONCAT(a, b) a ## b
CONCAT(char, *)   // → char*
CONCAT(hex_, 0xFF) // → hex_0xFF

// Практическое применение
#define DEFINE_GETTER(type, name) \
    type get_##name() { return name; }

DEFINE_GETTER(int, value)  // создаёт get_value()
```

### # — stringify
```c
#define STRINGIFY(x) #x
STRINGIFY(Hello)    // → "Hello"
STRINGIFY(__LINE__) // → "__LINE__" (строка!)

#define DEBUG_PRINT(fmt, ...) \
    fprintf(stderr, "%s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

DEBUG_PRINT("Value = %d", x);  // main.c:42: Value = 10
```

## Variadic Macros

```c
#define LOG(fmt, ...) \
    printf("[LOG] " fmt "\n", ##__VA_ARGS__)

LOG("Starting");           // [LOG] Starting\n
LOG("Value: %d", 42);    // [LOG] Value: 42\n
```

## Computed includes

```c
// Не работает в C (только в некоторых препроцессорах)
#define HEADER_NAME module.h
#include HEADER_NAME  // ошибка

// Решение
#define INCLUDE_FILE(x) #x
#define INCLUDE(x) INCLUDE_FILE(x)
INCLUDE(module.h)  // не сработает
```

## Подводные камни

### 1. Макрос раскрывается перед проверкой типов
```c
#define SQUARE(x) ((x) * (x))
double d = 5.5;
int i = SQUARE(d);  // i = 30, не 30.25 (double → int)
```

### 2. Побочные эффекты
```c
#define MAX(a, b) ((a) > (b) ? (a) : (b))
int x = 5, y = 10;
MAX(x++, y++);  // ((5++) > (10++) ? (5++) : (10++))
// Результат не определён!
```

### 3. Include guards в каждом файле
```c
// module.h
#ifndef MODULE_H
#define MODULE_H
// код
#endif // MODULE_H
```

### 4. Define после использования
```c
printf("%s", STRING);  // STRING ещё не определён
#define STRING "Hello"
```

## Compile-time assertions (C11)

```c
// Статическая проверка размера
_Static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");
_Static_assert(sizeof(void*) >= 8, "pointers must be 64-bit");

// До C11
#define CTASSERT(c) typedef char static_assert_[(c) ? 1 : -1]
CTASSERT(sizeof(int) >= 4);
```

## Практический пример: Platform detection

```c
#if defined(__linux__)
    #define PLATFORM "Linux"
    #include <unistd.h>
#elif defined(_WIN32)
    #define PLATFORM "Windows"
    #include <windows.h>
#elif defined(__APPLE__)
    #define PLATFORM "macOS"
#else
    #define PLATFORM "Unknown"
#endif

int main() {
    printf("Platform: %s\n", PLATFORM);
    return 0;
}
```