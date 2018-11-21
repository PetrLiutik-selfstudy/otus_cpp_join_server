#pragma once

#include "DataBase.h"

#include <string>
#include <map>
#include <functional>

namespace db {

/**
 * @brief Класс парсера запросов к БД.
 */
class RequestParser {
  public:
    /**
     * @brief Конструтор парсера запросов к БД.
     */
    explicit RequestParser(DataBase& db);

    ~RequestParser() = default;

    /**
     * @brief Парсинг принятого запроса.
     * @param request - принятый запрос.
     * @return результат обработки запроса.
     */
    Reply parse(const std::string& request);

  private:

    /// Таблица функций запросов к БД.
    std::map<std::string, std::function<Reply()>> req_func_tab_;
    /// Параметры запроса.
    req_params_t req_params_;
};

} // namespace db.
