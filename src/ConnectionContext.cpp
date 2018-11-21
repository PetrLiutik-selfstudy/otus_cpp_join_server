#include "ConnectionContext.h"

#include <algorithm>
#include <istream>
#include <thread>

namespace db {

ConnectionContext::ConnectionContext(DataBase& db) : parser_{db} {
  // Запуск необходимо числа потоков для обработки запросов одного соединения.
  start(1);
}

ConnectionContext::~ConnectionContext() {
  stop();
}

void ConnectionContext::process(const std::string& request, std::function<void(Reply&)> on_reply) {
  data_.append(request);
  for(;;) {
    auto end_line = data_.find('\n');
    if(end_line != std::string::npos) {
      std::string str = data_.substr(0, end_line);

      // Создание задачи для пула потоков для обработки запроса к БД.
      add_job([this, str, on_reply]() {
        auto reply = parser_.parse(str);
        on_reply(reply);
      });

      if(data_.size() > end_line)
        data_ = data_.substr(++end_line);
      else
        break;
    }
    else
      break;
  }
}

} // namespace db.
