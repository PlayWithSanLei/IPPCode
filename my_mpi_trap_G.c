#include <stdio.h>
#include <mpi.h>

double f(double x){
  return x*x;
}

double Trap(
  double left_endpt,   /*  in  */
  double right_endpt,  /*  in  */
  int trap_count,      /*  in  */
  double base_len      /*  in  */
  ){
  double estimate, x;
  int i;

  estimate = (f(left_endpt) + f(right_endpt))/2.0;
  for (i = 0; i < trap_count - 1; i++){
    x = left_endpt + (i + 1) * base_len;
    estimate += f(x);
  }
  return estimate * base_len;
}

int main(){
  int my_rank, comm_sz, n = 64, local_n;
  double a = 0.0, b = 3.0, h, local_a, local_b, remain = 0.0;
  double local_int, total_int;
  int source;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

  //Get_data(my_rank, comm_sz, &a, &b, &n);

  h = (b - a)/n; //h is the same for all processes

  local_n = n/comm_sz; // So is the number of trapezoids

  local_a = a + my_rank * local_n * h;
  local_b = local_a + local_n * h;
  local_int = Trap(local_a, local_b, local_n, h);



  if (my_rank != 0){
    MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  } else {

    total_int =local_int;
    for (source = 1; source < (comm_sz); source++){
      MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      total_int += local_int;
    }

    // 这里是因为有一些矩阵没有计算到，所以，非整除的情况下结果会有比较大的缺失。
    // 这里将这些矩形在0号线程上补上。
    if (n%comm_sz){
      remain = Trap(h * local_n * comm_sz, b, n%comm_sz, h);
    }
    
    total_int += remain;
  }

  if (my_rank == 0){
    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.15e\n", a, b, total_int);
  }

  MPI_Finalize();
  return 0;
}

