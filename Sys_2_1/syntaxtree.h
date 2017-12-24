#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include <set>
#include <map>

class node_t
{
public:
	node_t* parent;
	std::vector<node_t*> children;

  void add(node_t* node);
	node_t();
	node_t(node_t* parent);
	~node_t();
  virtual bool check(char ch) { return true; };
private:

};

class node_charset : public node_t
{
public:
	enum char_sets {
		pAnySymbol,pSymbol, pChar, pCharUpper, pCharLower, pDigit, pWhitespace
	};

  bool inverted;
	std::set<char> charset;

	node_charset(char_sets preset);
  node_charset(node_t* parent, char_sets preset);
  node_charset(node_t* parent, std::string additional);
  node_charset(node_t* parent, char_sets preset, std::string additional);
  ~node_charset();
	bool check(char ch) override {
		return (charset.find(ch) != charset.end()) ^ inverted;
	};
  void add_chars(std::string str);
private:
  void init(char_sets preset);
};

class node_repeat : public node_t
{
public:
  int step;
  bool greedy;
  size_t prev;
  node_t* start;

  node_repeat(bool greedy);
  node_repeat(node_t* parent, bool greedy);
  node_repeat(node_t* parent, bool greedy, node_t* start);
  ~node_repeat();
private:

};

// будет поглощать ввод если tokenid = tree.none
class node_token : public node_t
{
public:
  uint32_t tokenid;
  node_token(uint32_t tokenid);
  node_token(node_t* parent, uint32_t tokenid);
  ~node_token();
private:

};

class node_char : public node_t {
public:
  char _char;
  node_char(char ch) {
    parent = nullptr;
    this->_char = ch;
  }

  node_char(node_t* parent,char ch) {
    this->parent = parent;
    this->_char = ch;
    if (parent)
      parent->add(this);
  }

  bool check(char ch) override {
    return ch == _char;
  };
};

class syntaxtree{
private:
  std::map<std::string,uint32_t> keywords;
  bool lexema_start;
  std::string acuum;

  node_t* cursor;
  uint32_t nextid;
  std::vector<node_t*> nodes;
public:
  std::string lexema;
  uint32_t add_keyword(std::string keyword) {
    uint32_t id = gen_id();
    keywords[keyword] = id;
    return id;
  }
  size_t add(node_t *node) {
    nodes.push_back(node);
    return nodes.size() - 1;
  }

  void remove(size_t index) {
    nodes.erase(nodes.begin() + index);
  }

  node_t* operator[](size_t index) {
    return nodes[index];
  }
  size_t size() { return nodes.size(); }

  node_charset root;
	const uint32_t none = 0;
	const uint32_t unexpected = -1;
	uint32_t sendchar(char ch);
  uint32_t gen_id() {
    return nextid++;
  }
	syntaxtree();
	~syntaxtree();
};

