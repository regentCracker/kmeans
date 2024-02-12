#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main(){
    int a = 0;
    printf("%d", (&(a++))++);
    /*int n = 800;
    int d = 3;
    char trash;
    float points[n][d];

    for(int i = 0 ; i < n ; i++){
        for (int j = 0; j < d-1; ++j) {                
            scanf("%f", &points[i][j]);
            scanf("%c", &trash);
            //printf("%d %d",j ,d);
            //scanf("%s,", &tmp_str);
            //points[i][j] = strtod(tmp_str, &nothing);
        }
        scanf("%f", &points[i][d-1]);
        //printf("%d\n",i);
    }*/
    return 0;
}
//gcc kmeans.c -o kmeans
// ./kmeans 45