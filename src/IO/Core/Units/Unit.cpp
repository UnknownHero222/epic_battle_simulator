#include "Unit.hpp"

namespace sw::core {

void Unit::march() {
  if (x_ < targetX_) {
    ++x_;
  } else if (x_ > targetX_) {
    --x_;
  }

  if (y_ < targetY_) {
    ++y_;
  } else if (y_ > targetY_) {
    --y_;
  }
}

void Unit::setTarget(uint32_t x, uint32_t y) {
  targetX_ = x;
  targetY_ = y;
}

} // namespace sw::core