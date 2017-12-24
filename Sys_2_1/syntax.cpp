// Auto generated file by SyntaxBuilder program ver:0.4 alpha
// Please don't edit manually
#include <string.h>
#include <stdlib.h>
#include "../Sys_2_1/syntax.h"

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
			case 0:if ( ch == '/' ) { state = 1; push_state(&states, 0, 0); break; }
			case 1:if ( ch == 'c' ) { state = 9; push_state(&states, 0, 1); break; }
			case 2:if ( ch == 'e' ) { state = 13; push_state(&states, 0, 2); break; }
			case 3:if ( ch == 'f' ) { state = 17; push_state(&states, 0, 3); break; }
			case 4:if ( ch == 'i' ) { state = 24; push_state(&states, 0, 4); break; }
			case 5:if ( ch == 'r' ) { state = 28; push_state(&states, 0, 5); break; }
			case 6:if ( ch == 'v' ) { state = 34; push_state(&states, 0, 6); break; }
			case 7:if ( ch == 'w' ) { state = 38; push_state(&states, 0, 7); break; }
			case 8:if ( ch == '=' ) { state = 43; push_state(&states, 0, 8); break; }
			case 9:if ( ch == '*' ) { state = 44; push_state(&states, 0, 9); break; }
			case 10:if ( ch == ',' ) { state = 45; push_state(&states, 0, 10); break; }
			case 11:if ( ch == '>' ) { state = 47; push_state(&states, 0, 11); break; }
			case 12:if ( ch == '(' ) { state = 49; push_state(&states, 0, 12); break; }
			case 13:if ( ch == '<' ) { state = 50; push_state(&states, 0, 13); break; }
			case 14:if ( ch == '[' ) { state = 52; push_state(&states, 0, 14); break; }
			case 15:if ( ch == '&' ) { state = 53; push_state(&states, 0, 15); break; }
			case 16:if ( ch == '|' ) { state = 55; push_state(&states, 0, 16); break; }
			case 17:if ( ch == '{' ) { state = 57; push_state(&states, 0, 17); break; }
			case 18:if ( ch == '-' ) { state = 58; push_state(&states, 0, 18); break; }
			case 19:if ( ch == '%' ) { state = 59; push_state(&states, 0, 19); break; }
			case 20:if ( ch == '!' ) { state = 60; push_state(&states, 0, 20); break; }
			case 21:if ( ch == '+' ) { state = 62; push_state(&states, 0, 21); break; }
			case 22:if ( ch == ')' ) { state = 63; push_state(&states, 0, 22); break; }
			case 23:if ( ch == ']' ) { state = 64; push_state(&states, 0, 23); break; }
			case 24:if ( ch == '}' ) { state = 65; push_state(&states, 0, 24); break; }
			case 25:if ( ch == ';' ) { state = 66; push_state(&states, 0, 25); break; }
			case 26:if ( ch == '\'' ) { state = 67; push_state(&states, 0, 26); break; }
			case 27:if (digit(ch)) { state = 72; push_state(&states, 0, 27); break; }
			case 28:if ( ch == '.' ) { state = 74; push_state(&states, 0, 28); break; }
			case 29:if ( 'A' <= ch && ch <= 'Z' ||
				ch == '_' ||
			'a' <= ch && ch <= 'z') { state = 80; push_state(&states, 0, 29); break; }
			case 30:if ( ch == '\"' ) { state = 82; push_state(&states, 0, 30); break; }
			case 31:if (ws(ch)) { break; }
			default:
        if (j == i) j++;
				push_token(t_unexpected, i, j);
				return tokens_length;
			}
		str_begin = i;
		} break;
		case 1 : {
			switch (substate) {
			case 0:if ( ch == '/' ) { state = 2; push_state(&states, 1, 0); break; }
			case 1:if ( ch == '*' ) { state = 5; push_state(&states, 1, 1); break; }
			default:
				push_token(o_divide, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 2 : {
			switch (substate) {
			case 0:if ( ch == '*' ) { state = 3; push_state(&states, 2, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 3 : {
			switch (substate) {
			case 0:if (ch != '\n' && ch != '\r') { state = 4; push_state(&states, 3, 0); break; }
			default:
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 4 : {
			switch (substate) {
			case 0:if (ch != '\n' && ch != '\r') { state = 4; push_state(&states, 4, 0); break; }
			default:
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 5 : {
			switch (substate) {
			case 0:if ( ch == '*' ) { state = 7; push_state(&states, 5, 0); break; }
			default:
				state = 6;
			}
		} break;
		case 6 : {
			switch (substate) {
			case 0:if ( ch == '*' ) { state = 7; push_state(&states, 6, 0); break; }
			default:
				state = 6;
			}
		} break;
		case 7 : {
			switch (substate) {
			case 0:if ( ch == '/' ) { state = 8; push_state(&states, 7, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 8 : {
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 9 : {
			switch (substate) {
			case 0:if ( ch == 'h' ) { state = 10; push_state(&states, 9, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 10 : {
			switch (substate) {
			case 0:if ( ch == 'a' ) { state = 11; push_state(&states, 10, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 11 : {
			switch (substate) {
			case 0:if ( ch == 'r' ) { state = 12; push_state(&states, 11, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 12 : {
				push_token(k_char, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 13 : {
			switch (substate) {
			case 0:if ( ch == 'l' ) { state = 14; push_state(&states, 13, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 14 : {
			switch (substate) {
			case 0:if ( ch == 's' ) { state = 15; push_state(&states, 14, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 15 : {
			switch (substate) {
			case 0:if ( ch == 'e' ) { state = 16; push_state(&states, 15, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 16 : {
				push_token(k_else, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 17 : {
			switch (substate) {
			case 0:if ( ch == 'l' ) { state = 18; push_state(&states, 17, 0); break; }
			case 1:if ( ch == 'o' ) { state = 22; push_state(&states, 17, 1); break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 18 : {
			switch (substate) {
			case 0:if ( ch == 'o' ) { state = 19; push_state(&states, 18, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 19 : {
			switch (substate) {
			case 0:if ( ch == 'a' ) { state = 20; push_state(&states, 19, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 20 : {
			switch (substate) {
			case 0:if ( ch == 't' ) { state = 21; push_state(&states, 20, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 21 : {
				push_token(k_float, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 22 : {
			switch (substate) {
			case 0:if ( ch == 'r' ) { state = 23; push_state(&states, 22, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 23 : {
				push_token(k_for, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 24 : {
			switch (substate) {
			case 0:if ( ch == 'f' ) { state = 25; push_state(&states, 24, 0); break; }
			case 1:if ( ch == 'n' ) { state = 26; push_state(&states, 24, 1); break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 25 : {
				push_token(k_if, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 26 : {
			switch (substate) {
			case 0:if ( ch == 't' ) { state = 27; push_state(&states, 26, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 27 : {
				push_token(k_int, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 28 : {
			switch (substate) {
			case 0:if ( ch == 'e' ) { state = 29; push_state(&states, 28, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 29 : {
			switch (substate) {
			case 0:if ( ch == 't' ) { state = 30; push_state(&states, 29, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 30 : {
			switch (substate) {
			case 0:if ( ch == 'u' ) { state = 31; push_state(&states, 30, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 31 : {
			switch (substate) {
			case 0:if ( ch == 'r' ) { state = 32; push_state(&states, 31, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 32 : {
			switch (substate) {
			case 0:if ( ch == 'n' ) { state = 33; push_state(&states, 32, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 33 : {
				push_token(k_return, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 34 : {
			switch (substate) {
			case 0:if ( ch == 'o' ) { state = 35; push_state(&states, 34, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 35 : {
			switch (substate) {
			case 0:if ( ch == 'i' ) { state = 36; push_state(&states, 35, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 36 : {
			switch (substate) {
			case 0:if ( ch == 'd' ) { state = 37; push_state(&states, 36, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 37 : {
				push_token(k_void, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 38 : {
			switch (substate) {
			case 0:if ( ch == 'h' ) { state = 39; push_state(&states, 38, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 39 : {
			switch (substate) {
			case 0:if ( ch == 'i' ) { state = 40; push_state(&states, 39, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 40 : {
			switch (substate) {
			case 0:if ( ch == 'l' ) { state = 41; push_state(&states, 40, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 41 : {
			switch (substate) {
			case 0:if ( ch == 'e' ) { state = 42; push_state(&states, 41, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 42 : {
				push_token(k_while, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 43 : {
			switch (substate) {
			case 0:if ( ch == '=' ) { state = 46; push_state(&states, 43, 0); break; }
			default:
				push_token(o_assign, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 44 : {
				push_token(o_asterisk, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 45 : {
				push_token(o_comma, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 46 : {
				push_token(o_equal, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 47 : {
			switch (substate) {
			case 0:if ( ch == '=' ) { state = 48; push_state(&states, 47, 0); break; }
			default:
				push_token(o_great, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 48 : {
				push_token(o_greatequal, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 49 : {
				push_token(o_lb, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 50 : {
			switch (substate) {
			case 0:if ( ch == '=' ) { state = 51; push_state(&states, 50, 0); break; }
			default:
				push_token(o_less, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 51 : {
				push_token(o_lessequal, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 52 : {
				push_token(o_lid, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 53 : {
			switch (substate) {
			case 0:if ( ch == '&' ) { state = 54; push_state(&states, 53, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 54 : {
				push_token(o_logicaland, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 55 : {
			switch (substate) {
			case 0:if ( ch == '|' ) { state = 56; push_state(&states, 55, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 56 : {
				push_token(o_logicalor, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 57 : {
				push_token(o_lsb, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 58 : {
				push_token(o_minus, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 59 : {
				push_token(o_module, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 60 : {
			switch (substate) {
			case 0:if ( ch == '=' ) { state = 61; push_state(&states, 60, 0); break; }
			default:
				push_token(o_not, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 61 : {
				push_token(o_notequal, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 62 : {
				push_token(o_plus, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 63 : {
				push_token(o_rb, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 64 : {
				push_token(o_rid, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 65 : {
				push_token(o_rsb, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 66 : {
				push_token(o_semicolon, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 67 : {
			switch (substate) {
			case 0:if ( ch == '\\' ) { state = 68; push_state(&states, 67, 0); break; }
			case 1:if (ch != '\n' && ch != '\r') { state = 70; push_state(&states, 67, 1); break; }
			case 2:if ( ch == '\'' ) { state = 71; push_state(&states, 67, 2); break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 68 : {
			switch (substate) {
			case 0:if (ch != '\n' && ch != '\r') { state = 69; push_state(&states, 68, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 69 : {
			switch (substate) {
			case 0:if ( ch == '\'' ) { state = 71; push_state(&states, 69, 0); break; }
			case 1:if ( ch == '\\' ) { state = 68; push_state(&states, 69, 1); break; }
			case 2:if (ch != '\n' && ch != '\r') { state = 70; push_state(&states, 69, 2); break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 70 : {
			switch (substate) {
			case 0:if ( ch == '\'' ) { state = 71; push_state(&states, 70, 0); break; }
			case 1:if ( ch == '\\' ) { state = 68; push_state(&states, 70, 1); break; }
			case 2:if (ch != '\n' && ch != '\r') { state = 70; push_state(&states, 70, 2); break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 71 : {
				push_token(t_char, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
		} break;
		case 72 : {
			switch (substate) {
			case 0:if (digit(ch)) { state = 72; push_state(&states, 72, 0); break; }
			case 1:if ( ch == '.' ) { state = 74; push_state(&states, 72, 1); break; }
			case 2:if ( ch == '.' ) { state = 73; push_state(&states, 72, 2); break; }
			default:
				push_token(t_int, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 73 : {
			switch (substate) {
			case 0:if (digit(ch)) { state = 76; push_state(&states, 73, 0); break; }
			case 1:if ( ch == 'E' ||
			ch == 'e') { state = 77; push_state(&states, 73, 1); break; }
			default:
				push_token(t_float, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 74 : {
			switch (substate) {
			case 0:if (digit(ch)) { state = 75; push_state(&states, 74, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 75 : {
			switch (substate) {
			case 0:if (digit(ch)) { state = 76; push_state(&states, 75, 0); break; }
			case 1:if ( ch == 'E' ||
			ch == 'e') { state = 77; push_state(&states, 75, 1); break; }
			default:
				push_token(t_float, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 76 : {
			switch (substate) {
			case 0:if (digit(ch)) { state = 76; push_state(&states, 76, 0); break; }
			case 1:if ( ch == 'E' ||
			ch == 'e') { state = 77; push_state(&states, 76, 1); break; }
			default:
				push_token(t_float, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 77 : {
			switch (substate) {
			case 0:if ( ch == '+' ||
			ch == '-') { state = 78; push_state(&states, 77, 0); break; }
			case 1:if ( '0' <= ch && ch <= '9') { state = 79; push_state(&states, 77, 1); break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 78 : {
			switch (substate) {
			case 0:if ( '0' <= ch && ch <= '9') { state = 79; push_state(&states, 78, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 79 : {
			switch (substate) {
			case 0:if ( '0' <= ch && ch <= '9') { state = 79; push_state(&states, 79, 0); break; }
			default:
				push_token(t_float, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 80 : {
			switch (substate) {
			case 0:if (word(ch)) { state = 81; push_state(&states, 80, 0); break; }
			default:
				push_token(t_ident, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 81 : {
			switch (substate) {
			case 0:if (word(ch)) { state = 81; push_state(&states, 81, 0); break; }
			default:
				push_token(t_ident, str_begin, i);
				state = 0; substate = 0; str_begin = i;
				states.length = 0;
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 82 : {
			switch (substate) {
			case 0:if ( ch == '\\' ) { state = 83; push_state(&states, 82, 0); break; }
			case 1:if (ch != '\n' && ch != '\r') { state = 85; push_state(&states, 82, 1); break; }
			case 2:if ( ch == '\"' ) { state = 86; push_state(&states, 82, 2); break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 83 : {
			switch (substate) {
			case 0:if (ch != '\n' && ch != '\r') { state = 84; push_state(&states, 83, 0);break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 84 : {
			switch (substate) {
			case 0:if ( ch == '\"' ) { state = 86; push_state(&states, 84, 0); break; }
			case 1:if ( ch == '\\' ) { state = 83; push_state(&states, 84, 1); break; }
			case 2:if (ch != '\n' && ch != '\r') { state = 85; push_state(&states, 84, 2); break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 85 : {
			switch (substate) {
			case 0:if ( ch == '\"' ) { state = 86; push_state(&states, 85, 0); break; }
			case 1:if ( ch == '\\' ) { state = 83; push_state(&states, 85, 1); break; }
			case 2:if (ch != '\n' && ch != '\r') { state = 85; push_state(&states, 85, 2); break; }
			default: // recursively go up
				pop_state( &states, &state, &substate);
				ch = str[--i];
				if (i < str_lenght) goto send_to_top;
			}
		} break;
		case 86 : {
				push_token(t_string, str_begin, i);
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
	case c_multiline: return "c_multiline";
	case k_char: return "k_char";
	case k_else: return "k_else";
	case k_float: return "k_float";
	case k_for: return "k_for";
	case k_if: return "k_if";
	case k_int: return "k_int";
	case k_return: return "k_return";
	case k_void: return "k_void";
	case k_while: return "k_while";
	case o_assign: return "o_assign";
	case o_asterisk: return "o_asterisk";
	case o_comma: return "o_comma";
	case o_divide: return "o_divide";
	case o_equal: return "o_equal";
	case o_great: return "o_great";
	case o_greatequal: return "o_greatequal";
	case o_lb: return "o_lb";
	case o_less: return "o_less";
	case o_lessequal: return "o_lessequal";
	case o_lid: return "o_lid";
	case o_logicaland: return "o_logicaland";
	case o_logicalor: return "o_logicalor";
	case o_lsb: return "o_lsb";
	case o_minus: return "o_minus";
	case o_module: return "o_module";
	case o_not: return "o_not";
	case o_notequal: return "o_notequal";
	case o_plus: return "o_plus";
	case o_rb: return "o_rb";
	case o_rid: return "o_rid";
	case o_rsb: return "o_rsb";
	case o_semicolon: return "o_semicolon";
	case t_char: return "t_char";
	case t_float: return "t_float";
	case t_ident: return "t_ident";
	case t_int: return "t_int";
	case t_string: return "t_string";
	}
	return NULL;
}

