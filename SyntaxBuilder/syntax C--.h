// Auto generated file by SyntaxBuilder program ver: 1.5.2
// Please don't edit manually
#pragma once
#ifndef {UnitName}_h
#define {UnitName}_h

enum token_t { t_unexpected = -1,
	o_assign , o_asterisk , o_comma , o_divide , 
	o_equal , o_great , o_greatequal , o_lb , o_less , o_lessequal , o_lid , 
	o_logicaland , o_logicalor , o_lsb , o_minus , o_module , o_not , o_notequal , 
	o_plus , o_rb , o_rid , o_rsb , o_semicolon , t_char , t_float , t_ident , 
	t_int , t_string , k_char , k_else , k_float , k_for , k_if , k_int , 
	k_return , k_void , k_while , c_line , c_multiline , t_none , 
};

#define lchar char

struct selection_t {
	size_t begin, end;
};

struct states_t {
	size_t length, capacity;
	unsigned *state;
	unsigned *substate;
};
size_t lex(const lchar *str, size_t str_lenght);

// Get string of processed token
selection_t get_token_str();

// Convert token to string
char* token_to_string(token_t token);

// Retrieves a token from the queue
token_t pop_token();

#endif //syntax_C_h