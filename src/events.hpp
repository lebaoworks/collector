#pragma once

#include <stddef.h>
#include <string>

enum class EventType;

struct Event
{
    Event();
    virtual ~Event();

    /// @brief Get event's type
    /// @return Enum type of the event
    virtual EventType type();
    
    /// @brief Deserialize data to a event
    /// @param buffer array byte of data
    /// @param buffer_size size of the array
    /// @return pointer to a successfully deserialized event
    /// @exception runtime_error on any errors
    static Event* from(char* buffer, size_t buffer_size);
};

#ifdef WIN32
#else
enum class EventType {
    ProcessCreate,
    ProcessTerminate,
    ThreadCreate
};

struct ProcessCreateEvent : Event
{
    std::string process_path = "/bin/ls";

    EventType type();
};
#endif