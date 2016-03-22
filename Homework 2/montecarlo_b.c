/* MPI program that uses a monte carlo method to compute the value of INTEGRALVALUE */
/* Tyler A. Simon*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#define USE_MPI   
#define INTEGRALVALUE 1.045301308
#define DEBUG 0

/* Define the max number of accurate values until termination*/
#define MAX 10

double get_eps(void);
double getArea(double x);

int main(int argc, char *argv[])
{
   double x,y;
   int i;
   long int count=0,mycount; /* # of points in the 1st quadrant of unit circle */
   double z;
   double integral=0.0;
   int myid,numprocs,proc;
   MPI_Status status;
   int master =0;
   int tag = 123;
   long int myiter=1;
   int done=0;
   long int iterval=1000000; /* how many points per iteration to increase*/
   long int niter=iterval;
   double upperLimit = 1.0;
   double lowerLimit = 0.0;
   double fmax = getArea(upperLimit);
   
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD,&myid);

  srand48(time(NULL)*myid);
  double epsilon;
 // if(myid==0)epsilon=get_eps();

   
   if (argc <=1) {
     fprintf(stderr,"Usage: monte_pi_mpi epsilon\n");
     MPI_Finalize();
     exit(-1);
   }
	epsilon=atof(argv[1]);

  while(done!=MAX){
   mycount=0;
   myiter=niter/numprocs;

     /* initialize random numbers */
   for ( i=0; i<myiter; i++) {
      x = (double)drand48();
      y = (double)drand48();
	  double xr = x*upperLimit;
	  double fr = y*fmax;
	  double fxr = getArea(xr);  
      //z = x*x+y*y;
      if (fr<=fxr) mycount++; 
   }
   if (myid ==0) { /* if I am the master process gather results from others */
      count = mycount;
      for (proc=1; proc<numprocs; proc++) {
         MPI_Recv(&mycount,1,MPI_REAL,proc,tag,MPI_COMM_WORLD,&status);
         count +=mycount;        
      }
      integral=(double)count * fmax * upperLimit/(myiter*numprocs);
      if(DEBUG)printf("procs= %d, trials= %ld, estimate= %2.10f, INTEGRALVALUE= %2.10f, error= %2.10f \n", numprocs,myiter*numprocs,integral, INTEGRALVALUE,fabs(integral-INTEGRALVALUE));
	if(fabs(integral - INTEGRALVALUE) <= (epsilon * fabs(integral))) 
		{
		printf("\n# (%d) Accuracy Met: iters = %ld, INTEGRALVALUE= %2.10f, integral= %2.10f, error= %2.10f, eps= %2.10f\n", done,numprocs*myiter,INTEGRALVALUE,integral,fabs(integral-INTEGRALVALUE),epsilon); 
		//done=1;
		done++;
		}
		
	/* Tell everyone we are done*/
  	MPI_Bcast(&done,1,MPI_INT,myid,MPI_COMM_WORLD);
   }
   else { /* for all the slave processes send results to the master */
   //   printf("Processor %d sending results= %d to master process\n",myid,mycount);
      MPI_Send(&mycount,1,MPI_REAL,master,tag,MPI_COMM_WORLD);

	/* Have we received a done command */
      MPI_Bcast(&done,1,MPI_INT,0,MPI_COMM_WORLD);
   }
}//done they match!!   
   MPI_Finalize();             /* let MPI finish up */
  
return 0;
}

double getArea(double x) {
	return pow((pow(x, 0.5) +x), 0.5);
}

/*get machine epsilon*/
double get_eps(){
  double xxeps=1.0;
  int i;
  for(i=1; i<55; i++) /* we know about 52 bits */
  {
    xxeps=xxeps/2.0;
    if(1.0-xxeps == 1.0) break;
  }
  xxeps=2.0*xxeps;
  printf("type double eps=%24.17E \n", xxeps);
  return xxeps;
}//end get_eps
