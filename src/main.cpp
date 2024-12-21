#include "IO/System/EventLog.hpp"
#include "Simulator.hpp"

#include <fstream>
#include <iostream>

using namespace sw;
using namespace sw::core;
using namespace sw::io;
using namespace sw::simulator;

// TODO вынести потом в собственный класс
template <typename TCommand>
void createMap(std::ostream &stream, TCommand &command, sw::core::Map &map) {
  static_assert(std::is_same_v<TCommand, sw::io::CreateMap>,
                "Invalid command type for createMap");

  sw::printDebug(stream, command);
  sw::core::MapVisitor visitor;
  command.visit(visitor);
  map = visitor.build();
}

template <typename TCommand>
void spawnUnit(std::ostream &stream, TCommand &command, sw::core::Map &map) {
  //   static_assert(std::is_same_v<TCommand, sw::io::SpawnSwordsman> ||
  //                 std::is_same_v<TCommand, sw::io::SpawnHunter>, "Invalid
  //                 command type for spawnUnit");

  if (!map.isValidPosition(command.x, command.y)) {
    throw std::out_of_range("Invalid spawn position.");
  }

  auto &cell = map.getCell(command.x, command.y);
  if (!cell.is_empty()) {
    throw std::runtime_error("Cell is already occupied.");
  }

  core::UnitVisitor visitor(command.Name);
  command.visit(visitor);
  std::shared_ptr<core::Unit> unit = visitor.build();

  cell.setUnit(unit);

  printDebug(stream, command);
}

int main(int argc, char **argv) {
  using namespace sw;

  if (argc != 2) {
    throw std::runtime_error(
        "Error: No file specified in command line argument");
  }

  std::ifstream file(argv[1]);
  if (!file) {
    throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
  }

  // Example for the brutal debugging
  Simulator simulator;

  CommandParser parser;
  parser
      .add<CreateMap>(
          [&](auto command) { simulator.createMap(std::cout, command); })
      .add<SpawnSwordsman>(
          [&](auto command) { simulator.spawnUnit(std::cout, command); })
      .add<SpawnHunter>(
          [&](auto command) { simulator.spawnUnit(std::cout, command); })
      .add<March>(
          [&](auto command) { simulator.marchUnit(std::cout, command); });

  parser.parse(file);

  std::cout << "\n\nEvents:\n";

  simulator.run();

  return 0;
}
