#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int DEBUG = 0;

//distance between 2 points
double dist(double *p, double *q, int d){
    printf("welcome to dist!\n");
    double sumofsquares = 0;
    
    for(int i = 0 ; i < d ; ++i){
        printf("%lf ",*(p+i));
    }
    printf("\n");
    for(int i = 0 ; i < d ; ++i){
        printf("%lf ",*(q+i));
    }
    printf("\n");

    for(int i = 0 ; i < d ; ++i){
        sumofsquares += pow(*(p+i)-*(q+i), 2);
    }
    printf("\n");
    return sqrt(sumofsquares);
}

//assign to xi the cluster. returns the index.
int clusterSelection(double *x, double *mu, int K, int d){
    printf("welcome to clusterSelction for x=:");
    for(int i = 0 ; i < d; i++) printf("%lf ",*(x+i));
    printf("\n");
    double minDist = dist(x, mu, d);//the first mu
    printf("min dist: %lf\n", minDist);
    int index = 0;
    for(int i = 1; i < K ; ++i){
        double tmp=dist(x, mu+(i*d), d);
        printf("%lf\n", tmp);
        if(tmp < minDist) {
            minDist = tmp;
            index = i;
        }
    }
    return index;
}

//assign to all x the clusters
void assign(double *mu, double *DB, int *association, int d, int K, int n){
    for(int i = 0 ; i < n ; i++){
        printf("%d\n", clusterSelection(DB+(i*d),mu,K,d));
        *(association+i) = clusterSelection(DB+(i*d),mu,K,d);//the ith point.
    }
}

//update mu and returns if the delta condition is violated
// 1 - keep going
// 0 - stop:
int updateMu(double *mu, double *DB, int *association, int d, int K, int n){
    printf("updating...\n");
    printf("association:\n");
    for(int i = 0 ; i < n ; i++) printf("%d, ", association[i]);
    printf("\n");
    int clusterSize[K];//stores sizes of clusters.
    double muTmp[K][d];//stores the acccumelated positions.
    for(int i = 0 ; i < K*d ; i++) *((double *)muTmp+i) = 0;// initiate muTmp to zeros.
    for(int i = 0 ; i < K ; i++) clusterSize[i]=0;
    for(int i = 0 ; i < n; ++i){
        clusterSize[association[i]]++;
        for(int j = 0;j<d;j++) muTmp[association[i]][j] += DB[i*d+j];
    }
    for(int i = 0 ; i < K ; i++){
        for(int j = 0 ; j < d ; j++){
            muTmp[i][j] /= clusterSize[i];
            printf("%lf ", muTmp[i][j]);
        }
        printf("----\n");
    }

    int result = 0;
    for(int i = 0 ; i < K ; i++){
        printf("%lf, %lf, %lf\n", *(mu+i*d), *(mu+1+i*d), *(mu+2+i*d));
        //If even 1 of the centroids is far we keep going.
        if(dist(mu+(i*d),muTmp[i],d)>=0.001){
            result = 1;
        }
    }
    for(int i = 0 ; i < K ; i++){
        for(int j = 0 ; j < d ; j++){
            *(mu+(i*d+j)) = muTmp[i][j];
        }
    }
    
    return result;
}

//kmeans
void kmeans(double *DB, int d, int K, int n, int iter){ 
    //initiate mu:
    double mu[K][d];
    for(int i = 0; i < K; ++i){
        for(int j = 0; j < d; ++j){
            mu[i][j] = DB[i*d+j];
        }
    }
    
    int association[n];
    int count = 0;
    int deltaCondition;

    do{
        assign((double *)mu, DB, association, d, K, n);
        printf("BRUH:\n");
        for(int i = 0 ; i < n ; i++) printf("%d, ", association[i]);
        printf("\n");
        deltaCondition = updateMu((double *)mu, DB, (int *) association, d, K, n);
        printf("%d, %d\n", deltaCondition, count);
    }while(deltaCondition && (++count)<iter);
    
    printf("\n\n\n\n%d\n", count);

    for(int i = 0 ; i < K ; i++){
        for(int j = 0 ; j < d-1 ; j++){
            printf("%.4f,", mu[i][j]);
        }
        printf("%.4f\n", mu[i][d-1]);
    }

    //prints to a file named curout.txt as well:
    FILE *fp = fopen("curout.txt", "w");
    for(int i = 0 ; i < K ; i++){
        for(int j = 0 ; j < d-1 ; j++){
            fprintf(fp, "%.4f,", mu[i][j]);
        }
        fprintf(fp, "%.4f\n", mu[i][d-1]);
    }
    fclose(fp);


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
    double points[n][d];
    for(int i = 0 ; i < n ; i++){
        for (int j = 0; j < d-1; ++j) {                
            scanf("%lf", &points[i][j]);
            scanf("%c", &trash);
        }
        scanf("%lf\n", &points[i][d-1]);
    }
    kmeans((double *)points, d, K, n, iter);
    return 0;
}