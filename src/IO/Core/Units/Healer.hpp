#pragma once

#include "Unit.hpp"

namespace sw::core {

class Healer : public Unit {
public:
  Healer(uint32_t id, uint32_t x, uint32_t y, uint32_t hp, uint32_t range,
         uint32_t spirit);

  ActionResult action(Unit &targetUnit) override;

  uint32_t getRange() const { return range_; }
  uint32_t getSpirit() const { return spirit_; }

private:
  uint32_t heal(Unit &target);

private:
  uint32_t range_;

  uint32_t spirit_;
};

} // namespace sw::core