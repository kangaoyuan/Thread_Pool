#include <iostream>
#include "../../thread_pool.hpp"

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cerr << "This testfile need exactly one argument";
        exit(1); 
    }
    int num_threads = atoi(argv[1]);

    thread_pool thpool(num_threads);

    return 0;
}
