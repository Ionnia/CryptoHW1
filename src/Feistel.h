#include <string>
#include <vector>

#include "Utils.h"

#ifndef CRYPTOHW1_CRYPTOFUNCTIONGENERATOR_H
#define CRYPTOHW1_CRYPTOFUNCTIONGENERATOR_H

enum OperationType {
    XOR,
    SHL,
    SPLIT
};

struct Operation {
    OperationType type;
    uint8_t value;
    Operation(OperationType type, uint8_t value) {
        this->type = type;
        this->value = value;
    }
};

class Feistel {
public:
    Feistel(std::vector<Operation> operations);
    std::vector<uint8_t> encrypt(std::vector<uint8_t> data, uint32_t key);
private:
    uint32_t feistel(uint32_t data, uint32_t key, int numberOfIterations);
    uint16_t roundFunction(uint16_t data, uint16_t key);
    uint16_t shuffleKey(uint16_t key);
    uint16_t applyOperation(Operation operation, uint16_t data, uint16_t key);

    std::vector<Operation> operations;

    static const uint8_t BLOCK_SIZE_BITS = 16u;
    static const uint8_t ROUND_FUNCTION_ITERATIONS = 7u;
};

#endif //CRYPTOHW1_CRYPTOFUNCTIONGENERATOR_H
