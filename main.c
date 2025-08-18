#include <stdio.h>
#include "relu.h"
#include <stdlib.h>
int main(){
  printf("ReLU Test Functions for CNN development\n");
  double data[] = {1.0, -2.0, 3.0, -4.0, 0.5};
  int rows = 3, cols = 3;
  double *matrix = malloc(rows * cols * sizeof(double));
  //Example init
  double temp[9] = {37, -92, 0, -15, 84, 23, 100, -7, -68};
  for(int i = 0; i < rows * cols; i++) matrix[i]=temp[i];
  int data_size = sizeof(data) / sizeof(data[0]);
  printf("\nOrginal array: ");
  for (int i = 0; i < data_size; i++) {
    printf("%.2f ", data[i]);
  }
  printf("\n");
  relu_array(data, data_size);
  printf("Array after RELU: ");
  for (int i = 0; i< data_size; i++){
    printf("%.2f ", data[i]);
  }
  printf("\nDouble Matrix");
  printf("\nAfter Relu: ");
  relu_array_matrix(matrix, rows, cols);
  for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f ", matrix[i * cols + j]);
        }
        printf("\n");
    }
  printf("\n");
  free(matrix);

    return 0;
}

