#pragma once
#include <mutex>

class RWLock {
public:
    RWLock()
        : m_read_number(0) {}

    void ReadLock() {
        m_read.lock();
        ++m_read_number;
        if (m_read_number == 1) {
            m_write.lock();
        }
        m_read.unlock();
    }

    void ReadUnlock() {
        m_read.lock();
        --m_read_number;
        if (m_read_number == 0) {
            m_write.unlock();
        }
        m_read.unlock();
    }

    void WriteLock() {
        m_write.lock();
    }

    void WriteUnlock() {
        m_write.unlock();
    }

protected:
    int m_read_number;
    std::mutex m_read;
    std::mutex m_write;
    std::mutex m_lock;
};
