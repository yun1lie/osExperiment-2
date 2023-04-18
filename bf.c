#include <stdio.h>
#include <stdlib.h>

#define MAX_MEM_SIZE 1000

typedef struct FreeBlock
{
    int start;
    int size;
    struct FreeBlock *next;
} FreeBlock;

FreeBlock *free_list = NULL;

// 初始化空闲块链表
void init_free_list(int start, int size)
{
    FreeBlock *fb = malloc(sizeof(FreeBlock));
    fb->start = start;
    fb->size = size;
    fb->next = NULL;
    free_list = fb;
}

// 插入空闲块
void insert_free_block(FreeBlock *fb)
{
    FreeBlock *cur = free_list, *prev = NULL;
    while (cur != NULL && cur->start < fb->start)
    {
        prev = cur;
        cur = cur->next;
    }
    if (prev == NULL)
    {
        fb->next = free_list;
        free_list = fb;
    }
    else
    {
        prev->next = fb;
        fb->next = cur;
    }
}

// 删除空闲块
void delete_free_block(FreeBlock *fb)
{
    FreeBlock *cur = free_list, *prev = NULL;
    while (cur != NULL && cur != fb)
    {
        prev = cur;
        cur = cur->next;
    }
    if (cur != NULL)
    {
        if (prev == NULL)
        {
            free_list = fb->next;
        }
        else
        {
            prev->next = fb->next;
        }
        free(fb);
    }
}

// 查找最佳适应块
FreeBlock *find_best_fit(int size)
{
    FreeBlock *cur = free_list, *best_fit = NULL;
    while (cur != NULL)
    {
        if (cur->size >= size)
        {
            if (best_fit == NULL || cur->size < best_fit->size)
            {
                best_fit = cur;
            }
        }
        cur = cur->next;
    }
    return best_fit;
}

// 分配内存
int allocate_memory(int size)
{
    FreeBlock *best_fit = find_best_fit(size);
    if (best_fit == NULL)
    {
        return -1;
    }
    int start = best_fit->start;
    int end = best_fit->start + size;
    best_fit->start = end;
    best_fit->size -= size;
    if (best_fit->size == 0)
    {
        delete_free_block(best_fit);
    }
    return start;
}

// 释放内存
void free_memory(int start, int size)
{
    FreeBlock *fb = malloc(sizeof(FreeBlock));
    fb->start = start;
    fb->size = size;
    insert_free_block(fb);
    // 合并相邻的空闲块
    FreeBlock *cur = free_list, *prev = NULL;
    while (cur != NULL)
    {
        if (prev != NULL && cur->start == prev->start + prev->size)
        {
            prev->size += cur->size;
            prev->next = cur->next;
            free(cur);
            cur = prev->next;
        }
        else
        {
            prev = cur;
            cur = cur->next;
        }
    }
}

int main()
{
    init_free_list(0, MAX_MEM_SIZE);

    // 打印空闲块列表
    FreeBlock *cur = free_list;
    printf("Free blocks:\n");
    while (cur != NULL)
    {
        printf("start = %d, size = %d\n", cur->start, cur->size);
        cur = cur->next;
    }

    // 分配4个大小为100的块
    int i;
    for (i = 0; i < 4; i++)
    {
        int start = allocate_memory(100);
        printf("Allocated block: start = %d, size = %d\n", start, 100);
    }

    // 打印空闲块列表
    cur = free_list;
    printf("Free blocks:\n");
    while (cur != NULL)
    {
        printf("start = %d, size = %d\n", cur->start, cur->size);
        cur = cur->next;
    }

    // 释放第2个块
    free_memory(100, 100);

    // 打印空闲块列表
    cur = free_list;
    printf("Free blocks:\n");
    while (cur != NULL)
    {
        printf("start = %d, size = %d\n", cur->start, cur->size);
        cur = cur->next;
    }

    return 0;
}
