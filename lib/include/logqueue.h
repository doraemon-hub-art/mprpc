#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

// 异步写日志的模板队列
template <typename T>
class LockQueue{

public:
    // 从队尾插入数据
    // 实现线程间的同步和互斥
    void Push(const T &data){
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(data);
        m_condvariable.notify_one();
    }
    T Pop(){
        std::unique_lock<std::mutex>lock(m_mutex);
        while(m_queue.empty()){
            // 日志队列为空，线程进入wait状态
            m_condvariable.wait(lock);
        }

        T data = m_queue.front();
        m_queue.pop();
        return data;
    }
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condvariable;
};
