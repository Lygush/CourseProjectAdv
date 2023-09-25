#pragma once

#include <exception>
#include <string>
#include <vector>
#include <iostream>

class IniError : public std::exception {
    std::string massage{};
    std::vector<std::string> color_massage{};
    std::vector<std::string> colors{};
public:
    IniError(std::string massage) : massage(massage) {}

    IniError(std::vector<std::string> colors, std::vector<std::string> color_massage);

    const char* IniError::what() const;

    void set_color(std::string color);

    friend void operator<<(const std::ostream&, IniError out);
};