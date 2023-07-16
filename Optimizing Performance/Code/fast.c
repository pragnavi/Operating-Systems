#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>


#define BLOCK_SIZE 400000

unsigned int xorbuffer(unsigned int buffer[], int size)
{
    unsigned int outcome = 0;
    for (int i = 0; i < size; i++) {
        outcome ^= buffer[i];
    }
    return outcome;
}

int main(int argc, char *argv[])
{
    int fd;
    int block_count = 0;
    struct timeval start;
    struct timeval end;
	double duration = 0;
    double length = 0;
    unsigned int result = 0;
    unsigned int  temp;

    if (argc != 2) {
        fprintf(stderr, "Input arguments error ! Too few or too many arguments !!\n");
        fprintf(stderr, "Usage : ./run <filename>\n");
        exit(-1);
    }

    unsigned int buf[BLOCK_SIZE/sizeof(unsigned int)];
    fd = open(argv[1], O_RDONLY);

    if(fd < 0) {
        fprintf(stderr, "File open error %s\n", argv[1]);
        exit(-1);
    }

    gettimeofday(&start, NULL);

    while((temp = read(fd, buf, BLOCK_SIZE)) != 0) {
        length += temp;
        result ^= xorbuffer(buf, temp/sizeof(unsigned int));
        block_count++;
    }

    printf("xor value of the file is %08x\n", result);
    gettimeofday(&end, NULL);
    duration = ((end.tv_sec * 1000000) + end.tv_usec) - ((start.tv_sec * 1000000) + start.tv_usec);
    duration = duration / 1000000;
    length = length / (1024 * 1024);
    fprintf(stdout, "Time taken to read %d blocks of each size %d is %0.3f seconds\n", block_count, BLOCK_SIZE, duration);
    fprintf(stdout, "Performance : %0.3f Mb/s\n", length / duration);
    close(fd);
    return 0;
}
