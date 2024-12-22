#pragma once
#include "Unit.hpp"

namespace sw::core {

class Swordsman : public Unit {
public:
  Swordsman(uint32_t id, uint32_t x, uint32_t y, uint32_t hp, uint32_t strength)
      : Unit(id, UnitType::UnitSwordsman, x, y, hp), strength_(strength) {
        is_movable_ = true;
      }

  uint32_t attack(Unit &targetUnit) override;

  uint32_t getAffectRange() const override { return 1; };

  private:
    uint32_t strength_;
};

} // namespace sw::core
