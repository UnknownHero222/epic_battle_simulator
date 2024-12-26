#pragma once

#include "IO/Commands/CreateMap.hpp"
#include "IO/Commands/March.hpp"
#include "IO/Commands/SpawnGriffon.hpp"
#include "IO/Commands/SpawnHunter.hpp"
#include "IO/Commands/SpawnSwordsman.hpp"
#include "IO/Commands/SpawnTower.hpp"
#include "IO/Core/Map.hpp"
#include "IO/Core/MapVisitor.hpp"
#include "IO/Core/UnitVisitor.hpp"
#include "IO/Events/AllTargetsReached.hpp"
#include "IO/Events/MapCreated.hpp"
#include "IO/Events/MarchEnded.hpp"
#include "IO/Events/MarchStarted.hpp"
#include "IO/Events/UnitAttacked.hpp"
#include "IO/Events/UnitDied.hpp"
#include "IO/Events/UnitMoved.hpp"
#include "IO/Events/UnitSpawned.hpp"
#include "IO/Events/UnitWon.hpp"
#include "IO/System/CommandParser.hpp"
#include "IO/System/EventLog.hpp"
#include "IO/System/PrintDebug.hpp"
#include <memory>
#include <ostream>
#include <queue>
#include <tuple>
#include <unordered_map>

namespace sw::simulator {

using namespace sw::io;
using namespace sw::core;

using AffectedUnit = std::tuple<bool, uint32_t>;
class Simulator {
public:
  Simulator() = default;
  ~Simulator() = default;

  void run();

  template <typename TCommand>
  void createMap(std::ostream &stream, TCommand &command) {
    if (command.width == 0 || command.height == 0) {
      throw std::invalid_argument("Map dimensions must be greater than zero.");
    }

    map_ = std::make_unique<Map>(command.width, command.height);

    eventLog_.log(currentTick_, MapCreated{command.width, command.height});
  }

  template <typename TCommand>
  void spawnUnit(std::ostream &stream, TCommand &command) {
    if (!map_) {
      throw std::runtime_error("Map is not initialized.");
    }

    if (!map_->isValidPosition(command.x, command.y)) {
      throw std::out_of_range("Invalid spawn position.");
    }

    if (units_.find(command.unitId) != units_.end()) {
      throw std::runtime_error("Unit ID " + std::to_string(command.unitId) +
                               " already exists.");
    }

    auto &cell = map_->getCell(command.x, command.y);
    if (!cell.is_empty()) {
      throw std::runtime_error("Cell is already occupied.");
    }

    std::shared_ptr<Unit> unit{nullptr};
    if constexpr (std::is_same_v<TCommand, SpawnSwordsman>) {
      unit = std::make_shared<Swordsman>(command.unitId, command.x, command.y,
                                         command.hp, command.strength);
    } else if constexpr (std::is_same_v<TCommand, SpawnHunter>) {
      unit = std::make_shared<Hunter>(command.unitId, command.x, command.y,
                                      command.hp, command.agility,
                                      command.strength, command.range);
    } else if constexpr (std::is_same_v<TCommand, SpawnTower>) {
      unit = std::make_shared<Tower>(command.unitId, command.x, command.y,
                                     command.hp, command.power, command.range);
    } else if constexpr (std::is_same_v<TCommand, SpawnGriffon>) {
      unit = std::make_shared<Griffon>(command.unitId, command.x, command.y,
                                       command.hp, command.agility);
    } else {
      throw std::runtime_error("Unsupported unit type");
    }

    units_.emplace(command.unitId, unit);
    cell.setUnit(unit);

    unitQueue_.push(command.unitId);

    eventLog_.log(currentTick_, UnitSpawned{command.unitId, unit->getType(),
                                            command.x, command.y});
  }

  template <typename TCommand>
  void marchUnit(std::ostream &stream, TCommand &command) {
    auto unit = getUnit(command.unitId);

    if (!map_->isValidPosition(command.targetX, command.targetY)) {
      throw std::runtime_error("Invalid target position.");
    }

    if (units_.find(command.unitId) != units_.end()) {
      auto &unit = units_.at(command.unitId);
      unit->setTarget(command.targetX, command.targetY);
    } else {
      throw std::runtime_error("Unit ID " + std::to_string(command.unitId) +
                               " not found!");
    }
  }

private:
  const Map &getMap() const;

  void handleUnitAction(uint32_t unitId);
  AffectedUnit isAffectPossible(const Unit &activeUnit);
  std::shared_ptr<Unit> getTargetCandidate(const Unit &activeUnit, int x,
                                           int y);

  void processAttack(std::shared_ptr<Unit> &unit, uint32_t targetId);

  void processMovement(std::shared_ptr<Unit> &unit);

  void handleDeadUnit(uint32_t unitId);
  bool hasWinner();

  bool allUnitsAtTargets();

  std::shared_ptr<Unit> getUnit(uint32_t unitId);

private:
  std::unique_ptr<Map> map_;
  std::unordered_map<uint32_t, std::shared_ptr<Unit>> units_;
  EventLog eventLog_;

  uint32_t currentTick_{1};
  std::queue<uint32_t> unitQueue_;
};

} // namespace sw::simulator