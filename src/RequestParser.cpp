#include "RequestParser.h"

#include <sstream>
#include <vector>

namespace db {

RequestParser::RequestParser(DataBase& db) {
  req_func_tab_.emplace(std::make_pair("CREATE", std::bind(&DataBase::create, std::ref(db), std::ref(req_params_))));
  req_func_tab_.emplace(std::make_pair("DROP", std::bind(&DataBase::drop, std::ref(db), std::ref(req_params_))));
  req_func_tab_.emplace(std::make_pair("INSERT", std::bind(&DataBase::insert, std::ref(db), std::ref(req_params_))));
  req_func_tab_.emplace(std::make_pair("TRUNCATE", std::bind(&DataBase::truncate, std::ref(db), std::ref(req_params_))));
  req_func_tab_.emplace(std::make_pair("INTERSECTION", std::bind(&DataBase::intersection, std::ref(db), std::ref(req_params_))));
  req_func_tab_.emplace(std::make_pair("SYMMETRIC_DIFFERENCE", std::bind(&DataBase::symmetric_difference, std::ref(db), std::ref(req_params_))));
}

Reply RequestParser::parse(const std::string& request) {
  // Разбиение входной строки.
  std::string req_type; // Тип запроса.
  std::istringstream iss(request);
  std::getline(iss, req_type, ' ');

  auto it = req_func_tab_.find(req_type);
  if(it != req_func_tab_.end()) {
    req_params_.clear();
    std::string param; // Параметр запроса.
    for(; std::getline(iss, param, ' ');)
      req_params_.emplace_back(param);

    auto req_func = it->second;
    return req_func();
  }
  return Reply{false, "unknown request " + req_type};
}

} // namespace db.
