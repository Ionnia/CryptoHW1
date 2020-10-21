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
    uint32_t encBlock = 2863311530u; // Вектор инициализации 1010101010...
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
    auto *dataParts = (uint16_t*)&data;
    auto *keyParts = (uint16_t*)&key;

    for(int i = 0; i < numberOfIterations; ++i) {
        auto newLeftPart = dataParts[1];
        auto newRightPart = roundFunction(dataParts[0], keyParts[i % 2]) ^ dataParts[1];
        dataParts[0] = newLeftPart;
        dataParts[1] = newRightPart;
    }

    return data;
}

uint16_t Feistel::roundFunction(uint16_t data, uint16_t key) {
    for(Operation operation : Feistel::operations) {
        data = applyOperation(operation, data, key);
    }
    return data;
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

// 53432 42 |53405 0 |39413 42 |55069 0 |14545 42 |46650 0 |52134 42 |1605 42 |49186 0 |63393 42 |59073 0 |9018 42 |17353 0 |52184 42 |65473 42 |1950 0 |62049 42 |15714 0 |6299 42 |43397 0 |61705 42 |4877 42 |29602 0 |57387 42 |12562 0 |12770 42 |47354 0 |23778 42 |39278 42 |55360 0 |31244 42 |23136 0 |18781 42 |31088 0 |49876 42 |49721 42 |48985 0 |46776 42 |6759 0 |21308 42 |24753 0 |60312 42 |776 42 |59533 0 |31116 42 |16248 0 |60484 42 |20571 0 |26413 42 |39485 42 |6571 0 |4710 42 |51178 0 |57827 42 |5248 0 |61552 42 |24770 42 |19896 0 |14728 42 |58798 0 |13490 42 |58304 0 |45018 42 |40725 42 |26238 0 |63921 42 |50818 0 |2942 42 |18973 0 |57368 42 |15877 42 |10111 0 |12512 42 |36723 0 |60011 42 |21420 0 |29529 42 |3910 42 |52558 0 |31561 42 |56367 0 |28050 42 |31356 0 |42570 42 |
// 53432 42 |53405 0 |39413 42 |55069 0 |14545 42 |46650 0 |52134 42 |1605 42 |49186 0 |63393 42 |59073 0 |9018 42 |17353 0 |52184 42 |65473 42 |1950 0 |62049 42 |15714 0 |6299 42 |43397 0 |61705 42 |4877 42 |29602 0 |57387 42 |12562 0 |12770 42 |47354 0 |23778 42 |39278 42 |55360 0 |31244 42 |23136 0 |18781 42 |31088 0 |49876 42 |49721 42 |48985 0 |46776 42 |6759 0 |21308 42 |24753 0 |60312 42 |776 42 |59533 0 |31116 42 |16248 0 |60484 42 |20571 0 |26413 42 |39485 42 |6571 0 |4710 42 |51178 0 |57827 42 |5248 0 |61552 42 |24770 42 |19896 0 |14728 42 |58798 0 |13490 42 |58304 0 |45018 42 |40725 42 |26238 0 |63921 42 |50818 0 |2942 42 |18973 0 |57368 42 |15877 42 |10111 0 |12512 42 |36723 0 |60011 42 |21420 0 |29529 42 |3952 42 |52558 0 |24649 42 |50991 0 |63135 42 |24956 0 |15239 42 |