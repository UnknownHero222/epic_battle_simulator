#pragma once

#include <cstdint>
#include <string>

namespace sw::io {

struct UnitHealed {

  static constexpr const char *Name = "UNIT_HEALED";

  uint32_t healerId{};
  uint32_t targetUnitId{};
  uint32_t healedAmount{};
  uint32_t targetHp{};

  template <typename Visitor> void visit(Visitor &visitor) {
    visitor.visit("healerId", healerId);
    visitor.visit("targetUnitId", targetUnitId);
    visitor.visit("healedAmount", healedAmount);
    visitor.visit("targetHp", targetHp);
  }
};

} // namespace sw::io
