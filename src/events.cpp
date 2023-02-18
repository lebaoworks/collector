#include "events.hpp"

#include <stdexcept>

Event::Event() {}
Event::~Event() {};
EventType Event::type() { throw std::runtime_error("Do not use interface!"); }
Event* Event::from(char* buffer, size_t buffer_size) { throw std::runtime_error("not supported"); }


#ifdef WIN32
#else
EventType ProcessCreateEvent::type()  { return EventType::ProcessCreate; }
#endif
