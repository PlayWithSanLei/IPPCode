#include <stdio.h>
#include <mpi.h>
#define size 16

void Tree_SUM(int comm_sz, int my_rank) {
	int TongXin = comm_sz, sum = my_rank, half, temp;
	while(TongXin != 1) {
		half = TongXin / 2;
		if(my_rank < half) {
			MPI_Recv(&temp, 1, MPI_INT, my_rank+half, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum += temp;
		} else {
			MPI_Send(&sum, 1, MPI_INT, my_rank - half, 0, MPI_COMM_WORLD);
			return;
		}
		TongXin = half;
	}
	if(my_rank == 0) {
		printf("总和为：%d\n", sum);
	}
}

int main() {
	int comm_sz, local_num;
	int my_rank;
	int a[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	Tree_SUM(comm_sz, my_rank);
	
	MPI_Finalize();	
	return 0;	
}
