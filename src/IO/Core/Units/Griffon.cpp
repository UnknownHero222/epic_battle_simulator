#include "Griffon.hpp"
#include <cmath>
#include <cstdlib>

namespace sw::core {

Griffon::Griffon(uint32_t id, uint32_t x, uint32_t y, uint32_t hp,
                 uint32_t agility)
    : Unit(id, UnitType::UnitGriffon, x, y, hp), agility_(agility) {}

void Griffon::march(uint32_t targetX, uint32_t targetY) {
  int dx = targetX > x_ ? 1 : (targetX < x_ ? -1 : 0);
  int dy = targetY > y_ ? 1 : (targetY < y_ ? -1 : 0);

  // Перемещение сразу на 2 клетки
  x_ += 2 * dx;
  y_ += 2 * dy;
}

uint32_t Griffon::attack(Unit &targetUnit) {
  int targetHp = static_cast<int>(targetUnit.getHP());

  targetHp -= static_cast<int>(agility_);
  targetUnit.setHP(static_cast<uint32_t>(std::max(0, targetHp)));

  return agility_;
}
} // namespace sw::core