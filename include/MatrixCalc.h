#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_LINE_LEN 1000

#ifndef __MATRIX_SF
#define __MATRIX_SF

//Represents a matrix object.
typedef struct {
    char name;
    unsigned int num_rows;
    unsigned int num_cols;
    int values[]; 
} matrix_sf;


//Represents a node of the Binary Search Tree.
typedef struct bst_sf {
    matrix_sf *mat;
    struct bst_sf *left_child;
    struct bst_sf *right_child;
} bst_sf;


//functions declarations

bst_sf* create_bst_node(matrix_sf *mat, bst_sf* left_child, bst_sf* right_child);
bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root); 
matrix_sf* find_bst_sf(char name, bst_sf *root); 
void free_bst_sf(bst_sf *root); 
matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2); 
matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2); 
matrix_sf* transpose_mat_sf(const matrix_sf *mat); 
matrix_sf* create_matrix_sf(char name, const char *expr); 
matrix_sf* execute_script_sf(char *filename); 
matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root); 
char* infix2postfix_sf(char *infix); 

matrix_sf *copy_matrix(char name, unsigned int num_rows, unsigned int num_cols, int values[]);
void print_matrix_sf(matrix_sf *mat);

#endif // __MATRIX_SF
