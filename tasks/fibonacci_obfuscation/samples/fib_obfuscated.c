// gcc -O1 -Wall -Wextra -o fib_obfuscated fib_obfuscated.c

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Baseline ---------- */
__attribute__((noinline)) static unsigned fib_ref(unsigned n) {
    if (n == 0) return 0;
    unsigned a = 0, b = 1, s = 1;
    for (unsigned i = 2; i <= n; i++) {
        s = a + b; a = b; b = s;
    }
    return s;
}

/* ---------- Obf #1: switch-based flattening ---------- */
__attribute__((noinline)) static unsigned fib_flat_switch(unsigned n) {
    unsigned a, b, s, i;
    unsigned state = 0;
    for (;;) {
        switch (state) {
            case 0:
                a = 0; b = 1; s = 0; i = 2;
                state = (n == 0) ? 3 : 1;
                if (state == 1) s = b;
                break;
            case 1:
                if (i > n) { state = 3; break; }
                s = a + b; a = b; b = s; i++;
                state = 1;
                break;
            case 3:
                return s;
            default:
                return 0;
        }
    }
}

/* ---------- Obf #2: computed-goto flattening ---------- */
__attribute__((noinline)) static unsigned fib_flat_goto(unsigned n) {
#if defined(__GNUC__)
    volatile unsigned opaque0 = 0; /* thwarts some constant-folding */
    unsigned a=0, b=1, s=(n==0?0:1), i=2;
    static void *jt[] = { &&S0, &&S1, &&END };
    unsigned st = opaque0; /* start at S0 */

S0:
    if (n == 0) { st = 2; goto *jt[st]; }
    st = 1; goto *jt[st];

S1:
    if (i > n) { st = 2; goto *jt[st]; }
    /* bitwise add: s = a + b */
    s = (a ^ b) + ((a & b) << 1);
    a = b; b = s; i++;
    st = 1; goto *jt[st];

END:
    return s;
#else
    return fib_ref(n);
#endif
}

/* ---------- Obf #3: opaque predicates + junk ---------- */
__attribute__((noinline)) static unsigned fib_opaque(unsigned n) {
    if (n == 0) return 0;
    unsigned a = 0, b = 1;
    unsigned i = 2;

    /* Make an always-true predicate that doesn’t look constant. */
    volatile unsigned k = ((n | 1u) << 1); /* even */
#define OPAQUE_T (((k*k) % 4u) == 0u)

    while (OPAQUE_T && i <= n) {
        unsigned s = (a ^ b) + ((a & b) << 1); /* s = a+b */
        a = b; b = s; i++;
        if (!OPAQUE_T) { /* never taken: junk + barrier */
            for (volatile int j=0;j<3;j++) __asm__ __volatile__("" ::: "memory");
        }
    }
    return b;
}

__attribute__((noinline)) static unsigned fib_arithmetic(unsigned n) {
    unsigned a = 0; 
    unsigned b = 0;
    unsigned s = 0;
    unsigned i = 0;
    unsigned state = (n ^ 5) + 2 * (n & 5) - n - 5; // state = 0

    while (1) {
        switch (state) {
            case 0: 
                a = (n ^ 23) + 2 * (n & 23) - n -  23;
                b = (n ^ 1) + 2 * (n & 1) - n;
                s = (n ^ 1337) + 2 * (n & 1337) - n - 1337;
                i = (n ^ 2) + 2 * (n & 2) - n;
                if (n == 0) {
                    state = (n ^ 3) + 2 * (n & 3) - n; 
                } else {
                    s = b;
                    state = (n ^ 1) + 2 * (n & 1) - n;
                }
                break;

            case 1:
                if (i > n) {
                    state = (n ^ 3) + 2 * (n & 3) - n;
                } else {
                    s = (a ^ b) + 2 * (a & b);
                    a = (a ^ b) + 2 * (a & b) -  a; // a = b
                    b = (a ^ s) + 2 * (a & s) - a;
                    i = (i ^ 1) + 2 * (i & 1);
                    state = (a ^ 1) + 2 * (a & 1) - a;
                }
                break;

            case 3:
                return (n ^ s) + 2 * (n & s) - n;

            default:
                return (n ^ 1234) + 2 * (n & 1234) - n;
        }
    }
}


/* ---------- Obf #4: tiny bytecode VM (advanced) ---------- */
/*
   Simple register VM (r0..r4, zf) computing Fibonacci; program is XOR-decoded at runtime.
   Ops (1 byte op + optional imm):
     0x10 MOVN dst         ; r[dst] = n
     0x11 MOVI dst,imm8    ; r[dst] = imm
     0x12 MOVR dst,src     ; r[dst] = r[src]
     0x20 ADD  dst,src     ; r[dst] += r[src]
     0x21 INC  dst
     0x30 CMPGT a,b        ; zf = (r[a] > r[b])
     0x31 CMPEQ a,imm8     ; zf = (r[a] == imm)
     0x40 JZ   imm16       ; if zf==1 pc=imm16
     0x41 JMP  imm16
     0x50 RET  reg         ; return r[reg]
*/
__attribute__((noinline)) /* ---------- Obf #5: tiny bytecode VM ---------- */
/* Registers: r0=a, r1=b, r2=i, r3=n, r4=s; zf is a boolean. */
static unsigned fib_vm(unsigned n) {
    enum { R0=0, R1, R2, R3, R4 };
    unsigned r[5] = {0,0,0,0,0};
    int zf = 0;

    /* Program (absolute PC addresses), XOR-key = 0x5A
       Layout (unencoded):
         00: 10 R3               ; r3 = n
         02: 31 R3 00            ; zf = (r3 == 0)
         05: 40 2A 00            ; JZ ret0 (pc=42)
         08: 11 R0 00            ; r0 = 0
         11: 11 R1 01            ; r1 = 1
         14: 11 R2 02            ; r2 = 2
       loop=17:
         17: 30 R2 R3            ; zf = (r2 > r3)
         20: 40 28 00            ; JZ ret (pc=40) if (i>n)
         23: 12 R4 R0            ; r4 = r0
         26: 20 R4 R1            ; r4 += r1
         29: 12 R0 R1            ; r0 = r1
         32: 12 R1 R4            ; r1 = r4
         35: 21 R2               ; r2++
         37: 41 11 00            ; JMP loop (pc=17)
       ret=40:
         40: 50 R1               ; return r1
       ret0=42:
         42: 50 R0               ; return r0
    */
    static const uint8_t enc[] = {
        /* 00 */ 0x10^0x5A, R3^0x5A,
        /* 02 */ 0x31^0x5A, R3^0x5A, 0x00^0x5A,
        /* 05 */ 0x40^0x5A, 0x2A^0x5A, 0x00^0x5A,       /* JZ ret0 (42) */
        /* 08 */ 0x11^0x5A, R0^0x5A, 0x00^0x5A,
        /* 11 */ 0x11^0x5A, R1^0x5A, 0x01^0x5A,
        /* 14 */ 0x11^0x5A, R2^0x5A, 0x02^0x5A,
        /* 17 */ 0x30^0x5A, R2^0x5A, R3^0x5A,           /* CMPGT r2,r3 */
        /* 20 */ 0x40^0x5A, 0x28^0x5A, 0x00^0x5A,       /* JZ ret (40) if (i>n) */
        /* 23 */ 0x12^0x5A, R4^0x5A, R0^0x5A,
        /* 26 */ 0x20^0x5A, R4^0x5A, R1^0x5A,
        /* 29 */ 0x12^0x5A, R0^0x5A, R1^0x5A,
        /* 32 */ 0x12^0x5A, R1^0x5A, R4^0x5A,
        /* 35 */ 0x21^0x5A, R2^0x5A,
        /* 37 */ 0x41^0x5A, 0x11^0x5A, 0x00^0x5A,       /* JMP loop (17) */
        /* 40 */ 0x50^0x5A, R1^0x5A,                    /* ret */
        /* 42 */ 0x50^0x5A, R0^0x5A                     /* ret0 */
    };

    const size_t L = sizeof(enc);
    uint8_t *prog = (uint8_t*)malloc(L);
    for (size_t i=0;i<L;i++) prog[i] = enc[i] ^ 0x5A;

    unsigned pc = 0, ret = 0;
    for (;;) {
        uint8_t op = prog[pc++];
        switch (op) {
            case 0x10: r[prog[pc++]] = n; break;                        /* MOVN  */
            case 0x11: { uint8_t d=prog[pc++], imm=prog[pc++]; r[d]=imm; } break; /* MOVI */
            case 0x12: { uint8_t d=prog[pc++], s=prog[pc++];   r[d]=r[s]; } break; /* MOVR */
            case 0x20: { uint8_t d=prog[pc++], s=prog[pc++];   r[d]+=r[s]; } break;/* ADD  */
            case 0x21: r[prog[pc++]] += 1; break;                        /* INC  */
            case 0x30: { uint8_t a=prog[pc++], b=prog[pc++]; zf=(r[a]>r[b]); } break; /* CMPGT */
            case 0x31: { uint8_t a=prog[pc++], im=prog[pc++]; zf=(r[a]==im); } break; /* CMPEQ */
            case 0x40: { uint16_t t=prog[pc]|(prog[pc+1]<<8); pc+=2; if (zf) pc=t; } break; /* JZ */
            case 0x41: { uint16_t t=prog[pc]|(prog[pc+1]<<8); pc=t; } break;               /* JMP */
            case 0x50: ret = r[prog[pc++]]; free(prog); return ret;                         /* RET */
            default:   free(prog); return 0xDEAD;
        }
    }
}

/* ---------- Driver ---------- */
int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr,"usage: %s <n>\n", argv[0]); return 1; }
    unsigned n = (unsigned)strtoul(argv[1], NULL, 0);

    unsigned r0 = fib_ref(n);
    unsigned r1 = fib_flat_switch(n);
    unsigned r2 = fib_flat_goto(n);
    unsigned r3 = fib_opaque(n);
    unsigned r4 = fib_arithmetic(n);
    unsigned r5 = fib_vm(n);

    printf("fib_ref(%u)        = %u\n", n, r0);
    printf("fib_flat_switch    = %u\n", r1);
    printf("fib_flat_goto      = %u\n", r2);
    printf("fib_opaque         = %u\n", r3);
    printf("fib_arithmetic     = %u\n", r4);
    printf("fib_vm(bytecode)   = %u\n", r5);
    return 0;
}