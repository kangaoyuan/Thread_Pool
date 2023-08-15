#include <thread>
#include <iostream>
#include "thread_pool.hpp"

void task(int arg){
    std::cout << "Thread #" << std::this_thread::get_id() << " working on " << arg <<std::endl;
}

int main(){
    std::cout << "Initializing thread pool with 7 threads" << std::endl;
    auto thpool = thread_pool(7);

    std::cout << "Adding 70 tasks to thread pool" << std::endl;
    for(int i = 0; i < 70; ++i){
        thpool.add_task(task, i); 
    }
    thpool.wait_jobs();

    std::cout << "thread_pool ending" << std::endl;
    return 0;
}
