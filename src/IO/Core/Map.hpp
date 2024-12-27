#pragma once

#include "Units/Unit.hpp"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_set>
#include <vector>

namespace sw::core {

using CellUnitsIds = std::unordered_set<uint32_t>;

class Map {
public:
  Map() = default;
  Map(uint32_t width, uint32_t height);

  bool isValidPosition(uint32_t x, uint32_t y) const;

  uint32_t getWidth() const { return width_; }
  uint32_t getHeight() const { return height_; }

  class Cell {
  public:
    bool is_empty() const { return units_ids_.empty(); }

    void setUnit(uint32_t unitId) { units_ids_.insert(unitId); }

    void removeUnit(uint32_t unitId) { units_ids_.erase(unitId); }

    CellUnitsIds getUnitIds() const { return units_ids_; }

  private:
    CellUnitsIds units_ids_;
    std::shared_ptr<Unit> unit_;
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