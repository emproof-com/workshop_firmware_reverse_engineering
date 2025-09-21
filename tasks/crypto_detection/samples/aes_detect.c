// aes_detect.c
//
// Purpose: Detect AES usage, locate the EVP call site in a RE tool, and backtrack
//          the hardcoded key/IV (stored obfuscated) used to decrypt a file.
//
// Build (AArch64 Debian):
//   sudo apt-get update && sudo apt-get install -y build-essential libssl-dev zlib1g-dev libzstd-dev
//   gcc -O2 -static aes_detect.c -o aes_detect -Wl,--start-group -lcrypto -lzstd -lz -ldl -lpthread -Wl,--end-group -static-libgcc
//
// (Simpler dynamic fallback, if static is not required):
//   sudo apt-get install -y libssl-dev pkg-config
//   gcc -O2 aes_detect.c -o aes_detect $(pkg-config --cflags --libs openssl)
//
// Test vector used by this sample (hardcoded inside; key/iv are XOR-masked in .rodata):
//   AES-128-CBC key (hex): 2b7e151628aed2a6abf7158809cf4f3c
//   IV (hex):              000102030405060708090a0b0c0d0e0f
//
// Prepare a ciphertext for verification (on any host with OpenSSL CLI):
//   echo -n 'Hello World!\n' > plain.txt
//   openssl enc -aes-128-cbc -K 2b7e151628aed2a6abf7158809cf4f3c -iv 000102030405060708090a0b0c0d0e0f -in plain.txt -out secret.bin
//
// Decryption: 
//  openssl aes-128-cbc -d -K 2b7e151628aed2a6abf7158809cf4f3c -iv 000102030405060708090a0b0c0d0e0f -in secret.bin -out plain_decrypted.txt
//
// Run:
//   ./aes_detect --in secret.bin --out out.txt
//
// Note on static glibc builds: OpenSSL registers an atexit-cleanup by default.
// To avoid rare exit-time crashes in fully static glibc binaries, we disable
// the automatic atexit handler via OPENSSL_init_crypto(OPENSSL_INIT_NO_ATEXIT, NULL).

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <openssl/evp.h>
#include <openssl/crypto.h>  // for OPENSSL_init_crypto & OPENSSL_INIT_NO_ATEXIT

// Hardcoded key/IV in .rodata (no masking)
static const uint8_t KEY[16] = {
    0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c
};
static const uint8_t IV[16]  = {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f
};

static int aes128_cbc_decrypt_file(const char *in_path, const char *out_path) {
    FILE *fin = fopen(in_path, "rb");
    if (!fin) { perror("open input"); return 1; }
    FILE *fout = fopen(out_path, "wb");
    if (!fout) { perror("open output"); fclose(fin); return 1; }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) { fclose(fin); fclose(fout); return 1; }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, KEY, IV) != 1) {
        EVP_CIPHER_CTX_free(ctx); fclose(fin); fclose(fout); return 1;
    }

    unsigned char inbuf[4096], outbuf[4096 + EVP_MAX_BLOCK_LENGTH];
    int inlen = 0, outlen = 0;
    int ok = 0;

    while ((inlen = (int)fread(inbuf, 1, sizeof(inbuf), fin)) > 0) {
        if (EVP_DecryptUpdate(ctx, outbuf, &outlen, inbuf, inlen) != 1) goto done;
        if ((int)fwrite(outbuf, 1, outlen, fout) != outlen) goto done;
    }
    if (ferror(fin)) goto done;

    if (EVP_DecryptFinal_ex(ctx, outbuf, &outlen) != 1) goto done;
    if ((int)fwrite(outbuf, 1, outlen, fout) != outlen) goto done;

    ok = 1;
done:
    EVP_CIPHER_CTX_free(ctx);
    fclose(fin);
    fclose(fout);
    return ok ? 0 : 2;
}

static void usage(const char *argv0) {
    fprintf(stderr, "usage: %s --in <cipher.bin> --out <plain.out>\n", argv0);
}

int main(int argc, char **argv) {
    // Avoid OpenSSL's atexit cleanup in fully static glibc builds.
    OPENSSL_init_crypto(OPENSSL_INIT_NO_ATEXIT, NULL);

    const char *in_path = NULL, *out_path = NULL;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--in")  && i + 1 < argc) in_path  = argv[++i];
        else if (!strcmp(argv[i], "--out") && i + 1 < argc) out_path = argv[++i];
    }
    if (!in_path || !out_path) { usage(argv[0]); return 1; }

    int rc = aes128_cbc_decrypt_file(in_path, out_path);
    if (rc == 0) {
        printf("ok: wrote %s\n", out_path);
    } else {
        printf("decrypt failed (rc=%d)\n", rc);
    }
    return rc;
}