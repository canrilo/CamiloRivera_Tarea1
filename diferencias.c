#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#include <math.h>

int N=64, modes=3, num_proc ;
float beta=1,dt=5e-3;
float *w;

void save_energies(float *Energies,float *pos, float *vel, int index);
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
	int i,j,k=0;
	float Tmax = 5*pow(N,2.2);
	int tot_steps = (int) Tmax/dt, dif = (int) tot_steps/N, t_steps=1000;
	float *x,*v,*a,*x_mat,*En;
	FILE *f, *E;
	
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
	if(!(En = malloc(modes*(t_steps+1)*sizeof(float))))
	{
		printf("Allocation error En\n");
	}
	if(!(w = malloc(modes*sizeof(float))))
	{
		printf("Allocation error w\n");
	}

	
	//Valores iniciales
	num_proc = atoi(argv[1]);
	omp_set_num_threads(num_proc);
	#pragma omp parallel for private(i), shared(x,N)
		for (i=0;i<N;i++)
		{
			x[i]=sin(M_PI*i/(N-1));
		}
	
	//init_zeros(v,N);
	
	for(i=0;i<modes;i++)
	{
		w[i]=4.0*pow(sin((i+1)*M_PI)/(2.0*N),2.0);
	}
	
	compute_accel(x,v,a);
	
	#pragma omp parallel for private(i), shared(v,N)
		for (i=0;i<N;i++)
		{
			v[i]=0.5*dt*a[i];
		}
	
	save_energies(En,x,v,k);
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
			save_energies(En,x,v,k);
			k=save_vec(x,x_mat,k);
			//printf("%d\n",k);
		}
	}
	
	//Imprimir
	f = fopen("output.txt", "w");
	E = fopen("Energies.txt", "w");
	fprintf(f,"%d\n", N);
	fprintf(f,"%d\n", t_steps+1);
	fprintf(E,"%d\n", modes);
	fprintf(E,"%d\n", t_steps+1);
	for(i=0;i<t_steps+1;i++)
	{
		for(j=0;j<N;j++)
		{
			fprintf(f,"%f\n", x_mat[i*N+j]);	
		}
		for(j=0;j<modes;j++)
		{
			fprintf(E,"%f\n", En[i*modes+j]);	
		}
		
	}
	fclose(E);
	fclose(f);
	free(x);
	free(x_mat);
	free(v);
	free(a);
	free(En);
	free(w);
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
			accel[i]=(pos[i+1]-2.0*pos[i]+pos[i-1])+beta*(pow(pos[i+1]-pos[i],3)-pow(pos[i]-pos[i-1],3));
			//printf("%f\n",accel[i]);
		}
	//getchar();
}

void step(float *vector, float *deriv, float mult)
{
	//omp_set_num_threads(num_proc);
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

void save_energies(float *Energies,float *pos, float *vel,int index)
{
	int i,j;
	float sumx,sumv;
	for(j=0;j<modes;j++)
	{
		sumx=0.0;
		sumv=0.0;
		#pragma omp parallel for private(i), shared(Energies,pos,vel,index,j), reduction(+:sumx,sumv)
			for(i=0;i<N;i++)
			{
				sumx = sumx + pos[i]*sin(M_PI*(j+1)*(i+1)/N);
				sumv = sumv + vel[i]*sin(M_PI*(j+1)*(i+1)/N);	
			}
		Energies[index*modes+j]=(w[j]*pow(sumx,2.0)+pow(sumv,2.0))/N;
		//printf("%f\n",sqrt(2.0/N));
		//getchar();
	}
}
