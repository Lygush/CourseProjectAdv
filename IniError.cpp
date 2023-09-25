#include <Windows.h>

#include "IniError.h"

IniError::IniError(std::vector<std::string> colors, std::vector<std::string> color_massage) {
    for (const auto& i: color_massage) {
        massage += i;
    }
    this->color_massage = color_massage;
    this->colors = colors;
}

const char* IniError::what() const {
    return massage.c_str();
}

void IniError::set_color(std::string color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (color == "red") {
        SetConsoleTextAttribute(hConsole, 12);
    }
    else if (color == "purple") {
        SetConsoleTextAttribute(hConsole, 13);
    }
    else if (color == "green") {
        SetConsoleTextAttribute(hConsole, 10);
    }
    else if (color == "blue") {
        SetConsoleTextAttribute(hConsole, 9);
    }
    else if (color == "yellow") {
        SetConsoleTextAttribute(hConsole, 6);
    }
    else if (color == "default") {
        SetConsoleTextAttribute(hConsole, 7); 
    }
    else {
        SetConsoleTextAttribute(hConsole, 7); 
    }  
}

void operator<<(const std::ostream&, IniError out) {
    if (out.colors.empty()) {
        std::cout << out.what();
    }
    else {
        for (int i{}; i < out.color_massage.size(); ++i) {
            if (i < out.colors.size()) {
                out.set_color(out.colors.at(i)); // Закоментить для скучного серого
            }
            std::cout << out.color_massage.at(i);
        }
        out.set_color("default"); // Закоментить для скучного серого
    }
}