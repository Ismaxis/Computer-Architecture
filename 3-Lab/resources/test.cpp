asm("nop");
asm("srai a0, a1, 2");
asm("lui x2, 0x100");     // SP set to 1 MB Setting the stack pointer register to an address
asm("jal main");          // call main
asm("addi x11, x10, 0");  // save return value in a1
asm("addi x17, x0, 10");  // prepare ecall exit
asm("fence");             // unknown instruction
asm("ecall");             // now your simulator should stop

#include <stdint.h>
#define M 20
#define K 20
#define N 20

int8_t a[M][K];
int16_t b[K][N];
int32_t c[M][N];

void mmul() {
    int8_t *pa = *a;
    int32_t *pc = *c;
    for (int y = 0; y < M; y++) {
        for (int x = 0; x < N; x++) {
            int16_t *pb = *b;
            int32_t s = 0;
            for (int k = 0; k < K; k++) {
                s += pa[k] * pb[x];
                pb += N;
            }
            pc[x] = s;
        }
        pa += K;
        pc += N;
    }
}

int main() {
    mmul();
    return 0;
}