#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>

float interval_g;
volatile int breakflag = 0;
volatile int keep_going = 1;
int repetitions_g;
volatile unsigned long *samples_g;


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
	if ( interval >= 1.0 ){
		printf("Due to technical limitations of the alarm and\
			\nualarm functions this program\
			\nuses only less than a one seconds intervals\n");
		exit(3);
	}
	
	const int repetitions = (int)duration / interval;
	memcpy(&repetitions_g, &repetitions, sizeof(float));

	samples_g = (volatile unsigned long *) malloc(repetitions*sizeof(unsigned long));	
	if(samples_g == NULL){
		perror("Malloc Failed.\nExiting Now\n");
		exit(2);
	}

	interval = (int)(interval * 1000000);
	memcpy(&interval_g, &interval, sizeof(float)); 
	signal(SIGALRM, handler);
	
	ualarm(interval, 0);


	while(keep_going){
		pause();
	}
	writeToTxt(samples_g);
	exit(0);
}

void handler(){
	ualarm(interval_g , 0);
	if (breakflag >= repetitions_g){
		keep_going = 0;
		return;
	}
	samples_g[breakflag] = get_time();
	breakflag++;
}

unsigned long get_time() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000000 + tv.tv_usec;
}


void writeToTxt(volatile unsigned long *buffer){
	FILE *fp = fopen("resultNoTimer.txt", "w");
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
