#include "const.h"
#include "basic.h"
#include "manip.h"
#include "parser.h"

/* 
 * TODO
 * Implement op list (see line 12)
 * Might require changing inbrack and get_op (line 46 and 94)
 * 
 */

int main(int argc, char** argv) {

    size_t *op_count;
    *op_count = 0;
    struct op **ops = (struct op *)malloc(sizeof(struct op *) * INIT_MAX);    
    size_t *eq_size;
    eq_size = (size_t *) malloc(sizeof(size_t));

    assert(eq_size);
    printf("Please enter equation: ");
    char* eq = get_string(eq_size);
    remove_space(eq);
    printf("%s\n", eq);

    /*
    struct token *r;
    short *check;
    printf("eq_size = %lu\n", *eq_size);
    */
    /*
    check = (short *) malloc((*eq_size) * sizeof(short));
    assert(check);
    memset(check, 0, *eq_size * sizeof(check));

    r = inbrack(eq, eq_size, check); 
    pr_substring(r -> open, r -> close);
    */
    /* struct op *new_op = get_op(eq, eq + (*eq_size)); */
    exit(EXIT_SUCCESS);
	
}

/* Function to retrieve the innermost bracket
 * Returns struct with pointer to opening bracket and closing bracket
 * Returns NULL if not brackets found. */
struct token *inbrack(char* eq, size_t *eq_size, short *check, size_t *op_count) {
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
            memset((check + *(r -> open)), *op_count,
                    (size_t)((r -> close) - (r -> open))); 
            *op_count++;
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



/* sets the result in an op struct */
void set_result(struct op *s) {
    s -> result = (s -> sym)(s -> a, s -> b);
    return;
}


/* returns pointer to op */
struct op *get_op(char *start, char *end) {
    struct op *eq = (struct op *) malloc(sizeof(struct op));
    char *p;
    p = start;
    while (p != end + 1) {
        if (*p == POWR) {
            set_op(eq, powe, start, p, end);
            return (eq);
        }
        p++;
    }
    p = start;
    while (p != end + 1) { 
        if (*p == DIVI) {
            set_op(eq, divd, start, p, end);
            return (eq);
        } else if (*p == MULT) {
            set_op(eq, mul, start, p, end);
            return (eq);
        }
        p++;
    } 
    p = start;
    while (p != end + 1) {
        if (*p == PLUS) {
            set_op(eq, add, start, p, end);
            return (eq);
        } else if (*p == DIFF) {
            set_op(eq, sub, start, p, end);
            return (eq);
        }
        p++;
    }  
    return eq;
}

void set_op(struct op *eq, double(*func)(double, double), char *start, char *p, char *end) {  
    eq -> sym = func;
    eq -> a = strtod(start, &p);
    eq -> b = strtod(p + 1, &end);
    eq -> result = (eq -> sym)(eq -> a, eq -> b);
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
