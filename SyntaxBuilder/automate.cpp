#include <vector>
#include <set>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class node_t {
public
	int id;
	node_t parent;
	vector<node_t*> children;
	bool is_fork(){
		return children.size() > 1;
	}
	virtual string to_code(int level){
		return "";
	}
}

class charnode_t : public node_t{
public
	set<char> chars;
	string to_code(int level) override{
		string_stream result;
		char prev = '\0';
		char first = '\0';
		foreach(char c in chars){
			if (c - prev == 1){
				if (first == '\0')
					first = prev;
			}else{
				if (prev != '\0')
					result << "if (ch = " << prev << "){" << std::endl <<
					"\tself_state = " << id << ';' << std::endl;
					if (c - first > 2){
						result << "if (" << first << " <= ch and ch <= " << prev << "){" << std::endl <<
						"\tself_state = " << id << ';' << std::endl;
						first = '\0';
					}
			}
			prev = c;
		}
		if ()
	}
}