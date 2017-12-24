// Auto generated file by SyntaxBuilder program ver:0.4 alpha
// Please don't edit manually
#pragma once
#ifndef lexer_2_h
#define lexer_2_h

enum token_t { t_unexpected = -1,
	t_none, c_line, o_assign, o_lb, o_lsb, o_or, 
	o_prefix, o_rb, o_rsb, o_sufix, t_ident, t_str, 
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

#endif //lexer_2_h