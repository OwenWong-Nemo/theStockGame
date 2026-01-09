#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::string companies10;
    std::ifstream file("assets/companies10.txt");
    if (file.is_open()) {
        while (getline(file, companies10)) {
            std::cout << companies10 << std::endl;
        }
        file.close();
    } else {
        std::cout << "Unable to open file";
    }

    std::cout << companies10 << std::endl;
}
