#include <stdio.h>
#include <limits.h>
#include <float.h>

void printIntegerTypes() {
    printf("=== Целочисленные типы ===\n\n");

    printf("char:\n");
    printf("  размер: %zu байт\n", sizeof(char));
    printf("  signed диапазон: %d ... %d\n", CHAR_MIN, CHAR_MAX);
    printf("  unsigned диапазон: 0 ... %u\n", CHAR_MAX);

    char ch = 'A';
    printf("  пример: '%c' = %d (ASCII)\n\n", ch, ch);

    printf("short (short int):\n");
    printf("  размер: %zu байт\n", sizeof(short));
    printf("  диапазон: %d ... %d\n", SHRT_MIN, SHRT_MAX);

    printf("int:\n");
    printf("  размер: %zu байт\n", sizeof(int));
    printf("  диапазон: %d ... %d\n", INT_MIN, INT_MAX);

    printf("long (long int):\n");
    printf("  размер: %zu байт\n", sizeof(long));
    printf("  диапазон: %ld ... %ld\n", LONG_MIN, LONG_MAX);

    printf("long long (long long int):\n");
    printf("  размер: %zu байт\n", sizeof(long long));
    printf("  диапазон: %lld ... %lld\n", LLONG_MIN, LLONG_MAX);

    printf("unsigned int:\n");
    printf("  диапазон: 0 ... %u\n", UINT_MAX);

    unsigned int positive = 100;
    signed int negative = -50;
    printf("  примеры: positive=%u, negative=%d\n\n", positive, negative);
}

void printFloatingPointTypes() {
    printf("=== Типы с плавающей запятой ===\n\n");

    printf("float:\n");
    printf("  размер: %zu байт\n", sizeof(float));
    printf("  диапазон: %e ... %e\n", FLT_MIN, FLT_MAX);
    printf("  точность: %d бит мантиссы (~%d цифр)\n", FLT_MANT_DIG, FLT_DIG);

    float f_pi = 3.14159f;
    float f_e = 2.71828f;
    printf("  примеры: pi=%.5f, e=%.5f\n\n", f_pi, f_e);

    printf("double:\n");
    printf("  размер: %zu байт\n", sizeof(double));
    printf("  диапазон: %e ... %e\n", DBL_MIN, DBL_MAX);
    printf("  точность: %d бит мантиссы (~%d цифр)\n", DBL_MANT_DIG, DBL_DIG);

    double d_pi = 3.141592653589793;
    printf("  пример: pi=%.15f\n\n", d_pi);

    printf("long double:\n");
    printf("  размер: %zu байт\n", sizeof(long double));
}

void demonstrateCharType() {
    printf("=== Символьный тип char ===\n\n");

    char letter = 'G';
    char digit = '5';
    char space = ' ';
    char tab = '\t';
    char newline = '\n';

    printf("символ 'G' = %d (ASCII)\n", letter);
    printf("символ '5' = %d (ASCII), число %d\n", digit, digit - '0');
    printf("символ ' ' (пробел) = %d\n", space);
    printf("символ '\\t' = %d (табуляция)\n", tab);
    printf("символ '\\n' = %d (перевод строки)\n\n", newline);

    char upperA = 65;
    char lowerA = 97;
    printf("код 65 = '%c'\n", upperA);
    printf("код 97 = '%c'\n\n", lowerA);
}

void demonstrateIntegerOverflow() {
    printf("=== Переполнение целочисленных типов ===\n\n");

    signed char sc_max = SCHAR_MAX;
    signed char sc_min = SCHAR_MIN;
    printf("signed char: %d ... %d\n", SCHAR_MIN, SCHAR_MAX);

    signed char overflow = SCHAR_MAX;
    printf("  SCHAR_MAX + 1 = %d (переполнение!)\n", overflow + 1);

    unsigned char uc_max = UCHAR_MAX;
    unsigned char overflowU = UCHAR_MAX;
    printf("unsigned char: 0 ... %u\n", UCHAR_MAX);
    printf("  UCHAR_MAX + 1 = %u (переполнение!)\n", overflowU + 1);

    int i_max = INT_MAX;
    printf("\nint: %d ... %d\n", INT_MIN, INT_MAX);
    printf("  INT_MAX + 1 = %d (переполнение!)\n\n", i_max + 1);
}

void demonstrateFloatPrecision() {
    printf("=== Точность чисел с плавающей запятой ===\n\n");

    float a = 0.1f;
    float b = 0.2f;
    float sum = a + b;

    printf("0.1f + 0.2f = %.20f\n", sum);
    printf("0.3f       = %.20f\n", 0.3f);
    printf("равны? %s\n\n", (sum == 0.3f) ? "да" : "нет");

    printf("двойное сложение:\n");
    for (int i = 0; i < 10; i++) {
        sum += 0.1f;
    }
    printf("0.1f * 10 = %.20f\n\n", sum);

    double d_sum = 0.1 + 0.2;
    printf("double: 0.1 + 0.2 = %.20f\n", d_sum);
}

void demonstrateTypeCasting() {
    printf("=== Преобразование типов ===\n\n");

    int whole = 7;
    int divider = 3;
    int wrong = whole / divider;
    double correct = (double)whole / divider;

    printf("целочисленное деление: %d / %d = %d\n", whole, divider, wrong);
    printf("вещественное деление: (double)%d / %d = %.6f\n\n", whole, divider, correct);

    double d = 9.99;
    int truncated = (int)d;
    int rounded = (int)(d + 0.5);

    printf("отбрасывание: (int)%.2f = %d\n", d, truncated);
    printf("округление:  (int)(%.2f + 0.5) = %d\n\n", d, rounded);
}

void printSizeofSizes() {
    printf("=== Размеры типов на этой системе ===\n\n");

    printf("sizeof(char)      = %zu\n", sizeof(char));
    printf("sizeof(short)     = %zu\n", sizeof(short));
    printf("sizeof(int)      = %zu\n", sizeof(int));
    printf("sizeof(long)     = %zu\n", sizeof(long));
    printf("sizeof(long long)  = %zu\n", sizeof(long long));
    printf("sizeof(float)     = %zu\n", sizeof(float));
    printf("sizeof(double)   = %zu\n", sizeof(double));
    printf("sizeof(long double) = %zu\n", sizeof(long double));
    printf("sizeof(size_t)   = %zu\n", sizeof(size_t));
}

int main() {
    printIntegerTypes();
    printFloatingPointTypes();
    demonstrateCharType();
    demonstrateIntegerOverflow();
    demonstrateFloatPrecision();
    demonstrateTypeCasting();
    printSizeofSizes();

    return 0;
}