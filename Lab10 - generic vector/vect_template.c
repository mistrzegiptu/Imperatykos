#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

#pragma GCC diagnostic error "-Wpointer-arith"

typedef struct Vector {
	void *data;
	size_t element_size;
	size_t size;
	size_t capacity;
} Vector;



typedef int(*cmp_ptr)(const void*, const void*);
typedef int(*predicate_ptr)(void*);
typedef void(*read_ptr)(void*);
typedef void(*print_ptr)(const void*);

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size)
{
    vector->data = malloc(block_size * element_size);
    vector->element_size = element_size;
    vector->capacity = block_size;
    vector->size = 0;
}

// If new_capacity is greater than the current capacity,
// new data is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity)
{
    if(vector == NULL)
        return;

    if(vector->capacity < new_capacity)
    {
        vector->capacity = new_capacity;
        vector->data = realloc(vector->data, new_capacity * vector->element_size);
    }
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size)
{
    if(new_size > vector->capacity)
        reserve(vector, vector->capacity*2);

    if(vector->size < new_size)
        memset((char *)(vector->data)+(vector->size*vector->element_size), 0, (new_size-vector->size)*vector->element_size);
    else if(vector->size > new_size)
        memset((char *)(vector->data)+(new_size*vector->element_size), 0, (vector->size-new_size)*vector->element_size);

    vector->size = new_size;
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value)
{
    if(vector == NULL)
        return;
    if(vector->size + 1 > vector->capacity)
    {
        vector->capacity *= 2;
        vector->data = realloc(vector->data, vector->capacity * vector->element_size);
    }

    memcpy((char *)(vector->data)+(vector->size)*vector->element_size, value, vector->element_size);
    vector->size += 1;
}

// Remove all elements from the vector
void clear(Vector *vector)
{
    if(vector == NULL)
        return;

    memset(vector->data, 0, vector->element_size*vector->capacity);
    vector->size = 0;
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, size_t index, void *value)
{
    if(vector == NULL)
        return;
    if(vector->size == vector->capacity)
        reserve(vector, vector->capacity*2);

    memmove((char *)(vector->data)+((index+1)*vector->element_size), (char *)(vector->data)+(index*vector->element_size), (vector->size-index)*vector->element_size);
    memcpy((char *)(vector->data)+(index*vector->element_size), value, vector->element_size);
    vector->size += 1;
}

// Erase element at position index
void erase(Vector *vector, size_t index)
{
    if(vector == NULL)
        return;
    if(index > vector->size)
        return;

    memmove((char *)(vector->data)+((index)*vector->element_size), (char *)(vector->data)+((index+1)*vector->element_size), (vector->size-index)*vector->element_size);
    vector->size -= 1;
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, cmp_ptr cmp)
{
    if(vector == NULL)
        return;

    for(size_t i = 0; i < vector->size; i++)
    {
        if(cmp((char *)(vector->data)+i*vector->element_size, value) == 0)
        {
            erase(vector, i);
            i -= 1;
        }
    }
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *))
{
    if(vector == NULL)
        return;

    for(size_t i = 0; i < vector->size; i++)
    {
        if(predicate((char *)(vector->data)+(i*vector->element_size)) == 0)
        {
            erase(vector, i);
            i -= 1;
        }
    }
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector)
{
    if(vector == NULL)
        return;

    vector->data = realloc(vector->data, vector->element_size * vector->size);
    vector->capacity = vector->size;
}

// ---------------------------------------------------

typedef struct {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

void print_int(const void *v)
{
    printf("%d ", *(int *)v);
}

void print_char(const void *v)
{
    printf("%c ", *(char *)v);
}

void print_person(const void *v)
{
    Person p = *(Person *)v;
    printf("%d %s %s\n", p.age, p.first_name, p.last_name);
}

void print_vector(Vector *vector, print_ptr print)
{
    if(vector == NULL)
        return;

    printf("%zu\n", vector->capacity);
    for(size_t i = 0; i < vector->size; i++)
    {
        print((char *)(vector->data) + (i * vector->element_size));
    }
}

int int_cmp(const void *v1, const void *v2)
{
    int a = *(int *)v1;
    int b = *(int *)v2;

    return a-b;
}

int char_cmp(const void *v1, const void *v2)
{
    char a = *(char *)v1;
    char b = *(char *)v2;

    return a-b;
}

int person_cmp(const void *p1, const void *p2)
{
    Person *a = (Person *)p1;
    Person *b = (Person *)p2;

    if(a->age > b->age)
        return -1;
    else if(a->age < b->age)
        return 1;
    else
    {
        int firstNameRes = strcmp(a->first_name, b->first_name);
        if(firstNameRes == 0)
            return strcmp(a->last_name, b->last_name);
        else
            return firstNameRes;
    }
}

int is_even(void *value)
{
    int val = *(int *)value;

    return val % 2;
}

int is_vowel(void *value)
{
    char c = *(char *)value;
    if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y' || c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U' || c == 'Y')
        return 0;
    return 1;
}

int is_older_than_25(void *person)
{
    Person *p = (Person *)person;
    if(p->age > 25)
        return 0;
    return 1;
}

void read_int(void* value)
{
    scanf("%d", (int *)value);
}

void read_char(void* value)
{
    scanf(" %c", (char *)value);
}

void read_person(void* value)
{
    Person *p = (Person *)value;
    scanf("%d %s %s", &p->age, p->first_name, p->last_name);
}

void vector_test(Vector *vector, size_t block_size, size_t elem_size, int n, read_ptr read,
		 cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
	init_vector(vector, block_size, elem_size);
	void *v = malloc(vector->element_size);
	size_t index, size;
	for (int i = 0; i < n; ++i) {
		char op;
		scanf(" %c", &op);
		switch (op) {
			case 'p': // push_back
				read(v);
				push_back(vector, v);
				break;
			case 'i': // insert
				scanf("%zu", &index);
				read(v);
				insert(vector, index, v);
				break;
			case 'e': // erase
				scanf("%zu", &index);
				erase(vector, index);
				break;
			case 'v': // erase
				read(v);
				erase_value(vector, v, cmp);
				break;
			case 'd': // erase (predicate)
				erase_if(vector, predicate);
				break;
			case 'r': // resize
				scanf("%zu", &size);
				resize(vector, size);
				break;
			case 'c': // clear
				clear(vector);
				break;
			case 'f': // shrink
				shrink_to_fit(vector);
				break;
			case 's': // sort
				qsort(vector->data, vector->size,
				      vector->element_size, cmp);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
	print_vector(vector, print);
	free(vector->data);
	free(v);
}

int main(void) {
	int to_do, n;
	Vector vector_int, vector_char, vector_person;

	scanf("%d%d", &to_do, &n);

	switch (to_do) {
		case 1:
			vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp,
				is_even, print_int);
			break;
		case 2:
			vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp,
				is_vowel, print_char);
			break;
		case 3:
			vector_test(&vector_person, 2, sizeof(Person), n, read_person,
				person_cmp, is_older_than_25, print_person);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}

	return 0;
}

