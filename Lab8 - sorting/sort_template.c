#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int day; 
	int month; 
	int year;
} Date;

// 1 bsearch2

#define NAME_MAX  20   // max. length of name + 1
#define FOOD_MAX  30   // max. number of goods
#define RECORD_MAX 64  // max. line length

typedef struct {
	char name[NAME_MAX];
	float price;
	int amount;
	Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

int cmp_date(const void *d1, const void *d2)
{
    const Date *date1 = (Date *) d1;
    const Date *date2 = (Date *) d2;

    if(date1->year < date2->year)
        return -1;
    else if(date1->year > date2->year)
        return 1;
    else
    {
        if(date1->month < date2->month)
            return -1;
        else if(date1->month > date2->month)
            return 1;
        else
        {
            if(date1->day < date2->day)
                return -1;
            else if(date1->day > date2->day)
                return 1;
            else
            {
                return 0;
            }
        }
    }
}
int compare_price(const void* a, const void* b){
    Food *first = (Food*)a;
    Food *second = (Food*)b;

    if(first->price < second->price)
        return -1;
    else if(first->price > second->price)
        return 1;
    else{
        return 0;
    }
}
int cmp(const void *a, const void *b)
{
    const Food *first = (Food *)a;
    const Food *second = (Food *)b;

    int str = strcmp(first->name, second->name);
    if (str < 0)
        return -1;
    else if (str > 0)
        return 1;
    else{
        int price = compare_price(a, b);
        if(price < 0)
            return -1;
        else if (price > 0)
            return 1;
        else {
            int date = cmp_date(&first->valid_date, &second->valid_date);
            if (date < 0)
                return -1;
            else if (date > 0)
                return 1;
            else
                return 0;
        }
    }
}
void *bsearch2(const void *key, void *base0, size_t nitems, size_t size, int(*compar)(const void*, const void*), char *result)
{
    const char *base = (const char *) base0;
    int lim = 0, cmp;
    const void *p;
    *result = 0;

    for (lim = nitems; lim != 0; lim >>= 1)
    {
        p = base + (lim >> 1) * size;
        cmp = (*compar)(key, p);
        if (cmp == 0)
        {
            *result = 1;
            return (void *) p;
        }
        if (cmp > 0) {
            base = (const char *)p + size;
            lim--;
        }
    }
    return base + (lim >> 1) * size;
}
void print_art(Food *p, const int n, const char *art)
{
    for(int i = 0; i < n; i++)
    {
        if(strcmp(p[i].name, art) == 0)
            printf("%.2f %d %02d.%02d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);
    }
}

Food* add_record(Food *tab, size_t size, int *np, const ComparFp compar, const Food *new)
{
    char result;
    Food *returnValue = bsearch2(new, tab, *np, sizeof(Food), compar, &result);
    if(result == 1)
    {
        returnValue->amount += new->amount;
    }
    else
    {
        if(*np == size)
            return NULL;

        *np += 1;
        for(int i = *np; i > (int)(returnValue-tab); i--)
        {
            tab[i] = tab[i-1];
        }
        tab[(int)(returnValue-tab)] = *new;
    }
    return returnValue;
}

int read_goods(Food *tab, size_t size, int no, FILE *stream, const int sorted)
{
    int np = 0;
    for(int i = 0; i < no; i++)
    {
        Food newFood = {0};

        scanf("%s", newFood.name);
        scanf("%f %d", &newFood.price, &newFood.amount);
        scanf("%d.%d.%d", &newFood.valid_date.day, &newFood.valid_date.month, &newFood.valid_date.year);

        add_record(tab, size, &np, cmp, &newFood);
    }

    return np;
}

int cmp_qs(const void *a, const void *b) {
	const Food *fa = (Food*)a, *fb = (Food*)b;
	return cmp_date(&fa->valid_date, &fb->valid_date);
}

float value(Food *food_tab, const size_t n, const Date curr_date, const int days)
{
    struct tm t = {.tm_year = curr_date.year, .tm_mon = curr_date.month, .tm_mday = curr_date.day};
    t.tm_mday += days;
    mktime(&t);

    Date newDate = {0};
    newDate.year = t.tm_year;
    newDate.month = t.tm_mon;
    newDate.day = t.tm_mday;

    qsort(food_tab, n, sizeof(Food), cmp_qs);

    Food f = {0};
    f.valid_date = newDate;

    char result;
    float value = 0;
    Food *returnValue = bsearch2(&f, food_tab, n, sizeof(Food), cmp_qs, &result);

    if(result == 0)
        return 0;

    int i = (int)(returnValue-food_tab);
    for(int j = i; j < n; j++)
    {
        if(cmp_date(&food_tab[j].valid_date, &newDate) == 0)
            value += (float)food_tab[j].amount * food_tab[j].price;
        else
            break;
    }
    for(int j = i-1; j >= 0; j--)
    {
        if(cmp_date(&food_tab[j].valid_date, &newDate) == 0)
            value += (float)food_tab[j].amount * food_tab[j].price;
        else
            break;
    }

    return value;
}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
	enum Sex sex:1;
	enum BOOL in_line:1;
};

typedef struct {
	char *name;
	struct Bit_data bits;
	Date born;
	char *parent;
} Person;

typedef struct {
	char *par_name;
	int indexFirst;
    int indexLast;
} Parent;

const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(const Person *person1, const Person *person2) {
	if (person1->bits.sex == person2->bits.sex) return 0;
	if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
	if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
	return person2->bits.sex - person1->bits.sex;
}

int cmp_person(const void *a, const void *b)
{
    Person *first = (Person *)a;
    Person *second = (Person *)b;

    if(first->parent == NULL)
        return 1;
    else if(second->parent == NULL)
        return -1;

    return strcmp(first->parent, second->parent);
}

int cmp_parent(const void *a, const void *b)
{
    Parent *first = (Parent *)a;
    Parent *second = (Parent *)b;

    return strcmp(first->par_name, second->par_name);
}

int fill_indices_tab(Parent *idx_tab, const Person *pers_tab, const int size)
{
    int parentsCount = 0;

    for(int i = 0; i < size; i++)
    {
        int parentFound = 0;
        for(int j = 0; j < parentsCount; j++)
        {
            if(strcmp(pers_tab[i].name, idx_tab[j].par_name) == 0)
            {
                parentFound = 1;
                break;
            }
        }
        if(parentFound == 0)
        {
            idx_tab[parentsCount].par_name = pers_tab[i].name;

            Person toFind = {0};
            toFind.parent = pers_tab[i].name;

            Person *returnValue = bsearch(&toFind, pers_tab, size, sizeof(Person), cmp_person);

            if(returnValue == NULL)
                continue;

            int index = (int)(returnValue-pers_tab);
            int firstIndex = index, lastIndex = index;

            /*while(firstIndex >= 0 && cmp_person(&pers_tab[index], &pers_tab[firstIndex]) == 0)
                firstIndex--;

            while(lastIndex < size && cmp_person(&pers_tab[index], &pers_tab[lastIndex]) == 0)
                lastIndex++;*/

            while(firstIndex > 0 && cmp_person(&pers_tab[firstIndex], &pers_tab[firstIndex-1]) == 0)
                firstIndex--;

            while(lastIndex < size-1 && cmp_person(&pers_tab[lastIndex], &pers_tab[lastIndex+1]) == 0)
               lastIndex++;

            idx_tab[parentsCount].indexFirst = firstIndex;
            idx_tab[parentsCount].indexLast = lastIndex;

            parentsCount++;
        }
    }
    qsort(idx_tab, parentsCount, sizeof(Parent), cmp_parent);
    return parentsCount;
}

void persons_shifts(Person *person_tab, const int size, Parent *idx_tab, const int no_parents)
{
    int i = 0;
    Person tmp[33];
    while(i < size)
    {
        Parent toFind = {0};
        toFind.par_name = person_tab[i].name;

        Parent *returnValue = bsearch(&toFind, idx_tab, no_parents, sizeof(Parent), cmp_parent);

        if(returnValue != NULL)
        {
            int toCopy = (returnValue->indexLast - returnValue->indexFirst) + 1;

            memmove(tmp, &person_tab[returnValue->indexFirst], toCopy*sizeof(Person));
            memmove(&person_tab[i+toCopy+1], &person_tab[i+1], ((returnValue->indexFirst) - i - 1) * sizeof(Person));
            memmove(&person_tab[i+1], tmp, toCopy*sizeof(Person));

            for(int j = 0; j < no_parents; j++)
            {
                if(idx_tab[j].indexFirst < returnValue->indexFirst)
                {
                    idx_tab[j].indexFirst += toCopy;
                    idx_tab[j].indexLast += toCopy;
                }
            }
        }
        i++;
    }
}

int cleaning(Person *person_tab, int n)
{
    for(int i = 0; i < n; i++)
    {
        if(person_tab[i].bits.in_line == no)
        {
            memmove(&person_tab[i], &person_tab[i+1], (n-i)*sizeof(Person));
            i--;
            n--;
        }
    }
    return n;
}

void print_person(const Person *p) {
	printf("%s\n", p->name);
}
int compare(const void* a, const void* b)
{
    Person* first = (Person *)a;
    Person* second = (Person *)b;

    if(first->parent == NULL)
        return -1;
    if(second->parent == NULL)
        return 1;

    int parent = strcmp(first->parent, second->parent);

    if(parent < 0)
    {
        return -1;
    }
    else if(parent > 0)
    {
        return 1;
    }
    else
    {
        if(cmp_date(&first->born, &primo_date) < 0 && cmp_date(&second->born, &primo_date) < 0)
        {
            if(first->bits.sex == M && second->bits.sex == F)
                return -1;
            else if(second->bits.sex == M && first->bits.sex == F)
                return 1;
        }

        int born_res = cmp_date(&first->born, &second->born);

        if(born_res < 0)
            return -1;
        else if(born_res > 0)
            return 1;
        else
        {
            if(first->bits.sex > second->bits.sex)
                return 1;
            else if(first->bits.sex < second->bits.sex)
                return -1;
            else
                return 0;
        }
    }

}
int create_list(Person *person_tab, int n)
{
    qsort(person_tab, n, sizeof(Person), compare);

    Parent parent_tab[33];

    int no_parents = fill_indices_tab(parent_tab, person_tab, n);

    persons_shifts(person_tab, n, parent_tab, no_parents);

    return cleaning(person_tab, n);
}

int main(void) {
	Person person_tab[] = {
		{"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
		{"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
		{"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
		{"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
		{"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
		{"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
		{"William", {M,yes}, {21, 6, 1982}, "Charles III"},
		{"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
		{"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
		{"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
		{"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
		{"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
		{"James", {M,yes}, {17, 12, 2007}, "Edward"},
		{"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
		{"Charles", {M,yes}, {1, 7, 1999}, "David"},
		{"Margarita", {F,yes}, {14, 5, 2002}, "David"},
		{"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
		{"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
		{"George", {M,yes}, {22, 7, 2013}, "William"},
		{"George VI", {M,no}, {14, 12, 1895}, NULL},
		{"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
		{"Louis", {M,yes}, {23, 4, 2018}, "William"},
		{"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
		{"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
		{"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
		{"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
		{"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
		{"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
		{"Elizabeth II", {F,no}, {21, 4, 1926}, "George VI"},
		{"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
		{"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
		{"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
		{"August", {M,yes}, {9, 2, 2021}, "Eugenie"},
		{"Ernest", {M,yes}, {30, 5, 2023}, "Eugenie"}
	};

	int to_do, no;
	int n;
	Food food_tab[FOOD_MAX];
	char buff[RECORD_MAX];
	scanf("%d", &to_do);

	switch (to_do) {
		case 1:  // bsearch2
			scanf("%d", &no);
			n = read_goods(food_tab, FOOD_MAX, no, stdin, 1);
			scanf("%s",buff);
			print_art(food_tab, n, buff);
			break;
		case 2: // qsort
			scanf("%d", &no);
			n = read_goods(food_tab, FOOD_MAX, no, stdin, 0);
			Date curr_date = {0};
			int days;
			scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
			scanf("%d", &days);
			printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
			break;
		case 3: // succession
			scanf("%d", &no);
			const int no_persons = sizeof(person_tab) / sizeof(Person);
			create_list(person_tab,no_persons);
			print_person(person_tab + no - 1);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}

