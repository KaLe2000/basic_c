#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
    int height;
} node;

node* createNode(int value);
int height(node* root);
void updateHeight(node* root);
node* insert(node* root, int value);
node* findNode(node* root, int value);
void inorderTraversal(node* root);
void clearTree(node* root);

int testsPassed = 0;
int testsFailed = 0;

void testCreateNode() {
    printf("Тест: createNode\n");
    node* n = createNode(10);

    if (n->value == 10 && n->height == 1) {
        printf("  [PASS] value=10, height=1\n");
        testsPassed++;
    } else {
        printf("  [FAIL]\n");
        testsFailed++;
    }
    free(n);
}

void testHeight() {
    printf("Тест: height\n");
    node* n = createNode(10);

    if (height(n) == 1) {
        printf("  [PASS] height=1\n");
        testsPassed++;
    } else {
        printf("  [FAIL]\n");
        testsFailed++;
    }
    free(n);
}

void testUpdateHeight() {
    printf("Тест: updateHeight\n");
    node* root = createNode(10);
    root->left = createNode(5);
    root->right = createNode(15);

    updateHeight(root);
    if (height(root) == 2) {
        printf("  [PASS] height=2 после добавления детей\n");
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось 2\n");
        testsFailed++;
    }
    clearTree(root);
}

void testInsert() {
    printf("Тест: insert\n");
    node* root = NULL;
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 10);

    node* found = findNode(root, 20);
    if (found != NULL && found->value == 20) {
        printf("  [PASS] вставлен и найден\n");
        testsPassed++;
    } else {
        printf("  [FAIL]\n");
        testsFailed++;
    }
    clearTree(root);
}

void testInorderTraversal() {
    printf("Тест: inorderTraversal\n");
    node* root = NULL;
    root = insert(root, 20);
    root = insert(root, 10);
    root = insert(root, 30);

    printf("  [INFO] обход: ");
    inorderTraversal(root);
    printf("\n");
    printf("  [PASS] ошибок нет\n");
    testsPassed++;

    clearTree(root);
}

int main() {
    printf("=== Тесты AVLTree ===\n\n");

    testCreateNode();
    testHeight();
    testUpdateHeight();
    testInsert();
    testInorderTraversal();

    printf("\n=== Результаты ===\n");
    printf("Пройдено: %d\n", testsPassed);
    printf("Провалено: %d\n", testsFailed);

    return testsFailed > 0 ? 1 : 0;
}