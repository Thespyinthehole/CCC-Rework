#ifndef semantic_error_h
#define semantic_error_h
#include <string>

class SemanticError
{
public:
    static int m_count;

    static void throw_error(std::string message)
    {
        printf("Error: %s\n", message.c_str());
        m_count++;
    }

    static int count()
    {
        return m_count;
    }
};


class SemanticWarning
{
public:
    static int m_count;

    static void throw_warning(std::string message)
    {
        printf("Warning: %s\n", message.c_str());
        m_count++;
    }

    static int count()
    {
        return m_count;
    }
};

#endif