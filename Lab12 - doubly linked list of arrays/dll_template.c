#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// list node
typedef struct Node {
	int *data;
	size_t size;
	struct Node* next;
	struct Node* prev;
} Node;

// doubly linked list
typedef struct List {
	Node *head;
	Node *tail;
} List;

// iterator
typedef struct iterator {
	struct Node* node_ptr; // pointer to the node
	size_t position; // position of the element within the node
} iterator;

// forward initialization
iterator begin(Node* head) {
	iterator it = { head, 0 };
	return it;
}

// backward initialization
iterator end(Node* tail) {
	iterator it = { tail, tail->size }; // points to the element after the last one
	return it;
}

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr) return ptr;
	exit(EXIT_FAILURE);
}

void *safe_realloc(void *old_ptr, size_t size) {
	void *ptr = realloc(old_ptr, size);
	if (ptr) return ptr;
	free(old_ptr);
	exit(EXIT_FAILURE);
}

Node *create_node(int *data, size_t size, Node *next, Node *prev) {
	Node *node = safe_malloc(sizeof(Node));
	node->data = data;
	node->size = size;
	node->next = next;
	node->prev = prev;
	return node;
}

// initialize list
void init(List *list) {
	list->head = NULL;
	list->tail = NULL;
}

void free_node(Node **node) {
	if (*node == NULL) return;
	if ((*node)->data != NULL) free((*node)->data);
	free(*node);
	*node = NULL;
}

// to implement ...

// push front
void push_front(List *list, int *data, size_t size)
{
    Node *temp = safe_malloc(sizeof(Node));
    temp->size = size;
    temp->data = data;
    temp->prev = NULL;

    if(list->head == NULL)
    {
        temp->next = NULL;
        list->head = temp;
        list->tail = temp;
    }
    else
    {
        temp->next = list->head;
        list->head->prev = temp;
        list->head = temp;
    }
}

// append element to the list
void push_back(List *list, int *data, size_t size)
{
    Node *temp = safe_malloc(sizeof(Node));
    temp->size = size;
    temp->data = data;

    if(list->head == NULL)
    {
        temp->next = NULL;
        temp->prev = NULL;
        list->head = temp;
        list->tail = temp;
    }
    else
    {
        temp->next = NULL;
        temp->prev = list->tail;
        list->tail->next = temp;
        list->tail = temp;
    }
}

int pop_front(List *list)
{
    /*Node *temp = list->head;

    if(list->head != NULL)
    {
        list->head = list->head->next;
        list->head->prev = NULL;
        temp->next = NULL;
    }

    return temp;*/
}

int pop_back(List *list) {
}

// set iterator to move n elements forward from its current position
void skip_forward(iterator* itr, size_t n)
{
    while(n > 0 && itr->node_ptr != NULL)
    {
        if(n <= itr->node_ptr->size)
        {
            itr->position++;
            n--;
        }
        else
        {
            n -= itr->node_ptr->size;
            itr->position = 0;
            itr->node_ptr = itr->node_ptr->next;
        }
    }
    itr->position -= 1;
}

// forward iteration - get n-th element in the list
int get_forward(List *list, size_t n)
{
    iterator iter = begin(list->head);
    skip_forward(&iter, n);
    return iter.node_ptr->data[iter.position];
}

// set iterator to move n elements backward from its current position
void skip_backward(iterator* itr, size_t n)
{
    while(n > 0 && itr->node_ptr != NULL)
    {
        if(n <= itr->node_ptr->size)
        {
            itr->position--;
            n--;
        }
        else
        {
            n -= itr->node_ptr->size;
            itr->node_ptr = itr->node_ptr->prev;
            itr->position = itr->node_ptr->size;
        }
    }
}

// backward iteration - get n-th element from the end of the list
int get_backward(List *list, size_t n)
{
    iterator iter = end(list->tail);
    skip_backward(&iter, n);
    return iter.node_ptr->data[iter.position];
}

void remove_node(List *list, Node *node_ptr)
{
    if(list->head == node_ptr)
    {
        list->head = list->head->next;
        if(list->head != NULL)
            list->head->prev = NULL;
    }
    else if(list->tail == node_ptr)
    {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }
    else
    {
        node_ptr->prev->next = node_ptr->next;
        node_ptr->next->prev = node_ptr->prev;
    }

    free(node_ptr->data);
    free(node_ptr);
}

// remove n-th element; if array empty remove node
void remove_at(List *list, size_t n)
{
    iterator iter = begin(list->head);
    skip_forward(&iter, n);

    if(iter.node_ptr->size == 1)
        remove_node(list, iter.node_ptr);
    else
    {
        iter.node_ptr->size -= 1;
        for(size_t i = iter.position; i < iter.node_ptr->size; i++)
            iter.node_ptr->data[i] = iter.node_ptr->data[i+1];
    }

}

size_t digits(int n)
{
    size_t result = 0;
    n = abs(n);
    if(n == 0)
        return 1;
    while(n > 0)
    {
        n = n / 10;
        result++;
    }
    return result;
}

void insert_in_order(List *list, int value)
{
    size_t d = digits(value);
    if(list->head == NULL)
    {
        int *data = safe_malloc(sizeof(int));
        data[0] = value;
        push_back(list, data, 1);
    }
    else if(digits(list->head->data[0]) > d)
    {
        int *data = safe_malloc(sizeof(int));
        data[0] = value;
        push_front(list, data, 1);
    }
    else if(digits(list->tail->data[0]) < d)
    {
        int *data = safe_malloc(sizeof(int));
        data[0] = value;
        push_back(list, data, 1);
    }
    else
    {
        Node *iter = list->head;
        while (iter != NULL)
        {
            if (digits(iter->data[0]) == d)
            {
                size_t index = 0;
                while (index < iter->size && iter->data[index] < value)
                    index++;

                iter->size += 1;
                iter->data = safe_realloc(iter->data, sizeof(int) * iter->size);

                int move = 0;

                for (int i = iter->size - 1; i > index; i--)
                {
                    iter->data[i] = iter->data[i - 1];
                }

                iter->data[index] = value;
            } else if (digits(iter->data[0]) > d)
            {
                if (iter == list->head)
                {
                    int *data = safe_malloc(sizeof(int));
                    data[0] = value;
                    push_front(list, data, 1);
                }
                Node *temp = safe_malloc(sizeof(Node));

            }

            iter = iter->next;
        }
    }
}

// -------------------------------------------------------------
// helper functions

// print list
void dumpList(const List *list) {
	for(Node *node = list->head; node != NULL; node = node->next) {
		printf("-> ");
		for (int k = 0; k < node->size; k++) {
			printf("%d ", node->data[k]);
		}
		printf("\n");
	}
}

// free list
void freeList(List *list)
{
    Node *iter = list->head;
    while(iter != NULL)
    {
        Node *toDelete = iter;
        iter = iter->next;

        free(toDelete->data);
        free(toDelete);
    }
}

// read int vector
void read_vector(int array[], size_t n) {
	for (size_t i = 0; i < n; ++i) {
		scanf("%d", array + i);
	}
}

// initialize the list and push data
void read_list(List *list) {
	int n;
	size_t size;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%zu", &size);
		int *array = (int*) safe_malloc(size * sizeof(int));
		read_vector(array, size);
		push_back(list, array, size);
	}
}

int main(void) {
	int to_do, value;
	size_t size, offset;
	List list;
	init(&list);

	scanf("%d", &to_do);
	switch (to_do) {
		case 1:
			read_list(&list);
			dumpList(&list);
			break;
		case 2:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &offset);
				printf("%d ", get_forward(&list, offset));
			}
			printf("\n");
			break;
		case 3:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &offset);
				printf("%d ", get_backward(&list, offset));
			}
			printf("\n");
			break;
		case 4:
			read_list(&list);
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%zu", &offset);
				remove_at(&list, offset);
			}
			dumpList(&list);
			break;
		case 5:
			scanf("%zu", &size);
			for (int i = 0; i < size; i++) {
				scanf("%d", &value);
				insert_in_order(&list, value);
			}
			dumpList(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	freeList(&list);

	return 0;
}

