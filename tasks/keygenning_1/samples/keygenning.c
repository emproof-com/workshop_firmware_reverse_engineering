// gcc -O1 -o keygenning keygenning.c && strip keygenning
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

__attribute__((noinline)) int validate_license(const char *user, const char *serial) {
    size_t n = strlen(user);
    if (strlen(serial) != n) return 0;

    for (size_t i = 0; i < n; i++) {
        uint8_t u = (uint8_t)user[i];
        uint8_t p = (uint8_t)serial[i];
        if ((uint8_t)(u ^ 0x1b) != p) return 0;
    }
    return 1;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <username> <serial>\n", argv[0]);
        return 1;
    }

    if (validate_license(argv[1], argv[2])) {
        printf("License accepted!\n");
    } else {
        printf("License invalid!\n");
    }
    return 0;
}