#include "DataBase.h"

#include <algorithm>
#include <iterator>

namespace db {

std::ostream& operator <<(std::ostream& os, const Reply& reply) {
  if(reply.result_)
    os << reply.value_ << "OK\n";
  else
    os << "ERR " + reply.value_ << "\n";
  return os;
}

Reply DataBase::create(const req_params_t& req_params) {
  std::unique_lock<std::mutex> lock(storage_guard_);

  if(req_params.size() != 1)
    return Reply{false, "wrong number of arguments"};

  auto tab_name = req_params[0];

  auto table_it = storage_.find(tab_name);
  if(table_it != storage_.end())
    return Reply{false, "duplicate table " + tab_name};

  storage_.emplace(tab_name, table_t{});
    return Reply{true};
}

Reply DataBase::drop(const req_params_t& req_params) {
  std::unique_lock<std::mutex> lock(storage_guard_);

  if(req_params.size() != 1)
    return Reply{false, "wrong number of arguments"};

  auto tab_name = req_params[0];

  auto table_it = storage_.find(tab_name);
  if(table_it == storage_.end())
    return Reply{false, "unknown table name " + tab_name};

  storage_.erase(table_it);
    return Reply{true};
}

Reply DataBase::insert(const req_params_t& req_params) {
  std::unique_lock<std::mutex> lock(storage_guard_);

  if(req_params.size() != 3)
    return Reply{false, "wrong number of arguments"};

  auto tab_name = req_params[0];
  auto str_id = req_params[1];
  auto val = req_params[2];

  auto table_it = storage_.find(tab_name);
  if(table_it == storage_.end())
    return Reply{false, "unknown table name " + tab_name};

  auto id = strtoll(str_id.c_str(), nullptr, 0);
  if(id < 0 || id > std::numeric_limits<size_t>::max())
    return Reply{false, "incorrect id" + str_id};

  auto str_it = table_it->second.find(static_cast<size_t>(id));
  if(str_it != table_it->second.end())
    return Reply{false, "duplicate " + std::to_string(id)};

  table_it->second.emplace(id, val);
    return Reply{true};
}

Reply DataBase::truncate(const req_params_t& req_params) {
  std::unique_lock<std::mutex> lock(storage_guard_);

  if(req_params.size() != 1)
    return Reply{false, "wrong number of arguments"};

  auto tab_name = req_params[0];

  auto table_it = storage_.find(tab_name);
  if(table_it == storage_.end())
    return Reply{false, "unknown table name " + tab_name};

  table_it->second.clear();
    return Reply{true};
}

Reply DataBase::intersection(const req_params_t& req_params) {
  std::unique_lock<std::mutex> lock(storage_guard_);

  if(req_params.size() != 2)
    return Reply{false, "wrong number of arguments"};

  auto tab1_name = req_params[0];
  auto tab2_name = req_params[1];

  auto table1_it = storage_.find(tab1_name);
  if(table1_it == storage_.end())
    return Reply{false, "unknown table name " + tab1_name};

  auto table2_it = storage_.find(tab2_name);
  if(table2_it == storage_.end())
    return Reply{false, "unknown table name " + tab2_name};

  auto tab1 = table1_it->second;
  auto tab2 = table2_it->second;

  // Вычисление пересечения таблиц.
  table_t tab;
  std::set_intersection(tab1.begin(), tab1.end(), tab2.begin(), tab2.end(),
                        std::inserter(tab, tab.end()),
                        [](const auto& t1, const auto& t2) {
                          return t1.first < t2.first;
                        });

  // Формирование ответа.
  Reply reply{true};
  for(const auto& it: tab) {
    auto id = it.first;
    reply.add(std::to_string(id) + "," + tab1[id] + "," + tab2[id] + "\n");
  }
  return reply;
}

Reply DataBase::symmetric_difference(const req_params_t& req_params) {
  std::unique_lock<std::mutex> lock(storage_guard_);

  if(req_params.size() != 2)
    return Reply{false, "wrong number of arguments"};

  auto tab1_name = req_params[0];
  auto tab2_name = req_params[1];

  auto table1_it = storage_.find(tab1_name);
  if(table1_it == storage_.end())
    return Reply{false, "unknown table name " + tab1_name};

  auto table2_it = storage_.find(tab2_name);
  if(table2_it == storage_.end())
    return Reply{false, "unknown table name " + tab2_name};

  auto tab1 = table1_it->second;
  auto tab2 = table2_it->second;

  // Вычисление симметрической разности таблиц.
  table_t tab;
  std::set_symmetric_difference(tab1.begin(), tab1.end(), tab2.begin(), tab2.end(),
                                std::inserter(tab, tab.end()),
                                [](const auto& t1, const auto& t2) {
                                  return t1.first < t2.first;
                                });

  // Формирование ответа.
  Reply reply{true};
  for(const auto& it: tab) {
    auto id = it.first;
    reply.add(std::to_string(id) + ",");
    reply.add(((tab1.find(id) != tab1.end()) ? it.second + "," : "," + it.second) + "\n");
  }
  return reply;
}

} // namespace db.
