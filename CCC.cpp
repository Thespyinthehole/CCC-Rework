#include "CCC/grammar.h"
#include <fstream>
#include <iostream>
typedef std::pair<std::string, std::string> NamedPair;
typedef std::vector<std::string> StrVector;
typedef std::pair<std::string, FullProduction> ProductionPair;
// Stores infomation needed about the token
struct TokenData
{
    std::string regex;
    bool ignored;
};

// Stores infomation about functions and the variables they use
struct FuncVar
{
    Function func;
    Variables vars;
};

// Set the error count to 0
int SemanticError::m_count = 0;
int SemanticWarning::m_count = 0;

// Stores a list of both <> and "" types of include names
StrVector to_include_list;

// Stores name(key) and regex(value) of each token
std::vector<std::pair<std::string, TokenData>> token_vector;

// Stores the names of the tokens that have been used
StrSet token_names;

// Stores a list of productions
std::map<std::string, FullProduction> production_map;

// Stores the names of the productions that have been used
StrSet production_names;

// Stores the names of the functions that have been declared
std::vector<FuncVar> function_list;

// When we see a named token(< name >)
std::string on_named_token(Token name)
{
    // Return the value of the name
    return name.value;
}

// When we have included a name with the triangle brackets
std::string on_include_triangle(std::string name)
{
    // Re add the triangle brackets around the name
    return "<" + name + ">";
}

// When we have included a string ("blah")
std::string on_include_string(Token name)
{
    // We can just return the value as the regex extracts the ""
    return name.value;
}

// When we have included either a string or triangle name
void on_include_statement(std::string to_include)
{
    // Add that include to the list
    to_include_list.push_back(to_include);
}

// Add a token to the lists
void add_new_token(std::string name, Token value, bool ignore)
{
    // Check if the token has already been defined
    if (token_names.find(name) != token_names.end())
    {
        SemanticError::throw_error("Name " + name + " has been defined more than once");
        return;
    }

    // Add that token to the list
    token_vector.push_back({name, {value.value, ignore}});

    // Add the name
    token_names.insert(name);
}

// When a token has been declared
void on_token_declaration(std::string name, Token value)
{
    // Add the token
    add_new_token(name, value, false);
}

// When an ignored token has been declared
void on_ignored_token_declaration(std::string name, Token value)
{
    // Add the token
    add_new_token(name, value, true);
}

// When we check if this is ignored
bool on_ignored_token(Token is_ignore)
{
    // Return whether this token exists
    return is_ignore.hasNext;
}

// When a production has been finished
void on_production(std::string name, Prod production, Function function)
{
    // Check if the production has already been defined
    if (token_names.find(name) != token_names.end() || production_names.find(name) != production_names.end())
    {
        SemanticError::throw_error("Name " + name + " has been defined more than once");
        return;
    }

    // Add the production
    production_map.insert({name, {production, function}});

    // Add the name
    production_names.insert(name);
}

// When we get a token/production which has a variable attached
NamedPair on_variable_named(Token var, std::string name)
{
    // Return a link between then
    return {name, var.value};
}

// When we get a token/production which doesnt have a variable
NamedPair on_non_variable_named(std::string name)
{
    // Return a link of just the name
    return {name, ""};
}

// If we reach an epsilon
NamedPair on_not_variabled_named()
{
    // Return a link to an epsilon
    return {"ε", ""};
}

// When we reach a bracketless production expression
Prod on_non_bracketed_production_expression(NamedPair var)
{
    // If we used an ε return an epsilon production
    if (var.first == "ε")
        return Prod(new EpsilonProduction(var.second));

    // Return a named production
    return Prod(new NamedProduction(var.first, var.second));
}

// When we have reach a production section
Prod on_production_expression(Prod production, Prod extra)
{
    // Do I have anything extra to add
    if (dynamic_cast<NoProduction *>(extra.get()))
        return production;

    // Am I part of an or block
    if (!dynamic_cast<OrProduction *>(extra.get()) || ((OrProduction *)extra.get())->set())
    {
        // Warning if brackets are not used
        if (dynamic_cast<OrProduction *>(production.get()))
            SemanticWarning::throw_warning("Do not mix or and sequences without brackets");

        // If no I must be a sequence
        return Prod(new SequenceProduction(production, extra));
    }

    // Warning if brackets are not used
    if (dynamic_cast<SequenceProduction *>(production.get()))
        SemanticWarning::throw_warning("Do not mix or and sequences without brackets");

    // Set the left side of the or and return the or production
    ((OrProduction *)extra.get())->set_lhs(production);
    return extra;
}

// When reach a bracketed production
Prod on_bracketed_production_expression(Prod production)
{
    // Return that production but inside brackets
    return Prod(new BracketProduction(production));
}

// When I have reach an or production
Prod on_or_production_expression(Prod production)
{
    // Warning if brackets are not used
    if (dynamic_cast<SequenceProduction *>(production.get()))
        SemanticWarning::throw_warning("Do not mix or and sequences without brackets");

    // Production is the rhs of the or so set the rhs and return an or
    return Prod(new OrProduction(production));
}

// When I have nothing to include in the extra section
Prod on_no_production_expression()
{
    // Return the end of extra
    return Prod(new NoProduction);
}

// When I have got an extra production
Prod on_extra_production(Prod production)
{
    // Return it
    return production;
}

// When we dont have a function
Function on_no_production_function()
{
    // Return an empty function
    return Function("", {});
}

// When we reach a generic type
TypePtr on_triangle_type(TypePtr last_type)
{
    // Return a generic Type
    return TypePtr(new TriangleType(last_type));
}

// When we reach a comma type
TypePtr on_comma_type(TypePtr last_type)
{
    // Return a comma type
    return TypePtr(new CommaType(last_type));
}

// When we reach a namespace type
TypePtr on_colon_type(TypePtr last_type)
{
    // Return a namespace type
    return TypePtr(new ColonType(last_type));
}

// When we reached the end of types
TypePtr on_no_type()
{
    // Retun no type
    return TypePtr(new NoType);
}

// When we have an extra type, just pass it up
TypePtr on_extra_type(TypePtr last_type)
{
    return last_type;
}

// When we have a named type
TypePtr on_type(Token type, TypePtr last_type)
{
    // Return that named type
    return TypePtr(new BasicType(type.value, last_type));
}

// When we finished a type, just pass it up
TypePtr on_return_type(TypePtr type)
{
    return type;
}

// When we finish a function
Function on_production_function(Token name, TypePtr type, StrVector params)
{
    // If it has a return type
    if (dynamic_cast<NoType *>(type.get()))
        // Return a void function
        return Function(name.value, params);
    // Return a function which returns that type
    return Function(name.value, type->to_string(), params);
}

// When we have finished the params list
StrVector on_production_function_params_end()
{
    // Return a vector to be filled
    return {};
}

// When we finish filling the vector, just pass it up
StrVector on_production_function_params(StrVector current_params)
{
    return current_params;
}

// When we reach the next param
StrVector on_production_function_params_next(Token name, StrVector current_params)
{
    // Add the param to the list
    current_params.insert(current_params.begin(), name.value);
    // Pass the list up
    return current_params;
}

// When we reach the start of the params, just pass up the list
StrVector on_production_function_params_start(StrVector current_params)
{
    return current_params;
}

std::string filename;
// Changes , :: < and > into a different number of underscores so it can be used as a unique name
std::string flatten_type(std::string type)
{
    std::string output = "";
    for (char chr : type)
        if (chr == ',')
            output += "___";
        else if (chr == ' ')
            output += "";
        else if (chr == ':')
            output += "_";
        else if (chr == '<')
            output += "_";
        else if (chr == '>')
            output += "_";
        else
            output += chr;
    return output;
}

// Read a string from a file name given
std::string get_program_string(std::string filename)
{
    std::ifstream t(filename + ".ccc");
    std::string str;

    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    return str;
}

// Searches vars for a type if the variable name matches
std::string get_type(Variables vars, std::string var_name)
{
    for (Variable var : vars)
        if (var.first == var_name)
            return var.second;

    return "void";
}

// Write the header file
void write_header(std::string filename)
{
    // Open the header file
    std::ofstream file(filename + ".h");

    // Write the header guard
    file << "#ifndef " << filename.substr(filename.find_last_of("/\\") + 1) << "_h\n#define " << filename.substr(filename.find_last_of("/\\") + 1) << "_h\n\n";

    // Add all the includes specificed
    for (std::string include : to_include_list)
        file << "#include " << include << "\n";

    // Add spacer
    file << "\n";

    // Write the tokens
    if (token_vector.size() > 0)
    {
        // Start the token enum
        file << "enum Tokens : int {\n\ttoken_end_of_field = 1,\n";

        // Loop over each token - map is ordered
        int token_count = 0;

        for (std::pair<std::string, TokenData> token : token_vector)
            // Add the token to the enum
            file << "\ttoken_" << token.first << " = " << token_count-- << ",\n";

        // Finish the enum
        file << "};\n\n";
    }

    // Write the definition of the functions
    if (function_list.size() > 0)
    {
        // Write the function
        for (FuncVar func : function_list)
        {
            // Write the function type and name
            file << func.func.return_type() << " " << func.func.name() << "(";

            // Write the parameter types and names
            for (int i = 0; i < func.func.params().size(); i++)
            {
                if (i != 0)
                    file << ", ";
                file << get_type(func.vars, func.func.params()[i]) << " " << func.func.params()[i];
            }
            file << ");\n";
        }

        // Add a spacer
        file << "\n";
    }

    // Add a parse function to the header file
    file << "void parse(std::string);\n\n";

    // Finish the header guard
    file << "#endif";

    // Close the file
    file.close();

    printf("Created %s.h\n", filename.c_str());
}

// Write the implementation c++ file
void write_implementation(std::string filename)
{
    // Open the c++ file
    std::ofstream file(filename + ".cpp");

    // Write the includes for the file
    file << "#include \"" << filename.substr(filename.find_last_of("/\\") + 1) << ".h\"\n#include <stack>\n\n";

    // Find all the types that can be returned
    StrSet returnable_values;

    for (FuncVar func : function_list)
        if (func.func.return_type() != "void")
            returnable_values.insert(func.func.return_type());

    // If any returnable exist
    if (returnable_values.size() > 0)
    {
        for (std::string return_value : returnable_values)
            file << "std::stack<" << return_value << "> stack_" << flatten_type(return_value) << ";\n";
        // Add a spacer
        file << "\n";
    }

    // Find all the variables that has been made
    std::set<Variable> assigned_variables;

    // Populate the assigned_variables with all the types of variables that have been defined
    for (ProductionPair production : production_map)
    {
        Variables vars;
        production.second.production->get_variables(vars, token_names, production_map);
        for (Variable var : vars)
            assigned_variables.insert(var);
    }

    // If any variables have be declared
    if (assigned_variables.size() > 0)
    {
        // Make a stack for tokens
        file << "std::stack<Token> _stack_token;\n\n";

        // Add a function to add the token to that stack
        for (Variable var : assigned_variables)
            if (var.second == "Token")
                file << "Token _token_location_"
                     << var.first
                     << ";\nvoid token_function_"
                     << var.first
                     << "()\n{\n\t_stack_token.push(_token_location_"
                     << var.first
                     << ");\n\t_token_location_"
                     << var.first
                     << " = Token();\n}\n\n";
        file << "\n";
    }

    // Write the definition of the functions
    if (function_list.size() > 0)
    {
        // Write the function
        for (FuncVar func : function_list)
        {
            file << "void function_" << func.func.name() << "()\n{\n";
            for (int i = func.vars.size() - 1; i >= 0; i--)
                // Do we have a basic token
                if (func.vars[i].second == "Token")
                    // Write the basic token extractor from stack
                    file << "\tToken " << func.vars[i].first << " = _stack_token.top(); _stack_token.pop();\n";
                else
                    // Write the extractor based on the return type
                    file << "\t"
                         << func.vars[i].second
                         << " "
                         << func.vars[i].first
                         << " = stack_"
                         << flatten_type(func.vars[i].second)
                         << ".top(); stack_"
                         << flatten_type(func.vars[i].second)
                         << ".pop();\n";

            file << "\t";
            // If the function will return something, add it to a stack
            if (func.func.return_type() != "void")
                file << "stack_" << flatten_type(func.func.return_type()) << ".push(";

            // Write the function
            file << func.func.name() << "(";

            // Add the parameters
            for (int i = 0; i < func.func.params().size(); i++)
            {
                if (i != 0)
                    file << ", ";
                file << func.func.params()[i];
            }

            // Close the brackets
            file << ")";

            if (func.func.return_type() != "void")
                file << ")";

            // Add a spacer
            file << ";\n}\n\n";
        }

        // Add a spacer
        file << "\n";
    }

    // Pre define all the production functions
    if (production_map.size() > 0)
    {
        for (ProductionPair production : production_map)
            // Pre define a production
            file << "ParseTree production_" << production.first << "();\n";

        // Add a spacer
        file << "\n";
    }

    // Define all the production functions
    for (ProductionPair production : production_map)
    {
        // Pre define a production
        file << "ParseTree production_"
             << production.first
             << "()\n{\n\treturn ("
             << (production.second.function.name() == "" ? "" : "(")
             << production.second.production->to_string(token_names, production_names);

        // Add the function if it exists
        if (production.second.function.name() != "")
            file << ") >> function_" << production.second.function.name();

        file << ")";

        if(production.first == "program")
            file << " + token_end_of_field";
        // End the production and add a spacer
        file << ";\n}\n\n";
    }

    // Add the parse function
    file << "void parse(std::string program_string){\n";

    // Make the parser object
    file << "\tEasyParser parser;\n\n";

    // Register all the tokens
    if (token_vector.size() > 0)
    {
        for (std::pair<std::string, TokenData> token : token_vector)
        {
            // Register each token
            file << "\tparser.add_new_token(token_" << token.first << ", " << token.second.regex;
            if (token.second.ignored)
                file << ", true";
            file << ");\n";
        }

        // Add a spacer
        file << "\n";
    }

    // Register all productions
    if (production_map.size() > 0)
    {
        for (ProductionPair production : production_map)
            // Register the production
            file << "\tparser.register_tree(production_" << production.first << ");\n";

        // Add a spacer
        file << "\n";
    }

    // Set the parser params
    file << "\tparser.program = production_program;\n\tparser.end_of_field_token = token_end_of_field;\n\n";

    // Run the parser
    file << "\ttry\n"
         << "\t{\n"
         << "\t\tparser.parse(program_string);\n"
         << "\t}\n"
         << "\tcatch(LexicalException e)\n"
         << "\t{\n"
         << "\t\tprintf(\"%s\", e.what());\n"
         << "\t}\n"
         << "\tcatch(SyntaxException e)\n"
         << "\t{\n"
         << "\t\tprintf(\"Syntax error on line %d at character %d, error: %s\\n\", e.error_token.line_number, e.error_token.start_character, e.error_token.value.c_str());\n"
         << "\t}\n";

    // Close the parse function
    file << "}";

    // Close the file
    file.close();

    printf("Created %s.cpp\n", filename.c_str());
}

// When we have finished parsing
void on_finish()
{
    // Check if the program production has been defined
    if (production_names.find("program") == production_names.end())
        SemanticError::throw_error("Program production not defined");

    // Finish the checks on productions
    for (ProductionPair production : production_map)
    {
        // Check that the production is of the correct type
        production.second.production->to_string(token_names, production_names);

        // Set up the function list from the production
        if (production.second.function.name() != "")
        {
            Variables vars;
            production.second.production->get_variables(vars, token_names, production_map);
            function_list.push_back({production.second.function, vars});
        }
    }

    // If we had any errors report number and exit
    if (SemanticError::count() != 0)
    {
        printf("Error Count %d\n", SemanticError::count());
        exit(0);
    }

    // Report number of warnings
    if (SemanticWarning::count() != 0)
        printf("Warning Count: %d", SemanticWarning::count());

    // Write the files
    write_header(filename);
    write_implementation(filename);
}

int main(int args_count, char *args[])
{
    // Get the filename
    if (args_count != 2)
    {
        printf("Please enter a filename(without extension): ");
        std::cin >> filename;
    }
    else
        filename = args[1];

    // Parse the information
    parse(get_program_string(filename));
}