#include "Core/Map.hpp"
#include "Core/Units/Swordsman.hpp"
#include "Core/Units/Griffon.hpp"
#include <gtest/gtest.h>
#include <memory>

using namespace sw::core;

TEST(MapTest, MapInitialization) {
  Map map(10, 10);
  EXPECT_TRUE(map.isValidPosition(5, 5));
  // Позиция вне карты
  EXPECT_FALSE(map.isValidPosition(10, 10));
}

TEST(MapTest, CellManipulation) {
  Map map(5, 5);
  auto unit = std::make_shared<Swordsman>(1, 0, 0, 100, 10);
  auto &cell = map.getCell(0, 0);

  auto unitId = unit->getId();
  cell.setUnit(*unit);
  EXPECT_FALSE(cell.isEmpty());

  cell.removeUnit(unitId);
  EXPECT_TRUE(cell.isEmpty());
}

TEST(MapTest, FlyingUnitDoesNotOccupyCell) {
  Map map(5, 5);
  auto griffon = std::make_shared<Griffon>(2, 2, 2, 50, 15);
  auto &cell = map.getCell(2, 2);

  EXPECT_TRUE(cell.isEmpty());
}