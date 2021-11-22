#include <stdlib.h>
#include "math.h"
#include "mpi.h"
#include <stdio.h>

double func(double x){
	return pow(x,2);
}

double prostokaty(int a, double h, int rank){

return ((a+(rank+1)*h)-(a+rank*h))*func(a+rank*h);

}
double trapezy(int a , double h , int rank){

	return ((func(a+(rank+1)*h)+(func(a+rank*h)))*
		(((a+h*(rank+1))-(a+h*rank))/2));
}

int main(int argc, char** argv){
int rank,size;
int a,b;
a =atoi(argv[1]);
b=atoi(argv[2]);

//printf("%d %d ",a,b);

MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&rank);
MPI_Comm_size(MPI_COMM_WORLD,&size);
MPI_Status status;
double wynik;
double h;
double wyniktrapez;
if(rank==size-1){
h = (b-a)/(double)size;
//printf("%f\n",h);
//printf("%d\n",rank);
 wynik = prostokaty(a,h,rank);
//printf("%f\n",wynik);
wyniktrapez = trapezy(a,h,rank);

MPI_Send(&wynik,1,MPI_DOUBLE,rank-1,200,MPI_COMM_WORLD);
MPI_Send(&h,1,MPI_DOUBLE,rank-1,201,MPI_COMM_WORLD);
MPI_Send(&wyniktrapez,1,MPI_DOUBLE,rank-1,202,MPI_COMM_WORLD);

}else if(rank>0){
MPI_Recv(&wynik,1,MPI_DOUBLE,rank+1,200,MPI_COMM_WORLD,&status);
MPI_Recv(&h,1,MPI_DOUBLE,rank+1,201,MPI_COMM_WORLD,&status);
MPI_Recv(&wyniktrapez,1,MPI_DOUBLE,rank+1,202,MPI_COMM_WORLD,&status);

wynik+=prostokaty(a,h,rank);
wyniktrapez+=trapezy(a,h,rank);

MPI_Send(&wynik,1,MPI_DOUBLE,rank-1,200,MPI_COMM_WORLD);
MPI_Send(&h,1,MPI_DOUBLE,rank-1,201,MPI_COMM_WORLD);
MPI_Send(&wyniktrapez,1,MPI_DOUBLE,rank-1,202,MPI_COMM_WORLD);

}else if(rank==0){
MPI_Recv(&wynik,1,MPI_DOUBLE,rank+1,200,MPI_COMM_WORLD,&status);
MPI_Recv(&wyniktrapez,1,MPI_DOUBLE,rank+1,202,MPI_COMM_WORLD,&status);
MPI_Recv(&h,1,MPI_DOUBLE,rank+1,201,MPI_COMM_WORLD,&status);
wynik+=prostokaty(a,h,rank);
wyniktrapez+=trapezy(a,h,rank);
printf("wynik calkowania trapezami to: %f\n",wyniktrapez);
printf("wynik calkowania to: %f\n",wynik);
}


MPI_Finalize();

return 0;
}
