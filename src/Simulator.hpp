#pragma once

#include "IO/Commands/CreateMap.hpp"
#include "IO/Commands/March.hpp"
#include "IO/Commands/SpawnGriffon.hpp"
#include "IO/Commands/SpawnHealer.hpp"
#include "IO/Commands/SpawnHunter.hpp"
#include "IO/Commands/SpawnSwordsman.hpp"
#include "IO/Commands/SpawnTower.hpp"
#include "IO/Core/Map.hpp"
#include "IO/Core/MapVisitor.hpp"
#include "IO/Core/UnitVisitor.hpp"
#include "IO/Core/UnitParamsConverter.hpp"
#include "IO/Core/UnitFactory.hpp"
#include "IO/Events/AllTargetsReached.hpp"
#include "IO/Events/MapCreated.hpp"
#include "IO/Events/MarchEnded.hpp"
#include "IO/Events/MarchStarted.hpp"
#include "IO/Events/UnitAttacked.hpp"
#include "IO/Events/UnitDied.hpp"
#include "IO/Events/UnitHealed.hpp"
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
  Simulator();
  ~Simulator() = default;

  void run();

  template <typename TCommand>
  void createMap(std::ostream &stream, TCommand &command) try {
    if (command.width == 0 || command.height == 0) {
      throw std::invalid_argument("Map dimensions must be greater than zero.");
    }

    map_ = std::make_unique<Map>(command.width, command.height);

    eventLog_.log(currentTick_, MapCreated{command.width, command.height});
  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
  }

  template <typename TCommand>
  void spawnUnit(std::ostream &stream, TCommand &command) try {
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

    auto it = factories_.find(command.Name);
    if (it == factories_.end()) {
        throw std::runtime_error("Unsupported unit type:");
    }

    auto unit_params = convertToParams(command);

    std::shared_ptr<Unit> unit = it->second->createUnit(unit_params);

    auto &cell = map_->getCell(command.x, command.y);
    if (!cell.isEmpty() && !unit->isOccupyingCell()) {
      throw std::runtime_error("Cell is already occupied.");
    }

    units_.emplace(command.unitId, unit);
    cell.setUnit(*unit);
    unitQueue_.push(command.unitId);

    eventLog_.log(currentTick_, UnitSpawned{command.unitId, unit->getType(),
                                            command.x, command.y});
  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
  }

  template <typename TCommand>
  void marchUnit(std::ostream &stream, TCommand &command) try {
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
  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
  }

private:
  void initUnitsFactories();
  const Map &getMap() const;

  void handleUnitAction(uint32_t unitId);
  AffectedUnit isAffectPossible(const Unit &activeUnit);
  std::shared_ptr<Unit> getTargetCandidate(const Unit &activeUnit, int x,
                                           int y);

  void processAction(std::shared_ptr<Unit> unit, uint32_t targetId);

  void processMovement(std::shared_ptr<Unit> unit);

  void handleDeadUnit(uint32_t unitId);
  bool hasWinner();

  bool allUnitsAtTargets();

  std::shared_ptr<Unit> getUnit(uint32_t unitId);

private:
  std::unique_ptr<Map> map_;
  std::unordered_map<uint32_t, std::shared_ptr<Unit>> units_;
  std::unordered_map<std::string, std::unique_ptr<UnitFactory>> factories_;

  EventLog eventLog_;

  uint32_t currentTick_{1};
  std::queue<uint32_t> unitQueue_;
};

} // namespace sw::simulator