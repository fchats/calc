#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "const.h"
#include "manip.h"
#include "basic.h"

struct token {
    char* open;
    char* close;
};

struct op {
    unsigned int op_num;
    double a;
    double b;
    double (*sym)(double, double);
    double result;
    struct op *next;
    struct op *prev;
};

struct op_list {
    size_t op_count;
    size_t max_ops;
    struct op **eqs;
};

/* Function Prototypes */
struct token *inbrack(char* eq, size_t *eq_size, size_t *check, struct op_list *ops);
void set_result(struct op *s);
struct op *get_op(char *start, char *end);
void set_op(struct op *eq, double(*func)(double, double), char *start, char *p, char *end);
void add_op(struct op *eq, struct op_list *ops);

/* Debug functions */
#if (DEBUG)
void substring(char *start, char *end);
#endif
