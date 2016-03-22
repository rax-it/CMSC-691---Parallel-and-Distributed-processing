/*
 *  *   Circuit Satisfiability, Version 1
 *   *
 *    *   This MPI program determines whether a circuit is
 *     *   satisfiable, that is, whether there is a combination of
 *      *   inputs that causes the output of the circuit to be 1.
 *       *   The particular circuit being tested is "wired" into the
 *        *   logic of function 'check_circuit'. All combinations of
 *         *   inputs that satisfy the circuit are printed.
 *          *
 *           */

#include "mpi.h"
#include <stdio.h>

int main (int argc, char *argv[]) {
   int i;
   int pid;               /* Process rank */
   int p;                /* Number of processes */
   int temp, x=2, x16;

   MPI_Init (&argc, &argv);
   MPI_Comm_rank (MPI_COMM_WORLD, &pid);
   MPI_Comm_size (MPI_COMM_WORLD, &p);
   MPI_Status status;

   if (pid == 0){
        MPI_Send(&x,
		1,
		MPI_INT,
		1,
		0,
		MPI_COMM_WORLD);
	MPI_Recv(&x16,
		1,
		MPI_INT,
		p-1,
		0,
		MPI_COMM_WORLD,
		&status);
   }

   else if (pid > 0){
	MPI_Recv(&x,
		1,
		MPI_INT,
		pid-1,
		0,
		MPI_COMM_WORLD,
		&status);
	temp = x*x;
	printf("My received %d , process %d\n", x, pid);
	int x = pid+1;
	if(pid == p-1) x=0; 
	MPI_Send(&temp,
                1,
                MPI_INT,
                x,
                0,
                MPI_COMM_WORLD);
   }


  // printf("Count %d pid %d\n", count, id);
   //printf ("Process %d is done\n", id);
  
   if (pid == 0) {
   	printf("X^16 = %d\n", x16);
   }

   MPI_Finalize();
   return 0;
}


