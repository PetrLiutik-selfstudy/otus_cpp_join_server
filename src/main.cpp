#include "ver.h"
#include "JoinServer.h"

#include "DataBaseWrapper.h"

#include <iostream>


int main(int argc, char const *argv[]) {
  std::cout << "join_server version: "
            << ver_major() << "."
            << ver_minor() << "."
            << ver_patch() << std::endl;

//  if(argc != 2) {
//    std::cerr << "Wrong number of arguments (expected 1). Usage: join_server <port>. \n" << std::endl;
//    return EXIT_FAILURE;
//  }

//  auto port = std::strtoll(argv[1], nullptr, 0);
//  if((port <= 0) || (port > 65535)) {
//    std::cerr << "Port must be an integer between 0 and 65535.\n";
//    return EXIT_FAILURE;
//  }

//  // Создание и запуск сервера.
//  try {
//    bulk::JoinServer join_server{static_cast<uint16_t>(port)};
//    join_server.start();
//  } catch(std::exception& e) {
//    std::cerr << e.what() << std::endl;
//  }

  auto& db_proxy = db::DataBaseWrapper::get_instance();

  auto conn_id = db_proxy.connect();

  auto on_reply = [](db::Reply& reply) {
    std::cout << reply;
  };

  db_proxy.process(conn_id, "CREATE A\n", on_reply);

  db_proxy.process(conn_id, "CREATE A\n", on_reply);
  db_proxy.process(conn_id, "CREATE B\n", on_reply);

  db_proxy.process(conn_id, "INSERT A 1 aaa\n", on_reply);
  db_proxy.process(conn_id, "INSERT B 1 bbb\n", on_reply);
  db_proxy.process(conn_id, "INSERT A 2 xxx\n", on_reply);
  db_proxy.process(conn_id, "INSERT B 3 yyy\n", on_reply);

  db_proxy.process(conn_id, "INTERSECTION A B\n", on_reply);
  db_proxy.process(conn_id, "SYMMETRIC_DIFFERENCE A B\n", on_reply);

  db_proxy.disconnect(conn_id);

  return 0;
}
