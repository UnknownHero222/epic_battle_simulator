#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace sw::core {

struct UnitParams {
    uint32_t unitId{};
    uint32_t x{};
    uint32_t y{};
    uint32_t hp{};

    UnitParams() = default;
    UnitParams(uint32_t unitId, uint32_t x, uint32_t y, uint32_t hp)
        : unitId(unitId), x(x), y(y), hp(hp) {}
    virtual ~UnitParams() = default;
};

struct SwordsmanParams : public UnitParams {
    uint32_t strength{};

    SwordsmanParams() = default;
    SwordsmanParams(uint32_t unitId, uint32_t x, uint32_t y, uint32_t hp, uint32_t strength)
        : UnitParams(unitId, x, y, hp), strength(strength) {}
};

struct HunterParams : public UnitParams {
    uint32_t agility{};
    uint32_t strength{};
    uint32_t range{};

    HunterParams() = default;
    HunterParams(uint32_t unitId, uint32_t x, uint32_t y, uint32_t hp, uint32_t agility, uint32_t strength, uint32_t range)
        : UnitParams(unitId, x, y, hp), agility(agility), strength(strength), range(range) {}
};

struct TowerParams : public UnitParams {
    uint32_t power{};
    uint32_t range{};

    TowerParams() = default;
    TowerParams(uint32_t unitId, uint32_t x, uint32_t y, uint32_t hp, uint32_t power, uint32_t range)
        : UnitParams(unitId, x, y, hp), power(power), range(range) {}
};

struct GriffonParams : public UnitParams {
    uint32_t agility{};

    GriffonParams() = default;
    GriffonParams(uint32_t unitId, uint32_t x, uint32_t y, uint32_t hp, uint32_t agility)
        : UnitParams(unitId, x, y, hp), agility(agility) {}
};

struct HealerParams : public UnitParams {
    uint32_t range{};
    uint32_t spirit{};

    HealerParams() = default;
    HealerParams(uint32_t unitId, uint32_t x, uint32_t y, uint32_t hp, uint32_t range, uint32_t spirit)
        : UnitParams(unitId, x, y, hp), range(range), spirit(spirit) {}
};

} // namespace sw::core
