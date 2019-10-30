#include <assert.h>
#include <fcntl.h>
#include <iostream>
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

#define MAX 100000
//#define file "small.txt"
#define file "randomfile.txt"

using namespace std;

double getntime(struct timespec *begin, struct timespec *end)
{
    return (((end->tv_sec - begin->tv_sec)*1000000000) + \
            (end->tv_nsec - begin->tv_nsec)); //Time in nanoseconds
}

size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

int main(int argc, char const *argv[])
{
    int cpid[MAX];
    double *time;// = malloc(sizeof(double)*MAX);
    if(argc != 2)
    {
        cout << "try ./a.out NOCHild #change the name of the file in the source" << endl;
    }

    size_t filesize = getFilesize(file);

    int NChildren = 1;
    NChildren = atoi(argv[1]);
    struct timespec start, end;

    int shmid;
    key_t Key = 1123; //This could be anything. used to indentify the shared memory region

    shmid = shmget(Key, sizeof(double)*MAX, IPC_CREAT | 0666);
    if(shmid < 0)
    {
        perror("shmid not wokring");
        exit(-1);
    }
    time = (double*) shmat(shmid, NULL, 0);
    if(time == (double*)-1)
    {
        perror("shmat time ");
        exit(-1);
    }

    int i;
    for(i=0; i<NChildren; i++)
    {
        cpid[i] = fork(); 
        if(cpid[i] < 0)
        {
            perror("Unable to fork\n");
            exit(-1);
        }
        if(cpid[i] == 0) //Child Process
        {
            shmid = -1;
            shmid = shmget(Key, sizeof(double)*MAX, 0666);
            time = (double*)shmat(shmid, NULL, 0);

            int fd = open(file, O_RDWR, 0);
            assert(fd > 0);

            clock_gettime(CLOCK_REALTIME, &start);
            char *mem = (char*) mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_SHARED | MAP_POPULATE, fd, 0);
            //char *mem = (char*) mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
            if(mem == MAP_FAILED)
                cout << "mmap failed with errno " << strerror(errno);
            //and read completely
            char a;
            for(long j=0; j<filesize; j++)
            {
                a = mem[j];
               // cout << "ID " << getpid() << " " << a << endl;
            }
            clock_gettime(CLOCK_REALTIME, &end);

            time[i] =  getntime(&start, &end);
            cout << "Time for PID " << getpid() <<  " "  << getntime(&start, &end) << endl;
            return 0;
        }
    }
    int status, n = NChildren;
    pid_t pid;
    while(n--)
    {
        pid = wait(&status);
    }

    double tottime;
    for(int i=0; i<NChildren; i++)
    {
        tottime += time[i];
    }
    double avgtime = tottime/NChildren;
    printf("AvgTime to read the complete file is %lf milliseconds\n", avgtime/1000000);
    return 0;
}
