#include <stdio.h>
#include <stdlib.h>

typedef struct Block {
    int id;
    int start;
    int end;
    struct Block *next;
} Block;

void print_menu();
void enter_parameters(int *pm_size);
Block *allocate_memory_first_fit(Block *head, int id, int size, int pm_size);
Block *allocate_memory_best_fit(Block *head, int id, int size, int pm_size);
Block *deallocate_memory(Block *head, int id);
void defragment_memory(Block *head);
void print_allocated_blocks(Block *head);
void free_memory(Block *head);

int main() {
    int pm_size = 0;
    Block *head = NULL;
    int choice, id, size;

    while (1) {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                enter_parameters(&pm_size);
                break;
            case 2:
                printf("Enter block id: ");
                scanf("%d", &id);
                printf("Enter block size: ");
                scanf("%d", &size);
                head = allocate_memory_first_fit(head, id, size, pm_size);
                print_allocated_blocks(head);
                break;
            case 3:
                printf("Enter block id: ");
                scanf("%d", &id);
                printf("Enter block size: ");
                scanf("%d", &size);
                head = allocate_memory_best_fit(head, id, size, pm_size);
                print_allocated_blocks(head);
                break;
            case 4:
                printf("Enter block id: ");
                scanf("%d", &id);
                head = deallocate_memory(head, id);
                print_allocated_blocks(head);
                break;
            case 5:
                defragment_memory(head);
                print_allocated_blocks(head);
                break;
            case 6:
                free_memory(head);
                printf("Quitting program...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}

void print_menu() {
    printf("\nHole-fitting Algorithms\n");
    printf("-----------------------\n");
    printf("1) Enter parameters\n");
    printf("2) Allocate memory for block using First-fit\n");
    printf("3) Allocate memory for block using Best-fit\n");
    printf("4) Deallocate memory for block\n");
    printf("5) Defragment memory\n");
    printf("6) Quit program\n");
    printf("Enter selection: ");
}

void enter_parameters(int *pm_size) {
    printf("Enter size of physical memory: ");
    scanf("%d", pm_size);
}

Block *allocate_memory_first_fit(Block *head, int id, int size, int pm_size) {
    // Check if id already exists
    for (Block *ptr = head; ptr != NULL; ptr = ptr->next) {
        if (ptr->id == id) {
            printf("Block with id %d already exists.\n", id);
            return head;
        }
    }

    int start = 0;
    Block *prev = NULL;
    Block *current = head;
    while (current != NULL && current->start - start < size) {
        start = current->end;
        prev = current;
        current = current->next;
    }

    if (pm_size - start >= size) {
        Block *new_block = (Block *) malloc(sizeof(Block));
        new_block->id = id;
        new_block->start = start;
        new_block->end = start + size;
        new_block->next = current;

        if (prev == NULL) {
                        head = new_block;
        } else {
            prev->next = new_block;
        }
    } else {
        printf("Not enough space for block id %d.\n", id);
    }

    return head;
}

Block *allocate_memory_best_fit(Block *head, int id, int size, int pm_size) {
    // Check if id already exists
    for (Block *ptr = head; ptr != NULL; ptr = ptr->next) {
        if (ptr->id == id) {
            printf("Block with id %d already exists.\n", id);
            return head;
        }
    }

    int start = 0;
    int min_diff = pm_size + 1;
    Block *best_prev = NULL;
    Block *best_current = NULL;
    Block *prev = NULL;
    Block *current = head;

    while (current != NULL || start < pm_size) {
        int diff = (current != NULL) ? current->start - start : pm_size - start;

        if (diff >= size && diff < min_diff) {
            min_diff = diff;
            best_prev = prev;
            best_current = current;
        }

        if (current == NULL) {
            break;
        }

        start = current->end;
        prev = current;
        current = current->next;
    }

    if (min_diff != pm_size + 1) {
        Block *new_block = (Block *) malloc(sizeof(Block));
        new_block->id = id;
        new_block->start = best_prev ? best_prev->end : 0;
        new_block->end = new_block->start + size;
        new_block->next = best_current;

        if (best_prev == NULL) {
            head = new_block;
        } else {
            best_prev->next = new_block;
        }
    } else {
        printf("Not enough space for block id %d.\n", id);
    }

    return head;
}

Block *deallocate_memory(Block *head, int id) {
    Block *prev = NULL;
    Block *current = head;

    while (current != NULL && current->id != id) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Block with id %d not found.\n", id);
        return head;
    }

    if (prev == NULL) {
        head = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
    return head;
}

void defragment_memory(Block *head) {
    int start = 0;
    for (Block *ptr = head; ptr != NULL; ptr = ptr->next) {
        int diff = ptr->end - ptr->start;
        ptr->start = start;
        ptr->end = start + diff;
        start = ptr->end;
    }
}

void print_allocated_blocks(Block *head) {
    printf("ID Start End\n");
    printf("-------------------\n");
    for (Block *ptr = head; ptr != NULL; ptr = ptr->next) {
        printf("%d %d %d\n", ptr->id, ptr->start, ptr->end);
    }
}

void free_memory(Block *head) {
    Block *ptr = head;

    while (ptr != NULL) {
        Block *next = ptr->next;
        free(ptr);
        ptr = next;
    }
}
