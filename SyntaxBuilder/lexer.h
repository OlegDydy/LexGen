// Auto generated file by SyntaxBuilder program ver:0.4 alpha
// Please don't edit manually
#pragma once
#ifndef lexer_h
#define lexer_h

#include <string>
#include <vector>

namespace syntaxer {
	enum token_t {
		t_unexpected = -1,
		t_none, o_assign, o_concat, o_lb,
		o_lc, o_ls, o_or, o_rb, o_rc, o_rs, o_end, t_ident, t_string,
		c_line,
		// syntax
		go_up, lhs, rhs, rule, grammar, r1
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

	struct syntax_node {
		token_t t;
		selection_t sel;
		void *user_data;
		syntax_node* parent;
		syntax_node* child, *next, *prev;
		size_t uses;
		syntax_node* children(size_t id) {
			syntax_node* tmp = child;
			for (size_t i = 0; i < id && tmp != nullptr; i++, tmp = tmp->next);
			return tmp;
		}
		syntax_node(syntax_node* _parent, token_t token) {
			uses = 0;
			parent = _parent;
			user_data = NULL;
			child = NULL;
			next = NULL;
			prev = NULL;
			sel.begin = 0;
			sel.end = 0;
			t = token;
		}
		~syntax_node() {
			if (user_data)
				delete user_data;
			if (child) {
				syntax_node* tmp = child;
				for (syntax_node* node = tmp; node; node = tmp) {
					if (node->uses > 0)
						node->uses--;
					tmp = node->next;
					if (node->uses == 0)
						delete node;
				}
			}
		}
	};

	syntax_node* add_child(syntax_node* &cur, token_t token, size_t begin);
	syntax_node* add_child(syntax_node* &cur, token_t token, selection_t sel);

	size_t lex(const lchar *str, size_t str_lenght);

	syntax_node* syntax(std::string src);

	// Get string of processed token
	selection_t get_token_str();

	// Convert token to string
	char* token_to_string(token_t token);

	// Retrieves a token from the queue
	token_t pop_token();
}
#endif //lexer_h