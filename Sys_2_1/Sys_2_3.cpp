#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>
#include "syntax.h"

using namespace std;

// ¬ыводить только указанные токены
bool UseTextOut(token_t t);

void line_start(vector<size_t> &line_id, size_t id, size_t &line_start, size_t &line) {
  size_t begin = line_id.size() - 1;
  size_t end = 0;
  size_t mid;
  while (begin + 1 < end) {
    mid = begin + ((id - line_id[id]) * (end - begin)) / (line_id[end] - line_id[begin]);
    if (line_id[mid] < id) {
      begin = mid;
      if (line_id[mid] == id)
        break;
    }
    else
      end = mid;
  }
  line_start = line_id[begin];
  line = begin;
}

int main(int argc, char **argv) {
  string src;
  {
    ifstream fin("input.c");
    fin.unsetf(fin.skipws);
    istream_iterator<char> begin(fin), end;
    auto back = back_inserter<string>(src);
    copy(begin, end, back);
  }

  token_t token;
  size_t i = 0;
  size_t count;
  vector<size_t> line_id;
  line_id.push_back(0);
  for (size_t i = 0; i < src.size(); i++) {
    if (src[i] == '\n')
      line_id.push_back(i);
  }

  count = lex(src.c_str(), src.length());
  for (int j = 0; j < count; j++) {
    selection_t diap;
    token = pop_token();
    // если токен отличен от t_none (корректный символ но не конечный)
    if (token == t_unexpected) {
      size_t start, line;
      diap = get_token_str();
      line_start(line_id, diap.begin, start, line);
      cout << "[ERROR] Unexpected token \"" <<
        src.substr(diap.begin, diap.end - diap.begin) << "\" at " << 
        line + 1 << ':' << diap.begin - start + 1 << endl;
      break;
    }
    if (token) {
      // нужно ли выводить текст токена?
      diap = get_token_str();
      if (UseTextOut(token))
        cout << token_to_string(token) << " : " << src.substr(diap.begin, diap.end - diap.begin) << endl;
      else
        cout << token_to_string(token) << " : -" << endl;
    }
  }
  system("pause");
  return 0;
}

// не примен€ть к данных 
bool UseTextOut(token_t t) {
  return
   t == t_ident || t == t_int || t == t_float ||
    t == t_char || t == t_string;
}