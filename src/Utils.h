//
// Created by Валентин Баймаканов on 21.10.2020.
//
#include <cstdint>
#include <vector>

#ifndef CRYPTOHW1_UTILS_H
#define CRYPTOHW1_UTILS_H

uint16_t toUint16(uint8_t byte1, uint8_t byte2);

std::vector<uint8_t> fromUint16(uint16_t data);

uint32_t toUint32(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4);

std::vector<uint8_t> fromUint32(uint32_t data);

#endif //CRYPTOHW1_UTILS_H
