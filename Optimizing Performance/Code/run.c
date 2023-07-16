#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>


unsigned int xorbuffer(unsigned int *buffer, int size)
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
    int block_size = 0;
    int block_count = 0;
	int max = 125;
    int min = 33;
    struct timeval start;
    struct timeval end;
	double duration = 0;
    double length = 0;
    unsigned int result = 0;
    unsigned int  temp;

    if (argc != 5) {
		fprintf(stderr, "Input arguments error ! Too few or too many arguments !!\n");
		fprintf(stderr, "Usage : ./run <filename> [-r|-w] <block_size> <block_count>\n");
		exit(-1);
    }

    block_size = atoi(argv[3]);
    block_count = atoi(argv[4]);
    unsigned int buf[block_size/sizeof(unsigned int)];
    char buf1[block_size];

    if (strcmp("-w", argv[2]) == 0) {
		fprintf(stdout, "Write\n");
		for(int i = 0; i < block_size; i++) {
	    	srand(time(0) + i);
	    	buf1[i] = (rand() % (max - min + 1)) + min;
		}

		fd = open(argv[1], O_WRONLY | O_CREAT,0777);
		if(fd < 0) {
			fprintf(stderr, "File open error %s\n", argv[1]);
			exit(-1);
		}

		gettimeofday(&start, NULL);
		
		for(int i = 0; i < block_count; i++) {
			length += write(fd, buf, block_size);
		}
		gettimeofday(&end, NULL);
		duration = ((end.tv_sec * 1000000) + end.tv_usec) - ((start.tv_sec * 1000000) + start.tv_usec);
		duration = duration / 1000000;
		length = length / (1024 * 1024);
		fprintf(stdout, "Time taken to write %d blocks of each size %d is %0.3f seconds\n", block_count, block_size, duration);
		fprintf(stdout, "Performance : %0.3f Mb/s\n", length / duration);
    }

    else if (strcmp("-r", argv[2]) == 0) {
		fprintf(stdout, "Read\n");
		fd = open(argv[1], O_RDONLY);
		if(fd < 0) {
			fprintf(stderr, "File open error %s\n", argv[1]);
			exit(-1);
		}
		gettimeofday(&start, NULL);
		for(int i = 0; i < block_count; i++) {
			temp = read(fd, buf, block_size);
			length += temp;
			result ^= xorbuffer(buf, temp/sizeof(unsigned int));	
		}
		printf("xor value of the file is %08x\n", result);
		gettimeofday(&end, NULL);
		duration = ((end.tv_sec * 1000000) + end.tv_usec) - ((start.tv_sec * 1000000) + start.tv_usec);
		duration = duration / 1000000;
		length = length / (1024 * 1024);
		fprintf(stdout, "Time taken to read %d blocks of each size %d is %0.3f seconds\n", block_count, block_size, duration);
		fprintf(stdout, "Performance : %0.3f Mb/s\n", length / duration);
    }
	
    else {
		fprintf(stderr, "Error : Invalid Argument!!!\n");
		exit(-1);
    }

    close(fd);
    return 0;
}










