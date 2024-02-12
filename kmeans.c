#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int DEBUG = 0;

//distance between 2 points
float dist(float *p, float *q, int d){
    float sumofsquares = 0;
    if(DEBUG) printf("error here 1\n");
    
    for(int i = 0 ; i < d ; ++i){
        if(DEBUG) printf("asd\n");
        float bruh = *p;
        if(DEBUG) printf("%f\n", bruh);
        sumofsquares += pow(p[i]-q[i], 2);
    }
    return sqrt(sumofsquares);
}

//assign to xi the cluster. returns the index.
int clusterSelection(float *x, float *mu, int K, int d){
    if(DEBUG) printf("error here 2\n");
    float minDist = dist(x, mu, d);//the first mu
    if(DEBUG) printf("17\n");
    int index = 0;
    for(int i = 1; i < K ; ++i){
        if(DEBUG) printf("error here 3\n");
        float tmp=dist(x, mu+d, d);
        if(tmp < minDist) {
            minDist = tmp;
            index = i;
        }
    }
    return index;
}

//assign to all x the clusters
void assign(float *mu, float *DB, int *association, int d, int K, int n){
    for(int i = 0 ; i < n ; i++){
        if(DEBUG) printf("error here 4\n");
        association[i] = clusterSelection(DB+(i*d),mu,K,d);//the ith point.
    }
}

//update mu and returns if the delta condition is violated
// 0 - keep going
// 1 - stop:
int updateMu(float *mu, float *DB, int *association, int d, int K, int n){
    if(DEBUG) printf("error here 5");
    int clusterSize[K];//stores sizes of clusters.
    float muTmp[K][d];//stores the acccumelated positions.
    for(int i = 0 ; i < K ; i++) clusterSize[i]=0;
    for(int i = 0 ; i < n; ++i){
        clusterSize[association[i]]++;
        for(int j = 0;j<d;j++) muTmp[association[i]][j] += DB[i+d*j];
    }
    for(int i = 0 ; i < K ; i++){
        for(int j = 0;j<d;j++) muTmp[i][j] /= clusterSize[i];
    }
    for(int i = 0 ; i < K ; i++){
        //If even 1 of the centroids is far we keep going.
        if(dist(mu+(i*d),muTmp[i],d)>=0.001) return 0;
    }
    for(int i = 0 ; i < K ; i++){
        for(int j = 0 ; j < d ; j++){
            *(mu+(i*d+j)) = muTmp[i][j];
        }
    }
    return 1;
}

//kmeans
void kmeans(float *DB, int d, int K, int n, int iter){ 
    //initiate mu:
    if(DEBUG) printf("error here 6\n");
    float mu[K][d];
    for(int i = 0; i < K; ++i){
        for(int j = 0; j < d; ++j){
            //printf("error here 7\n");
            mu[i][j] = DB[i+d*j];
            //printf("huh?1");
            //printf("%d %d\n", i, j);
        }
        if(DEBUG) printf("huh?\n");
    }
    if(DEBUG) printf("gj\n");
    
    int association[n];
    int count = 0;
    do{
        assign((float *)mu, DB, association, d, K, n);
    }while(!updateMu((float *)mu, DB, (int *) association, d, K, n) && (++count)<iter);
    if(DEBUG) printf("82\n");
    for(int i = 0 ; i < K ; i++){
        for(int j = 0 ; j < d-1 ; j++){
            printf("%.4f, ", mu[i][j]);
        }
        printf("%.4f\n", mu[i][d-1]);
    }


}

int main(int argc, char **argv){
    //inputting the terminal arguments
    double K_tmp, n_tmp, d_tmp, iter_tmp;
    char *nothing;
    K_tmp = strtod(argv[1], &nothing);
    n_tmp = strtod(argv[2], &nothing);
    d_tmp = strtod(argv[3], &nothing);
    char* tmp_str;
    if(argc==5){
        iter_tmp = strtod(argv[4],&nothing);
        if(!(iter_tmp>1)||!(iter_tmp<1000)||!(iter_tmp == (double)(int)iter_tmp)){
            printf("Invalid maximum iteration!");
            return 0;
        }
    }
    else{iter_tmp = 200;}
    if(!(n_tmp>1)||!(n_tmp == (double)(int)n_tmp)){
        printf("Invalid number of points!");
        return 0;
    }
    if(!(K_tmp>1)||!(K_tmp<n_tmp)||!(K_tmp == (double)(int)K_tmp)){
        printf("Invalid number of clusters!");
        return 0;
    }
    if(!(d_tmp == (double)(int)d_tmp)){
        printf("Invalid dimension of point!");
        return 0;
    }
    else{}
    //passed all tests
    int K = (int)K_tmp;
    int n = (int)n_tmp;
    int d = (int)d_tmp;
    int iter = (int)iter_tmp;
    //inputting the points
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
        scanf("%f\n", &points[i][d-1]);
        //printf("%d\n",i);
    }
    if(DEBUG) printf("error here 7\n");
    kmeans((float *)points, d, K, n, iter);
    if(DEBUG) printf("error here 8\n");
    return 0;
}