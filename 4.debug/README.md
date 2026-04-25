# Отладка — gdb и valgrind

## gdb — GNU Debugger

### Компиляция для отладки
```bash
gcc -g -O0 program.c -o program
# -g — отладочная информация
# -O0 — без оптимизации (иначе отладка затруднена)
```

### Запуск
```bash
gdb ./program
gdb ./program core    # анализ дампа памяти
gdb ./program 123   # с аргументами
```

### Команды gdb

| Команда | Сокращение | Описание |
|---------|-----------|---------|
| run [args] | r | запуск |
| quit | q | выход |
| break location | b | точка останова |
| delete n | d n | удалить точку n |
| continue | c | продолжить |
| next | n | следующая строка |
| step | s | войти в функцию |
| until | u | до следующей строки |
| print expr | p | вывести значение |
| display expr | | автоматический print |
| info breakpoints | i b | список breakpoints |
| info locals | i locals | локальные переменные |
| backtrace | bt | стек вызовов |
| frame n | f n | переключить frame |
| list | l | исходный код |
| disassemble | disas | ассемблер |

### Breakpoints

```bash
b main              # на функции
b 42                # на строке 42
b file.c:42         # конкретный файл
b function if x > 0  # условный

info breakpoints    # список
delete 1            # удалить номер 1
disable 2           # отключить (не удалять)
enable 2           # включить
```

### Watchpoints (наблюдение за изменениями)

```bash
watch variable       # остановка при изменении
rwatch variable     # остановка при чтении
awatch variable    # остановка при чтении/записи
```

### Продолжение выполнения

```bash
continue           # c — до следующего breakpoint
next              # n — следующая строка (без входа в функции)
step              # s — следующая строка (с входом в функции)
finish           # завершить текущую функцию
until 42          # u — до строки 42
```

### Просмотр данных

```bash
print x            # значение x
print *ptr         # разыменовать
print arr[0]@10    # массив из 10 элементов
print /x x         # в hex
print /d x         # в decimal
print /t x         # в binary
print 'file.c'::var  # статическая переменная

display x         # автоматически показывать
undisplay 1        # убрать из display
```

### Стек вызовов

```bash
backtrace          # bt — кто кого вызвал
frame 2           # перейти в frame 2
info frame        # информация о frame
up               # вверх по стеку (к вызывающему)
down              # вниз по стеку
```

### Пример сессии

```bash
(gdb) b main
(gdb) r
Breakpoint 1, main () at program.c:10
10          int x = 42;
(gdb) n
(gdb) print x
$1 = 42
(gdb) n
(gdb) bt
#0  main () at program.c:10
(gdb) q
```

### Post-mortem анализ

```bash
gdb ./program core    # открыть дамп
bt                   # стек на момент crash
print variable        # значение переменной
```

## valgrind — проверка памяти

### Установка
```bash
# Ubuntu/Debian
sudo apt install valgrind

# macOS
brew install valgrind
```

### Основные проверки

```bash
valgrind --leak-check=full ./program
valgrind --leak-check=summary ./program
valgrind --show-leak-kinds=all ./program
```

### Опции

| Опция | Описание |
|-------|---------|
| `--leak-check=full` | детальный отчёт об утечках |
| `--show-leak-kinds=all` | все типы утечек |
| `--track-origins=yes` | откуда пришли данные |
| `--error-exitcode=1` | exit code при ошибках |
| `--vgdb=error` | interactive debugging |

### Типы утечек

```
Definitely lost: память не освобождена, нет указателя
Indirectly lost: память в tree, нет root указателя
Possibly lost: есть указатель, может быть утечка
Still reachable: не освобождена, но есть root (global)

This is fine for globals, but consider freeing anyway.
```

### Пример вывода

```
==12345== Memcheck, a memory error detector
==12345== Copyright (C) 2002-2017, and GNU GPL'd
==12345== Using Valgrind-3.13.0 and LibVEX
==12345== For bug reports: http://www.valgrind.org

==12345== Invalid write of size 4
==12345==    at 0x4: main (program.c:42)
==12345==  Address 0x0 is not in stack

==12345== HEAP SUMMARY:
==12345==   in use at exit: 0 bytes in 0 blocks
==12345==   total heap usage: 10 allocs, 9 frees, 1024 bytes allocated
==12345==   1 bytes in 1 blocks are definitely lost

==12345== LEAK SUMMARY:
==12345==    definitely lost: 1 bytes in 1 blocks
==12345==    indirectly lost: 0 bytes in 0 blocks
```

### memcheck — проверки

- **Invalid write/read** — запись/чтение за границами
- **Invalid free** — двойной free или free стека
- **Uninitialized values** — использование неинициализированных
- **Memory leaks** — утечки памяти
- **Mismatched free** — free/delete mismatch

### Команда для CI

```bash
valgrind --leak-check=full --error-exitcode=1 ./program
echo $?  # 0 = OK, 1 = errors
```

## address sanitizer (ASan)

Альтернатива для быстрой проверки:

```bash
gcc -fsanitize=address -g program.c -o program
./program

# Также для UB
gcc -fsanitize=undefined -g program.c -o program
```

## memory sanitizer (MSan) — отслеживание неинициализированных

```bash
gcc -fsanitize=memory -g -O1 program.c -o program
```

## thread sanitizer (TSan) — race conditions

```bash
gcc -fsanitize=thread -g program.c -o program
./program
```

## Подготовка к отладке

### Assertions
```c
#include <assert.h>

assert(ptr != NULL);
assert(count > 0);
```

### Debug logging
```c
#ifdef DEBUG
    printf("DEBUG: x=%d, ptr=%p\n", x, (void*)ptr);
#endif
```

### Compile flags
```makefile
ifeq ($(DEBUG),1)
    CFLAGS += -g -O0 -DDEBUG
else
    CFLAGS += -O2 -DNDEBUG
endif
```

## Cheat sheet

```bash
# Компиляция для отладки
gcc -g -O0 -Wall -Wextra program.c -o program

# Запуск в gdb
gdb ./program
(gdb) b main
(gdb) r
(gdb) n/s
(gdb) p variable

# Запуск в valgrind
valgrind --leak-check=full --vgdb=error ./program

# ASan (быстрее valgrind)
gcc -fsanitize=address -g program.c -o program
./program

# CI/CD
valgrind --leak-check=full --error-exitcode=1 ./test_runner
```