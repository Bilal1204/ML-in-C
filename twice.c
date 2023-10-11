#include<stdio.h>
#include<stdlib.h>
#include<time.h>

    // y = x*w
float train[][2] = {
    {0,0},
    {1,2},
    {2,4},
    {3,6},
    {4,8}
};

#define train_count (sizeof(train)/sizeof(train[0]))

float rand_float(){
    return (float) rand()/(float) RAND_MAX;
}

float cost(float w,float b){
     float result = 0.0f;
    for(int i=0;i<train_count;i++){
        float x = train[i][0];
        float y = x*w + b;
        float d = y - train[i][1];
        result += d*d;
    }
    result /= train_count;
    return result;
}

int main(){
    srand(69);
    float w = rand_float()*10.0f;
    float b = rand_float()*5.0f;
    

    float eps = 1e-3;
    float rate = 1e-3;

    printf("Weight: %f\n",w);
    printf("Cost: %f\n",cost(w,b));


    float d = (cost(w + eps,b) - cost(w,b))/eps;
    printf("Derivative:- %f\n",d);

    for(int i=0;i<4000;i++){
        float c = cost(w,b);
        
        float dw = (cost(w + eps,b) - c)/eps;
        float db = (cost(w , b+eps) - c)/eps;

        w -= rate*dw;
        b -= rate*db;

        printf("%d:- Weight: %f, Bias:- %f, Cost: %f\n",i+1,w,b,cost(w,b));
    }

    printf("Weight:- %f, Bias:- %f",w,b);
    return 0;
}
