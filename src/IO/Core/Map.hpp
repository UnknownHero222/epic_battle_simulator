#pragma once

#include "Units/Unit.hpp"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace sw::core {

using CellUnits = std::unordered_map<uint32_t, Unit>;

class Map {
public:
  Map() = default;
  Map(uint32_t width, uint32_t height);

  bool isValidPosition(uint32_t x, uint32_t y) const;

  uint32_t getWidth() const { return width_; }
  uint32_t getHeight() const { return height_; }

  class Cell {
  public:
    bool isEmpty() const {
      if (units_.empty()) {
        return true;
      }

      for (const auto &[id, unit] : units_) {
        if (!unit.isOccupyingCell()) {
          return false;
        }
      }

      return true;
    }

    void setUnit(const Unit &unit) { units_[unit.getId()] = unit; }

    void removeUnit(uint32_t unitId) { units_.erase(unitId); }

    std::vector<uint32_t> getUnitIds() const {
      std::vector<uint32_t> ids;
      ids.reserve(units_.size());

      for (const auto &[id, _] : units_) {
        ids.push_back(id);
      }

      return ids;
    }

  private:
    CellUnits units_;
  };

  Cell &getCell(uint32_t x, uint32_t y) {
    if (!isValidPosition(x, y)) {
      throw std::out_of_range("Invalid position on map");
    }
    return grid_[y][x];
  }

private:
  uint32_t width_;
  uint32_t height_;
  std::vector<std::vector<Cell>> grid_;
};

} // namespace sw::core