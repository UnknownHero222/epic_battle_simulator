#include "Griffon.hpp"
#include <cmath>
#include <cstdlib>

namespace sw::core {

Griffon::Griffon(uint32_t id, uint32_t x, uint32_t y, uint32_t hp,
                 uint32_t agility)
    : Unit(id, "Griffon", x, y, hp), agility_(agility) {
  isFlying_ = true;
}

ActionResult Griffon::action(Unit &targetUnit) {
  auto damage = attack(targetUnit);

  return {ActionType::Attack, damage, targetUnit.getId(), targetUnit.getHP()};
}

void Griffon::march() {
  if (x_ < targetX_) {
    x_ += 2;
  } else if (x_ > targetX_) {
    x_ -= 2;
  }

  if (y_ < targetY_) {
    y_ += 2;
  } else if (y_ > targetY_) {
    y_ -= 2;
  }
}

uint32_t Griffon::attack(Unit &targetUnit) {
  int targetHp = static_cast<int>(targetUnit.getHP());

  targetHp -= static_cast<int>(agility_);
  targetUnit.setHP(static_cast<uint32_t>(std::max(0, targetHp)));

  return agility_;
}
} // namespace sw::core