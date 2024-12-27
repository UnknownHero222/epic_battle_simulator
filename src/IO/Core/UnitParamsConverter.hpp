#pragma once
#include "Core/UnitParamsMapper.hpp"

namespace sw::core {

// Базовый шаблон конвертера
template <typename TCommand>
typename UnitParamsMapper<TCommand>::Params convertToParams(const TCommand& command) {
    static_assert(!std::is_same_v<typename UnitParamsMapper<TCommand>::Params, void>,
                  "Unsupported command type!");
}

template <>
inline SwordsmanParams convertToParams<sw::io::SpawnSwordsman>(
    const io::SpawnSwordsman& command) {
    return SwordsmanParams{command.unitId, command.x, command.y, command.hp, command.strength};
}

template <>
inline HunterParams convertToParams<sw::io::SpawnHunter>(
    const sw::io::SpawnHunter& command) {
    return HunterParams{command.unitId, command.x, command.y, command.hp,
                        command.agility, command.strength, command.range};
}

template <>
inline TowerParams convertToParams<sw::io::SpawnTower>(
    const sw::io::SpawnTower& command) {
    return TowerParams{command.unitId, command.x, command.y, command.hp,
                       command.power, command.range};
}

template <>
inline GriffonParams convertToParams<sw::io::SpawnGriffon>(
    const sw::io::SpawnGriffon& command) {
    return GriffonParams{command.unitId, command.x, command.y, command.hp, command.agility};
}

template <>
inline HealerParams convertToParams<sw::io::SpawnHealer>(
    const sw::io::SpawnHealer& command) {
    return HealerParams{command.unitId, command.x, command.y, command.hp,
                        command.range, command.spirit};
}

} // namespace sw::core
