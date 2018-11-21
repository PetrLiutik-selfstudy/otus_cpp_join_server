#pragma once

#include "RequestParser.h"

#include "ThreadPool.h"

#include <functional>
#include <list>
#include <memory>

namespace db {

using conn_id_t = void*;

/**
 * @brief Класс котнекста обработчика команд.
 */
class ConnectionContext : public ThreadPool {
  public:
    /**
     * @brief Конструктор.
     * @param db - база данных.
     */
    explicit ConnectionContext(DataBase& db);

    ~ConnectionContext();

    /**
     * @brief Обработка входной команды.
     * @param request - запрос.
     * @param on_reply - функция обратного вызова по результату обработки запроса.
     */
    void process(const std::string& request, std::function<void(Reply&)> on_reply);

  private:

    /// Интерпретатор команд.
    RequestParser parser_;
    /// Строка, содержащая данные со входными командами.
    std::string data_{};
    /// id от имени которого выполняется обработка.
    conn_id_t conn_id_{};
};

} // namespace db.
