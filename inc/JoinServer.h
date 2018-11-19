#pragma once

#include <boost/asio.hpp>

namespace ba = boost::asio;

namespace bulk {

/**
 * @brief Класс сервера обработки запросов к БД.
 */
class JoinServer {
public:
  /**
   * @brief Конструктор сервера обработки запросов к БД.
   * @param port - номер порта.
   */
  explicit JoinServer(uint16_t port);

  ~JoinServer();

  /**
   * @brief Запуск сервера.
   */
  void start();

private:
  /**
   * @brief Обработка акцептирования сокета.
   */
  void handle_accept();

  /**
   * @brief Обработка останова сервера.
   */
  void handle_stop();

  ba::io_service        service_{};
  ba::ip::tcp::acceptor acceptor_;
  ba::ip::tcp::socket   sock_;
};

} // namespace bulk.
