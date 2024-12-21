#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace sw::core {

class Unit;

class Map {
public:
  Map() = default;
  Map(uint32_t width, uint32_t height);

  bool isValidPosition(uint32_t x, uint32_t y) const;

  uint32_t getWidth() const { return width_; }
  uint32_t getHeight() const { return height_; }

  class Cell {
  public:
    bool is_empty() const { return !unit_; }

    void setUnit(std::shared_ptr<Unit> unit) { unit_ = std::move(unit); }

    std::shared_ptr<Unit> getUnit() const { return unit_; }

  private:
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