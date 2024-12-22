#include "Griffon.hpp"
#include <cmath>
#include <cstdlib>

namespace sw::core {

Griffon::Griffon(uint32_t id, uint32_t x, uint32_t y, uint32_t hp,
                 uint32_t agility)
    : Unit(id, UnitType::UnitGriffon, x, y, hp), agility_(agility) {
  isFlying_ = true;
}

void Griffon::march(uint32_t targetX, uint32_t targetY) {
  if (x_ < targetX) {
    x_ += 2;
  } else if (x_ > targetX) {
    x_ -= 2;
  }

  if (y_ < targetY) {
    y_ += 2;
  } else if (y_ > targetY) {
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