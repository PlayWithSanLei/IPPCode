#include <stdio.h>
#include <omp.h>

int main() {
#ifdef _OPENMP
    printf("%d\n", _OPENMP);
#endif
    return 0;
}
