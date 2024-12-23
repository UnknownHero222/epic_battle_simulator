#include "Unit.hpp"

namespace sw::core {

void Unit::march(uint32_t targetX, uint32_t targetY) {
  if (x_ < targetX) {
    ++x_;
  } else if (x_ > targetX) {
    --x_;
  }

  if (y_ < targetY) {
    ++y_;
  } else if (y_ > targetY) {
    --y_;
  }
}

void Unit::setTarget(uint32_t x, uint32_t y) {
  targetX_ = x;
  targetY_ = y;
}

} // namespace sw::core