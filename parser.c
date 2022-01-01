#include "parser.h"
#include "manip.h"
#include "const.h"
#include "basic.h"

int main() {

    size_t size = 0;
    char* str;
    str = get_string(&size);
    remove_space(str, &size);

    struct op *te;
    te = op_str(str);
    pr_op(te);
    
}

struct op *op_str(char *str) {

    struct op *re = (struct op *) malloc(sizeof(struct op));
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

    while (*p != '\0') {
        if (*p == DIVI) {
            re -> sym = divd;
            #if DEBUG
            re -> sym_n = (char*) malloc(sizeof(char) * 4);
            re -> sym_n = "divd";
            #endif
            s = p;
            break;
        } else if (*p == MULT) {
            re -> sym = mul;
            #if DEBUG
            re -> sym_n = (char*) malloc(sizeof(char) * 3);
            re -> sym_n = "mul";
            #endif
            s = p;
            break;
        } 
        p++;
    }
    p = str;
    while (*p != '\0') {
        if (*p == PLUS) {
            re -> sym = add;
            #if DEBUG
            re -> sym_n = (char *) malloc(sizeof(char) * 3);
            re -> sym_n = "add";
            #endif
            s = p;
            break;
        } else if (*p == DIFF) {
            re -> sym = sub;
            #if DEBUG
            re -> sym_n = (char *) malloc(sizeof(char) * 3);
            re -> sym_n = "sub";
            #endif
            s = p;
            break;
        }
        p++;
    }

    /* Get left number (a) 
     * move p back from symbol until start. */
    p = s - 1;
    char *a_start, *a_end;
    a_end = s - 1;
    /* Will have to check for closing bracket here but will implement later */
    while (p != str && (isdigit(*p) || *p == '.')) {
        p--;
    }
    a_start = p;
    printf("a_start -> a_end = ");
    pr_substring(a_start, a_end);
    re -> a = strtod(a_start, &a_end);    
    
    /* Get right number (b) 
     * move p forward from symbol until end. */
    p = s + 1;
    char *b_start, *b_end;
    b_start = s + 1;
    /* Will have to check for opening bracket here but will implement later */
    while (p != str && (isdigit(*p) || *p == '.')) {
        p++;
    }
    b_end = p;
    printf("b_start -> b_end = ");
    pr_substring(b_start, b_end);
    re -> b = strtod(b_start, &b_end);

    /* Set op result */
    re -> result = (re -> sym)(re -> a, re -> b);

    return(re);
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
    printf("func_n: %s\n", eq -> sym_n);
    printf("result: %10lf\n", eq -> result);
    NL();
}
#endif
