#include "DataBaseWrapper.h"

#include <algorithm>
#include <iostream>

namespace db {

DataBaseWrapper& DataBaseWrapper::get_instance() {
  static DataBaseWrapper instance{};
  return instance;
}

conn_id_t DataBaseWrapper::connect() {
  auto context = std::make_shared<ConnectionContext>(db_);
  auto conn_id = reinterpret_cast<conn_id_t>(context.get());

  std::unique_lock<std::mutex> lock(contexts_mutex_);
  conns_.emplace(std::make_pair(conn_id, context));

  return conn_id;
}

void DataBaseWrapper::disconnect(const conn_id_t& conn_id) {
  std::unique_lock<std::mutex> lock(contexts_mutex_);
  auto it = conns_.find(conn_id);
  if(it != conns_.end())
    conns_.erase(it);
}

void DataBaseWrapper::process(const conn_id_t& conn_id, const std::string& request,
                              std::function<void(Reply&)> on_reply) {
  std::cout << request;
  std::unique_lock<std::mutex> lock(contexts_mutex_);
  auto it = conns_.find(conn_id);
  if(it != conns_.end())
    it->second->process(request, on_reply);
}

} // namespace db.
