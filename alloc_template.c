#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

// 1
////////////////////////////////////////////////////////////

int get(int cols, int row, int col, const int* A)
{
    return A[row*cols + col];
}

void set(int cols, int row, int col, int* A, int value)
{
    A[row*cols + col] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int* A, int* B, int* AB)
{
    for(int i = 0; i < rowsA; i++)
    {
        for(int j = 0; j < colsB; j++)
        {
            int value = 0;
            for(int k = 0; k < colsA; k++)
            {
                value += get(colsA, i, k, A) * get(colsB, k, j, B);
            }
            set(rowsA, i, j, AB, value);
        }
    }
}

void read_mat(int rows, int cols, int* t)
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            scanf("%d", &t[i*cols + j]);
        }
    }
}

void print_mat(int rows, int cols, int* t)
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            printf("%d ", t[i*cols + j]);
        }
        printf("\n");
    }
}

// 2
/////////////////////////////////////////////////////////////

int read_ints_from_line(char* c_buf, int *int_buf)
{
    return 0;
}
void fill_int_array(int *array, int n, int value)
{
    for(int i = 0; i < n; i++)
        array[i] = value;
}
int read_int_lines_cont(int* ptr_array[])
{
    int *line = malloc(sizeof(int) * TAB_SIZE);
    fill_int_array(line, TAB_SIZE, INT_MAX);
    int i = 0, j = 0, value = 0;
    char c = fgetc(stdin);

    while(c != EOF)
    {
        if(c == '\n')
        {
            line[j] = value;
            ptr_array[i] = line;
            line = malloc(sizeof(int) * TAB_SIZE);
            fill_int_array(line, TAB_SIZE, INT_MAX);
            i++;
            j = 0;
            value = 0;
        }
        if(c >= '0' && c <= '9')
        {
            value = value * 10 + (c - '0');
        }
        else if(c == ' ')
        {
            line[j] = value;
            j++;
            value = 0;
        }
        c = fgetc(stdin);
    }

    return i;
}

void write_int_line_cont(int *ptr_array[], int n)
{
    for(int i = 0; i < TAB_SIZE; i++)
    {
        if(ptr_array[n][i] == INT_MAX)
            return;
        printf("%d ", ptr_array[n][i]);
    }
}

// 3
///////////////////////////////////////////////////////////

int read_char_lines(char *array[])
{
    char *line = calloc(sizeof(char ) * TAB_SIZE, sizeof(char));
    int i = 0, j = 0, value = 0;
    char c = fgetc(stdin);

    while(c != EOF)
    {
        if(c == '\n')
        {
            line[j] = value;
            array[i] = line;
            line = calloc(sizeof(char) * TAB_SIZE, sizeof(char));
            i++;
            j = 0;
        }
        line[j] = c;
        j++;
        c = fgetc(stdin);
    }

    return i;
}

void write_char_line(char *array[], int n)
{
    for(int i = 0; i < TAB_SIZE; i++)
    {
        if(array[n][i] == '\0')
            return;
        printf("%c", array[n][i]);
    }
}

void delete_lines(char *array[], int n)
{
    for(int i = 0; i < n; i++)
        free(array[i]);
}

// 4
////////////////////////////////////////////////

typedef struct {
	int *values;
	int len;
	double average;
} line_type;

int read_int_lines(line_type lines_array[])
{
    return 0;
}

void write_int_line(line_type lines_array[], int n) {
}

void delete_int_lines(line_type array[], int line_count) {
}

int cmp (const void *a, const void *b)
{
    return 0;
}

void sort_by_average(line_type lines_array[], int line_count) {
}

// 5
///////////////////////////////////////////////////////////

typedef struct {
	int r, c, v;
} triplet;

int read_sparse(triplet* triplet_array, int n_triplets)
{
    return 0;
}

int cmp_triplets(const void* t1, const void* t2)
{
    return 0;
}

void make_CSR(triplet* triplet_array, int n_triplets, int rows, int* V, int* C, int* R) {
}

void multiply_by_vector(int rows, const int* V, const int* C, const int* R,
						const int* x, int* y) {
}

void read_vector(int* v, int n) {
}

void write_vector(int* v, int n) {
}

// auxiliary
////////////////////////////////////////////////////////////

int read_int(void) {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	int rows, cols, n_triplets;
	char* char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int* ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type int_lines_array[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d", &rowsA, &colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d", &rowsB, &colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA, colsA, colsB, A, B, AB);
			print_mat(rowsA, colsB, AB);
            //print_mat(rowsA, colsA, A);
            //print_mat(rowsB, colsB, B);
			break;
		case 2:
			n = read_int() - 1; // we count from 1 :)
			ptr_array[0] = continuous_array;
			lines_counter = read_int_lines_cont(ptr_array);
			write_int_line_cont(ptr_array, n);
            for(int i = 0; i < lines_counter; i++)
                free(ptr_array[i]);
			break;
		case 3:
			n = read_int() - 1;
			lines_counter = read_char_lines(char_lines_array);
			write_char_line(char_lines_array, n);
			delete_lines(char_lines_array, lines_counter);
			break;
		case 4:
			n = read_int() - 1;
			lines_counter = read_int_lines(int_lines_array);
			sort_by_average(int_lines_array, lines_counter);
			write_int_line(int_lines_array, n);
			delete_int_lines(int_lines_array, lines_counter);
			break;
		case 5:
			scanf("%d %d %d", &rows, &cols, &n_triplets);
			n_triplets = read_sparse(triplet_array, n_triplets);
			read_vector(x, cols);
			make_CSR(triplet_array, n_triplets, rows, V, C, R);
			multiply_by_vector(rows, V, C, R, x, y);
			write_vector(V, n_triplets);
			write_vector(C, n_triplets);
			write_vector(R, rows + 1);
			write_vector(y, rows);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

