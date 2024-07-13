#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

struct List;
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef  int (*CompareDataFp)(const void*, const void*);

typedef struct ListElement {
    struct ListElement *next;
    void *data;
} ListElement;

typedef struct {
    ListElement *head;
    ListElement *tail;
    ConstDataFp dump_data;
    DataFp free_data;
    CompareDataFp compare_data;
    DataFp modify_data;
} List;

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if(ptr) return ptr;
    printf("malloc error\n");
    exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string) {
    void *ptr = strdup(string);
    if(ptr) return ptr;
    printf("strdup error\n");
    exit(EXIT_FAILURE);
}

// --- generic functions --- for any data type

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data, CompareDataFp compare_data, DataFp modify_data)
{
    p_list->head = NULL;
    p_list->tail = NULL;
    p_list->dump_data = dump_data;
    p_list->free_data = free_data;
    p_list->compare_data = compare_data;
    p_list->modify_data = modify_data;
}

// Print elements of the list
void dump_list(const List* p_list)
{
    if(p_list->head == NULL)
        return;

    ListElement *iter = p_list->head;
    while(iter != NULL)
    {
        p_list->dump_data(iter->data);
        iter = iter->next;
    }
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data)
{
    if(p_list->head == NULL || data == NULL)
        return;

    ListElement *iter = p_list->head;
    while(iter != NULL)
    {
        if(p_list->compare_data(iter->data, data) == 0)
            p_list->dump_data(iter->data);

        iter = iter->next;
    }
}

// Free element pointed by data using free_data() function
void free_element(DataFp free_data, ListElement *to_delete)
{
    free_data(to_delete);
}

// Free all elements of the list
void free_list(List* p_list)
{
    ListElement *iter = p_list->head;
    while(iter != NULL)
    {
        ListElement *temp = iter;
        iter = iter->next;
        p_list->free_data(temp->data);
        free(temp);
    }
    p_list->head = NULL;
    p_list->tail = NULL;
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data)
{
    ListElement *temp = malloc(sizeof(ListElement));
    temp->data = data;
    temp->next = p_list->head;

    if(p_list->head == NULL)
        p_list->tail = temp;

    p_list->head = temp;
}

// Push element at the end of the list
void push_back(List *p_list, void *data)
{
    ListElement *temp = malloc(sizeof(ListElement));

    temp->data = data;
    temp->next = NULL;

    if(p_list->head == NULL)
        p_list->head = temp;
    else
        p_list->tail->next = temp;

    p_list->tail = temp;
}

// Remove the first element
void pop_front(List *p_list)
{
    if(p_list == NULL || p_list->head == NULL)
        return;

    void *temp = p_list->head->next;

    p_list->free_data(p_list->head->data);
    free(p_list->head);

    p_list->head = temp;
}

// Reverse the list
void reverse(List *p_list)
{
    if(p_list->head == NULL)
        return;

    ListElement *prev = NULL;
    ListElement *current = p_list->head;

    while(current != NULL)
    {
        void *temp = current->next;
        current->next = prev;
        prev = current;
        current = temp;
    }

    void *swapper = p_list->head;
    p_list->head = p_list->tail;
    p_list->tail = swapper;
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element)
{
    if(p_list->head == NULL || p_element == NULL)
        return NULL;

    ListElement *insertPos = NULL;
    ListElement *iter = p_list->head;
    while(iter != NULL)
    {
        if(p_list->compare_data(iter->data, p_element->data) <= 0)
            insertPos = iter;
        iter = iter->next;
    }

    return insertPos;
}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous)
{
    if(p_list->head == NULL || previous == NULL)
        return;

    ListElement *temp = malloc(sizeof(ListElement));
    temp->data = data;
    temp->next = previous->next;
    previous->next = temp;
}

// Insert element preserving order
void insert_in_order(List *p_list, void *p_data)
{
    if(p_list == NULL || p_data == NULL)
        return;
    ListElement *toInsert = malloc(sizeof(ListElement));
    toInsert->data = p_data;
    toInsert->next = NULL;

    ListElement *insertPos = find_insertion_point(p_list, toInsert);

    if(insertPos == NULL)
    {
        push_front(p_list, p_data);
        free(toInsert);
    }
    else
    {
        if(p_list->compare_data(insertPos->data, p_data) == 0)
        {
            if(p_list->modify_data != NULL)
                p_list->modify_data(insertPos->data);
            p_list->free_data(p_data);
            free(toInsert);
        }
        else
        {
            if(p_list->tail == insertPos)
                p_list->tail = toInsert;

            toInsert->next = insertPos->next;
            insertPos->next = toInsert;
        }
    }
}

// -----------------------------------------------------------
// --- type-specific definitions

// int element

void dump_int(const void *d)
{
    printf("%d ", *(int *)d);
}

void free_int(void *d)
{
    free((int *)d);
}

int cmp_int(const void *a, const void *b)
{
    int A = *(int *)a;
    int B = *(int *)b;

    return A-B;
}

int *create_data_int(int v)
{
    int *data = malloc(sizeof(int));
    *data = v;

    return data;
}

// Word element

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void dump_word (const void *d)
{
    DataWord *dw = (DataWord *)d;
    printf("%s ", dw->word);
}

void free_word(void *d)
{
    DataWord *dw = (DataWord *)d;

    free(dw->word);
    free(dw);
}

int cmp_word_alphabet(const void *a, const void *b)
{
    DataWord *A = (DataWord *)a;
    DataWord *B = (DataWord *)b;

    return strcmp(A->word, B->word);
}

int cmp_word_counter(const void *a, const void *b)
{
    DataWord *A = (DataWord *)a;
    DataWord *B = (DataWord *)b;

    return A->counter - B->counter;
}

void modify_word(void *p)
{
    DataWord *dw = (DataWord *)p;
    dw->counter++;
}

void *create_data_word(const char *string, int counter, int lowerCase) {
    DataWord *dw = malloc(sizeof(DataWord));

    dw->counter = counter;
    dw->word = strdup(string);

    if (lowerCase)
    {
        int i = strlen(string);

        while (i >= 0)
        {
            dw->word[i] = (char) tolower(string[i]);
            i--;
        }
    }

    return dw;
}

// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp, int lowerCase) {
    const char delimits[] = " \r\t\n.,?!:;-";
    char *token, *string = malloc(sizeof(char)*BUFFER_SIZE);

    if(cmp)
        p_list->compare_data = cmp;

    while(fgets(string, BUFFER_SIZE, stream))
    {
        token = strtok(string, delimits);

        while(token)
        {
            void *toAdd = create_data_word(token, 1, lowerCase);

            if(cmp)
                insert_in_order(p_list, toAdd);
            else
                push_back(p_list, toAdd);

            token = strtok(NULL, delimits);
        }
    }
    free(string);
}

// test integer list
void list_test(List *p_list, int n) {
    char op;
    int v;
    for (int i = 0; i < n; ++i) {
        scanf(" %c", &op);
        switch (op) {
            case 'f':
                scanf("%d", &v);
                push_front(p_list, create_data_int(v));
                break;
            case 'b':
                scanf("%d", &v);
                push_back(p_list, create_data_int(v));
                break;
            case 'd':
                pop_front(p_list);
                break;
            case 'r':
                reverse(p_list);
                break;
            case 'i':
                scanf("%d", &v);
                insert_in_order(p_list, create_data_int(v));
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
    }
}

int main(void) {
    int to_do, n;
    List list;

    scanf ("%d", &to_do);
    switch (to_do) {
        case 1: // test integer list
            scanf("%d",&n);
            init_list(&list, dump_int, free_int, cmp_int, NULL);
            list_test(&list, n);
            dump_list(&list);
            free_list(&list);
            break;
        case 2: // read words from text, insert into list, and print
            init_list(&list, dump_word, free_word, NULL, NULL);
            stream_to_list(&list, stdin, NULL, 0);
            dump_list(&list);
            free_list(&list);
            break;
        case 3: // read words, insert into list alphabetically, print words encountered n times
            scanf("%d",&n);
            init_list(&list, dump_word, free_word, NULL, modify_word);
            stream_to_list(&list, stdin, cmp_word_alphabet, 1);
            list.compare_data = cmp_word_counter;
            DataWord data = { NULL, n };
            dump_list_if(&list, &data);
            free_list(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}