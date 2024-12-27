#pragma once

#include "helpers/ActionResult.hpp"
#include <string>
#include <unordered_set>

namespace sw::core {

using Coordinates = std::pair<uint32_t, uint32_t>;

class Unit {
public:
  Unit() = default;
  explicit Unit(uint32_t id, const std::string &type, uint32_t x, uint32_t y,
                uint32_t hp)
      : id_(id), type_(type), x_(x), y_(y), hp_(hp), maxHP_(hp) {}

  virtual ~Unit() = default;

  virtual void march();
  virtual ActionResult action(Unit &targetUnit) { return {}; };
  virtual uint32_t getAffectRange() const { return 0; };
  virtual bool canAttack(const Unit &targetUnit) const { return true; };
  virtual bool isAttackable() const { return isAttackable_; }

  uint32_t getId() const { return id_; }
  std::string getType() const { return type_; }
  uint32_t getX() const { return x_; }
  uint32_t getY() const { return y_; }
  uint32_t getHP() const { return hp_; }
  uint32_t getMaxHP() const { return maxHP_; }
  void setHP(uint32_t hp) { hp_ = hp; }
  uint32_t getTargetX() const { return targetX_; }
  uint32_t getTargetY() const { return targetY_; }

  void setTarget(uint32_t x, uint32_t y);
  bool isMovable() const { return isMovable_; }
  bool isFlying() const { return isFlying_; }
  bool isStartedMarch() const { return isStartedMarch_; }
  void startMarch() { isStartedMarch_ = true; }
  void stopMarch() { isStartedMarch_ = false; }

protected:
  uint32_t id_;
  std::string type_;
  uint32_t x_;
  uint32_t y_;
  uint32_t hp_;
  uint32_t maxHP_;
  uint32_t targetX_;
  uint32_t targetY_;
  bool isAttackable_{true};
  bool isStartedMarch_{false};
  bool isMovable_{true};
  bool isFlying_{false};

  std::unordered_set<uint32_t> alliedUnits_;
};

} // namespace sw::core
