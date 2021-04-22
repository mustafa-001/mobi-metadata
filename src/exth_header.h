
#ifndef EXTH_HEADER_H
#define EXTH_HEADER_H
#include "parse_int.h"
#include <cstdlib>
#include <iostream>
#include <istream>
#include <iterator>
#include <stdint.h>
#include <string>
enum struct mobi_metadata_record_type : uint32_t {
  author = 100,
  publisher = 101,
  imprint = 102,
  description = 103,
  isbn = 104,
  subject = 105,
  publishing_date = 106,
  contributer = 108,
  cover_offset = 201,
  thumb_offset = 202,
  has_fake_cover = 203,
  updated_title = 503,
};
struct mobi_metadata_record {
  mobi_metadata_record_type type;
  std::string data;
};

auto parse_record(std::istream &input) -> mobi_metadata_record;

class mobi_exth_header {
public:
  std::vector<mobi_metadata_record> records;
  void serialize(std::ostream &output);
  void deserialize(std::istream &input);
  void write(std::ostream &out);
  auto length() -> unsigned int;
  auto nul_padded_length() -> unsigned int;
private:
};

auto mobi_exth_header::nul_padded_length() -> unsigned int {
  if (length() % 4 == 0) {
    return length();
  } else {
    return (length() / 4 + 1) * 4;
  }
}
auto mobi_exth_header::length() -> unsigned int {
  unsigned int res = 0;
  for (auto &r : records) {
    res += (r.data.size() + 4 + 4);
  }
  return res + 4 + 4 + 4;
}

void mobi_exth_header::deserialize(std::istream &input) {
  int exth_start = input.tellg();
  auto exth_length = parse_u32_be(input, exth_start + 4);
  auto record_count = parse_u32_be(input);
  // std::cout << "records count: " << record_count
  //           << "   exth lenght: " << exth_length;
  std::vector<char> buffer(exth_length, '\0');
  for (unsigned int i = 0; i < record_count; i++) {
    auto r = parse_record(input);
    records.push_back(r);
  }
  //While this is not necessary by MOBI standart two option available isn't much
  //useful: first we could throw and exception, second we would try to skip end of
  //the exth header via exth_length, meaning it's still cumbersome.
  //Some books may give ´exth_length´ as nul padded value so we need to account for possible
  //3 bytes difference between ´exth_length´ and this.length()
  while (exth_length - 3 > length()){
    std::cout << "There is still data in exth header after parsing all records!" << std::endl;
    std::cout << "exth length in book: " << exth_length << "   currently parsed: " << length() << std::endl;
    auto r = parse_record(input);
    records.push_back(r);
  }

  //Consume the nul padding. Some books may give ´exth_length´ as nul padded value so
  //we cannot count on this.length(). (actually we can)
  if (exth_length % 4 != 0) {
    for (unsigned int i = 0; i < 4-(exth_length % 4); i++) {
      input.get();
    }
  };
}

void mobi_exth_header::serialize(std::ostream &out) {
  out << "EXTH";
  for (auto &c : encode_u32_be(length())) {
    out.put(c);
  }
  for (auto &c : encode_u32_be(records.size())) {
    out.put(c);
  }
  for (auto &r : records) {
    for (auto &c : encode_u32_be(static_cast<uint32_t>(r.type))) {
      out.put(c);
    }
    for (auto &c : encode_u32_be(r.data.length() + 8)) {
      out.put(c);
    }
    if(!out.write(r.data.data(), r.data.length())){
      std::cout << "error writing exth records data";
      std::exit(-1);

    };
  }
  if (length() % 4 != 0) {
    for (unsigned int i = length() % 4; i < 4; i++) {
      out.put('\0');
    }
  }
}
// void mobi_exth_header::deserialize

auto parse_record(std::istream &input) -> mobi_metadata_record {
  mobi_metadata_record_type type{parse_u32_be(input)};
  uint32_t length = parse_u32_be(input);
  auto data = std::string{}; //(length - 8, '\0');
  // data.resize(length -8);
  for (unsigned int i = 0; i < length - 8; i++) {
    data.push_back(input.get());
  }
  return mobi_metadata_record{type, data};
}
#endif