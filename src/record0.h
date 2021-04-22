#ifndef RECORD0_H
#define RECORD0_H
#include <bits/stdint-uintn.h>
#include <iostream>
#include <istream>
#include "parse_int.h"
#include "exth_header.h"
#include "mobi_header.h"

const uint8_t PalmDOC_HEADER_LENGTH = 16;

struct record0{
  private:
  std::vector<char> end_blob = std::vector<char>();
  public:
  std::vector<char> palmDoc_header = std::vector<char>(PalmDOC_HEADER_LENGTH);
  mobi_exth_header exth_header;
  mobi_header mobi_h;
  void deserialize(std::istream& input, unsigned record_length);
  void serialize(std::ostream& out);
  auto length() -> int;
};


void record0::deserialize(std::istream& input,unsigned record_length){
  int record0_start = input.tellg();
  int mobi_header_length = parse_u32_be(input, record0_start+4 + PalmDOC_HEADER_LENGTH);
  input.seekg(record0_start);
  input.read(palmDoc_header.data(), PalmDOC_HEADER_LENGTH);
  mobi_h = mobi_header{input};
  exth_header = mobi_exth_header{};
  exth_header.deserialize(input);
  mobi_h.parse_full_title(input);
  end_blob.resize(record_length - PalmDOC_HEADER_LENGTH -mobi_h.length()  - mobi_h.get_full_title_length() -(((exth_header.length() & 1 | 1 << 3) & 0b00)));
  input.read(end_blob.data(), end_blob.size());
  std::cout << "record0 end blob length: " << end_blob.size() << "\n";
  // std::cout.write(end_blob.data(), end_blob.size());
}

void record0::serialize(std::ostream &out){
  out.write(palmDoc_header.data(), PalmDOC_HEADER_LENGTH);
  mobi_h.serialize(out);
  exth_header.serialize(out);
  mobi_h.write_full_title(out);
  out.write(end_blob.data(), end_blob.size());
}
#endif