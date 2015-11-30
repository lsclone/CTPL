#include "ctpl_stl.h"
#include <list>
#include <mutex>
#include <atomic>
#include <iostream>

std::mutex _mutex;
std::list<int> _list;
std::atomic<int> _index(0);

void thread_fun(int id, int loops) {
    std::cout << "thread id: " << id << std::endl;
    //std::unique_lock<std::mutex> lck(_mutex);
    for(int i = 0; i < loops; i++) {
        _mutex.lock();
        _list.push_back(_index++);
        _mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    ctpl::thread_pool tp(3);
    tp.push(thread_fun, 5);
    tp.push(thread_fun, 4);
    tp.push(thread_fun, 6);
    tp.push(thread_fun, 2);

    for(;;) {
        std::unique_lock<std::mutex> lck(_mutex);
        if(_list.size() == 0) {
            continue;
        }
        std::cout << "item: " << _list.front() << std::endl;
        _list.pop_front();
    }

    return 0;
}
