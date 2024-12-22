#include "IO/System/EventLog.hpp"
#include "Simulator.hpp"

#include <fstream>
#include <iostream>

using namespace sw;
using namespace sw::core;
using namespace sw::io;
using namespace sw::simulator;

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
      .add<SpawnTower>(
          [&](auto command) { simulator.spawnUnit(std::cout, command); })
      .add<March>(
          [&](auto command) { simulator.marchUnit(std::cout, command); });

  parser.parse(file);

  std::cout << "\n\nEvents:\n";

  simulator.run();

  return 0;
}
