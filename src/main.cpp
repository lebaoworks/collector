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

#include <set>
#include <memory>

template<typename T>
class ActionSet
{
private:
    struct _ActionSet {
        std::mutex _mutex;
        std::set<T> _set;
    };
    struct _ActionRef {
        std::shared_ptr<_ActionSet> _action_set;
        typename std::set<T>::iterator _ite;
        _ActionRef(std::shared_ptr<_ActionSet>& action_set, typename std::set<T>::iterator ite) : _action_set(action_set), _ite(ite) {}
        ~_ActionRef() { std::lock_guard<std::mutex> lk(_action_set->_mutex); _action_set->_set.erase(_ite); }
    };
    
    std::shared_ptr<_ActionSet> _shared;
public:
    ActionSet() : _shared(std::make_shared<_ActionSet>()) {}
    ~ActionSet() {}

    std::shared_ptr<_ActionRef> acquire(const T& key)
    {
        std::lock_guard<std::mutex> lk(_shared->_mutex);
        auto ret = _shared->_set.insert(key);
        if (ret.second)
            return std::make_shared<_ActionRef>(_shared, ret.first);
        return nullptr;
    }
};

int main()
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    {
        ActionSet<std::string> a;
        auto locker = a.acquire("OK");
        if (locker == nullptr)
            printf("Failed");
        auto locker2 = a.acquire("OK");
        if (locker2 != nullptr)
            printf("Failed");
        std::thread([&, locker]{ std::this_thread::sleep_for(std::chrono::milliseconds(1000)); }).detach();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return 0;
} 