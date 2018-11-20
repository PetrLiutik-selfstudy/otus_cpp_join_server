#pragma once

#include <ctime>
#include <ostream>
#include <string>
#include <vector>
#include <map>

namespace db {

/// Тип параметров запроса к БД.
using req_params_t = std::vector<std::string>;

/// Тип ответа на обращение к БД.
using reply_t = std::pair<bool, std::string>;

/**
 * @brief Класс базы данных.
 */
class DataBase {
  public:
    explicit DataBase() = default;
    ~DataBase() = default;

    /**
     * @brief Создать таблицу.
     * @param req_params - параметры запроса:
     *        req_params[0] - название таблицы.
     * @return - результат создания таблицы.
     */
    reply_t create(const req_params_t& req_params);

    /**
     * @brief Уничтожить таблицу таблицу.
     * @param req_params - параметры запроса:
     *        req_params[0] - название таблицы.
     * @return - результат уничтожения таблицы.
     */
    reply_t drop(const req_params_t& req_params);

    /**
     * @brief Поместить данные в таблицу.
     * @param req_params - параметры запроса:
     *        req_params[0] - название таблицы.
     *        req_params[1] - первичный ключ.
     *        req_params[2] - значение.
     * @return - результат помещения данных в таблицу.
     */
    reply_t insert(const req_params_t& req_params);

    /**
     * @brief Очистка таблицы.
     * @param req_params - параметры запроса:
     *        req_params[0] - название таблицы.
     * @return - результат очистки таблицы.
     */
    reply_t truncate(const req_params_t& req_params);

    /**
     * @brief Дать пересечение двух таблиц.
     * @param req_params - параметры запроса:
     *        req_params[0] - название таблицы 1.
     *        req_params[1] - название таблицы 2.
     * @return - результат формирования пересечения таблиц.
     */
    reply_t intersection(const req_params_t& req_params);

    /**
     * @brief Дать симметрическую разность двух таблиц.
     * @param req_params - параметры запроса:
     *        req_params[0] - название таблицы 1.
     *        req_params[1] - название таблицы 2.
     * @return - результат формирования симметрической разности таблиц.
     */
    reply_t symmetric_difference(const req_params_t& req_params);


  private:
    /// Тип таблицы, содержащейся в базе данных.
    using table_t = std::map<size_t, std::string>;
    /// Тип хранилища данных.
    using storage_t = std::map<std::string, table_t>;

    /// Хранилище данных.
    storage_t storage_{};
};

} // namespace db.
