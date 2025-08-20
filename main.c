#include <stdio.h>
#include "relu.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define IMG_SIZE 100
#define FILTER_SIZE 3
#define NUM_FILTERS 2
#define POOL_SIZE 2
#define FC_SIZE 50
#define NUM_CLASSES 2
#define LEARNING_RATE 0.01
#define PADDING 1

float rand_float(){
  return ((float)rand()/ RAND_MAX) * 2 - 1;
}

float relu(float x) {return x > 0 ? x : 0;}
float relu_derv(float x){return x > 0 ? 1 : 0;}

float softmax_den[NUM_CLASSES];
//activation Functions
void softmax(float input[NUM_CLASSES], float output[NUM_CLASSES]){
  float max = input[0];
  for(int i = 1; i < NUM_CLASSES; i++) if(input[i] > max) max = input[i];
  float sum = 0;
  for (int i = 0;i< NUM_CLASSES;i++) {
    output[i] = exp(input[i]-max);
    sum += output[i];
  }
  for (int i = 0; i < NUM_CLASSES;i++) {
    output[i] /= sum;
  }

}
//Activation Functions
void conv2d(float input[IMG_SIZE][IMG_SIZE], float output[IMG_SIZE-2][IMG_SIZE-2], float filter[FILTER_SIZE][FILTER_SIZE])){
  for (int i = 0;i< IMG_SIZE - FILTER_SIZE + 1; i++) {
    for (int j = 0 ;j < IMG_SIZE - FILTER_SIZE + 1 ;j++) {
      float sum = 0;
      for(int fi = 0; fi < FILTER_SIZE; fi++)
        for (int fj = 0; fj < FILTER_SIZE; fj++)
          sum += input[i+fi][j+fj] * filter[fi][fj];
      output[i][j] = relu(sum);
    }
  }
}
//Max Pooling 2x2 operations 
void maxpool2d(float input[IMG_SIZE-2][IMG_SIZE-2], float output[(IMG_SIZE-2)/POOL_SIZE][(IMG_SIZE-2)/POOL_SIZE]){
  int out_size = (IMG_SIZE-2)/POOL_SIZE;
  for (int i = 0;i < out_size;i++) {
    for (int j = 0;j< out_size; j++) {
     float maxVal = -1e9;
     for (int pi = 0 ;pi < POOL_SIZE;pi++) {
      for(int pj = 0; pj < POOL_SIZE; pj++){
        //find max value
        if(input[POOL_SIZE*i + pi][j*POOL_SIZE + pj] > maxVal)
          //Check value of the poolsize*index +pool index 
          max_val = input[i*POOL_SIZE + pi][j*POOL_SIZE + pj];
      }
     }
     output[i][j] = maxVal;
    }
  }
}
// Turn the [x][x] matrix into 1D
void flatten(float input[(IMG_SIZE-2)/POOL_SIZE][(IMG_SIZE-2)/POOL_SIZE], float output[((IMG_SIZE-2)/POOL_SIZE))*((IMG_SIZE-2)/POOL_SIZE)]){
  int inx = 0;
  for(int i = 0;i < (IMG_SIZE-2)/POOL_SIZE; i++){
    for (int j = 0;j < (IMG_SIZE-2)/POOL_SIZE; j++) {
      output[inx++] = input[i][j];
    }
  }
}
//Forward pass for fully connected layers
void fc_forward(float input[((IMG_SIZE-2)/POOL_SIZE)*((IMG_SIZE-2)/POOL_SIZE)], float output[FC_SIZE],float weights[FC_SIZE][((IMG_SIZE-2)/POOL_SIZE)*((IMG_SIZE-2)/POOL_SIZE)], float bias[FC_SIZE]){
  for (int i = 0; i < FC_SIZE; i++) {
    float sum = bias[i];
    for (int j = 0;j < ((IMG_SIZE-2)/POOL_SIZE)*((IMG_SIZE-2)/POOL_SIZE);j++) {
      sum += weights[i][j]*input[j];
    }
    output[i] = relu(sum);
  }
}
int main(){
  srand(time(0));
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

