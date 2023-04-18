#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FREE_BLOCK_SIZE sizeof(FreeBlock)
#define ALLOC_BLOCK_SIZE sizeof(AllocBlock)
#define JOB_SIZE sizeof(Job)

typedef struct FreeBlock
{
    int size;
    int addr;
    struct FreeBlock *next;
} FreeBlock;

typedef struct AllocBlock
{
    int size;
    int addr;
} AllocBlock;

typedef struct Job
{
    char name[20];
    int size;
} Job;

const int MAX_FREE_BLOCKS = 1000;
const int MAX_ALLOC_BLOCKS = 1000;
const int MAX_JOBS = 1000;

void init_free_table(FreeBlock **head, int size)
{
    *head = malloc(FREE_BLOCK_SIZE);
    (*head)->size = size;
    (*head)->addr = 0;
    (*head)->next = NULL;
}

void insert_free_block(FreeBlock **head, int size, int addr)
{
    FreeBlock *block = malloc(FREE_BLOCK_SIZE);
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

void delete_free_block(FreeBlock **head, FreeBlock *block)
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

int allocate_memory(FreeBlock **head, int size)
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
                delete_free_block(head, cur);
            }
            return addr;
        }
        cur = cur->next;
    }
    return -1;
}

void free_memory(FreeBlock **head, int addr, int size)
{
    insert_free_block(head, size, addr);
}

void print_free_table(FreeBlock *head)
{
    printf("Free Block Table:\n");
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

void init_alloc_table(AllocBlock *table, int size)
{
    memset(table, -1, size * ALLOC_BLOCK_SIZE);
}

int find_free_alloc_block(AllocBlock *table, int size)
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

void insert_alloc_block(AllocBlock *table, int size, int addr, int index)
{
    table[index].size = size;
    table[index].addr = addr;
}

void delete_alloc_block(AllocBlock *table, int index)
{
    table[index].size = -1;
    table[index].addr = -1;
}

void print_alloc_table(AllocBlock *table, int size)
{
    printf("Allocated Block Table:\n");
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

void init_job_queue(Job *queue, int size)
{
    memset(queue, 0, size * JOB_SIZE);
}

int find_free_job_queue(Job *queue, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (strlen(queue[i].name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void insert_job(Job *queue, int size, char *name, int job_size, int index)
{
    strncpy(queue[index].name, name, 20);
    queue[index].size = job_size;
}

void delete_job(Job *queue, int index)
{
    memset(&queue[index], 0, JOB_SIZE);
}

void print_job_queue(Job *queue, int size)
{
    printf("Job Queue:\n");
    printf("-----------------------\n");
    printf("| index | name     | size |\n");
    printf("-----------------------\n");
    int i;
    for (i = 0; i < size; i++)
    {
        if (strlen(queue[i].name) != 0)
        {
            printf("| %5d | %8s | %4d |\n", i, queue[i].name, queue[i].size);
        }
    }
    printf("-----------------------\n");
}

int main()
{
    FreeBlock *free_head = NULL;
    AllocBlock alloc_table[MAX_ALLOC_BLOCKS];
    Job job_queue[MAX_JOBS];
    int i;

    init_free_table(&free_head, 1000);
    init_alloc_table(alloc_table, MAX_ALLOC_BLOCKS);
    init_job_queue(job_queue, MAX_JOBS);

    printf("Memory Allocation Simulation\n\n");

    insert_free_block(&free_head, 500, 2000);
    insert_free_block(&free_head, 300, 4000);
    insert_free_block(&free_head, 200, 7000);
    insert_free_block(&free_head, 150, 9000);

    print_free_table(free_head);
    print_alloc_table(alloc_table, MAX_ALLOC_BLOCKS);
    print_job_queue(job_queue, MAX_JOBS);

    int addr1 = allocate_memory(&free_head, 400);
    printf("allocate_memory(&free_head, 400) returns: %d\n", addr1);
    if (addr1 != -1)
    {
        insert_alloc_block(alloc_table, 400, addr1, find_free_alloc_block(alloc_table, MAX_ALLOC_BLOCKS));
    }

    int addr2 = allocate_memory(&free_head, 200);
    printf("allocate_memory(&free_head, 200) returns: %d\n", addr2);
    if (addr2 != -1)
    {
        insert_alloc_block(alloc_table, 200, addr2, find_free_alloc_block(alloc_table, MAX_ALLOC_BLOCKS));
    }

    int addr3 = allocate_memory(&free_head, 1000);
    printf("allocate_memory(&free_head, 1000) returns: %d\n", addr3);
    if (addr3 != -1)
    {
        insert_alloc_block(alloc_table, 1000, addr3, find_free_alloc_block(alloc_table, MAX_ALLOC_BLOCKS));
    }

    print_free_table(free_head);
    print_alloc_table(alloc_table, MAX_ALLOC_BLOCKS);

    insert_job(job_queue, MAX_JOBS, "job1", 300, find_free_job_queue(job_queue, MAX_JOBS));
    insert_job(job_queue, MAX_JOBS, "job2", 800, find_free_job_queue(job_queue, MAX_JOBS));

    print_job_queue(job_queue, MAX_JOBS);

    delete_job(job_queue, 1);

    printf("After deleting job 1:\n");
    print_job_queue(job_queue, MAX_JOBS);

    free_memory(&free_head, addr1, 400);
    delete_alloc_block(alloc_table, 0);

    printf("After freeing memory and deleting alloc block:\n");
    print_free_table(free_head);
    print_alloc_table(alloc_table, MAX_ALLOC_BLOCKS);

    return 0;
}
