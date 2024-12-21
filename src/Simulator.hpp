#pragma once

#include "IO/Commands/CreateMap.hpp"
#include "IO/Commands/March.hpp"
#include "IO/Commands/SpawnHunter.hpp"
#include "IO/Commands/SpawnSwordsman.hpp"
#include "IO/Core/Map.hpp"
#include "IO/Core/MapVisitor.hpp"
#include "IO/Core/UnitVisitor.hpp"
#include "IO/Events/MapCreated.hpp"
#include "IO/Events/MarchEnded.hpp"
#include "IO/Events/MarchStarted.hpp"
#include "IO/Events/UnitAttacked.hpp"
#include "IO/Events/UnitDied.hpp"
#include "IO/Events/UnitMoved.hpp"
#include "IO/Events/UnitSpawned.hpp"
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
using Coordinates = std::pair<uint32_t, uint32_t>;

class Simulator {
public:
  Simulator() = default;
  ~Simulator() = default;

  void run();
  AffectedUnit isAffectPossible(const Unit &activeUnit);
  Coordinates getNextStep(const Unit &unit);

  template <typename TCommand>
  void createMap(std::ostream &stream, TCommand &command) {
    // static_assert(std::is_same_v<TCommand, sw::io::CreateMap>, "Invalid
    // command type for createMap");

    if (command.width == 0 || command.height == 0) {
      throw std::invalid_argument("Map dimensions must be greater than zero.");
    }

    map_ = std::make_unique<Map>(command.width, command.height);

    eventLog_.log(currentTick_, MapCreated{command.width, command.height});
  }

  template <typename TCommand>
  void spawnUnit(std::ostream &stream, TCommand &command) {
    // static_assert(std::is_same_v<TCommand, sw::io::SpawnSwordsman> ||
    //               std::is_same_v<TCommand, sw::io::SpawnHunter>, "Invalid
    //               command type for spawnUnit");

    if (!map_) {
      throw std::runtime_error("Map is not initialized.");
    }

    if (!map_->isValidPosition(command.x, command.y)) {
      throw std::out_of_range("Invalid spawn position.");
    }

    auto &cell = map_->getCell(command.x, command.y);
    if (!cell.is_empty()) {
      throw std::runtime_error("Cell is already occupied.");
    }

    Unit unit;
    if constexpr (std::is_same_v<TCommand, SpawnSwordsman>) {
      unit = Swordsman(command.unitId, command.x, command.y, command.hp,
                       command.strength);
    } else if constexpr (std::is_same_v<TCommand, SpawnHunter>) {
      unit = Hunter(command.unitId, command.x, command.y, command.hp,
                    command.agility, command.strength, command.range);
    }

    units_.emplace(command.unitId, unit);
    cell.setUnit(std::make_shared<Unit>(unit));

    unitQueue_.push(command.unitId);

    eventLog_.log(currentTick_, UnitSpawned{command.unitId, unit.getUnitName(),
                                            command.x, command.y});
  }

  template <typename TCommand>
  void marchUnit(std::ostream &stream, TCommand &command) {
    auto unit = getUnit(command.unitId);

    if (!map_->isValidPosition(command.targetX, command.targetY)) {
      throw std::out_of_range("Invalid target position.");
    }
    
    if (units_.find(command.unitId) != units_.end()) {
      auto &unit = units_.at(command.unitId);
      unit.setTarget(command.targetX, command.targetY);
    } else {
      throw std::runtime_error("Unit ID " + std::to_string(command.unitId) +
                               " not found!");
    }
  }

  const Map &getMap() const;
  Unit getUnit(uint32_t unitId) const;

private:
  std::unique_ptr<Map> map_;
  std::unordered_map<uint32_t, Unit> units_;
  EventLog eventLog_;

  uint32_t currentTick_{1};
  std::queue<uint32_t> unitQueue_;
};

} // namespace sw::simulator