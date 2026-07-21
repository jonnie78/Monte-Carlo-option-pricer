//Thread pool class header file

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<iostream>
#include<vector>
#include<thread>
#include<mutex>
#include<functional>
#include<condition_variable>
#include<queue>
#include<future>
#include"Types.h"

class ThreadPool {
private:
    //Vector to hold threads
    std::vector<std::thread> threads;
    //Queue where tasks are held
    std::queue<std::function<void()>> queue;
    //Mutex to ensure synchronisation
    std::mutex mutex;
    //Condition variable to determine if thread should take task
    std::condition_variable condition_variable;
    //Function that each thread loops to take tasks off the queue
    void worker();
    //Variable for if thread pool should stop
    bool stop;
public:
    //Constructor
    ThreadPool(std::size_t num_threads = std::thread::hardware_concurrency());
    //Destructor
    ~ThreadPool();
    //For submitting new tasks to queue
    std::future<PartialSums> submit_task(std::function<PartialSums()> task);
    //Copy constructor
    ThreadPool(const ThreadPool&) = delete;
    //Copy assignment
    ThreadPool& operator=(const ThreadPool&) = delete;
    //Move constructor
    ThreadPool(ThreadPool&&) = delete;
    //Move assignment
    ThreadPool& operator=(ThreadPool&&) = delete;
};

#endif
