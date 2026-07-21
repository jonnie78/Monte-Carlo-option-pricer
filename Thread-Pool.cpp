#include"Thread-Pool.h"

using namespace std;

//Constructor
ThreadPool::ThreadPool(size_t num_threads) {
    stop = false;
    for (size_t i = 0; i < num_threads; i++) {
        threads.emplace_back(&ThreadPool::worker, this);
    }
}
//Destructor
ThreadPool::~ThreadPool() {
    unique_lock<std::mutex> lock(mutex);
    stop = true;
    condition_variable.notify_all();
    for (thread& thread : threads) {
        thread.join();
    }
}
//Submit task function
future<PartialSums> ThreadPool::submit_task(function<PartialSums()> task) {
    auto packaged = make_shared<packaged_task<PartialSums()>>(task);
    future<PartialSums> result = packaged -> get_future();
    unique_lock<std::mutex> lock(mutex);
    queue.emplace([packaged] () {(*packaged)();});
    condition_variable.notify_one();
    return result;
}
//Worker function
void ThreadPool::worker() {
    while (true) {
        function<void()> task; {
            unique_lock<std::mutex> lock(mutex);
            condition_variable.wait(lock, [this] { return stop || !queue.empty(); });
            if (stop && queue.empty()) {
                return;
            }
            task = move(queue.front());
            queue.pop();
        }
        task();
    }
}