#ifndef production_h
#define production_h
#include "semantic_error.h"
#include <memory>
#include <set>
#include <map>
#include "function.h"
#define StrSet std::set<std::string>
#define Variable std::pair<std::string, std::string>
#define Variables std::vector<Variable>

bool variable_exists(Variables vars, Variable var);


class Production;
#define Prod std::shared_ptr<Production>

struct FullProduction
{
    Prod production;
    Function function;
};

class Production
{
public:
    virtual std::string to_string(StrSet, StrSet) = 0;
    virtual void get_variables(Variables &, StrSet, std::map<std::string, FullProduction>) = 0;
};

class NamedProduction : public Production
{
private:
    std::string m_name;
    std::string m_variable = "";

public:
    NamedProduction(std::string n) : m_name(n) {}
    NamedProduction(std::string n, std::string v) : m_name(n), m_variable(v) {}

    std::string name() { return m_name; }

    std::string to_string(StrSet tokens, StrSet productions)
    {
        if (tokens.find(m_name) != tokens.end())
		{
            if (m_variable == "")
                return "token_" + m_name;
			 else 
                return "(_token_location_" + m_variable + " << token_" + m_name + " >>  token_function_" + m_variable + ")";
			}
        if (productions.find(m_name) != productions.end())
            return "ParseTree(production_" + m_name + ")";
        SemanticError::throw_error("Name " + m_name + " is not defined");
        return "";
    }

    void get_variables(Variables &vars, StrSet tokens, std::map<std::string, FullProduction> productions)
    {
        Variable v = {m_variable, tokens.find(m_name) != tokens.end() ? "Token" : productions[m_name].function.return_type()};
        if (m_variable != "")
        {
            if (!variable_exists(vars, v))
                vars.push_back(v);
        }
    }
};

class BracketProduction : public Production
{
private:
    Prod m_production;

public:
    BracketProduction(Prod p) : m_production(p) {}
    Prod production() { return m_production; }

    std::string to_string(StrSet tokens, StrSet productions)
    {
        return "(" + m_production->to_string(tokens, productions) + ")";
    }

    void get_variables(Variables &vars, StrSet tokens, std::map<std::string, FullProduction> productions)
    {
        m_production->get_variables(vars, tokens, productions);
    }
};

class OrProduction : public Production
{
private:
    Prod m_rhs;
    Prod m_lhs;
    bool m_set = false;

public:
    OrProduction(Prod p) : m_rhs(p) {}
    Prod rhs() { return m_rhs; }
    Prod lhs() { return m_lhs; }
    bool set() { return m_set; }

    void set_lhs(Prod p)
    {
        m_lhs = p;
        m_set = true;
    }

    std::string to_string(StrSet tokens, StrSet productions)
    {
        return m_lhs->to_string(tokens, productions) + " | " + m_rhs->to_string(tokens, productions);
    }

    void get_variables(Variables &vars, StrSet tokens, std::map<std::string, FullProduction> productions)
    {
        m_lhs->get_variables(vars, tokens, productions);
        m_rhs->get_variables(vars, tokens, productions);
    }
};

class NoProduction : public Production
{
public:
    std::string to_string(StrSet, StrSet)
    {
        return "";
    }

    void get_variables(Variables &vars, StrSet tokens, std::map<std::string, FullProduction> productions) {}
};

class EpsilonProduction : public Production
{
private:
    std::string m_variable = "";

public:
    EpsilonProduction() {}
    EpsilonProduction(std::string v) : m_variable(v) {}

    std::string to_string(StrSet, StrSet)
    {
        return "epsilon";
    }

    void get_variables(Variables &vars, StrSet tokens, std::map<std::string, FullProduction> productions)
    {
        if (m_variable != "")
            vars.push_back({m_variable, "Token"});
    }
};

class SequenceProduction : public Production
{
private:
    Prod m_rhs;
    Prod m_lhs;

public:
    SequenceProduction(Prod l, Prod r) : m_lhs(l), m_rhs(r) {}
    Prod rhs() { return m_rhs; }
    Prod lhs() { return m_lhs; }

    std::string to_string(StrSet tokens, StrSet productions)
    {
        return m_lhs->to_string(tokens, productions) + " + " + m_rhs->to_string(tokens, productions);
    }

    void get_variables(Variables &vars, StrSet tokens, std::map<std::string, FullProduction> productions)
    {
        m_lhs->get_variables(vars, tokens, productions);
        m_rhs->get_variables(vars, tokens, productions);
    }
};

#endif