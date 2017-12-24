
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <vector>
#include "SyntaxTree.h"
#include "builder_version.h"
#include "lexer.h"
#include "syntaxer_misc.h"
#include "format.h"

using namespace std;
// Токены компилятора компиляторов
enum token_t {
	t_unknown, t_none,
	t_token, t_operator, t_keyword, t_mute, t_sysyem,
	t_ident, t_expr, t_eol, t_eof
};

// Описание Компилируемого Токена (Имя и как его найти)
struct token_description {
	token_t type;
	string name;
	string expr;
};

// Здесь храним все определения
vector<token_description> t_descs = vector<token_description>();

// Определяем что нашли (идентификатор/ключевое слово)
token_t recognize(const string &str);
// обработать токен (tex - текстовый вывод для отладки)
void process_token(token_t t, const string &data);

// Запись созданного конечного автомата
void write_header(string output);
void write_source_file(string output);

// поток ошибок
ofstream ferr("info.log");


// начальная инициализация
void init();

// Функции для записи построенного дерева лексического анализа
// в виде конечного авомата на основе switch
bool write_transition_condition(ofstream &fout, node_t *node, uint32_t state, uint32_t substate);
void get_transition_condition(vector<node_t*> &terminals, node_t *node);
void write_state_machine(ofstream &fout, const vector<node_t*> &nodes);

// Вспомогательная функция для вывода структуры дерева
void print_struct(ofstream &fout, node_t* node, string spacing = "");

string& _value(syntaxer::syntax_node *node);

struct rule_header
{
	string name;
	bool skippable;
	bool redefine;
	rule_header() {
		skippable = false;
		redefine = false;
	}
};

rule_header& _rule(syntaxer::syntax_node *node);

void expand_groups(syntaxer::syntax_node *rule);

void toAST(syntaxer::syntax_node *&root, const string &src);

int main(int argc, char **argv) {

	string sample = format("Hello {0}", { "User name 1" });
	
	string input = "input.lang";
	string input_synt = "input.synt";
	string output = "lexer_2";
	string src = "";
	string tmp;
	// Считываем Описание синтаксиса
	{
		ifstream fin(input_synt);
		istreambuf_iterator<char> i(fin), end;
		auto back = back_inserter(src);
		copy(i, end, back);
	}

	syntaxer::syntax_node* root = syntaxer::syntax(src);

	toAST(root, src);

	// Считываем Описание лексики
	{
		ifstream fin(input);
		istreambuf_iterator<char> i(fin), end;
		auto back = back_inserter(src);
		copy(i, end, back);
	}

	// подготовка
	init();

	size_t i = 0;
	size_t j;
	size_t line = 1;
	size_t line_begin = -1;
	bool mute_expr = false;
	// обработка
	while (i < src.length()) {
		// ищем начало
		for (; i < src.length() && ws(src[i]); i++) {
			if (src[i] == '\n') {
				// просто начало строки (вместо разделения точкой с запятой)
				process_token(t_eol, tmp);
				mute_expr = false;
				line++;
				line_begin = i;
			}
		}
		j = i;

		switch (src[i]) {
		case '#':
			// комментарий
			for (i++; i < src.length() && src[i] != '\n'; i++);
			continue;
			// начало выражения
		case '(':
		{
			if (mute_expr)
				break;
			// начало выражения потом его в собственный компилятор загоним
			int brack = 1;
			bool screen = false;
			for (i++; i < src.length() && brack > 0 && src[i] != '\n'; i++)
			{
				if (screen)
					// экранироване действует лишь на один символ
					// что конкретно экранируется сейчас не особо важно
					screen = false;
				else {
					// не экранированные символы участвуют в подсчете
					if (src[i] == '(') brack++;
					else if (src[i] == ')') brack--;
					else if (src[i] == '\\') screen = true;
				}
			}
			tmp = src.substr(j, i - j);

			// очищаем от внешних скобок они больше не нужны
			if (brack > 1) {
				// где-то в выражении есть незакрытые скобки
				cerr << "[ERROR] unbalanced brackets at " << line << ':' << j - line_begin;
				ferr << "[ERROR] unbalanced brackets at " << line << ':' << j - line_begin;
				return -1;
			}
			else if (brack == 1) {
				// одну скобку мы можем закрыть, так как точно знаем что это за скобка
				// (с неё начинается любое регулярное выражение)
				cerr << "[WARNING] expression not closed at " << line << ':' << j - line_begin;
				ferr << "[WARNING] expression not closed at " << line << ':' << j - line_begin;
			}
			else
				tmp.pop_back();
			tmp.erase(0, 1);

			// отправляем на дальнейшую обработку

			try {
				process_token(t_expr, tmp);
			}
			catch (const std::exception& e) {
				cerr << "[ERROR] " << e.what() << " \"" << tmp << "\" at " << line << ':' << i - line_begin - tmp.length();
				ferr << "[ERROR] " << e.what() << " \"" << tmp << "\" at " << line << ':' << i - line_begin - tmp.length();
				return -1;
			}
			continue;
		}
		}

		// найдем идентификатор
		for (; i < src.length() && !ws(src[i]); i++);
		tmp = src.substr(j, i - j);
		// уточним найденный идентификатор (ключевое слово/идентификатор)
		token_t token = recognize(tmp);
		// отправка на обраотку
		try {
			process_token(token, tmp);
		}
		catch (const std::exception& e) {
			cerr << "[ERROR] " << e.what() << " \"" << tmp << "\" at " << line << ':' << i - line_begin - tmp.length();
			ferr << "[ERROR] " << e.what() << " \"" << tmp << "\" at " << line << ':' << i - line_begin - tmp.length();
			return -1;
		}
	}
	process_token(t_eof, tmp);
	// Компиляция включаемых файлов

	sort(t_descs.begin(), t_descs.end(), [](token_description a, token_description b) {
		if (a.type < b.type)
			return false;
		if (a.type > b.type)
			return true;
		else
			return a.name < b.name;
	});

	int id = 0;
	for each (token_description item in t_descs)
	{
		try
		{
			// добавить ветвь в дерево состояний
			if (item.type != t_sysyem)
				if (item.type == t_mute)
					add_branch(item.expr, 0);
				else
					add_branch(item.expr, id);
			id++;
		}
		catch (const indeterminate_e& e)
		{
			cerr << "[ERROR] " << e.what() << " \"" << tmp << "\" at " << line << ':' << i - line_begin - tmp.length();
			ferr << "[ERROR] " << e.what() << t_descs[e.first_token].name << " and " << item.name;
			ferr.close();
			exit(-1);
		}
		catch (const exception& e)
		{
			cerr << "[ERROR]" << e.what();
			ferr << "[ERROR]" << e.what();
			ferr.close();
			exit(-1);
		}
	}

	//	остается в синтаксисе разрешить 
	//	левую рекурсию и избавиться от общих префиксов
	//	1. Преобразуем каждую группу в отдельное правило 
	//	2. Удаляем общие префиксы
	//		2.1. 
	//	3. Удаляем левую рекурсию
	//		3.1. находим в правиле и правилах на которое оно ссылается ссылку на исследуемое правило
	//		3.2. Вырезаем ссылку и помещаем в список
	//		3.3. Формируем новое правило (с суффиксом _1 или другим)
	//		3.4. вставляем в конец всех цепочек рассматриваемого правила

	syntaxer::syntax_node *rule = root->child;
	for (; rule; rule = rule->next) {
		expand_groups(rule);
		// remove_prefixes(rule);
		// remove_recursion(rule);
	}

	// Заголовочный файл
	write_header(output);
	// машина состояний / конченный автомат
	write_source_file(output);

	//delete root;

	return 0;
}

string& _value(syntaxer::syntax_node *node)
{
	if (!node->user_data)
		node->user_data = new string();
	return *(string*)node->user_data;
}

rule_header& _rule(syntaxer::syntax_node *node)
{
	if (!node->user_data) {
		node->user_data = new rule_header();
	}
	return *(rule_header*)node->user_data;
}

syntaxer::syntax_node *convert_to_rule(syntaxer::syntax_node *node, string name,
	syntaxer::syntax_node *&new_rule) {
	syntaxer::syntax_node *result;
	syntaxer::syntax_node *item;
	syntaxer::syntax_node *prop = node->child;
	for (; prop->next; prop = prop->next);

	/*
	1. создать новое правило
	2. перегрузить в него потомков узла
	3. заменить исходный узел вызовом
	4. подцепить к узлу всё что идет после скобки
	5. подрезать новое правило на основе prop
	*/
	return result;
}

void expand_groups(syntaxer::syntax_node *rule) {
	int rule_id;
	ostringstream ss;
	syntaxer::syntax_node *new_rule;
	syntaxer::syntax_node *item = rule->child;

	vector<syntaxer::syntax_node*> added_rules;

	for (; item; item = item->next) {
		if (item->t == syntaxer::t_none) {
			item = convert_to_rule(item, "rule", new_rule);
		}
	}

}

void remove_prefixes(syntaxer::syntax_node *rule) {

}

void remove_recursion(syntaxer::syntax_node *rule) {

}

void toAST(syntaxer::syntax_node *&root, const string &src) {
	// преобразуем в АСД
	syntaxer::syntax_node* cur = root, *tmp_node, *prev = NULL;

	// спускаемся вниз
	while (cur) {
		switch (cur->t)
		{
		case syntaxer::rule:
			if (prev->t == syntaxer::grammar || prev->t == syntaxer::rule) {
				prev = cur;
				cur = cur->child;
			}
			else {
				prev = cur;
				if (cur->next)
					cur = cur->next;
				else
					cur = cur->parent;
			}
			break;
		case syntaxer::grammar:
			if (prev)
				cur = NULL;
			else {
				prev = cur;
				cur = cur->child;
			}
			// do nothing
			break;
		case syntaxer::lhs:
			//prev = cur;
			cur = delete_node(cur);
			_rule(cur->parent).name = src.substr(cur->sel.begin, cur->sel.end - cur->sel.begin);
			// выпиливаем помтомка
			cur = delete_node(cur);
			break;
		case syntaxer::o_assign:
			// исключаем
			//prev = cur;
			cur = delete_node(cur);
			break;
		case syntaxer::rhs:
			cur = delete_node(cur);
			break;
		case syntaxer::o_concat:
			cur = delete_node(cur);
			break;
		case syntaxer::o_or:
			// удаляем оператор
			cur = delete_node(cur);
			// всплываем
			for (tmp_node = cur; tmp_node->parent != NULL &&
				tmp_node->parent->t != syntaxer::rule &&
				tmp_node->parent->t != syntaxer::o_lb &&
				tmp_node->parent->t != syntaxer::o_lc &&
				tmp_node->parent->t != syntaxer::o_ls; tmp_node = tmp_node->parent);
			// вытягиваем снизу
			cur->parent->child = nullptr;
			cur->parent = tmp_node->parent;

			tmp_node->next = cur;
			cur->prev = tmp_node;
			// insert_after(cur, tmp_node);
			break;
		case syntaxer::o_rb:
		case syntaxer::o_rc:
		case syntaxer::o_rs:
		{
			syntaxer::token_t reply = syntaxer::token_t(cur->t - (syntaxer::o_rb - syntaxer::o_lb));
			tmp_node = cur;
			// ответная часть гарантированно существует
			// иначе это бы обнаружилось на этапе синтакического анализа
			// всплываем до соответствующей открывающей скобки
			// для всех ветвей спускаем вниз как выход
			for (; tmp_node->parent && tmp_node->t != reply; tmp_node = tmp_node->parent);
			syntaxer::syntax_node *n = tmp_node->child;
			while (n->next) {
				// вниз
				for (; n->child && n->child != cur; n = n->child);
				if (n->child != cur) {
					n->child = cur;
					// увеличиваем число ссылок на умный указатель
					cur->uses++;
				}
				// вверх и вправо
				for (; n->parent != tmp_node && !n->next; n = n->parent);
				n = n->next;
			}
			n->next = cur;
			cur->prev = n;
			// выход без повторов
			cur->parent = tmp_node;
			cur->uses++;
			// сгибаем
			cur = bend(cur);
			// удаляем ответную часть превращая в группирующий элемент
			tmp_node->t = syntaxer::t_none;
			cur = cur->child;
		}  break;
		case syntaxer::o_end:
			// тоже выпиливаем
			tmp_node = cur;
			for (cur = cur->parent;
				cur->parent && cur->t != syntaxer::rule;
				cur = cur->parent);
			delete_node(tmp_node);
			break;
		default:
			// возможно не нужно
			if (cur->t == syntaxer::t_ident ||
				cur->t == syntaxer::t_string)
				_value(cur) = src.substr(cur->sel.begin, cur->sel.end - cur->sel.begin);
			prev = cur;
			if (cur->next)
			{
				cur = bend(cur);
				// go deeper
				cur = cur->child;
			}
			else {
				cur = cur->parent;
			}
		}
	}
}

void write_header(string output) {
	ofstream fout = ofstream(output + ".h");
	string headername = output.substr(output.find_last_of({ '\\','/' }) + 1);
	// Шапка
	fout <<
		"// Auto generated file by SyntaxBuilder program ver:" << PROGRAM_VERSION << "\n"
		"// Please don't edit manually\n"
		"#pragma once\n"
		"#ifndef " << headername << "_h\n"
		"#define " << headername << "_h\n"
		"\n";
	// вывод всех используемых токенов
	{
		ostringstream ss;
		size_t begin_string = 0;
		ss << "enum token_t { t_unexpected = -1,\n\t";
		for each(token_description t in t_descs) {
			if (ss.str().length() + t.name.length() + 2 - begin_string > 80) {
				ss << "\n\t";
				begin_string = ss.str().length() - 1;
			}
			ss << t.name << ", ";
		}
		ss << "\n};\n\n";
		fout << ss.str();
	}
	// заголовки экспортируемых функций
	fout <<
		"#define lchar char\n"
		"\n"
		"struct selection_t {\n"
		"\tsize_t begin, end;\n"
		"};\n"
		"\n"
		"struct states_t {\n"
		"\tsize_t length, capacity;\n"
		"\tunsigned *state;\n"
		"\tunsigned *substate;\n"
		"};\n"
		"size_t lex(const lchar *str, size_t str_lenght);\n\n"
		"// Get string of processed token\n"
		"selection_t get_token_str();\n\n"
		"// Convert token to string\n"
		"char* token_to_string(token_t token);\n\n"
		"// Retrieves a token from the queue\n"
		"token_t pop_token();\n\n"
		"#endif //" << headername << "_h";
	fout.close();
}

void write_source_file(string output) {
	ofstream fout = ofstream(output + ".cpp");
	// Шапка файла
	fout <<
		"// Auto generated file by SyntaxBuilder program ver:" << PROGRAM_VERSION << "\n"
		"// Please don't edit manually\n"
		"#include <string.h>\n"
		"#include <stdlib.h>\n"
		"#include \"" << output << ".h\"\n"
		"\n"
		// Вспомогателные функции
		"size_t tokens_length = 0;\n"
		"size_t tokens_capacity = 0;\n"
		"selection_t *token_str = NULL;\n"
		"token_t *tokens = NULL;\n"
		"\n"
		"void push_token(token_t token, size_t begin, size_t end) {\n"
		"\tif (tokens_length == tokens_capacity) {\n"
		"\t\ttokens_capacity += 32;\n"
		"\t\ttokens = (token_t*)realloc(tokens, tokens_capacity * sizeof(tokens[0]));\n"
		"\t\ttoken_str = (selection_t*)realloc(token_str, tokens_capacity * sizeof(token_str[0]));\n"
		"\t}\n"
		"\ttokens[tokens_length] = token;\n"
		"\ttoken_str[tokens_length].begin = begin;\n"
		"\ttoken_str[tokens_length].end = end;\n"
		"\ttokens_length++;\n"
		"}\n"
		"\n"
		"token_t pop_token() {\n"
		"\ttokens_length--;\n"
		"\treturn tokens[tokens_length];\n"
		"}\n"
		"\n"
		"selection_t get_token_str() {\n"
		"\treturn token_str[tokens_length];\n"
		"}\n"
		"\n"
		"bool ws(char ch) {\n"
		"\treturn ch == ' ' || ch == '\\f' ||\n"
		"\t\tch == '\\t' || ch == '\\v' || ch == '\\n' || ch == '\\r';\n"
		"}\n"
		"\n"
		"bool digit(char ch) {\n"
		"\treturn '0' <= ch && ch <= '9';\n"
		"}\n"
		"\n"
		"bool word(char ch) {\n"
		"\treturn 'a' <= ch && ch <= 'z' ||\n"
		"\t\t'A' <= ch && ch <= 'Z' ||\n"
		"\t\t'0' <= ch && ch <= '9' ||\n"
		"\t\tch == '_';\n"
		"}\n"
		"\n"
		"// Save current state\n"
		"void push_state(states_t *states, unsigned state, unsigned substate) {\n"
		"\tif (states->length == states->capacity) {\n"
		"\t\tstates->capacity += 32;\n"
		"\t\tstates->state = (unsigned*)realloc(\n"
		"\t\t\tstates->state, sizeof(states->state[0]) * states->capacity);\n"
		"\t\tstates->substate = (unsigned*)realloc(\n"
		"\t\t\tstates->substate, sizeof(states->substate[0]) * states->capacity);\n"
		"\t}\n"
		"\tstates->state[states->length] = state;\n"
		"\tstates->substate[states->length] = substate;\n"
		"\tstates->length++;\n"
		"}\n"
		"\n"
		"// Load state\n"
		"void pop_state(states_t *states, unsigned *state, unsigned *substate) {\n"
		"\tif (states->length > 0) {\n"
		"\t\tstates->length--;\n"
		"\t\t*state = states->state[states->length];\n"
		"\t\t*substate = states->substate[states->length] + 1;\n"
		"\t}\n"
		"\telse {\n"
		"\t\t*state = 0;\n"
		"\t\t*substate = 0;\n"
		"\t}\n"
		"}\n\n";

	// запись конечного автомата
	write_state_machine(fout, *get_elements());

	// Функция для преобразования токена в текстовую форму
	fout << "\n"
		"char* token_to_string(token_t token){\n"
		"\tswitch(token) {";
	for each (token_description t in t_descs)
		fout << "\tcase " << t.name << ": return \"" << t.name << "\";\n";
	fout << "\t}\n"
		"\treturn NULL;\n"
		"}\n\n";

	fout.close();
}

void write_term(ofstream &fout, const vector<node_t*> &terminals, uint32_t id) {
	const char go_up[] =
		"\t\t\tdefault: // recursively go up\n"
		"\t\t\t\tpop_state( &states, &state, &substate);\n"
		"\t\t\t\tch = str[--i];\n"
		"\t\t\t\tif (i < str_lenght) goto send_to_top;\n";
	bool no_error = false;
	size_t i = 0;
	// если переходов больше одного  
	if (terminals.size() > 1) {
		//этапы проверки
		fout << "\t\t\tswitch (substate) {\n";
		for (i = 0; i < terminals.size(); i++) {
			if (terminals[i]->nt == ntToken || terminals[i]->nt == ntAnyChar && terminals[i]->invert)
				fout << "\t\t\tdefault:\n";
			else
				fout << "\t\t\tcase " << i << ':';
			no_error |= write_transition_condition(fout, terminals[i], id, (uint32_t)i);
			if (terminals[i]->nt != ntToken && (terminals[i]->nt != ntAnyChar || !terminals[i]->invert))
				fout << " break; }\n";
		}
		// если посылает ошибку
		if (id == 0) {
			fout <<
				"\t\t\tcase " << i << ":if (ws(ch)) { break; }\n"
				"\t\t\tdefault:\n"
				"\t\t\t\tif (j == i) j++;\n"
				"\t\t\t\tpush_token(t_unexpected, i, j);\n"
				"\t\t\t\treturn tokens_length;\n";
		}
		else
			if (!no_error)
				fout << go_up;
		fout << "\t\t\t}\n";
	}
	// если один переход
	else if (terminals.size() == 1) {
		if (terminals[0]->nt == ntToken || terminals[0]->nt == ntAnyChar && terminals[0]->invert)
			write_transition_condition(fout, terminals[0], id, 0);
		else {
			fout <<
				"\t\t\tswitch (substate) {\n"
				"\t\t\tcase 0:";
			if (write_transition_condition(fout, terminals[0], id, 0))
				fout << " break; }\n";
			else
				fout <<
				"break; }\n" <<
				go_up <<
				"\t\t\t}\n";
		}
	}
}

// запись конечного автомата
void write_state_machine(ofstream &fout, const vector<node_t*> &nodes) {

	bool send_error;
	vector<node_t*> terminals;
	// начало фунцции
	fout << "size_t lex(const lchar *str, size_t str_lenght) {\n"
		"\tstates_t states;\n"
		"\tunsigned depth = 0;\n"
		"\tunsigned state = 0;\n"
		"\tunsigned substate = 0;\n"
		"\tsize_t str_begin = 0;\n"
		"\tsize_t i;\n"
		"\tchar ch;\n"
		"\tstates.capacity = 0;\n"
		"\tstates.length = 0;\n"
		"\tstates.state = nullptr;\n"
		"\tstates.substate = nullptr;\n"
		"\tfor (size_t j = 0; j <= str_lenght; j++) {\n"
		"\t\tif (j == str_lenght)\n"
		"\t\t\tif (state == 0)\n"
		"\t\t\t\tbreak;\n"
		"\t\t\telse\n"
		"\t\t\t\tsubstate = -1;\n"
		"\t\tch = str[j];\n"
		"\t\ti = j;\n"
		"\tsend_to_top:\n"
		"\t\tswitch (state) {\n"
		"\t\tcase 0: {\n"
		;

	// запись элементов начального состояния
	send_error = false;
	for each(auto item in nodes[0]->children)
		if (item->nt != ntNone)
			get_transition_condition(terminals, item);
	write_term(fout, terminals, 0);
	fout <<
		"\t\tstr_begin = i;\n"
		"\t\t} break;\n";

	for each(node_t * node in nodes) {
		// не самостоятельный блок описывающий структуру
		// не выводим
		if (node->id == 0)
			continue;
		// посылает ли этот узел исключения
		// узел "Все символы" и узлы 
		// возвращающие токены не посылают ошибок
		send_error = false;
		terminals.clear();
		for each(auto item in node->children)
			get_transition_condition(terminals, item);
		for (size_t i = terminals.size() - 1; i != (size_t)(-1); i--) {
			if (terminals[i]->nt == ntAnyChar && terminals[i]->invert) {
				node_t* tmp = terminals[i];
				for (size_t j = i + 1; j < terminals.size(); j++) {
					terminals[j - 1] = terminals[j];
				}
				if (terminals.back()->nt == ntToken)
					terminals.pop_back();
				else
					terminals.back() = tmp;
			}
		}
		fout << "\t\tcase " << node->id << " : {\n";
		write_term(fout, terminals, node->id);
		fout <<
			"\t\t} break;\n";
	}
	fout <<
		"\t\t}\n"
		"\t\tsubstate = 0;\n"
		"\t\tif (i < j) {\n"
		"\t\t\tch = str[++i];\n"
		"\t\t\tgoto send_to_top;\n"
		"\t\t}\n"
		"\t}\n"
		"\tselection_t sel;\n"
		"\ttoken_t token;\n"
		"\tfor (size_t i = 0, j = tokens_length - 1; i < tokens_length / 2; i++, j--) {\n"
		"\t\ttoken = tokens[i];\n"
		"\t\ttokens[i] = tokens[j];\n"
		"\t\ttokens[j] = token;\n"
		"\t\tsel = token_str[i];\n"
		"\t\ttoken_str[i] = token_str[j];\n"
		"\t\ttoken_str[j] = sel;\n"
		"\t}\n"
		"\treturn tokens_length;\n"
		"}\n";
}

// записать условия перехода в следующее состояние из текущего
void get_transition_condition(vector<node_t*> &terminals, node_t *node) {
	node_t * node_repeat = nullptr;
	bool unique = true;

	if (node->nt == ntBlock)
		throw exception("Block node in final syntax tree");

	// структурный блок - дальше список переходов
	if (node->nt == ntNone || node->nt == ntRepeat) {
		vector<node_t*>::iterator item;

		if (node->nt == ntRepeat) {
			// жадный цикл - запишем условие повтора в начале блока
			if (node->greedy) {
				// если максимум 1 то не возвращаемся к началу блока
				if (node->B == 0 || node->B > 1)
					for (item = node->parent->children.begin(); item != node->parent->children.end() && (*item)->nt != ntToken; item++) {
						unique = true;
						for (size_t i = 0; i < terminals.size(); i++) {
							if (terminals[i] == *item) {
								unique = false;
								break;
							}
						}
						if (unique) {
							terminals.push_back(*item);
						}
					}
			}
			else {
				// обработать потом
				node_repeat = node;
			}
		}

		for (item = node->children.begin(); item != node->children.end(); item++) {

			if ((*item)->nt == ntRepeat)
				// есть еще один внешний цикл
				get_transition_condition(terminals, *item);
			else {
				unique = true;
				for (size_t i = 0; i < terminals.size(); i++) {
					if (terminals[i] == *item) {
						unique = false;
						break;
					}
				}
				if (unique) {
					terminals.push_back(*item);
				}
			}
		}

		// Не жадный цикл - условие повтора после всех проверок
		if (node_repeat) {
			// если максимум 1 то не возвращаемся к началу блока
			if (node->B == 0 || node->B > 1) {
				for (item = node_repeat->parent->children.begin();
					item != node_repeat->parent->children.end() && (*item)->nt != ntToken; item++) {
					unique = true;
					for (size_t i = 0; i < terminals.size(); i++) {
						if (terminals[i] == *item) {
							unique = false;
							break;
						}
					}
					if (unique) {
						terminals.push_back(*item);
					}
				}
			}
		}
	}
	else {
		unique = true;
		for (size_t i = 0; i < terminals.size(); i++) {
			if (terminals[i] == node) {
				unique = false;
				break;
			}
		}
		if (unique) {
			terminals.push_back(node);
		}
	}
}

// экранирование спец. символов
string auto_screen(char c) {
	switch (c) {
	case'\a': return "'\\a'"; break;
	case'\b': return "'\\b'"; break;
	case'\r': return "'\\r'"; break;
	case'\n': return "'\\n'"; break;
	case'\t': return "'\\t'"; break;
	case'\v': return "'\\v'"; break;
	case'\f': return "'\\f'"; break;
	case'\'': return "'\\''"; break;
	case'\"': return "'\\\"'"; break;
	case'\\': return "'\\\\'"; break;
	}
	return "\'" + string(1, c) + '\'';
}

// Записать условие перехода в заданный блок (зависит от типа узла)
bool write_transition_condition(
	ofstream &fout,
	node_t *node,
	uint32_t state,
	uint32_t substate)
{
	bool no_error = false;
	if (!(node->nt == ntAnyChar && node->invert == true) &&
		node->nt != ntToken)
		fout << "if (";
	switch (node->nt) {
		// один символ
	case ntChar:
		fout << " ch == " <<
			auto_screen((char)node->A) << " ) { state = " << node->id << ';';
		break;
		// произвольный символьный класс
	case ntCharset: {
		char prev = *node->set.begin();
		char start = prev;
		char c;
		bool first = true;
		fout << ' ';
		// максимально компактная запись всех значений символьного класса
		for each (c in node->set) {
			if (c - prev > 1) {
				// если не первый то добавляем условие или
				if (first)
					first = false;
				else
					fout << " ||\n\t\t\t\t";
				// если интервал содержит 1 или 2 значения то
				// записываем как отдельные условия
				// если больше то как интервал char 1 <= x <= char 2
				switch (prev - start) {
				case 0:
					fout << "ch == " << auto_screen(prev);
					break;
				case 1:
					fout << "ch == " << start <<
						" ||\n\t\t\t\tch == " << auto_screen(prev);
					break;
				default:
					fout << auto_screen(start) << " <= ch && ch <= " << auto_screen(prev);
					break;
				}
				start = c;
			}
			prev = c;
		}
		// обработка последнего интервала
		if (first)
			first = false;
		else
			fout << " ||\n\t\t\t";
		switch (prev - start) {
		case 0:
			fout << "ch == " << auto_screen(prev);
			break;
		case 1:
			fout << "ch == " << start <<
				" ||\n\t\t\tch == " << auto_screen(prev);
			break;
		default:
			fout << auto_screen(start) << " <= ch && ch <= " << auto_screen(prev);
			break;
		}
		fout << ") { state = " << node->id << ';';
	}  break;

		// встроенный класс "Все символы"
	case ntAnyChar:
		if (node->invert) {
			no_error = true;
			fout << "\t\t\t\tstate = " << node->id << ";\n";
		}
		else
			fout << "ch != '\\n' && ch != '\\r') { state = " << node->id << ';';
		break;

		// Встроенный класс "Пробельные символы"
	case ntWhiteSpace:
		if (node->invert)
			fout << "!ws(ch)) {\n"
			"\t\t\tstate = " << node->id << ';';
		else
			fout << "ws(ch)) {\n"
			"\t\t\tstate = " << node->id << ';';
		break;

		// встроенный класс "Цифры"
	case ntDigit:
		if (node->invert)
			fout << "!digit(ch)) { state = " << node->id << ';';
		else
			fout << "digit(ch)) { state = " << node->id << ';';
		break;

		// встроенный класс "Слова"
	case ntWord:
		if (node->invert)
			fout << "!word(ch)) { state = " << node->id << ';';
		else
			fout << "word(ch)) { state = " << node->id << ';';
		break;

		// Вернуть токен
	case ntToken:
		no_error = true;
		if (node->A > 0)
			fout <<
			"\t\t\t\tpush_token(" << t_descs[node->A].name << ", str_begin, i);\n";
		fout <<
			"\t\t\t\tstate = 0; substate = 0; str_begin = i;\n"
			"\t\t\t\tstates.length = 0;\n"
			"\t\t\t\tif (i < str_lenght) goto send_to_top;\n";
		break;
	default:
		break;
	}
	// записываем хитрую конструкцию откатов
	if (!(node->nt == ntAnyChar && node->invert == true) &&
		node->nt != ntToken)
		fout << " push_state(&states, " << state << ", " << substate << ");";
	return no_error;
}

// вывод структуры
void print_struct(ofstream &fout, node_t* node, string spacing) {
	switch (node->nt) {
	case ntNone:
		if (node->parent == nullptr) {
			fout << "[sync] + ";
			spacing += "      ";
		}
		break;
	case ntAnyChar:
		fout << "[any] + ";
		spacing += "     ";
		break;
	case ntBlock:
		fout << "[ERROR]Block + ";
		spacing += "         ";
		break;
	case ntChar:
		fout << "[" << (char)node->A << "] + ";
		spacing += "   ";
		break;
	case ntCharset:
		fout << "[charset] + ";
		spacing += "         ";
		break;
	case ntDigit:
		fout << "[digit] + ";
		spacing += "       ";
		break;
	case ntRepeat:
		fout << "[repeat] + ";
		spacing += "        ";
		break;
	case ntToken:
		fout << "[token:" << t_descs[node->A].name << "]";
		break;
	case ntWhiteSpace:
		fout << "[whitespace] + ";
		spacing += "            ";
		break;
	case ntWord:
		fout << "[word] + ";
		spacing += "      ";
		break;
	default:
		break;
	}

	int i = 0;
	for (auto item = node->children.begin();
		item != node->children.end(); item++) {
		if ((*item)->nt != ntNone) {
			if (i == 1)
				spacing += " | ";
			i++;
			print_struct(fout, node, spacing);
		}
		fout << endl << spacing;
	}
}

// инициаизация
void init() {
	get_root()->A = 1;
	token_description tmp;
	tmp.name = "t_none";
	tmp.expr = "";
	tmp.type = t_sysyem;
	t_descs.push_back(tmp);
}

// распознавание
token_t recognize(const string &str) {
	if (str == "token")
		return t_token;
	if (str == "oper")
		return t_operator;
	if (str == "mute")
		return t_mute;
	if (str == "kword")
		return t_keyword;
	return t_ident;
}

// обработка токена
void process_token(token_t t, const string &data) {

	static token_t state = t_unknown;
	static set<token_t> wait_next;
	static token_description descr;

	if (t != t_eol && t != t_eof &&
		!wait_next.empty() && wait_next.find(t) == wait_next.end()) {
		throw exception("Unexpected token");
	}

	switch (t)
	{
	case t_unknown:
		break;
	case t_operator:
	case t_mute:
	case t_keyword:
	case t_token:
		state = t;
		wait_next.clear();
		wait_next.insert(t_ident);
		break;
	case t_ident:
		// получен идентификатор
		switch (state)
		{
		case t_operator:
		case t_mute:
		case t_keyword:
		case t_token:
			// Name must be unique
			for each (token_description item in t_descs)
				if (item.name == data)
					throw exception("Token name must be unique");
			// do some
			descr.type = state;
			descr.name = data;
			wait_next.clear();
			wait_next.insert(t_expr);
			break;
		}
		break;
	case t_expr:
		// получено выражение
		switch (state) {
		case t_operator:
		case t_mute:
		case t_keyword:
		case t_token: {
			// do some
			descr.expr = data;

			t_descs.push_back(descr);

			wait_next.clear();
		} break;
		}
		break;
	case t_eof:
	case t_eol:
		state = t_unknown;
		wait_next.clear();
		break;
	}
}
