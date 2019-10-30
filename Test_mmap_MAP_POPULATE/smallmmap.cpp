#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#define FIL "small.txt" //Bigger the file, more data

using namespace std;

size_t getFilesize(const char* filename) {
    //cout << "filename " << filename  << endl;
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

int main(int argc, char *argv[])
{
    long filesize = getFilesize(FIL);
    cout << "File size = " << filesize << endl;
    int fd = open(FIL, O_RDWR, 0664);
    assert(fd > 0);

    //char *mem = (char *)mmap(NULL, filesize, PROT_READ, MAP_SHARED, fd, 0);
    char *mem = (char*) mmap(NULL, filesize, PROT_READ, MAP_SHARED, fd, 0);
    if(mem == MAP_FAILED)
        cout << "mmap failed with errno " << strerror(errno);


    for(long i=0; i<filesize; i++)
    {
        printf("%c", mem[i]);
    }
    munmap(mem, filesize);
    close(fd);
    return 0;
}
