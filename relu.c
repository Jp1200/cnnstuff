#include "relu.h"
#include <stdio.h>
// The rectified linear Unit is a popular activation function in neural networks. 
// it introduces non-linearity and is defined as:
// f(x) = max(0,x)
// This means that if the input x is positive, the output is x. 
// If the input x, is negative, the output is zero
double relu(double x){
  return (x > 0) ? x : 0;
}

void relu_array(double* arr, int size){
  for (int i = 0; i < size; i++) {
    arr[i] = arr[i] > 0 ? arr[i] : 0; // if arr[i] is pos return it else return 0
  }
}
void relu_array_matrix(double *arr, int rows, int cols){
 for (int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++){
      int index = i * cols + j; // convert 2D index to 1D
      arr[index] = arr[index] > 0 ? arr[index] : 0;
    }
 }
}
