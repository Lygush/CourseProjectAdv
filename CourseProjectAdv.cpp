#include <iostream>
#include "IniParser.h"
#include <Windows.h>
#include "IniError.h"

int main() {
    try {
        std::cout << "INI Parser\nEnter the path of the ini file: ";
        std::string path{};
        std::cin >> path;
        Parser test(path);
        std::cout << "Select mode: \n1. Search section. \n2. Search variable in section.\n";
        int mode{};
        std::cin >> mode;
        if (!(mode == 1 || mode == 2)) {
            throw IniError({"red"},{"Error! Invalid mode."});
        }
        while (true) {
            if (mode == 1) {
                std::cout << "Enter section name or \"exit\": ";
                std::string section{};
                std::cin >> section;
                if (section == "exit") {
                    break;
                }
                try {
                    auto result = test.serch_section(section);
                    test.print_section(result, section);
                }
                catch(IniError& ex) {
                    std::cout << ex;
                    std::cout << "\n\n";
                }
            }
            else if (mode == 2) {
                std::cout << "Enter section name or \"exit\": ";
                std::string section{};
                std::cin >> section;
                if (section == "exit") {
                    break;
                }
                std::cout << "Enter value name or \"exit\": ";
                std::string value_name{};
                std::cin >> value_name;
                if (value_name == "exit") {
                    break;
                }
                try {
                    auto result = test.get_value<std::string>(section, value_name);
                    std::cout << value_name << " = " << result << "\n\n";
                }
                catch(IniError& ex) {
                std::cout << ex;
                std::cout << "\n\n";
                }
            } 
        }
    }
    catch(IniError& ex) {
        std::cout << ex;
    }
}