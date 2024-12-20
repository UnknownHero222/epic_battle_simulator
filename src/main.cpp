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
  core::Map map(0, 0);

  // Code for example...

  std::cout << "Commands:\n";
  io::CommandParser parser;
  parser
      .add<io::CreateMap>(
          [&](auto command) { createMap(std::cout, command, map); })
      .add<io::SpawnSwordsman>(
          [&](auto command) { spawnUnit(std::cout, command, map); })
      .add<io::SpawnHunter>(
          [&](auto command) { spawnUnit(std::cout, command, map); });
  // parser.add<io::CreateMap>([](auto command) { printDebug(std::cout,
  // command); }) 	.add<io::SpawnSwordsman>([](auto command) {
  // printDebug(std::cout, command); }) 	.add<io::SpawnHunter>([](auto
  // command) { printDebug(std::cout, command); }) .add<io::March>([](auto
  // command) { printDebug(std::cout, command); });

  parser.parse(file);

  std::cout << "\n\nEvents:\n";

  EventLog eventLog;

  eventLog.log(1, io::MapCreated{10, 10});
  eventLog.log(1, io::UnitSpawned{1, "Swordsman", 0, 0});
  eventLog.log(1, io::UnitSpawned{2, "Hunter", 9, 0});
  // eventLog.log(1, io::MarchStarted{1, 0, 0, 9, 0});
  // eventLog.log(1, io::MarchStarted{2, 9, 0, 0, 0});
  // eventLog.log(1, io::UnitSpawned{3, "Swordsman", 0, 9});
  // eventLog.log(1, io::MarchStarted{3, 0, 9, 0, 0});

  // eventLog.log(2, io::UnitMoved{1, 1, 0});
  // eventLog.log(2, io::UnitMoved{2, 8, 0});
  // eventLog.log(2, io::UnitMoved{3, 0, 8});

  // eventLog.log(3, io::UnitMoved{1, 2, 0});
  // eventLog.log(3, io::UnitMoved{2, 7, 0});
  // eventLog.log(3, io::UnitMoved{3, 0, 7});

  // eventLog.log(4, io::UnitMoved{1, 3, 0});
  // eventLog.log(4, io::UnitAttacked{2, 1, 5, 0});
  // eventLog.log(4, io::UnitDied{1});
  // eventLog.log(4, io::UnitMoved{3, 0, 6});

  // eventLog.log(5, io::UnitMoved{2, 6, 0});
  // eventLog.log(5, io::UnitMoved{3, 0, 5});

  // eventLog.log(6, io::UnitMoved{2, 5, 0});
  // eventLog.log(6, io::UnitMoved{3, 0, 4});

  // eventLog.log(7, io::UnitAttacked{2, 3, 5, 5});
  // eventLog.log(7, io::UnitMoved{3, 0, 3});

  // eventLog.log(8, io::UnitAttacked{2, 3, 5, 0});
  // eventLog.log(8, io::UnitDied{3});

  return 0;
}
