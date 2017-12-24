#pragma once

#include <stdint.h>
#include <vector>
#include <set>
#include <iostream>
class indeterminate_e :public std::exception {
  public:
    uint32_t first_token;
    indeterminate_e(const char* const message, uint32_t first_token):
      std::exception(message) { 
      this->first_token = first_token;
    };
};

enum nodetype { ntNone, ntChar, ntCharset, ntAnyChar, ntWhiteSpace, ntDigit, ntWord, ntBlock, ntRepeat, ntToken };
// Узел
struct node_t {
  uint32_t id;
  nodetype nt;
  bool invert;// ntAnyChar - include \r \n
  bool greedy;
  uint32_t A;// ntChar - char; ntRepeat - minCount; 
  uint32_t B;// ; ntRepeat - maxCount
  node_t *parent;
  std::vector<node_t *> children;
  std::set<char> set;

  uint32_t next_id() {
    uint32_t result = A;
    A++;
    return result;
  }

  node_t() {
    id = 0;
    nt = ntNone;
    greedy = false;
    invert = false;
    A = 0;
    B = 0;
    parent = nullptr;
  }
};

// operand "\s"
bool ws(char ch);

// operand "\d"
bool digit(char ch);

// operand "\w"
bool word(char ch);

// operand "."
bool any(char ch, bool inlude_linebreak);

node_t* get_root();
const std::vector<node_t*>* get_elements();

void add_branch(const std::string &expr, uint32_t token_id);