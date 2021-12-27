#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

/* Toggle for optional terminal messages for debug */
#define DEBUG 1

/* Symbols to parse */
#define PLUS  '+'
#define MULT  '*'
#define DIFF  '-'
#define DIVI  '/'

#define OPEN  '('
#define CLOSE ')'

#define POWR  '^'
#define MOD   '%'

/* Initial max string Length */
#define INIT_MAX 100


#define NL printf("\n");
#define NEND(a) (a != '\0' && a != EOF)


struct token {
    char* open;
    char* close;
};

/* Function Prototypes */
void remove_space(char* string);
char* get_string(size_t *size);
char** token_eq(char* eq);
void shift_left(char* string);


/* Basic Operator Functions for function pointers */
double add(double a, double b);
double sub(double a, double b);
/* double div(double a, double b); */
double mul(double a, double b);

/* Debug functions */
#if (DEBUG)
void substring(char *start, char *end);
#endif

int main(int argc, char** argv) {

    size_t eq_size;
    printf("Please enter equation: ");
    char* eq = get_string(&eq_size);
    printf("%s\n", eq);
    struct token *r;
    r = inbrack(eq);
    pr_substring(r -> open, r -> close);
    exit(EXIT_SUCCESS);
	
}

char** token_eq(char* eq) {
    char *curr = eq;
    struct token **tokens;
    while (*curr != '\0' || *curr != EOF) {
        
    } 
}

/* Function to retrieve the innermost bracket
 * Returns pointer to start */
struct token *inbrack(char* eq) {
    struct token *r = (struct token *)malloc(sizeof(struct token));
    r -> open = (char*) malloc(sizeof(char));
    r -> close = (char*) malloc(sizeof(char));
    r -> open = eq;
    while (NEND(*(r -> open))) {
        if (*(r -> open) = OPEN) {
            r -> close = (r -> open) + 1;
            while (NEND(*(r -> close))) {
                if (*(r -> close) == CLOSE) {
                    return(r);
                } else {
                    (r -> close)++;
                } 
            }
        }
    }
    printf("NO CLOSING BRACKETS\n");
    exit(EXIT_FAILURE);
}

char* get_string(size_t *size) {
    int i;
    size = (size_t *)malloc(sizeof(size_t));
    size_t* max = (size_t *)malloc(sizeof(int));
    *max = INIT_MAX;
    char* str;
    str = (char*)malloc(sizeof(char) * (*max));
    *size = 0;
    char input;
    for (i = 0; (input = getchar()) != '\n' /*|| input != EOF*/; i++) {
        *(str + i) = input;
        (*size)++;
        if (*size >= *max) {
            *max = 2 * (*max);
            str = realloc(str, *max);
            #if (DEBUG)
            printf("doubled size\n");
            #endif
        }
    }
    return(str);
}

void remove_space(char* string) {
    char *p;
    p = string;
    while (*p != '\0') {
        if ((*p) == ' ') {
            shift_left(p);
        }
        p++;
    }
}

void shift_left(char* string) {

    char* p = string + 1;
    while (*p != '\0') {
        *(p - 1) = *p;  
        p++;
    }
    *(p - 1) = '\0';
    return;
}

/* Basic Operator Functions for function pointers */

double add(double a, double b) {
    return (a + b);
}

double sub (double a, double b) {
    return (a - b);
}

double mul(double a, double b) {
    return (a * b);
}

/*
double div(double a, double b) {
    return (a / b);
}
*/

/* Debug Functions */
#if (DEBUG)
/* print substring */
void pr_substing(char *start, char *end) {
    char *p;
    p = start;
    dir = (end - start) / abs(end - start);
    while (p != end) {
        printf("%c", *p);
        p = p + dir;
    }
    printf("%c\n", *end);
    return;
}
#endif
