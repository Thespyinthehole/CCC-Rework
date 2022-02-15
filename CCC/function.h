#ifndef function_h
#define function_h
#include <vector>

class Function
{
private:
    std::string m_name;
    std::string m_return_type = "void";
    std::vector<std::string> m_params;

public:
    Function() {}
    Function(std::string n, std::vector<std::string> p) : m_name(n), m_params(p) {}
    Function(std::string n, std::string t, std::vector<std::string> p) : m_name(n), m_return_type(t), m_params(p) {}
    std::string name() { return m_name; }
    std::string return_type() { return m_return_type; }
    std::vector<std::string> params() { return m_params; }
};

#endif