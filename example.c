#include <stdio.h>
#define BITLANG_PRIV
#include "bitlang.h"

int main(int argc, char *argv[])
{
    int sz;
    bitlang vm;
    bitlang_state st;
    char bytes[128];
    int x, y;
    FILE *fp;

    sz = 256;

    bitlang_init(&vm);
    bitlang_state_init(&st, bytes, 128);

    /* compile the string into bytecode */
    /* this is a formula based on one by Foldster */
    bitlang_compile(&st, "x y + abs x y - abs 1 + ^ 2 << 3 % !");

    /* width, linked to 'w' */
    bitlang_regset(&vm, 2, sz);
    /* height, linked to 'h' */
    bitlang_regset(&vm, 3, sz);

    fp = fopen("example.pbm", "w");

    /* PBM header */
    fprintf(fp, "P1\n# bitlang test image\n%d %d\n", sz, sz);

    for (y = 0; y < sz; y++) {
        for (x = 0; x < sz; x++) {
            int val, rc;
            val = 0;

            /* x, y coordinates are registers 0 and 1 */

            bitlang_regset(&vm, 0, x);
            bitlang_regset(&vm, 1, y);
            bitlang_reset(&vm);

            rc = bitlang_exec(&vm, &st);

            if (rc) {
                printf("error\n");
                break;
            }

            if (x != 0) {
                fputc(' ', fp);
            }

            bitlang_pop(&vm, &val);

            if (val) fputc('1', fp);
            else fputc('0', fp);
        }

        fputc('\n', fp);
    }

    fclose(fp);
    return 0;
}
