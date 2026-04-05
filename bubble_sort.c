/* bubble_sort.c
 * Read integers from stdin, sort them using bubble sort, and print the sorted list.
 * Example: echo "3 1 2" | ./bubble_sort
 */
#include <stdio.h>
#include <stdlib.h>

void bubble_sort(int *a, int n) {
    for (int i = 0; i < n - 1; ++i) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i; ++j) {
            if (a[j] > a[j+1]) {
                int t = a[j]; a[j] = a[j+1]; a[j+1] = t;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

int main(void) {
    int capacity = 16, n = 0;
    int *a = malloc(capacity * sizeof(int));
    if (!a) return 1;
    int x;
    while (scanf("%d", &x) == 1) {
        if (n >= capacity) {
            capacity *= 2;
            int *tmp = realloc(a, capacity * sizeof(int));
            if (!tmp) { free(a); return 1; }
            a = tmp;
        }
        a[n++] = x;
    }
    if (n == 0) {
        fprintf(stderr, "Usage: provide integers via stdin (e.g., echo \"3 1 2\" | ./bubble_sort)\n");
        free(a); return 1;
    }
    bubble_sort(a, n);
    for (int i = 0; i < n; ++i) {
        if (i) printf(" ");
        printf("%d", a[i]);
    }
    printf("\n");
    free(a);
    return 0;
}
