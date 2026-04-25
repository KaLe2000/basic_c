#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void demonstrateDeclaration() {
    printf("=== Объявление и инициализация ===\n\n");

    char s1[] = "Hello";
    char s2[6] = "Hello";
    char s3[10] = "Hello";
    char *s4 = "World";  // указатель на строковый литерал

    printf("s1[] = \"%s\", size=%zu\n", s1, sizeof(s1));
    printf("s2[6] = \"%s\", size=%zu\n", s2, sizeof(s2));
    printf("s3[10] = \"%s\", size=%zu\n", s3, sizeof(s3));
    printf("s4 = \"%s\"\n\n", s4);

    printf("Символы s1: ");
    for (int i = 0; i < sizeof(s1); i++) {
        printf("[%d]='%c' ", s1[i], s1[i]);
    }
    printf("\n\n");
}

void demonstrateStringFunctions() {
    printf("=== Строковые функции ===\n\n");

    char src[] = "Hello";
    char dest[20];

    printf("strlen(\"%s\") = %zu\n", src, strlen(src));

    strcpy(dest, src);
    printf("strcpy() → \"%s\"\n", dest);

    strcat(dest, " World");
    printf("strcat() → \"%s\"\n", dest);

    printf("strcmp(\"Hello\", \"Hello\") = %d\n", strcmp("Hello", "Hello"));
    printf("strcmp(\"Hello\", \"World\") = %d\n", strcmp("Hello", "World"));
    printf("strcmp(\"World\", \"Hello\") = %d\n\n", strcmp("World", "Hello"));
}

void demonstrateStrncpy() {
    printf("=== strncpy подводные камни ===\n\n");

    char buf[10];

    strcpy(buf, "Hello");
    printf("Исходная строка: \"%s\"\n");

    strncpy(buf, "TooLongString", 9);
    printf("strncpy с длинной строкой: \"%s\"\n");
    printf("  Длина через strlen: %zu\n", strlen(buf));
    printf("  Проблема: нет завершающего '\\0'!\n\n");

    strncpy(buf, "Hi", 9);
    buf[9] = '\0';  // Исправление
    printf("strncpy с короткой строкой + '\\0': \"%s\"\n\n");
}

void demonstrateSearchFunctions() {
    printf("=== Функции поиска ===\n\n");

    char text[] = "Hello World";

    char *found = strchr(text, 'o');
    printf("strchr(\"Hello World\", 'o') = \"%s\"\n", found);

    found = strchr(text, 'z');
    printf("strchr(\"Hello World\", 'z') = %s\n\n", found ? found : "NULL");

    found = strstr(text, "World");
    printf("strstr(\"Hello World\", \"World\") = \"%s\"\n\n", found);
}

void demonstrateConversion() {
    printf("=== Преобразования ===\n\n");

    printf("atoi(\"123\") = %d\n", atoi("123"));
    printf("atoi(\"10abc\") = %d\n", atoi("10abc"));
    printf("atoi(\"abc\") = %d (непонятно — ошибка или 0)\n\n", atoi("abc"));

    printf("atof(\"3.14\") = %f\n", atof("3.14"));

    char *end;
    long l = strtol("  42abc", &end, 10);
    printf("strtol(\"  42abc\", &end, 10) = %ld\n", l);
    printf("  end указывает на: \"%s\"\n\n", end);
}

void demonstrateSprintf() {
    printf("=== Форматирование строк ===\n\n");

    char buf[100];
    int value = 42;
    double price = 19.99;

    sprintf(buf, "Value: %d, Price: %.2f", value, price);
    printf("sprintf: \"%s\"\n", buf);

    snprintf(buf, 20, "Value: %d, Price: %.2f", value, price);
    printf("snprintf (ограничено 20): \"%s\"\n\n", buf);
}

void demonstrateScanf() {
    printf("=== Ввод строк ===\n\n");

    char name[50];

    printf("Введите имя (например: Ivan Ivanov):\n");
    int read = scanf("%49s", name);
    printf("scanf %%s прочитал: \"%s\" (только до пробела)\n\n", name);

    while (getchar() != '\n');  // очистка буфера

    printf("Введите имя снова:\n");
    if (fgets(name, 50, stdin) != NULL) {
        size_t len = strlen(name);
        if (len > 0 && name[len - 1] == '\n') {
            name[len - 1] = '\0';
        }
        printf("fgets прочитал: \"%s\" (целиком)\n\n", name);
    }
}

void demonstrateDangerousPatterns() {
    printf("=== Опасные паттерны ===\n\n");

    printf("1. Переполнение буфера:\n");
    char small[5];
    strncpy(small, "Hello", 4);  // без переполнения для демо
    small[4] = '\0';
    printf("   char small[5]; strcpy(small, \"Hello\"); — UB!\n\n");

    printf("2. atoi без проверки:\n");
    printf("   atoi(\"abc\") = %d (это ошибка или \"0\"?)\n\n", atoi("abc"));

    printf("3. scanf без ширины:\n");
    printf("   scanf(\"%%s\", buf); — переполнение если ввод > буфер\n");
    printf("   Правильно: scanf(\"%%49s\", buf);\n\n");
}

int main() {
    demonstrateDeclaration();
    demonstrateStringFunctions();
    demonstrateStrncpy();
    demonstrateSearchFunctions();
    demonstrateConversion();
    demonstrateSprintf();
    demonstrateScanf();
    demonstrateDangerousPatterns();

    return 0;
}