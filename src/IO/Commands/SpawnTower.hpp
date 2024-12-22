#pragma once

#include <cstdint>
#include <string>
#include <utility>

namespace sw::io {

struct SpawnTower {
    constexpr static const char* Name = "SPAWN_TOWER";

    uint32_t unitId{};
    uint32_t x{};
    uint32_t y{};
    uint32_t hp{};
    uint32_t power{};
    uint32_t range{};

    template <typename Visitor>
    void visit(Visitor& visitor) {
        visitor.visit("unitId", unitId);
        visitor.visit("x", x);
        visitor.visit("y", y);
        visitor.visit("hp", hp);
        visitor.visit("power", power);
        visitor.visit("range", range);
    }
};

}  // namespace sw::io
