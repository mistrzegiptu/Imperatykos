#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
    int va = *(char*)a-FIRST_CHAR;
    int vb = *(char*)b-FIRST_CHAR;
    if (count[va] == count[vb])
        return va - vb;
    return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    // sort according to second char if counts and the first char equal
    if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
    // sort according to first char if counts equal
    if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
    return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc)
{
    (*nl) = 0;
    (*nw) = 0;
    (*nc) = 0;
    int flag = 0;

    char c = fgetc(stdin);

    while(c != EOF)
    {
        if(c == NEWLINE)
            (*nl) += 1;
        if(flag == 0 && c >= FIRST_CHAR && c <= LAST_CHAR)
        {
            flag = 1;
            (*nw) += 1;
        }
        if(flag == IN_WORD && (c == '\t' || c == ' ' || c == '\n'))
            flag = 0;

        (*nc) += 1;
        c = fgetc(stdin);
    }
}
void char_count(int char_no, int *n_char, int *cnt)
{
    char charTab[MAX_CHARS] = {0};

    for(int i = 0; i < MAX_CHARS; i++)
        charTab[i] = FIRST_CHAR + i;
    char c = fgetc(stdin);
    while(c != EOF)
    {
        if(c >= FIRST_CHAR && c <= LAST_CHAR)
        {
            count[(c - FIRST_CHAR)] += 1;
        }
        c = fgetc(stdin);
    }
    qsort(charTab, MAX_CHARS, sizeof(char), cmp);
    (*n_char) = charTab[char_no-1];
    (*cnt) = count[(*n_char)-FIRST_CHAR];
}

void bigram_count(int bigram_no, int bigram[])
{
    int bigrams[MAX_BIGRAMS];
    for(int i = 0; i < MAX_CHARS; i++)
        for (int j = 0; j < MAX_CHARS; j++)
            bigrams[i*MAX_CHARS+j] = i*MAX_CHARS+j;

    char c = fgetc(stdin);
    char nextc = fgetc(stdin);

    while(nextc != EOF)
    {
        if(c >= FIRST_CHAR && c < LAST_CHAR && nextc >= FIRST_CHAR && nextc < LAST_CHAR)
            count[(int)(c-FIRST_CHAR)*MAX_CHARS+(int)(nextc-FIRST_CHAR)]++;

        c = nextc;
        nextc = fgetc(stdin);
    }
    qsort(bigrams, MAX_BIGRAMS, sizeof(int), cmp_di);

    int result = bigrams[bigram_no-1];
    bigram[0] = result / MAX_CHARS + FIRST_CHAR;
    bigram[1] = result % MAX_CHARS + FIRST_CHAR;
    bigram[2] = count[bigrams[bigram_no-1]];
}

void find_comments(int *line_comment_counter, int *block_comment_counter)
{
    (*line_comment_counter) = 0;
    (*block_comment_counter) = 0;
    char c = fgetc(stdin);
    char nextc = fgetc(stdin);
    int flag = 0;

    while(nextc != EOF)
    {
        if(flag == 0)
        {
            if(c == '/' && nextc == '/')
            {
                flag = IN_LINE_COMMENT;
                (*line_comment_counter) += 1;
            }
            else if(c == '/' && nextc == '*')
            {
                flag = IN_BLOCK_COMMENT;
                (*block_comment_counter) += 1;
            }
            if(flag != 0)
            {
                c = nextc;
                nextc = fgetc(stdin);
            }
        }
        else if(flag == IN_LINE_COMMENT)
        {
            if(c == NEWLINE || nextc == NEWLINE)
                flag = 0;

        }
        else if(flag == IN_BLOCK_COMMENT)
        {
            if(c == '*' && nextc == '/')
                flag = 0;
        }
        c = nextc;
        nextc = fgetc(stdin);
    }
}

#define MAX_LINE 128

int read_int() {
    char line[MAX_LINE];
    fgets(line, MAX_LINE, stdin); // to get the whole line
    return (int)strtol(line, NULL, 10);
}

int main(void) {
    int to_do;
    int nl, nw, nc, char_no, n_char, cnt;
    int line_comment_counter, block_comment_counter;
    int bigram[3];

    to_do = read_int();
    switch (to_do) {
        case 1: // wc()
            wc(&nl, &nw, &nc);
            printf("%d %d %d\n", nl, nw, nc);
            break;
        case 2: // char_count()
            char_no = read_int();
            char_count(char_no, &n_char, &cnt);
            printf("%c %d\n", n_char, cnt);
            break;
        case 3: // bigram_count()
            char_no = read_int();
            bigram_count(char_no, bigram);
            printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
            break;
        case 4:
            find_comments(&line_comment_counter, &block_comment_counter);
            printf("%d %d\n", block_comment_counter, line_comment_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}