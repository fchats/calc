#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "const.h"
#include "manip.h"
#include "basic.h"

struct token {
    char* open;
    char* close;
};

struct op {
    size_t op_num;
    double a;
    double b;
    double (*sym)(double, double);
    #if DEBUG
    char *sym_n;
    #endif
    double result;
};

struct op_list {
    size_t op_count;
    size_t max_ops;
    struct op **eqs;
    char   *str;
    size_t eq_size;
    size_t *check;
};

/* Function Prototypes */
struct op *op_str(char *str);


/* Debug functions */
#if (DEBUG)
void substring(char *start, char *end);
void pr_op_list(struct op_list *ops);
void pr_op(struct op *eq);
#endif
