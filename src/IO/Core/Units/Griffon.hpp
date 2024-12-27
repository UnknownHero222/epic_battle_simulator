#pragma once

#include "Unit.hpp"
#include <memory>

namespace sw::core {

static constexpr auto kGriffonDefaultAttackRange = 2;

class Griffon : public Unit {
public:
  Griffon(uint32_t id, uint32_t x, uint32_t y, uint32_t hp, uint32_t agility);

  void march() override;

  ActionResult action(Unit &targetUnit) override;

  uint32_t getAffectRange() const override {
    return kGriffonDefaultAttackRange;
  };

private:
  uint32_t attack(Unit &targetUnit);

private:
  uint32_t agility_;
};

} // namespace sw::core
