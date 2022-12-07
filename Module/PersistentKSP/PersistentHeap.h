#pragma once


#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
using namespace std;


//template <typename ValueType>
class PersistentHeap	// 퍼시스턴트 Heap (Sink -> Source로 역방향 Dijkstra를 구한 뒤 Persistent Heap으로 관리)
{
public:
	using ValueType = long long;
	using Integer = long long;

	struct Node {
		Node* son[2];
		ValueType value;
		int curr_vertex, next_vertex;

		Node(ValueType _value, int _curr_vertex, int _next_vertex) : Node((Node*)nullptr, (Node*)nullptr, _value, _curr_vertex, _next_vertex) {}
		Node(Node* _left_son, Node* _right_son, ValueType _value, int _curr_vertex, int _next_vertex) : value(_value), curr_vertex(_curr_vertex), next_vertex(_next_vertex) { son[0] = _left_son; son[1] = _right_son; }
	};

private:
	size_t _Size_Of_Heap_Table;
	vector<Node*> _Heap_Table;
	Integer _Memory_Cnt = 0;

public:
	PersistentHeap() :PersistentHeap(0) {}
	PersistentHeap(size_t _size) :_Size_Of_Heap_Table(_size), _Heap_Table(vector<Node*>(_size, (Node*)nullptr)) {}

	Node*& operator[](int _index) { return _Heap_Table[_index]; }

	void Assign(size_t _size) {
		_Size_Of_Heap_Table = _size;
		_Heap_Table.assign(_size, (Node*)nullptr);
	}

	Integer Get_Memeory() {
		Searching(0, false);
		return _Memory_Cnt;
	}

	void Searching(int _index, bool _print = true) {
		function<void(Node*, int)> _Searching_Sub = [&](Node* _curr, int _depth) {
			if (_curr == NULL) return;

			_Memory_Cnt += sizeof(Node);
			if (_print) {
				for (int i = 0; i < _depth; i++) cout << "      ";
				cout << _curr->curr_vertex << " -> " << _curr->next_vertex << " delay: " << _curr->value << '\n';
			}

			for (int i = 0; i < 2; i++) _Searching_Sub(_curr->son[i], _depth + 1);
			_Searching_Sub(_Heap_Table[_curr->next_vertex], _depth + 1);
		};
		_Searching_Sub(_Heap_Table[_index], 0);
	}

public:
	static Node* NodeCopy(Node* _node) {
		if (_node == (Node*)nullptr) return (Node*)nullptr;
		return new Node(_node->son[0], _node->son[1], _node->value, _node->curr_vertex, _node->next_vertex);
	}

	static Node* NodeMerge(Node* _x, Node* _y, function<int(int, int)>& _Get_Random) {
		if (_x == (Node*)nullptr) return _y;
		if (_y == (Node*)nullptr) return _x;
		if (_x->value > _y->value ? true : _x->value == _y->value ? _x->next_vertex > _y->next_vertex : false) swap(_x, _y);
		int _random_value = _Get_Random(0, 1);
		if (_x->son[_random_value] != (Node*)nullptr) _x->son[_random_value] = NodeCopy(_x->son[_random_value]);
		_x->son[_random_value] = NodeMerge(_x->son[_random_value], _y, _Get_Random);
		return _x;
	}
};