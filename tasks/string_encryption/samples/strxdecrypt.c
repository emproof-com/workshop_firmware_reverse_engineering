// gcc -O2 -o strxdecrypt strxdecrypt.c && strip strxdecrypt
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static const uint8_t KEY = 0x5A;

/* encrypted strings (each byte XORed with KEY), no plaintext resides in .rodata */
static const unsigned char enc_welcome[] = {
    0x0D, 0x3F, 0x36, 0x39, 0x35, 0x37, 0x3F, 0x7A, 0x2E, 0x35, 0x7A, 0x2E,
    0x32, 0x3F, 0x7A, 0x37, 0x33, 0x34, 0x33, 0x7A, 0x39, 0x3B, 0x36, 0x39,
    0x7B, 0x50
};
static const size_t enc_welcome_len = 26;

static const unsigned char enc_usage[] = {
    0x0F, 0x29, 0x3B, 0x3D, 0x3F, 0x60, 0x7A, 0x7F, 0x29, 0x7A, 0x66, 0x3B,
    0x3E, 0x3E, 0x26, 0x37, 0x2F, 0x36, 0x64, 0x7A, 0x66, 0x3B, 0x64, 0x7A,
    0x66, 0x38, 0x64, 0x50
};
static const size_t enc_usage_len = 28;

static const unsigned char enc_unknown[] = {
    0x0F, 0x34, 0x31, 0x34, 0x35, 0x2D, 0x34, 0x7A, 0x35, 0x2A, 0x3F, 0x28,
    0x3B, 0x2E, 0x33, 0x35, 0x34, 0x74, 0x50
};
static const size_t enc_unknown_len = 19;

static const unsigned char enc_result[] = {
    0x08, 0x3F, 0x29, 0x2F, 0x36, 0x2E, 0x60, 0x7A, 0x7F, 0x3E, 0x50
};
static const size_t enc_result_len = 11;

static const unsigned char enc_goodbye[] = {
    0x1D, 0x35, 0x35, 0x3E, 0x38, 0x23, 0x3F, 0x7B, 0x50
};
static const size_t enc_goodbye_len = 9;

static const unsigned char enc_op_add[] = { 0x3B, 0x3E, 0x3E };
static const size_t enc_op_add_len = 3;

static const unsigned char enc_op_mul[] = { 0x37, 0x2F, 0x36 };
static const size_t enc_op_mul_len = 3;

__attribute__((noinline)) static void xor_decrypt(const unsigned char *enc, size_t n, uint8_t key, char *out) {
    for (size_t i = 0; i < n; i++) out[i] = (char)(enc[i] ^ key);
    out[n] = '\0';
}

int main(int argc, char **argv) {
    /* decrypt all messages at runtime */
    char welcome[64], usage[64], unknown[64], result_fmt[32], goodbye[32];
    char op_add[8], op_mul[8];

    xor_decrypt(enc_welcome, enc_welcome_len, KEY, welcome);
    xor_decrypt(enc_usage,   enc_usage_len,   KEY, usage);
    xor_decrypt(enc_unknown, enc_unknown_len, KEY, unknown);
    xor_decrypt(enc_result,  enc_result_len,  KEY, result_fmt);
    xor_decrypt(enc_goodbye, enc_goodbye_len, KEY, goodbye);
    xor_decrypt(enc_op_add,  enc_op_add_len,  KEY, op_add);
    xor_decrypt(enc_op_mul,  enc_op_mul_len,  KEY, op_mul);

    printf("%s", welcome);

    if (argc != 4) {
        printf(usage, argv[0]); /* usage contains a single %s placeholder */
        return 1;
    }

    const char *op = argv[1];
    int a = atoi(argv[2]);
    int b = atoi(argv[3]);

    if (strcmp(op, op_add) == 0) {
        printf(result_fmt, a + b);
    } else if (strcmp(op, op_mul) == 0) {
        printf(result_fmt, a * b);
    } else {
        printf("%s", unknown);
        return 2;
    }

    printf("%s", goodbye);
    return 0;
}