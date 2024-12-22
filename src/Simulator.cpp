#include "Simulator.hpp"
#include <iostream>
#include <stdexcept>

namespace sw::simulator {

using namespace sw::core;
using namespace sw::io;

void Simulator::run() {
  while (!unitQueue_.empty()) {
    size_t queueSize = unitQueue_.size();
    for (size_t i = 0; i < queueSize; ++i) {
      uint32_t unitId = unitQueue_.front();
      unitQueue_.pop();

      // проверка жив ли еще юнит (если нет - в Зал Славы)
      if (getUnit(unitId)->getHP() <= 0) {
        handleDeadUnit(unitId);
        continue;
      }

      processUnitTurn(unitId);

      // Погибшие в зал славы, выжившие сражаются/двигаются дальше
      if (units_.at(unitId)->getHP() > 0) {
        unitQueue_.push(unitId);
      } else {
        handleDeadUnit(unitId);
      }
    }

    if (checkSimulationEnd()) {
      break;
    }

    currentTick_++;
  }
}

void Simulator::processUnitTurn(uint32_t unitId) {
  auto unit = getUnit(unitId);
  auto &currentCell = map_->getCell(unit->getX(), unit->getY());

  auto [isAffected, targetId] = isAffectPossible(*unit);
  if (isAffected) {
    processAttack(unit, targetId);
  } else {
    processMovement(unit);
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

void Simulator::processAttack(std::shared_ptr<Unit> &unit, uint32_t targetId) {
  auto damageLevel = unit->attack(*units_[targetId]);
  eventLog_.log(currentTick_, UnitAttacked{unit->getId(), targetId, damageLevel,
                                           getUnit(targetId)->getHP()});
}

void Simulator::processMovement(std::shared_ptr<Unit> &unit) {
  if (!unit->isMovable()) {
    return;
  }

  auto [nextX, nextY] = getNextStep(*unit);
  unit->march(nextX, nextY);

  if (nextX == unit->getTargetX() && nextY == unit->getTargetY()) {
    eventLog_.log(currentTick_, MarchEnded{unit->getId(), nextX, nextY});
  } else {
    eventLog_.log(currentTick_, UnitMoved{unit->getId(), nextX, nextY});
  }

  auto &currentCell = map_->getCell(unit->getX(), unit->getY());
  auto &nextCell = map_->getCell(nextX, nextY);

  nextCell.setUnit(unit);
  currentCell.removeUnit();
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

void Simulator::handleDeadUnit(uint32_t unitId) {
  auto &currentCell = map_->getCell(getUnit(unitId)->getX(), getUnit(unitId)->getY());
  currentCell.removeUnit();

  units_.erase(unitId);

  eventLog_.log(currentTick_, UnitDied{unitId});
}

bool Simulator::checkSimulationEnd() {
  if (unitQueue_.size() <= 1) {
    eventLog_.log(currentTick_, UnitWon{unitQueue_.front()});
    return true;
  }
#warning "Second condition should be added here no units to move"

  return false;
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
