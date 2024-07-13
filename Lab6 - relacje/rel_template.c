#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;

int get_domain(pair *tab, int n, int *domain);
int is_connected(pair *tab, int n);

// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair)
{
    if(n == MAX_REL_SIZE)
        return n;

    for(int i = 0; i < n; i++)
    {
        if(tab[i].first == new_pair.first && tab[i].second == new_pair.second)
            return n;
    }

    tab[n] = new_pair;

    return n+1;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation)
{
    int n = 0;
    int first = 0, second = 0;
    scanf("%d", &n);

    for(int i = 0; i < n; i++)
    {
        scanf("%d %d", &first, &second);
        relation[i].first = first;
        relation[i].second = second;
    }

    return n;
}

void print_int_array(int *array, int n) {
	printf("%d\n", n);
	for (int i = 0; i < n; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

//------------------------------------------------

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair *tab, int n)
{
    for(int i = 0; i < n; i++)
    {
        int relation = 0;
        for(int j = 0; j < n; j++)
        {
            if(tab[i].first == tab[j].first && tab[i].first == tab[j].second)
                relation = 1;
        }
        if(!relation)
            return 0;
    }
    return 1;
}

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair *tab, int n)
{
    for(int i = 0; i < n; i++)
    {
        if(tab[i].first == tab[i].second)
            return 0;
    }

    return 1;
}

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair *tab, int n)
{
    for(int i = 0; i < n; i++)
    {
        int symmetric = 0;
        for(int j = 0; j < n; j++)
        {
            if(tab[i].first == tab[j].second && tab[i].second == tab[j].first)
                symmetric = 1;
        }
        if(!symmetric)
            return 0;
    }

    return 1;
}

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair *tab, int n)
{

    for(int i = 0; i < n; i++)
    {
        int antisymmetric = 0;
        for(int j = 0; j < n; j++)
        {
            if(tab[i].first == tab[j].second && tab[i].second == tab[j].first)
            {
                if(tab[i].first == tab[j].first)
                    antisymmetric = 1;
                else
                    return 0;
            }
        }
    }

    return 1;
}

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair *tab, int n)
{

    for(int i = 0; i < n; i++)
    {
        int asymmetric = 0;
        for(int j = 0; j < n; j++)
        {
            if(tab[i].first == tab[j].second && tab[i].second == tab[j].first)
                return 0;
        }
    }

    return 1;
}

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair *tab, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(tab[i].second == tab[j].first)
            {
                int homogenous = 0;
                for(int k = 0; k < n; k++)
                {
                    if(tab[k].first == tab[i].first && tab[j].second == tab[k].second)
                        homogenous = 1;
                }
                if(!homogenous)
                    return 0;
            }
        }
    }

    return 1;
}
//------------------------------------------------

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair *tab, int n)
{
    return is_reflexive(tab, n) && is_antisymmetric(tab, n) && is_transitive(tab, n);
}

// A total order relation is a partial order relation that is connected
int is_total_order(pair *tab, int n)
{
    return is_partial_order(tab, n) && is_connected(tab, n);
}

int find_max_elements(pair *tab, int n, int *max_elements)
{ // tab - partial order
    int domain[MAX_RANGE] = {0};
    int n_domain = get_domain(tab, n, domain);
    int index = 0;

    for(int i = 0; i < n_domain; i++)
    {
        int is_max = 1;
        for(int j = 0; j < n; j++)
        {
            if(tab[j].first == domain[i] && tab[j].second != domain[i])
                is_max = 0;
        }
        if(is_max)
        {
            max_elements[index] = domain[i];
            index++;
        }
    }

    return index;
}

int find_min_elements(pair *tab, int n, int *min_elements)
{
    int domain[MAX_RANGE] = {0};
    int n_domain = get_domain(tab, n, domain);
    int index = 0;

    for(int i = 0; i < n_domain; i++)
    {
        int is_min = 1;
        for(int j = 0; j < n; j++)
        {
            if(tab[j].second == domain[i] && tab[j].first != domain[i])
                is_min = 0;
        }
        if(is_min)
        {
            min_elements[index] = domain[i];
            index++;
        }
    }

    return index;
}

int get_domain(pair *tab, int n, int *domain)
{
    int index = 0;

    for(int i = 0; i < n; i++)
    {
        domain[tab[i].first]++;
        domain[tab[i].second]++;
    }
    for(int i = 0; i < 100; i++)
    {
        if (domain[i] > 0)
        {
            domain[index] = i;
            index++;
        }
    }

    return index;
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair *tab, int n)
{
    int domain[MAX_RANGE] = {0};
    int n_domain = get_domain(tab, n, domain);

    for(int i = 0; i < n_domain; i++)
    {
        for(int j = 0; j < n_domain; j++)
        {
            int total = 0;
            for(int k = 0; k < n; k++)
            {
                if((tab[k].first == domain[i] && tab[k].second == domain[j]) || (tab[k].first == domain[j] && tab[k].second == domain[i]))
                {
                    total = 1;
                }
            }
            if(!total)
                return 0;
        }
    }

    return 1;
}

//------------------------------------------------

// Case 3:

// x(S o R)z iff exists y: xRy and ySz
int composition (pair *rel_1, int n1, pair *rel_2, int n2, pair *rel_3)
{
    int index = 0;

    for(int i = 0; i < n1; i++)
    {
        for(int j = 0; j < n2; j++)
        {
            if(rel_1[i].second == rel_2[j].first)
            {
                int in_relation = 0;
                for(int k = 0; k < index; k++)
                {
                    if(rel_3[k].first == rel_1[i].first && rel_3[k].second == rel_2[j].second)
                        in_relation = 1;
                }
                if(!in_relation)
                {
                    rel_3[index].first = rel_1[i].first;
                    rel_3[index].second = rel_2[j].second;
                    index++;
                }
            }
        }
    }

    return index;
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE] = {0};
	int domain[MAX_REL_SIZE] = {0};
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	
	return 0;
}

