#include "Simulator.hpp"
#include <iostream>
#include <stdexcept>

namespace sw::simulator {

using namespace sw::core;
using namespace sw::io;

void Simulator::run() {
  while (!unitQueue_.empty()) {
    for (auto i = 0; i < unitQueue_.size(); ++i) {
      auto unitId = unitQueue_.front();
      unitQueue_.pop();

      auto unit = getUnit(unitId);
      if (unit.getHP() <= 0) {
        continue;
      }

      auto &currentCell = map_->getCell(unit.getTargetX(), unit.getTargetY());
      auto [isAffected, targetId] = isAffectPossible(unit);
      if (isAffected) {
        unit.attack(units_[targetId]);
      } else {
#warning moving units to the correct cell on the map
        auto [nextX, nextY] = getNextStep(unit);
        unit.march(nextX, nextY);
      }

      if (unit.getHP() > 0) {
        unitQueue_.push(unitId);
      }
    }

    if (unitQueue_.size() <= 1) {
      break;
    }

    currentTick_++;
  }
}

AffectedUnit Simulator::isAffectPossible(const Unit &activeUnit) {
  int range = activeUnit.getAffectRange();
  std::shared_ptr<Unit> lowestHpTarget{nullptr};
  uint32_t minHP{std::numeric_limits<uint32_t>::max()};

  // TODO возможна оптизимизация по диагоналям
  for (int dx = -range; dx <= range; ++dx) {
    for (int dy = -range; dy <= range; ++dy) {
      int nx = activeUnit.getX() + dx;
      int ny = activeUnit.getY() + dy;

      if (map_->isValidPosition(nx, ny)) {
        auto &cell = map_->getCell(nx, ny);
        if (!cell.is_empty()) {
          auto target = cell.getUnit();
          if (target && target->getId() != activeUnit.getId()) {
            // Простенькая приоритизация, бить будем по слабейшему
            if (target->getHP() < minHP) {
              minHP = target->getHP();
              lowestHpTarget = target;
            }
          }
        }
      }
    }
  }

  if (lowestHpTarget) {
    return std::make_tuple(true, lowestHpTarget->getId());
  }

  return std::make_tuple(false, 0);
}

Coordinates Simulator::getNextStep(const Unit &unit) {
  uint32_t currentX = unit.getX();
  uint32_t currentY = unit.getY();

  uint32_t targetX = unit.getTargetX();
  uint32_t targetY = unit.getTargetY();

  uint32_t dx = (targetX > currentX) ? 1 : (targetX < currentX) ? -1 : 0;
  uint32_t dy = (targetY > currentY) ? 1 : (targetY < currentY) ? -1 : 0;

  return {currentX + dx, currentY + dy};
}

const Map &Simulator::getMap() const {
  if (!map_) {
    throw std::runtime_error("Map is not initialized.");
  }
  return *map_;
}

Unit Simulator::getUnit(uint32_t unitId) const {
  auto it = units_.find(unitId);
  if (it == units_.end()) {
    throw std::runtime_error("Unit with ID " + std::to_string(unitId) +
                             " not found.");
  }
  return it->second;
}

} // namespace sw::simulator
