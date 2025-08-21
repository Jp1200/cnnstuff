#include <stdio.h>
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
void conv2d(float input[IMG_SIZE][IMG_SIZE], float output[IMG_SIZE-2][IMG_SIZE-2], float filter[FILTER_SIZE][FILTER_SIZE]) {
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
          maxVal = input[i*POOL_SIZE + pi][j*POOL_SIZE + pj];
      }
     }
     output[i][j] = maxVal;
    }
  }
}
// Turn the [x][x] matrix into 1D
void flatten(float input[(IMG_SIZE-2)/POOL_SIZE][(IMG_SIZE-2)/POOL_SIZE], float output[((IMG_SIZE-2)/POOL_SIZE)*((IMG_SIZE-2)/POOL_SIZE)]){
  int inx = 0;
  for(int i = 0;i < (IMG_SIZE-2)/POOL_SIZE; i++){
    for (int j = 0;j < (IMG_SIZE-2)/POOL_SIZE; j++) {
      output[inx++] = input[i][j];
    }
  }
}
//Forward pass for fully connected layers
void fc_forward(float input[((IMG_SIZE-2)/POOL_SIZE)*((IMG_SIZE-2)/POOL_SIZE)], float weights[FC_SIZE][((IMG_SIZE-2)/POOL_SIZE)*((IMG_SIZE-2)/POOL_SIZE)], float bias[FC_SIZE], float output[FC_SIZE]) {
    for(int i = 0; i < FC_SIZE; i++) {
        float sum = bias[i];
        for(int j = 0; j < ((IMG_SIZE-2)/POOL_SIZE)*((IMG_SIZE-2)/POOL_SIZE); j++){
            sum += weights[i][j] * input[j];
        }
        output[i] = relu(sum);
    }
}
int main(){
  clock_t start_time, end_time;
  start_time = clock();
  srand(time(0));
  float image[IMG_SIZE][IMG_SIZE];
  for (int i = 0; i < IMG_SIZE; i++) {
    for (int j = 0; j < IMG_SIZE; j++) {
      image[i][j] = rand_float();
      //todo for training downsize images to 100x100 pixel grayscale
      //instead of random float images
    }
  }
  //convolution layers
  float filters[NUM_FILTERS][FILTER_SIZE][FILTER_SIZE];
  for (int f = 0; f < NUM_FILTERS; f++) {
    for (int i = 0; i < FILTER_SIZE; i++) {
      for (int j = 0; j < FILTER_SIZE; j++) {
        filters[f][i][j] = rand_float();
      }
    }
  }
  float conv_output[NUM_FILTERS][IMG_SIZE-2][IMG_SIZE-2];
  for (int f = 0; f < NUM_FILTERS; f++) {
    conv2d(image, conv_output[f],filters[f]);
  }
  //pooling 
  float pool_out[NUM_FILTERS][(IMG_SIZE-2)/POOL_SIZE][(IMG_SIZE-2)/POOL_SIZE];
  for (int f = 0; f < NUM_FILTERS; f++) {
    maxpool2d(conv_output[f],pool_out[f]);
  }
  //flatten and combine
  float flattened[NUM_FILTERS*((IMG_SIZE-2)/POOL_SIZE)*((IMG_SIZE-2)/POOL_SIZE)];
  int idx = 0;
  for (int f = 0; f < NUM_FILTERS; f++) {
    for (int i = 0; i < (IMG_SIZE-2)/POOL_SIZE; i++) {
      for (int j = 0; j < (IMG_SIZE-2)/POOL_SIZE; j++) {
        flattened[idx++] = pool_out[f][i][j];
      }
    }
  }

 
 float fc_weights[FC_SIZE][((IMG_SIZE-2)/POOL_SIZE)*((IMG_SIZE-2)/POOL_SIZE)];
    float fc_bias[FC_SIZE];
    for(int i = 0; i < FC_SIZE; i++) {
        fc_bias[i] = rand_float();
        for(int j = 0; j < ((out_size)/POOL_SIZE)*((IMG_SIZE-2)/POOL_SIZE); j++)
            fc_weights[i][j] = rand_float();
    }
  float fc_out[FC_SIZE];
  fc_forward(flattened, fc_weights, fc_bias, fc_out);
// Output layer (softmax)
  float output[NUM_CLASSES] = {0};
  float out_weights[NUM_CLASSES][FC_SIZE];
  float out_bias[NUM_CLASSES];
  for(int i = 0; i < NUM_CLASSES; i++) {
    out_bias[i] = rand_float();
    for(int j = 0; j < FC_SIZE; j++)
        out_weights[i][j] = rand_float();
  }

  // Compute final logits
  float logits[NUM_CLASSES];
  for(int i = 0; i < NUM_CLASSES; i++) {
    logits[i] = out_bias[i];
    for(int j = 0; j < FC_SIZE; j++){
      logits[i] += out_weights[i][j] * fc_out[j];
    }
  }

  // Softmax probabilities
  softmax(logits, output);

  printf("Output probabilities:\n");
  for(int i = 0; i < NUM_CLASSES; i++)
        printf("Class %d: %.4f\n", i, output[i]);
  end_time = clock();
  double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
  printf("Time in Seconds to execute: %f\n", time_taken);
  return 0;
}

