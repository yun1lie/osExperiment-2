#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义空闲块结构体
typedef struct FreeBlock
{
    int size;
    int addr;
    struct FreeBlock *next;
} FreeBlock;

// 初始化空闲块链表
void initFreeTable(FreeBlock **head, int size)
{
    *head = (FreeBlock *)malloc(sizeof(FreeBlock));
    (*head)->size = size;
    (*head)->addr = 0;
    (*head)->next = NULL;
}

// 插入空闲块到空闲块链表
void insertFreeBlock(FreeBlock **head, int size, int addr)
{
    FreeBlock *block = (FreeBlock *)malloc(sizeof(FreeBlock));
    block->size = size;
    block->addr = addr;
    block->next = NULL;
    if (*head == NULL)
    {
        *head = block;
    }
    else
    {
        FreeBlock *cur = *head;
        while (cur->next != NULL && cur->next->addr < addr)
        {
            cur = cur->next;
        }
        block->next = cur->next;
        cur->next = block;
    }
}

// 从空闲块链表中删除一个空闲块
void deleteFreeBlock(FreeBlock **head, FreeBlock *block)
{
    if (*head == block)
    {
        *head = (*head)->next;
    }
    else
    {
        FreeBlock *cur = *head;
        while (cur != NULL && cur->next != block)
        {
            cur = cur->next;
        }
        if (cur != NULL)
        {
            cur->next = block->next;
        }
    }
    free(block);
}

// 分配内存空间
int allocateMemory(FreeBlock **head, int size)
{
    if (*head == NULL)
    {
        return -1;
    }
    FreeBlock *cur = *head;
    while (cur != NULL)
    {
        if (cur->size >= size)
        {
            int addr = cur->addr;
            cur->size -= size;
            cur->addr += size;
            if (cur->size == 0)
            {
                deleteFreeBlock(head, cur);
            }
            return addr;
        }
        cur = cur->next;
    }
    return -1;
}

// 回收内存空间
void freeMemory(FreeBlock **head, int addr, int size)
{
    insertFreeBlock(head, size, addr);
}

// 打印空闲块链表
void printFreeTable(FreeBlock *head)
{
    printf("空闲区表：\n");
    printf("---------------------------\n");
    printf("| addr      | size      |\n");
    printf("---------------------------\n");
    FreeBlock *cur = head;
    while (cur != NULL)
    {
        printf("| %8d | %8d |\n", cur->addr, cur->size);
        cur = cur->next;
    }
    printf("---------------------------\n");
}

// 定义已分配块结构体
typedef struct AllocBlock
{
    int size;
    int addr;
} AllocBlock;

// 初始化已分配块数组
void initAllocTable(AllocBlock *table, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        table[i].size = -1;
        table[i].addr = -1;
    }
}

// 查找空闲的已分配块位置
int findFreeAllocBlock(AllocBlock *table, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (table[i].size == -1)
        {
            return i;
        }
    }
    return -1;
}

// 插入已分配块到已分配块数组
void insertAllocBlock(AllocBlock *table, int size, int addr, int index)
{
    table[index].size = size;
    table[index].addr = addr;
}

// 从已分配块数组中删除一个已分配块
void deleteAllocBlock(AllocBlock *table, int index)
{
    table[index].size = -1;
    table[index].addr = -1;
}

// 打印已分配块数组
void printAllocTable(AllocBlock *table, int size)
{
    printf("已分配区表：\n");
    printf("---------------------------\n");
    printf("| index | addr      | size  |\n");
    printf("---------------------------\n");
    int i;
    for (i = 0; i < size; i++)
    {
        if (table[i].size != -1)
        {
            printf("| %5d | %8d | %5d |\n", i, table[i].addr, table[i].size);
        }
    }
    printf("---------------------------\n");
}

// 定义作业结构体
typedef struct Job
{
    char name[20];
    int size;
} Job;

// 初始化作业队列
void initJobQueue(Job *queue, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        queue[i].name[0] = '\0';
        queue[i].size = -1;
    }
}

// 查找空闲的作业队列位置
int findFreeJobQueue(Job *queue, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (queue[i].size == -1)
        {
            return i;
        }
    }
    return -1;
}

// 插入作业到作业队列
void insertJob(Job *queue, int size, char *name, int jobSize, int index)
{
    strncpy(queue[index].name, name, 20);
    queue[index].size = jobSize;
}

// 从作业队列中删除一个作业
void deleteJob(Job *queue, int index)
{
    queue[index].name[0] = '\0';
    queue[index].size = -1;
}

// 打印作业队列
void printJobQueue(Job *queue, int size)
{
    printf("作业队列：\n");
    printf("-----------------------\n");
    printf("| index | name     | size |\n");
    printf("-----------------------\n");
    int i;
    for (i = 0; i < size; i++)
    {
        if (queue[i].size != -1)
        {
            printf("| %5d | %8s | %4d |\n", i, queue[i].name, queue[i].size);
        }
    }
    printf("-----------------------\n");
}

int main()
{
    // 初始化空闲块链表
    FreeBlock *head = NULL;
    initFreeTable(&head, 100);
    // 插入空闲块到空闲块链表中
    insertFreeBlock(&head, 30, 150);
    insertFreeBlock(&head, 50, 250);
    insertFreeBlock(&head, 20, 400);
    // 分配内存
    int addr1 = allocateMemory(&head, 40);
    int addr2 = allocateMemory(&head, 25);
    // 回收内存
    freeMemory(&head, addr1, 40);
    // 打印空闲块链表
    printFreeTable(head);

    // 初始化已分配块数组
    int size = 10;
    AllocBlock *table = (AllocBlock *)malloc(size * sizeof(AllocBlock));
    initAllocTable(table, size);
    // 插入已分配块到已分配块数组中
    int index1 = findFreeAllocBlock(table, size);
    insertAllocBlock(table, 30, 100, index1);
    int index2 = findFreeAllocBlock(table, size);
    insertAllocBlock(table, 50, 200, index2);
    // 删除已分配块
    deleteAllocBlock(table, index1);
    // 打印已分配块数组
    printAllocTable(table, size);



    // 初始化作业队列
    int size = 10;
    Job *queue = (Job*)malloc(size * sizeof(Job));
    initJobQueue(queue, size);
    // 插入作业到作业队列中
    int index1 = findFreeJobQueue(queue, size);
    insertJob(queue, size, "job1", 30, index1);
    int index2 = findFreeJobQueue(queue, size);
    insertJob(queue, size, "job2", 50, index2);
    // 删除作业
    deleteJob(queue, index1);
    // 打印作业队列
    printJobQueue(queue, size);
    return 0;
}
