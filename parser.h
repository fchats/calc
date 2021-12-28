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
    int op_num;
    double a;
    double b;
    double (*sym)(double, double);
    double result;
    struct op *next;
    struct op *prev;
};

/* Function Prototypes */
struct token *inbrack(char* eq, size_t *eq_size, short *check,
                      size_t* op_count);
void set_result(struct op *s);
struct op *get_op(char *start, char *end);
void set_op(struct op *eq, double(*func)(double, double), char *start, char *p, char *end);
/* struct op **list_ops() */

/* Debug functions */
#if (DEBUG)
void substring(char *start, char *end);
#endif
