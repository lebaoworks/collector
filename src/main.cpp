#include <cstring>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <stdint.h>
#include <thread>

#include "collector.hpp"
#include "macros.h"

static std::unique_ptr<Collector> obj;
static std::unique_ptr<char[]> config;

int init(const char* data, uint32_t data_size)
{
    if (data != nullptr)
        try
        {
            config.reset(new char[data_size]);
            memcpy(config.get(), data, data_size);
            return 0;
        } catch (std::exception& e) { TRACE_EXCEPTION(e); }
    return 1;
}

int start()
{
    try
    {
        obj.reset(new Collector(config.get()));
        return 0;
    } catch (std::exception& e) { TRACE_EXCEPTION(e); }
    return 1;
}

int stop()
{
    try
    {
        obj.reset();
        return 0;
    } catch (std::exception& e) { TRACE_EXCEPTION(e); }
    return 1;
}

int clean_up()
{
    return stop();
}

int main()
{
    std::string config = "asd";
    init(config.c_str(), config.length());
    start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    stop();
    return 0;
}