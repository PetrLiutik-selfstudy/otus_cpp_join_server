#pragma once

#include <ctime>
#include <map>
#include <mutex>
#include <ostream>
#include <string>
#include <vector>

namespace db {

/// Тип параметров запроса к базе данных.
using req_params_t = std::vector<std::string>;

/// Класс ответа на запрос к базе данных.
class Reply {
public:
  explicit Reply(bool result, const std::string& value) : result_{result}, value_{value} {
  }
  explicit Reply(bool result) : result_{result}, value_{} {
  }
  ~Reply() = default;
  Reply(const Reply& other) = default;
  Reply(Reply&& other) noexcept = default;
  Reply& operator =(const Reply& other) = default;
  Reply& operator =(Reply&& other) = default;

  void add(const std::string& value) {
    value_ += value;
  }

  /**
   * @brief Оператор вывода в поток блока команд.
   * @param os - поток вывода.
   * @param bulk - блок команд для вывода в поток.
   * @return
   */
  friend std::ostream& operator <<(std::ostream& os, const Reply& reply);

private:
  bool result_{};
  std::string value_{};
};


/**
 * @brief Класс базы данных.
 */
class DataBase {
  public:
    explicit DataBase();
    ~DataBase() = default;

    /**
     * @brief Создать таблицу.
     * @param req_params - параметры запроса:
     *        req_params[0] - название таблицы.
     * @return - результат создания таблицы.
     */
    Reply create(const req_params_t& req_params);

    /**
     * @brief Уничтожить таблицу таблицу.
     * @param req_params - параметры запроса:
     *        req_params[0] - название таблицы.
     * @return - результат уничтожения таблицы.
     */
    Reply drop(const req_params_t& req_params);

    /**
     * @brief Поместить данные в таблицу.
     * @param req_params - параметры запроса:
     *        req_params[0] - название таблицы.
     *        req_params[1] - первичный ключ.
     *        req_params[2] - значение.
     * @return - результат помещения данных в таблицу.
     */
    Reply insert(const req_params_t& req_params);

    /**
     * @brief Очистка таблицы.
     * @param req_params - параметры запроса:
     *        req_params[0] - название таблицы.
     * @return - результат очистки таблицы.
     */
    Reply truncate(const req_params_t& req_params);

    /**
     * @brief Дать пересечение двух таблиц.
     * @param req_params - параметры запроса:
     *        req_params[0] - название таблицы 1.
     *        req_params[1] - название таблицы 2.
     * @return - результат формирования пересечения таблиц.
     */
    Reply intersection(const req_params_t& req_params);

    /**
     * @brief Дать симметрическую разность двух таблиц.
     * @param req_params - параметры запроса:
     *        req_params[0] - название таблицы 1.
     *        req_params[1] - название таблицы 2.
     * @return - результат формирования симметрической разности таблиц.
     */
    Reply symmetric_difference(const req_params_t& req_params);


  private:
    /// Тип таблицы, содержащейся в базе данных.
    using table_t = std::map<size_t, std::string>;
    /// Тип хранилища данных.
    using storage_t = std::map<std::string, table_t>;


    /// Хранилище данных.
    storage_t storage_{};
    std::mutex storage_guard_{};
};

} // namespace db.
