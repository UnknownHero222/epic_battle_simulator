#include "Hunter.hpp"

namespace sw::core {

static constexpr auto kHunterStrengthAttackDistance = 1;

ActionResult Hunter::action(Unit &targetUnit) {
  auto damage = attack(targetUnit);

  return {ActionType::Attack, damage, targetUnit.getId(), targetUnit.getHP()};
}

uint32_t Hunter::attack(Unit &targetUnit) {
  // Компилятор встает на дыбы (придется кастить к int'у)
  uint32_t distance =
      std::max(std::abs(static_cast<int>(targetUnit.getX() - x_)),
               std::abs(static_cast<int>(targetUnit.getY() - y_)));

  int targetHp = static_cast<int>(targetUnit.getHP());
  int initTargetHp = targetHp;

  if (distance <= range_ && distance > kHunterStrengthAttackDistance) {
    targetHp -= static_cast<int>(agility_);
  } else if (distance == kHunterStrengthAttackDistance) {
    targetHp -= static_cast<int>(strength_);
  }

  targetHp = std::max(0, targetHp);
  targetUnit.setHP(static_cast<uint32_t>(targetHp));

  return initTargetHp - targetHp;
}

} // namespace sw::core