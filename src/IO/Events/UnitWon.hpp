#pragma once

#include <cstdint>
#include <string>

namespace sw::io {
struct UnitWon {
  constexpr static const char *Name = "UNIT_WON";

  uint32_t unitId{};
  std::string unitType{};

  template <typename Visitor> void visit(Visitor &visitor) {
    visitor.visit("unitId", unitId);
    visitor.visit("unitType", unitType);
  }
};
} // namespace sw::io
