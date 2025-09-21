// license_check_crc.c
//
// Usage:
// 1) Print CRC for the current guarded region (do NOT edit check_license between steps):
//    gcc -O1 -fno-pie -no-pie -o license_check_crc license_check_crc.c && ./license_check_crc --print-crc
// 2) Enforce CRC (rebuild with the printed value; stripping optional):
//    gcc -O1 -fno-pie -no-pie -DEXPECTED_CRC=0x5056A075 -o license_check_crc license_check_crc.c && strip license_check_crc
//
// Notes:
// - Keep optimization at -O1 for a stable byte layout. Higher -O or LTO can change bytes.
// - Build non-PIE (-fno-pie -no-pie).
// - This is an anti-patching hurdle, not a guarantee; patchers can always disable the check itself.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* ---- Tiny CRC32 (poly 0xEDB88320), table-less ---- */
static uint32_t crc32_buf(const void *data, size_t len) {
    const unsigned char *p = (const unsigned char*)data;
    uint32_t crc = ~0u;
    for (size_t i = 0; i < len; i++) {
        crc ^= p[i];
        for (int b = 0; b < 8; b++)
            crc = (crc >> 1) ^ (0xEDB88320u & -(int)(crc & 1));
    }
    return ~crc;
}

/* If EXPECTED_CRC is 0 or undefined, we print and exit. */
#ifndef EXPECTED_CRC
#  define EXPECTED_CRC 0u
#endif

/* Anchors for the guarded region (labels emitted inside check_license). */
extern const unsigned char __cl_start, __cl_end;


__attribute__((noinline))
static int check_license(char *input_license) {
    static const char correct_license[] = "ABC123-XYZ789";
    int ok = (strcmp(input_license, correct_license) == 0);
    return ok;
}

static void unlock_feature(void) {
    puts("Access granted! The premium feature is now unlocked.");
}


/* Guarded function: we place begin/end labels inside this function.
   Ensure single exit (no early returns) so the guarded bytes are stable. */
int main(int argc, char **argv) {
    /* Compute CRC over the guarded region (the body of check_license). */
    size_t len = (size_t)((const unsigned char *)&__cl_end
                        - (const unsigned char *)&__cl_start);
    uint32_t crc = crc32_buf(&__cl_start, len);

    int print_crc = (argc > 1 && strcmp(argv[1], "--print-crc") == 0);
    if (print_crc || EXPECTED_CRC == 0u) {
        printf("CRC32(check_license) = 0x%08X (len=%zu bytes)\n", crc, len);
        if (!print_crc) _Exit(0);
    } else if (crc != EXPECTED_CRC) {
        fprintf(stderr, "Integrity check failed (got 0x%08X, expected 0x%08X)\n",
                crc, EXPECTED_CRC);
        _Exit(5);
    }

    __asm__ __volatile__ (".globl __cl_start\n__cl_start:");
    char input_license[64];
    printf("Enter your license key: ");
    if (scanf("%63s", input_license) != 1) return 1;

    if (check_license(input_license)) {
        unlock_feature();
    } else {
        puts("Invalid license key. Access denied.");
    }
    __asm__ __volatile__ (".globl __cl_end\n__cl_end:");

    return 0;
}