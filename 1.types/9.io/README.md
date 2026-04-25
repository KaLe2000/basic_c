# File I/O — Работа с файлами

## Открытие и закрытие

### fopen / fclose
```c
#include <stdio.h>

FILE *fp = fopen("file.txt", "r");  // чтение
if (fp == NULL) {
    perror("fopen");
    return -1;
}

// работа с файлом

fclose(fp);
```

### Режимы открытия

| Режим | Описание | Файл создаётся | Указатель |
|-------|---------|----------------|-----------|
| r | чтение | нет | начало |
| w | запись | да (обнуляет) | начало |
| a | добавление | да | конец |
| r+ | чтение+запись | нет | начало |
| w+ | чтение+запись | да (обнуляет) | начало |
| a+ | чтение+добавление | да | конец/начало |

### Бинарный режим
```c
FILE *fp = fopen("data.bin", "wb");  // w+b для бинарных
```

## Чтение

### fgetc / fputc — символ
```c
int ch;
while ((ch = fgetc(fp)) != EOF) {
    putchar(ch);
}
```

### fgets — строка
```c
char buf[100];
while (fgets(buf, sizeof(buf), fp) != NULL) {
    fputs(buf, stdout);
}
```

### fscanf — форматированное
```c
int x, y;
while (fscanf(fp, "%d %d", &x, &y) == 2) {
    printf("%d + %d = %d\n", x, y, x + y);
}
```

### fread — бинарное
```c
struct Record {
    int id;
    char name[32];
    double value;
} rec;

size_t n = fread(&rec, sizeof(rec), 1, fp);
// n = количество прочитанных элементов

int *arr = (int*)malloc(n * sizeof(int));
size_t read = fread(arr, sizeof(int), n, fp);
```

## Запись

### fputc
```c
fputc('A', fp);  // записать символ
```

### fputs
```c
fputs("Hello, World!\n", fp);
```

### fprintf
```c
fprintf(fp, "ID: %d, Name: %s, Value: %.2f\n", id, name, value);
```

### fwrite
```c
struct Record rec = {1, "Test", 3.14};
fwrite(&rec, sizeof(rec), 1, fp);
```

## Буферизация

```c
FILE *fp = fopen("file.txt", "w");

// Режимы буферизации
setvbuf(fp, NULL, _IONBF, 0);   // без буфера
setvbuf(fp, buf, _IOFBF, 1024);  // полная буферизация
setvbuf(fp, buf, _IOLBF, 256);   // построчная буферизация

fprintf(fp, "Data");
fflush(fp);  // сбросить буфер
```

### fflush
```c
fflush(fp);  // сбросить буфер записи
fflush(stdin);  // сбросить буфер чтения (не portable)
```

## Позиционирование

### fseek
```c
//SEEK_SET = начало
//SEEK_CUR = текущая позиция
//SEEK_END = конец

fseek(fp, 0, SEEK_SET);    // в начало
fseek(fp, 0, SEEK_END);    // в конец
fseek(fp, 10, SEEK_CUR);   // на 10 байт вперёд
fseek(fp, -10, SEEK_CUR);  // на 10 байт назад
```

### ftell / rewind
```c
long pos = ftell(fp);  // текущая позиция
rewind(fp);           // в начало
```

### fgetpos / fsetpos
```c
fpos_t pos;
fgetpos(fp, &pos);     // сохранить позицию

fseek(fp, 0, SEEK_END);
fsetpos(fp, &pos);    // восстановить
```

## Проверка ошибок

```c
if (ferror(fp)) {
    perror("File error");
}

// Проверка конца файла
if (feof(fp)) {
    printf("End of file reached\n");
}
```

## Temporary files

```c
FILE *tmp = tmpfile();  // автоматическое удаление при close
// создаёт файл в /tmp

char *name = tmpnam(NULL);  // уникальное имя (не safe!)
char name[L_tmpnam];
tmpnam(name);
FILE *fp = fopen(name, "w+");
remove(name);  // удалить вручную
```

## Стандартные потоки

```c
stdin   // стандартный ввод
stdout  // стандартный вывод
stderr  // ошибки

fprintf(stderr, "Error: %s\n", msg);
```

## Полный пример

```c
#include <stdio.h>
#include <stdlib.h>

int copyFile(const char *src, const char *dst) {
    FILE *in = fopen(src, "r");
    if (in == NULL) {
        perror("Source file");
        return -1;
    }

    FILE *out = fopen(dst, "w");
    if (out == NULL) {
        perror("Destination file");
        fclose(in);
        return -1;
    }

    char buf[1024];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), in)) {
        if (fwrite(buf, 1, n, out) != n) {
            perror("Write error");
            fclose(in);
            fclose(out);
            return -1;
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}

int main() {
    return copyFile("input.txt", "output.txt");
}
```

## Binary file example

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    char name[32];
    float score;
} Record;

int writeRecords(const char *filename, Record *records, size_t count) {
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) return -1;

    // Записываем количество
    fwrite(&count, sizeof(count), 1, fp);

    // Записываем массив
    fwrite(records, sizeof(Record), count, fp);

    fclose(fp);
    return 0;
}

Record* readRecords(const char *filename, size_t *count) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) return NULL;

    fread(count, sizeof(*count), 1, fp);
    Record *records = (Record*)malloc((*count) * sizeof(Record));
    fread(records, sizeof(Record), *count, fp);

    fclose(fp);
    return records;
}
```

## Подводные камни

### 1. Проверка fopen
```c
FILE *fp = fopen("file.txt", "r");
*fp;  // UB если файл не найден
```

### 2. Режим w обнуляет
```c
FILE *fp = fopen("file.txt", "w");  // существующий файл удаляется
```

### 3. Бинарный vs текстовый
```c
// Windows: \n → \r\n в текстовом режиме
FILE *fp = fopen("file.bin", "rb");  // бинарный режим
```

### 4. fread/fwrite возвращают количество
```c
// Возвращают количество ЭЛЕМЕНТОВ, не байт
size_t n = fread(buf, 1, 100, fp);  // n = байт (size=1)
size_t n = fread(buf, sizeof(int), 100, fp);  // n = элементов
```