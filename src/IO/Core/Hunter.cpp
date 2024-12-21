#include "Hunter.hpp"

namespace sw::core {

static constexpr auto kHunterStrengthAttackDistance = 1;

void Hunter::attack(Unit &targetUnit) {
  // Компилятор встает на дыбы (придется кастить к int'у)

  uint32_t distance =
      std::max(std::abs(static_cast<int>(targetUnit.getX() - x_)),
               std::abs(static_cast<int>(targetUnit.getY() - y_)));

  if (distance <= range_ && distance > kHunterStrengthAttackDistance) {
    targetUnit.setHP(targetUnit.getHP() - agility_);
  } else {
    targetUnit.setHP(targetUnit.getHP() - strength_);
  }
}

} // namespace sw::core