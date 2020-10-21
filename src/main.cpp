#include <iostream>
#include <vector>
#include <fstream>
#include "Feistel.h"

struct Input {
    std::string filename;
    int numbilets;
    int parameter;
};

Input* processArgs(int argc, char** argv);
std::vector<std::string> readFile(std::string filename);
std::vector<Operation> initializeRoundFunction();

int main(int argc, char** argv) {
    try {
        auto input = processArgs(argc, argv);

        std::vector<std::string> studentNamesList = readFile(input->filename);

        Feistel feistel(initializeRoundFunction());
        for(auto str : studentNamesList) {
            std::vector<uint8_t> vec(str.begin(), str.end());
            std::vector<uint8_t> encrypted = feistel.encrypt(vec, input->parameter);
            std::string encStr(encrypted.begin(), encrypted.end());
            uint32_t encSize = encrypted.size();
            uint32_t hash = toUint32(encrypted[encSize-4], encrypted[encSize-3], encrypted[encSize-2], encrypted[encSize-1]);
            std::cout << str << ": " << hash % input->numbilets + 1 << std::endl;
        }
    } catch(std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

Input* processArgs(int argc, char** argv) {
    if(argc != 7) {
        throw std::invalid_argument("Слишком мало аргументов");
    }
    Input *input = new Input();

    for(int i = 0; i < 3; i++) {
        char *command = argv[i * 2 + 1];
        char *arg = argv[i * 2 + 2];
        if(strcmp(command, "--file") == 0) {
            input->filename = std::string(arg);
        } else if(strcmp(command, "--numbilets") == 0) {
            input->numbilets = std::stoi(arg);
        } else if(strcmp(command, "--parameters") == 0) {
            input->parameter = std::stoi(arg);
        } else {
            throw std::invalid_argument("Неверные аргументы");
        }
    }

    return input;
}

std::vector<std::string> readFile(std::string filename) {
    std::ifstream file;
    file.open(filename);
    if(!file.is_open()) {
        throw std::invalid_argument("Файл не был найден!");
    }

    std::vector<std::string> fileLines;

    std::string line;
    while(std::getline(file, line)) {
        fileLines.push_back(line);
    }

    return fileLines;
}

std::vector<Operation> initializeRoundFunction() {
    std::vector<Operation> operations;

    operations.emplace_back(XOR, 0);
    operations.emplace_back(SHL, 7);
    operations.emplace_back(XOR, 0);
    operations.emplace_back(SHL, 3);
    operations.emplace_back(XOR, 0);
    operations.emplace_back(SPLIT, 11);
    operations.emplace_back(XOR, 0);
    operations.emplace_back(SPLIT, 5);

    return operations;
}