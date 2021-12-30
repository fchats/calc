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

    struct op_list *ops = (struct op_list *) malloc(sizeof(struct op_list));
    assert(ops);
    ops -> op_count = 0;
    ops -> max_ops = INIT_MAX;
    ops -> eqs = (struct op **) malloc(sizeof(struct op *) * (ops -> max_ops));
    

    size_t *eq_size;
    eq_size = (size_t *) malloc(sizeof(size_t));
    assert(eq_size);
    printf("Please enter equation: ");
    char* eq = get_string(eq_size);
    remove_space(eq);
    printf("%s\n", eq); 
    exit(EXIT_SUCCESS);
	
}

/* Function to retrieve the innermost bracket
 * Returns struct with pointer to opening bracket and closing bracket
 * Returns NULL if not brackets found. */
struct token *inbrack(char* eq, size_t *eq_size, size_t *check, struct op_list *ops) {
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
            memset((check + *(r -> open)), ops -> op_count,
                    (size_t)((r -> close) - (r -> open))); 
            (ops -> op_count)++;
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
/* Need to pass check down to set_op and use to check if already parsed,
 * then call inbrack as well during one of these two functions.
 * */
struct op *get_op(char *start, char *end, struct op_list *ops, size_t *check) {
    struct op *eq = (struct op *) malloc(sizeof(struct op));
    char *p;
    p = start;
    while (p != end + 1) {
        if (*p == POWR) {

            set_op(eq, powe, start, p, end, check);
            return (eq);
        }
        p++;
    }
    p = start;
    while (p != end + 1) { 
        if (*p == DIVI) {
            set_op(eq, divd, start, p, end, check);
            return (eq);
        } else if (*p == MULT) {
            set_op(eq, mul, start, p, end, check);
            return (eq);
        }
        p++;
    } 
    p = start;
    while (p != end + 1) {
        if (*p == PLUS) {
            set_op(eq, add, start, p, end, check);
            return (eq);
        } else if (*p == DIFF) {
            set_op(eq, sub, start, p, end, check);
            return (eq);
        }
        p++;
    }  
    return eq;
}

void set_op(struct op *eq, double(*func)(double, double), char *start, char *p,
            char *end, struct op_list *ops, size_t *check) {  
    
    
    eq -> sym = func; 
   
    eq -> a = strtod(start, &p);
    eq -> b = strtod(p + 1, &end);
    eq -> result = (eq -> sym)(eq -> a, eq -> b);
    return;
}

void add_op(struct op *eq, struct op_list *ops) {
    if (ops -> op_count == ops -> max_ops) {
        ops -> max_ops = INCREASE * ops -> max_ops;
        ops -> eqs = (struct op **) realloc(ops -> eqs, sizeof(struct op *) * ops -> max_ops);
    }
    *(ops -> eqs + ops -> op_count) = eq;
    ops -> op_count ++;
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
