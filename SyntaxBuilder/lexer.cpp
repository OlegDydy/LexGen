// Auto generated file by SyntaxBuilder program ver:0.4 alpha
// Please don't edit manually
#include "lexer.h"
#include <stdlib.h>
#include <vector>
#include <exception>
#include <sstream>
#include <iostream>

namespace syntaxer {

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
        case 0:if (ch == '#') { state = 1; push_state(&states, 0, 0); break; }
        case 1:if (ch == '=') { state = 3; push_state(&states, 0, 1); break; }
        case 2:if (ch == ',') { state = 4; push_state(&states, 0, 2); break; }
        case 3:if (ch == '(') { state = 5; push_state(&states, 0, 3); break; }
        case 4:if (ch == '{') { state = 6; push_state(&states, 0, 4); break; }
        case 5:if (ch == '[') { state = 7; push_state(&states, 0, 5); break; }
        case 6:if (ch == '|') { state = 8; push_state(&states, 0, 6); break; }
        case 7:if (ch == ')') { state = 9; push_state(&states, 0, 7); break; }
        case 8:if (ch == '}') { state = 10; push_state(&states, 0, 8); break; }
        case 9:if (ch == ']') { state = 11; push_state(&states, 0, 9); break; }
        case 10:if (ch == ';') { state = 24; push_state(&states, 0, 10); break; }
        case 11:if ('A' <= ch && ch <= 'Z' ||
          ch == '_' ||
          'a' <= ch && ch <= 'z') {
          state = 12; push_state(&states, 0, 11); break;
        }
        case 12:if (ch == '\"') { state = 14; push_state(&states, 0, 12); break; }
        case 13:if (ch == '\'') { state = 19; push_state(&states, 0, 13); break; }
        case 14:if (ws(ch)) { break; }
        default:
          if (j == i) j++;
          push_token(t_unexpected, i, j);
          return tokens_length;
        }
        str_begin = i;
      } break;
      case 1: {
        switch (substate) {
        case 0:if (ch != '\n' && ch != '\r') { state = 2; push_state(&states, 1, 0); break; }
        default:
          state = 0; substate = 0; str_begin = i;
          states.length = 0;
          if (i < str_lenght) goto send_to_top;
        }
      } break;
      case 2: {
        switch (substate) {
        case 0:if (ch != '\n' && ch != '\r') { state = 2; push_state(&states, 2, 0); break; }
        default:
          state = 0; substate = 0; str_begin = i;
          states.length = 0;
          if (i < str_lenght) goto send_to_top;
        }
      } break;
      case 3: {
        push_token(o_assign, str_begin, i);
        state = 0; substate = 0; str_begin = i;
        states.length = 0;
        if (i < str_lenght) goto send_to_top;
      } break;
      case 4: {
        push_token(o_concat, str_begin, i);
        state = 0; substate = 0; str_begin = i;
        states.length = 0;
        if (i < str_lenght) goto send_to_top;
      } break;
      case 5: {
        push_token(o_lb, str_begin, i);
        state = 0; substate = 0; str_begin = i;
        states.length = 0;
        if (i < str_lenght) goto send_to_top;
      } break;
      case 6: {
        push_token(o_lc, str_begin, i);
        state = 0; substate = 0; str_begin = i;
        states.length = 0;
        if (i < str_lenght) goto send_to_top;
      } break;
      case 7: {
        push_token(o_ls, str_begin, i);
        state = 0; substate = 0; str_begin = i;
        states.length = 0;
        if (i < str_lenght) goto send_to_top;
      } break;
      case 8: {
        push_token(o_or, str_begin, i);
        state = 0; substate = 0; str_begin = i;
        states.length = 0;
        if (i < str_lenght) goto send_to_top;
      } break;
      case 9: {
        push_token(o_rb, str_begin, i);
        state = 0; substate = 0; str_begin = i;
        states.length = 0;
        if (i < str_lenght) goto send_to_top;
      } break;
      case 10: {
        push_token(o_rc, str_begin, i);
        state = 0; substate = 0; str_begin = i;
        states.length = 0;
        if (i < str_lenght) goto send_to_top;
      } break;
      case 11: {
        push_token(o_rs, str_begin, i);
        state = 0; substate = 0; str_begin = i;
        states.length = 0;
        if (i < str_lenght) goto send_to_top;
      } break;
      case 12: {
        switch (substate) {
        case 0:if (word(ch)) { state = 13; push_state(&states, 12, 0); break; }
        default:
          push_token(t_ident, str_begin, i);
          state = 0; substate = 0; str_begin = i;
          states.length = 0;
          if (i < str_lenght) goto send_to_top;
        }
      } break;
      case 13: {
        switch (substate) {
        case 0:if (word(ch)) { state = 13; push_state(&states, 13, 0); break; }
        default:
          push_token(t_ident, str_begin, i);
          state = 0; substate = 0; str_begin = i;
          states.length = 0;
          if (i < str_lenght) goto send_to_top;
        }
      } break;
      case 14: {
        switch (substate) {
        case 0:if (ch == '\\') { state = 15; push_state(&states, 14, 0); break; }
        case 1:if (ch != '\n' && ch != '\r') { state = 17; push_state(&states, 14, 1); break; }
        case 2:if (ch == '\"') { state = 18; push_state(&states, 14, 2); break; }
        default: // recursively go up
          pop_state(&states, &state, &substate);
          ch = str[--i];
          if (i < str_lenght) goto send_to_top;
        }
      } break;
      case 15: {
        switch (substate) {
        case 0:if (ch != '\n' && ch != '\r') { state = 16; push_state(&states, 15, 0); break; }
        default: // recursively go up
          pop_state(&states, &state, &substate);
          ch = str[--i];
          if (i < str_lenght) goto send_to_top;
        }
      } break;
      case 16: {
        switch (substate) {
        case 0:if (ch == '\"') { state = 18; push_state(&states, 16, 0); break; }
        case 1:if (ch == '\\') { state = 15; push_state(&states, 16, 1); break; }
        case 2:if (ch != '\n' && ch != '\r') { state = 17; push_state(&states, 16, 2); break; }
        default: // recursively go up
          pop_state(&states, &state, &substate);
          ch = str[--i];
          if (i < str_lenght) goto send_to_top;
        }
      } break;
      case 17: {
        switch (substate) {
        case 0:if (ch == '\"') { state = 18; push_state(&states, 17, 0); break; }
        case 1:if (ch == '\\') { state = 15; push_state(&states, 17, 1); break; }
        case 2:if (ch != '\n' && ch != '\r') { state = 17; push_state(&states, 17, 2); break; }
        default: // recursively go up
          pop_state(&states, &state, &substate);
          ch = str[--i];
          if (i < str_lenght) goto send_to_top;
        }
      } break;
      case 18: {
      } break;
      case 19: {
        switch (substate) {
        case 0:if (ch == '\\') { state = 20; push_state(&states, 19, 0); break; }
        case 1:if (ch != '\n' && ch != '\r') { state = 22; push_state(&states, 19, 1); break; }
        case 2:if (ch == '\'') { state = 23; push_state(&states, 19, 2); break; }
        default: // recursively go up
          pop_state(&states, &state, &substate);
          ch = str[--i];
          if (i < str_lenght) goto send_to_top;
        }
      } break;
      case 20: {
        switch (substate) {
        case 0:if (ch != '\n' && ch != '\r') { state = 21; push_state(&states, 20, 0); break; }
        default: // recursively go up
          pop_state(&states, &state, &substate);
          ch = str[--i];
          if (i < str_lenght) goto send_to_top;
        }
      } break;
      case 21: {
        switch (substate) {
        case 0:if (ch == '\'') { state = 23; push_state(&states, 21, 0); break; }
        case 1:if (ch == '\\') { state = 20; push_state(&states, 21, 1); break; }
        case 2:if (ch != '\n' && ch != '\r') { state = 22; push_state(&states, 21, 2); break; }
        default: // recursively go up
          pop_state(&states, &state, &substate);
          ch = str[--i];
          if (i < str_lenght) goto send_to_top;
        }
      } break;
      case 22: {
        switch (substate) {
        case 0:if (ch == '\'') { state = 23; push_state(&states, 22, 0); break; }
        case 1:if (ch == '\\') { state = 20; push_state(&states, 22, 1); break; }
        case 2:if (ch != '\n' && ch != '\r') { state = 22; push_state(&states, 22, 2); break; }
        default: // recursively go up
          pop_state(&states, &state, &substate);
          ch = str[--i];
          if (i < str_lenght) goto send_to_top;
        }
      } break;
      case 23: {
        push_token(t_string, str_begin, i);
        state = 0; substate = 0; str_begin = i;
        states.length = 0;
        if (i < str_lenght) goto send_to_top;
      } break;
      case 24: {
        push_token(o_end, str_begin, i);
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

  syntax_node* add_child(syntax_node* &cur, token_t token, size_t begin) {
    syntax_node* result = new syntax_node(cur, token);
    if (cur != nullptr) {
      syntax_node* node = cur->child;
      if (node == nullptr)
        cur->child = result;
      else
      {
        for (; node->next != nullptr; node = node->next);
        node->next = result;
        result->prev = node;
      }
    }
    result->uses++;
    result->sel.begin = begin;
    return result;
  }

  syntax_node* add_child(syntax_node* &cur, token_t token, selection_t sel) {
    syntax_node* result = new syntax_node(cur, token);
    if (cur != nullptr) {
      syntax_node* node = cur->child;
      if (node == nullptr)
        cur->child = result;
      else
      {
        for (; node->next != nullptr; node = node->next);
        node->next = result;
        result->prev = node;
      }
    }
    result->uses++;
    result->sel = sel;
    return result;
  }
  /* LL(1)
  * by default all rules are unskipping
  1)lhs = t_ident ;
  2)rhs = t_ident << lnk
  3)    | t_string << lnk
  4)    | o_ls , rhs , o_rs << lnk
  5)    | o_lc , rhs , o_rc << lnk
  6)    | o_lb , rhs , o_rb << lnk
  resolving)    | rhs , o_or , rhs
  resolving)    | rhs , o_concat , rhs ;
  // lnk - skipping
  7)lnk = o_or , rhs
  8)    | o_concat , rhs ;
  9)rule = lhs , o_assign , rhs , o_end ;
  10)grammar = { rule } ; == rule , grammar
   *
   */

  std::string errstr;
  std::ostringstream err_out;

  void error_wait_for(std::string &src, token_t wait_for, selection_t at, std::vector<size_t> &lines) {
    size_t line = 0;
    for (; line < lines.size() && lines[line] < at.begin; line++);
    line--;

    err_out = std::ostringstream();
    err_out << "[ERROR] Unexpected token \"" << src.substr(at.begin, at.end - at.begin) <<
      "\". Waiting for " << token_to_string(wait_for) << " at " <<
      line + 1 << ':' << at.begin - lines[line] + 1;
    errstr = err_out.str();
    throw std::exception(errstr.c_str());
  }

  syntax_node* syntax(std::string src)
  {
    // lhs
    token_t rule_1[] = { go_up, t_ident };

    // rhs
    token_t rule_2[] = { go_up, r1, t_ident };
    token_t rule_3[] = { go_up, r1, t_string };
    token_t rule_4[] = { go_up, r1, o_rs, rhs, o_ls };
    token_t rule_5[] = { go_up, r1, o_rc, rhs, o_lc };
    token_t rule_6[] = { go_up, r1, o_rb, rhs, o_lb };

    // rhs - additional
    token_t rule_7[] = { rhs, o_or };
    token_t rule_8[] = { rhs, o_concat };

    // rule
    token_t rule_9[] = { go_up, o_end, rhs, o_assign, lhs };

    // grammar
    token_t rule_10[] = { grammar, rule };

    uint8_t jumps[5][13] = {
      //$ = , ( { [ | ) } ] ; ident string
      { 0,0,0,0,0,0,0,0,0,0,0,  1  ,   0  }, // lhs
      { 0,0,0,6,5,4,0,0,0,0,0,  2  ,   3  }, // rhs
      { 0,0,0,0,0,0,0,0,0,0,0,  9  ,   0  }, // rule
      { 0,0,0,0,0,0,0,0,0,0,0,  10 ,   0  }, // grammar
      { 0,0,8,0,0,0,7,0,0,0,0,  0  ,   0  }, // additional rule 1
    };
    std::vector<token_t> stack;
    syntax_node* syntax_tree = nullptr, *cur;
    size_t count = lex(src.c_str(), src.length());

    std::vector<size_t> lines;
    lines.push_back(0);
    for (size_t i = 0; i < src.length(); i++)
      if (src[i] == '\n')
        lines.push_back(i + 1);

    // push axiom
    syntax_tree = new syntax_node(nullptr, grammar);

    stack.push_back(go_up);
    stack.push_back(grammar);

    cur = syntax_tree;
    token_t t;
    selection_t s;
    for (size_t i = 0; i < count; i++) {
      t = pop_token();
      s = get_token_str();
      if (t == t_unexpected) {
        size_t line = 0;
        for (; line < lines.size() && lines[line] < s.begin; line++);
        line--;

        err_out.clear();
        err_out << "[ERROR] Unexpected lexem \"" << src.substr(s.begin, s.end - s.begin) <<
          "\" at " <<
          line + 1 << ':' << s.begin - lines[line] + 1;
        errstr = err_out.str();
        throw std::exception(errstr.c_str());
      }
      int rule_id;
      // пока на вершине не терминал
      while (stack.back() >= lhs) {
        rule_id = jumps[stack.back() - lhs][t];
        if (rule_id == 0 && stack.back() != r1) {
          // если поглощаем обязательное правило то сообщаем об этом
          error_wait_for(src, stack.back(), s, lines);
        }
        stack.pop_back();

        switch (rule_id) {
        case 0: /* правило не существует - поглощаем */
          while (stack.back() == go_up) {
            stack.pop_back();
            cur->sel.end = s.end;
            cur = cur->parent;
          }
          break;
        case 1: cur = add_child(cur, lhs, s.begin); stack.insert(stack.end(), rule_1, rule_1 + 2);    break;
        case 2: cur = add_child(cur, rhs, s.begin); stack.insert(stack.end(), rule_2, rule_2 + 3);    break;
        case 3: cur = add_child(cur, rhs, s.begin); stack.insert(stack.end(), rule_3, rule_3 + 3);    break;
        case 4: cur = add_child(cur, rhs, s.begin); stack.insert(stack.end(), rule_4, rule_4 + 5);    break;
        case 5: cur = add_child(cur, rhs, s.begin); stack.insert(stack.end(), rule_5, rule_5 + 5);    break;
        case 6: cur = add_child(cur, rhs, s.begin); stack.insert(stack.end(), rule_6, rule_6 + 5);    break;
        case 7: stack.insert(stack.end(), rule_7, rule_7 + 2);   break;
        case 8: stack.insert(stack.end(), rule_8, rule_8 + 2);   break;
        case 9: cur = add_child(cur, rule, s.begin); stack.insert(stack.end(), rule_9, rule_9 + 5);    break;
        case 10: stack.insert(stack.end(), rule_10, rule_10 + 2);  break;
        }
      }
      // на вершине терминал
      if (stack.back() == t) {
        // токен совпал - удалаем
        stack.pop_back();      
        add_child(cur, t, s);
        
        while (stack.back() == go_up) {
          stack.pop_back();
          cur->sel.end = s.end;
          cur = cur->parent;
        }
      }
      else {

      }
    }
    // если в стеке осталась только аксиома то разбор успешен
    if (stack.size() == 1 && stack.back() == grammar)
      stack.pop_back();
    
    // print_tree(syntax_tree);

    return syntax_tree;
  }

  char* token_to_string(token_t token) {
    switch (token) {
    case t_none: return "t_none";
    case o_assign: return "'='";
    case o_concat: return "','";
    case o_lb: return "left bracket";
    case o_lc: return "left curly bracket";
    case o_ls: return "left square bracket";
    case o_or: return "'|'";
    case o_rb: return "right bracket";
    case o_rc: return "right curly bracket";
    case o_rs: return "right square bracket";
    case o_end: return "';'";
    case t_ident: return "identifier";
    case t_string: return "string";
    case lhs: return "rule identifier";
    case rhs: return "identifier or correct expression";
    case rule: return "rule definition";
    case grammar: return "grammar";
    }
    return "";
  }

}