#pragma once

#include <vector>
using namespace std;


template <typename WeightType>
class Graph
{
public:
	typedef long long int_l;

	struct Edge {	// weighted edge
		int next_vertex, level; WeightType weight;
		Edge(int _next_vertex, int _level, WeightType _weight) :next_vertex(_next_vertex), level(_level), weight(_weight) {}
	};

private:
	WeightType _Empty_Weight, _INF_Weight;
	vector<vector<Edge*>> _Edge_List;

public:
	//Graph(){}

	vector<Edge*>& operator[](int _idx) { return _Edge_List[_idx]; }	// access vertex data by index

	void add_directed_edge(int _u, int _v, int _level, WeightType _weight) { _Edge_List[_u].push_back(new Edge(_v, _level, _weight)); }

	void add_undirected_edge(int _u, int _v, int _level, WeightType _weight) { add_directed_edge(_u, _v, _level, _weight); add_directed_edge(_v, _u, _level, _weight); }
};