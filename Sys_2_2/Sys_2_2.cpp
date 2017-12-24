#include <iostream>
#include <fstream>
#include <string>
#include "syntax.h"

using namespace std;

int main() {
  ifstream fin("input.txt");
  ofstream fout("output.txt");

  while (!fin.eof()) {
    string tmp;
    getline(fin, tmp);
    tmp += '\n';
    for each(char c in tmp) {
      token_t t = lex(c);
      if (t == t_var) {
        if (c == '\n')
          // Вся строка соответствует выражению
          fout << "yes" << endl;
        else
          // только часть строки соответствует выражению
          fout << "no" << endl;// (partially)
        lex('\n');
        break;
      }
      if (t == t_unexpected) {
        fout << "no" << endl;
        break;
      }
    }
  }
}