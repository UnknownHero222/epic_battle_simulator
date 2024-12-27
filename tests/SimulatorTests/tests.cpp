#include "Core/Map.hpp"
#include "Core/UnitVisitor.hpp"
#include <gtest/gtest.h>
#include <memory>

using namespace sw::core;

// ================ Тесты Swordsman ================
TEST(SwordsmanTest, SwordsmanInitialization) {
  Swordsman unit(1, 0, 0, 100, 10); // id=1, x=0, y=0, hp=100, strength=10
  EXPECT_EQ(unit.getId(), 1);
  EXPECT_EQ(unit.getX(), 0);
  EXPECT_EQ(unit.getY(), 0);
  EXPECT_EQ(unit.getHP(), 100);
}

TEST(SwordsmanTest, SwordsmanMove) {
  Swordsman unit(1, 0, 0, 100, 10); // id=1, x=0, y=0, hp=100, strength=10
  unit.setTarget(1, 1);
  unit.march();
  EXPECT_EQ(unit.getX(), 1);
  EXPECT_EQ(unit.getY(), 1);
}

TEST(SwordsmanTest, SwordsmanAttack) {
  Swordsman attacker(1, 0, 0, 100, 10); // id=1, x=0, y=0, hp=100, strength=10
  Swordsman target(2, 1, 0, 50, 5);     // id=2, x=1, y=0, hp=50, strength=5
  auto actionResult = attacker.action(target);

  EXPECT_EQ(actionResult.type, ActionType::Attack);
  EXPECT_EQ(actionResult.value, 10);
  EXPECT_EQ(target.getHP(), 40);
}

TEST(SwordsmanTest, SwordsmanCannotAttackFlyingUnit) {
  Swordsman swordsman(1, 0, 0, 100, 10); // id=1, x=0, y=0, hp=100, strength=10
  Griffon griffon(2, 1, 1, 50, 15);      //  id=2, x=1, y=1, hp=50, agility=15
  EXPECT_FALSE(swordsman.canAttack(griffon));
}

// ============= Тесты для класса Griffon =================
TEST(GriffonTest, GriffonInitialization) {
  Griffon griffon(2, 3, 3, 50, 15); // id=2, x=3, y=3, hp=50, agility=15
  EXPECT_EQ(griffon.getId(), 2);
  EXPECT_EQ(griffon.getX(), 3);
  EXPECT_EQ(griffon.getY(), 3);
  EXPECT_EQ(griffon.getHP(), 50);
  EXPECT_TRUE(griffon.isFlying());
}

TEST(GriffonTest, GriffonMove) {
  Griffon griffon(2, 0, 0, 50, 15); // id=2, x=0, y=0, hp=50, agility=15
  griffon.setTarget(2, 2);
  griffon.march();
  EXPECT_EQ(griffon.getX(), 2);
  EXPECT_EQ(griffon.getY(), 2);
}

TEST(GriffonTest, GriffonAttack) {
  Griffon griffon(2, 3, 3, 50, 15);  // id=2, x=3, y=3, hp=50, agility=15
  Swordsman target(3, 2, 3, 40, 10); // id=3, x=2, y=3, hp=40, strength=10

  auto actionResult = griffon.action(target);

  EXPECT_EQ(actionResult.type, ActionType::Attack);
  EXPECT_EQ(actionResult.value, 15);
  EXPECT_EQ(target.getHP(), 25);
}

// ================= Тесты для класса Hunter =================
TEST(HunterTest, HunterAttackRange) {
  Hunter hunter(3, 0, 0, 50, 5, 1,
                4); // id=3, x=0, y=0, hp=50, agility=5, strength=1, range=4
  Swordsman target(4, 3, 0, 50, 5); //    id=4, x=3, y=0, hp=50, strength=5

  auto actionResult = hunter.action(target);

  EXPECT_EQ(actionResult.type, ActionType::Attack);
  EXPECT_EQ(actionResult.value, 5);
  EXPECT_EQ(target.getHP(), 45);
}

TEST(HunterTest, HunterCloseRangeAttack) {
  // Ловкость 5, сила 1
  Hunter hunter(3, 0, 0, 50, 5, 1,
                4); // id=3, x=0, y=0, hp=50, agility=5, strength=1, range=4
  Swordsman target(4, 1, 0, 50, 5); // id=4, x=1, y=0, hp=50, strength=5

  auto actionResult = hunter.action(target);

  EXPECT_EQ(actionResult.type, ActionType::Attack);
  EXPECT_EQ(actionResult.value, 1);
  EXPECT_EQ(target.getHP(), 49);
}

// ================= Тесты для класса Healer =================

TEST(HealerTest, HealerMove) {
  Healer healer(1, 0, 0, 100, 2,
                5); // id=1, x=0, y=0, hp=100, range=2, spirit=5
  healer.setTarget(1, 1);
  healer.march();
  EXPECT_EQ(healer.getX(), 1);
  EXPECT_EQ(healer.getY(), 1);
}

TEST(HealerTest, HealDamagedUnit) {
  Healer healer(1, 0, 0, 10, 2, 5); // id=1, x=0, y=0, hp=10, range=2, spirit=5
  Swordsman target(2, 1, 0, 10, 4); // id=2, x=1, y=0, hp=10, strength=4

  target.setHP(6);
  ASSERT_EQ(target.getHP(), 6);

  auto actionResult = healer.action(target);
  EXPECT_EQ(target.getHP(), 10);
}

TEST(HealerTest, HealFullHP) {
  Healer healer(1, 0, 0, 10, 2, 5); // id=1, x=0, y=0, hp=10, range=2, spirit=5
  Swordsman target(2, 1, 0, 10, 4); // id=2, x=1, y=0, hp=10, strength=4

  target.setHP(10);

  auto actionResult = healer.action(target);

  EXPECT_EQ(target.getHP(), target.getMaxHP());
}

// ================= Общие тесты для Юнитов =================

TEST(UnitTest, UnitDeath) {
  Swordsman attacker(1, 0, 0, 100, 10);
  Swordsman target(2, 1, 0, 5, 5);
  auto actionResult = attacker.action(target);
  EXPECT_EQ(target.getHP(), 0);
}

TEST(UnitTest, RemoveDeadUnitFromCell) {
  Map map(5, 5);
  auto unit = std::make_shared<Swordsman>(1, 2, 2, 10, 5);

  auto unitId = unit->getId();
  map.getCell(2, 2).setUnit(*unit);

  unit->setHP(0);
  map.getCell(2, 2).removeUnit(unitId);
  EXPECT_TRUE(map.getCell(2, 2).isEmpty());
}

// ================= Тесты для класса Map =================
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

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
