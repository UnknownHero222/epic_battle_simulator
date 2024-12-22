#include "Swordsman.hpp"
#include <cstdlib>
namespace sw::core {

uint32_t Swordsman::attack(Unit &targetUnit) {
  int strength_ = static_cast<int>(getStrength());
  int targetHp = static_cast<int>(targetUnit.getHP());

  targetHp -= strength_;
  targetHp = std::max(0, targetHp);

  targetUnit.setHP(static_cast<uint32_t>(targetHp));

  return getStrength();
}

} // namespace sw::core