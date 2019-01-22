#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint;

typedef struct {
	double *data;
	uint nrows, ncols;
} Matrix;

void printm(Matrix *M) {
	uint nrows = M->nrows;
	uint ncols = M->ncols;
	for(int i=0; i<nrows;i++){
		for(int j=0; j<ncols; j++){
			printf(" %.2f ", M->data[(ncols*i)+j]);
		}
		printf("\n");
	}
	printf("\n");
	
}
Matrix* alloc_matrix(uint nrows, uint ncols) {
	//Alocate memory of matriz
	Matrix* M = malloc(sizeof(Matrix));

	if(M != NULL){
		if(nrows > 0 && ncols > 0){
			M->data = (double *)malloc(nrows * ncols * sizeof(double));
			M->nrows = nrows;
			M->ncols = ncols;
			return M;
			
		}else{
			printf("Rows or columns must be grater than 0\n");
		}
	}else{
		printf("Matrix is NULL alloc_matrix\n");
	}
	return NULL;
}

void set(Matrix *M, uint row, uint col, double val) {
	if(M != NULL){
		uint nrows = M->nrows;
		uint ncols = M->ncols;
		if (row >= 0 && row < nrows){
			if (col >= 0 && col <ncols){
				M->data[(ncols*row) + col] = val;
			}else{
				printf("Error with cols!\n");
			}
		}else{
			printf("Error with rows!\n");
		}
	}
}

void matrix_mult(Matrix *A, Matrix *B, Matrix *C) {
	if (A != NULL && B != NULL){
		if (A->ncols == B->nrows){
			 int cc = C->ncols;
			 int cr = C->nrows;
			 float aux = 0;
			 
			 for(int i = 0; i<cr ; i++){
				 for(int j=0; j<cc; j++){
					 for(int k=0; k<B->nrows; k++){
						 aux += (A->data[(A->ncols*i) + k])* (B->data[(B->ncols*k) + j]);
					 }
					 
					 C->data[(C->ncols*i) + j] = aux;
					 aux = 0;
				 }
			 }
		}else{
			printf("Matrix A number of cols has to be the same with Matrix B number of rows\n");
		}
	}else{
		printf("Matrix A or B is Null\n");
	}
}

void free_matrix(Matrix *M) {
	 if(M!=NULL){
		 free(M->data);
		free(M);
	 }else{
		printf("Error to free matrix NULL\n");
	 }
}


int main(int argc, char* argv[]) {
	printf("Creating the matrix A:\n");
	Matrix *A = alloc_matrix(3, 2);
	
	printf("Setting the matrix A:\n");
	set(A, 0, 0, 1.2);
	set(A, 0, 1, 2.3);
	set(A, 1, 0, 3.4);
	set(A, 1, 1, 4.5);
	set(A, 2, 0, 5.6);
	set(A, 2, 1, 6.7);
	printf("Printing the matrix A:\n");
	printm(A);
	
	printf("Creating the matrix B:\n");
	Matrix *B = alloc_matrix(2, 3);
	printf("Setting the matrix B:\n");
	set(B, 0, 0, 5.5);
	set(B, 0, 1, 6.6);
	set(B, 0, 2, 7.7);
	set(B, 1, 0, 1.2);
	set(B, 1, 1, 2.1);
	set(B, 1, 2, 3.3);
	printf("Printing the matrix B:\n");
	printm(B);
	
	printf("Creating the matrix C:\n");
	Matrix *C = alloc_matrix(3, 3);
	printf("A x B = C:\n");
	matrix_mult(A, B, C);
	printf("Printing the matrix C:\n");
	printm(C);
	
	printf("B x A = C:\n");
	matrix_mult(B, A, C);
	printf("Printing the matrix C:\n");
	printm(C);
	
	Matrix *D = NULL;
	printf("Setting a NULL matrix (D):\n");
	set(D, 0, 0, 10);
	
	printf("A x D(NULL) = C:\n");
	matrix_mult(A, D, C);
	printf("D(NULL) x A = C:\n");
	matrix_mult(D, A, C);
	
	printf("Allocating E with (0,0): \n");
	Matrix *E = alloc_matrix(0, 0);
	
	printf("Freeing A:\n");
	free_matrix(A);
	printf("Freeing B:\n");
	free_matrix(B);
	printf("Freeing C:\n");
	free_matrix(C);
	printf("Freeing D(NULL):\n");
	free_matrix(D);
	printf("Freeing E(NULL):\n");
	free_matrix(E);

	return 0;
}
