#include "../src/record0.h"
#include "r0.h"
#include <fstream>
#include <gtest/gtest.h>
#include <ios>
#include <sstream>

TEST(record0, deserialize_record0) {
  auto r = record0{};
  auto i = std::stringstream{};
  std::copy(record0_example.begin(), record0_example.end(),
            std::ostream_iterator<uint8_t>(i));
  r.deserialize(i, record0_example.size());
  EXPECT_TRUE(r.palmDoc_header[1] == 0x02);
  EXPECT_TRUE(r.exth_header.records[1].type ==
              mobi_metadata_record_type::publisher);
};

TEST(record0, round_trip) {
  auto r = record0{};
  auto i = std::stringstream{};
  auto o = std::stringstream{};
  std::copy(record0_example.begin(), record0_example.end(),
            std::ostream_iterator<uint8_t>(i));
  r.deserialize(i, record0_example.size());
  r.serialize(o);
  EXPECT_EQ(o.str()[1], i.str()[1]);
  EXPECT_EQ(o.str()[100], i.str()[100]);
  EXPECT_EQ(o.str()[100], i.str()[100]);
  EXPECT_EQ(o.str()[250], i.str()[250]);
  EXPECT_EQ(o.str()[-1], i.str()[-1]);
  EXPECT_EQ(o.str().length(), i.str().length());
}

TEST(mobi_header, deserialize) {
  auto r = record0{};
  auto i = std::stringstream{};
  std::copy(record0_example.begin(), record0_example.end(),
            std::ostream_iterator<uint8_t>(i));
  r.deserialize(i, record0_example.size());
  EXPECT_TRUE(r.mobi_h.length() == 0xe8);
  EXPECT_EQ(r.mobi_h.get_first_non_book_index(), 0x81);
  EXPECT_EQ(r.mobi_h.get_full_title_offset(), 0x6e8);
  // EXPECT_EQ(r.mobi_h.get_full_title_length()(), "Foundation");
};

TEST(exth_header, deserialize) {
  auto e = mobi_exth_header{};
  auto i = std::stringstream{};
  std::copy(record0_example.begin(), record0_example.end(),
            std::ostream_iterator<uint8_t>(i));
  i.seekg(232 + 16);
  e.deserialize(i);
  EXPECT_EQ(e.records.size(), 11);
  EXPECT_TRUE(e.records[0].type == mobi_metadata_record_type::author);
}
TEST(exth_header, serialize) {
  auto e = mobi_exth_header{};
  auto o = std::stringstream{};
  mobi_metadata_record r{mobi_metadata_record_type::publisher, "tes"};
  e.records.push_back(r);
  // std::copy(record0_example.begin(), record0_example.end(),
  // std::ostream_iterator<uint8_t>(i)); i.seekg(232 + 16); e.deserialize(i);
  e.serialize(o);
  EXPECT_EQ(o.str()[0], 'E');
  EXPECT_TRUE(o.str()[7] > 5);
  EXPECT_EQ(o.str()[11], 1);
  EXPECT_EQ(o.str()[15], 101);
  EXPECT_EQ(o.str()[19], 11);
  EXPECT_EQ(o.str()[20], 't');
  EXPECT_EQ(o.str().length() % 4, 0);
  EXPECT_EQ(o.str().length(), 4 + 4 + 4 + 4 + 4 + 3 + 1);
}
TEST(exth_header, round) {
  auto e = mobi_exth_header{};
  auto i = std::stringstream{};
  auto o = std::stringstream{};
  std::copy(record0_example.begin(), record0_example.end(),
            std::ostream_iterator<uint8_t>(i));
  i.seekg(232 + 16);
  e.deserialize(i);
  e.serialize(o);
  // for(int j=0; j < 100; j++){
  //   EXPECT_EQ(i.str()[248+j], o.str()[j]);
  // }
  EXPECT_EQ(i.str()[248], o.str()[0]);
  EXPECT_EQ(i.str()[249], o.str()[1]);
  EXPECT_EQ(i.str()[258], o.str()[10]);
  EXPECT_EQ(i.str()[268], o.str()[20]);
  EXPECT_EQ(i.str()[288], o.str()[40]);
  EXPECT_EQ(i.str()[458], o.str()[210]);
}