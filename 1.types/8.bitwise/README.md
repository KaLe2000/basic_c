# Bitwise Operations — Побитовые операции

## Операции

| Операция | Символ | Описание |
|----------|--------|---------|
| AND | `&` | Бит 1 только если оба 1 |
| OR | `\|` | Бит 1 если хотя бы один 1 |
| XOR | `^` | Бит 1 если один 1, но не оба |
| NOT | `~` | Инверсия бит |
| LSHIFT | `<<` | Сдвиг влево |
| RSHIFT | `>>` | Сдвиг вправо |

## Таблицы истинности

### AND (&)
```
a & b | 0 | 1
-------+---+---
0      | 0 | 0
1      | 0 | 1
```

### OR (|)
```
a | b | 0 | 1
-------+---+---
0      | 0 | 1
1      | 1 | 1
```

### XOR (^)
```
a ^ b | 0 | 1
-------+---+---
0      | 0 | 1
1      | 1 | 0
```

## Базовые операции

### AND — маска
```c
// Получить младший байт
unsigned char x = 0xAB;      // 10101011
unsigned char low = x & 0x0F;  // 00001011 = 11

// Получить конкретный бит
unsigned char x = 0b10100101;
int bit_2 = (x >> 2) & 1;  // получить 2-й бит
```

### OR — установка бита
```c
// Установить 3-й бит
unsigned char x = 0b10000000;
x = x | 0b00001000;  // 10001000

// Или x |= 0b00001000;
```

### XOR — toggle
```c
// Инвертировать бит
unsigned char x = 0b10101010;
x = x ^ 0b00001111;  // 10100101

// Если бит уже установлен — сбросится
x = x ^ 0b00001111;  // обратно 10101010
```

### NOT — инверсия
```c
unsigned char x = 0xAA;  // 10101010
unsigned char y = ~x;   // 01010101
```

## Shift operations

### Left shift (<<)
```c
unsigned char x = 1;    // 00000001
x = x << 3;            // 00001000 = 8
x = x << 1;            // 00010000 = 16

// Умножение на 2^n
x = 5 << 2;  // 5 * 4 = 20
```

### Right shift (>>)
```c
unsigned char x = 0b10000000;  // 128
x = x >> 3;                  // 00010000 = 16

// Деление на 2^n (unsigned)
x = 32 >> 2;  // 8
```

**Подводный камень**: для signed negative используется арифметический сдвиг (расширение знака).

## Маски

### Установка бита
```c
#define SET_BIT(x, n) ((x) | (1 << (n)))

unsigned char flags = 0;
flags = SET_BIT(flags, 3);  // 00001000
```

### Сброс бита
```c
#define CLEAR_BIT(x, n) ((x) & ~(1 << (n)))

unsigned char flags = 0b11111111;
flags = CLEAR_BIT(flags, 3);  // 11110111
```

### Toggle бита
```c
#define TOGGLE_BIT(x, n) ((x) ^ (1 << (n)))

unsigned char flags = 0b11110000;
flags = TOGGLE_BIT(flags, 0);  // 11110001
```

### Проверка бита
```c
#define IS_BIT_SET(x, n) (((x) >> (n)) & 1)

unsigned char x = 0b10100000;
IS_BIT_SET(x, 5);  // 1
IS_BIT_SET(x, 4);  // 0
```

## Флаги

### Определение флагов
```c
#define FLAG_READ   0x01  // 00000001
#define FLAG_WRITE 0x02  // 00000010
#define FLAG_EXEC  0x04  // 00000100
#define FLAG_SYNC  0x08  // 00001000
```

### Проверка флагов
```c
unsigned char mode = FLAG_READ | FLAG_WRITE;

if (mode & FLAG_READ) {
    // можно читать
}

// Проверка всех флагов
if ((mode & (FLAG_READ | FLAG_WRITE)) == (FLAG_READ | FLAG_WRITE)) {
    // есть оба флага
}

// Проверка любого
if (mode & (FLAG_READ | FLAG_WRITE)) {
    // есть хотя бы один
}
```

### Изменение флагов
```c
mode |= FLAG_EXEC;     // добавить
mode &= ~FLAG_WRITE;  // удалить
mode ^= FLAG_SYNC;    // toggle
```

## Практические применения

### Чётность
```c
int isOdd(unsigned int x) {
    return x & 1;  // нечётное = 1
}
```

### Абсолютное значение (без ветвлений)
```c
int abs(int x) {
    int sign = x >> 31;      // знаковый бит (0 или -1)
    return (x ^ sign) - sign;  // x + 1 или x - 1
}
```

### Обмен без временной переменной
```c
a = a ^ b;
b = a ^ b;  // b = (a ^ b) ^ b = a
a = a ^ b;  // a = (a ^ b) ^ a = b
```

### Минимум/максимум
```c
int min(int a, int b) {
    int diff = a - b;
    int sign = diff >> 31;
    return a - (diff & sign);  // sign = 0 если a >= b
}
```

## Bit fields

```c
struct Flags {
    unsigned int active : 1;   // 1 бит
    unsigned int ready : 1;    // 1 бит
    unsigned int error : 4;    // 4 бита
    unsigned int : 2;        // 2 неиспользуемых бита
};  // sizeof = 1 байт

struct Flags f = {0};
f.active = 1;
f.error = 7;  // 0-7
```

## Endianness

```c
// Big-endian: старший байт первый
// 0x12345678 → [12][34][56][78]

// Little-endian: младший байт первый
// 0x12345678 → [78][56][34][12]

unsigned int x = 0x12345678;
unsigned char *p = (unsigned char*)&x;

// Little-endian: p[0] = 0x78
// Big-endian:    p[0] = 0x12
```

### Swap endianness
```c
unsigned int swap32(unsigned int x) {
    return ((x & 0x000000FF) << 24) |
           ((x & 0x0000FF00) << 8)  |
           ((x & 0x00FF0000) >> 8)  |
           ((x & 0xFF000000) >> 24);
}
```

## Подводные камни

### 1. Shift на ширину типа
```c
unsigned int x = 1;
x = x << 32;  // UB: ширина unsigned int = 32 бита
x = x << 31;  // OK
```

### 2. Negative shift для signed
```c
int x = -1;
x = x >> 1;  // UB в некоторых реализациях
```

### 3. Overflow для signed
```c
int x = 1 << 31;  // UB: переполнение знакового
unsigned x = 1U << 31;  // OK: 2^31
```