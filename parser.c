#include "parser.h"

int main() {

    /* handle input */
    size_t size = 0;
    char* str;
    printf("Please enter an equation: \n");
    str = get_string(&size);
    remove_space(str, &size);

    /* tests for the op struct */
    //struct op *te;

    /* tests for the op_list struct */
    struct op_list *ops = new_op_list(str, size);

    /*
    while (in(-1, ops -> check, ops -> str_size)) {
        te = op_str(ops -> str, get_end(ops -> str), ops);
        add_op(ops, te);
    }
    pr_op_list(ops); 
    printf("%s = %lf\n", ops -> str, (ops -> eqs)[ops -> op_count - 1] -> result);
    */
    /* test inbrack */
    /*
    struct token *test;
    test = inbrack(ops);
    pr_substring(test -> open, test -> close);
    */
    eval(ops);
    #if DEBUG
    pr_op_list(ops);
    #endif
}

void eval(struct op_list *ops) {
    struct op *te;
    struct token *to;
    while(in(-1, ops -> check, ops -> str_size)) {
        if ((to = inbrack(ops))) {
            te = op_str(to -> open, to -> close, ops);
            
        } else {
            te = op_str(ops -> str, get_end(ops -> str), ops);
        }
        add_op(ops, te);
    }
}
struct token *inbrack(struct op_list *ops) {
    /* Checking for brackets */
    
    char *open, *close;
    struct token *t;
    t = (struct token *) malloc(sizeof(struct token));
    t -> open = NULL;
    t -> close = NULL;
        
    for (open = ops -> str; open != get_end(ops -> str) && *open != EOF; open++) {
        printf("%c ", *open);
        if (*open == OPEN && (ops -> check)[open - (ops -> str)] == -1) {
            for (close = open + 1; *close != '\0' && *close != EOF; close++) { 
                #if DEBUG
                printf("%c ", *close);
                #endif
                if (*close == OPEN && (ops -> check)[close - (ops -> str)] == -1) {
                    open = close; 
                } else if (*close == CLOSE && (ops -> check)[close - (ops -> str)] == -1) {
                    t -> open = open;
                    t -> close = close;
                    return t;
                }    
            }
        NL();   
        }
    } 
    t = NULL;
    return t;
}
/* add op to op_list 
 * function assumes that both op_list and op 
 * have been malloced */
void add_op(struct op_list *ops, struct op *eq) {

    /* Increase size if op_list too long */
    if (ops -> max_ops == ops -> op_count) {
        ops -> max_ops = ops -> max_ops * INCREASE;
        ops -> eqs = (struct op **) realloc(ops -> eqs, ops -> max_ops);
    }
    (ops -> eqs)[ops -> op_count] = eq;
    char *p;
    p = eq -> op_start;
    while (p != eq -> op_end + 1) {
        (ops -> check)[p - (ops -> str)] = ops -> op_count;
        printf("Adding %lu in check[%ld]\n", ops -> op_count, p - (ops -> str));
        p++;
    }
    eq -> op_num = ops -> op_count;
    ops -> op_count++;
    return;
}

/* Allocates memory for new op list */
struct op_list *new_op_list(char *str, size_t str_size) {
    struct op_list *ops = (struct op_list *) malloc(sizeof(struct op_list));
    assert(ops);
    ops -> op_count = 0;
    ops -> max_ops = INIT_MAX;
    ops -> eqs = (struct op **) malloc(sizeof(struct op *) * INIT_MAX);
    assert(ops -> eqs);
    ops -> str = str;
    ops -> str_size = str_size;
    ops -> check = (int *) malloc(sizeof(size_t) * str_size);
    assert(ops -> check);
    /* init check to {-1} */
    size_t i = 0;
    while (i < str_size) {
        *(ops -> check + i) = (-1);
        i++;
    }
    #if DEBUG
    printf("            ops:     %p\n", ops);
    printf("ops -> op_count:     %lu\n", ops -> op_count);
    printf(" ops -> max_ops:     %lu\n", ops -> max_ops);
    printf("     ops -> eqs:     %p\n", ops -> eqs);
    printf("     ops -> str:     %s\n", ops -> str);
    printf("ops -> str_size:     %lu\n", ops -> str_size);
    printf("   ops -> check:     %p\n", ops -> check);
    for (int i = 0; i < ops -> str_size; i++) {
        printf("%d", ops -> check[i]);
    }
    NL();
    #endif

    return(ops);
}

/* Function to create op struct from given string */

struct op *op_str(char *str, char *end, struct op_list *ops) {

    struct op *re = (struct op *) malloc(sizeof(struct op));
    assert(re);
        re -> a      = 0;
        re -> b      = 0;
        re -> sym    = NULL;
        #if DEBUG
        re -> sym_n  = NULL;
        #endif
        re -> result = 0;

    /* Get operation symbol */
    char *p, *s;
    p = str;
    unsigned short found = 0;

    while (p != end && !found) {
        if (*p == DIVI && ops -> check[p - str] == -1) {
            re -> sym = divd;
            #if DEBUG
            re -> sym_n = (char*) malloc(sizeof(char) * 4);
            assert(re -> sym_n);
            re -> sym_n = "divd";
            #endif
            s = p;
            found = 1;
            break;
        } else if (*p == MULT && ops -> check[p - str] == -1) {
            re -> sym = mul;
            #if DEBUG
            re -> sym_n = (char*) malloc(sizeof(char) * 3);
            assert(re -> sym_n);
            re -> sym_n = "mul";
            #endif
            s = p;
            found = 1;
            break;
        } 
        p++;
    }

    p = str;
    while (p != end && !found) {
        if (*p == PLUS && ops -> check[p - str] == -1) {
            re -> sym = add;
            #if DEBUG
            re -> sym_n = (char *) malloc(sizeof(char) * 3);
            assert(re -> sym_n);
            re -> sym_n = "add";
            #endif
            found = 1;
            s = p;
            break;
        } else if (*p == DIFF && ops -> check[p - str] == -1) {
            re -> sym = sub;
            #if DEBUG
            re -> sym_n = (char *) malloc(sizeof(char) * 3);
            assert(re -> sym_n);
            re -> sym_n = "sub";
            #endif
            s = p;
            found = 1;
            break;
        }
        p++;
    }

    if (!found) {
        fprintf(stderr, "NO OPERATION FOUND\n");
        exit(EXIT_FAILURE);
    }
    /* Get left number (a) 
     * move p back from symbol until start. */
    if (ops -> check[(s - 1) - str] != -1) {
        /* Absolutely scuffed */
        if (*(s - 1) == CLOSE) {

            re -> a = (ops -> eqs)[ops -> check[s - 1 - str]] -> result;
            re -> op_start = (ops -> eqs)[ops -> check[s - 1 - str]] -> op_start;
        } else {
            re -> a = (ops -> eqs)[ops -> check[s - 1 - str]] -> result;
            re -> op_start = (ops -> eqs)[ops -> check[s - str]] -> op_start;
            
        }
    } else {

        p = s - 1;
        char *a_start, *a_end;
        a_end = s - 1;
        /* Will have to check for closing bracket here but will implement later */
        while (p != str && (isdigit(*(p - 1)) || *(p - 1) == '.')) {
            p--;
        }
        a_start = p;
        //printf("a_start -> a_end = ");
        //pr_substring(a_start, a_end);
        re -> a = strtod(a_start, &a_end);    
        if (isdigit(*a_start) && *(a_start - 1) == OPEN) {
            re -> op_start = a_start - 1;
        } else {    
            re -> op_start = a_start;
        }
        
    }
    /* Get right number (b) 
     * move p forward from symbol until end. */
    if (ops -> check[(s + 1) - str] != -1) {
        re -> b = (ops -> eqs)[ops -> check[(s + 1) - str]] -> result;
        re -> op_end = (ops -> eqs)[ops -> check[(s + 1) - str]] -> op_end;
    } else { 
        p = s + 1;
        char *b_start, *b_end;
        b_start = s + 1;
        /* Will have to check for opening bracket here but will implement later */
        while (p != end && (isdigit(*(p + 1)) || *(p + 1) == '.')) {
            p++;
        }
        b_end = p - 1;
        printf("b_start -> b_end = ");
        //pr_substring(b_start, b_end);
        re -> b = strtod(b_start, &b_end);
        re -> op_end = b_end;
    }

    /* Set op result */
    re -> result = (re -> sym)(re -> a, re -> b);
    return(re);
}

short in(int val, int *list, size_t size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("Checking check[%d] == %d\n", i, val);
        if (list[i] == val) {
            return 1;
        }
    }
    return(0);
}

/* Debug Functions */
#if (DEBUG)
/* print substring */
void pr_substring(char *start, char *end) {
    char *p;
    p = start;
    while (p != end) {
        printf("%c", *p);
        p++;
    }
    printf("%c\n", *end);
    return;
}

void pr_op_list(struct op_list *ops) {
    int i; 
    printf("OP_LIST:\n");
    printf("            ops:     %p\n", ops);
    printf("ops -> op_count:     %lu\n", ops -> op_count);
    printf(" ops -> max_ops:     %lu\n", ops -> max_ops);
    printf("     ops -> eqs:     %p\n", ops -> eqs);
    printf("     ops -> str:     %s\n", ops -> str);
    printf("ops -> str_size:     %lu\n", ops -> str_size);
    printf("   ops -> check:     %p\n", ops -> check); 
    for (i = 0; i < ops -> str_size; i++) {
        printf("%d", ops -> check[i]);
    }
    NL();
    printf("OPERSTIONS:\n");
    for (i = 0; i < (ops -> op_count); i++) {
        pr_op((ops -> eqs)[i]);
    }

}
void pr_op(struct op *eq) {
    //pr_substring(eq -> op_start, eq -> op_end);
    printf("  op_num: %10lu\n", eq -> op_num);
    printf("       a: %10lf\n", eq -> a);
    printf("       b: %10lf\n", eq -> b);
    printf("    func: %p\n", eq -> sym);
    printf("  func_n: %s\n", eq -> sym_n);
    printf("  result: %10lf\n", eq -> result);
    printf("op_start: %c\n", *(eq -> op_start));
    printf("  op_end: %c\n", *(eq -> op_end));
    NL();
}
#endif
