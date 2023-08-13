#include "MatrixCalc.h"

/*
Uncomment the main function if running the program manually.


int main(int argc, char *argv[]) {    
    (void)argc;
    char script[200];
    sprintf(script, "tests.in/%s.txt", argv[1]);
    assert(access(script, F_OK) == 0);
    matrix_sf *mat = execute_script_sf(script);
    print_matrix_sf(mat);
    fflush(stdout);
    free(mat);
    return 0;
}
*/

/**
 * @brief
 *      Creates a BST node based on the given parameters.
 * 
 * @param mat 
 *      Pointer pointing to a valid matrix object (row-major). 
 * @param left_child 
 *      Left child of the current node.
 * @param right_child 
 *      Right child of the current node.
 * 
 * @return
 *      Pointer to the created BST node.
 */
bst_sf* create_bst_node(matrix_sf *mat, bst_sf* left_child, bst_sf* right_child){
    bst_sf *newNode = malloc(sizeof(bst_sf));
    newNode->mat = mat;
    newNode->left_child = left_child;
    newNode->right_child = right_child;
    return newNode;
}


/**
 * @brief 
 *      Inserts a matrix object into the BST pointed by root.
 * 
 * @param mat
 *      Pointer to a valid matrix object.
 * @param root
 *      Pointer to the root of a BST object.
 * 
 * @return
 *      Pointer to the root of the input BST object.
 */
bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    if(!root){
        root = create_bst_node(mat, NULL, NULL);
        return root;
    }

    if(root->mat->name < mat->name)
        root->right_child = insert_bst_sf(mat, root->right_child);
    else
        root->left_child = insert_bst_sf(mat, root->left_child);
    
    return root;
}


/**
 * @brief 
 *      Searches for a BST node with the specified matrix name.
 * 
 * @param name
 *      Name of the wanted matrix.
 * @param root 
 *      Pointer to the root of the BST object.
 * 
 * @return
 *      Pointer to the wanted matrix. 
 */
matrix_sf* find_bst_sf(char name, bst_sf *root) {

    if(!root)
        return NULL;

    if(root->mat->name == name)
        return root->mat;
    
    if(name > root->mat->name)
        return find_bst_sf(name, root->right_child);
    else
        return find_bst_sf(name, root->left_child);
        
}


/**
 * @brief 
 *      Deallocates memory of the entire BST object.
 * 
 * @param root
 *      Pointer to the root of the BST object.
 */
void free_bst_sf(bst_sf *root) {

    if(!root)
        return;

    if(root->left_child)
        free_bst_sf(root->left_child);

    if(root->right_child)
        free_bst_sf(root->right_child);

    free(root->mat);
    free(root);

}


/**
 * @brief
 *      Generates the sum of two matrices.
 * 
 * @param mat1
 *      Pointer to the matrix being added (operand 1).
 * @param mat2
 *      Pointer to the matrix being added (operand 2).
 * 
 * @return
 *      Pointer to the sum product.
 */
matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    int values[mat1->num_rows * mat1->num_cols];

    for(unsigned int i = 0; i < mat1->num_rows * mat1->num_cols; i++)
        values[i] = mat1->values[i] + mat2->values[i];
        
    return copy_matrix('?', mat1->num_rows, mat2->num_cols, values);
}


/**
 * @brief 
 *      Generates the product of two matrices.
 * 
 * @param mat1
 *      Pointer to the matrix being multiplied (operand 1).
 * @param mat2
 *      Pointer to the matrix being multiplied (operand 2).
 * 
 * @return
 *      Pointer to the product matrix.
 */
matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    int values[mat1->num_rows * mat2->num_cols];
    
    unsigned int index = 0;

    /*
    Since the matrix is in row-major format, we need to skip num_cols number of elements to get reach the next row.
    
    The most outer loop visit each row of mat1.

    The second outer loop visit each column of mat2.
    
    The inner while loop generates the elements of the product matrix. 
    
    Please view this site for a better understanding of matrix multiplication: https://www.mathsisfun.com/algebra/matrix-multiplying.html.
    
    */
    for(unsigned int i = 0; i < mat1->num_rows * mat1->num_cols; i+=mat1->num_cols){
        for(unsigned int j = 0; j < mat2->num_cols; j++){
            int sum = 0;
            unsigned int row = i, col = j;
            while(row < i + mat1->num_cols && col < mat2->num_rows * mat2->num_cols){
                sum += mat1->values[row] * mat2->values[col];
                row++;
                col+=mat2->num_cols;
            }
            values[index++] = sum;
        }
    }

    return copy_matrix('?', mat1->num_rows, mat2->num_cols, values);
}


/**
 * @brief 
 *      Generates the transpose of the input matrix.
 * 
 * @param mat
 *      Pointer to a valid matrix object.
 *  
 * @return
 *      Pointer to the tranpose matrix.
 *      
 */
matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    int values[mat->num_rows * mat->num_cols];

    unsigned int index = 0;

    for(unsigned int i = 0; i < mat->num_cols; i++)
        for(unsigned int j = i; j < mat->num_rows * mat->num_cols; j+=mat->num_cols)
            values[index++] = mat->values[j];

    return copy_matrix('?', mat->num_cols, mat->num_rows, values);
}


/**
 * @brief
 *      Creates a matrix object based on the input expr string.
 * 
 * @param name
 *      Name of the created matrix.
 * @param expr 
 *      String indicating the number of rows/columns and elements of the created matrix.
 * 
 * @return
 *      Pointer to the created matrix.
 * 
 * @note
 *      Input expr string must be in the following format: 
 *      [number of rows] [number of columns] [list of matrix elements]
 * @note
 *      Matrix elements must be enclosed with brackets "[]" and rows must be seperated by semicolons ";"
 */
matrix_sf* create_matrix_sf(char name, const char *expr) {

    char expression[strlen(expr) + 1];
    strcpy(expression, expr);

    /*
    break the input expr string into two parts, where one part contains the row/column number 
    and the other part contains a list of matrix elements.
    */
    char *rowAndcol = strtok(expression, "["), *matrix_element = strtok(NULL, "[");

    int row = strtol(strtok(rowAndcol, " "), NULL, 10), col = strtol(strtok(NULL, " "), NULL, 10),
            i = 0, values[row * col];
    
    matrix_element = strtok(matrix_element, " ");

    //loop through the list and store the listed intergers as elements of the created martix
    while(i < row * col && matrix_element && matrix_element[0] != ']'){
        int len = strlen(matrix_element);
        char *semi = strchr(matrix_element, ';'); //checks if the current matrix_element string contains a semicolon.

        if(!semi){ 
            //deals with the case "9"
            values[i] = strtol(matrix_element, NULL, 10);
            i++;
        }
        else{ 
            if(len > 1){
                if(matrix_element[0] == ';' && (isdigit(semi[1]) || (semi[1] == '-' && isdigit(semi[2])))) //deals with the case ";9" and ";-9"
                    values[i] = strtol(matrix_element + 1, NULL, 10);
                else if(matrix_element[len - 1] == ';' && isdigit(matrix_element[len - 2])) //deals with the case "9;"
                    values[i] = strtol(matrix_element, NULL, 10);
                else{
                    //deals with the case "9;9" and "9;-9"
                    values[i] = strtol(matrix_element, NULL, 10);
                    i++;
                    values[i] = strtol(semi + 1, NULL, 10);
                }
                i++;
            }
        }
        matrix_element = strtok(NULL, " "); //move to the next element in the list.
    }

    return copy_matrix(name, row, col, values);
}


/**
 * @brief 
 *      Returns the precedence of the input matrix operation.
 * 
 * @param oper
 *      Matrix operations (add(+), multiply(*), and transpose(')).
 * 
 * @return
 *      Integer representing the precedence of the input operation oper.
 * 
 * @note
 *      Precedence from high to low: transpose > multiply > add.
 */
int precedence(char oper){
    switch(oper){
        case '+':
            return 1;
        case '*':
            return 2;
        case '\'':
            return 3;
        default:
            return -1;
    }
}


/**
 * @brief 
 *      Checks if the input operation oper is a valid matrix operation.
 * 
 * @param oper
 *      Matrix operations (add(+), multiply(*), and transpose(')).
 * 
 * @return
 *      0 if not a valid matrix operation, otherwise 1 or greater.
 */
int isOperator(char oper){
    return (oper == '+' || oper == '*' || oper == '\'');
}


char* infix2postfix_sf(char *infix) {
    int len = strlen(infix);
    char *postfix = malloc(len + 2);
    char stack[len + 1];

    int top = -1, i = 0, j = 0;

    for(; i < len; i++){
        if(infix[i] == '\n')
            break;
        if(infix[i] == ' ' || infix[i] == '\t')
            continue;
        else if(isalpha(infix[i]))
            postfix[j++] = infix[i];
        else if(infix[i] == '(')
            stack[++top] = infix[i];
        else if(infix[i] == ')'){
            while(top > -1 && stack[top] != '(')
                postfix[j++] = stack[top--];
            top--;
        }
        else if(isOperator(infix[i])){
            while(top > -1 && precedence(stack[top]) >= precedence(infix[i]))
                postfix[j++] = stack[top--];
            stack[++top] = infix[i];
        }
    }

    while(top > -1)
        postfix[j++] = stack[top--];
    
    postfix[j] = '\0';

    return postfix;
}


matrix_sf* evaluate_expr_sf(char name, char *expr, bst_sf *root) {
    char *pExpr = infix2postfix_sf(expr);
    int len = strlen(pExpr), top = -1;
    matrix_sf *matStack[len + 2], *operand1 = NULL, *operand2 = NULL;

    for(int i = 0; i < len; i++){
        if(isalpha(pExpr[i]) && top < len + 2)
            matStack[++top] = find_bst_sf(pExpr[i], root);
        else if(isOperator(pExpr[i])){
            if(pExpr[i] == '\''){
                operand1 = matStack[top--];
                matStack[++top] = transpose_mat_sf(operand1);
                if(operand1 && !isalpha(operand1->name))
                    free(operand1);
            }
            else{
                operand2 = matStack[top--];
                operand1 = matStack[top--];

                if(pExpr[i] == '*') matStack[++top] = mult_mats_sf(operand1, operand2);
                else matStack[++top] = add_mats_sf(operand1, operand2);

                if(operand1 && !isalpha(operand1->name)) free(operand1);

                if(operand2 && !isalpha(operand2->name)) free(operand2);
            }
        }
    }

    matStack[top]->name = name;

    free(pExpr);

    return matStack[top];
}


matrix_sf *execute_script_sf(char *filename) {
    char *str = NULL;
    FILE *file = fopen(filename, "r");
    size_t max_line_size = MAX_LINE_LEN;
    matrix_sf *solution = NULL, *mPtr;
    bst_sf *root = NULL;

    while(getline(&str, &max_line_size, file) != -1){
        
        char *equal_sign = strchr(str, '=');

        if(!equal_sign)
            continue;
            
        if(strchr(str, '[')){
            mPtr = create_matrix_sf(str[0], equal_sign + 1);
            root = insert_bst_sf(mPtr, root);
        }
        else{
            if(solution)
                free(solution);
            mPtr = evaluate_expr_sf(str[0], equal_sign + 1, root);
            root = insert_bst_sf(mPtr, root);
            solution = copy_matrix(mPtr->name, mPtr->num_rows, mPtr->num_cols, mPtr->values);
        }
        
    }

    free(str);
    free_bst_sf(root);
    fclose(file);

    return solution;
}


/**
 * @brief 
 *      Generates a copy of the matrix based on the input matrix name, number of rows, number of columns, and matrix elements.
 * 
 * @param name
 *      Name of the copied matrix.
 * @param num_rows
 *      Number of rows in the copied matrix.
 * @param num_cols 
 *      Number of columns in the copied matrix.
 * @param values 
 *      Elements in the copied matrix.
 * 
 * @return
 *      Pointer to the copy.
 */
matrix_sf *copy_matrix(char name, unsigned int num_rows, unsigned int num_cols, int values[]){
    matrix_sf *matrix = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    matrix->name = name;
    matrix->num_rows = num_rows;
    matrix->num_cols = num_cols;
    memcpy(matrix->values, values, num_rows*num_cols*sizeof(int));
    return matrix;
}

/**
 * @brief 
 *      Prints the input the matrix in the following format:
 *      [number of rows] [number of columns] [matrix elements enclosed with brackets and rows seperated by semicolons]
 * 
 * @param mat 
 *      Pointer to a valid matrix object.
 * 
 * @warning
 *      For testing purposes, DO NOT MODIFY THIS FUNCTION.
 */
void print_matrix_sf(matrix_sf *mat) {
    assert(mat != NULL);
    assert(mat->num_rows <= 1000);
    assert(mat->num_cols <= 1000);
    printf("%d %d [", mat->num_rows, mat->num_cols);

    for (unsigned int i = 0; i < mat->num_rows*mat->num_cols; i++) {
        printf("%d", mat->values[i]);
        if (i < mat->num_rows*mat->num_cols-1){
            if(i % mat->num_cols  == mat->num_cols - 1)
                printf(";");
            
            printf(" ");
        }
        else
            printf("]");
    }
    printf("\n");
}

