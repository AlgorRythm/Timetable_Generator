#pragma once

#include "PersistentHeap.h"

#include <iostream>
#include <functional>
#include <random>
#include <vector>
#include <queue>
using namespace std;


//template <typename WeightType>
class WeightedGraph
{
public:
	using WeightType = long long;
	using Integer = long long;
	using Bits = long long;
	using PathState = pair<Bits, Bits>;							// { 선택된 정점, 충돌 상태 }

	struct Edge {	// weighted edge
		int next_vertex, id;
		WeightType weight;
		Edge(int _next_vertex, WeightType _weight, int _index) :next_vertex(_next_vertex), weight(_weight), id(_index) {}
	};

	struct Path {	// 희소 경로
		WeightType weight;				// 가중치
		Bits sequence;
		Path() :Path(0, 0) {}
		Path(WeightType _weight, Bits _sequence) :weight(_weight), sequence(_sequence) {}
	};



private:
	PersistentHeap _Persistent_Heap;

	WeightType _INF_Value, _Big_Value, _Empty_Value;
	size_t _Size_Of_Graph, _Number_Of_Edge;			// 간선 개수(간선 넘버링할 때 활용)
	vector<vector<Edge>> _Graph, _Reverse_Graph;	// 그래프, 역방향 그래프(Sink -> Source로 역방향 Dijkstra를 구한 뒤 Persistent Heap으로 관리)

	vector<int> _Prev_Vertex, _Prev_Edge, _Used_Vertex;	
	vector<WeightType> _Distance;		

	vector<PathState> _Suffix_State_List;
	vector<int> _Suffix_Path_List;

	Integer _Memory_Cnt = 0;


public:
	WeightedGraph(WeightType _pos_inf, WeightType _neg_inf, WeightType _empty_val) :WeightedGraph(0, _pos_inf, _neg_inf, _empty_val) {}
	WeightedGraph(size_t _size, WeightType _inf_value, WeightType _big_value, WeightType _empty_value)
		:_Size_Of_Graph(_size), _INF_Value(_inf_value), _Big_Value(_big_value), _Empty_Value(_empty_value), _Number_Of_Edge(0), _Graph(vector<vector<Edge>>(_size, vector<Edge>())), _Reverse_Graph(vector<vector<Edge>>(_size, vector<Edge>())) {}

	void Assign(size_t _size) {
		_Size_Of_Graph = _size;
		_Number_Of_Edge = 0;
		_Graph.assign(_size, vector<Edge>());
		_Reverse_Graph.assign(_size, vector<Edge>());
	}

	void Add_Directed_Edge(int _from, int _to, WeightType _weight) {
		_Graph[_from].push_back(Edge(_to, _weight, _Number_Of_Edge));
		_Reverse_Graph[_to].push_back(Edge(_from, _weight, _Number_Of_Edge));
		_Number_Of_Edge++;
	}

	Integer Get_Memeory() {
		for (auto& _edge_list : _Graph) _Memory_Cnt += _edge_list.size() * sizeof(Edge);
		for (auto& _edge_list : _Reverse_Graph) _Memory_Cnt += _edge_list.size() * sizeof(Edge);
		_Memory_Cnt += _Prev_Vertex.size() * sizeof(int);
		_Memory_Cnt += _Prev_Edge.size() * sizeof(int);
		_Memory_Cnt += _Used_Vertex.size() * sizeof(int);
		_Memory_Cnt += _Distance.size() * sizeof(WeightType);
		_Memory_Cnt += _Suffix_State_List.size() * sizeof(PathState);
		_Memory_Cnt += _Suffix_Path_List.size() * sizeof(int);
		_Memory_Cnt += _Persistent_Heap.Get_Memeory();
		return _Memory_Cnt;
	}

private:
	void Reverse_Dijkstra(int _sink)	// 역방향(Sink -> Source) Dijkstra 수행 
	{
		priority_queue<pair<WeightType, int>, vector<pair<WeightType, int>>, greater<pair<WeightType, int>>> _pq;
		_Prev_Vertex.assign(_Size_Of_Graph, -1);
		_Prev_Edge.assign(_Size_Of_Graph, -1);
		_Used_Vertex.clear();
		_Distance.assign(_Size_Of_Graph, _INF_Value);

		auto _Enque = [&](int _curr, int _next, WeightType _weight, int _edge_index) {
			if (_Distance[_next] > _weight) {
				_Distance[_next] = _weight;
				_Prev_Vertex[_next] = _curr;
				_Prev_Edge[_next] = _edge_index;
				_pq.emplace(_weight, _next);
			}
		};

		_Enque(-1, _sink, _Empty_Value, -1);
		while (!_pq.empty()) {
			auto _curr = _pq.top(); _pq.pop();

			if (_Distance[_curr.second] != _curr.first) continue;
			_Used_Vertex.push_back(_curr.second);

			for (auto& _edge : _Reverse_Graph[_curr.second]) _Enque(_curr.second, _edge.next_vertex, _edge.weight + _curr.first, _edge.id);
		}
	}


	void Construct_Persistent_Heap()
	{
		mt19937 _random_generator(0x69420);
		function<int(int, int)> _Get_Random = [&](int lb, int ub) { return uniform_int_distribution<int>(lb, ub)(_random_generator); };

		_Persistent_Heap.Assign(_Size_Of_Graph);

		for (auto& _vertex : _Used_Vertex) {
			if (_Prev_Vertex[_vertex] != -1) 	// 다른 정점으로 부터 이 정점에 온 경우(최단 거리가 갱신된 경우), 이전 정점의 heap을 계승받는다
				_Persistent_Heap[_vertex] = PersistentHeap::NodeCopy(_Persistent_Heap[_Prev_Vertex[_vertex]]);

			for (auto& _edge : _Graph[_vertex]) {
				if (_edge.id == _Prev_Edge[_vertex]) continue;	// 이미 사용되고 있는 간선이라면 스킵

				WeightType _side_track = _Distance[_edge.next_vertex] + _edge.weight - _Distance[_vertex];	// side track 가중치 = sidetrack(u, v, w) =  d[v] + w - d[u];
				if (_side_track < _Big_Value)	// 현재 정점의 heap과 다음 정점의 힙을 합친다
					_Persistent_Heap[_vertex] = PersistentHeap::NodeMerge(_Persistent_Heap[_vertex], new PersistentHeap::Node(_side_track, _vertex, _edge.next_vertex), _Get_Random);
			}
		}
	}

	PathState Dfs_for_Path_State(int _curr, vector<Bits>& _conflict, function<int(int)>& Conv_Index)
	{
		if (_Suffix_State_List[_curr].first > 0) return _Suffix_State_List[_curr];
		auto _select = Dfs_for_Path_State(_Prev_Vertex[_curr], _conflict, Conv_Index);
		_select.first |= ((Bits)1 << (Bits)Conv_Index(_curr));
		_select.second |= _conflict[Conv_Index(_curr)];
		return _Suffix_State_List[_curr] = _select;
	}
	void Construct_Path_State(int _sink, vector<Bits>& _conflict, function<int(int)>& Conv_Index)
	{
		_Suffix_State_List.assign(_Size_Of_Graph, PathState(0, 0));
		_Suffix_State_List[_sink] = PathState((Bits)1 << (Bits)Conv_Index(_sink), 0);
		for (int i = 0; i < _Size_Of_Graph; i++)
			if (_Prev_Vertex[i] != -1) Dfs_for_Path_State(i, _conflict, Conv_Index);
	}


	void K_Shortest_Paths(int _source, int _sink, int _k_value, vector<Path> &_path_list, vector<Bits>& _conflict, function<int(int)>& Conv_Index, int& compare_cnt)
	{
		if (_Distance[_source] > _Big_Value) { _path_list.assign(_k_value, Path(_INF_Value, 0)); return; }

		using PqState = pair<WeightType, PersistentHeap::Node*>;		// { 가중치, PersistentHeap의 노드 }

		priority_queue<pair<PqState, pair<PathState, int>>, vector<pair<PqState, pair<PathState, int>>>, greater<pair<PqState, pair<PathState, int>>>> _pq;
		_path_list.reserve(_k_value);

		auto _Add_Path = [&](WeightType _weight, PathState& _prefix_state, int _prefix_back) {
			if (++compare_cnt, (_prefix_state.first | _Suffix_State_List[_Prev_Vertex[_prefix_back]].first) & (_prefix_state.second | _Suffix_State_List[_Prev_Vertex[_prefix_back]].second)) return;
			_path_list.push_back(Path(_weight, _prefix_state.first | _Suffix_State_List[_prefix_back].first));
		};

		PathState _init_preifx_state = PathState((Bits)1, 0);
		_Add_Path(_Distance[_source], _init_preifx_state, _source);

		if (_Persistent_Heap[_source] != nullptr)	// 시작점의 가중치 + 시작점의 힙의 delay, 시작점의 힙
			_pq.emplace(pair<PqState, pair<PathState, int>>(PqState(_Distance[_source] + _Persistent_Heap[_source]->value, _Persistent_Heap[_source]), pair<PathState, int>(_init_preifx_state, _source)));;

		while (!_pq.empty() && _path_list.size() < _k_value) {
			auto _curr = _pq.top(); _pq.pop();
			auto& _curr_weight = _curr.first.first;
			auto& _curr_vertex = _curr.first.second;
			auto& _curr_prefix_state = _curr.second.first;
			auto& _curr_prefix_back = _curr.second.second;	

			auto _new_ps = _curr_prefix_state;
			auto _new_pb = _curr_prefix_back;
			if (_new_pb == _curr_vertex->curr_vertex) {
				_new_ps.first |= ((Bits)1 << (Bits)Conv_Index(_curr_vertex->next_vertex));
				_new_ps.second |= (_conflict[Conv_Index(_curr_vertex->next_vertex)]);
				_new_pb = _curr_vertex->next_vertex;
			}
			else {
				for (int _vertex = _Prev_Vertex[_new_pb]; _vertex != _curr_vertex->curr_vertex; _vertex = _Prev_Vertex[_vertex]) {
					_new_ps.first |= ((Bits)1 << (Bits)Conv_Index(_vertex));
					_new_ps.second |= (_conflict[Conv_Index(_vertex)]);
					_new_pb = _vertex;
				}
				_new_ps.first |= ((Bits)1 << (Bits)Conv_Index(_curr_vertex->curr_vertex)); _new_ps.second |= _conflict[Conv_Index(_curr_vertex->curr_vertex)]; _new_pb = _curr_vertex->curr_vertex;
				_new_ps.first |= ((Bits)1 << (Bits)Conv_Index(_curr_vertex->next_vertex)); _new_ps.second |= _conflict[Conv_Index(_curr_vertex->next_vertex)]; _new_pb = _curr_vertex->next_vertex;
			}

			if (!(_new_ps.first & _new_ps.second)) _Add_Path(_curr_weight, _new_ps, _new_pb);

			for (int j = 0; j < 2; j++) {	// 자식이 있는 경우: 현재 가중치 - 현재 정점의 힙의 delay, 현재 정점의 힙 (다음 정점으로 갈수도 같은 정점의 상태일 수도)
				if (_curr_vertex->son[j] != (PersistentHeap::Node*)nullptr)	// 방금엣지 사용 안함
					_pq.emplace(pair<PqState, pair<PathState, int>>(PqState(_curr_weight - _curr_vertex->value + _curr_vertex->son[j]->value, _curr_vertex->son[j]), pair<PathState, int>(_curr_prefix_state, _curr_prefix_back)));
			}

			int _next_vertex = _curr_vertex->next_vertex;
			if (!(_new_ps.first & _new_ps.second) && _Persistent_Heap[_next_vertex] != (PersistentHeap::Node*)nullptr) {	// 사용한 엣지 보존 (+ 출력은 next, prev[next]활용)
				_pq.emplace(pair<PqState, pair<PathState, int>>(PqState(_curr_weight + _Persistent_Heap[_next_vertex]->value, _Persistent_Heap[_next_vertex]), pair<PathState, int>(_new_ps, _new_pb)));
			}
		}
	//	while (_path_list.size() < _k_value) _path_list.push_back(SparsePath(_Negative_INF, _Negative_INF));
	}


public:

	vector<Path> Persistent_Ksp(int _source, int _sink, int _k_value, vector<Bits>& _conflict, function<int(int)> &Conv_Index, int& compare_cnt)
	{
		vector<Path> _path_list;

		Reverse_Dijkstra(_sink);
		Construct_Persistent_Heap();
		Construct_Path_State(_sink, _conflict, Conv_Index);
		K_Shortest_Paths(_source, _sink, _k_value, _path_list, _conflict, Conv_Index, compare_cnt);

		//	_Persistent_Heap.Searching(_source);

		return _path_list;
	}
};