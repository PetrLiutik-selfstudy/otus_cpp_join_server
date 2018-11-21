#pragma once

#include "ConnectionContext.h"
#include "DataBase.h"

#include <mutex>
#include <set>
#include <vector>

namespace db {

/**
 * @brief Класс обертки базы данных.
 */
class DataBaseWrapper {
  public:
    /**
     * @brief Дать объект класса DataBaseWrapper.
     * @return объект класса DataBaseWrapper.
     */
    static DataBaseWrapper& get_instance();

    /**
     * @brief Подключиться к БД.
     * @return id соединения с БД.
     */
    conn_id_t connect();

    /**
     * @brief Отключиться от БД.
     * @param conn_id - id соединения с БД.
     */
    void disconnect(const conn_id_t& conn_id);

    /**
     * @brief Обработать запрос к БД.
     * @param conn_id - id соединения с БД.
     * @param request - запрос.
     * @param on_reply - функция обратного вызова по результату обработки запроса.
     */
    void process(const conn_id_t& conn_id, const std::string& request,
                 std::function<void(Reply&)> on_reply);

  private:
    DataBaseWrapper() = default;
    ~DataBaseWrapper() = default;

    DataBase db_;

    std::mutex contexts_mutex_{};
    std::map<conn_id_t, std::shared_ptr<ConnectionContext>> conns_;
    uint8_t context_id_{};
    std::vector<conn_id_t> free_conn_ids_;
};

} // namespace db.
