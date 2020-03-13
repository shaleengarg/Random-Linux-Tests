#include <stdio.h>
#include <math.h>
#include <sys/sysinfo.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include <omp.h>


#define MAXDIVISIONS 10 //Number of times we want to 
#define PAGE_SIZE 4096 //size of a memory page


typedef struct mem{
	int A;
	//struct mem *next;
	//char padding[PAGE_SIZE - 1*sizeof(struct mem*) - sizeof(int)];
	char padding[PAGE_SIZE - sizeof(int)];
}Mem;

//Returns memory in bytes
unsigned long long mem_avail()
{
	FILE *meminfo = fopen("/proc/meminfo", "r");
	unsigned long long totalMemory = 0;
	if(meminfo == NULL)
	{
		printf("Meminfo is null");
		exit(-1);
	}
	char buff[256];
	unsigned long long ramKB;
	while(fgets(buff, sizeof(buff), meminfo))
	{
		if(sscanf(buff, "MemAvailable: %llu kB", &ramKB) == 1)
		{
			//totalMemory = ramKB/1024.0;
			totalMemory = ramKB * 1024;
		}
	}
	if(fclose(meminfo) != 0)
	{
		printf("fclose Meminfo is null");
		exit(-1);
	}
	//printf("%d\n", totalMemory);
	return totalMemory;
}

int main()
{
	unsigned long long AvailableMem = mem_avail();
	printf("freeram = %llu bytes\n", AvailableMem);

	unsigned long long G = 1024*1024;
	G *= 100;
	printf("reduced by = %llu bytes\n", G);
	AvailableMem = AvailableMem - G;
	printf("freeram reduced = %llu bytes\n", AvailableMem);

	int nprocs;
	nprocs=get_nprocs();
	printf("Number of available threads %d\n", nprocs);

	unsigned long MemPerThread = floor(AvailableMem/nprocs);
	printf("mem per thread = %lu\n", MemPerThread);

	//omp_set_dynamic(0);
	omp_set_num_threads(nprocs);
	Mem *traverse;
	//traverse = (Mem *)malloc(AvailableMem);
	traverse = (Mem*) mmap(NULL, AvailableMem, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_POPULATE, NULL, 0);
	if (traverse == MAP_FAILED)
	{
		printf("malloc failure\n");
	}

	int n;
	int i;
#pragma omp parallel for schedule(dynamic)
	for (n=0; n<nprocs; n++)
	{
		int this_thread = omp_get_thread_num();
		int num_threads = omp_get_num_threads();
		//printf ("Number of elements = %d\n", MemPerThread/PAGE_SIZE);
		for(i=0; i<AvailableMem/PAGE_SIZE; i++)
		{
			traverse[i].A = i*1;
		}
	}
	printf("traverse[%d] = %d\n", 2, traverse[2].A);
	//free(traverse);
	//Newmmap
	////////////////////////////////////
	//Allocate Mem
	Mem *Newtraverse;
	int Memo = 1024 * 1024 * 20;
	for(n=0; n<100; n++)
	{
		Newtraverse = (Mem*) mmap(NULL, Memo, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_POPULATE, NULL, 0);
		for(i=0; i<Memo/PAGE_SIZE; i++)
		{
			Newtraverse[i].A = i+1;
		}
		printf("%d Newtraverse[%d] = %d\n", n, 2, traverse[2].A);
		munmap(Newtraverse, Memo);
	}
	////////////////////////////////////
	munmap(traverse, AvailableMem);

	return 0;
}
