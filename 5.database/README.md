# SQLite — встраиваемая база данных

## Основы

SQLite — это библиотека, не сервер. База данных в одном файле.

## Подключение

```bash
gcc -Wall -lsqlite3 program.c -o program
```

## API

```c
#include <sqlite3.h>

sqlite3 *db;
char *err;

// Создание/открытие базы
sqlite3_open("test.db", &db);

// SQL запрос
sqlite3_exec(db, "CREATE TABLE users (id INTEGER PRIMARY KEY, name TEXT)", 
            NULL, NULL, &err);

// Подготовленный запрос
sqlite3_stmt *stmt;
sqlite3_prepare_v2(db, "SELECT * FROM users WHERE id = ?", -1, &stmt, NULL);
sqlite3_bind_int(stmt, 1, 42);

// Выполнение
if (sqlite3_step(stmt) == SQLITE_ROW) {
    const char *name = (const char*)sqlite3_column_text(stmt, 1);
}

// Завершение
sqlite3_finalize(stmt);
sqlite3_close(db);
```

## Создание таблицы

```c
const char *sql = 
    "CREATE TABLE IF NOT EXISTS users ("
    "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "  name TEXT NOT NULL,"
    "  email TEXT UNIQUE,"
    "  age INTEGER DEFAULT 0,"
    "  created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
    ")";

if (sqlite3_exec(db, sql, NULL, NULL, &err) != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err);
    sqlite3_free(err);
}
```

## Вставка данных

### Простой способ
```c
sqlite3_exec(db, "INSERT INTO users (name, email) VALUES ('Ivan', 'i@mail')",
            NULL, NULL, &err);
```

### Подготовленные запросы (защита от SQL injection)
```c
const char *sql = "INSERT INTO users (name, email, age) VALUES (?, ?, ?)";
sqlite3_stmt *stmt;
sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

sqlite3_bind_text(stmt, 1, "Ivan", -1, SQLITE_TRANSIENT);
sqlite3_bind_text(stmt, 2, "i@mail", -1, SQLITE_TRANSIENT);
sqlite3_bind_int(stmt, 3, 25);

if (sqlite3_step(stmt) != SQLITE_DONE) {
    fprintf(stderr, "INSERT failed: %s\n", sqlite3_errmsg(db));
}

sqlite3_finalize(stmt);
```

### Массовая вставка в транзакции
```c
sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, &err);

for (int i = 0; i < 1000; i++) {
    // INSERT
}

sqlite3_exec(db, "COMMIT", NULL, NULL, &err);
```

## Выборка данных

### Callback function
```c
typedef struct {
    int count;
    char names[100][50];
} UserResult;

int callback(void *data, int cols, char **values, char **cols_names) {
    UserResult *result = (UserResult*)data;
    strncpy(result->names[result->count], values[1], 49);
    result->count++;
    return 0;  // 0 = продолжить, не 0 = остановить
}

UserResult result = {0};
char *sql = "SELECT id, name FROM users";

sqlite3_exec(db, sql, callback, &result, &err);
printf("Found %d users\n", result.count);
```

### Подготовленные запросы
```c
const char *sql = "SELECT * FROM users WHERE age >= ? AND age <= ?";
sqlite3_stmt *stmt;
sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
sqlite3_bind_int(stmt, 1, 18);
sqlite3_bind_int(stmt, 2, 30);

while (sqlite3_step(stmt) == SQLITE_ROW) {
    int id = sqlite3_column_int(stmt, 0);
    const char *name = (const char*)sqlite3_column_text(stmt, 1);
    int age = sqlite3_column_int(stmt, 3);

    printf("%d: %s (%d)\n", id, name, age);
}

sqlite3_finalize(stmt);
```

## Обновление и удаление

```c
// UPDATE
const char *sql = "UPDATE users SET age = ? WHERE id = ?";
sqlite3_stmt *stmt;
sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
sqlite3_bind_int(stmt, 1, 26);
sqlite3_bind_int(stmt, 2, 1);
sqlite3_step(stmt);
sqlite3_finalize(stmt);

// DELETE
sqlite3_exec(db, "DELETE FROM users WHERE id = 1", NULL, NULL, &err);
```

## Функции binding

| Функция | Тип данных |
|---------|-----------|
| `sqlite3_bind_int(stmt, n, val)` | int |
| `sqlite3_bind_int64(stmt, n, val)` | long long |
| `sqlite3_bind_double(stmt, n, val)` | double |
| `sqlite3_bind_text(stmt, n, str, len, destructor)` | const char* |
| `sqlite3_bind_null(stmt, n)` | NULL |

Destructors: `SQLITE_TRANSIENT` (копирует), `SQLITE_STATIC` (статическая память).

## Обработка ошибок

```c
int result = sqlite3_exec(db, sql, NULL, NULL, &err);
if (result != SQLITE_OK) {
    fprintf(stderr, "SQL Error: %s\n", err);
    sqlite3_free(err);
    
    // Транзакция откатится автоматически
    // или
    sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
}
```

## Транзакции

```c
// Ручное управление для производительности
sqlite3_exec(db, "BEGIN", NULL, NULL, &err);
// ... операции ...
sqlite3_exec(db, "COMMIT", NULL, NULL, &err);

// Или rollback при ошибке
sqlite3_exec(db, "BEGIN", NULL, NULL, &err);
if (some_error) {
    sqlite3_exec(db, "ROLLBACK", NULL, NULL, &err);
} else {
    sqlite3_exec(db, "COMMIT", NULL, NULL, &err);
}
```

## NULL handling

```c
if (sqlite3_column_type(stmt, 0) != SQLITE_NULL) {
    int value = sqlite3_column_int(stmt, 0);
}
```

## Пример: CRUD operations

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

typedef struct {
    int id;
    char name[100];
    double price;
} Product;

int main() {
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (sqlite3_open("shop.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Создание таблицы
    const char *create_sql =
        "CREATE TABLE IF NOT EXISTS products ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  name TEXT NOT NULL,"
        "  price REAL NOT NULL"
        ")";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);

    // INSERT
    const char *insert_sql = "INSERT INTO products (name, price) VALUES (?, ?)";
    sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);

    Product products[] = {
        {"Apple", 0.50},
        {"Bread", 2.00},
        {"Milk", 1.25}
    };

    for (size_t i = 0; i < 3; i++) {
        sqlite3_bind_text(stmt, 1, products[i].name, -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 2, products[i].price);
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }
    sqlite3_finalize(stmt);

    // SELECT
    const char *select_sql = "SELECT * FROM products";
    sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);

    printf("Products:\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char*)sqlite3_column_text(stmt, 1);
        double price = sqlite3_column_double(stmt, 2);
        printf("  %d: %s - $%.2f\n", id, name, price);
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);
    return 0;
}
```

## Полезные функции

```c
sqlite3_last_insert_rowid(db);  // последний INSERT ID
sqlite3_changes(db);           // количество изменённых строк
sqlite3_total_changes(db);     // всех изменений

// экранирование
sqlite3_mprintf("INSERT INTO t VALUES (%Q)", user_input);
// %Q автоматически экранирует ' и NULL
```

## Подводные камни

### 1. SQL injection
```c
// НЕПРАВИЛЬНО
sprintf(sql, "SELECT * FROM users WHERE name = '%s'", input);
sqlite3_exec(db, sql, ...);

// ПРАВИЛЬНО
sqlite3_stmt *stmt;
sqlite3_prepare_v2(db, "SELECT * FROM users WHERE name = ?", ...);
sqlite3_bind_text(stmt, 1, input, -1, SQLITE_TRANSIENT);
```

### 2. Параметры binding
```c
// SQLite использует 1-based индексы
sqlite3_bind_int(stmt, 1, value);  // первый ?
sqlite3_bind_int(stmt, 2, value); // второй ?
```

### 3. Callback в том же потоке
```c
// sqlite3_exec блокирует до завершения
// Используй подготовленные запросы для асинхронности
```

### 4. Типы данных
```c
// SQLite динамическая типизация, но:
// INTEGER: 64-bit signed
// REAL: IEEE 754 double
// TEXT: UTF-8
// BLOB: binary
```

## Компиляция

```bash
# Установка SQLite
sudo apt install libsqlite3-dev   # Ubuntu/Debian
brew install sqlite3             # macOS

gcc -Wall -lsqlite3 program.c -o program
```