#pragma once

#include "Units/Griffon.hpp"
#include "Units/Healer.hpp"
#include "Units/Hunter.hpp"
#include "Units/Swordsman.hpp"
#include "Units/Tower.hpp"
#include "Units/Unit.hpp"
#include <memory>
#include <stdexcept>
#include <string>

namespace sw::core {

enum UnitType { UnitSwordsman, UnitHunter, UnitTower, UnitGriffon, UnitHealer };

class UnitVisitor {
public:
  explicit UnitVisitor(const std::string &name) {
    if (name == "SPAWN_SWORDSMAN") {
      unitType_ = UnitType::UnitSwordsman;
    } else if (name == "SPAWN_HUNTER") {
      unitType_ = UnitType::UnitHunter;
    } else if (name == "SPAWN_TOWER") {
      unitType_ = UnitType::UnitTower;
    } else if (name == "SPAWN_GRIFFON") {
      unitType_ = UnitType::UnitGriffon;
    } else if (name == "SPAWN_HEALER") {
      unitType_ = UnitType::UnitHealer;
    } else {
      throw std::runtime_error("Unsupported unit type");
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
    case UnitType::UnitTower:
      return std::make_shared<Tower>(unitId_, x_, y_, hp_, power_, range_);
    case UnitType::UnitGriffon:
      return std::make_shared<Griffon>(unitId_, x_, y_, hp_, agility_);
    case UnitType::UnitHealer:
      return std::make_shared<Healer>(unitId_, x_, y_, hp_, range_, spirit_);
    default:
      break;
    }

    throw std::runtime_error("Unsupported unit type");
  }

private:
  UnitType unitType_;
  uint32_t unitId_, x_, y_, hp_;
  uint32_t strength_, agility_, range_, power_, spirit_;
};

} // namespace sw::core
