#include "tracing.hpp"

#include <unordered_map>

struct event : tracing::info {
    decltype(std::chrono::steady_clock::now()) begin;
};
thread_local std::unordered_map<const char*, event> events;

void tracing::begin(const char* name)
{
    decltype(events[name])& obj = events[name];
    obj.count++;
    obj.begin = std::chrono::steady_clock::now();
}

void tracing::end(const char* name)
{
    decltype(events[name])& obj = events[name];
    obj.total_elapsed += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - obj.begin).count();
}

tracing::info tracing::report(const char* name)
{
    return events[name];
}