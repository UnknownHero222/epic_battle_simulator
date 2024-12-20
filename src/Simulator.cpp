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

      // is it possible to attack now
      if (true) {
        // attack
      } else {
        // move
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
