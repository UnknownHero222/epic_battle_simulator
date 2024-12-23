#pragma once

#include <cstdint>
#include <string>

namespace sw::io {
struct AllTargetsReached {
  constexpr static const char *Name = "ALL_TARGETS_REACHED";

   std::string reason;

  template <typename Visitor> void visit(Visitor &visitor) {
    visitor.visit("reason", reason);
  }
};
} // namespace sw::io
