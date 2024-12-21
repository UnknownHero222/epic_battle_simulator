#pragma once
#include "Unit.hpp"

namespace sw::core {

class Swordsman : public Unit {
public:
  Swordsman(uint32_t id, uint32_t x, uint32_t y, uint32_t hp, uint32_t strength)
      : Unit(id, UnitType::UnitSwordsman, x, y, hp, strength) {}

  void attack(Unit &targetUnit) override;
};

} // namespace sw::core
