#include "Healer.hpp"

namespace sw::core {

Healer::Healer(uint32_t id, uint32_t x, uint32_t y, uint32_t hp, uint32_t range,
               uint32_t spirit)
    : Unit(id, "Healer", x, y, hp), range_(range), spirit_(spirit) {
  isAttackable_ = false;
}

uint32_t Healer::heal(Unit &target) {
  if (target.getHP() == target.getMaxHP()) {
    return 0;
  }

  uint32_t healedAmount = std::min(target.getHP() + spirit_, target.getMaxHP());
  target.setHP(healedAmount);

  return spirit_;
}

} // namespace sw::core