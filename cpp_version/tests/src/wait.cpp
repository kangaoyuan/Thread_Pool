#include <iostream>
#include "../../thread_pool.hpp"

int main(int argc, char* argv[]){
    if (argc < 3){
        std::cerr << "This testfile needs at least two arguments" << std::endl;
		exit(1);
	}


	int num_jobs         = atol(argv[1]);
	int num_threads      = atol(argv[2]);
	int wait_each_job    = argv[3] ? atol(argv[3]) : 0;
	int sleep_per_thread = argv[4] ? atol(argv[4]) : 1;

	thread_pool thpool(num_threads);

	for (int n = 0; n < num_jobs; ++n){
        thpool.add_task([](const int* sec){
            sleep(*sec); 
        }, &sleep_per_thread);
		if (wait_each_job)
			thpool.wait_jobs();
	}
	if (!wait_each_job)
        thpool.wait_jobs();
    return 0;
}
