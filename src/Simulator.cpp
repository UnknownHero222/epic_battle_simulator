#include "Simulator.hpp"
#include <iostream>
#include <limits>
#include <stdexcept>

namespace sw::simulator {

using namespace sw::core;
using namespace sw::io;

void Simulator::run() {
  if (!map_) {
    throw std::runtime_error("Map is not initialized.");
  }

  while (!unitQueue_.empty()) {
    size_t queueSize = unitQueue_.size();
    for (size_t i = 0; i < queueSize; ++i) {
      uint32_t unitId = unitQueue_.front();
      unitQueue_.pop();

      // а не погиб ли юнит на предыдущем ходу?
      if (getUnit(unitId)->getHP() <= 0) {
        handleDeadUnit(unitId);
        continue;
      }

      handleUnitAction(unitId);

      // Погибшие в зал славы, выжившие сражаются/двигаются дальше
      if (units_.at(unitId)->getHP() > 0) {
        unitQueue_.push(unitId);
      } else {
        handleDeadUnit(unitId);
      }
    }

    if (hasWinner()) {
      break;
    }

    if (allUnitsAtTargets()) {
      break;
    }

    currentTick_++;
  }
}

void Simulator::handleUnitAction(uint32_t unitId) {
  auto unit = getUnit(unitId);
  auto &currentCell = map_->getCell(unit->getX(), unit->getY());

  auto [isAffected, targetId] = isAffectPossible(*unit);
  if (isAffected) {
    processAction(unit, targetId);
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

      if (!map_->isValidPosition(nx, ny)) {
        continue;
      }

      auto targetCandidate = getTargetCandidate(activeUnit, nx, ny);
      if (!targetCandidate) {
        continue;
      }

      // простенькая приоритизация цели - бьем по слабейшему
      if (targetCandidate->getHP() < minHP) {
        minHP = targetCandidate->getHP();
        targetUnitId = targetCandidate->getId();
      }
    }
  }

  if (targetUnitId != 0) {
    return std::make_tuple(true, targetUnitId);
  }

  return std::make_tuple(false, 0);
}

std::shared_ptr<Unit> Simulator::getTargetCandidate(const Unit &activeUnit,
                                                    int x, int y) {
  auto &cell = map_->getCell(x, y);

  if (cell.is_empty()) {
    return nullptr;
  }

  auto targetCandidate = cell.getUnit();

  if (!targetCandidate || targetCandidate->getId() == activeUnit.getId() ||
      !activeUnit.canAttack(*targetCandidate)) {
    return nullptr;
  }

  return targetCandidate;
}

void Simulator::processAction(std::shared_ptr<Unit> &unit, uint32_t targetId) {
#warning correct event should be logged here
  auto damageLevel = unit->action(*units_[targetId]);
  eventLog_.log(currentTick_, UnitAttacked{unit->getId(), targetId, damageLevel,
                                           getUnit(targetId)->getHP()});
}

void Simulator::processMovement(std::shared_ptr<Unit> &unit) {
  if (!unit->isMovable()) {
    return;
  }

  if (!unit->isStartedMarch()) {
    eventLog_.log(currentTick_,
                  MarchStarted{unit->getId(), unit->getX(), unit->getY(),
                               unit->getTargetX(), unit->getTargetY()});
    unit->startMarch();
  }

  unit->march();

  auto nextX = unit->getX();
  auto nextY = unit->getY();

  if (!map_->isValidPosition(nextX, nextY)) {
    throw std::out_of_range("Invalid position for the next point: [" +
                            std::to_string(nextX) + ", " +
                            std::to_string(nextY) + "]");
  }

  auto &currentCell = map_->getCell(unit->getX(), unit->getY());
  auto &nextCell = map_->getCell(nextX, nextY);

  nextCell.setUnit(unit);
  currentCell.removeUnit();

  if (nextX == unit->getTargetX() && nextY == unit->getTargetY()) {
    unit->stopMarch();
    eventLog_.log(currentTick_, MarchEnded{unit->getId(), nextX, nextY});
  } else {
    eventLog_.log(currentTick_, UnitMoved{unit->getId(), nextX, nextY});
  }
}

void Simulator::handleDeadUnit(uint32_t unitId) {
  if (units_.find(unitId) == units_.end()) {
    return;
  }

  auto &currentCell =
      map_->getCell(getUnit(unitId)->getX(), getUnit(unitId)->getY());
  currentCell.removeUnit();

  units_.erase(unitId);

  eventLog_.log(currentTick_, UnitDied{unitId});
}

bool Simulator::hasWinner() {
  if (unitQueue_.size() <= 1) {
    auto unit = getUnit(unitQueue_.front());
    eventLog_.log(currentTick_, UnitWon{unitQueue_.front(), unit->getType()});
    return true;
  }

  return false;
}

bool Simulator::allUnitsAtTargets() {
  for (const auto &[id, unit] : units_) {
    if (unit->getX() != unit->getTargetX() ||
        unit->getY() != unit->getTargetY()) {
      return false;
    }
  }

  eventLog_.log(currentTick_,
                AllTargetsReached{"All units reached their targets"});
  return true;
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
