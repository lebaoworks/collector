#include "collector_producer.hpp"
#include "macros.h"

CollectorProducer::CollectorProducer(std::condition_variable& event_signal) :
    _event_signal(event_signal),
    _producer([&]{producer_job();}) { LOG(__FUNCTION__); }

CollectorProducer::~CollectorProducer()
{
    _not_exit = false;
    _producer.join();
    LOG(__FUNCTION__);
}

std::unique_ptr<Event> CollectorProducer::get()
{
    std::lock_guard<std::mutex> lk(_queue_mutex);
    auto ret = std::move(_queue.front());
    _queue.pop();
    return ret;
}

void CollectorProducer::producer_job() noexcept
{
    while (_not_exit)
    {
        try
        {
            std::lock_guard<std::mutex> lk(_queue_mutex);
            _queue.push(std::unique_ptr<Event>(new ProcessCreateEvent()));
            _event_signal.notify_one();
        }
        catch (std::exception& e) { TRACE_EXCEPTION(e); }
    }
}