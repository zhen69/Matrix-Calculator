#include "unit_tests.h"
#include "MatrixCalc.h"
int main() { 
    matrix_sf *Q = copy_matrix('Q', 1, 4, (int[]){-123, 47, -4, 140});
    matrix_sf *G = copy_matrix('G', 1, 4, (int[]){-16, 122, 135, 107});
    matrix_sf *Z = add_mats_sf(Q, G);
    free(Q);
    free(G);
    free(Z);
    return 0;
}
