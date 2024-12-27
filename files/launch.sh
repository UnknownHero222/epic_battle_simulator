#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Usage: $0 <scenario_number>"
  echo "Available scenarios:"
  echo "  1 - 1_swordsman_hunter_battle.txt"
  echo "  2 - 2_tower_sw_hun_battle.txt"
  echo "  3 - 3_griffon_hunters_battle.txt"
  echo "  4 - 4_swordsman_healer.txt"
  echo "  5 - 5_all_targets_reached_commands.txt"
  exit 1
fi

SCENARIOS=(
  "1_swordsman_hunter_battle.txt"
  "2_tower_sw_hun_battle.txt"
  "3_griffon_hunters_battle.txt"
  "4_swordsman_healer.txt"
  "5_all_targets_reached_commands.txt"
)

# тут пока просто вручную задам путь к файлам, надо бы как-то автоматизировать
SCENARIO_DIR="commands"
EXECUTABLE="./../build/src/TotalBattleSimulator"

if [[ $1 -lt 1 || $1 -gt ${#SCENARIOS[@]} ]]; then
  echo "Invalid scenario number. Choose between 1 and ${#SCENARIOS[@]}."
  exit 1
fi

SCENARIO_FILE=${SCENARIOS[$(($1 - 1))]}

CMD_FILE="$SCENARIO_DIR/$SCENARIO_FILE"

if [ ! -f "$CMD_FILE" ]; then
  echo "Scenario file '$CMD_FILE' not found!"
  exit 1
fi

echo "=========================================================="
echo "Starting scenario: $SCENARIO_FILE"

$EXECUTABLE "$CMD_FILE"

echo "Scenario completed: $SCENARIO_FILE"
echo "=========================================================="

# Возвращаем код выполнения
exit $?

