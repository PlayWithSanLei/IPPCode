/*
 * 代码编写于2021年3月21日
 * 作者：张昊
 * 本程序在my_mpi_trap1.c的基础上加入了输入操作
 */

#include <mpich/mpi.h>
#include <stdio.h>

void Input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p);

double Trap(double a, double b, int n, double h);

double f(double x);

int main() {
	int my_rank, comm_sz, n, local_n;
	double a, b, h, local_a, local_b;
	double local_result, total_result;
	int source;

	MPI_Init(NULL, NULL);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

	Input(my_rank, comm_sz, &a, &b, &n);
	h = (b-a) / n;

	local_n = n/comm_sz;
	local_a = a + my_rank * local_n * h;
	local_b = local_a + local_n * h;
	local_result = Trap(local_a, local_b, local_n, h);

	if (my_rank != 0) {
		MPI_Send(&local_result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	} else {
		total_result = local_result;
		for(source = 1;source < comm_sz; source ++) {
			MPI_Recv(&local_result, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			total_result += local_result;
		}
	}

	if (my_rank == 0) {
                printf("当n=%d时，区间%f到%f的估计值为%.15e\n",n, a, b, total_result);
        }
       
       	MPI_Finalize();
        return 0;
}

void Input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p) {
	int dest;

	if (my_rank == 0) {
		printf("请输入a, b, n:");
		scanf("%lf %lf %d", a_p, b_p, n_p);
		for(dest = 1; dest < comm_sz; dest++) {
			MPI_Send(a_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
			MPI_Send(b_p, 1, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
			MPI_Send(n_p, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
		}
	} else {
		MPI_Recv(a_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(b_p, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(n_p, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}

double Trap(double a, double b, int n, double h) {
	double estimate, x;
	estimate = (f(a) + f(b)) / 2.0;
	for(int i = 1; i <= n-1; i++) {
		x = a + i * h;
	       estimate += f(x);	
	}
	estimate = estimate * h;
	return estimate;
}

double f(double x) {
	return x*x;
}
