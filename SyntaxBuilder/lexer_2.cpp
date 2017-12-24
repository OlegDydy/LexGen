// Auto generated file by SyntaxBuilder program ver:0.4 alpha
// Please don't edit manually
#include <string.h>
#include <stdlib.h>
#include "lexer_2.h"

size_t tokens_length = 0;
size_t tokens_capacity = 0;
selection_t *token_str = NULL;
token_t *tokens = NULL;

void push_token(token_t token, size_t begin, size_t end) {
	if (tokens_length == tokens_capacity) {
		tokens_capacity += 32;
		tokens = (token_t*)realloc(tokens, tokens_capacity * sizeof(tokens[0]));
		token_str = (selection_t*)realloc(token_str, tokens_capacity * sizeof(token_str[0]));
	}
	tokens[tokens_length] = token;
	token_str[tokens_length].begin = begin;
	token_str[tokens_length].end = end;
	tokens_length++;
}

token_t pop_token() {
	tokens_length--;
	return tokens[tokens_length];
}

selection_t get_token_str() {
	return token_str[tokens_length];
}

bool ws(char ch) {
	return ch == ' ' || ch == '\f' ||
		ch == '\t' || ch == '\v' || ch == '\n' || ch == '\r';
}

bool digit(char ch) {
	return '0' <= ch && ch <= '9';
}

bool word(char ch) {
	return 'a' <= ch && ch <= 'z' ||
		'A' <= ch && ch <= 'Z' ||
		'0' <= ch && ch <= '9' ||
		ch == '_';
}

// Save current state
void push_state(states_t *states, unsigned state, unsigned substate) {
	if (states->length == states->capacity) {
		states->capacity += 32;
		states->state = (unsigned*)realloc(
			states->state, sizeof(states->state[0]) * states->capacity);
		states->substate = (unsigned*)realloc(
			states->substate, sizeof(states->substate[0]) * states->capacity);
	}
	states->state[states->length] = state;
	states->substate[states->length] = substate;
	states->length++;
}

// Load state
void pop_state(states_t *states, unsigned *state, unsigned *substate) {
	if (states->length > 0) {
		states->length--;
		*state = states->state[states->length];
		*substate = states->substate[states->length] + 1;
	}
	else {
		*state = 0;
		*substate = 0;
	}
}

size_t lex(const lchar *str, size_t str_lenght) {
	states_t states;
	unsigned depth = 0;
	unsigned state = 0;
	unsigned substate = 0;
	size_t str_begin = 0;
	size_t i;
	char ch;
	states.capacity = 0;
	states.length = 0;
	states.state = nullptr;
	states.substate = nullptr;
	for (size_t j = 0; j <= str_lenght; j++) {
		if (j == str_lenght)
			if (state == 0)
				break;
			else
				substate = -1;
		ch = str[j];
		i = j;
	send_to_top:
		switch (state) {
		case 0: {
			switch (substate) {
			case 0:if ( ch == '#' ) { state = 1; push_state(&states, 0, 0); break; }
			case 1:if ( ch == '=' ) { state = 3; push_state(&states, 0, 1); break; }
			case 2:if ( ch == '(' ) { state = 4; push_state(&states, 0, 2); break; }
			case 3:if ( ch == '[' ) { state = 5; push_state(&states, 0, 3); break; }
			case 4:if ( ch == '|' ) { state = 6; push_state(&states, 0, 4); break; }
			case 5:if ( ch == '>' ) { state = 7; push_state(&states, 0, 5); break; }
			case 6:if ( ch == ')' ) { state = 8; push_state(&states, 0, 6); break; }
			case 7:if ( ch == ']' ) { state = 9; push_state(&states, 0, 7); break; }
			case 8:if ( ch == '<' ) { state = 10; push_state(&states, 0, 8); break; }
			case 9:if ( 'A' <= ch && ch <= 'Z' ||
				ch == '_' ||
			'a' <= ch && ch <= 'z') { state = 11; push_state(&states, 0, 9); break; }
			case 10:if ( ch == '\"' ) { state = 13; push_state(&states, 0, 10); break; }
			case 11:if (ws(ch)) { break; }
			default:
				if (j == i) j++;
				push_token(t_unexpected, i, j);
				return tokens_length;
			}
		str_begin = i;
		} break;
		case 1 : {
			switch (substate) {
			case 0:if (ch != '\n' && ch != '\r') { state = 2; push_state(&states, 1, 0); break; }
			default:
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 2 : {
			switch (substate) {
			case 0:if (ch != '\n' && ch != '\r') { state = 2; push_state(&states, 2, 0); break; }
			default:
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 3 : {
				push_token(o_assign, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 4 : {
				push_token(o_lb, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 5 : {
				push_token(o_lsb, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 6 : {
				push_token(o_or, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 7 : {
				push_token(o_prefix, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 8 : {
				push_token(o_rb, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 9 : {
				push_token(o_rsb, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 10 : {
				push_token(o_sufix, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 11 : {
			switch (substate) {
			case 0:if (word(ch)) { state = 12; push_state(&states, 11, 0); break; }
			default:
				push_token(t_ident, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 12 : {
			switch (substate) {
			case 0:if (word(ch)) { state = 12; push_state(&states, 12, 0); break; }
			default:
				push_token(t_ident, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 13 : {
			switch (substate) {
			case 0:if (ch != '\n' && ch != '\r') { state = 14; push_state(&states, 13, 0); break; }
			case 1:if ( ch == '\"' ) { state = 15; push_state(&states, 13, 1); break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 14 : {
			switch (substate) {
			case 0:if ( ch == '\"' ) { state = 15; push_state(&states, 14, 0); break; }
			case 1:if (ch != '\n' && ch != '\r') { state = 14; push_state(&states, 14, 1); break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 15 : {
				push_token(t_str, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		}
		substate = 0;
		if (i < j) {
			ch = str[++i];
			goto send_to_top;
		}
	}
	selection_t sel;
	token_t token;
	for (size_t i = 0, j = tokens_length - 1; i < tokens_length / 2; i++, j--) {
		token = tokens[i];
		tokens[i] = tokens[j];
		tokens[j] = token;
		sel = token_str[i];
		token_str[i] = token_str[j];
		token_str[j] = sel;
	}
	return tokens_length;
}

char* token_to_string(token_t token){
	switch(token) {	case t_none: return "t_none";
	case c_line: return "c_line";
	case o_assign: return "o_assign";
	case o_lb: return "o_lb";
	case o_lsb: return "o_lsb";
	case o_or: return "o_or";
	case o_prefix: return "o_prefix";
	case o_rb: return "o_rb";
	case o_rsb: return "o_rsb";
	case o_sufix: return "o_sufix";
	case t_ident: return "t_ident";
	case t_str: return "t_str";
	}
	return NULL;
}

