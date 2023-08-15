#include <iostream>
#include "../../thread_pool.hpp"

int main(){
    int num = 0;
    thread_pool thpool(10);

    for(int i = 0; i < 4; i++){
        thpool.add_task([](){
            sleep(2);
            std::cout << "after 2s sleep" << std::endl;
        });
    }

    sleep(1);
    num = thpool.get_num_threads_working();
	if (num != 4) {
		printf("Expected 4 threads working, got %d \n", num);
		return -1;
	};

	/* Test (same as above) */
	thread_pool thpool1(5);
    for(int i = 0; i < 2; i++){
        thpool1.add_task([](){
            sleep(2);
            std::cout << "after 2s sleep" << std::endl;
        });
    }

    sleep(1);
    num = thpool1.get_num_threads_working();
	if (num != 2) {
		printf("Expected 2 threads working, got %d", num);
		return -1;
	};

	return 0;
}

