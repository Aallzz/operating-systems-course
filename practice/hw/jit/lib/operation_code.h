#ifndef MY_OP_CODES
#define MY_OP_CODES

#include <vector>
#include <cstdint>
#include <functional>

namespace operation_code {
  
  enum class xmm_operation {
    plus = 0x58,
    minus = 0x5C,
    mult = 0x59,
    div = 0x5E  
  };
  
const std::function<std::vector<uint8_t>(uint8_t const)> 
push_xmm = [](uint8_t const idx) {
  return std::vector<uint8_t>{0x48, 0x83, 0xEC, 0x10, 
                              0xF3, 0x0F, 0x7F, (uint8_t)(0x04 + idx * 8), 0x24};
};

const std::function<std::vector<uint8_t>(uint8_t const)> 
pop_xmm = [](uint8_t const idx) {
  return std::vector<uint8_t>{0xF3, 0x0F, 0x6F, (uint8_t)(0x04 + idx * 8), 0x24, 
                              0x48, 0x83, 0xC4, 0x10};
};

const std::function<std::vector<uint8_t>(operation_code::xmm_operation const, uint8_t const, uint8_t const)>
op_xmm = [](xmm_operation const op, uint8_t const idx1, uint8_t const idx2) {
  return std::vector<uint8_t>{0xF2, 0x0F, static_cast<uint8_t>(op), (uint8_t)(0xC0 + idx1 * 8 + idx2)};
};

const std::function<std::vector<uint8_t>(uint8_t const, uint32_t const)>
load_xmm = [](uint8_t const idx, uint32_t const src) {
  uint8_t part1 = (src & 0xFF);
  uint8_t part2 = (src & 0xFF00) >> 8;
  uint8_t part3 = (src & 0xFF0000) >> 16;
  uint8_t part4 = (src & 0xFF000000) >> 24;
  return std::vector<uint8_t>{0x66, 0x0F, 0x6F, (uint8_t)(0x81 + idx * 8), part1, part2, part3, part4};
};

const std::function<std::vector<uint8_t>(uint8_t const, uint8_t const)>
mov_xmm_idx1_from_idx2 = [](uint8_t const idx1, uint8_t const idx2) {
  return std::vector<uint8_t>{0x66, 0x0F, 0x28, (uint8_t)(0xC0 + idx1 * 8 + idx2)}; 
};

const std::function<std::vector<uint8_t>(int32_t const)>
lea_rcx_rip_p = [](uint32_t const offset ) {
  uint8_t part1 = (offset & 0xFF);
  uint8_t part2 = (offset & 0xFF00) >> 8;
  uint8_t part3 = (offset & 0xFF0000) >> 16;
  uint8_t part4 = (offset & 0xFF000000) >> 24;
  return std::vector<uint8_t>{0x48, 0x8D, 0x0D, part1, part2, part3, part4}; 
};

const std::vector<uint8_t> ret = {0xC3};

};

#endif
