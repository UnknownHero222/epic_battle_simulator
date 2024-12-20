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
#include <unordered_map>

namespace sw::simulator {

using namespace sw::io;
using namespace sw::core;

class Simulator {
public:
  Simulator() = default;
  ~Simulator() = default;

  void run();

  template <typename TCommand>
  void createMap(std::ostream &stream, const TCommand &command) {
    // static_assert(std::is_same_v<TCommand, sw::io::CreateMap>, "Invalid
    // command type for createMap");

    if (command.width == 0 || command.height == 0) {
      throw std::invalid_argument("Map dimensions must be greater than zero.");
    }

    map_ = std::make_unique<Map>(command.width, command.height);
    stream << "Map created with dimensions: " << command.width << "x"
           << command.height << "\n";
  }

  template <typename TCommand>
  void spawnUnit(std::ostream &stream, const TCommand &command) {
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

    std::shared_ptr<Unit> unit;
    if constexpr (std::is_same_v<TCommand, SpawnSwordsman>) {
      unit = std::make_shared<Swordsman>(command.unitId, command.x, command.y,
                                         command.hp, command.strength);
    } else if constexpr (std::is_same_v<TCommand, SpawnHunter>) {
      unit = std::make_shared<Hunter>(command.unitId, command.x, command.y,
                                      command.hp, command.strength,
                                      command.agility, command.range);
    }

    units_.emplace(command.unitId, unit);
    cell.setUnit(unit);

    // TODO debug here
  }

  const Map &getMap() const {
    if (!map_) {
      throw std::runtime_error("Map is not initialized.");
    }

    return *map_;
  }

  Unit getUnit(uint32_t unitId) const {
    auto it = units_.find(unitId);
    if (it == units_.end()) {
      throw std::runtime_error("Unit with ID " + std::to_string(unitId) +
                               " not found.");
    }
    return it->second;
  }

private:
  std::unique_ptr<Map> map_;
  std::unordered_map<uint32_t, Unit> units_;
};

} // namespace sw::simulator