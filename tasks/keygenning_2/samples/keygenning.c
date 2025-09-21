// gcc -O1 -o keygenning keygenning.c && strip keygenning
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static const uint8_t KEY = 0x5A;
static const char SALT[] = "PaX-42";
#define SALT_LEN (sizeof(SALT) - 1)

// serial is hex: length == 2*strlen(user); each byte is %02X
__attribute__((noinline)) int validate_license(const char *user, const char *serial) {
    size_t n = strlen(user);
    if (strlen(serial) != 2 * n) return 0;

    for (size_t i = 0; i < n; i++) {
        unsigned int got;  // sscanf needs an unsigned int
        if (sscanf(&serial[2*i], "%2x", &got) != 1) return 0;

        uint8_t expected = ((uint8_t)user[i]) ^ ((uint8_t)SALT[i % SALT_LEN]) ^ KEY;
        if ((uint8_t)got != expected) return 0;
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