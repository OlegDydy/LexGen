// Auto generated file by SyntaxBuilder program
// Please don't edit manually
#pragma once
#ifndef syntax_h
#define syntax_h

enum token_t { t_none, t_unexpected, t_var, };

token_t lex(char ch);

char* ToString(token_t token);

token_t isKeyword(const char* const str);

#endif //syntax_h