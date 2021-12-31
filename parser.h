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
struct token *inbrack(struct op_list *ops);
void set_result(struct op *s);
struct op *get_op(char *start, char *end, struct op_list *ops);
void set_op(struct op *eq, double(*func)(double, double), char *start, char *p, 
            char *end, struct op_list *ops);
void add_op(struct op *eq, struct op_list *ops);
struct op_list *eval(struct op_list *ops);
int in(size_t *val, size_t *list, size_t size);

/* Debug functions */
#if (DEBUG)
void substring(char *start, char *end);
void pr_op_list(struct op_list *ops);
void pr_op(struct op *eq);
#endif
