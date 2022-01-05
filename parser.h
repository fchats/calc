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
    double      a;
    double      b;
    double      (*sym)(double, double);
    #if DEBUG
    char        *sym_n;
    #endif
    double      result;
    char        *op_start;
    char        *op_end;
};

struct op_list {
    size_t op_count;
    size_t max_ops;
    struct op **eqs;
    char   *str;
    size_t str_size;
    int *check;
};

/* Function Prototypes */
struct op *op_str(char *str, char *end, struct op_list *ops);
struct op_list *new_op_list(char *str, size_t str_size);
void add_op(struct op_list *ops, struct op *eq);
short in(int val, int *list, size_t size);

/* Debug functions */
#if (DEBUG)
void substring(char *start, char *end);
void pr_op_list(struct op_list *ops);
void pr_op(struct op *eq);
#endif
