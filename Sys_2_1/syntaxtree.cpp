#include "syntaxtree.h"
#include <typeinfo>

/* syntaxtree */

uint32_t syntaxtree::sendchar(char ch)
{
  uint32_t token = none;
process_again:
  if (cursor->parent == nullptr && cursor->check(ch))
    return token;
  bool changed = false;
  repeat:
  for each(node_t *child in cursor->children) {
    if (child->check(ch)) {
      if (cursor->parent == nullptr) {
        lexema_start = true;
        acuum.clear();
      }
      if (typeid(*cursor) == typeid(node_repeat))
        dynamic_cast<node_repeat*>(cursor)->step = 0;
      cursor = child;
      changed = true;
      
      if (typeid(node_t) == typeid(*cursor))
        goto repeat;
      else
        break;
    }
  }

  if (!changed) {
    lexema = ch;
    return unexpected;
  }
  
  if (typeid(*cursor) == typeid(node_repeat)) {
    // цикл
    dynamic_cast<node_repeat*>(cursor)->step++;
    
    if (dynamic_cast<node_repeat*>(cursor)->greedy) {
      cursor = dynamic_cast<node_repeat*>(cursor)->start;
    }
  }

  if (typeid(*cursor) == typeid(node_token)) {
    // Токен
    token = dynamic_cast<node_token*>(cursor)->tokenid;
    cursor = &root;
    lexema_start = false;
    lexema = acuum;
    if (keywords.find(lexema) != keywords.end())
      token = keywords[lexema];

    goto process_again;
  }else
    acuum += ch;
  return token;
}

syntaxtree::syntaxtree():
  root(node_charset::pWhitespace){
  cursor = &root;
  nextid = 1;
}


syntaxtree::~syntaxtree() {
  for each (node_t *node in nodes)
  {
    delete node;
  }
}

/* node_t */

void node_t::add(node_t * node){
  if (node) {
    children.push_back(node);
    node->parent = this;
  }
}

node_t::node_t(){
  parent = nullptr;
}

node_t::node_t(node_t * parent) {
  this->parent = parent;
  if (parent)
    parent->add(this);
}

node_t::~node_t(){
/*  for each (node_t *child in children) {
    child->parent = nullptr;
    child->links--;
    if (child->links == 0) {
      if (child != this)
        delete child;
    }
  }*/
}

/* node_charset_t */
node_charset::node_charset(char_sets preset){
  init(preset);
  parent = nullptr;
}

node_charset::node_charset(node_t * parent, char_sets preset) {
  init(preset);
  this->parent = parent;
  if (parent) 
    parent->add(this);
}

node_charset::node_charset(node_t * parent, std::string additional) {
  add_chars(additional);
  this->parent = parent;
  if (parent)
    parent->add(this);
}

node_charset::node_charset(node_t * parent, char_sets preset, std::string additional) {
  init(preset);
  add_chars(additional);
  this->parent = parent;
  if (parent)
    parent->add(this);
}

void node_charset::add_chars(std::string str){
  charset.insert(str.begin(), str.end());
}

void node_charset::init(char_sets preset)
{
  const char upper[] = "ABCDEFGHIGKLMNOPQRSTUVWXYZ";
  const char lower[] = "abcdefghijklmnopqrstuvwxyz";
  const char num[] = "0123456789";
  const char ws[] = "\x20\f\n\r\t\v";
  inverted = false;
  switch (preset)
  {
  case node_charset::pAnySymbol:
    inverted = true;
    charset.insert('\r');
    charset.insert('\n');
    charset.insert('\f');
    break;
  case node_charset::pSymbol:
    charset.insert(upper, upper + 26);
    charset.insert(lower, lower + 26);
    charset.insert(num, num + 10);
    charset.insert('_');
    break;
  case node_charset::pChar:
    charset.insert(upper, upper + 26);
    charset.insert(lower, lower + 26);
    break;
  case node_charset::pCharUpper:
    charset.insert(upper, upper + 26);
    break;
  case node_charset::pCharLower:
    charset.insert(lower, lower + 26);
    break;
  case node_charset::pDigit:
    charset.insert(num, num + 10);
    break;
  case node_charset::pWhitespace:
    charset.insert(ws, ws + 6);
    break;
  default:
    break;
  }
}

node_charset::~node_charset() {

}

/* node_repeat_t */
node_repeat::node_repeat(bool greedy) {
  parent = nullptr;
  this->greedy = greedy;
  start = nullptr;
}

node_repeat::node_repeat(node_t * parent, bool greedy){
  this->parent = parent;
  this->greedy = greedy;
  start = nullptr;
  if (parent)
    parent->add(this);
}


node_repeat::node_repeat(node_t * parent, bool greedy, node_t * start){
  this->parent = parent;
  this->greedy = greedy;
  this->start = start;
  if (parent)
    parent->add(this);
}

node_repeat::~node_repeat()
{
}

/* node_token */

node_token::node_token(uint32_t tokenid){
  this->tokenid = tokenid;
  parent = nullptr;
}

node_token::node_token(node_t * parent, uint32_t tokenid){
  this->tokenid = tokenid;
  this->parent = parent;
  if (parent)
    parent->add(this);
}

node_token::~node_token(){

}
