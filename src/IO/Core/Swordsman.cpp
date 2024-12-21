#include "Swordsman.hpp"
#include <cmath>

namespace sw::core {

void Swordsman::attack(Unit &targetUnit) {
   //if (std::abs(targetUnit.getX() - x_) <= 1 && std::abs(targetUnit.getY() - y_) <= 1) {
     targetUnit.setHP(targetUnit.getHP() - strength_);
   //}
}

}   // namespace sw::core