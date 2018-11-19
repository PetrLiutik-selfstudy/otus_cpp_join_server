#include "JoinServer.h"
#include "JoinClientSession.h"

namespace bulk {

JoinServer::JoinServer(uint16_t port) :
  acceptor_(service_, ba::ip::tcp::endpoint(ba::ip::tcp::v4(), port)),
  sock_(service_) {
}

JoinServer::~JoinServer() {
}

void JoinServer::handle_accept() {
  acceptor_.async_accept(sock_, [this](const boost::system::error_code& ec) {
    if(!ec) {
      std::make_shared<BulkClientSession>(std::move(sock_))->start();
    }
    handle_accept();
  });
}

void JoinServer::start() {
  ba::signal_set signals(service_, SIGINT, SIGTERM);
  signals.async_wait([this, &signals](const boost::system::error_code&, int) {
    handle_stop();
  });
  handle_accept();
  service_.run();
}

void JoinServer::handle_stop() {
  service_.stop();
}

} // namespace bulk.
