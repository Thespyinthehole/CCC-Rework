#include "production.h"

bool variable_exists(Variables vars, Variable var)
{
    for (Variable v : vars)
        if (v.first == var.first)
        {
            if (v.second != var.second)
                SemanticError::throw_error("Variable " + v.first + " has been declared with different types");
            return true;
        }
    return false;
}