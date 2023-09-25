#include <fstream>
#include <algorithm>
#include "IniParser.h"
#include "IniError.h"

Parser::Parser(std::string path) {
    std::ifstream in_f(path);
    if (!in_f.is_open()) {
        throw IniError({"red", "yellow"}, {"Error! File is not open.\n", "Check PATH."});
    }
    int line{};
    std::string parse_str{};
    std::string case_name{};
    while (!in_f.eof()) {
        std::getline(in_f, parse_str);
        line++;
        bool error{};
        bool found_key_symbol{}, found_other_symbol{}, semicolon{};
        for (int i{}; i < parse_str.size(); ++i) {
            if (parse_str.at(i) != ' ') {
                if (parse_str.at(i) == ';') {
                    break;
                }
                found_other_symbol = true;
            }
            if (parse_str.at(i) == '[') {
                case_name = parse_name_case(parse_str, line, i);
                found_key_symbol = true;
                break;
            }
            if (parse_str.at(i) == '=') {
                parse_value(parse_str, line, i, case_name);
                found_key_symbol = true;
                break;
            }
        }
        if (!found_key_symbol && found_other_symbol) {
            throw IniError({"red", "yellow", "red", "yellow","red","yellow","red"},
                           {"Error in line: ", std::to_string(line), "\nSymbol", " '[' ", "or ","'=' ",  "not found."});
        }
    }
}

std::string Parser::parse_name_case(std::string& str, int& line, int i) {
    std::string case_name{};
    bool found_key_symbol{};
    for (i; i < str.size(); ++i) {
        if (str.at(i) == ']') {
            found_key_symbol = true;
            break;
        }
        if (str.at(i) == '[') {
            i++;
        }
        case_name += std::tolower( str.at(i));
    }
    if (found_key_symbol) {
        ini[case_name];
        return case_name;
    }
    else {
        throw IniError({"red", "yellow", "red", "yellow","red"},
                       {"Error in line: ", std::to_string(line), "\nSymbol", " ']' ", "not found."});
    }
}   

void Parser::parse_value(std::string& str, int& line, int i, const std::string& case_name) {
    std::string name{}, value{};
    auto begin_to_name{str.begin()};
    auto equal_to_name{str.begin() + i};
    for (begin_to_name; *begin_to_name == ' '; ++begin_to_name) {};
    if (*str.begin() == '=') {
        throw IniError({"red","yellow","red"}, {"Error in line: ", std::to_string(line), "\nVariable name is empty."});
    }
    if (*(equal_to_name - 1) == ' ') {
        for (equal_to_name; *(equal_to_name - 1) == ' '; --equal_to_name) {};
    }
    std::for_each(begin_to_name, equal_to_name, [line, &name](const char& name_char) { 
            if (name_char == ' ') {
            throw IniError({"red", "yellow", "red"}, {"Error in line: ", std::to_string(line), "\nVariable name must not contain spaces."});
            }
            else {
            name += std::tolower(name_char);
            }
    });
    auto equal_to_val{str.begin() + i + 1};
    if (equal_to_val != str.end()) {
        if (*(equal_to_val) == ' ') {
            for (equal_to_val; *(equal_to_val) == ' '; ++equal_to_val) {};
        }
        fill_value(value, str, equal_to_val);
        if (*(value.end() - 1) == ' ') {
            value.erase(value.end() - 1);
        }
    } 
    ini[case_name][name] = value;
}

void Parser::fill_value(std::string& value, std::string& str, std::string::iterator& iterator) {
        bool spases{};
        while (*iterator == ' ') {
            iterator++;
            spases = true;
        }
        if (spases) {
            if (iterator + 1 != str.end()) {
                value += ' ';
            }
        }
        while (iterator != str.end()) {
            if (*iterator == ' ') {
                spases = true;
                break;
            }
            if (*iterator == ';') {
                while (*(value.end() - 1) == ' ') {
                    value.erase(value.end() - 1);
                }
                return;
            }
            value += *iterator;
            iterator++;
        }
        if (iterator == str.end()) {
            spases = false;
        }
        if (spases) {
            fill_value(value, str, iterator);
        }
    }

std::string Parser::get_value_string(std::string section, std::string var) {
    std::transform(section.begin(), section.end(), section.begin(), [](char& to_lower) { to_lower = std::tolower(to_lower); return to_lower;});
    std::transform(var.begin(), var.end(), var.begin(), [](char& to_lower) {to_lower = std::tolower(to_lower); return to_lower;});
    auto iter_case = ini.find(section);
    if (iter_case != ini.end()) {
        auto iter_name = (*iter_case).second.find(var);
        if (iter_name != (*iter_case).second.end()) {
            return (*iter_name).second;
        }
        else {
            throw IniError({"red","yellow","red","blue","purple"},
                           {"Variable ", "\"" + var + "\"", " not found.", "\nMaybe you searching one of this variables:\n",this->section_to_str((*iter_case).second)});
        } 
        }
    else {
        throw IniError({"red","yellow","red"}, {"Section ", "\"" + section + "\"", " not found."});
    }
}

int Parser::str_to_int(std::string& value, std::string& section) {
    bool sign{};
    if (*value.begin() == '-') {
        sign = true;
        value.erase(value.begin());
    }
    for(auto& i: value) {
        if (i < 48 || i > 57) {
            throw IniError({"red", "green", "yellow", "red", "green", "red", "blue", "purple"}, 
                           {"Error! Invalid request.\n", "string ","\"" + value + "\"", " cannot be converted to"," int", ".\n",  "Maybe you searching one of this variables:\n"
                           ,this->section_to_str(this->serch_section(section))});
        }
    }
    if (sign) {
        value.insert(value.begin(), '-');
    }
    try {
        return std::stoi(value);
    }
    catch (std::exception&) {
        throw IniError({"red","yellow","red","green","red","blue","green","blue"},
                       {"Error! Variable value: ", "\"" + value + "\""," does not fit into the type ", "\"int\"", ".\n","Try using ","\"std::string\" ","as a template function parameter."});
    }
}

double Parser::str_to_double(std::string& value, std::string& section) {
    bool sign{}, dot{};
    if (*value.begin() == '-') {
        sign = true;
        value.erase(value.begin());
    }
    for (auto& i: value) {
        if ((i < 48 || i > 57)) {
            if (i == '.' && !dot) {
                dot = true;
            }
            else {
                throw IniError({"red", "green", "yellow", "red", "green", "red", "blue", "purple"}, 
                              {"Error! Invalid request.\n", "string ", "\"" + value + "\"", " cannot be converted to"," double", ".\n",  "Maybe you searching one of this variables:\n"
                              ,this->section_to_str(this->serch_section(section))});
            }
        }
    }
    std::string check{std::to_string(std::stod(value))};
    for (auto i{check.end() - 1}; i != check.begin(); --i) {
        if (*i == '0') {
            check.erase(i);
        }
        else break;
    }
    if (check != value) {
        throw IniError({"red","yellow","red","green","red","blue","green","blue"},
                       {"Error! Variable value: ", "\"" + value + "\""," does not fit into the type ", "\"double\"", ".\n","Try using ","\"std::string\" ","as a template function parameter."});
    }
    return std::stod(value);
}

std::unordered_map<std::string, std::string> Parser::serch_section(std::string& section_name) {
    std::transform(section_name.begin(), section_name.end(), section_name.begin(), [](char& to_lower) { to_lower = std::tolower(to_lower); return to_lower;});
    auto iter_sec = ini.find(section_name);
    if (iter_sec != ini.end()) {
        return (*iter_sec).second;
    }
    else {
        throw IniError({"red","yellow","red"}, {"Section \"", section_name, + "\" not found."});
    }
}

void Parser::print_section(std::unordered_map<std::string, std::string>& section, std::string& section_name) {
    std::cout << "[" << section_name << "]\n";
    for (const auto& i: section) {
        std::cout << i.first << " = " << i.second << "\n";
    }
}

std::string Parser::section_to_str(std::unordered_map<std::string, std::string>& section) {
    std::string section_string{};
    for (const auto& i: section) {
        section_string += i.first + " = " + i.second + "\n";
    }
    return section_string;
}