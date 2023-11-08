#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>


//* If we dont have enough neurons the cost of xor model will not go below 0.25
//? 2 Layers, 1st Layer containing OR and NAND gate the output of which will be given to AND gate in 2nd layer

typedef struct {
    float or_w1;
    float or_w2;
    float or_b;

    float nand_w1;
    float nand_w2;
    float nand_b;

    float and_w1;
    float and_w2;
    float and_b;
} Xor;

typedef float sample[3];

sample xor_train[] = {
    {0,0,0},
    {0,1,1},
    {1,0,1},
    {1,1,0}
};

sample or_train[] = {
    {0,0,0},
    {0,1,1},
    {1,0,1},
    {1,1,1}
};

sample and_train[] = {
    {0,0,0},
    {0,1,0},
    {1,0,0},
    {1,1,1}
};

sample nand_train[] = {
    {0,0,1},
    {0,1,1},
    {1,0,1},
    {1,1,0}
};

sample *train = xor_train;
int train_count = 4;


float sigmoidf(float x){
    return 1.f/(1.f + expf(-x));
}

float forward(Xor m, float x, float y){
    float a = sigmoidf(m.or_w1*x + m.or_w2*y + m.or_b);
    float b = sigmoidf( m.nand_w1*x + m.nand_w2*y + m.nand_b);
    return sigmoidf(m.and_w1*a + m.and_w2*b + m.and_b);
}

//! In the above forward function we have two linear equations like:-
//! a1 = x1*w1 + x2*w3 + b1
//! a2 = x1*w2 + x2*w4 + b2
//! by seeing the above two equations the intution of using matrices comes
//! sigmoid ( [x1 x2] * [ w1  w2]  + [b1  b2] ) = [a1 a2]
//!                    [ w3  w4]               


float cost(Xor m){
    float result = 0.0f;
    for(int i=0;i<train_count;i++){
    float x1 = train[i][0];
    float x2 = train[i][1];
    float y = forward(m,x1,x2);
    float d = y - train[i][2];
    result += d*d;
    }
    result /= train_count;
    return result;
}

float rand_float(){
    return (float) rand()/(float) RAND_MAX;
}

Xor rand_xor(){
    Xor m;
     m.or_w1 = rand_float();
     m.or_w2 = rand_float();
     m.or_b = rand_float();

     m.nand_w1 = rand_float();
     m.nand_w2 = rand_float();
     m.nand_b = rand_float();

     m.and_w1 = rand_float();
     m.and_w2 = rand_float();
     m.and_b = rand_float();

     return m;
}

void print_xor(Xor m){
    printf("or_w1 = %f\n",m.or_w1);
    printf("or_w2 = %f\n",m.or_w2);
    printf("or_b = %f\n",m.or_b);

    printf("nand_w1 = %f\n",m.nand_w1);
    printf("nand_w2 = %f\n",m.nand_w2);
    printf("nand_b = %f\n",m.nand_b);

    printf("and_w1 = %f\n",m.and_w1);
    printf("and_w2 = %f\n",m.and_w2);
    printf("and_b = %f\n",m.and_b);
    return;
}

Xor finite_diff(Xor m,float eps){

    Xor g;
    float c = cost(m);

    float saved = m.or_w1;
    m.or_w1 += eps;
    g.or_w1 = (cost(m) - c)/eps;
    m.or_w1 = saved;

    saved = m.or_w2;
    m.or_w2 += eps;
    g.or_w2 = (cost(m) - c)/eps;
    m.or_w2 = saved;

    saved = m.or_b;
    m.or_b += eps;
    g.or_b = (cost(m) - c)/eps;
    m.or_b = saved;
        
    saved = m.and_w1;
    m.and_w1 += eps;
    g.and_w1 = (cost(m) - c)/eps;
    m.and_w1 = saved;

    saved = m.and_w2;
    m.and_w2 += eps;
    g.and_w2 = (cost(m) - c)/eps;
    m.and_w2 = saved;

    saved = m.and_b;
    m.and_b += eps;
    g.and_b = (cost(m) - c)/eps;
    m.and_b = saved;

            
    saved = m.nand_w1;
    m.nand_w1 += eps;
    g.nand_w1 = (cost(m) - c)/eps;
    m.nand_w1 = saved;

    saved = m.nand_w2;
    m.nand_w2 += eps;
    g.nand_w2 = (cost(m) - c)/eps;
    m.nand_w2 = saved;

    saved = m.nand_b;
    m.nand_b += eps;
    g.nand_b = (cost(m) - c)/eps;
    m.nand_b = saved;

    return g;
}

Xor learn(Xor m, Xor g, float rate){

    m.or_w1 -= rate*g.or_w1;
    m.or_w2 -= rate*g.or_w2;
    m.or_b -= rate*g.or_b;

    m.nand_w1 -= rate*g.nand_w1;
    m.nand_w2 -= rate*g.nand_w2;
    m.nand_b -= rate*g.nand_b;

    m.and_w1 -= rate*g.and_w1;
    m.and_w2 -= rate*g.and_w2;
    m.and_b -= rate*g.and_b;

    return m;
}

int main(){
    srand(time(0));
    float eps = 1e-1;
    float rate = 1e-1;

    Xor m = rand_xor();

    for(int i=0;i<10000;i++){
        Xor g = finite_diff(m,eps);
        m = learn(m,g,rate);
    }
        // printf("%f\n",cost(m));
    printf("Final XOR\n");
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            printf("%d ^ %d = %f\n",i,j, forward(m,i,j));
        }
    }

    printf("------------\n");
    printf("OR-Neuron\n");
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
           printf("%d | %d = %f\n",i,j,sigmoidf(m.or_w1*i + m.or_w2*j + m.or_b)) ;
        }
    }

    printf("------------\n");
    printf("NAND-Neuron\n");
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
           printf("~(%d & %d) = %f\n",i,j,sigmoidf(m.nand_w1*i + m.nand_w2*j + m.nand_b)) ;
        }
    }

    printf("------------\n");
    printf("AND-Neuron\n");
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
           printf("%d & %d = %f\n",i,j,sigmoidf(m.and_w1*i + m.and_w2*j + m.and_b)) ;
        }
    }

    return 0;
}

//! The main observation here is that the architecture I thought would be sufficient to achieve the building of XOR gate consist of OR, AND, NAND Gate but after training the model when we traverse the 3 Neurons, they might behave completely different
//? So a Neuron could behave as any Gate other than our regular(OR, AND, NOR... etc) but when combined together they behave as XOR