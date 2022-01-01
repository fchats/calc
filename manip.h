#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <assert.h>
#include <string.h>

#include "const.h"

char* get_string(size_t *size);
void remove_space(char* string, size_t *size);
void shift_left(char* string);
void pr_substring(char *start, char *end); 
