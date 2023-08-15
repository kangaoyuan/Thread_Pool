#include <mutex>                // std::mutex
#include <queue>                // std::queue<>
#include <thread>               // std::thread
#include <atomic>               // std::atomic<>
#include <future>               // std::future<>
#include <csignal>              // sigal
#include <tuple>
#include <utility>              // std::move()
#include <cassert>              // assert         
#include <iostream>             // std::cout...
#include <functional>           // std::function<()>
#include <type_traits>          // std::result_of_t<()>
#include <condition_variable>   // std::condition_variable

class thread_pool{
public:
    ~thread_pool();
    thread_pool(int num_threads);

    thread_pool(const thread_pool&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;

    void wait_jobs();
    void pause_threads();
    void resume_threads();
    static void thread_hold(int sig_id);
    template<typename F, typename... Args>
    decltype(auto) add_task(F&& f, Args&&... args);
    
    int get_num_threads_working(){
        return num_threads_working; 
    }

private:
    std::mutex task_mtx, count_mtx;
    std::condition_variable task_cv, idle_cv;
    
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;

    bool stop = {false};
    static int threads_on_hold;
    std::atomic_int num_threads_alive = {0};
    // The number of working threads which are performing tasks(not idle)
    std::atomic_int num_threads_working = {0};
};
int thread_pool::threads_on_hold = 0;

template<typename F, typename... Args>
decltype(auto) thread_pool::add_task(F&& f, Args&&... args){
    //using result_type = decltype(declval<F>(f)(args...));
    using result_type = std::result_of_t<F(Args...)>;

    //std::packaged_task<result_type()> task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    auto task = std::make_shared<std::packaged_task<result_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    std::future<result_type> fut_result = task->get_future();

    std::unique_lock<std::mutex> queue_lck(task_mtx);
    jobs.emplace([task](){(*task)();});
    queue_lck.unlock();

    task_cv.notify_one();
    return fut_result;
}

// Will wait for all queued jobs to finish.
void thread_pool::wait_jobs(){
    std::unique_lock<std::mutex> count_lck(count_mtx);
    while(jobs.size() || num_threads_working){
       idle_cv.wait(count_lck); 
    } 
}

void thread_pool::thread_hold(int sig_id){
    threads_on_hold = 1;
    while(threads_on_hold){
        std::this_thread::yield(); 
    }
}

// All threads will be paused no matter if they are idle and working. While being paused, new job can be added.
void thread_pool::pause_threads(){
    for(int n = 0; n < num_threads_alive; n++){
        pthread_kill(threads[n].native_handle(), SIGUSR1);
    }
}

// All threads will return to their previous states continuing processing, if they are paused.
void thread_pool::resume_threads(){
    threads_on_hold = 0;
}

thread_pool::thread_pool(int num_threads){
    assert(num_threads > 0);
    for(int i = 0; i < num_threads; i++){
        // each thread in thread_pool is an endless loop callable object(lambda), waiting the condition_variable to notify when the job(task) added.
        threads.emplace_back([this](){
            struct sigaction act;
            act.sa_flags = 0;
            sigemptyset(&act.sa_mask);
            act.sa_handler = &thread_pool::thread_hold;
            if(sigaction(SIGUSR1, &act, NULL) == -1){
                fprintf(stderr, "can't set handler SIGUSR1 for thread");
            }
            num_threads_alive++; 
            while(true){ 
                std::unique_lock<std::mutex> queue_lck(task_mtx);
                task_cv.wait(queue_lck, [this](){
                    return stop || !jobs.empty(); 
                }); 
                if(stop && jobs.empty())
                    break;
                num_threads_working++;
                auto task = std::move(jobs.front());
                jobs.pop();
                queue_lck.unlock();
                task();
                num_threads_working--;
                if(!num_threads_working)
                    idle_cv.notify_one();
            }
            num_threads_alive--;
        });
        // threads[i].detach();
    } 
    while(num_threads_alive != num_threads){}
}

thread_pool::~thread_pool(){
    stop = true;

    while(num_threads_alive){
        task_cv.notify_all();
    }

    //std::cout << "~thread_pool() clear all threads" << std::endl;
    for(auto& thread: threads){
        thread.join(); 
    }
};

