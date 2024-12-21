#include "Unit.hpp"

namespace sw::core {

void Unit::march(uint32_t targetX, uint32_t targetY) {
    if (x_ < targetX) {
      ++x_;
    } else if (x_ > targetX) {
      --x_;
    }

    if (y_ < targetY) {
      ++y_;
    } else if (y_ > targetY) {
      --y_;
    }
  }

  std::string Unit::getUnitName() const {
    switch (type_) {
    case UnitSwordsman:
      return "Swordsman";
    case UnitHunter:
      return "Hunter";
    default:
      return "Unknown";
    }
  }

}