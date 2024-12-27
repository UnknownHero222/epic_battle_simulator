#pragma once

#include "Core/Units/Unit.hpp"
#include "Core/Units/Swordsman.hpp"
#include "Core/Units/Hunter.hpp"
#include "Core/Units/Tower.hpp"
#include "Core/Units/Griffon.hpp"
#include "Core/Units/Healer.hpp"
#include "Core/UnitParams.hpp"

#include <memory>
#include <stdexcept>

namespace sw::core {

class UnitFactory {
public:
    virtual ~UnitFactory() = default;
    virtual std::shared_ptr<Unit> createUnit(const UnitParams &params) const = 0;
};

// ======================== Swordman ========================
class SwordsmanFactory : public UnitFactory {
public:
    std::shared_ptr<Unit> createUnit(const UnitParams& params) const override {
        auto& p = dynamic_cast<const SwordsmanParams&>(params);
        return std::make_shared<Swordsman>(p.unitId, p.x, p.y, p.hp, p.strength);
    }
};

//======================== Hunter ========================
class HunterFactory : public UnitFactory {
public:
    std::shared_ptr<Unit> createUnit(const UnitParams& params) const override {
        auto& p = dynamic_cast<const HunterParams&>(params);
        return std::make_shared<Hunter>(p.unitId, p.x, p.y, p.hp, p.agility, p.strength, p.range);
    }
};

// ======================== Tower ========================
class TowerFactory : public UnitFactory {
public:
    std::shared_ptr<Unit> createUnit(const UnitParams& params) const override {
        auto& p = dynamic_cast<const TowerParams&>(params);
        return std::make_shared<Tower>(p.unitId, p.x, p.y, p.hp, p.power, p.range);
    }
};

// ======================== Griffon ========================
class GriffonFactory : public UnitFactory {
public:
    std::shared_ptr<Unit> createUnit(const UnitParams& params) const override {
        auto& p = dynamic_cast<const GriffonParams&>(params);
        return std::make_shared<Griffon>(p.unitId, p.x, p.y, p.hp, p.agility);
    }
};

// ======================== Healer ========================
class HealerFactory : public UnitFactory {
public:
    std::shared_ptr<Unit> createUnit(const UnitParams& params) const override {
        auto& p = dynamic_cast<const HealerParams&>(params);
        return std::make_shared<Healer>(p.unitId, p.x, p.y, p.hp, p.range, p.spirit);
    }
};



} // namespace sw::core
