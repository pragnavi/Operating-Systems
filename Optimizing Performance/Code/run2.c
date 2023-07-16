#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>


#define TIME_LOWER 10000000

int main(int argc, char *argv[])
{
    int fd;
    int block_size = 0;
    int block_count = 0;
    struct timeval start;
    struct timeval end;
    double duration = 0;
    double length = 0;
    

    if (argc != 3) {
        fprintf(stderr, "Too many or Too few arguments\n");
        fprintf(stderr, "Usage : <application> <block_size>\n");
        exit(-1);
    }

    block_size = atoi(argv[2]);
    char buf[block_size];

    fd = open(argv[1], O_RDONLY);
    if(fd < 0) {
        fprintf(stderr, "File open error");
        exit(-1);
    }

    do {
        gettimeofday(&start, NULL);
        length += read(fd, buf, block_size);
        gettimeofday(&end, NULL);
        duration += ((end.tv_sec * 1000000) + end.tv_usec) - ((start.tv_sec * 1000000) + start.tv_usec);

        if(duration > TIME_LOWER) {
            break;
        }
        block_count++;
    }while(duration < TIME_LOWER);
    fprintf(stderr, "Time taken : %0.3f seconds\nBlock size : %d Bytes\nBlock count : %d\nFile size %0.3f MB\n", duration / 1000000, block_size, block_count, length / (1024 * 1024));
    fprintf(stdout, "Performance : %0.3f Mb/s\n", length / duration);
    close(fd);

    return 0;
}