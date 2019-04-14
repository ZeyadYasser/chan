#ifndef CHAN_H
#define CHAN_H

#include <queue>
#include <thread>
#include <condition_variable>

template<typename T>
class Chan {
public:
    Chan() : m_closed(false) {}
    void push(const T &val) {
        std::unique_lock<std::mutex> lock(m_mutex);
        if(m_closed)
            std::runtime_error("channel closed");
        m_queue.push(val);
        m_cv.notify_one();
    }
    bool get(T &res) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [&](){
            return m_closed || !m_queue.empty(); 
        });
        if(m_queue.empty())
            return false;
        res = m_queue.front();
        m_queue.pop();
        return true;
    }
    void close() {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_closed = true;
        m_cv.notify_all();
    }
    bool closed() {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_closed;
    }
private:
    std::queue<T> m_queue;
    bool m_closed;
    std::mutex m_mutex;
    std::condition_variable m_cv;
};

#endif