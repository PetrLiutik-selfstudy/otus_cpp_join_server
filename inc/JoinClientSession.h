#pragma once

//#include "CmdProcessor.h"

//#include <array>
//#include <memory>

//#include <boost/asio.hpp>

//namespace ba = boost::asio;

//namespace bulk {

///**
// * @brief Класс сессии клиента.
// */
//class JoinClientSession : public std::enable_shared_from_this<JoinClientSession> {
//public:
//  /**
//   * @brief Конструктор клиентской сессии.
//   * @param sock - сокет, через который установлено соединение с клиентом.
//   */
//  explicit JoinClientSession(ba::ip::tcp::socket sock);

//  ~JoinClientSession() = default;

//  /**
// * @brief Запустить клиентскую сессию.
// */
//  void start();

//private:

//  /**
//   * @brief Обработка асинхронного чтения данных из сокета.
//   */
//  void handle_read();

//  /// Размер приемного буфера.
//  static constexpr size_t buf_len = 1024;

//  ba::ip::tcp::socket                sock_;
//  handle_t                           handle_{};
//  std::array<char, buf_len>          buf_;
//  std::shared_ptr<JoinClientSession> self_;
//};

//} // namespace bulk.
