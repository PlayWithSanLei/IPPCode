#include <stdio.h>
#include <mpi.h> 
#include <string.h>

int main(void) {
   int my_rank, comm_sz;
   char message[100];
   MPI_Init(NULL, NULL); 
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
   
   if(my_rank != 0) { 
  	 sprintf(message, "Proc %d of %d > Does anyone have a toothpick?\n", my_rank, comm_sz);
	 MPI_Send(message, strlen(message)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
   } else {
	   printf("Proc %d of %d > Does anyone have a toothpick?\n\n", my_rank, comm_sz);
	   for(int s = 1; s < comm_sz; s++) {
		   MPI_Recv(message, 100, MPI_CHAR, s, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		   printf("%s\n", message);
	   }
   }
   

   MPI_Finalize();
   return 0;
}  /* main */
