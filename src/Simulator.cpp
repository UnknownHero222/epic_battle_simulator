#include "Simulator.hpp"
#include <iostream>
#include <limits>
#include <stdexcept>

namespace sw::simulator {

using namespace sw::core;
using namespace sw::io;

Simulator::Simulator() {
  initUnitsFactories();
}

void Simulator::initUnitsFactories() {
  factories_[SpawnSwordsman::Name] = std::make_unique<SwordsmanFactory>();
  factories_[SpawnHunter::Name] = std::make_unique<HunterFactory>();
  factories_[SpawnTower::Name] = std::make_unique<TowerFactory>();
  factories_[SpawnGriffon::Name] = std::make_unique<GriffonFactory>();
  factories_[SpawnHealer::Name] = std::make_unique<HealerFactory>();
}

void Simulator::run() try {
  if (!map_) {
    throw std::runtime_error("Map is not initialized.");
  }

  while (!unitQueue_.empty()) {
    if (hasWinner() || allUnitsAtTargets()) {
      break;
    }

    for (size_t i = 0; i < unitQueue_.size(); ++i) {
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

    currentTick_++;
  }
} catch (const std::exception &ex) {
  std::cerr << "Error: " << ex.what() << std::endl;
  return;
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

  if (cell.isEmpty()) {
    return nullptr;
  }

  auto targetCandidates = cell.getUnitIds();

  for (auto unitId : targetCandidates) {
    if (unitId == activeUnit.getId()) {
      continue;
    }

    auto targetCandidate = getUnit(unitId);
    if (activeUnit.canAttack(*targetCandidate)) {
      return targetCandidate;
    }
  }

  return nullptr;
}

void Simulator::processAction(std::shared_ptr<Unit> unit, uint32_t targetId) {
  auto actionResult = unit->action(*units_[targetId]);

  switch (actionResult.type) {
  case ActionType::Attack:
    eventLog_.log(currentTick_,
                  UnitAttacked{unit->getId(), targetId, actionResult.value,
                               units_[targetId]->getHP()});
    break;
  case ActionType::Heal:
    eventLog_.log(currentTick_,
                  UnitHealed{unit->getId(), targetId, actionResult.value,
                             units_[targetId]->getHP()});
    break;
  default:
    break;
  }
}

void Simulator::processMovement(std::shared_ptr<Unit> unit) {
  if (!unit->isMovable()) {
    return;
  }

  if (!unit->isStartedMarch()) {
    eventLog_.log(currentTick_,
                  MarchStarted{unit->getId(), unit->getX(), unit->getY(),
                               unit->getTargetX(), unit->getTargetY()});
    unit->startMarch();
  }

  auto &currentCell = map_->getCell(unit->getX(), unit->getY());

  unit->march();

  auto nextX = unit->getX();
  auto nextY = unit->getY();

  auto &nextCell = map_->getCell(nextX, nextY);

  auto currentUnitId = unit->getId();

  nextCell.setUnit(*unit);
  currentCell.removeUnit(currentUnitId);

  if (nextX == unit->getTargetX() && nextY == unit->getTargetY()) {
    unit->stopMarch();
    eventLog_.log(currentTick_, MarchEnded{currentUnitId, nextX, nextY});
  } else {
    eventLog_.log(currentTick_, UnitMoved{currentUnitId, nextX, nextY});
  }
}

void Simulator::handleDeadUnit(uint32_t unitId) {
  if (units_.find(unitId) == units_.end()) {
    return;
  }

  auto unit = getUnit(unitId);
  auto &currentCell =
      map_->getCell(unit->getX(), unit->getY());
  
  currentCell.removeUnit(unitId);

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
