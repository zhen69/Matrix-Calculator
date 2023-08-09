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
bst_sf* create_bst_node(matrix_sf *mat, int size, bst_sf* left_child, bst_sf* right_child){
    bst_sf *newNode = malloc(size);
    newNode->mat = mat;
    newNode->left_child = left_child;
    newNode->right_child = right_child;
    return newNode;
}


bst_sf* insert_bst_sf(matrix_sf *mat, bst_sf *root) {
    if(root == NULL){
        root = create_bst_node(mat, sizeof(bst_sf), NULL, NULL);
        return root;
    }

    if(root->mat->name < mat->name)
        root->right_child = insert_bst_sf(mat, root->right_child);
    else
        root->left_child = insert_bst_sf(mat, root->left_child);
    
    return root;
}


matrix_sf* find_bst_sf(char name, bst_sf *root) {

    bst_sf *currNode = root;

    while(currNode != NULL){
        char nodeName = currNode->mat->name;
        if(nodeName == name)
            return currNode->mat;

        if(nodeName > name){
            currNode = currNode->left_child;
        }   
        else{
            currNode = currNode->right_child;
        }
    }

    return NULL;
        
}


void free_bst_sf(bst_sf *root) {

    if(root == NULL)
        return;

    if(root->left_child != NULL)
        free_bst_sf(root->left_child);

    if(root->right_child != NULL)
        free_bst_sf(root->right_child);

    free(root->mat);
    free(root);

}


matrix_sf* add_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    matrix_sf *sumMatrix = malloc(sizeof(matrix_sf) + mat1->num_rows * mat1->num_cols * sizeof(int));

    sumMatrix->name = '@';
    sumMatrix->num_rows = mat1->num_rows;
    sumMatrix->num_cols = mat1->num_cols;

    const int *arrPtr1 = mat1->values, *arrPtr2 = mat2->values; 
    
    int *sumPtr = sumMatrix->values, len = mat1->num_rows * mat1->num_cols;

    for(int i = 0; i < len; i++){
        sumPtr[i] = arrPtr1[i] + arrPtr2[i];
    }

    return sumMatrix;
}


matrix_sf* mult_mats_sf(const matrix_sf *mat1, const matrix_sf *mat2) {
    const int *matrixA = mat1->values, *matrixB = mat2->values; 

    int lenA = mat1->num_rows * mat1->num_cols, lenB = mat2->num_rows * mat2->num_cols;

    matrix_sf *product = malloc(sizeof(matrix_sf) + mat1->num_rows * mat2->num_cols * sizeof(int));

    product->name = '$';
    product->num_rows = mat1->num_rows;
    product->num_cols = mat2->num_cols;

    int *pArr = product->values, index = 0, colA = mat1->num_cols, colB = mat2->num_cols;

    for(int i = 0; i < lenA; i+=colA){
        for(int j = 0; j < colB; j++){
            int sum = 0, r = i, c = j;
            while(r < i + colA && c < lenB){
                sum += matrixA[r] * matrixB[c];
                r++;
                c+=colB;
            }

            pArr[index++] = sum;
        }
    }

    return product;
}


matrix_sf* transpose_mat_sf(const matrix_sf *mat) {
    matrix_sf *trans = malloc(sizeof(matrix_sf) + mat->num_rows * mat->num_cols * sizeof(int));

    trans->name = '^';
    trans->num_rows = mat->num_cols;
    trans->num_cols = mat->num_rows;

    int *tArr = trans->values, size = mat->num_rows * mat->num_cols, index = 0;

    const int *cArr = mat->values, col = mat->num_cols;

    for(int i = 0; i < col; i++){
        for(int j = i; j < size; j+=col){
            tArr[index++] = cArr[j];
        }
    }

    return trans;
}


matrix_sf* create_matrix_sf(char name, const char *expr) {

    char expression[strlen(expr) + 1];

    strcpy(expression, expr);

    char *rowAndcol = strtok(expression, "["), *list = strtok(NULL, "[");

    int row = strtol(strtok(rowAndcol, " "), NULL, 10), col = strtol(strtok(NULL, " "), NULL, 10),
            i = 0;

    matrix_sf *matrix = malloc(sizeof(matrix_sf) + row * col * sizeof(int));

    matrix->name = name;
    matrix->num_rows = row;
    matrix->num_cols = col;

    int *mat = matrix->values;
    
    list = strtok(list, " ");

    while(i < row * col && list != NULL){
        int len = strlen(list);
        if(list[0] == ']')
            break;

        char *semi = strchr(list, ';');

        if(semi == NULL){
            mat[i] = strtol(list, NULL, 10);
            i++;
        }
        else{
            if(len > 1){
                if(list[0] == ';' && (isdigit(semi[1]) || (semi[1] == '-' && isdigit(semi[2]))))
                    mat[i] = strtol(list + 1, NULL, 10);
                else if(list[len - 1] == ';' && isdigit(list[len - 2]))
                    mat[i] = strtol(list, NULL, 10);
                else{
                    mat[i] = strtol(list, NULL, 10);

                    if(isdigit(semi[1]) || (semi[1] == '-' && isdigit(semi[2]))){
                        i++;
                        mat[i] = strtol(semi + 1, NULL, 10);
                    }
                }
                i++;
            }
        }
        list = strtok(NULL, " ");
    }

    return matrix;
}

/*precedence(), isOperator(), and infix2postfix_sf() are functions obtained from Geeks For Geeks.

site: https://www.geeksforgeeks.org/convert-infix-expression-to-postfix-expression/
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


int isOperator(char oper){
    return (oper == '+' || oper == '*' || oper == '\'');
}

char* infix2postfix_sf(char *infix) {
    int len = strlen(infix);
    char *postfix = malloc(len + 2);
    char stack[len + 1];

    int top = -1, i, j;

    for(i = 0, j = 0; i < len; i++){
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
                if(operand1 != NULL && !isalpha(operand1->name)){
                    free(operand1);
                }
            }
            else{
                operand2 = matStack[top--];
                operand1 = matStack[top--];

                if(pExpr[i] == '*'){
                    matStack[++top] = mult_mats_sf(operand1, operand2);
                }
                else{
                    matStack[++top] = add_mats_sf(operand1, operand2);
                }

                if(operand1 != NULL && !isalpha(operand1->name)){
                    free(operand1);
                }

                if(operand2 != NULL && !isalpha(operand2->name)){
                    free(operand2);
                }
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
    matrix_sf *sol = NULL, *mPtr;
    bst_sf *root = NULL;

    while(getline(&str, &max_line_size, file) != -1){
        
        char *equ = strchr(str, '=');

        if(equ == NULL)
            continue;
            
        if(strchr(str, '[') != NULL){
            mPtr = create_matrix_sf(str[0], equ + 1);
            root = insert_bst_sf(mPtr, root);
        }
        else{
            if(sol != NULL)
                free(sol);

            mPtr = evaluate_expr_sf(str[0], equ + 1, root);
            root = insert_bst_sf(mPtr, root);

            sol = copy_matrix(mPtr->num_rows, mPtr->num_cols, mPtr->values);
            sol->name = mPtr->name;
        }
        
    }

    free(str);
    free_bst_sf(root);
    fclose(file);

    return sol;
}

// This is a utility function used during testing. Feel free to adapt the code to implement some of
// the assignment. Feel equally free to ignore it.
matrix_sf *copy_matrix(unsigned int num_rows, unsigned int num_cols, int values[]) {
    matrix_sf *m = malloc(sizeof(matrix_sf)+num_rows*num_cols*sizeof(int));
    m->name = '?';
    m->num_rows = num_rows;
    m->num_cols = num_cols;
    memcpy(m->values, values, num_rows*num_cols*sizeof(int));
    return m;
}

// Don't touch this function. It's used by the testing framework.
// It's been left here in case it helps you debug and test your code.
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

