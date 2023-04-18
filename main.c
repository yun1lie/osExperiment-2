#include <stdio.h>
#include <stdlib.h>

// 双向链表节点结构体
typedef struct ListNode {
    int val;
    struct ListNode *prev;
    struct ListNode *next;
} ListNode;

// 创建新节点
ListNode* createNode(int val) {
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    node->val = val;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

// 在链表尾部插入节点
void insert(ListNode** head, int val) {
    ListNode *node = createNode(val);
    if (*head == NULL) {
        *head = node;
    } else {
        ListNode *cur = *head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = node;
        node->prev = cur;
    }
}

// 删除指定节点
void deleteNode(ListNode **head, ListNode *node) {
    if (*head == NULL || node == NULL) {
        return;
    }
    if (*head == node) {
        *head = (*head)->next;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    free(node);
}

// 打印链表
void printList(ListNode *head) {
    ListNode *cur = head;
    while (cur != NULL) {
        printf("%d ", cur->val);
        cur = cur->next;
    }
    printf("\n");
}

int main() {
    ListNode *head = NULL;
    insert(&head, 1);
    insert(&head, 2);
    insert(&head, 3);
    printList(head);  // 1 2 3
    deleteNode(&head, head->next);
    printList(head);  // 1 3
    return 0;
}