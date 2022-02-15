#include "grammar.h"
#include <stack>

std::stack<Function> stack_Function;
std::stack<Prod> stack_Prod;
std::stack<TypePtr> stack_TypePtr;
std::stack<std::pair<std::string, std::string>> stack_std__pair_std__string___std__string_;
std::stack<std::string> stack_std__string;
std::stack<std::vector<std::string>> stack_std__vector_std__string_;

std::stack<Token> _stack_token;

Token _token_location_name;
void token_function_name()
{
	_stack_token.push(_token_location_name);
	_token_location_name = Token();
}

Token _token_location_type;
void token_function_type()
{
	_stack_token.push(_token_location_type);
	_token_location_type = Token();
}

Token _token_location_value;
void token_function_value()
{
	_stack_token.push(_token_location_value);
	_token_location_value = Token();
}

Token _token_location_var;
void token_function_var()
{
	_stack_token.push(_token_location_var);
	_token_location_var = Token();
}


void function_on_bracketed_production_expression()
{
	Prod production = stack_Prod.top(); stack_Prod.pop();
	stack_Prod.push(on_bracketed_production_expression(production));
}

void function_on_colon_type()
{
	TypePtr last_type = stack_TypePtr.top(); stack_TypePtr.pop();
	stack_TypePtr.push(on_colon_type(last_type));
}

void function_on_comma_type()
{
	TypePtr last_type = stack_TypePtr.top(); stack_TypePtr.pop();
	stack_TypePtr.push(on_comma_type(last_type));
}

void function_on_extra_production()
{
	Prod production = stack_Prod.top(); stack_Prod.pop();
	stack_Prod.push(on_extra_production(production));
}

void function_on_extra_type()
{
	TypePtr last_type = stack_TypePtr.top(); stack_TypePtr.pop();
	stack_TypePtr.push(on_extra_type(last_type));
}

void function_on_include_statement()
{
	std::string to_include = stack_std__string.top(); stack_std__string.pop();
	on_include_statement(to_include);
}

void function_on_include_string()
{
	Token name = _stack_token.top(); _stack_token.pop();
	stack_std__string.push(on_include_string(name));
}

void function_on_include_triangle()
{
	std::string name = stack_std__string.top(); stack_std__string.pop();
	stack_std__string.push(on_include_triangle(name));
}

void function_on_named_token()
{
	Token name = _stack_token.top(); _stack_token.pop();
	stack_std__string.push(on_named_token(name));
}

void function_on_no_production_expression()
{
	stack_Prod.push(on_no_production_expression());
}

void function_on_no_production_function()
{
	stack_Function.push(on_no_production_function());
}

void function_on_no_type()
{
	stack_TypePtr.push(on_no_type());
}

void function_on_non_bracketed_production_expression()
{
	std::pair<std::string, std::string> var = stack_std__pair_std__string___std__string_.top(); stack_std__pair_std__string___std__string_.pop();
	stack_Prod.push(on_non_bracketed_production_expression(var));
}

void function_on_non_variable_named()
{
	std::string name = stack_std__string.top(); stack_std__string.pop();
	stack_std__pair_std__string___std__string_.push(on_non_variable_named(name));
}

void function_on_not_variabled_named()
{
	stack_std__pair_std__string___std__string_.push(on_not_variabled_named());
}

void function_on_or_production_expression()
{
	Prod production = stack_Prod.top(); stack_Prod.pop();
	stack_Prod.push(on_or_production_expression(production));
}

void function_on_production()
{
	Function function = stack_Function.top(); stack_Function.pop();
	Prod production = stack_Prod.top(); stack_Prod.pop();
	std::string name = stack_std__string.top(); stack_std__string.pop();
	on_production(name, production, function);
}

void function_on_production_expression()
{
	Prod extra = stack_Prod.top(); stack_Prod.pop();
	Prod production = stack_Prod.top(); stack_Prod.pop();
	stack_Prod.push(on_production_expression(production, extra));
}

void function_on_production_function()
{
	TypePtr type = stack_TypePtr.top(); stack_TypePtr.pop();
	std::vector<std::string> params = stack_std__vector_std__string_.top(); stack_std__vector_std__string_.pop();
	Token name = _stack_token.top(); _stack_token.pop();
	stack_Function.push(on_production_function(name, type, params));
}

void function_on_production_function_params()
{
	std::vector<std::string> current_params = stack_std__vector_std__string_.top(); stack_std__vector_std__string_.pop();
	stack_std__vector_std__string_.push(on_production_function_params(current_params));
}

void function_on_production_function_params_end()
{
	stack_std__vector_std__string_.push(on_production_function_params_end());
}

void function_on_production_function_params_next()
{
	std::vector<std::string> current_params = stack_std__vector_std__string_.top(); stack_std__vector_std__string_.pop();
	Token name = _stack_token.top(); _stack_token.pop();
	stack_std__vector_std__string_.push(on_production_function_params_next(name, current_params));
}

void function_on_production_function_params_start()
{
	std::vector<std::string> current_params = stack_std__vector_std__string_.top(); stack_std__vector_std__string_.pop();
	stack_std__vector_std__string_.push(on_production_function_params_start(current_params));
}

void function_on_finish()
{
	on_finish();
}

void function_on_return_type()
{
	TypePtr type = stack_TypePtr.top(); stack_TypePtr.pop();
	stack_TypePtr.push(on_return_type(type));
}

void function_on_token_declaration()
{
	Token value = _stack_token.top(); _stack_token.pop();
	std::string name = stack_std__string.top(); stack_std__string.pop();
	on_token_declaration(name, value);
}

void function_on_ignored_token_declaration()
{
	Token value = _stack_token.top(); _stack_token.pop();
	std::string name = stack_std__string.top(); stack_std__string.pop();
	on_ignored_token_declaration(name, value);
}

void function_on_triangle_type()
{
	TypePtr last_type = stack_TypePtr.top(); stack_TypePtr.pop();
	stack_TypePtr.push(on_triangle_type(last_type));
}

void function_on_type()
{
	TypePtr last_type = stack_TypePtr.top(); stack_TypePtr.pop();
	Token type = _stack_token.top(); _stack_token.pop();
	stack_TypePtr.push(on_type(type, last_type));
}

void function_on_variable_named()
{
	std::string name = stack_std__string.top(); stack_std__string.pop();
	Token var = _stack_token.top(); _stack_token.pop();
	stack_std__pair_std__string___std__string_.push(on_variable_named(var, name));
}


ParseTree production_bracketed_production_expression();
ParseTree production_colon_type();
ParseTree production_comma_type();
ParseTree production_extra_production();
ParseTree production_extra_type();
ParseTree production_include_section();
ParseTree production_include_statement();
ParseTree production_include_statements();
ParseTree production_include_string();
ParseTree production_include_triangle();
ParseTree production_named_token();
ParseTree production_no_production_expression();
ParseTree production_no_production_function();
ParseTree production_no_type();
ParseTree production_non_bracketed_production_expression();
ParseTree production_non_variable_named();
ParseTree production_not_variabled_named();
ParseTree production_or_production_expression();
ParseTree production_production();
ParseTree production_production_expression();
ParseTree production_production_function();
ParseTree production_production_function_params();
ParseTree production_production_function_params_end();
ParseTree production_production_function_params_next();
ParseTree production_production_function_params_start();
ParseTree production_production_section();
ParseTree production_productions();
ParseTree production_program();
ParseTree production_return_type();
ParseTree production_token_declaration();
ParseTree production_token_declaration_ignored();
ParseTree production_token_declarations();
ParseTree production_token_section();
ParseTree production_triangle_type();
ParseTree production_type();
ParseTree production_variable_named();

ParseTree production_bracketed_production_expression()
{
	return (token_open_bracket + ParseTree(production_production_expression) + token_close_bracket) >> function_on_bracketed_production_expression;
}

ParseTree production_colon_type()
{
	return (token_colons + ParseTree(production_type)) >> function_on_colon_type;
}

ParseTree production_comma_type()
{
	return (token_comma + ParseTree(production_type)) >> function_on_comma_type;
}

ParseTree production_extra_production()
{
	return (ParseTree(production_or_production_expression) | ParseTree(production_production_expression) | ParseTree(production_no_production_expression)) >> function_on_extra_production;
}

ParseTree production_extra_type()
{
	return (((ParseTree(production_triangle_type) | ParseTree(production_comma_type) | ParseTree(production_colon_type))) | ParseTree(production_no_type)) >> function_on_extra_type;
}

ParseTree production_include_section()
{
	return token_keyword_include + token_open_scope + ParseTree(production_include_statements) + token_close_scope;
}

ParseTree production_include_statement()
{
	return ((ParseTree(production_include_triangle) | ParseTree(production_include_string)) + token_line_end) >> function_on_include_statement;
}

ParseTree production_include_statements()
{
	return (ParseTree(production_include_statement) + ParseTree(production_include_statements)) | epsilon;
}

ParseTree production_include_string()
{
	return ((_token_location_name << token_string >>  token_function_name)) >> function_on_include_string;
}

ParseTree production_include_triangle()
{
	return (ParseTree(production_named_token)) >> function_on_include_triangle;
}

ParseTree production_named_token()
{
	return (token_open_triangle + (_token_location_name << token_var_name >>  token_function_name) + token_close_triangle) >> function_on_named_token;
}

ParseTree production_no_production_expression()
{
	return (epsilon) >> function_on_no_production_expression;
}

ParseTree production_no_production_function()
{
	return (epsilon) >> function_on_no_production_function;
}

ParseTree production_no_type()
{
	return (epsilon) >> function_on_no_type;
}

ParseTree production_non_bracketed_production_expression()
{
	return (ParseTree(production_variable_named) | ParseTree(production_non_variable_named) | ParseTree(production_not_variabled_named)) >> function_on_non_bracketed_production_expression;
}

ParseTree production_non_variable_named()
{
	return (ParseTree(production_named_token)) >> function_on_non_variable_named;
}

ParseTree production_not_variabled_named()
{
	return (token_epsilon) >> function_on_not_variabled_named;
}

ParseTree production_or_production_expression()
{
	return (token_or + ParseTree(production_production_expression)) >> function_on_or_production_expression;
}

ParseTree production_production()
{
	return (ParseTree(production_named_token) + token_assignment + token_open_bracket + ParseTree(production_production_expression) + token_close_bracket + (ParseTree(production_production_function) | ParseTree(production_no_production_function)) + token_line_end) >> function_on_production;
}

ParseTree production_production_expression()
{
	return ((ParseTree(production_bracketed_production_expression) | ParseTree(production_non_bracketed_production_expression)) + ParseTree(production_extra_production)) >> function_on_production_expression;
}

ParseTree production_production_function()
{
	return ((token_arrow + (_token_location_name << token_var_name >>  token_function_name) + token_open_bracket + ParseTree(production_production_function_params_start) + token_close_bracket + ParseTree(production_return_type))) >> function_on_production_function;
}

ParseTree production_production_function_params()
{
	return ((token_comma + ParseTree(production_production_function_params_next)) | ParseTree(production_production_function_params_end)) >> function_on_production_function_params;
}

ParseTree production_production_function_params_end()
{
	return (epsilon) >> function_on_production_function_params_end;
}

ParseTree production_production_function_params_next()
{
	return ((_token_location_name << token_var_name >>  token_function_name) + ParseTree(production_production_function_params)) >> function_on_production_function_params_next;
}

ParseTree production_production_function_params_start()
{
	return ((ParseTree(production_production_function_params_next)) | ParseTree(production_production_function_params_end)) >> function_on_production_function_params_start;
}

ParseTree production_production_section()
{
	return token_keyword_productions + token_open_scope + ParseTree(production_productions) + token_close_scope;
}

ParseTree production_productions()
{
	return (ParseTree(production_production) + ParseTree(production_productions)) | epsilon;
}

ParseTree production_program()
{
	return (ParseTree(production_include_section) + ParseTree(production_token_section) + ParseTree(production_production_section)) >> function_on_finish;
}

ParseTree production_return_type()
{
	return ((token_arrow + ParseTree(production_type)) | ParseTree(production_no_type)) >> function_on_return_type;
}

ParseTree production_token_declaration()
{
	return (ParseTree(production_named_token) + token_assignment + (_token_location_value << token_string >>  token_function_value) + token_line_end) >> function_on_token_declaration;
}

ParseTree production_token_declaration_ignored()
{
	return (token_keyword_ignore + ParseTree(production_named_token) + token_assignment + (_token_location_value << token_string >>  token_function_value) + token_line_end) >> function_on_ignored_token_declaration;
}

ParseTree production_token_declarations()
{
	return ((ParseTree(production_token_declaration) | ParseTree(production_token_declaration_ignored)) + ParseTree(production_token_declarations)) | epsilon;
}

ParseTree production_token_section()
{
	return token_keyword_tokens + token_open_scope + ParseTree(production_token_declarations) + token_close_scope;
}

ParseTree production_triangle_type()
{
	return (token_open_triangle + ParseTree(production_type) + token_close_triangle) >> function_on_triangle_type;
}

ParseTree production_type()
{
	return ((_token_location_type << token_var_name >>  token_function_type) + ParseTree(production_extra_type)) >> function_on_type;
}

ParseTree production_variable_named()
{
	return ((_token_location_var << token_var_name >>  token_function_var) + token_equal + ParseTree(production_named_token)) >> function_on_variable_named;
}

void parse(std::string program_string){
	EasyParser parser;

	parser.add_new_token(token_whitespace, "\\s+", true);
	parser.add_new_token(token_comment, "##[^\\n]*\\n", true);
	parser.add_new_token(token_keyword_include, "Include");
	parser.add_new_token(token_keyword_tokens, "Tokens");
	parser.add_new_token(token_keyword_productions, "Productions");
	parser.add_new_token(token_keyword_ignore, "ignore");
	parser.add_new_token(token_open_scope, "\\{");
	parser.add_new_token(token_close_scope, "\\}");
	parser.add_new_token(token_open_bracket, "\\(");
	parser.add_new_token(token_close_bracket, "\\)");
	parser.add_new_token(token_or, "\\|");
	parser.add_new_token(token_string, "\"(?:\\\\\"|.)*?\"");
	parser.add_new_token(token_line_end, ";");
	parser.add_new_token(token_open_triangle, "<");
	parser.add_new_token(token_close_triangle, ">");
	parser.add_new_token(token_assignment, ":=");
	parser.add_new_token(token_equal, "=");
	parser.add_new_token(token_epsilon, "ε");
	parser.add_new_token(token_arrow, "->");
	parser.add_new_token(token_colons, "::");
	parser.add_new_token(token_comma, ",");
	parser.add_new_token(token_var_name, "[a-zA-Z_]+");

	parser.register_tree(production_bracketed_production_expression);
	parser.register_tree(production_colon_type);
	parser.register_tree(production_comma_type);
	parser.register_tree(production_extra_production);
	parser.register_tree(production_extra_type);
	parser.register_tree(production_include_section);
	parser.register_tree(production_include_statement);
	parser.register_tree(production_include_statements);
	parser.register_tree(production_include_string);
	parser.register_tree(production_include_triangle);
	parser.register_tree(production_named_token);
	parser.register_tree(production_no_production_expression);
	parser.register_tree(production_no_production_function);
	parser.register_tree(production_no_type);
	parser.register_tree(production_non_bracketed_production_expression);
	parser.register_tree(production_non_variable_named);
	parser.register_tree(production_not_variabled_named);
	parser.register_tree(production_or_production_expression);
	parser.register_tree(production_production);
	parser.register_tree(production_production_expression);
	parser.register_tree(production_production_function);
	parser.register_tree(production_production_function_params);
	parser.register_tree(production_production_function_params_end);
	parser.register_tree(production_production_function_params_next);
	parser.register_tree(production_production_function_params_start);
	parser.register_tree(production_production_section);
	parser.register_tree(production_productions);
	parser.register_tree(production_program);
	parser.register_tree(production_return_type);
	parser.register_tree(production_token_declaration);
	parser.register_tree(production_token_declaration_ignored);
	parser.register_tree(production_token_declarations);
	parser.register_tree(production_token_section);
	parser.register_tree(production_triangle_type);
	parser.register_tree(production_type);
	parser.register_tree(production_variable_named);

	parser.program = production_program;
	parser.end_of_field_token = token_end_of_field;

	try
	{
		parser.parse(program_string);
	}
	catch(LexicalException e)
	{
		printf("%s", e.what());
	}
	catch(SyntaxException e)
	{
		printf("Syntax error on line %d at character %d, error: %s\n", e.error_token.line_number, e.error_token.start_character, e.error_token.value.c_str());
	}
}