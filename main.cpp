#include <iostream>
#include <vector>
#include <fstream>

struct Input {
    std::string filename;
    int numbilets;
    int parameter;
};

Input* processArgs(int argc, char** argv);
std::vector<std::string> readFile(std::string filename);

int main(int argc, char** argv) {
    try {
        auto input = processArgs(argc, argv);
        std::cout << "Filename: " << input->filename << std::endl;
        std::cout << "Numbilets: " << input->numbilets << std::endl;
        std::cout << "Parameter: " << input->parameter << std::endl;

        std::vector<std::string> studentNamesList = readFile(input->filename);

        std::cout << "Students" << std::endl;
        for(auto str : studentNamesList) {
            std::cout << str << std::endl;
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