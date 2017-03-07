#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#include <math.h>

int N=64;
double beta=1,dt=5e-3;

void step(float *vector, float *deriv, float mult);
void init_zeros(float *v, int size);
void compute_accel(float *pos, float *vel, float *accel);
int save_vec(float *vector, float *matrix, int index);

int main (int argc, char **argv)
{
	if(argc!=2)
	{
		printf("Must introduce number of processors\n");
		exit(0);
	}
	
	//Declaracion de variables
	int num_proc = atoi(argv[1]);
	int i,j,k=0;
	float Tmax = 5*pow(N,2.2);
	int tot_steps = (int) Tmax/dt, dif = (int) tot_steps/N, t_steps=1000;
	float *x,*v,*a,*x_mat,*E1,*E2,*E3;
	FILE *f;
	
	//Asignacion de Memoria
	if(!(x_mat = malloc(N*(t_steps+1)*sizeof(float))))
	{
		printf("Allocation error x_mat\n");
	}
	if(!(v = malloc(N*sizeof(float))))
	{
		printf("Allocation error v\n");
	}
	if(!(a = malloc(N*sizeof(float))))
	{
		printf("Allocation error a\n");
	}
	if(!(x = malloc(N*sizeof(float))))
	{
		printf("Allocation error x\n");
	}
	if(!(E1 = malloc((t_steps+1)*sizeof(float))))
	{
		printf("Allocation error E1\n");
	}
	if(!(E2 = malloc((t_steps+1)*sizeof(float))))
	{
		printf("Allocation error E2\n");
	}
	if(!(E3 = malloc((t_steps+1)*sizeof(float))))
	{
		printf("Allocation error E3\n");
	}
	
	omp_set_num_threads(num_proc);
	
	//Valores iniciales
	#pragma omp parallel for private(i), shared(x,N)
		for (i=0;i<N;i++)
		{
			x[i]=sin(M_PI*i/(N-1));
		}
	
	//init_zeros(v,N);
	
	compute_accel(x,v,a);
	
	#pragma omp parallel for private(i), shared(v,N)
		for (i=0;i<N;i++)
		{
			v[i]=0.5*dt*a[i];
		}
	
	save_energies(
	k=save_vec(x,x_mat,k);
	
	dif = (int) tot_steps/t_steps;
	//Iterar
	for(i=1;i<tot_steps;i++)
	{
		step(x,v,1.0);
		compute_accel(x,v,a);
		step(v,a,1.0);
		//printf("step: %d\n",i);
		if(i%dif==0)
		{
			k=save_vec(x,x_mat,k);
			//printf("%d\n",k);
		}
	}
	
	//Imprimir
	f = fopen("output.txt", "w");
	E = fopen("Energies.txt", "w");
	fprintf(f,"%d\n", N);
	fprintf(f,"%d\n", t_steps+1);
	for(i=0;i<t_steps+1;i++)
	{
		for(j=0;j<N;j++)
		{
			fprintf(f,"%f\n", x_mat[i*N+j]);	
		}
	}
	fclose(E);
	fclose(f);
	free(x);
	free(x_mat);
	free(v);
	free(a);
	free(E1);
	free(E2);
	free(E3);
	
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
			//printf("%f\n",accel[i]);
		}
	//getchar();
}

void step(float *vector, float *deriv, float mult)
{
	int i;
	
	#pragma omp parallel for private(i), shared(vector,deriv,N)
		for(i=0;i<N;i++)
		{
			vector[i]+=mult*deriv[i]*dt;
			//printf("%f\n",vector[i]);
		}
	//getchar();
}

int save_vec(float *vector, float *matrix, int index)
{
	int i;
	#pragma omp parallel for private(i), shared(vector,matrix,index)
		for(i=0;i<N;i++)
		{
			matrix[index*N+i]=vector[i];
			//printf("%f\n",matrix[index*N+i]);
		}
	//getchar();
	return ++index;
}
