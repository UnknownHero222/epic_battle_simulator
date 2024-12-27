#pragma once

#include <cstdint>

namespace sw::core {

// при необходимости можно добавить дополнительные типы действий
enum class ActionType {
    Attack,
    Heal,
    None
};

struct ActionResult {
    ActionType type{ActionType::None}; 
    uint32_t value{0};                
    uint32_t targetId{0};              
    uint32_t remainingHP{0};           
};

}