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

    printf("=== SQLite Demo ===\n\n");

    // Открытие/создание базы данных
    if (sqlite3_open("shop.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    printf("Database 'shop.db' opened\n");

    // Создание таблицы
    const char *create_sql =
        "CREATE TABLE IF NOT EXISTS products ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  name TEXT NOT NULL,"
        "  price REAL NOT NULL"
        ")";

    if (sqlite3_exec(db, create_sql, NULL, NULL, NULL) != SQLITE_OK) {
        fprintf(stderr, "CREATE TABLE error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    printf("Table 'products' created/verified\n");

    // INSERT с подготовленным запросом
    const char *insert_sql = "INSERT INTO products (name, price) VALUES (?, ?)";
    sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);

    Product products[] = {
        {0, "Apple", 0.50},
        {0, "Bread", 2.00},
        {0, "Milk", 1.25},
        {0, "Eggs (10)", 3.50}
    };
    size_t count = sizeof(products) / sizeof(products[0]);

    printf("\nInserting %zu products:\n", count);
    for (size_t i = 0; i < count; i++) {
        sqlite3_bind_text(stmt, 1, products[i].name, -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 2, products[i].price);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            int id = (int)sqlite3_last_insert_rowid(db);
            printf("  [%d] %s - $%.2f\n", id, products[i].name, products[i].price);
        }

        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
    }
    sqlite3_finalize(stmt);

    // SELECT
    const char *select_sql = "SELECT id, name, price FROM products ORDER BY price";
    sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);

    printf("\nAll products:\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char*)sqlite3_column_text(stmt, 1);
        double price = sqlite3_column_double(stmt, 2);
        printf("  %2d: %-15s $%.2f\n", id, name, price);
    }
    sqlite3_finalize(stmt);

    // UPDATE
    const char *update_sql = "UPDATE products SET price = ? WHERE name = ?";
    sqlite3_prepare_v2(db, update_sql, -1, &stmt, NULL);
    sqlite3_bind_double(stmt, 1, 1.00);
    sqlite3_bind_text(stmt, 2, "Apple", -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    printf("\nUpdated Apple price to $1.00\n");

    // DELETE
    const char *delete_sql = "DELETE FROM products WHERE name = ?";
    sqlite3_prepare_v2(db, delete_sql, -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, "Eggs (10)", -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    printf("Deleted 'Eggs (10)'\n");

    // Итоговый SELECT
    select_sql = "SELECT id, name, price FROM products ORDER BY id";
    sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);

    printf("\nFinal products:\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char *name = (const char*)sqlite3_column_text(stmt, 1);
        double price = sqlite3_column_double(stmt, 2);
        printf("  %2d: %-15s $%.2f\n", id, name, price);
    }
    sqlite3_finalize(stmt);

    // Закрытие
    sqlite3_close(db);
    printf("\nDatabase closed\n");

    return 0;
}