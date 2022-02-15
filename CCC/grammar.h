#ifndef grammar_h
#define grammar_h

#include "EasyParser/EasyParser.h"
#include "production.h"
#include "function.h"
#include "type.h"

enum Tokens : int {
	token_end_of_field = 1,
	token_whitespace = 0,
	token_comment = -1,
	token_keyword_include = -2,
	token_keyword_tokens = -3,
	token_keyword_productions = -4,
	token_keyword_ignore = -5,
	token_open_scope = -6,
	token_close_scope = -7,
	token_open_bracket = -8,
	token_close_bracket = -9,
	token_or = -10,
	token_string = -11,
	token_line_end = -12,
	token_open_triangle = -13,
	token_close_triangle = -14,
	token_assignment = -15,
	token_equal = -16,
	token_epsilon = -17,
	token_arrow = -18,
	token_colons = -19,
	token_comma = -20,
	token_var_name = -21,
};

Prod on_bracketed_production_expression(Prod production);
TypePtr on_colon_type(TypePtr last_type);
TypePtr on_comma_type(TypePtr last_type);
Prod on_extra_production(Prod production);
TypePtr on_extra_type(TypePtr last_type);
void on_include_statement(std::string to_include);
std::string on_include_string(Token name);
std::string on_include_triangle(std::string name);
std::string on_named_token(Token name);
Prod on_no_production_expression();
Function on_no_production_function();
TypePtr on_no_type();
Prod on_non_bracketed_production_expression(std::pair<std::string, std::string> var);
std::pair<std::string, std::string> on_non_variable_named(std::string name);
std::pair<std::string, std::string> on_not_variabled_named();
Prod on_or_production_expression(Prod production);
void on_production(std::string name, Prod production, Function function);
Prod on_production_expression(Prod production, Prod extra);
Function on_production_function(Token name, TypePtr type, std::vector<std::string> params);
std::vector<std::string> on_production_function_params(std::vector<std::string> current_params);
std::vector<std::string> on_production_function_params_end();
std::vector<std::string> on_production_function_params_next(Token name, std::vector<std::string> current_params);
std::vector<std::string> on_production_function_params_start(std::vector<std::string> current_params);
void on_finish();
TypePtr on_return_type(TypePtr type);
void on_token_declaration(std::string name, Token value);
void on_ignored_token_declaration(std::string name, Token value);
TypePtr on_triangle_type(TypePtr last_type);
TypePtr on_type(Token type, TypePtr last_type);
std::pair<std::string, std::string> on_variable_named(Token var, std::string name);

void parse(std::string);

#endif