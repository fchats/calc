#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <assert.h>
#include <string.h>

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

struct op {
    double a;
    double b;
    double (*sym)(double, double);
    double result;
    struct op *next;
    struct op *prev;
};

/* Function Prototypes */
void remove_space(char* string);
char* get_string(size_t *size);
void shift_left(char* string);
struct token *inbrack(char* eq, size_t *eq_size, short *check);
void pr_substring(char *start, char *end); 
void set_result(struct op *s);

/* Basic Operator Functions for function pointers */
double add(double a, double b);
double sub(double a, double b);
double divd(double a, double b);
double mul(double a, double b);

/* Debug functions */
#if (DEBUG)
void substring(char *start, char *end);
#endif

int main(int argc, char** argv) {

    size_t *eq_size;
    eq_size = (size_t *) malloc(sizeof(size_t));

    assert(eq_size);
    printf("Please enter equation: ");
    char* eq = get_string(eq_size);
    remove_space(eq);
    printf("%s\n", eq);
    struct token *r;
    short *check;
    printf("eq_size = %lu\n", *eq_size);
    check = (short *) malloc((*eq_size) * sizeof(short));
    assert(check);
    memset(check, 0, *eq_size * sizeof(check));

    r = inbrack(eq, eq_size, check); 
    pr_substring(r -> open, r -> close);
    exit(EXIT_SUCCESS);
	
}

/* Function to retrieve the innermost bracket
 * Returns struct with pointer to opening bracket and closing bracket
 * Returns NULL if not brackets found. */
struct token *inbrack(char* eq, size_t *eq_size, short *check) {
    struct token *r = (struct token *)malloc(sizeof(struct token));
    assert(r);
    r -> open = (char*) malloc(sizeof(char));
    assert(r -> open);
    r -> close = (char*) malloc(sizeof(char));
    assert(r -> close);
    char *p;
    p = eq;
    while (NEND(*p)) {
        if (*p == OPEN && !check[p - eq]) {
            r -> open = p;
            p++;
            continue;
        } 
        p++;
    }

    
    if (r -> open) {
        p = r -> open;
    }
    while (NEND(*p)) {
        if ((r -> open) && (*p == CLOSE) && !(check[p - eq])) {
            r -> close = p;
            check[(r -> open) - eq] = 1;
            check[p - eq] = 1;
            return r;
        }
        p++;
    }
    if (r -> open && !(r -> close)) {
        printf("BRACKET MISMATCH\n");
        exit(EXIT_FAILURE);
    } else {
        return NULL;
    }


}

/* Simple function to retrieve a string from stdin using getchar()*/
char* get_string(size_t *size) {
    int i;
    size_t* max = (size_t *)malloc(sizeof(int));
    *max = INIT_MAX;
    char* str;
    str = (char*)malloc(sizeof(char) * (*max));
    *size = 0;
    char input;
    for (i = 0; (input = getchar()) != '\n' /*|| input != EOF*/; i++) {
        *(str + i) = input;
        (*size)++;
        #if (DEBUG)
        //printf("%lu\n", *size);
        #endif
        if (*size >= *max) {
            *max = 2 * (*max);
            str = realloc(str, *max);
            #if (DEBUG)
            printf("doubled size\n");
            #endif
        }
    }
    //printf("%lu\n", *size);
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


double divd(double a, double b) {
    return (a / b);
}


void set_result(struct op *s) {
    s -> result = s -> sym(s -> a, s -> b);
    return;
}
/* Debug Functions */
#if (DEBUG)
/* print substring */
void pr_substring(char *start, char *end) {
    char *p;
    p = start;
    short dir = (end - start) / abs(end - start);
    while (p != end) {
        printf("%c", *p);
        p = p + dir;
    }
    printf("%c\n", *end);
    return;
}
#endif
