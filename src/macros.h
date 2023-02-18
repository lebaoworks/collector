#include <spdlog/spdlog.h>

#define TRACE_VARIABLE(x) spdlog::info("{} = {} at {}:{}", #x, x, __FILE__, __LINE__)
#define TRACE_EXCEPTION(e) spdlog::warn("Exception: \"{}\" at {}:{}", e.what(), __FILE__, __LINE__)
#define LOG(...) spdlog::info(__VA_ARGS__)
