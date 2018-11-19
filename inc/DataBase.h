#pragma once

#include <ctime>
#include <ostream>
#include <string>
#include <vector>
#include <map>

namespace bulk {

/**
 * @brief Класс блока команд.
 */
class DataBase {
  public:
    using reply_t = std::pair<bool, std::string>;

    DataBase() = default;
    ~DataBase() = default;

    /**
     * @brief Поместить данные в таблицу.
     * @param table_name - название таблицы.
     * @param id - первичный ключ.
     * @param value - значение.
     * @return - результат помещения данных в таблицу.
     */
    reply_t insert(const std::string& table_name, int id, const std::string& value);

    /**
     * @brief Очистка таблицы.
     * @param - название таблицы.
     * @return - результат очистки таблицы.
     */
    reply_t truncate(const std::string& table_name);

    /**
     * @brief Дать пересечение двух таблиц.
     * @param table1_name - название таблицы 1.
     * @param table2_name - название таблицы 2.
     * @return - результат формирования пересечения таблиц.
     */
    reply_t intersection(const std::string& table1_name, const std::string& table2_name);

    /**
     * @brief Дать симметрическую разность двух таблиц.
     * @param table1_name - название таблицы 1.
     * @param table2_name - название таблицы 2.
     * @return - результат формирования симметрической разности таблиц.
     */
    reply_t symmetric_difference(const std::string& table1_name, const std::string& table2_name);


  private:
    /// Тип таблицы, содержащейся в базе данных.
    using table_t = std::map<int, std::string>;
    /// Тип базы данных.
    using database_t = std::map<std::string, table_t>;


    database_t database_;
};



} // namespace bulk.
