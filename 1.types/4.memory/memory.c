#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void demonstrateMalloc() {
    printf("=== malloc — выделение памяти ===\n\n");

    int *ptr = (int*)malloc(sizeof(int));
    if (ptr == NULL) {
        printf("Ошибка выделения памяти\n");
        return;
    }

    printf("До записи: *ptr = %d (мусор)\n", *ptr);
    *ptr = 42;
    printf("После записи: *ptr = %d\n", *ptr);

    free(ptr);
    ptr = NULL;
    printf("После free: ptr = %p\n\n", (void*)ptr);
}

void demonstrateCalloc() {
    printf("=== calloc — выделение с обнулением ===\n\n");

    int *arr = (int*)calloc(5, sizeof(int));
    if (arr == NULL) {
        printf("Ошибка выделения памяти\n");
        return;
    }

    printf("Массив calloc (обнулён): ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    arr[0] = 10;
    arr[4] = 50;
    printf("После заполнения: %d ... %d\n", arr[0], arr[4]);

    free(arr);
    printf("\n");
}

void demonstrateRealloc() {
    printf("=== realloc — перевыделение ===\n\n");

    int *arr = (int*)malloc(3 * sizeof(int));
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;

    printf("Исходный массив: %d, %d, %d\n", arr[0], arr[1], arr[2]);
    printf("Размер old: %zu байт\n", 3 * sizeof(int));

    int *newArr = (int*)realloc(arr, 6 * sizeof(int));
    if (newArr == NULL) {
        printf("Ошибка realloc\n");
        free(arr);
        return;
    }

    arr = newArr;
    arr[3] = 40;
    arr[4] = 50;
    arr[5] = 60;

    printf("Расширенный массив: ");
    for (int i = 0; i < 6; i++) {
        printf("%d ", arr[i]);
    }
    printf("\nРазмер new: %zu байт\n\n", 6 * sizeof(int));

    free(arr);
}

void demonstrateDynamicVector() {
    printf("=== Динамический вектор ===\n\n");

    size_t capacity = 2;
    size_t size = 0;
    int *data = (int*)malloc(capacity * sizeof(int));

    printf("Добавляем элементы:\n");
    for (int i = 1; i <= 10; i++) {
        if (size >= capacity) {
            capacity *= 2;
            data = (int*)realloc(data, capacity * sizeof(int));
            printf("  Расширяем: capacity = %zu\n", capacity);
        }
        data[size++] = i * 10;
    }

    printf("Массив: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    free(data);
    printf("\n");
}

void demonstrateLinkedList() {
    printf("=== Связный список ===\n\n");

    typedef struct Node {
        int value;
        struct Node *next;
    } Node;

    Node* createNode(int value) {
        Node *node = (Node*)malloc(sizeof(Node));
        node->value = value;
        node->next = NULL;
        return node;
    }

    Node *head = createNode(10);
    Node *current = head;
    for (int i = 20; i <= 50; i += 10) {
        current->next = createNode(i);
        current = current->next;
    }

    printf("Связный список: ");
    current = head;
    while (current) {
        printf("%d ", current->value);
        current = current->next;
    }
    printf("\n");

    current = head;
    while (current) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    printf("\n");
}

void demonstrateMemoryLeak() {
    printf("=== Демонстрация утечки (не повторяй!) ===\n\n");

    printf("Утечка: перезапись указателя\n");
    int *ptr = (int*)malloc(100);
    printf("  Первый malloc потерян при перезаписи\n");
    ptr = (int*)malloc(200);  // первый malloc потерян

    printf("Решение: сохранить новый указатель\n");
    int *newPtr = (int*)realloc(ptr, 300);
    if (newPtr) {
        ptr = newPtr;  // правильно
    }

    free(ptr);
    printf("\n");
}

void demonstrateDoubleFree() {
    printf("=== Защита от double free ===\n\n");

    int *ptr = (int*)malloc(sizeof(int));
    *ptr = 42;

    printf("Первый free\n");
    free(ptr);

    printf("Обнуляем указатель\n");
    ptr = NULL;

    printf("Проверка перед free:\n");
    if (ptr != NULL) {
        free(ptr);
        printf("  Освобождён\n");
    } else {
        printf("  Указатель NULL, не освобождаем\n");
    }
    printf("\n");
}

int main() {
    demonstrateMalloc();
    demonstrateCalloc();
    demonstrateRealloc();
    demonstrateDynamicVector();
    demonstrateLinkedList();
    demonstrateMemoryLeak();
    demonstrateDoubleFree();

    return 0;
}