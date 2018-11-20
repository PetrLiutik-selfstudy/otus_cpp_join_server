#include "DataBase.h"

#include <algorithm>
#include <iterator>

namespace db {

reply_t DataBase::create(const req_params_t& req_params) {
  if(req_params.size() != 1)
    return reply_t{false, "wrong number of arguments"};

  auto tab_name = req_params[0];

  auto table_it = storage_.find(tab_name);
  if(table_it != storage_.end())
    return reply_t{false, "duplicate table " + tab_name};

  storage_.emplace(tab_name, table_t{});
    return reply_t{true, ""};
}

reply_t DataBase::drop(const req_params_t& req_params) {
  if(req_params.size() != 1)
    return reply_t{false, "wrong number of arguments"};

  auto tab_name = req_params[0];

  auto table_it = storage_.find(tab_name);
  if(table_it == storage_.end())
    return reply_t{false, "unknown table name " + tab_name};

  storage_.erase(table_it);
    return reply_t{true, ""};
}

reply_t DataBase::insert(const req_params_t& req_params) {
  if(req_params.size() != 3)
    return reply_t{false, "wrong number of arguments"};

  auto tab_name = req_params[0];
  auto str_id = req_params[1];
  auto val = req_params[2];

  auto table_it = storage_.find(tab_name);
  if(table_it == storage_.end())
    return reply_t{false, "unknown table name " + tab_name};

  auto id = strtoll(str_id.c_str(), nullptr, 0);
  if(id < 0 || id > std::numeric_limits<size_t>::max())
    return reply_t{false, "incorrect id" + str_id};

  auto str_it = table_it->second.find(static_cast<size_t>(id));
  if(str_it != table_it->second.end())
    return reply_t{false, "duplicate " + std::to_string(id)};

  table_it->second.emplace(id, val);
    return reply_t{true, ""};
}

reply_t DataBase::truncate(const req_params_t& req_params) {
  if(req_params.size() != 1)
    return reply_t{false, "wrong number of arguments"};

  auto tab_name = req_params[0];

  auto table_it = storage_.find(tab_name);
  if(table_it == storage_.end())
    return reply_t{false, "unknown table name " + tab_name};

  table_it->second.clear();
    return reply_t{true, ""};
}

reply_t DataBase::intersection(const req_params_t& req_params) {
  if(req_params.size() != 2)
    return reply_t{false, "wrong number of arguments"};

  auto tab1_name = req_params[0];
  auto tab2_name = req_params[0];

  auto table1_it = storage_.find(tab1_name);
  if(table1_it == storage_.end())
    return reply_t{false, "unknown table name " + tab1_name};

  auto table2_it = storage_.find(tab2_name);
  if(table1_it == storage_.end())
    return reply_t{false, "unknown table name " + tab2_name};

  table_t result_tab;
  std::set_intersection(table1_it->second.begin(), table1_it->second.end(),
                        table2_it->second.begin(), table2_it->second.end(),
                        std::inserter(result_tab, result_tab.end()),
                        [](const auto &tab1, const auto& tab2) {
                            return tab1.first < tab2.first;
                        });

  reply_t reply{true, {}};
  for(const auto& it: result_tab){
    auto id = it.first;
    reply.second += std::to_string(id) + ",";
    reply.second += table1_it->second[id] + "," + table2_it->second[id] + "\n";
  }
  return reply;
}

reply_t DataBase::symmetric_difference(const req_params_t& req_params) {


  reply_t reply;
  reply.first = true;
  reply.second = "OK";
  return reply;
}

} // namespace db.
