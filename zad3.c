% % sh cat > pi - mpi.c << EOF
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <sys/time.h>
#define N 40
                     MPI_Status status;
double A[N][N], B[N][N], C[N][N];
int main(int argc, char **argv)
{
    int processCount, processId, slaveTaskCount, source, dest, rows, offset;
    struct timeval start, stop;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    MPI_Comm_size(MPI_COMM_WORLD, &processCount);

    slaveTaskCount = processCount - 1;

    if (processId == 0)
    {
        double start = MPI_Wtime();

        srand(time(NULL));
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                A[i][j] = rand() % 10;
                B[i][j] = rand() % 10;
            }
        }

        printf("\n Mnozenie macierzy za pomoca MPI \n");
        printf("\nMacierz A\n\n");
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("%.0f\t", A[i][j]);
            }
            printf("\n");
        }
        printf("\nMacierz B\n\n");
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("%.0f\t", B[i][j]);
            }
            printf("\n");
        }

        rows = N / slaveTaskCount;

        offset = 0;

        for (dest = 1; dest <= slaveTaskCount; dest++)
        {

            MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);

            MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);

            MPI_Send(&A[offset][0], rows * N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);

            MPI_Send(&B, N * N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);

            offset = offset + rows;
        }

        for (int i = 1; i <= slaveTaskCount; i++)
        {
            source = i;

            MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&C[offset][0], rows * N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
        }

        printf("\nWynikowa macierz C = A * B:\n\n");
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                printf("%.0f\t", C[i][j]);
            printf("\n");
        }
        printf("\n");
        double end = MPI_Wtime();
        printf("Czas obliczen %f", end - start);

        if (processId > 0)
        {

            source = 0;

            MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);

            MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);

            for (int k = 0; k < N; k++)
            {
                for (int i = 0; i < rows; i++)
                {

                    C[i][k] = 0.0;
                    B[j][k] for (int j = 0; j < N; j++)
                        C[i][k] = C[i][k] + A[i][j] * B[j][k];
                }
            }

            MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);

            MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);

            MPI_Send(&C, rows * N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
        }
        MPI_Finalize();
    }
    EOF
            mpicc pi -
        mpi.c &&mpirun - n 4 --allow - run - as - root a.out