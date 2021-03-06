#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void Usage(char prog_name[]);
void Get_args(char* argv[], int* thread_count, int* n);
void Schedule(int thread_count, int n);

int main(int argc, char* argv[]) {
   int thread_count, n;
   
   if (argc != 3) Usage(argv[0]);
   Get_args(argv, &thread_count, &n);
   
   Schedule(thread_count, n);

   return 0;
}  /* main */

void Usage(char prog_name[]) {
   fprintf(stderr, "usage: %s <thread_count> <n>\n", prog_name);
   exit(0);
}  /* Usage */

void Get_args(
           char* argv[]          /* in  */,
           int*  thread_count    /* out */,
           int*  n               /* out */) {
   *thread_count = strtol(argv[1], NULL, 10);  
   *n = strtol(argv[2], NULL, 10);
}  /* Get_args */

void Schedule(int thread_count, int n) {
   int  i;
   int* min = malloc(thread_count*sizeof(int));
   int* max = malloc(thread_count*sizeof(int));
   
   for (i = 0; i < thread_count; i++) {
      min[i] = n;
      max[i] = 0;
   }
   
#  pragma omp parallel num_threads(thread_count) \
      default(none) private(i) shared(min, max, n)
   {
   int my_rank = omp_get_thread_num();
#     pragma omp for
      for (i = 0; i < n; i++) {
#        ifdef DEBUG      
         printf("Th %d > iteration %d\n", my_rank, i);
#        endif
         if (i < min[my_rank])
            min[my_rank] = i;
         if (i > max[my_rank])
            max[my_rank] = i;
      }
   }  /* End of omp parallel */
   
   for (i = 0; i < thread_count; i++) {
      if (min[i] == n && max[i] == 0)
         printf("Th %d > No iterations\n", i);
      else if (min[i] != max[i])
         printf("Th %d > Iterations %d - %d\n", i, min[i], max[i]);
      else
         printf("Th %d > Iteration  %d\n", i, min[i]);
   }
   
   free(min);
   free(max);
}  /* Schedule */
