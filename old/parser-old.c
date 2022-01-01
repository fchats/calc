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
    

    size_t eq_size;
    printf("Please enter equation: ");
    char* eq = get_string(&eq_size);
    NL();
    remove_space(eq, &eq_size);
    printf("%s\n", eq); 
    
    ops -> str = eq;
    ops -> eq_size = eq_size;
    ops -> check = (size_t *) malloc(sizeof(size_t) * eq_size);
    memset(ops -> check, 0, ops -> eq_size);
    assert(ops -> check);
    ops = eval(ops);
    pr_op_list(ops);
    exit(EXIT_SUCCESS);
	
}

/* Function to retrieve the innermost bracket
 * Returns struct with pointer to opening bracket and closing bracket
 * Returns NULL if not brackets found. */
struct token *inbrack(struct op_list *ops) {
    struct token *r = (struct token *)malloc(sizeof(struct token));
    assert(r);
    r -> open = (char*) malloc(sizeof(char));
    assert(r -> open);
    r -> close = (char*) malloc(sizeof(char));
    assert(r -> close);
    char *p;
    p = ops -> str;
    while (NEND(*p)) {
        if (*p == OPEN && !(ops -> check)[p - (ops -> str)]) {
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
        if ((r -> open) && (*p == CLOSE) && !((ops -> check)[p - (ops -> str)])) {
            r -> close = p;
            memset(((ops -> check) + *(r -> open)), ops -> op_count,
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
struct op *get_op(char *start, char *end, struct op_list *ops) {
    struct op *eq = (struct op *) malloc(sizeof(struct op));
    char *p;
    p = start;
    if (*start == DIFF) {
        struct op *ze = (struct op *) malloc(sizeof(struct op));
        ze -> a = 0;
        char* num_start = start + 1, *num_end = num_start;
        while (isdigit(*(num_end + 1)) || *(num_end + 1) == '.') {
            num_end++;
        }
        ze -> b = strtod(num_start, &num_end);
        ze -> sym = sub;
        ze -> result = (ze -> sym)(ze -> a, ze -> b);
        add_op(ze, ops);  
    }
    while (p != end) {
        if (*p == POWR) {

            set_op(eq, powe, start, p, end, ops);
            
            add_op(eq, ops);
            return (eq);
        }
        p++;
    }
    p = start;
    while (p != end) { 
        if (*p == DIVI) {
            set_op(eq, divd, start, p, end, ops);
            add_op(eq, ops);
            return (eq);
        } else if (*p == MULT) {
            set_op(eq, mul, start, p, end, ops);
            add_op(eq, ops);
            return (eq);
        }
        p++;
    } 
    p = start;
    while (p != end + 1) {
        if (*p == PLUS) {
            set_op(eq, add, start, p, end, ops);
            add_op(eq, ops);
            return (eq);
        } else if (*p == DIFF) {
            set_op(eq, sub, start, p, end, ops);
            add_op(eq, ops);
            return (eq);
        }
        p++;
    }  
    return eq;
}

void set_op(struct op *eq, double(*func)(double, double), char *start, char *p,
        char *end, struct op_list *ops) {  
    eq -> sym = func; 
    char *num_start = start, *num_end = num_start;

    ops -> op_count ++;
    if (*((ops -> check) + (num_start - ops -> str))) {
        eq -> a = (ops -> eqs)[num_start - (ops -> str)] -> result;

    } else {
        while ((isdigit(*(num_end + 1)) || *(num_end + 1) == '.') && num_end < p) {
            printf("Set: check[%ld] = %lud\n", (num_end - (ops -> str)), ops -> op_count);
            (ops -> check)[num_end - (ops -> str)] = ops -> op_count;
            num_end++;
        } 
        eq -> a = strtod(num_start, &num_end);
    }
    
    num_start = p + 1;
    num_end = num_start;

    if ((ops -> check)[end - start]) {
        eq -> b = (ops -> eqs)[end - start] -> result;
    } else {
        while ((isdigit(*num_end + 1) || *(num_end + 1) == '.') && num_end <= end) {
            (ops -> check)[num_end - (ops -> str)] = ops -> op_count;
            num_end++;
        } 
        eq -> b = strtod(num_start, &num_end);
    }
          
    eq -> result = (eq -> sym)(eq -> a, eq -> b);
    return;
}

void add_op(struct op *eq, struct op_list *ops) {
    if (ops -> op_count == ops -> max_ops) {
        ops -> max_ops = INCREASE * ops -> max_ops;
        ops -> eqs = (struct op **) realloc(ops -> eqs, sizeof(struct op *) * ops -> max_ops);
    }
    *(ops -> eqs + ops -> op_count) = eq;
    eq -> op_num = ops -> op_count;
    return;
}

struct op_list *eval(struct op_list *ops) {
    size_t t = 0;
    struct token *n = (struct token *) malloc(sizeof(struct token));
    struct op *n_op = (struct op *) malloc(sizeof(struct op));
    
    printf("%d\n", in(0, ops -> check, ops -> eq_size));

    while(in(t, ops -> check, ops -> eq_size)) {
        while ((n = inbrack(ops))) {
            n_op = get_op(n -> open, n -> close, ops);
            add_op(n_op, ops);
        }
        n_op = get_op(ops -> str, ops -> str + ops -> eq_size, ops);
        add_op(n_op, ops);
    }

    return ops;
}

int in(size_t val, size_t *list, size_t size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%lud", list[i]);
        if (val == list[i]) {
            return(1);
        }
    }
    NL();
    return(0);
}

struct op *new_op() {
    struct op *ne = (struct op *) malloc(sizeof(struct op));
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

void pr_op_list(struct op_list *ops) {
    int i;
    for (i = 0; i < (ops -> op_count); i++) {
        pr_op((ops -> eqs)[i]);
    }
}
void pr_op(struct op *eq) {
    printf("op_num: %10lud\n", eq -> op_num);
    printf("     a: %10lf\n", eq -> a);
    printf("     b: %10lf\n", eq -> b);
    printf("  func: %p\n", eq -> sym);
    printf("result: %10lf\n", eq -> result);
    NL();
}
#endif
