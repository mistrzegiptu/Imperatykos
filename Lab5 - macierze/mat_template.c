#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n)
{
    for(int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; ++j)
        {
            AB[i][j] = 0.0;
            for (int k = 0; k < p; ++k)
            {
                AB[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n)
{
    double det = 1.0;
    for(int i = 0; i < n; i++)
    {
        for(int j = i + 1; j < n; j++)
        {
            double ratio = A[j][i] / A[i][i];
            for(int k = 0; k < n; k++)
                A[j][k] -= ratio * A[i][k];
        }
    }
    for(int i = 0; i < n; i++)
    {
        if(A[i][i] == 0.0)
            return NAN;
        det *= A[i][i];
    }

    return det;
}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {
}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.
void swapIndexes(int *indexes, int i, int j)
{
    int swapper = indexes[i];
    indexes[i] = indexes[j];
    indexes[j] = swapper;
}
double gauss(double A[][SIZE], double b[], double x[], const int n, const double eps)
{
    int indexes[n];
    int sign = 1;

    for(int i = 0; i < n; i++)
    {
        indexes[i] = i;
        x[i] = 0;
    }

    for(int i = 0; i < n; i++)
    {
        double maxElement = A[indexes[i]][i];
        int maxIndex = i;
        for (int j = i+1; j < n; j++)
        {
            if(fabs(A[indexes[j]][i]) > maxElement)
            {
                maxElement = fabs(A[indexes[j]][i]);
                maxIndex = j;
            }
        }
        if(maxIndex != i)
        {
            swapIndexes(indexes, i, maxIndex);
            sign = -sign;
        }

        for (int j = i+1; j < n; j++)
        {
            double ratio = A[indexes[j]][i] / A[indexes[i]][i];
            b[indexes[j]] -= ratio * b[indexes[i]];
            for (int k = 0; k < n; k++)
                A[indexes[j]][k] -= ratio * A[indexes[i]][k];
        }

        if(fabs(A[indexes[i]][i]) < eps)
            return 0;
    }

    double detW = 1.0*(double)sign;
    for (int i = 0; i < n; i++)
        detW *= A[indexes[i]][i];

    if(detW == 0.0)
        return 0;

    for(int i = n-1; i >= 0; i--)
    {
        double result = b[indexes[i]];

        for (int j = i+1; j < n; j++)
            result -= x[j] * A[indexes[i]][j];

        x[i] = result / A[indexes[i]][i];
    }

    return detW;
}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps)
{
    int indexes[n];
    int sign = 1;
    double inverse[SIZE][SIZE] = {0};

    for(int i = 0; i < n; i++)
    {
        indexes[i] = i;
        inverse[i][i] = 1.0;
    }

    for(int i = 0; i < n; i++)
    {
        double maxElement = A[indexes[i]][i];
        int maxIndex = i;
        for (int j = i+1; j < n; j++)
        {
            if(fabs(A[indexes[j]][i]) > maxElement)
            {
                maxElement = fabs(A[indexes[j]][i]);
                maxIndex = j;
            }
        }
        if(maxIndex != i)
        {
            swapIndexes(indexes, i, maxIndex);
            sign = -sign;
        }

        for (int j = i+1; j < n; j++)
        {
            double ratio = A[indexes[j]][i] / A[indexes[i]][i];

            for (int k = 0; k < n; k++)
            {
                A[indexes[j]][k] -= ratio * A[indexes[i]][k];
                inverse[indexes[j]][k] -= ratio * inverse[indexes[i]][k];
            }
        }

        if(fabs(A[indexes[i]][i]) < eps)
            return 0;
    }

    double det = 1.0 * (double)sign;
    for(int i = 0; i < n; i++)
    {
        det *= A[indexes[i]][i];
    }

    for(int i = n-1; i >= 0; i--)
    {
        for(int j = i-1; j >= 0; j--)
        {
            double ratio = A[indexes[j]][i] / A[indexes[i]][i];
            A[indexes[j]][i] = 0.0;
            for (int k = 0; k < n; k++)
            {
                inverse[indexes[j]][k] -= ratio * inverse[indexes[i]][k];
            }
        }
    }

    for(int i = 0; i < n; i++)
    {
        double ratio = A[indexes[i]][i];
        A[indexes[i]][i] = 1.0;
        for (int j = 0; j < n; j++)
        {
            inverse[indexes[i]][j] /= ratio;
        }
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
            B[i][j] = inverse[indexes[i]][j];
    }

    return det;
}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
			if (det) print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

