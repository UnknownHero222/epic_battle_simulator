#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Usage: $0 <scenario_number>"
  echo "Available scenarios:"
  echo "  1 - swordsman_vs_swordsman.txt"
  echo "  2 - swordsman_vs_hunter.txt"
  echo "  3 - swordsman_hunter_tower.txt"
  echo "  4 - tower_vs_griffon.txt"
  echo "  5 - all_targets_reached_commands.txt"
  echo "  6 - all_units_battle.txt.txt"
  exit 1
fi

SCENARIOS=(
  "swordsman_vs_swordsman.txt"
  "swordsman_vs_hunter.txt"
  "swordsman_hunter_tower.txt"
  "tower_vs_griffon.txt"
  "all_targets_reached_commands.txt"
  "all_units_battle.txt"
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

