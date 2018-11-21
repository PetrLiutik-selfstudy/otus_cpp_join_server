#include "ver.h"
#include "JoinServer.h"

#include <iostream>


int main(int argc, char const *argv[]) {
  std::cout << "join_server version: "
            << ver_major() << "."
            << ver_minor() << "."
            << ver_patch() << std::endl;

  if(argc != 2) {
    std::cerr << "Wrong number of arguments (expected 1). Usage: join_server <port>. \n" << std::endl;
    return EXIT_FAILURE;
  }

  auto port = std::strtoll(argv[1], nullptr, 0);
  if((port <= 0) || (port > 65535)) {
    std::cerr << "Port must be an integer between 0 and 65535.\n";
    return EXIT_FAILURE;
  }

  // Создание и запуск сервера.
  try {
    db::JoinServer join_server{static_cast<uint16_t>(port)};
    join_server.start();
  } catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
