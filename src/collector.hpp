#pragma once

#include <condition_variable>
#include <thread>

#include "collector_producer.hpp"

class Collector
{
public:
    Collector(char* config);
    ~Collector();

private:
    std::condition_variable _producer_signal;
    std::mutex _producer_mutex;
    CollectorProducer _producer;
    
    bool _exit = false;
    std::thread _worker;
    void worker_job() noexcept;
};