#include <ratio>
#include <vector>   // std::vector
#include <string>   // std::string
#include <future>   // std::future
#include <thread>   // std::this_thread::sleep_for
#include <chrono>   // std::chrono::seconds
#include <iostream> // std::cout, std::endl

#include "thread_pool.hpp"

int main()
{
    std::mutex mtx;
    // create a thread pool with max. 4 concurrency threads
    thread_pool pool(4);
    // create execution results list
    std::vector< std::future<std::string> > results;

    // start eight thread task
    for(int i = 0; i < 8; ++i) {
        // add all task to result list
        results.emplace_back(
            // ass print task to thread pool
            pool.add_task([&mtx, i] {
                mtx.lock();
                std::cout << "hello " << i << std::endl;
                mtx.unlock();
                // wait a sec when the previous line is out
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                // keep output and return the status of execution
                mtx.lock(); 
                std::cout << "world " << i << std::endl;
                mtx.unlock();
                return std::string("---thread ") + std::to_string(i) + std::string(" finished.---");
            })
        );
    }

    // outputs
    for(auto && result: results)
        std::cout << result.get() << std::endl;
    std::cout << std::endl;

    return 0;
}
