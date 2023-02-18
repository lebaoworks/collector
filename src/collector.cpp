#include "collector.hpp"
#include "macros.h"

#include <chrono>

Collector::Collector(char* config) :
    _producer(_producer_signal),
    _worker([&]{worker_job();}) { LOG(__FUNCTION__); }

Collector::~Collector()
{
    LOG(__FUNCTION__);
    _exit = true;
    _worker.join();
}

void Collector::worker_job() noexcept
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(_producer_mutex);
        _producer_signal.wait(lk);
        lk.unlock();

        if (_exit)
            break;

        auto event = _producer.get();
        // LOG("Event {} -> path: {}",
        //     int(event->type()),
        //     dynamic_cast<ProcessCreateEvent*>(event.get())->process_path
        // );
    }
}