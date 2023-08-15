#include <iostream>
#include "../../thread_pool.hpp"

/* 
 * THIS TEST NEEDS TO BE TIMED TO BE MEANINGFULL
 * 
 * main:    sleep 3 secs   sleep 2 secs
 *                      
 * thpool:                 sleep 4 secs
 * 
 * Thus the program should take just a bit more than 7 seconds.
 * 
 * */

void sleep_4_secs(){
	sleep(4);
	puts("SLEPT");
}

int main(int argc, char *argv[]){

	char* p;
	if (argc != 2){
		puts("This testfile needs exactly one arguments");
		exit(1);
	}
	int num_threads = strtol(argv[1], &p, 10);

	thread_pool thpool(num_threads);
	
	thpool.pause_threads();
	
	// Since pool is paused, threads should not start before resume. However, it still can add jobs
    thpool.add_task(sleep_4_secs);
    thpool.add_task(sleep_4_secs);
	
	sleep(3);
	
	// Now we will start threads in no-parallel with main
	thpool.resume_threads();

	sleep(2); // Give some time to threads to get the work
	
	// Wait for work to finish

	return 0;
}
