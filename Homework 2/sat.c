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
   int id;               /* Process rank */
   int p;                /* Number of processes */
   int check_circuit (int, int);
   int c, count, total;

   MPI_Init (&argc, &argv);
   MPI_Comm_rank (MPI_COMM_WORLD, &id);
   MPI_Comm_size (MPI_COMM_WORLD, &p);

   for (i = id; i < 65536; i += p){
      c = check_circuit (id, i);
	if (c == 1) {count++;}
   }

   printf("Count %d pid %d\n", count, id);
   printf ("Process %d is done\n", id);
  
   MPI_Reduce(&count,
                &total,
                1,
                MPI_INT,
                MPI_SUM,
                0,
                MPI_COMM_WORLD);
   if (id == 0) {
   	printf("Total number of solutions: %d\n", total);
   }

   fflush (stdout);
   MPI_Finalize();
   return 0;
}

/* Return 1 if 'i'th bit of 'n' is 1; 0 otherwise */
#define EXTRACT_BIT(n,i) ((n&(1<<i))?1:0)

int check_circuit (int id, int z) {
   int v[16];        /* Each element is a bit of z */
   int i, c=0;

   for (i = 0; i < 16; i++) v[i] = EXTRACT_BIT(z,i);
   if ((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3])
      && (!v[3] || !v[4]) && (v[4] || !v[5])
      && (v[5] || !v[6]) && (v[5] || v[6])
      && (v[6] || !v[15]) && (v[7] || !v[8])
      && (!v[7] || !v[13]) && (v[8] || v[9])
      && (v[8] || !v[9]) && (!v[9] || !v[10])
      && (v[9] || v[11]) && (v[10] || v[11])
      && (v[12] || v[13]) && (v[13] || !v[14])
      && (v[14] || v[15])) {
      printf ("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n", id,
         v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9],
         v[10],v[11],v[12],v[13],v[14],v[15]);
      fflush (stdout);
      c++;
   }
	return c;
}

