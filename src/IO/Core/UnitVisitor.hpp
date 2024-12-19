#pragma once

#include "Hunter.hpp"
#include "Swordsman.hpp"
#include "Unit.hpp"
#include <memory>
#include <stdexcept>
#include <string>

namespace sw::core {

class UnitVisitor {
public:
  explicit UnitVisitor(const std::string &name) {
    if (name == "SPAWN_SWORDSMAN") {
      unitType_ = UnitType::UnitSwordsman;
    } else if (name == "SPAWN_HUNTER") {
      unitType_ = UnitType::UnitHunter;
    }
  }

  template <typename T> void visit(const std::string &name, const T &value) {
    if (name == "unitId") {
      unitId_ = value;
    } else if (name == "x") {
      x_ = value;
    } else if (name == "y") {
      y_ = value;
    } else if (name == "hp") {
      hp_ = value;
    } else if (name == "strength") {
      strength_ = value;
    } else if (name == "agility") {
      agility_ = value;
    } else if (name == "range") {
      range_ = value;
    }
  }

  std::shared_ptr<Unit> build() const {
    switch (unitType_) {
    case UnitType::UnitSwordsman:
      return std::make_shared<Swordsman>(unitId_, x_, y_, hp_, strength_);
    case UnitType::UnitHunter:
      return std::make_shared<Hunter>(unitId_, x_, y_, hp_, strength_, agility_,
                                      range_);
    default:
      break;
    }

    throw std::runtime_error("Unsupported unit type");
  }

private:
  UnitType unitType_;
  uint32_t unitId_, x_, y_, hp_, strength_, agility_, range_;
};

} // namespace sw::core
