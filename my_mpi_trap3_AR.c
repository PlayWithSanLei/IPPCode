#include <stdio.h>
#include <mpi.h>
void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p,
        int* n_p);
double Trap(double left_endpt, double right_endpt, int trap_count, 
        double base_len);    
double f(double x); 
int main(void) {
    int my_rank, comm_sz, n, local_n;   
    double a, b, h, local_a, local_b;
    double local_int, total_int;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    Get_input(my_rank, comm_sz, &a, &b, &n);

    h = (b-a)/n;          
    local_n = n/comm_sz;  
    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    local_int = Trap(local_a, local_b, local_n, h);

    MPI_Allreduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("当n= %d 时, 区间%f to %f 的估计值是%.15e\n", n, a, b, total_int);
    }

    MPI_Finalize();

    return 0;
} /*  main  */

void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p,
        int* n_p) {

    if (my_rank == 0) {
        printf("请输入a， b，n\n");
        scanf("%lf %lf %d", a_p, b_p, n_p);
    } 
    MPI_Bcast(a_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(b_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(n_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
}  /* Get_input */

double Trap(
        double left_endpt  /* in */, 
        double right_endpt /* in */, 
        int    trap_count  /* in */, 
        double base_len    /* in */) {
    double estimate, x; 
    int i;

    estimate = (f(left_endpt) + f(right_endpt))/2.0;
    for (i = 1; i <= trap_count-1; i++) {
        x = left_endpt + i*base_len;
        estimate += f(x);
    }
    estimate = estimate*base_len;

    return estimate;
} /*  Trap  */

double f(double x) {
    return x*x;
} 

