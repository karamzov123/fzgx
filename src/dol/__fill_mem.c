#include "types.h"

#define cps ((unsigned char *)src)
#define cpd ((unsigned char *)dst)
#define lps ((unsigned long *)src)
#define lpd ((unsigned long *)dst)
#define deref_auto_inc(p) *++(p)

__declspec(section ".init") void __fill_mem(void *dst, int val, unsigned long n) {
    unsigned long v = (unsigned char)val;
    unsigned long i;

    cpd = ((unsigned char *)dst) - 1;

    if (n >= 32) {
        i = (~(unsigned long)dst) & 3;

        if (i) {
            n -= i;

            do {
                deref_auto_inc(cpd) = v;
            } while (--i);
        }

        if (v) {
            v |= v << 24 | v << 16 | v << 8;
        }

        lpd = ((unsigned long *)(cpd + 1)) - 1;

        i = n >> 5;

        if (i) {
            do {
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
            } while (--i);
        }

        i = (n & 31) >> 2;

        if (i) {
            do {
                deref_auto_inc(lpd) = v;
            } while (--i);
        }

        cpd = ((unsigned char *)(lpd + 1)) - 1;

        n &= 3;
    }

    if (n) {
        do {
            deref_auto_inc(cpd) = v;
        } while (--n);
    }

    return;
}
