#include "Map.hpp"

using namespace sw::core;

Map::Map(uint32_t width, uint32_t height)
        : width_(width), height_(height), grid_(height, std::vector<Cell>(width)) {}

bool Map::isValidPosition(uint32_t x, uint32_t y) const {
        return x < width_ && y < height_;
}

