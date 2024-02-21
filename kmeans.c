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
    int *clusterSize = (int *)malloc(K*sizeof(int));/*stores sizes of clusters.*/
    double *muTmp = (double *)malloc(K*d*sizeof(double));/*stores the acccumelated positions.*/
    int i,j;
    int result = 0;
    for(i = 0 ; i < K*d ; i++) *((double *)muTmp+i) = 0;/* initiate muTmp to zeros.*/
    for(i = 0 ; i < K ; i++) clusterSize[i]=0;
    for(i = 0 ; i < n; ++i){
        clusterSize[association[i]]++;
        for(j = 0;j<d;j++) muTmp[association[i]*d+j] += DB[i*d+j];
    }
    for(i = 0 ; i < K ; i++){
        for(j = 0 ; j < d ; j++){
            muTmp[i*d+j] /= clusterSize[i];
        }
    }

    for(i = 0 ; i < K ; i++){
        /*If even 1 of the centroids is far we keep going.*/
        if(dist(mu+(i*d),(muTmp+i*d),d)>=0.001){
            result = 1;
        }
    }
    for(i = 0 ; i < K ; i++){
        for(j = 0 ; j < d ; j++){
            *(mu+(i*d+j)) = muTmp[i*d+j];
        }
    }
    free(clusterSize);
    free(muTmp);
    return result;
}

/*kmeans*/
void kmeans(double *DB, int d, int K, int n, int iter){ 
    /*initiate mu:*/
    int count = 0;
    int deltaCondition;
    int i,j;
    double *mu = (double *)malloc(K*d*sizeof(double));
    int *association = (int *)malloc(n*sizeof(int));;
    
    for(i = 0; i < K; ++i){
        for(j = 0; j < d; ++j){
            mu[i*d+j] = DB[i*d+j];
        }
    }

    do{
        assign((double *)mu, DB, association, d, K, n);
        deltaCondition = updateMu((double *)mu, DB, (int *) association, d, K, n);
    }while(deltaCondition && (++count)<iter);

    for(i = 0 ; i < K ; i++){
        for(j = 0 ; j < d-1 ; j++){
            printf("%.4f,", mu[i*d+j]);
        }
        printf("%.4f\n", mu[i*d+d-1]);
    }
    free(mu);
    free(association);
}

int main(int argc, char **argv){
    /*inputting the terminal arguments*/
    char *nothing;
    char trash;
    int i,j;
    double K_tmp = strtod(argv[1], &nothing);
    double n_tmp = strtod(argv[2], &nothing);
    double d_tmp = strtod(argv[3], &nothing);
    double iter_tmp = strtod(argv[argc-1],&nothing);
    /*if passed all tests*/
    int K;
    int n;
    int d;
    int iter;
    double *points;
    iter = (int)iter_tmp;
    d = (int)d_tmp;
    n = (int)n_tmp;
    K = (int)K_tmp;
    points = (double *)malloc(n*d*sizeof(double));
    if(argc==5){
        if(!(iter_tmp>1)||!(iter_tmp<1000)||!(iter_tmp == (double)(int)iter_tmp)){
            printf("Invalid maximum iteration!\n");
            return 1;
        }
    }
    else{iter = 200;}
    if(!(n_tmp>1)||!(n_tmp == (double)(int)n_tmp)){
        printf("Invalid number of points!\n");
        return 1;
    }
    if(!(K_tmp>1)||!(K_tmp<n_tmp)||!(K_tmp == (double)(int)K_tmp)){
        printf("Invalid number of clusters!\n");
        return 1;
    }
    if(!(d_tmp == (double)(int)d_tmp)&&(d_tmp>0){
        printf("Invalid dimension of point!\n");
        return 1;
    }
    else{}
    /*inputting the points*/
    for(i = 0 ; i < n ; i++){
        for (j = 0; j < d-1; ++j) {                
            scanf("%lf", &points[i*d+j]);
            scanf("%c", &trash);
        }
        scanf("%lf\n", &points[i*d+d-1]);
    }
    kmeans((double *)points, d, K, n, iter);
    return 0;
}
