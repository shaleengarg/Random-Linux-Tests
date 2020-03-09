#include <stdio.h>
#include <math.h>
#include <sys/sysinfo.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <stdlib.h>

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
unsigned long mem_avail()
{
	FILE *meminfo = fopen("/proc/meminfo", "r");
	unsigned long totalMemory = 0;
	if(meminfo == NULL)
	{
		printf("Meminfo is null");
		exit(-1);
	}
	char buff[256];
	while(fgets(buff, sizeof(buff), meminfo))
	{
		unsigned long ramKB;
		if(sscanf(buff, "MemFree: %lu kB", &ramKB) == 1)
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
	unsigned long AvailableMem = mem_avail();
	printf("freeram = %lu bytes\n", AvailableMem);

	int nprocs;
	nprocs=get_nprocs();
	printf("Number of available threads %d\n", nprocs);

	unsigned long MemPerThread = floor(AvailableMem/nprocs);
	printf("mem per thread = %lu\n", MemPerThread);

	//omp_set_dynamic(0);
	omp_set_num_threads(nprocs);

	int n;
#pragma omp parallel for schedule(dynamic)
	for (n=0; n<nprocs; n++)
	{
		int this_thread = omp_get_thread_num();
		int num_threads = omp_get_num_threads();
		printf("Thread id = %d of %d\n", this_thread, num_threads);
		int i = 0;
		Mem *traverse;
		traverse = (Mem *)malloc(MemPerThread);

		//printf ("Number of elements = %d\n", MemPerThread/PAGE_SIZE);
		for(i=0; i<MemPerThread/PAGE_SIZE; i++)
		{
			traverse[i].A = i+1;
		}
		printf("traverse[%d] = %d\n", 2, traverse[2].A);
		free(traverse);
	}

	return 0;
}
