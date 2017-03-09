#include <stdio.h>
#include <stdlib.h>
#include "omp.h"
#include <math.h>
#include <time.h>

int N=64, modes=3, num_proc;
double beta=1.0,dt=5e-3;
double *w;

void save_energies(double *Energies,double *pos, double *vel, int index);
void step(double *vector, double *deriv, double mult);
void init_zeros(double *v);
void compute_vel(double *pos, double *vel, double mult);
int save_vec(double *vector, double *matrix, int index);

int main (int argc, char **argv)
{
	clock_t start = clock();
	if(argc!=2)
	{
		printf("Must introduce number of processors\n");
		exit(0);
	}
	
	//Declaracion de variables
	int i,j,k=0;
	double Tmax = 5.0*pow(N,2.2);
	int tot_steps = (int) Tmax/dt, dif = (int) tot_steps/N, t_steps=999;
	double *x,*v,*x_mat,*En;
	FILE *f, *E, *T;
	
	//Asignacion de Memoria
	if(!(x_mat = malloc(N*(t_steps+1)*sizeof(double))))
	{
		printf("Allocation error x_mat\n");
	}
	if(!(v = malloc(N*sizeof(double))))
	{
		printf("Allocation error v\n");
	}
	if(!(x = malloc(N*sizeof(double))))
	{
		printf("Allocation error x\n");
	}
	if(!(En = malloc(modes*(t_steps+1)*sizeof(double))))
	{
		printf("Allocation error En\n");
	}
	if(!(w = malloc(modes*sizeof(double))))
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
	
	for(i=0;i<modes;i++)
	{
		w[i]=4.0*pow(sin((i+1)*M_PI/(2.0*N)),2.0);
	}
	
	init_zeros(v);
	
	save_energies(En,x,v,k);
	k=save_vec(x,x_mat,k);
	
	compute_vel(x,v,0.5);
	
	dif = (int) tot_steps/t_steps;
	
	//Iterar
	for(i=1;i<tot_steps;i++)
	{
		step(x,v,1.0);
		if(i%dif==0)
		{
			save_energies(En,x,v,k);
			k=save_vec(x,x_mat,k);
		}
		compute_vel(x,v,1.0);
		
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
	free(En);
	free(w);
	
	clock_t end = clock();
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	T = fopen("Times.txt", "a");
	fprintf(T,"%d\t%f\n",num_proc,seconds);
	fclose(T);
	
	return 0;
}

void init_zeros(double *v)
{
	int i;
	#pragma omp parallel for private(i), shared(v)
		for(i=0;i<N;i++)
		{
			v[i]=0;
		}
}
	
void compute_vel(double *pos, double *vel, double mult)
{
	int i;
	#pragma omp parallel for private(i), shared(pos,vel)
		for (i=1;i<N-1;i++)
		{
			vel[i]+=dt*mult*((pos[i+1]-2.0*pos[i]+pos[i-1])+beta*(pow(pos[i+1]-pos[i],3.0)-pow(pos[i]-pos[i-1],3.0)));
		}
}

void step(double *vector, double *deriv, double mult)
{
	int i;
	#pragma omp parallel for private(i), shared(vector,deriv)
		for(i=0;i<N;i++)
		{
			vector[i]+=mult*deriv[i]*dt;
		}
}

int save_vec(double *vector, double *matrix, int index)
{
	int i;
	#pragma omp parallel for private(i), shared(vector,matrix,index)
		for(i=0;i<N;i++)
		{
			matrix[index*N+i]=vector[i];
		}
	return ++index;
}

void save_energies(double *Energies,double *pos, double *vel,int index)
{
	int i,j;
	double sumx,sumv;
	for(j=0;j<modes;j++)
	{
		sumx=0.0;
		sumv=0.0;
		#pragma omp parallel for reduction(+:sumx,sumv), private(i), shared(Energies,pos,vel,index,j)
			for(i=0;i<N;i++)
			{
				sumx = sumx + pos[i]*sin(M_PI*(j+1)*(i)/(N));
				sumv = sumv + vel[i]*sin(M_PI*(j+1)*(i)/(N));	
			}
		Energies[index*modes+j]= (double)(w[j]*pow(sumx,2)+pow(sumv,2))/N;
	}
}
