#include <stdio.h>

#define T_SIZE 10

int in(size_t val, size_t *list, size_t size);

int main() {

    size_t t[T_SIZE] = {1, 1, 1, 1, 1, 1, 0, 1, 1, 1};
    printf("%d\n", in(0, t, T_SIZE));
}

int in(size_t val, size_t *list, size_t size) {
    int i;
    for (i = 0; i < size; i++) {
        if (val == list[i]) {
            return 1;
        }
    }
    return(0);
}
