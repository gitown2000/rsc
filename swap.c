/* swap.c
 * Swap two values in-place using arithmetic — no temporary variable.
 * a = a+b; b = a-b; a = a-b;
 *
 * Limitations:
 *   - Integers: signed overflow is undefined behaviour in C (works in practice
 *     on two's-complement machines, but not guaranteed by the standard).
 *   - Floats/doubles: rounding errors mean the restored value may not be
 *     bit-for-bit identical to the original.
 *   - Non-numeric types (structs, pointers, …): not applicable.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void swap_int(int *a, int *b) {
    if (a == b) return;
    *a = *a + *b;
    *b = *a - *b;   /* b = (a+b) - b = original a */
    *a = *a - *b;   /* a = (a+b) - original a = original b */
}

void swap_double(double *a, double *b) {
    if (a == b) return;
    *a = *a + *b;
    *b = *a - *b;
    *a = *a - *b;
}

void swap_xor(int *a, int *b) {
    if (a == b) return;
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

/* XOR-swap on the raw 64-bit representation of two doubles.
 * Uses memcpy to avoid strict-aliasing UB; always bit-perfect. */
void swap_xor_double(double *a, double *b) {
    if (a == b) return;
    uint64_t ua, ub;
    memcpy(&ua, a, sizeof ua);
    memcpy(&ub, b, sizeof ub);
    ua ^= ub;
    ub ^= ua;
    ua ^= ub;
    memcpy(a, &ua, sizeof ua);
    memcpy(b, &ub, sizeof ub);
}

void compare_accuracy_double(double a, double b) {
    /* Expected result after swap: x == original b, y == original a */
    double x1 = a, y1 = b;
    double x2 = a, y2 = b;

    swap_double(&x1, &y1);
    int ok_arith = (x1 == b && y1 == a);
    printf("swap_double:      %.17g %.17g  (expected %.17g %.17g) -> %s\n",
           x1, y1, b, a, ok_arith ? "PASS" : "FAIL (rounding error)");

    swap_xor_double(&x2, &y2);
    int ok_xor = (x2 == b && y2 == a);
    printf("swap_xor_double:  %.17g %.17g  (expected %.17g %.17g) -> %s\n",
           x2, y2, b, a, ok_xor ? "PASS" : "FAIL");
}

void compare_accuracy(int a, int b) {
    /* Expected result after swap: x == original b, y == original a */
    int x1 = a, y1 = b;
    int x2 = a, y2 = b;

    swap_int(&x1, &y1);
    int ok_int = (x1 == b && y1 == a);
    printf("swap_int:  %d %d  (expected %d %d) -> %s\n",
           x1, y1, b, a, ok_int ? "PASS" : "FAIL");

    swap_xor(&x2, &y2);
    int ok_xor = (x2 == b && y2 == a);
    printf("swap_xor:  %d %d  (expected %d %d) -> %s\n",
           x2, y2, b, a, ok_xor ? "PASS" : "FAIL");
}

int main(void) {
    int ix = 10, iy = 20;
    printf("int    before: %d  %d\n", ix, iy);
    swap_int(&ix, &iy);
    printf("int    after:  %d  %d\n\n", ix, iy);

    compare_accuracy(10, 20);

    printf("\n--- double: similar magnitudes (arithmetic swap survives) ---\n");
    compare_accuracy_double(1.1, 2.2);

    printf("\n--- double: vastly different magnitudes (arithmetic swap loses precision) ---\n");
    /* 1e16 has ULP=2, so 1e16+1.0 == 1e16 exactly — 1.0 is silently lost */
    compare_accuracy_double(1e16, 1.0);

    return 0;
}
