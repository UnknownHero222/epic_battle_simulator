#pragma once
#include <string>
#include <unordered_set>

namespace sw::core {

class Unit {
public:
  Unit() = default;
  explicit Unit(uint32_t id, const std::string &type, uint32_t x, uint32_t y,
                uint32_t hp)
      : id_(id), type_(type), x_(x), y_(y), hp_(hp) {}

  virtual ~Unit() = default;

  virtual void march(uint32_t targetX, uint32_t targetY);
#warning "Check this again later"
  virtual uint32_t attack(Unit &targetUnit){};
  virtual uint32_t getAffectRange() const {};
  virtual bool canAttack(const Unit &targetUnit) const { return true; };

  uint32_t getId() const { return id_; }
  std::string getType() const { return type_; }
  uint32_t getX() const { return x_; }
  uint32_t getY() const { return y_; }
  uint32_t getHP() const { return hp_; }
  void setHP(uint32_t hp) { hp_ = hp; }
  uint32_t getTargetX() const { return targetX_; }
  uint32_t getTargetY() const { return targetY_; }
  void setTarget(uint32_t x, uint32_t y);
  bool isMovable() const { return isMovable_; }
  bool isFlying() const { return isFlying_; }

protected:
  uint32_t id_;
  std::string type_;
  uint32_t x_;
  uint32_t y_;
  uint32_t hp_;
  uint32_t targetX_;
  uint32_t targetY_;
  std::string name_;
  bool isMovable_{true};
  bool isFlying_{false};

  std::unordered_set<uint32_t> alliedUnits_;
};

} // namespace sw::core
