%%sh
cat > lpi-mpi.c << EOF
#include <mpi.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
  int rank, size;
  MPI_Init( &argc, &argv );
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
 
  double sum = 0.0;
  long Max_iterations = 1000;
  double result;
 
  for (long long n = rank; n<Max_iterations; n += size)
  {
    double temp = 1.0 / (2.0 * (double)n + 1);
    if (n % 2 != 0){ sum -= temp; } 
    else if(n % 2 == 0){ sum += temp; }
  }
 
  MPI_Reduce(&sum, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
 
  if (rank == 0){ printf("PI = %f", result*4.0); }
 
  MPI_Finalize();
  return 0;
}
EOF
mpicc lpi-mpi.c && mpirun -n 10 --allow-run-as-root a.out