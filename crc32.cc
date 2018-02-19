//
// Integrity Descriptor
//

#include "crc32.hh"

#include <iostream>
#include <vector>

const uint32_t POLYNOMIAL = 0x04C11DB7;

static uint32_t lut[8][256];

namespace integrityd
{
namespace crc32
{
void init_table()
{
    for (size_t i = 0; i < 256; ++i)
    {
        uint32_t crc = i;
        for (size_t j = 0; j < 8; ++j)
        {
            crc = (crc >> 1) ^ (-static_cast<uint32_t>(crc & 1) & POLYNOMIAL);
        }
        lut[0][i] = crc;
    }

    for (size_t i = 0; i < 256; ++i)
    {
        lut[1][i] = (lut[0][i] >> 8) ^ lut[0][lut[0][i] & 0xFF];
        lut[2][i] = (lut[1][i] >> 8) ^ lut[0][lut[1][i] & 0xFF];
        lut[3][i] = (lut[2][i] >> 8) ^ lut[0][lut[2][i] & 0xFF];
        lut[4][i] = (lut[3][i] >> 8) ^ lut[0][lut[3][i] & 0xFF];
        lut[5][i] = (lut[4][i] >> 8) ^ lut[0][lut[4][i] & 0xFF];
        lut[6][i] = (lut[5][i] >> 8) ^ lut[0][lut[5][i] & 0xFF];
        lut[7][i] = (lut[6][i] >> 8) ^ lut[0][lut[6][i] & 0xFF];
    }
}

static inline uint32_t calculate_chunk(const uint32_t *buffer, size_t length, uint32_t previous)
{
    uint32_t crc = ~previous;
    uint32_t one;
    uint32_t two;

    while (length >= 8)
    {
        one = *buffer++ ^ crc;
        two = *buffer++;
        crc = lut[7][one  &  0xFF] ^
              lut[6][(one >> 8)  & 0xFF] ^
              lut[5][(one >> 16) & 0xFF] ^
              lut[4][one  >> 24] ^
              lut[3][two  &  0xFF] ^
              lut[2][(two >> 8)  & 0xFF] ^
              lut[1][(two >> 16) & 0xFF] ^
              lut[0][two  >> 24];
        length -= 8;
    }

    const uint8_t *buffer_byte_iterator = (const uint8_t *) buffer;
    while(length--)
    {
        crc = (crc >> 8) ^ lut[0][(crc & 0xFF) ^ *buffer_byte_iterator++];
    }

    return ~crc;
}

//TODO something here is still not right
std::string calculate(const fs::path &target, size_t chunk_size)
{
    std::vector<uint32_t> buffer;
    buffer.resize(chunk_size / 4);

    std::ifstream fin(target.c_str(), std::ios::binary | std::ios::in);

    uint32_t crc = 0;
    if (fin)
    {
        while (fin.good())
        {
            fin.read((char *) buffer.data(), chunk_size);
            crc = calculate_chunk(buffer.data(), fin.gcount(), crc);
        }
    }

    fin.close();

    return std::to_string(crc);
}
}
}
