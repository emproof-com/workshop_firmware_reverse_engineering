// gcc -o hello_world hello_world.c && ./hello_world 10 20
#include <stdio.h>
#include <stdlib.h>

static int compute(int a, int b) {
    // compute some magic
    return (a * 7) ^ (b + 13);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <a> <b>\n", argv[0]);
        return 1;
    }
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int r = compute(a, b);
    printf("%d\n", r);
    return 0;
}
