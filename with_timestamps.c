#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

volatile int breakflag = 0;
int repetitions_g;
volatile unsigned long *samples_g;
volatile int keep_going = 1;

void handler();
void writeToTxt(volatile unsigned long *);
unsigned long get_time();

int main(int argc, char * argv[]){
	
	if (argc != 3){
		printf("\nUsage ./a.out Duration(sec) Interval(sec)\n\n");
		exit(1);
	}
	const float duration = atof(argv[1]);
	float interval = atof(argv[2]);
	const int repetitions = (int)duration / interval;
	
	memcpy(&repetitions_g, &repetitions, sizeof(int));
	samples_g = (volatile unsigned long *) malloc(repetitions*sizeof(unsigned long));	
	interval =(int)(interval*1000000);
	
	signal(SIGALRM, handler);
	ualarm(interval,interval);
	
	while(keep_going)
		pause();	

	writeToTxt(samples_g);
	exit(0);
}

void handler(){
	if (breakflag >= repetitions_g){
		keep_going = 0;
		return;
	};
	samples_g[breakflag]=get_time();
	breakflag++;
}

unsigned long get_time() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000000 + tv.tv_usec;
}

void writeToTxt(volatile unsigned long *buffer){
	FILE *fp = fopen("resultWithTimer.txt", "w");
	if (fp == NULL){
		perror("COULD not open file");
		exit(1);
	}
	else{
		for(int i=0; i<repetitions_g ; ++i){
			fprintf(fp , "%ld\n",buffer[i]);
		}
	}
	fclose(fp);
}
