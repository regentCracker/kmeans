#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*distance between 2 points*/
double dist(double *p, double *q, int d){
    double sumofsquares = 0;
    int i;
    for(i = 0 ; i < d ; ++i){
        sumofsquares += pow(*(p+i)-*(q+i), 2);
    }
    return sqrt(sumofsquares);
}

/*assign to xi the cluster. returns the index.*/
int clusterSelection(double *x, double *mu, int K, int d){
    double minDist = dist(x, mu, d);/*the first mu*/
    int index = 0;
    int i;
    for(i = 1; i < K ; ++i){
        double tmp=dist(x, mu+(i*d), d);
        if(tmp < minDist) {
            minDist = tmp;
            index = i;
        }
    }
    return index;
}

/*assign to all x the clusters*/
void assign(double *mu, double *DB, int *association, int d, int K, int n){
    int i;
    for(i = 0 ; i < n ; i++){
        *(association+i) = clusterSelection(DB+(i*d),mu,K,d);/*the ith point.*/
    }
}

/*update mu and returns if the delta condition is violated
// 1 - keep going
// 0 - stop:*/
int updateMu(double *mu, double *DB, int *association, int d, int K, int n){
    int clusterSize[K];/*stores sizes of clusters.*/
    double muTmp[K][d];/*stores the acccumelated positions.*/
    int i,j;
    for(i = 0 ; i < K*d ; i++) *((double *)muTmp+i) = 0;/* initiate muTmp to zeros.*/
    for(i = 0 ; i < K ; i++) clusterSize[i]=0;
    for(i = 0 ; i < n; ++i){
        clusterSize[association[i]]++;
        for(j = 0;j<d;j++) muTmp[association[i]][j] += DB[i*d+j];
    }
    for(i = 0 ; i < K ; i++){
        for(j = 0 ; j < d ; j++){
            muTmp[i][j] /= clusterSize[i];
        }
    }

    int result = 0;
    for(i = 0 ; i < K ; i++){
        /*If even 1 of the centroids is far we keep going.*/
        if(dist(mu+(i*d),muTmp[i],d)>=0.001){
            result = 1;
        }
    }
    for(i = 0 ; i < K ; i++){
        for(j = 0 ; j < d ; j++){
            *(mu+(i*d+j)) = muTmp[i][j];
        }
    }
    
    return result;
}

/*kmeans*/
void kmeans(double *DB, int d, int K, int n, int iter){ 
    /*initiate mu:*/
    double mu[K][d];
    int i,j;
    for(i = 0; i < K; ++i){
        for(j = 0; j < d; ++j){
            mu[i][j] = DB[i*d+j];
        }
    }
    
    int association[n];
    int count = 0;
    int deltaCondition;

    do{
        assign((double *)mu, DB, association, d, K, n);
        deltaCondition = updateMu((double *)mu, DB, (int *) association, d, K, n);
    }while(deltaCondition && (++count)<iter);

    for(i = 0 ; i < K ; i++){
        for(j = 0 ; j < d-1 ; j++){
            printf("%.4f,", mu[i][j]);
        }
        printf("%.4f\n", mu[i][d-1]);
    }

}

int main(int argc, char **argv){
    /*inputting the terminal arguments*/
    double K_tmp, n_tmp, d_tmp, iter_tmp;
    char *nothing;
    int i,j;
    K_tmp = strtod(argv[1], &nothing);
    n_tmp = strtod(argv[2], &nothing);
    d_tmp = strtod(argv[3], &nothing);
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
    /*passed all tests*/
    int K = (int)K_tmp;
    int n = (int)n_tmp;
    int d = (int)d_tmp;
    int iter = (int)iter_tmp;
    /*inputting the points*/
    char trash;
    double points[n][d];
    for(i = 0 ; i < n ; i++){
        for (j = 0; j < d-1; ++j) {                
            scanf("%lf", &points[i][j]);
            scanf("%c", &trash);
        }
        scanf("%lf\n", &points[i][d-1]);
    }
    kmeans((double *)points, d, K, n, iter);
    return 0;
}