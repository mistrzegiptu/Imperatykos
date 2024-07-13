#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING_SINGLE 4
#define IN_STRING_DOUBLE 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024
#define MAX_KEYWORDS 32

int index_cmp(const void*, const void*);
int isUnical(char* str, int maxIndex);
int isKeyword(char* str);
int isident(char c);
int isidentstart(char c);
int cmp(const void*, const void*);

char identifiers[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};
int isident(char c)
{
    return isalnum(c) | (c == '_') | (c == '/');
}
int isidentstart(char c)
{
    return isalpha(c) | (c == '_') | (c == '/');
}
int isUnical(char* str, int maxIndex)
{
    for(int i = 0; i < maxIndex; i++)
    {
        if(strcmp(str, identifiers[i]) == 0)
            return 1;
    }
    return 0;
}
int isKeyword(char* str)
{
    for(int i = 0; i < MAX_KEYWORDS; i++)
    {
        if(strcmp(str, keywords[i])==0)
            return 1;
    }
    return 0;
}
int find_idents()
{
    char c = fgetc(stdin);
    char str[MAX_ID_LEN] = {0};
    int flag = 0, currentIndex = 0;

    while(c != EOF)
    {
        if(flag == 0)
        {
           if(c == '/')
           {
               c = fgetc(stdin);
               if(c == '/')
               {
                   flag = IN_LINE_COMMENT;
                   continue;
               }
               else if(c == '*')
                   flag = IN_BLOCK_COMMENT;
           }
           else if(c == '"')
               flag = IN_STRING_DOUBLE;
           else if(c == '\'')
               flag = IN_STRING_SINGLE;
           else if(c == '\\')
           {
               fgetc(stdin);
               c = fgetc(stdin);
           }

           int i = 0;
           if(isidentstart(c))
           {
               str[i] = c;
               i++;
               c = fgetc(stdin);

               while(isident(c))
               {
                   str[i] = c;
                   i++;
                   c = fgetc(stdin);
               }
           }
           else if(isdigit(c))
           {
               while(isident(c))
                   c = fgetc(stdin);
           }

           str[i] = '\0';
           if(strlen(str) > 0 && isKeyword(str) == 0 && isUnical(str, currentIndex) == 0)
           {
               strcpy(identifiers[currentIndex], str);
               currentIndex++;
               continue;
           }
        }
        else
        {
            if(c == '\\')
            {
                fgetc(stdin);
            }
            else if(flag == IN_LINE_COMMENT && (c == '\n'))
                flag = 0;
            else if(flag == IN_BLOCK_COMMENT && (c == '*'))
            {
                c = fgetc(stdin);
                if(c == '/')
                    flag = 0;
            }
            else if(flag == IN_STRING_DOUBLE && (c == '"'))
                flag = 0;
            else if(flag == IN_STRING_SINGLE && (c == '\''))
                flag = 0;
        }
        c = fgetc(stdin);
    }
    /*for(int j = 0; j < currentIndex; j++)
        puts(identifiers[j]);*/
    return currentIndex;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(identifiers[a], identifiers[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}

