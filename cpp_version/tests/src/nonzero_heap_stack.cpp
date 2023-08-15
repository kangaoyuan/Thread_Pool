#include <cstring>
#include <iostream>
#include "../../thread_pool.hpp"

void task(){
	printf("Thread #%u working on task\n", (int)pthread_self());
}


void nonzero_stack(){
    char buf[40096];
    memset(buf, 0x80, 40096);
}


void nonzero_heap(){
    void *ptrs[200];

    for (int i = 0; i < 200; i++){
        ptrs[i] = malloc((i+1) << 4);
        if (ptrs[i])
            memset(ptrs[i], 0x80, (i+1) << 4);
    }

    for (int i = 0; i < 200; i++){
        free(ptrs[i]);
    }
}

int main(){
    nonzero_stack();
    nonzero_heap();

    std::cout << "Making thread pool with 4 threads" << std::endl;

    thread_pool thpool(4);
    for(int i = 0; i < 20; ++i){
        thpool.add_task(task);
    }

    return 0;
}


