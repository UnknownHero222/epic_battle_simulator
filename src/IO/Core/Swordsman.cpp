#include "Swordsman.hpp"
#include <cstdlib>
namespace sw::core {

void Swordsman::attack(Unit &targetUnit) {
  // Компилятор встает на дыбы (придется кастить к int'у)
  if (std::abs(static_cast<int>(targetUnit.getX() - x_)) <= 1 &&
      std::abs(static_cast<int>(targetUnit.getY() - y_)) <= 1) {
    targetUnit.setHP(targetUnit.getHP() - strength_);
  }
}

} // namespace sw::core