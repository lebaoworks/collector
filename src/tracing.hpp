#include <chrono>
#include <stdint.h>

namespace tracing
{

struct info
{
    size_t count = 0;
    uint64_t total_elapsed = 0; // nanoseconds
};
void begin(const char*);
void end(const char*);
info report(const char*);

};