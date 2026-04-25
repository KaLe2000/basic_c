#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

int testsPassed = 0;
int testsFailed = 0;

void testOpenDatabase() {
    printf("Тест: открытие базы данных\n");
    
    sqlite3 *db;
    int result = sqlite3_open(":memory:", &db);
    
    if (result == SQLITE_OK) {
        printf("  [PASS] база данных открыта\n");
        testsPassed++;
        sqlite3_close(db);
    } else {
        printf("  [FAIL] код ошибки: %d\n", result);
        testsFailed++;
    }
}

void testCreateTable() {
    printf("Тест: создание таблицы\n");
    
    sqlite3 *db;
    sqlite3_open(":memory:", &db);
    
    char *err;
    const char *sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT)";
    int result = sqlite3_exec(db, sql, NULL, NULL, &err);
    
    if (result == SQLITE_OK) {
        printf("  [PASS] таблица создана\n");
        testsPassed++;
    } else {
        printf("  [FAIL] %s\n", err);
        sqlite3_free(err);
        testsFailed++;
    }
    
    sqlite3_close(db);
}

void testInsertAndSelect() {
    printf("Тест: INSERT и SELECT\n");
    
    sqlite3 *db;
    sqlite3_open(":memory:", &db);
    
    sqlite3_exec(db, "CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT)", NULL, NULL, NULL);
    
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "INSERT INTO test (value) VALUES (?)", -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, "hello", -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    sqlite3_prepare_v2(db, "SELECT value FROM test", -1, &stmt, NULL);
    sqlite3_step(stmt);
    
    const char *result = (const char*)sqlite3_column_text(stmt, 0);
    if (result && strcmp(result, "hello") == 0) {
        printf("  [PASS] SELECT вернул 'hello'\n");
        testsPassed++;
    } else {
        printf("  [FAIL] результат: %s\n", result ? result : "NULL");
        testsFailed++;
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void testPreparedStatements() {
    printf("Тест: prepared statements с параметрами\n");
    
    sqlite3 *db;
    sqlite3_open(":memory:", &db);
    
    sqlite3_exec(db, "CREATE TABLE test (a INTEGER, b INTEGER)", NULL, NULL, NULL);
    
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "INSERT INTO test (a, b) VALUES (?, ?)", -1, &stmt, NULL);
    
    sqlite3_bind_int(stmt, 1, 10);
    sqlite3_bind_int(stmt, 2, 20);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    sqlite3_prepare_v2(db, "SELECT a + b FROM test", -1, &stmt, NULL);
    sqlite3_step(stmt);
    
    int sum = sqlite3_column_int(stmt, 0);
    if (sum == 30) {
        printf("  [PASS] сумма = %d\n", sum);
        testsPassed++;
    } else {
        printf("  [FAIL] сумма = %d\n", sum);
        testsFailed++;
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

int main() {
    printf("=== Тесты SQLite ===\n\n");
    
    testOpenDatabase();
    testCreateTable();
    testInsertAndSelect();
    testPreparedStatements();
    
    printf("\n=== Результаты ===\n");
    printf("Пройдено: %d\n", testsPassed);
    printf("Провалено: %d\n", testsFailed);
    
    return testsFailed > 0 ? 1 : 0;
}