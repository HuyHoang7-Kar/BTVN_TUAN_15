#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Định nghĩa cấu trúc Node
typedef struct Node {
    char title[100];       // Ten cua muc
    int page;              // So trang
    struct Node *left;     // Con tro toi muc con dau tien
    struct Node *next;     // Con tro toi anh em tiep theo
} Node;

// Định nghĩa cấu trúc Book
typedef struct Book {
    Node *root;            // Goc cua cay
} Book;

// Tao mot node moi
Node *createNode(const char *title, int page) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (!newNode) {
        printf("Cap phat bo nho that bai\n");
        exit(1);
    }
    strcpy(newNode->title, title);
    newNode->page = page;
    newNode->left = NULL;
    newNode->next = NULL;
    return newNode;
}
// Cap nhat tong so trang cua sach bang tong cua tat ca cac chuong
void TotalPages(Book *b) {
    int totalPages = 0;
    Node *current = b->root->left;
    while (current) {
        totalPages += current->page;
        current = current->next;
    }
    b->root->page = totalPages;
}

// Dem so chuong (con truc tiep cua root)
int countChapters(Book *b) {
    int cnt = 0;
    Node *current = b->root->left;
    while (current) {
        cnt++;
        current = current->next;
    }
    return cnt;
}

// Tim chuong dai nhat
Node *findLongestChapter(Book *b) {
    Node *current = b->root->left;
    Node *longest = NULL;
    int maxPages = 0;

    while (current) {
        if (current->page > maxPages) {
            maxPages = current->page;
            longest = current;
        }
        current = current->next;
    }
    return longest;
}

// Giai phong bo nho cua toan bo cay
void freeTree(Node *node) {
    if (!node) return;
    freeTree(node->left);  // Giai phong cac muc con
    freeTree(node->next);  // Giai phong cac anh em
    free(node);            // Giai phong node hien tai
}

// Tim va xoa mot muc theo ten
int removeNode(Node *parent, const char *target) {
    Node *current = parent->left;
    Node *prev = NULL;

    while (current) {
        if (strcmp(current->title, target) == 0) {
            // Cap nhat lien ket
            if (prev) {
                prev->next = current->next;
            } else {
                parent->left = current->next;
            }

            // Tru so trang cua muc xoa khoi muc cha
            parent->page -= current->page;

            // Giai phong cay con
            freeTree(current);
            return 1;
        }
        if (removeNode(current, target)) {
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 0;
}

// Giao dien xoa mot muc trong sach
int removeSection(Book *b, const char *target) {
    return removeNode(b->root, target);
}

// Hien thi cuon sach
void display(Node *node, int depth) {
    if (!node) return;
    printf("%*s%s (%d trang)\n", depth * 2, "", node->title, node->page);
    display(node->left, depth + 1);  // Hien thi cac muc con
    display(node->next, depth);     // Hien thi cac anh em
}

int main() {
    // Tao cuon sach
    Book book;
    book.root = createNode("My Book", 0);

    // Them cac chuong va muc con
    Node *chapter1 = createNode("Chuong 1", 80);
    chapter1->left = createNode("Muc 1.1", 20);
    chapter1->left->next = createNode("Muc 1.2", 50);

    Node *chapter2 = createNode("Chuong 2", 70);
    chapter2->left = createNode("Muc 2.1", 40);
    chapter2->left->next = createNode("Muc 2.2", 30);

    // Ket noi cac chuong vao sach
    book.root->left = chapter1;
    chapter1->next = chapter2;
    TotalPages(&book);
    // Hien thi cuon sach
    printf("Sach ban dau:\n");
    display(book.root, 0);

    // So chuong cua sach
    printf("\nSo chuong: %d\n", countChapters(&book));

    // Chuong dai nhat
    Node *longest = findLongestChapter(&book);
    if (longest) {
        printf("Chuong dai nhat: %s (%d trang)\n", longest->title, longest->page);
    }

    // Tim va xoa mot muc
    removeSection(&book, "Muc 1.2");
    TotalPages(&book);
    printf("\nSau khi xoa Muc 1.2:\n");
    
    display(book.root, 0);

    // Chuong dai nhat sau khi xoa
    longest = findLongestChapter(&book);
    if (longest) {
        printf("Chuong dai nhat: %s (%d trang)\n", longest->title, longest->page);
    }

    // Giai phong bo nho
    freeTree(book.root);

    return 0;
}
