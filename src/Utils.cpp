//
// Created by Валентин Баймаканов on 21.10.2020.
//

#include "Utils.h"

uint16_t toUint16(uint8_t byte1, uint8_t byte2) {
    uint16_t result = byte1;
    result <<= 8u;
    result += byte2;
    return result;
}

std::vector<uint8_t> fromUint16(uint16_t data) {
    std::vector<uint8_t> resultVec;
    auto *dataPtr = (uint8_t*)&data;

    resultVec.push_back(dataPtr[1]);
    resultVec.push_back(dataPtr[0]);

    return resultVec;
}

uint32_t toUint32(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) {
    uint32_t result = byte1;
    result <<= 8u;
    result += byte2;
    result <<= 8u;
    result += byte3;
    result <<= 8u;
    result += byte4;
    return result;
}

std::vector<uint8_t> fromUint32(uint32_t data) {
    std::vector<uint8_t> resultVec;
    auto *dataPtr = (uint8_t*)&data;

    resultVec.push_back(dataPtr[3]);
    resultVec.push_back(dataPtr[2]);
    resultVec.push_back(dataPtr[1]);
    resultVec.push_back(dataPtr[0]);

    return resultVec;
}