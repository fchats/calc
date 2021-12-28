#include "manip.h"
#include "const.h"

/* Simple function to retrieve a string from stdin using getchar()*/
char* get_string(size_t *size) {
    int i;
    size_t* max = (size_t *)malloc(sizeof(int));
    *max = INIT_MAX;
    char* str;
    str = (char*)malloc(sizeof(char) * (*max));
    *size = 0;
    char input;
    for (i = 0; (input = getchar()) != '\n' /*|| input != EOF*/; i++) {
        *(str + i) = input;
        (*size)++;
        #if (DEBUG)
        //printf("%lu\n", *size);
        #endif
        if (*size >= *max) {
            *max = 2 * (*max);
            str = realloc(str, *max);
            #if (DEBUG)
            printf("doubled size\n");
            #endif
        }
    }
    //printf("%lu\n", *size);
    return(str);
}


void remove_space(char* string) {
    char *p;
    p = string;
    while (*p != '\0') {
        if ((*p) == ' ') {
            shift_left(p);
        }
        p++;
    }
}


void shift_left(char* string) {
    char* p = string + 1;
    while (*p != '\0') {
        *(p - 1) = *p;  
        p++;
    }
    *(p - 1) = '\0';
    return;
}

