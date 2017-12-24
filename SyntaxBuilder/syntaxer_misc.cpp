#include "syntaxer_misc.h"

using namespace std;
using namespace syntaxer;

syntax_node* delete_node(syntax_node* node) {
	syntax_node* tmp;
	// пересв€зываем
	if (node->child) {
		tmp = node->child;
		// ¬ставл€ем весь список потомков вместо узла
		if (node->prev)
			// предидущий есть - сшиваем
			node->prev->next = tmp;
		else
			// предидущего нет - мен€ем ссылку родител€
			node->parent->child = tmp;

		tmp->prev = node->prev;
		// новый родитель дл€ всех детей
		for (; tmp->next != NULL; tmp = tmp->next)
			tmp->parent = node->parent;
		tmp->parent = node->parent;
		// св€зываем конец списка детей со следующим узлом
		if (node->next)
			node->next->prev = tmp;
		tmp->next = node->next;
		// отдел€ем узел от детей
		tmp = node->child;
		node->child = NULL;
	}
	else {
		// потомков нет
		if (node->prev)
			node->prev->next = node->next;
		else
			node->parent->child = node->next;

		if (node->next) {
			node->next->prev = node->prev;
			tmp = node->next;
		}
		else
			tmp = node->parent;
		node->next = NULL;
		node->prev = NULL;
	}
	delete node;

	return tmp;
}

syntax_node* bend(syntax_node* node) {
	syntax_node* tmp_node = node->next;
	// bend
	// break
	tmp_node->prev = nullptr;
	node->next = nullptr;
	// horizontal to vertical
	node->child = tmp_node;
	// новый родитель
	for (; tmp_node; tmp_node = tmp_node->next)
		tmp_node->parent = node;
	return node;
}