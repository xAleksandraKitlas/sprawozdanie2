% % sh cat > pi - mpi.c << EOF
#include <mpi.h>
#include <stdio.h>

                 double
                 funkcja(double x)
{
    return x;
}
int main(int argc, char *argv[])
{
    int i, np, lp;
    int tag = 50;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &np);
    MPI_Comm_size(MPI_COMM_WORLD, &lp);
    if (np == 0)
    {

        double dx, xp, xk, pole, s = 0;
        printf("\nObliczanie calki oznaczonej metoda prostokątów dla funkcji f(x)=x");

        xp = 1.0;

        xk = 4.0;

        dx = (xk - xp) / (double)lp;

        for (i = 0; i < lp; i++)
        {
            MPI_Send(&xp, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
            MPI_Send(&xk, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
            MPI_Send(&dx, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD);
        }

        for (i = 0; i < lp; i++)
        {
            MPI_Recv(&s, 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
            pole += s;
        }
        printf("\nPole pod calka wynosi = %lf\n", pole);
    }
    else
    {

        double s, dx, xp, xk;
        double np2 = np;

        MPI_Recv(&xp, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&xk, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&dx, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);

        s = funkcja(dx * np + xp) * dx;

        MPI_Send(&s, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}
EOF
        mpicc pi -
    mpi.c &&mpirun - n 8 --allow - run - as - root a.out