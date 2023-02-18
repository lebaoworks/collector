#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

#include "events.hpp"

class CollectorProducer
{ 
public:
    CollectorProducer(std::condition_variable& event_signal);
    ~CollectorProducer();

    /// @brief Get a event from queue
    /// @return A pointer to event
    /// @exception runtime_error if queue is empty
    /// @note The returned pointer is always a valid pointer
    std::unique_ptr<Event> get();

private:
    std::condition_variable& _event_signal;
    std::queue<std::unique_ptr<Event>> _queue;
    std::mutex _queue_mutex;

    std::thread _producer;
    volatile bool _not_exit = true;
    void producer_job() noexcept;
};