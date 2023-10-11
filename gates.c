#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

//* y = x*w

float sigmoidf(float x){
    return 1.f/(1.f + expf(-x));
}    

//* Can be Used for modelling OR /AND/ NAND Gate

//! XOR Gate Cant be modelled using single neuron because after plotting the x1, x2 points, we cannot segregate them using a single line so couple of layers are required


float train[][3] = {
    {0,0,0},
    {0,1,0},
    {1,0,0},
    {1,1,1}
};  

#define train_count (sizeof(train)/sizeof(train[0]))

float rand_float(){
    return (float) rand()/(float) RAND_MAX;
}

float cost(float w1,float w2,float b){
     float result = 0.0f;
    for(int i=0;i<train_count;i++){
        float x1 = train[i][0];
        float x2 = train[i][1];
        float y = sigmoidf(x1*w1 + x2*w2 + b);
        float d = y - train[i][2];
        result += d*d;
    }
    result /= train_count;
    return result;
}

int main(){
    srand(time(0));
    float w1 = rand_float()*10.0 - 5;
    float w2 = rand_float()*10.0 - 5;
    float b = rand_float();
    

    float eps = 1e-1;
    float rate = 1e-1;

    for(int i=0;i<10000;i++){
        float c = cost(w1,w2,b);
        // printf("w1 = %f, w2 = %f, c = %f\n",w1,w2,c);
        float dw1 = (cost(w1 + eps,w2, b) - c)/eps;
        float dw2 = (cost(w1 , w2 + eps, b) - c)/eps;
        float db = (cost(w1 , w2 ,b + eps) - c)/eps;
        
        w1 -= rate*dw1;
        w2 -= rate*dw2;
        b -= rate*db;
    }

    for(int i=0;i<2;i++){
        for(int j = 0;j<2;j++){
            printf("%d | %d = %f\n",i,j,sigmoidf(i*w1 + j *w2 + b));
        }
    }
    return 0;
}
