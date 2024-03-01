#include <malloc.h>
#include <stdio.h>
#include <math.h>

float PInum = 3.14159265359;


float CosTaylor(float x)
{
    float sign = x > PInum && x < 2*PInum ? -1 : 1;
    x = fmod(x, PInum);
    return sign * (1-(pow(x,2)/2)+(pow(x,4)/24)-(pow(x,6)/720)+(pow(x,8)/40320)-(pow(x,10)/3628800)+(pow(x,12)/479001600)-(pow(x,14)/87178291200)+(pow(x,16)/20922789888000)-(pow(x,18)/6402373705728000)+(pow(x,20)/2432902008176640000));
}
int main()
{
    float start = 0, end = 0, deltaX = 0;
    scanf("%f", &start);
    scanf("%f", &end);
    scanf("%f", &deltaX);

    int size = (int)((end-start)/deltaX)+1;
    float** arr = malloc(sizeof(float*) * size);
    
    for(int i = 0; i < size; i++)
    {
        arr[i] = malloc(sizeof(float) * 3);
    }
    
    for(int i = 0; i < size; i++)
    {
        arr[i][0] = start + deltaX * i;
        arr[i][1] = cosf(arr[i][0]);
        arr[i][2] = CosTaylor(arr[i][0]);
    }

    for (int i = 0; i < size; i++)
    {
        printf("%f, %f, %f \n", arr[i][0], arr[i][1], arr[i][2]);
    }
    
    return 0;
}