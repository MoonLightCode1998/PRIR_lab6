#include <stdlib.h>
#include "mpi.h"
#include <stdio.h>
#include <math.h>
double oblicz(int n){
	return pow(-1,n-1)/(2*n-1);
}
int main(int argc, char** argv ){
int rank,size;
int to;
double data;
MPI_Status status;
//size = atoi(argv[1]);
MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD,&size);
MPI_Comm_rank(MPI_COMM_WORLD,&rank);
if(rank==0){//test
}
else if(rank==1){
data=1;
MPI_Send(&data,1,MPI_DOUBLE,rank+1,200,MPI_COMM_WORLD);}
else if(rank<size-1){
MPI_Recv(&data,1,MPI_DOUBLE,rank-1,200,MPI_COMM_WORLD,&status);
//data+=1;
//printf("data = %f from: %d\n",data,rank);
data+=oblicz(rank);
MPI_Send(&data,1,MPI_DOUBLE,rank+1,200,MPI_COMM_WORLD);
}else{
MPI_Recv(&data,1,MPI_DOUBLE,rank-1,200,MPI_COMM_WORLD,&status);
data+=oblicz(rank)+oblicz(rank+1);
data = data*4;
printf("Przybzenie pi =: %f\n",data);
//printf("data = %f from: %d\n",data,rank);
}


MPI_Finalize();



return 0;
}
