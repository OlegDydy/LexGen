#include "syntaxer_misc.h"

using namespace std;
using namespace syntaxer;

syntax_node* delete_node(syntax_node* node) {
	syntax_node* tmp;
	// �������������
	if (node->child) {
		tmp = node->child;
		// ��������� ���� ������ �������� ������ ����
		if (node->prev)
			// ���������� ���� - �������
			node->prev->next = tmp;
		else
			// ����������� ��� - ������ ������ ��������
			node->parent->child = tmp;

		tmp->prev = node->prev;
		// ����� �������� ��� ���� �����
		for (; tmp->next != NULL; tmp = tmp->next)
			tmp->parent = node->parent;
		tmp->parent = node->parent;
		// ��������� ����� ������ ����� �� ��������� �����
		if (node->next)
			node->next->prev = tmp;
		tmp->next = node->next;
		// �������� ���� �� �����
		tmp = node->child;
		node->child = NULL;
	}
	else {
		// �������� ���
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
	// ����� ��������
	for (; tmp_node; tmp_node = tmp_node->next)
		tmp_node->parent = node;
	return node;
}