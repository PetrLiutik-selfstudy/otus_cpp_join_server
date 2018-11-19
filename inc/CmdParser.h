#pragma once

#include <string>

namespace bulk {

/**
 * @brief Класс парсера команд.
 */
class CmdParser {
public:
  /**
  * @brief Конструктор.
  * @param bulk_size - макисмальный размер блока команд.
  */
  explicit CmdParser() = default;

  ~CmdParser() = default;

  /**
   * @brief Парсинг принятой команды.
   * @param request - принятый запрос.
   * @param reply - ответ.
   * @return true - запрос обработан без ошибок, false - при обработке запроса возникла ошибка.
   */
  bool parse(const std::string& request, std::string& reply);

private:


};

} // namespace bulk.
