#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#include <math.h>

int N=64;
float beta=1,dt=5e-3;

void step(float *vector, float *deriv);
void init_zeros(float *v, int size);
void compute_accel(float *pos, float *vel, float *accel);

int main (int argc, char **argv)
{
	if(argc!=2)
	{
		printf("Must introduce number of processors\n");
		exit(0);
	}
	
	//Declaracion de variables
	int num_proc = atoi(argv[1]);
	int i,j;
	float Tmax = 5*pow(N,2.2);
	float tot_steps = Tmax/dt;
	float *x,*v,*a;
	FILE *f;
	
	//Asignacion de Memoria
	if(!(x = malloc(N*N*sizeof(float))))
	{
		printf("Allocation error x\n");
	}
	if(!(v = malloc(N*sizeof(float))))
	{
		printf("Allocation error v\n");
	}
	if(!(a = malloc(N*sizeof(float))))
	{
		printf("Allocation error a\n");
	}
	
	omp_set_num_threads(num_proc);
	
	//Valores iniciales
	#pragma omp parallel for private(i), shared(x,N)
		for (i=0;i<N;i++)
		{
			x[i]=sin(M_PI*i/(N-1));
		}
	
	init_zeros(v,N);
	
	compute_accel(&x[0],v,a);
	
	#pragma omp parallel for private(i), shared(v,N)
		for (i=0;i<N;i++)
		{
			v[i]+=0.5*dt*a[i];
		}
	
	//Iterar
	for(i=1;i<tot_steps;i++)
	{
		step(&x[i*N],v);
		compute_accel(&x[i*N],v,a);
		step(v,a);
	}
	
	//Imprimir
	f = fopen("output.txt", "w");
	fprintf(f,"%d\n", N);
	for(i=0;i<N*N;i++)
	{
		fprintf(f,"%f\n", x[i]);
	}
	fclose(f);
	free(x);
	free(v);
	free(a);
	
	return 0;
}

void init_zeros(float *v, int size)
{
	int i;
	
	#pragma omp parallel for private(i), shared(size,v)
		for(i=0;i<size;i++)
		{
			v[i]=0;
		}
}
	
void compute_accel(float *pos, float *vel, float *accel)
{
	int i;
	#pragma omp parallel for private(i), shared(pos,vel,accel,N)
		for (i=1;i<N-1;i++)
		{
			accel[i]=(pos[i+1]-2*pos[i]+pos[i-1])+beta*(pow(pos[i+1]-pos[i],3)-pow(pos[i]-pos[i-1],3));
		}
}

void step(float *vector, float *deriv)
{
	int i;
	
	#pragma omp parallel for private(i), shared(vector,deriv,N)
		for(i=0;i<N;i++)
		{
			vector[i]+=deriv[i]*dt;
		}
}
