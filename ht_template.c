#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  (-1)

typedef union {
    int int_data;
    char char_data;
    // ... other primitive types used
    void *ptr_data;
} data_union;

typedef struct ht_element {
    struct ht_element *next;
    data_union data;
} ht_element;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef int (*CompareDataFp)(data_union, data_union);
typedef size_t (*HashFp)(data_union, size_t);
typedef data_union (*CreateDataFp)(void*);

typedef struct {
    size_t size;
    size_t no_elements;
    ht_element *ht;
    DataFp dump_data;
    CreateDataFp create_data;
    DataFp free_data;
    CompareDataFp compare_data;
    HashFp hash_function;
    DataPFp modify_data;
} hash_table;

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, size_t size, DataFp dump_data, CreateDataFp create_data, DataFp free_data, CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data)
{
    if(p_table == NULL)
        return;

    p_table->size = size;
    p_table->ht = calloc(size, sizeof(ht_element));
    p_table->no_elements = 0;
    p_table->dump_data = dump_data;
    p_table->create_data = create_data;
    p_table->free_data = free_data;
    p_table->compare_data = compare_data;
    p_table->hash_function = hash_function;
    p_table->modify_data = modify_data;
    p_table->compare_data = compare_data;
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, size_t n)
{
    ht_element *head = p_table->ht[n].next;

    while(head != NULL)
    {
        p_table->dump_data(head->data);
        head = head->next;
    }
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete)
{
    if(free_data != NULL)
        free_data(to_delete->data);
    free(to_delete);
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table)
{
    for(size_t i = 0; i < p_table->size; i++)
    {
        ht_element *head = p_table->ht[i].next;
        while(head != NULL)
        {
            ht_element *temp = head;
            head = head->next;
            free_element(p_table->free_data, temp);
        }
    }
    free(p_table->ht);
}

// calculate hash function for integer k
size_t hash_base(int k, size_t size) {
    static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
    double tmp = k * c;
    return (size_t)floor((double)size * (tmp - floor(tmp)));
}

void rehash(hash_table *p_table)
{
    size_t newSize = p_table->size * 2;
    ht_element *newHt = calloc(newSize, sizeof(ht_element));

    for(size_t i = 0; i < p_table->size; i++)
    {
        ht_element *head = p_table->ht[i].next;

        while(head != NULL)
        {
            size_t newIndex = p_table->hash_function(head->data, newSize);

            ht_element *temp = malloc(sizeof(ht_element));
            temp->next = newHt[newIndex].next;
            temp->data = head->data;

            newHt[newIndex].next = temp;

            ht_element *toDelete = head;
            head = head->next;
            free(toDelete);
        }
    }

    free(p_table->ht);
    p_table->size = newSize;
    p_table->ht = newHt;
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data)
{
    size_t index = p_table->hash_function(*data, p_table->size);
    ht_element *head = p_table->ht[index].next;

    while(head != NULL)
    {
        if(p_table->compare_data(head->data, *data) == 0)
            return head;
        head = head->next;
    }

    return NULL;
}

// insert element
void insert_element(hash_table *p_table, data_union *data)
{
    ht_element *temp = malloc(sizeof(ht_element));
    temp->next = NULL;
    temp->data = *data;

    size_t index = p_table->hash_function(*data, p_table->size);
    ht_element *head = &p_table->ht[index];

    while(head->next != NULL && p_table->compare_data(head->next->data, *data) != 0)
        head = head->next;


    if(head->next != NULL)
    {
        if(p_table->modify_data != NULL)
            p_table->modify_data(&head->next->data);
    }
    else
    {
        temp->next = p_table->ht[index].next;
        p_table->ht[index].next = temp;
    }

    p_table->no_elements++;
    if(p_table->no_elements/p_table->size > MAX_RATE)
        rehash(p_table);
}

// remove element
void remove_element(hash_table *p_table, data_union data)
{
    size_t index = p_table->hash_function(data, p_table->size);

    ht_element *head = &p_table->ht[index];

    while(head->next != NULL)
    {
        if(p_table->compare_data(head->next->data, data) == 0)
        {
            ht_element *temp = head->next;
            head->next = temp->next;
            free_element(p_table->free_data, temp);
            p_table->no_elements--;
            break;
        }
        head = head->next;
    }
}

// type-specific definitions

// int element

size_t hash_int(data_union data, size_t size) {
    return hash_base(data.int_data, size);
}

void dump_int(data_union data)
{
    printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b)
{
    return a.int_data - b.int_data;
}

data_union create_int(void* value)
{
    int data = 0;
    scanf("%d", &data);

    return (data_union){.int_data = data};
}

// char element

size_t hash_char(data_union data, size_t size) {
    return hash_base((int)data.char_data, size);
}

void dump_char(data_union data)
{
    printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b)
{
    return a.char_data - b.char_data;
}

data_union create_char(void* value)
{
    char data;
    scanf(" %c", &data);

    return (data_union){.char_data = data};
}

// Word element

typedef struct DataWord {
    char *word;
    int counter;
} DataWord;

void dump_word(data_union data)
{
    DataWord *dw = (DataWord *)data.ptr_data;
    printf("%s %d", dw->word, dw->counter);
}

void free_word(data_union data)
{
    DataWord *dw = (DataWord *)data.ptr_data;
    free(dw->word);
    free(dw);
}

int cmp_word(data_union a, data_union b)
{
    DataWord *A = (DataWord *)a.ptr_data;
    DataWord *B = (DataWord *)b.ptr_data;

    return strcmp(A->word, B->word);
}

size_t hash_word(data_union data, size_t size) {
    int s = 0;
    DataWord *dw = (DataWord*)data.ptr_data;
    char *p = dw->word;
    while (*p) {
        s += *p++;
    }
    return hash_base(s, size);
}

void modify_word(data_union *data)
{
    DataWord *dw = (DataWord *)data->ptr_data;
    dw->counter++;
}

data_union create_data_word(void *value)
{
    DataWord *dw = malloc(sizeof(DataWord));
    dw->word = strdup((char *)value);
    dw->counter = 1;

    for(size_t i = 0; i < strlen(dw->word); i++)
        dw->word[i] = tolower(dw->word[i]);

    return (data_union){.ptr_data = dw};
}

// read text, parse it to words, and insert these words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream)
{
    char *p, buf[BUFFER_SIZE] = {0};
    char delimits[] = " \r\t\n.,?!-;";

    while(fgets(buf,BUFFER_SIZE,stream)) {
        p = strtok(buf, delimits);
        while(p) {
            data_union data_union = create_data_word(p);
            insert_element(p_table, &data_union);

            p = strtok(NULL, delimits);
        }
    }
}

// test primitive type list
void test_ht(hash_table *p_table, int n) {
    char op;
    data_union data;
    for (int i = 0; i < n; ++i) {
        scanf(" %c", &op);
//		p_table->create_data(&data);
        data = p_table->create_data(NULL); // should give the same result as the line above
        switch (op) {
            case 'r':
                remove_element(p_table, data);
                break;
            case 'i':
                insert_element(p_table, &data);
                break;
            default:
                printf("No such operation: %c\n", op);
                break;
        }
    }
}

int main(void) {
    int to_do, n;
    size_t index;
    hash_table table;
    char buffer[BUFFER_SIZE];
    data_union data;

    scanf ("%d", &to_do);
    switch (to_do) {
        case 1: // test integer hash table
            scanf("%d %zu", &n, &index);
            init_ht(&table, 4, dump_int, create_int, NULL, cmp_int, hash_int, NULL);
            test_ht(&table, n);
            printf ("%zu\n", table.size);
            dump_list(&table, index);
            break;
        case 2: // test char hash table
            scanf("%d %zu", &n, &index);
            init_ht(&table, 4, dump_char, create_char, NULL, cmp_char, hash_char, NULL);
            test_ht(&table, n);
            printf ("%zu\n", table.size);
            dump_list(&table, index);
            break;
        case 3: // read words from text, insert into hash table, and print
            scanf("%s", buffer);
            init_ht(&table, 8, dump_word, create_data_word, free_word, cmp_word, hash_word, modify_word);
            stream_to_ht(&table, stdin);
            printf ("%zu\n", table.size);
            data = table.create_data(buffer);
            ht_element *e = get_element(&table, &data);
            if (e) table.dump_data(e->data);
            if (table.free_data) table.free_data(data);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    free_table(&table);

    return 0;
}