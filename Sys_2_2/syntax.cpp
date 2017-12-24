// Auto generated file by SyntaxBuilder program
// Please don't edit manually
#include <string.h>
#include "syntax.h"

token_t lex(char ch) {
	static unsigned state;
	token_t result = t_none;

	send_to_top:
 	switch (state){
	case 0:{
		if ( ch == 'b' ) { state = 1; break; }
	} break;
	case 1:{
		if ( ch == 'b' ) { state = 2; break; }
		if ( ch == 'd' ) { state = 4; break; }
		result = t_var;
		state = 0;
		goto send_to_top;
	} break;
	case 2:{
		if ( ch == 'c' ) { state = 3; break; }
    state = 0;
		return t_unexpected;
	} break;
	case 3:{
		if ( ch == 'f' ) { state = 6; break; }
    state = 0;
		return t_unexpected;
	} break;
	case 4:{
		if ( ch == 'e' ) { state = 5; break; }
    state = 0;
		return t_unexpected;
	} break;
	case 5:{
		if ( ch == 'f' ) { state = 6; break; }
    state = 0;
		return t_unexpected;
	} break;
	case 6:{
		if ( ch == 'b' ) { state = 2; break; }
		if ( ch == 'd' ) { state = 4; break; }
		result = t_var;
		state = 0;
		goto send_to_top;
	} break;
	}
	return result;
}

char* ToString(token_t token){
	switch(token) {	case t_none: return "t_none";
	case t_unexpected: return "t_unexpected";
	case t_var: return "t_var";
	}
	return NULL;
}

token_t isKeyword(const char* const str){	return t_none;
}