#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <time.h>
typedef struct __node {
    struct list_head list;
    long value;
} node_t;

void list_add__(struct list_head *head, struct list_head *node)
{
    list_add(node, head);
}

struct list_head *list_tail(struct list_head *head)
{
    return head->prev;
}

int list_length(struct list_head *head)
{
    int n = 0;
    struct list_head *tmp;
    list_for_each(tmp, head){
        ++n;
    }
    return n;
}

void node_construct(node_t *head, int n)
{   
    if(!head)
        return;
    node_t *node = malloc(sizeof(node_t));
    INIT_LIST_HEAD(&node->list);
    node->value = n;
    list_add__(&head->list, &node->list);
}

void node_free(node_t *head)
{
    if(!head)
        return;
    node_t *entry;
    node_t *safe;
    list_for_each_entry_safe(entry, safe, &head->list, list){
        list_del(&entry->list);
        free(entry);
    }
}
bool cmp(struct list_head *a,int value)
{
    return list_entry(a, node_t, list)->value > value;
}
void quick_sort(node_t *head) {
    clock_t time = clock();
    if (!head || list_empty(&head->list))
        return;
    // int n = list_length(&head->list);
    // int max_level = n;
    // max_level=100;
    int i = 0;
    int level_count = 0;
    // struct list_head *begin[max_level], *end[max_level];
    struct list_head **begin = malloc(sizeof(struct list_head*)), **end = malloc(sizeof(struct list_head*));

    struct list_head *result = malloc(sizeof(struct list_head));
    struct list_head *left = malloc(sizeof(struct list_head)); 
    struct list_head *right = malloc(sizeof(struct list_head));
    struct list_head *ppviot = malloc(sizeof(struct list_head));


    INIT_LIST_HEAD(result);

    INIT_LIST_HEAD(*begin);
        
    INIT_LIST_HEAD(*end);    

    struct list_head *next = head->list.next, *prev = head->list.prev;
    list_add__(begin,next);
    list_add__(end, prev);
 
    // begin[0] = head->list.next;
    // end[0] = head->list.prev;

    while (i >= 0) {
        struct list_head *L = list_tail(*begin), *R = list_tail(*end);
        level_count = level_count > i ? level_count : i;
        
        if (L != R) {

            struct list_head *pivot = L;
            int value = list_entry(pivot, node_t, list)->value;
            // printf("value: %d\n",value);
            struct list_head *p = pivot->next;

            list_del(pivot);
            INIT_LIST_HEAD(ppviot);


            list_add__(ppviot, pivot);

            // exit(1);
            struct list_head *stop = p->prev;
            INIT_LIST_HEAD(left);
            INIT_LIST_HEAD(right);
            while (p != stop) {
                struct list_head *n = p;
                p = p->next;
                list_del(n);
                list_add__(cmp(n, value) ? right : left, n);
            }
            if(!list_empty(left)) {
                struct list_head *n = left->next;
                struct list_head *p = left->prev;
                list_add_tail(*begin, n);
                list_add_tail(*end, p);
            }

            list_add_tail(begin, ppviot->next);
            list_add_tail(end, list_tail(ppviot));

            if(!list_empty(right)) {
                struct list_head *n = right->next;
                struct list_head *p = right->prev;
                list_add_tail(begin, n);
                list_add_tail(end, p);
            }    

 
    
            // begin[i] = list_empty(left) ? NULL : left->next;
            // end[i] = list_empty(left) ? NULL : list_tail(left);
            // begin[i + 1] = ppviot->next;
            // end[i + 1] = list_tail(ppviot);
            // begin[i + 2] = list_empty(right) ? NULL : right->next;
            // end[i + 2] = list_empty(right) ? NULL : list_tail(right);
            i += 2;
        } else {
            printf("1");
            if (L) 
                list_add__(result, L);
            list_del_init(begin->prev);
            list_del_init(end->prev);
             

            // free()
            i--;
        }
    }
    list_add__(result, &head->list);
    list_del(result);
    free(left);
    free(right);
    free(result);
    free(ppviot);
    time = clock() - time;
    printf("max level : %d\t cpu clock time : %ld\n",level_count,time);
}


/* Verify if list is order */
static bool list_is_ordered(node_t *head)
{    
    if(!head)
        return false;   
    bool first = true;
    int value;
    struct list_head *tmp;
    INIT_LIST_HEAD(tmp);
    list_for_each_entry(head, tmp, list){
        if (first) {
                value = head->value;
                first = false;
            } else {
                if (head->value < value)
                    return false;
                value = head->value;
            }
    }
    return true;
}

/* shuffle array, only work if n < RAND_MAX */
// void print_sort(node_t *p)
// {
//     if(!p)
//         return;
//     node_t *entry;
//     list_for_each_entry(entry, &p->list, list){
//         printf("%ld\n",entry->value);
//     }
// }
void shuffle(int *array, size_t n)
{
    if (n <= 0)
        return;

    for (size_t i = 0; i < n - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

int main(int argc, char **argv)
{
    size_t count = 1000;

    int *test_arr = malloc(sizeof(int) * count);

    for (int i = 0; i < count; ++i)
        test_arr[i] = i;
    shuffle(test_arr, count);

    node_t *head = malloc(sizeof(node_t)*1);
    INIT_LIST_HEAD(&head->list);
    while (count--)
        node_construct(head, test_arr[count]);
 
    quick_sort(head);
    assert(list_is_ordered(head));

    node_free(head);
    free(test_arr);

    return 1;
}
