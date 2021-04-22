
#include <bits/stdint-uintn.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "parse_int.h"
const unsigned int MOBI_HEADER_LENGTH_POS = 0x04;
const unsigned int FIRST_NON_BOOK_INDEX_POS = 0x40;
const unsigned int FULL_TITLE_OFFSET_POS = 0x44;
const unsigned int FULL_TITLE_LENGTH_POS = 0x48;

class mobi_header {
 private:
  unsigned int full_title_offset;
  std::vector<char> data;
  std::string full_title;

 public:
  auto get_first_non_book_index() -> uint32_t;
  void deserialize(std::istream& input);
  mobi_header(std::istream& input);
  mobi_header()= default;;
  void parse_full_title(std::istream& input);
  void write_full_title(std::ostream &output);
  void serialize();

  void serialize(std::ostream& output);
  auto length() -> unsigned int;

  void set_full_title_offset(uint32_t offset);
  auto get_full_title_offset() -> uint32_t;

  auto get_full_title_length() -> uint32_t;
  void set_full_title_length(uint32_t len);
};

void mobi_header::parse_full_title(std::istream &input){
  full_title = std::string(get_full_title_length(), '\0');
  input.read(full_title.data(), get_full_title_length());
  unsigned padding;
  if (length() % 4 == 0){
    padding = 0;
  } else {
    padding = (length()/4+1)*4;
  }
  for(unsigned i=0; i<padding; ++i){
    input.get();
  }
}

void mobi_header::write_full_title(std::ostream &output){
  output << full_title;
  unsigned padding = 0;
  if (get_full_title_length() % 4 == 0 ){
    padding = get_full_title_length() - (get_full_title_length() & 3);
  }
  for(unsigned i=0; i<padding; ++i){
    output.put('\0');
  }

}


mobi_header::mobi_header(std::istream& input) {
  deserialize(input);
}

auto mobi_header::get_first_non_book_index() -> uint32_t {
  return parse_u32_be(data.data() + FIRST_NON_BOOK_INDEX_POS * sizeof(char));
}

auto mobi_header::length() -> uint32_t {
  return parse_u32_be(data.data() + MOBI_HEADER_LENGTH_POS * sizeof(char));
}

auto mobi_header::get_full_title_offset() -> uint32_t {
  return parse_u32_be(data.data() + FULL_TITLE_OFFSET_POS * sizeof(char));
}
void mobi_header::set_full_title_offset(uint32_t offset) {
  auto b = encode_u32_be(offset);
  for (int i = 0; i < 4; i++) {
    data[FULL_TITLE_OFFSET_POS + i] = b[i];
  }
}

auto mobi_header::get_full_title_length() -> uint32_t {
  return parse_u32_be(data.data() + FULL_TITLE_LENGTH_POS * sizeof(char));
}
void mobi_header::set_full_title_length(uint32_t len) {
  auto b = encode_u32_be(len);
  for (int i = 0; i < 4; i++) {
    data[FULL_TITLE_LENGTH_POS + i] = b[i];
  }
}

void mobi_header::deserialize(std::istream& input) {
  int header_start = input.tellg();
  if (int a = parse_u32_be(input); a != 0x4d4f4249) {
    std::cout << "Mobi header doesn't start with letters \"MOBI\"   : " << a;
    std::exit(-1);
  }
  auto mobi_header_length = parse_u32_be(input);
  std::clog << "Mobi header length: " << mobi_header_length << "\n";
  data.resize(mobi_header_length);
  input.seekg(header_start);
  input.read(data.data(), mobi_header_length);
}

void mobi_header::serialize(std::ostream& output) {
  output.write(data.data(), data.size());
}