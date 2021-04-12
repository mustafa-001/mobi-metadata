#ifndef RECORD0_H
#define RECORD0_H
#include <iostream>
#include <istream>
#include "parse_int.h"
#include "exth_header.h"

struct record0{
  private:
  std::vector<char> end_blob = std::vector<char>();
  public:
  std::vector<char> mobi_blob = std::vector<char>();
  mobi_exth_header exth_header;
  void deserialize(std::istream& input, int record_length);
  void serialize(std::ostream& out);
  int length();
};


void record0::deserialize(std::istream& input,int record_length){
  int record0_start = input.tellg();
  if (int a = parse_u32_be(input, record0_start+16) != 0x4d4f4249){
    std::cout << "Mobi header doesn't start with letters \"MOBI\"   : " << a;
    std::exit(-1);
  }
  int mobi_header_length = parse_u32_be(input, record0_start+4 + 16);
  mobi_blob.resize(mobi_header_length+16);
  input.seekg(record0_start);
  input.read(mobi_blob.data(), mobi_header_length+16);
  exth_header.deserialize(input);
  std::cout << "record0 mobi header length: " << mobi_blob.size() << "\n";
  end_blob.resize(record_length - mobi_blob.size()  - exth_header.nul_padded_length());
  input.read(end_blob.data(), record_length - mobi_blob.size()- exth_header.nul_padded_length());
  std::cout << "record0 nd blob length: " << end_blob.size() << "\n";
  // std::cout.write(end_blob.data(), end_blob.size());
}

void record0::serialize(std::ostream &out){
  out.write(mobi_blob.data(), mobi_blob.size());
  exth_header.serialize(out);
  out.write(end_blob.data(), end_blob.size());

}
#endif