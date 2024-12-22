#pragma once

#include "Unit.hpp"

namespace sw::core {

class Tower : public Unit {
public:
    Tower(uint32_t id, uint32_t x, uint32_t y, uint32_t hp, uint32_t power, uint32_t range);

    uint32_t attack(Unit &targetUnit) override;

private:
    uint32_t power_;
    uint32_t range_;
};

}  // namespace sw::core
