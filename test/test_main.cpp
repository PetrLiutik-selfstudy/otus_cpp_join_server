#include "gtest/gtest.h"
#include "../inc/ver.h"
#include "../inc/DataBase.h"

#include <vector>
#include <string>
#include <sstream>

TEST(ver_test_case, ver_major_test) {
  EXPECT_GE(ver_major(), 1);
}

TEST(ver_test_case, ver_minor_test) {
  EXPECT_GE(ver_minor(), 1);
}

TEST(ver_test_case, ver_patch_test) {
  EXPECT_GE(ver_patch(), 1);
}

TEST(database_test_case, create_test) {
  db::DataBase db;

  auto reply = db.create({"C"});
  std::ostringstream oss;
  oss << reply;
  EXPECT_EQ(oss.str(), std::string{"OK\n"});

  auto reply1 = db.create({"C"});
  std::ostringstream oss1;
  oss1 << reply1;
  EXPECT_EQ(oss1.str(), std::string{"ERR duplicate table C\n"});
}

TEST(database_test_case, drop_test) {
  db::DataBase db;

  db.create({"C"});

  auto reply = db.drop({"C"});
  std::ostringstream oss;
  oss << reply;
  EXPECT_EQ(oss.str(), std::string{"OK\n"});

  auto reply1 = db.drop({"C"});
  std::ostringstream oss1;
  oss1 << reply1;
  EXPECT_EQ(oss1.str(), std::string{"ERR unknown table name C\n"});
}

TEST(database_test_case, insert_test) {
  db::DataBase db;

  db.create({"C"});

  auto reply = db.insert({"C", "1", "aaa"});
  std::ostringstream oss;
  oss << reply;
  EXPECT_EQ(oss.str(), std::string{"OK\n"});

  auto reply1 = db.insert({"C", "1", "bbb"});
  std::ostringstream oss1;
  oss1 << reply1;
  EXPECT_EQ(oss1.str(), std::string{"ERR duplicate 1\n"});

  auto reply2 = db.insert({"C", "2", "bbb"});
  std::ostringstream oss2;
  oss2 << reply2;
  EXPECT_EQ(oss2.str(), std::string{"OK\n"});

  auto reply3 = db.insert({"D", "3", "ccc"});
  std::ostringstream oss3;
  oss3 << reply3;
  EXPECT_EQ(oss3.str(), std::string{"ERR unknown table name D\n"});

  auto reply4 = db.insert({"C", "3"});
  std::ostringstream oss4;
  oss4 << reply4;
  EXPECT_EQ(oss4.str(), std::string{"ERR wrong number of arguments\n"});
}

TEST(database_test_case, truncate_test) {
  db::DataBase db;

  db.create({"C"});
  db.insert({"C", "1", "aaa"});
  db.insert({"C", "2", "bbb"});

  auto reply = db.truncate({"C"});
  std::ostringstream oss;
  oss << reply;
  EXPECT_EQ(oss.str(), std::string{"OK\n"});

  auto reply1 = db.insert({"C", "1", "aaa"});
  std::ostringstream oss1;
  oss1 << reply1;
  EXPECT_EQ(oss1.str(), std::string{"OK\n"});
}

TEST(database_test_case, intersection_test) {
  db::DataBase db;

  db.create({"C"});
  db.insert({"C", "1", "aaa"});
  db.insert({"C", "2", "bbb"});

  db.create({"D"});
  db.insert({"D", "1", "xxx"});
  db.insert({"D", "3", "yyy"});

  auto reply = db.intersection({"C", "D"});
  std::ostringstream oss;
  oss << reply;
  EXPECT_EQ(oss.str(), std::string{"1,aaa,xxx\nOK\n"});

  auto reply1 = db.intersection({"C", "C"});
  std::ostringstream oss1;
  oss1 << reply1;
  EXPECT_EQ(oss1.str(), std::string{"1,aaa,aaa\n2,bbb,bbb\nOK\n"});

  db.truncate({"D"});
  auto reply2 = db.intersection({"C", "D"});
  std::ostringstream oss2;
  oss2 << reply2;
  EXPECT_EQ(oss2.str(), std::string{"OK\n"});
}

TEST(database_test_case, symmetric_difference_test) {
  db::DataBase db;

  db.create({"C"});
  db.insert({"C", "1", "aaa"});
  db.insert({"C", "2", "bbb"});

  db.create({"D"});
  db.insert({"D", "1", "xxx"});
  db.insert({"D", "3", "yyy"});

  auto reply = db.symmetric_difference({"C", "D"});
  std::ostringstream oss;
  oss << reply;
  EXPECT_EQ(oss.str(), std::string{"2,bbb,\n3,,yyy\nOK\n"});

  auto reply1 = db.symmetric_difference({"C", "C"});
  std::ostringstream oss1;
  oss1 << reply1;
  EXPECT_EQ(oss1.str(), std::string{"OK\n"});

  db.truncate({"D"});
  auto reply2 = db.symmetric_difference({"C", "D"});
  std::ostringstream oss2;
  oss2 << reply2;
  EXPECT_EQ(oss2.str(), std::string{"1,aaa,\n2,bbb,\nOK\n"});
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
