#include "Simulator.hpp"
#include <iostream>
#include <stdexcept>

namespace sw::simulator {

using namespace sw::core;
using namespace sw::io;

#warning split to the more independent functions
void Simulator::run() {
  while (!unitQueue_.empty()) {
    for (auto i = 0; i < unitQueue_.size(); ++i) {
      auto unitId = unitQueue_.front();
      unitQueue_.pop();

      auto unit = getUnit(unitId);

      auto &currentCell = map_->getCell(unit->getX(), unit->getY());

      auto [isAffected, targetId] = isAffectPossible(*unit);
      if (isAffected) {
        auto damageLevel = unit->attack(*units_[targetId]);
        eventLog_.log(currentTick_,
                      UnitAttacked{unit->getId(), targetId, damageLevel,
                                   getUnit(targetId)->getHP()});
      } else {
        auto [nextX, nextY] = getNextStep(*unit);
        unit->march(nextX, nextY);

        if (nextX == unit->getTargetX() && nextY == unit->getTargetY()) {
          eventLog_.log(currentTick_, MarchEnded{unit->getId(), nextX, nextY});
        } else {
          eventLog_.log(currentTick_, UnitMoved{unit->getId(), nextX, nextY});
        }

        auto &nextCell = map_->getCell(nextX, nextY);
        nextCell.setUnit(unit);
        currentCell.removeUnit();
      }

      // погибших отбрасываем, выжившие снова в бой
      if (unit->getHP() > 0) {
        unitQueue_.push(unitId);
      } else {
        units_.erase(unitId);
        eventLog_.log(currentTick_, UnitDied{unitId});
      }
    }

    if (unitQueue_.size() <= 1) {
#warning "maybe we should anounce winner here"
      break;
    }

    currentTick_++;
  }
}

AffectedUnit Simulator::isAffectPossible(const Unit &activeUnit) {
  int range = activeUnit.getAffectRange();
  uint32_t targetUnitId{0};
  uint32_t minHP{std::numeric_limits<uint32_t>::max()};

  // TODO возможна оптизимизация по диагоналям
  for (int dx = -range; dx <= range; ++dx) {
    for (int dy = -range; dy <= range; ++dy) {
      int nx = activeUnit.getX() + dx;
      int ny = activeUnit.getY() + dy;

      if (map_->isValidPosition(nx, ny)) {
        auto &cell = map_->getCell(nx, ny);
        if (!cell.is_empty()) {
          auto targetCandidate = cell.getUnit();
          if (targetCandidate &&
              targetCandidate->getId() != activeUnit.getId()) {
            // Простенькая приоритизация, бить будем по слабейшему
            if (targetCandidate->getHP() < minHP) {
              minHP = targetCandidate->getHP();
              targetUnitId = targetCandidate->getId();
            }
          }
        }
      }
    }
  }

  if (targetUnitId != 0) {
    return std::make_tuple(true, targetUnitId);
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

std::shared_ptr<Unit> Simulator::getUnit(uint32_t unitId) {
  auto it = units_.find(unitId);
  if (it == units_.end()) {
    throw std::runtime_error("Unit with ID " + std::to_string(unitId) +
                             " not found.");
  }
  return it->second;
}

} // namespace sw::simulator
