#ifndef PARSE_IN_H
#define PARSE_IN_H
// #include <bits/stdint-uintn.h>
#include <istream>
#include <vector>
uint32_t parse_u32_be(char *buffer);
uint32_t parse_u32_be(std::istream &stream, uint32_t offset);
uint32_t parse_u32_be(std::istream &stream);
std::vector<char> encode_u32_be(uint32_t n);
uint16_t parse_u16_be(char *buffer);
uint16_t parse_u16_be(std::istream &stream, uint32_t offset);
uint16_t parse_u16_be(std::istream &stream);
std::vector<char> encode_u16_be(uint16_t n);

uint32_t parse_u32_be(std::istream &stream, uint32_t offset) {
  stream.seekg(offset);
  std::vector<char> buffer(4, '\0');
  stream.read(&buffer[0], 4);
  return parse_u32_be(&buffer[0]);
}
uint32_t parse_u32_be(std::istream &stream) {
  std::vector<char> buffer(4, '\0');
  stream.read(&buffer[0], 4);
  return parse_u32_be(&buffer[0]);
}

uint32_t parse_u32_be(char *buffer) {
  return reinterpret_cast<unsigned char *>(buffer)[0] * 256 * 256 * 256 +
         reinterpret_cast<unsigned char *>(buffer)[1] * 256 * 256 +
         reinterpret_cast<unsigned char *>(buffer)[2] * 256 +
         reinterpret_cast<unsigned char *>(buffer)[3];
}
std::vector<char> encode_u32_be(uint32_t n) {
  std::vector<char> r(4, '\0');
  r[0] = n / 256 / 256 / 256;
  r[1] = n / 256 / 256;
  r[2] = n / 256;
  r[3] = n % 256;
  return r;
}
uint16_t parse_u16_be(std::istream &stream, uint32_t offset) {
  stream.seekg(offset);
  std::vector<char> buffer(2, '\0');
  stream.readsome(&buffer[0], 2);
  return parse_u16_be(&buffer[0]);
}
uint16_t parse_u16_be(std::istream &stream) {
  std::vector<char> buffer(2, '\0');
  stream.readsome(&buffer[0], 2);
  return parse_u16_be(&buffer[0]);
}

uint16_t parse_u16_be(char *buffer) {
  return reinterpret_cast<unsigned char *>(buffer)[0] * 256 +
         reinterpret_cast<unsigned char *>(buffer)[1];
}
std::vector<char> encode_u16_be(uint16_t n) {
  std::vector<char> r(2, '\0');
  r[0] = n / 256;
  r[1] = n % 256;
  return r;
}
#endif