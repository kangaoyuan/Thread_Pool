#include <iostream>
#include "../../thread_pool.hpp"

int main(int argc, char* argv[]){
    if(argc != 3){
        std::cerr << "This testfile need exactly two arguments" << std::endl;
        exit(1);
    }

    int sum = 0;
    int num_jobs = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    thread_pool thpool(num_threads);

    for(int i = 0; i < num_jobs; ++i){
        std::mutex mtx;
        thpool.add_task([&](){
            std::unique_lock<std::mutex> lck(mtx);
            sum++;}); 
    }
    thpool.wait_jobs();

    std::cout << "sum = " << sum << std::endl;
    return 0;
}
