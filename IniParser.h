#pragma once

#include <unordered_map>
#include <string>

class Parser {
    std::unordered_map<std::string, std::unordered_map<std::string, std::string> > ini{}; 
public:                                                                                    
    ~Parser() = default;
    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

    Parser(std::string path);

private:
    std::string parse_name_case(std::string& str, int& line, int i);

    void parse_value(std::string& str, int& line, int i, const std::string& case_name);

    void fill_value(std::string& value, std::string& str, std::string::iterator& iterator);

public:
    template<class T> 
    T get_value(std::string section, std::string var) {
        static_assert (sizeof(T) == -1, "not implemented type for get_value");
    }

    template<>
    std::string get_value(std::string section, std::string var) {
        return get_value_string(section, var);
    }

    template<>
    int get_value(std::string section, std::string var) {
        std::string value{get_value_string(section, var)};
        return str_to_int(value, section);
    }

    template<>
    double get_value(std::string section, std::string var) {
        std::string value{get_value_string(section,var)};
        return str_to_double(value, section);
    }

    std::unordered_map<std::string, std::string> serch_section(std::string& section_name);
    void print_section(std::unordered_map<std::string, std::string>& section, std::string& section_name);

private:
    std::string get_value_string(std::string section, std::string var);
    int str_to_int(std::string& value, std::string& section);
    double str_to_double(std::string& value, std::string& section);

    std::string section_to_str(std::unordered_map<std::string, std::string>& section);
};