/*
 * 代码编写于2021年3月21日
 * 作者：张昊
 * 本程序是利用并行方式使用梯形法则计算二次函数的积分
 * 其中a,b,n均为写死的，不可以从外部输入
 */
#include <mpi.h>
#include <stdio.h>

double Trap(double a, double b, int n, double h);
double f(double x);

int main() {
	int my_rank, comm_sz, n = 1024, local_n;
	double a = 0.0, b = 3.0, h, local_a, local_b;
	double local_result, total_result;
	int source;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	
	h = (b-a) / n;
	local_n = n / comm_sz;

	local_a = a + my_rank * local_n * h;
	local_b = local_a + local_n*h;
	local_result = Trap(local_a, local_b, local_n, h);

	if (my_rank != 0) {
		MPI_Send(&local_result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	} else {
		total_result = local_result;
		for(source = 1; source < comm_sz; source++) {
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

double Trap(double a, double b, int n, double h) {
	double result;

	result = (f(a) + f(b)) / 2.0;
	for(int k = 1; k < n; k++) {
		result += f(a+k*h);
	}
	result = result * h;
	return result;
}

double f(double x) {
	return x*x;
}

