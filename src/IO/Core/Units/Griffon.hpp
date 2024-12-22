#pragma once

#include "Unit.hpp"
#include <memory>

namespace sw::core {

class Griffon : public Unit {
public:
  Griffon(uint32_t id, uint32_t x, uint32_t y, uint32_t hp, uint32_t agility);

  void march(uint32_t targetX, uint32_t targetY) override;

  uint32_t attack(Unit &targetUnit) override;

  uint32_t getAffectRange() const override { return 2; };

private:
  uint32_t agility_;
};

} // namespace sw::core
