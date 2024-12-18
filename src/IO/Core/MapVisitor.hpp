#pragma once

#include "Map.hpp"
#include <stdexcept>
#include <string>
#include <cstdint>

namespace sw::core {

class MapVisitor {
public:
    explicit MapVisitor() : width_(0), height_(0) {}

    template <typename T>
    void visit(const std::string& name, const T& value) {
        if (name == "width") {
            width_ = value;
        } else if (name == "height") {
            height_ = value;
        }
    }

    Map build() {
        if (width_ == 0 || height_ == 0) {
            throw std::runtime_error("Map dimensions must be greater than zero.");
        }

        return Map(width_, height_);
    }

private:
    uint32_t width_;
    uint32_t height_;

    Map map_;
};

}   
