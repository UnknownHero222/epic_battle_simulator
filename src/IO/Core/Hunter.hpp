#pragma once
#include "Unit.hpp"

namespace sw::core {

class Hunter : public Unit {
public:
  explicit Hunter(uint32_t id, uint32_t x, uint32_t y, uint32_t hp,
                  uint32_t strength, uint32_t agility, uint32_t range)
      : Unit(id, UnitType::UnitHunter, x, y, hp, strength), agility_(agility),
        range_(range) {}

  uint32_t getAgility() const { return agility_; }
  uint32_t getRange() const { return range_; }

private:
  uint32_t agility_;
  uint32_t range_;
};

} // namespace sw::core
