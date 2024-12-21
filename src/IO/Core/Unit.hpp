#pragma once
#include <string>

namespace sw::core {

enum UnitType { UnitSwordsman, UnitHunter };

class Unit {
public:
  Unit() = default;
  explicit Unit(uint32_t id, UnitType type, uint32_t x, uint32_t y, uint32_t hp,
                uint32_t strength)
      : id_(id), type_(type), x_(x), y_(y), hp_(hp), strength_(strength) {}

  virtual ~Unit() = default;

  void march(uint32_t targetX, uint32_t targetY);
  #warning "Check this again later"
  virtual void attack(Unit &unit){};

  std::string getUnitName() const;

  uint32_t getId() const { return id_; }
  UnitType getType() const { return type_; }
  uint32_t getX() const { return x_; }
  uint32_t getY() const { return y_; }
  uint32_t getHP() const { return hp_; }
  void setHP(uint32_t hp) { hp_ = hp; }
  uint32_t getStrength() const { return strength_; }
  uint32_t getTargetX() const { return targetX_; }
  uint32_t getTargetY() const { return targetY_; }
  void setTarget(uint32_t x, uint32_t y) {
    targetX_ = x;
    targetY_ = y;
  }
  bool isMoveable() const { return is_movable_; }

protected:
  uint32_t id_;
  UnitType type_;
  uint32_t x_;
  uint32_t y_;
  uint32_t hp_;
  uint32_t strength_;
  uint32_t targetX_;
  uint32_t targetY_;
  std::string name_;
  bool is_movable_;
};

} // namespace sw::core
