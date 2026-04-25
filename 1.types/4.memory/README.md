# Управление памятью

В языке C управление памятью — ответственность программиста. Стандартная библиотека предоставляет функции для динамического выделения памяти.

## Стек и куча

| Стек (Stack) | Куча (Heap) |
|-------------|-------------|
| автоматическое управление | ручное управление |
| локальные переменные | динамические данные |
| small, fast | large, flexible |
| `int x;` | `malloc()` |

## malloc — выделение памяти

```c
#include <stdlib.h>

int *ptr = (int*)malloc(sizeof(int));  // 1 элемент
if (ptr == NULL) {
    // обработка ошибки
}
*ptr = 42;
free(ptr);  // освобождение
```

### Выделение массива
```c
size_t n = 100;
int *arr = (int*)malloc(n * sizeof(int));
if (arr == NULL) {
    // обработка ошибки
}
// использовать массив
free(arr);
```

### Забыли freed — утечка памяти (memory leak):
```c
void badFunction() {
    int *ptr = (int*)malloc(sizeof(int));
    // ... используем ptr
    return;  // ptr потерян навсегда!
}
```

## calloc — выделение с обнулением

```c
int *arr = (int*)calloc(10, sizeof(int));  // 10 элементов = 0
```

**Разница**: malloc не инициализирует память (мусор), calloc обнуляет.

## realloc — перевыделение

```c
int *arr = (int*)malloc(5 * sizeof(int));
// ... заполняем массив

arr = (int*)realloc(arr, 10 * sizeof(int));  // увеличиваем
```

**Подводный камень**: realloc может вернуть новый указатель:
```c
int *newArr = (int*)realloc(arr, newSize);
if (newArr == NULL) {
    // обработка ошибки, old arr still valid
}
arr = newArr;  // обновляем указатель
```

### Уменьшение размера
```c
arr = (int*)realloc(arr, 5 * sizeof(int));  // уменьшаем
```

### Освобождение через realloc
```c
arr = (int*)realloc(arr, 0);  // equivalent to free(arr)
```

## free — освобождение памяти

```c
free(ptr);
ptr = NULL;  // защита от UB
```

**Важно**: после free память использовать нельзя.

## Подводные камни

### 1. Утечка памяти (Memory Leak)
```c
void leak() {
    int *ptr = malloc(100);
    // забыли free(ptr)
}  // ptr потерян при выходе из функции
```

### 2. Двойное освобождение (Double Free)
```c
free(ptr);
free(ptr);  // UB: повторное освобождение
```

### 3. Использование после free
```c
free(ptr);
*ptr = 10;  // UB: использование freed memory
```

### 4. Освобождение нединамической памяти
```c
int arr[10];
free(arr);  // UB: arr на стеке
```

### 5. Неправильный размер
```c
double *d = (double*)malloc(sizeof(int));  // мало памяти для double
```

### 6. Разыменование NULL
```c
int *ptr = NULL;
*ptr = 10;  // UB: запись по NULL
```

### 7. Сдвиг неинициализированного void*
```c
void *ptr = malloc(10);
char *c = (char*)ptr;
c++;  // OK
```

## Динамические структуры данных

### Динамический массив (vector-подобный)
```c
typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} Vector;

void vectorInit(Vector *v) {
    v->capacity = 2;
    v->size = 0;
    v->data = (int*)malloc(v->capacity * sizeof(int));
}

void vectorPush(Vector *v, int value) {
    if (v->size >= v->capacity) {
        v->capacity *= 2;
        v->data = (int*)realloc(v->data, v->capacity * sizeof(int));
    }
    v->data[v->size++] = value;
}

void vectorFree(Vector *v) {
    free(v->data);
    v->data = NULL;
}
```

### Связный список
```c
typedef struct Node {
    int value;
    struct Node *next;
} Node;

Node* nodeCreate(int value) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->value = value;
    node->next = NULL;
    return node;
}

void nodeFree(Node *head) {
    while (head) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}
```

## valgrind — проверка утечек

```bash
valgrind --leak-check=full ./program
```

Вывод:
```
==12345== Memcheck, a memory error detector
==12345== HEAP SUMMARY:
==12345==   in use at exit: 0 bytes in 0 blocks
==12345==   total heap usage: 10 allocs, 10 frees, 1024 bytes allocated
==12345== All heap blocks were freed -- no leaks are possible
```

## Анализ утечек

### Утечка с сохранением указателя
```c
int *ptr = malloc(100);
ptr = malloc(200);  // первый malloc потерян!
```

### Утечка в условной ветке
```c
if (condition) {
    int *ptr = malloc(100);
    // забыли free
}
```

### Утечка через возврат
```c
int* create() {
    int arr[100];  // стек, не malloc
    return arr;    // UB: возврат локальной переменной
}
```

## Практические правила

1. **Проверяй malloc/calloc/realloc на NULL**
2. **Всегда free то, что malloc**
3. **После free обнуляй указатель**
4. **Не используй память после free**
5. **Выделяй достаточно памяти для типа**
6. **Используй valgrind для проверки**