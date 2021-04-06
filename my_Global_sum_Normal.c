#include <stdio.h>
#include <mpi.h>

void Tree_SUM(int comm_sz, int my_rank) {
	int TongXin = comm_sz, sum = my_rank, half, m, temp;
	while(TongXin != 1) {
		half = TongXin / 2;
		m = TongXin % 2;
		if(my_rank < half) {
			MPI_Recv(&temp, 1, MPI_INT, my_rank+half+m, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum += temp;
		} else if(my_rank >= half+m && my_rank < TongXin){
			MPI_Send(&sum, 1, MPI_INT, my_rank-half-m, 0, MPI_COMM_WORLD);
			return;
		}
		TongXin = half + m;
	}
	if(my_rank == 0) {
		printf("总和为：%d\n", sum);
	}
}

int main() {
	int comm_sz, local_num;
	int my_rank;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	Tree_SUM(comm_sz, my_rank);
	
	MPI_Finalize();	
	return 0;	
}
