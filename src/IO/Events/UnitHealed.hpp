#pragma once

#include <cstdint>
#include <string>

namespace sw::io {

struct UnitHealed {
    uint32_t healerId;
    uint32_t targetId;
    uint32_t healedAmount;
    uint32_t targetHp;

    template <typename Visitor>
    void visit(Visitor &visitor) {
        visitor(healerId, "healerId");
        visitor(targetId, "targetId");
        visitor(healedAmount, "healedAmount");
        visitor(targetHp, "targetHp");
    }

    static constexpr const char *Name = "UNIT_HEALED";
};

} // namespace sw::io
