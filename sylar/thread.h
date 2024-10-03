#ifndef __SYLAR_THREAD_H__
#define __SYLAR_THREAD_H__
#include <iostream>
#include <memory>
#include <functional>
#include <pthread.h>
#include <thread>
#include <stdint.h>
#include <semaphore.h>
namespace sylar
{

    class Semaphore
    {
    public:
        Semaphore(uint32_t count = 0);
        ~Semaphore();
        bool wait();
        void notify();
        bool tryWait();
        bool post();

    private:
        Semaphore(const Semaphore &) = delete;
        Semaphore(const Semaphore &&) = delete;
        Semaphore &operator=(const Semaphore &) = delete;

    private:
        sem_t m_semaphore;
    };

    class Thread
    {
    public:
        typedef std::shared_ptr<Thread> ptr;
        Thread(std::function<void()> cb, const std::string &name);
        ~Thread();
        pid_t getId() const { return m_id; }

        const std::string &getName() const { return m_name; }

        void join();

        static Thread *GetThis();
        static const std::string GetName();
        static void SetName(const std::string &name);

    private:
        Thread(const Thread &) = delete;
        Thread(const Thread &&) = delete;
        Thread &operator=(const Thread &) = delete;
        static void *run(void *arg);

    private:
        pid_t m_id = -1;
        pthread_t m_thread = 0;
        std::function<void()> m_cb;
        std::string m_name;
    };
}
#endif