#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 1

#ifdef DEBUG
#define LOG(fmt, ...) fprintf(stderr, "[DEBUG] %s:%d: " fmt "\n", \
                          __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define LOG(...) do {} while(0)
#endif

int* create_array(size_t size) {
    LOG("Creating array of size %zu", size);
    int* arr = (int*)malloc(size * sizeof(int));
    if (arr == NULL) {
        return NULL;
    }
    return arr;
}

int sum_array(int* arr, size_t size) {
    int sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

int main() {
    printf("=== Debugging Demo ===\n\n");

    size_t size = 10;
    int* arr = create_array(size);

    if (arr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (size_t i = 0; i < size; i++) {
        arr[i] = i + 1;
    }

    printf("Array: ");
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("Sum: %d\n", sum_array(arr, size));

    free(arr);
    arr = NULL;

    LOG("Program completed successfully");

    return 0;
}