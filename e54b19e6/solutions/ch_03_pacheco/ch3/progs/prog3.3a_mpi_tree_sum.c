/*
 * File:     prog3.3a_mpi_tree_sum.c
 *
 * Purpose:  Use tree-structured communication to find the global sum
 *           of a random collection of ints
 *
 * Compile:  mpicc -g -Wall -o prog3.3a_mpi_tree_sum prog3.3a_mpi_tree_sum.c
 * Run:      mpiexec -n <comm_sz> ./prog3.3a_mpi_tree_sum
 *
 * Input:    None
 * Output:   Random values generated by processes, and their global sum.
 *
 * Note:     This version assumes comm_sz is a power of 2.
 *
 * Author:   Jinyoung Choi
 *
 * IPP:      Programming Assignment Chapter 3.3, first part
 */


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int Global_sum(int my_int, int my_rank, int comm_sz, MPI_Comm comm); 

const int MAX_CONTRIB = 20;

int main(void) {
   int i, sum, my_int;
   int my_rank, comm_sz;
   MPI_Comm comm;
   int* all_ints = NULL;
   
   MPI_Init(NULL, NULL);
   comm = MPI_COMM_WORLD;
   MPI_Comm_size(comm, &comm_sz);
   MPI_Comm_rank(comm, &my_rank);

   srandom(my_rank + 1);
   my_int = random() % MAX_CONTRIB;
   
   sum = Global_sum(my_int, my_rank, comm_sz, comm);
   
   if ( my_rank == 0) {
      all_ints = malloc(comm_sz*sizeof(int));
      MPI_Gather(&my_int, 1, MPI_INT, all_ints, 1, MPI_INT, 0, comm);
      printf("Ints being summed:\n   ");
      for (i = 0; i < comm_sz; i++)
         printf("%d ", all_ints[i]);
      printf("\n");
      printf("Sum = %d\n",sum);
      free(all_ints);
   } else {
      MPI_Gather(&my_int, 1, MPI_INT, all_ints, 1, MPI_INT, 0, comm);
   }
   
   MPI_Finalize();
   return 0;
}  /* main */

/*-------------------------------------------------------------------
 * Function:   Global_sum
 * Purpose:    Implement a global sum using tree-structured communication
 * Notes:       
 * 1.  comm_sz must be a power of 2
 * 2.  The return value is only valid on process 0
 */
int Global_sum(
      int my_int    /* in */,
      int my_rank   /* in */,
      int comm_sz   /* in */,
      MPI_Comm comm /* in */) {
   
   int partner, recvtemp;
   int my_sum = my_int;
   unsigned bitmask = 1;
   
   while (bitmask < comm_sz) {
      partner = my_rank ^ bitmask;
      
      if (my_rank < partner) {
         MPI_Recv(&recvtemp, 1, MPI_INT, partner, 0, comm, 
               MPI_STATUS_IGNORE);
         my_sum += recvtemp;
         bitmask <<= 1;
      } else {
         MPI_Send(&my_sum, 1, MPI_INT, partner, 0, comm);
         break;
      }
   }  /* while */

   return my_sum;
}  /* Global_sum */
