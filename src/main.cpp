#include "exth_header.h"
#include "palm_header.h"
#include "parse_int.h"
#include <stdint.h>
#include <cstddef>
#include <filesystem>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include "record0.h"

// change exth;
// find new exth length;
// update mobi_header_record_offsets;
// write new  palmdatabase header;
// write 16 byte palmdoc header;
// write mobi header
// write exth header
// write remaining mobi header
// copy all other records

const uint32_t EXTH_START = 0x590;
const uint32_t FIRST_RECORD_OFFSET_OFFSET = 78;
const uint32_t MOBI_PALMDOC_HEADER_LENGTH = 0x10;
const uint32_t MOBI_LENGTH_OFFSET = 0x04;
int main(int argc, char **argv) {
  // palmdatabase_header a{};
  // std::stringstream b;
  // b.write("asdfasdfasdfasdfassdaa", 20);
  // b.seekg(0);
  // a.deserialize(b);
  // a.serialize(std::cout);
  // std::exit(-1);
  std::filesystem::path input_book_path{argv[1]};

  if (std::ifstream input_book{input_book_path, std::ios::binary | std::ios::in }) {
    std::cout << "\nbook name: " << argv[argc-1] << std::endl;
    std::ofstream output_book;
    output_book.open(input_book_path.replace_filename(input_book_path.stem().string()+"_2"+input_book_path.extension().string()),  std::ios::trunc| std::ios::out |std::ios::binary);
    auto palm_header = palmdatabase_header{};
    palm_header.deserialize(input_book);
    record0 r0{}; 
    input_book.seekg(palm_header.record_offsets[0]);
    r0.deserialize(input_book, palm_header.record_offsets[1]-palm_header.record_offsets[0]);
    unsigned int old_exth_len = r0.exth_header.nul_padded_length();
    std::cout << "old exth len: " << old_exth_len << std::endl ;
    // r0.exth_header.records[0].data = "A new and long author name";
    // for(auto& r: r0.exth_header.records){
    //   if(r.type == mobi_metadata_record_type::updated_title){
    //     r.data ="A new and realllllllly long new title";
    //   }
    // }
    unsigned int new_exth_len = r0.exth_header.nul_padded_length();
    std::cout << "new exth len: " << new_exth_len << std::endl;
    int exth_len_delta = new_exth_len - old_exth_len;
    std::cout << "delta: " << exth_len_delta << "\n";
    palm_header.update_record_offsets(exth_len_delta); 
    palm_header.serialize(output_book);
    r0.serialize(output_book);
    output_book.flush();

    std::vector<char> b(100000, '\0');

    //TODO: Instead of a random number find a way to determine last records size.
    int record_size = 10000;
    while(input_book.read(b.data(), record_size)){
      output_book.write(b.data(), record_size);
      if (!input_book.good())
        std::cout <<"Error in input \n";
      if (input_book.bad())
        std::cout << "I/O error while reading\n";
      else if (input_book.eof())
        std::cout << "End of file reached successfully\n";
      else if (input_book.fail())
        std::cout << "Non-integer data encountered\n";
    }
    output_book.close();
    
  }
}
