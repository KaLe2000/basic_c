#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} node;

node* createNode(int value);
void clearTree(node* root);
node* findNodeBSTree(node* root, int value);
int getCountLists(node* root);
void inorderTraversal(node* root);

int testsPassed = 0;
int testsFailed = 0;

void testCreateNode() {
    printf("Тест: createNode\n");
    node* n = createNode(10);

    if (n != NULL && n->value == 10 && n->left == NULL && n->right == NULL) {
        printf("  [PASS] node{value=10, left=NULL, right=NULL}\n");
        testsPassed++;
    } else {
        printf("  [FAIL] неверные значения\n");
        testsFailed++;
    }
    free(n);
}

void testFindNodeBSTree() {
    printf("Тест: findNodeBSTree\n");
    node* root = createNode(20);
    root->left = createNode(10);
    root->right = createNode(30);
    root->left->right = createNode(15);

    node* found = findNodeBSTree(root, 15);
    if (found != NULL && found->value == 15) {
        printf("  [PASS] найден node с value=15\n");
        testsPassed++;
    } else {
        printf("  [FAIL] node не найден\n");
        testsFailed++;
    }
    clearTree(root);
}

void testFindNodeNotFound() {
    printf("Тест: findNodeBSTree не найден\n");
    node* root = createNode(20);
    root->left = createNode(10);

    node* found = findNodeBSTree(root, 100);
    if (found == NULL) {
        printf("  [PASS] возвращает NULL\n");
        testsPassed++;
    } else {
        printf("  [FAIL] должен быть NULL\n");
        testsFailed++;
    }
    clearTree(root);
}

void testGetCountLists() {
    printf("Тест: getCountLists\n");
    node* root = createNode(10);
    root->left = createNode(5);
    root->right = createNode(15);
    root->left->left = createNode(3);
    root->left->right = createNode(7);
    root->right->left = createNode(12);
    root->right->right = createNode(17);

    int count = getCountLists(root);
    if (count == 4) {
        printf("  [PASS] листьев = 4\n");
        testsPassed++;
    } else {
        printf("  [FAIL] ожидалось 4\n");
        testsFailed++;
    }
    clearTree(root);
}

int main() {
    printf("=== Тесты BSTree ===\n\n");

    testCreateNode();
    testFindNodeBSTree();
    testFindNodeNotFound();
    testGetCountLists();

    printf("\n=== Результаты ===\n");
    printf("Пройдено: %d\n", testsPassed);
    printf("Провалено: %d\n", testsFailed);

    return testsFailed > 0 ? 1 : 0;
}