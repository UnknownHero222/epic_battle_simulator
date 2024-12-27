#pragma once
#include "UnitParams.hpp"
#include "Commands/SpawnSwordsman.hpp"
#include "Commands/SpawnSwordsman.hpp"
#include "Commands/SpawnHunter.hpp"
#include "Commands/SpawnTower.hpp"
#include "Commands/SpawnGriffon.hpp"
#include "Commands/SpawnHealer.hpp"

namespace sw::core {

template <typename TCommand>
struct UnitParamsMapper {
    using Params = void;
};

template <>
struct UnitParamsMapper<sw::io::SpawnSwordsman> {
    using Params = SwordsmanParams;
};

template <>
struct UnitParamsMapper<sw::io::SpawnHunter> {
    using Params = HunterParams;
};

template <>
struct UnitParamsMapper<sw::io::SpawnTower> {
    using Params = TowerParams;
};

template <>
struct UnitParamsMapper<sw::io::SpawnGriffon> {
    using Params = GriffonParams;
};

template <>
struct UnitParamsMapper<sw::io::SpawnHealer> {
    using Params = HealerParams;
};

} // namespace sw::core
