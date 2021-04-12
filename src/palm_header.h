#ifndef PALM_HEADER_H
#define PALM_HEADER_H
#include <stdint.h>
#include <ios>
#include <iostream>
#include <istream>
#include <vector>
#include "parse_int.h"
struct palmdatabase_header{
  private:
  std::vector<char> start_blob = std::vector<char>();
  std::vector<char> end_blob = std::vector<char>();
  public:
  std::vector<uint32_t> record_offsets;
  void deserialize(std::istream& input);
  void serialize(std::ostream& out);
  void update_record_offsets(int delta);
};

void palmdatabase_header::update_record_offsets(int delta){
  for(auto& o: record_offsets){
    o += delta;
  }
  record_offsets[0] -= delta;
}

void palmdatabase_header::deserialize(std::istream& input){
  start_blob.resize(76);
  input.readsome(start_blob.data(), 76);
  auto pdb_records = parse_u16_be(input);
  std::cout<< "number of pdb records: " << std::hex << std::showbase << pdb_records <<"\n";
  for(int i=0; i < pdb_records; i++ ){
    record_offsets.push_back(parse_u32_be(input));
    //record info, mostly useless
    parse_u32_be(input);
  }
  std::cout << "First record offset: " << std::hex << std::showbase << record_offsets[0] << "\n";
  //76: start of pdb header, 2: number of pdb records, 8: each offset and info field for pdb records.
  // Remaining bytes of pdb header are end_blob.(We are not interested in changing start and end data)
  end_blob.resize(record_offsets[0]-76-2-8*record_offsets.size());
  input.read(end_blob.data(),record_offsets[0]-76-2-8*record_offsets.size());
}
void palmdatabase_header::serialize(std::ostream& out){
  for(auto& c: start_blob){
    out.put(c);
  }
  for (auto& c:encode_u16_be(record_offsets.size())){
      out.put(c);
  }
  for(auto& o: record_offsets){
    for (auto& c:encode_u32_be(o)){
      out.put(c);
    }
    // for record info 
    for (auto& c:encode_u32_be(0)){
      out.put(c);
    }
  }
  for(auto& c: end_blob){
    out.put(c);
  }
}
#endif