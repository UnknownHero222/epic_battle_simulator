#include <gtest/gtest.h>
#include "Core/UnitVisitor.hpp"
#include "Core/Map.hpp"
#include <memory>

using namespace sw::core;

// ================ Тесты Swordsman ================
TEST(UnitTest, SwordsmanInitialization) {
    Swordsman unit(1, 0, 0, 100, 10);
    EXPECT_EQ(unit.getId(), 1);
    EXPECT_EQ(unit.getX(), 0);
    EXPECT_EQ(unit.getY(), 0);
    EXPECT_EQ(unit.getHP(), 100);
}

TEST(UnitTest, SwordsmanMove) {
    Swordsman unit(1, 0, 0, 100, 10);
    unit.march(1, 1);
    EXPECT_EQ(unit.getX(), 1);
    EXPECT_EQ(unit.getY(), 1);
}

TEST(UnitTest, SwordsmanAttack) {
    Swordsman attacker(1, 0, 0, 100, 10);
    Swordsman target(2, 1, 0, 50, 5);
    uint32_t damage = attacker.attack(target);
    EXPECT_EQ(damage, 10);
    EXPECT_EQ(target.getHP(), 40);
}

TEST(UnitTest, SwordsmanCannotAttackFlyingUnit) {
    Swordsman swordsman(1, 0, 0, 100, 10);
    Griffon griffon(2, 1, 1, 50, 15);
    EXPECT_FALSE(swordsman.canAttack(griffon));
}

// ============= Тесты для класса Griffon =================
TEST(UnitTest, GriffonInitialization) {
    Griffon griffon(2, 3, 3, 50, 15);
    EXPECT_EQ(griffon.getId(), 2);
    EXPECT_EQ(griffon.getX(), 3);
    EXPECT_EQ(griffon.getY(), 3);
    EXPECT_EQ(griffon.getHP(), 50);
    EXPECT_TRUE(griffon.isFlying());
}

TEST(UnitTest, GriffonMove) {
    Griffon griffon(2, 3, 3, 50, 15);
    griffon.march(5, 5);
    EXPECT_EQ(griffon.getX(), 5);
    EXPECT_EQ(griffon.getY(), 5);
}

TEST(UnitTest, GriffonAttack) {
    Griffon griffon(2, 3, 3, 50, 15);
    Swordsman target(3, 2, 3, 40, 10);
    uint32_t damage = griffon.attack(target);
    EXPECT_EQ(damage, 15);
    EXPECT_EQ(target.getHP(), 25);
}

// ================= Тесты для класса Hunter =================
TEST(UnitTest, HunterAttackRange) {
    // Ловкость 5, сила 1, радиус 4
    Hunter hunter(3, 0, 0, 50, 5, 1, 4); 
    Swordsman target(4, 3, 0, 50, 5);
    uint32_t damage = hunter.attack(target);
    EXPECT_EQ(damage, 5);
    EXPECT_EQ(target.getHP(), 45);
}

TEST(UnitTest, HunterCloseRangeAttack) {
    // Ловкость 5, сила 1
    Hunter hunter(3, 0, 0, 50, 5, 1, 4); 
    Swordsman target(4, 1, 0, 50, 5);
    uint32_t damage = hunter.attack(target);
    EXPECT_EQ(damage, 1);
    EXPECT_EQ(target.getHP(), 49);
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
    auto& cell = map.getCell(0, 0);

    cell.setUnit(unit);
    EXPECT_FALSE(cell.is_empty());

    cell.removeUnit();
    EXPECT_TRUE(cell.is_empty());
}

TEST(MapTest, FlyingUnitDoesNotOccupyCell) {
    Map map(5, 5);
    auto griffon = std::make_shared<Griffon>(2, 2, 2, 50, 15);
    auto& cell = map.getCell(2, 2);

    EXPECT_TRUE(cell.is_empty());
}

TEST(UnitTest, UnitDeath) {
    Swordsman attacker(1, 0, 0, 100, 10);
    Swordsman target(2, 1, 0, 5, 5);
    attacker.attack(target);
    EXPECT_EQ(target.getHP(), 0);
}

TEST(UnitTest, RemoveDeadUnitFromCell) {
    Map map(5, 5);
    auto unit = std::make_shared<Swordsman>(1, 2, 2, 10, 5);
    map.getCell(2, 2).setUnit(unit);

    unit->setHP(0);
    map.getCell(2, 2).removeUnit();
    EXPECT_TRUE(map.getCell(2, 2).is_empty());
}

// Главная функция для запуска всех тестов
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
