#include <iostream>
#include <fstream>
#include <regex>
#include <string>


#include "include/formater.hpp"

bool formater_ended =false;

int formater(void) {
    std::ifstream inputFile("txts/input.txt");
    std::ofstream outputFile("txts/output.txt");

    formater_ended = false;

    if (!inputFile || !outputFile) {
        std::cerr << "Error abriendo los archivos." << std::endl;
        return 1;
    }

    std::string line;
    std::regex credentialsRegex(R"(([^:]+):\/\/[^\/]+\/[^:]+:([^:]+):(.+))");  // Extrae user:pass
    std::regex validUserRegex(R"(^[A-Za-z0-9._@+-]+$)"); // Caracteres válidos para un email user

    while (std::getline(inputFile, line)) {
        std::smatch match;
        if (std::regex_search(line, match, credentialsRegex)) {
            std::string user = match[2];
            std::string pass = match[3];

            if (std::regex_match(user, validUserRegex)) {
                outputFile << user << ":" << pass << std::endl;
            }
        }
    }

    inputFile.close();
    outputFile.close();

    std::cout<<"formater finalizado"<<std::endl;
    formater_ended = true;
    return 0;
}
