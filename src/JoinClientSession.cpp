#include "JoinClientSession.h"

#include <iostream>
#include <sstream>
#include <vector>

namespace db {

JoinClientSession::JoinClientSession(ba::ip::tcp::socket sock) : sock_{std::move(sock)} {
}

void JoinClientSession::start() {
  db_conn_id_ = DataBaseWrapper::get_instance().connect();
  self_   = shared_from_this();
  handle_read();
}

void JoinClientSession::handle_read() {
  sock_.async_read_some(ba::buffer(buf_), [this](const boost::system::error_code& ec, std::size_t len) {
    if (!ec) {
      std::string request(buf_.data(), len);
      DataBaseWrapper::get_instance().process(db_conn_id_, request, [this](Reply& reply) {
          std::cout << reply;
          std::ostringstream oss;
          oss << reply;
          write();
        });
      handle_read();
    } else {
      DataBaseWrapper::get_instance().disconnect(db_conn_id_);
      sock_.close();
      self_.reset();
    }
  });
}

void JoinClientSession::write(const std::string& data) {
  std::vector<boost::asio::const_buffer> buf{boost::asio::buffer(data)};
  sock_.async_write_some(buf, [](const boost::system::error_code&, std::size_t) {});
}

} // namespace db.
