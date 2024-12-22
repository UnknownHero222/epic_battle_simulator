#include "Tower.hpp"

#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <stdexcept>

namespace sw::core {

    Tower::Tower(uint32_t id, uint32_t x, uint32_t y, uint32_t hp, uint32_t power, uint32_t range)
    : Unit(id, UnitType::UnitHunter, x, y, hp), power_(power), range_(range) {
        is_movable_ = false;
}

uint32_t Tower::attack(Unit& targetUnit) {
    uint32_t distance = std::max(std::abs(static_cast<int>(targetUnit.getX() - getX())),
                                 std::abs(static_cast<int>(targetUnit.getY() - getY())));

    int targetHp = static_cast<int>(targetUnit.getHP());
        
    targetHp -= power_;
    targetHp = std::max(0, targetHp);

    targetUnit.setHP(static_cast<uint32_t>(targetHp));

    return power_;    
}

}  // namespace sw::core