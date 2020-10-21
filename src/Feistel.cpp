#include "Feistel.h"
#include <iostream>

Feistel::Feistel(std::vector<Operation> operations) {
    this->operations = std::move(operations);
}

std::vector<uint8_t> Feistel::encrypt(std::vector<uint8_t> data, uint32_t key) {
    const uint8_t NUMBER_OF_BYTES = 4;
    // Дополняем последний блок данных
    uint32_t dataSize = data.size();
    if(dataSize % NUMBER_OF_BYTES != 0) {
        for(int i = 0; i < NUMBER_OF_BYTES - (dataSize % NUMBER_OF_BYTES); ++i) {
            if(dataSize > 0) {
                uint8_t completionBlock = data[0];
                for(int j = 1; j < dataSize && j < 10; ++j) {
                    completionBlock ^= data[j];
                }
                data.push_back(completionBlock ^ (170u * (i + 1)));
            } else {
                data.push_back(170u * (i + 1)); // 10101010 in binary
            }
        }
    }

    std::vector<uint8_t> encrypted;
    uint32_t encBlock = 0u; // Вектор инициализации 1010101010...
    for(int i = 0; i < data.size(); i += NUMBER_OF_BYTES) {
        encBlock = feistel(encBlock ^ toUint32(data[i], data[i+1], data[i+2], data[i+3]), key, ROUND_FUNCTION_ITERATIONS);
        std::vector<uint8_t> encBlockVec = fromUint32(encBlock);

        encrypted.push_back(encBlockVec[0]);
        encrypted.push_back(encBlockVec[1]);
        encrypted.push_back(encBlockVec[2]);
        encrypted.push_back(encBlockVec[3]);
    }

    return encrypted;
}

uint32_t Feistel::feistel(uint32_t data, uint32_t key, int numberOfIterations) {
    uint32_t roundKey = key;

    auto *dataParts = (uint16_t*)&data;
    auto *roundKeyParts = (uint16_t*)&roundKey;

    for(int i = 0; i < numberOfIterations; ++i) {
        roundKey = key ^ data;
        auto newLeftPart = dataParts[1];
        auto newRightPart = roundFunction(dataParts[0], roundKeyParts[i % 2]) ^ dataParts[1];
        dataParts[0] = newLeftPart;
        dataParts[1] = newRightPart;
    }

    return data;
}

uint16_t Feistel::roundFunction(uint16_t data, uint16_t key) {
    for(Operation operation : Feistel::operations) {
        data = applyOperation(operation, data, key);
        key = shuffleKey(key);
    }
    return data;
}

uint16_t Feistel::shuffleKey(uint16_t key) {
    key = applyOperation(Operation(SPLIT, 5), key, 0);
    key = applyOperation(Operation(SHL, 3), key, 0);

    return key;
}

uint16_t Feistel::applyOperation(Operation operation, uint16_t data, uint16_t key) {
    switch (operation.type) {
        case XOR:
            return data ^ key;
        case SHL: {
            uint8_t value = operation.value % Feistel::BLOCK_SIZE_BITS;
            uint16_t leftPart = data >> (uint8_t) (Feistel::BLOCK_SIZE_BITS - value);
            uint16_t rightPart = data << value;
            return rightPart + leftPart;
        }
        case SPLIT: {
            uint8_t shiftRight = operation.value % Feistel::BLOCK_SIZE_BITS;
            uint8_t shiftLeft = Feistel::BLOCK_SIZE_BITS - shiftRight;
            uint16_t newRightSide = data >> shiftRight;
            uint16_t newLeftSide = data << shiftLeft;
            return newLeftSide + newRightSide;
        }
        default:
            throw std::invalid_argument("Неизвестная операция в сети Фейстеля");
    }
}