#include "Swordsman.hpp"
#include <cstdlib>
namespace sw::core {

ActionResult Swordsman::action(Unit &targetUnit) {
  auto damage = attack(targetUnit);

  return {ActionType::Attack, damage, targetUnit.getId(), targetUnit.getHP()};
}

uint32_t Swordsman::attack(Unit &targetUnit) {
  int targetHp = static_cast<int>(targetUnit.getHP());

  targetHp -= static_cast<int>(strength_);

  targetUnit.setHP(static_cast<uint32_t>(std::max(0, targetHp)));

  return strength_;
}

bool Swordsman::canAttack(const Unit &targetUnit) const {
  return (targetUnit.isFlying()) ? false : true;
}

} // namespace sw::core