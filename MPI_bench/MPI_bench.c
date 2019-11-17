#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <mpi.h>
#include <math.h>
#include <unistd.h>


int Myrank = 0;

int usage()
{
    if (Myrank == 0) 
    {
        printf("\n");
        printf("  Usage:  mpiBench [options] [operations]\n");
        printf("\n");
        printf("  Options:\n");
        printf("    -b <byte>  Beginning message size in bytes (default 0)\n");
        printf("    -e <byte>  Ending message size in bytes (default 1K)\n");
        printf("    -m <byte>  Process memory buffer limit (send+recv) in bytes (default 1G)\n");
        printf("    -i <itrs>  Maximum number of iterations for a single test (default 1000)\n");
        printf("    -t <usec>  Time limit for any single test in microseconds (default 0 = infinity)\n");
        printf("    -d <ndim>  Number of Cartesian dimensions to split processes in (default 0 = MPI_COMM_WORLD only)\n");
        printf("    -p <size>  Minimum partition size (number of ranks) to divide MPI_COMM_WORLD by\n");
        printf("    -c         Check receive buffer for expected data in last interation (default disabled)\n");
        printf("    -C         Check receive buffer for expected data every iteration (default disabled)\n");
        printf("    -h         Print this help screen and exit\n");
        printf("    where <byte> = [0-9]+[KMG], e.g., 32K or 64M\n");
        printf("\n");
        printf("  Operations:\n");
        printf("    Barrier\n");
        printf("    Bcast\n");
        printf("    Alltoall, Alltoallv\n");
        printf("    Allgather, Allgatherv\n");
        printf("    Gather, Gatherv\n");
        printf("    Scatter\n");
        printf("    Allreduce\n");
        printf("    Reduce\n");
        printf("\n");
    }
    exit(1);
}


int main(int argc, char *argv[])
{
    
    return 0;
}
