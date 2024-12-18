#pragma once

#include "Event.hpp"

namespace sw::core {

class EventHandler {
public:
    virtual ~EventHandler() = default;
    virtual void handle(const Event& event) = 0;
};

}