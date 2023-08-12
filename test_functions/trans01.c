#include "unit_tests.h"
#include "MatrixCalc.h"
int main() { 
    matrix_sf *M = copy_matrix('M', 4, 4, (int[]){-7, 78, -87, -113, -144, -94, 22, -75, -137, -130, -113, -106, 85, -120, 50, 55});
    matrix_sf *G = transpose_mat_sf(M);
    free(M);
    free(G);
    return 0;
}
