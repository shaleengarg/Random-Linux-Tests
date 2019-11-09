#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fp = fopen(__FILE__, "r");
    char * line = NULL;
    size_t len = 0;
    ssize_t read; 

    if (fp == NULL)
        return -1;

    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
        printf("%s", line);
    }
    fclose(fp);
    return 0;
}
