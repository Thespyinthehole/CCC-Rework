#ifndef type_h
#define type_h

#include <memory>
#include <string>

class Type
{
public:
    virtual std::string to_string() = 0;
};

#define TypePtr std::shared_ptr<Type>

class NoType : public Type
{
public:
    std::string to_string() { return ""; };
};

class CommaType : public Type
{
private:
    TypePtr m_last_type;

public:
    CommaType(TypePtr l) : m_last_type(l) {}
    std::string to_string() { return ", " + m_last_type.get()->to_string(); };
};

class TriangleType : public Type
{
private:
    TypePtr m_last_type;

public:
    TriangleType(TypePtr l) : m_last_type(l) {}
    std::string to_string() { return "<" + m_last_type.get()->to_string() + ">"; };
};

class ColonType : public Type
{
private:
    TypePtr m_last_type;

public:
    ColonType(TypePtr l) : m_last_type(l) {}
    std::string to_string() { return "::" + m_last_type.get()->to_string(); };
};

class BasicType : public Type
{
private:
    std::string m_name;
    TypePtr m_last_type;

public:
    BasicType(std::string n, TypePtr l) : m_name(n), m_last_type(l) {}
    std::string to_string()
    {
        return m_name + m_last_type.get()->to_string();
    };
};
#endif