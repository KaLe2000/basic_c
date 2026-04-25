# Язык программирования C — Учебный проект

Учебный проект для освоения языка C с нуля. Покрывает все ключевые темы от примитивных типов до многопоточности и сетевого программирования.

## Структура

```
basic_c/
├── README.md              # документация
├── Makefile            # сборка
├── run_tests.sh        # запуск всех тестов
│
├── 1.types/           # типы данных
│   ├── 0.primitives/   # примитивные типы
│   ├── 1.arrays/       # массивы
│   ├── 2.strings/      # строки
│   ├── 3.pointers/     # указатели
│   ├── 4.memory/       # управление памятью
│   ├── 5.structures/   # структуры, union
│   ├── 6.enums/        # перечисления
│   ├── 7.preprocessor/  # препроцессор
│   ├── 8.bitwise/       # побитовые операции
│   ├── 9.io/           # файловый ввод/вывод
│   ├── 10.variadic/     # variadic функции
│   ├── 11.threads/     # многопоточность
│   ├── 12.sockets/     # сетевые сокеты
│   ├── arrays/          # функции для массивов
│   └── trees/          # BST, AVL деревья
│
├── 4.debug/          # отладка (gdb, valgrind)
├── 5.database/        # SQLite
├── coding_standards/  # стандарты кода
│
├── 2.algos/           # алгоритмы
│   └── sort/         # сортировки
│
├── 3.build/          # Makefile/cmake
└── 0.testing/        # тестовые примеры
```

## Быстрый старт

```bash
# Сборка демо
gcc 1.types/0.primitives/primitives.c -o primitives -lm

# Тесты
make test
bash run_tests.sh

# Все тесты вручную
for d in 1.types/*/; do
  test=$(find "$d" -name "test*.c" | head -1)
  [ -f "$test" ] && gcc -pthread -Wall -Wextra "$test" -o /tmp/t -lm 2>/dev/null && /tmp/t | tail -3
done
```

## Содержание по уровням

### Junior C Developer (обязательно)

| Раздел | Описание | Тесты |
|--------|----------|-------|
| primitives | char, int, float, double, size_t | 11 |
| arrays | статические и динамические массивы | 6 |
| strings | char[], string.h | 12 |
| pointers | &, *, void*, функции | 10 |
| memory | malloc/calloc/realloc/free | 7 |
| structures | struct, typedef, union | 9 |
| enums | enum | - |
| preprocessor | #define, macros | - |
| bitwise | &, \|, ^, <<, >> | - |
| io | fopen, fread, fwrite | - |
| variadic | va_list, ... | - |
| sort | bubble, merge, quick | 12 |
| trees | BST, AVL | 9 |
| **ИТОГО** | | **76** |

### Дополнительно

| Раздел | Описание |
|--------|----------|
| threads | pthread, mutex, condition variables |
| sockets | TCP/UDP server/client |
| debug | gdb, valgrind |
| database | SQLite |

## Компиляция

```bash
gcc -Wall -Wextra -Werror -std=c11 -g program.c -o program -lm
# -pthread для потоков
# -lsqlite3 для SQLite
```

## Тесты

```bash
make test                          # основные
bash run_tests.sh                 # все
valgrind --leak-check=full ./prog  # проверка памяти
```

## Новые темы

### Потоки (pthread)
```bash
gcc -pthread 1.types/11.threads/testThreads.c -o testThreads
./testThreads
```

### Сокеты
```bash
# Сервер (в одном терминале)
gcc 1.types/12.sockets/tcp_server.c -o server && ./server

# Клиент (в другом терминале)
gcc 1.types/12.sockets/tcp_client.c -o client && ./client localhost 8080
```

### SQLite
```bash
sudo apt install libsqlite3-dev
gcc -lsqlite3 5.database/sqlite_demo.c -o sqlite_demo
./sqlite_demo
```

### Отладка
```bash
gcc -g -O0 -Wall program.c -o program
gdb ./program

valgrind --leak-check=full ./program
```

## Подводные камни

1. **Переполнение** — `INT_MAX + 1` UB
2. **Float сравнение** — `0.1 + 0.2 == 0.3` false
3. **Указатели** — проверяй NULL
4. **Memory leaks** — `free()`, потом `ptr = NULL`
5. **Race conditions** — mutex для shared данных
6. **Buffer overflow** — проверяй размеры

## Линтеры

```bash
cppcheck --enable=all --std=c11 src/
gcc -Wall -Wextra -Werror -pedantic -std=c11
```

## Ресурсы

- C11 Standard: ISO/IEC 9899:2011
- man 3 function_name
- pthread_create(3), socket(7)