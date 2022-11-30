#pragma once

#include <vector>
using namespace std;


class PriorityValue
{
public:
	enum State { Variable, Minimum, Maximum };	// 값의 상태 (변수, 최소 고정, 최대 고정)
	enum Priority { First, Second, Third, Fourth, Fifth };	// 우선순위 목록
	enum Arithmetic { Addition, Subtraction, Multiplication, Division, Modulus };	// 산술연산 타입


private:
	const int P_Number = 5;
	State _State;
	vector<int> _Counting_Table;


public:
	PriorityValue() :_State(Variable), _Counting_Table(vector<int>(5, 0)) {}
	PriorityValue(State _value_state) :_State(_value_state), _Counting_Table(vector<int>(5, 0)) {}
	PriorityValue(State _value_state, Priority _priority) :_State(_value_state), _Counting_Table(vector<int>(5, 0)) { _Counting_Table[_priority]++; }
	PriorityValue(State _value_state, vector<int> _counting_table) :_State(_value_state), _Counting_Table(_counting_table) {}


	// 비교 연산자
	bool operator<(PriorityValue _other) {
		if (_State == Variable && _other._State == Variable) {
			for (int _idx = 0; _idx < P_Number; _idx++)
				if (_Counting_Table[_idx] != _other._Counting_Table[_idx]) return _Counting_Table[_idx] < _other._Counting_Table[_idx];
			return false;
		}
		return _State == Variable ? _other._State == Maximum : _other._State == Variable ? _State == Minimum : _State < _other._State;
	}
	bool operator<=(PriorityValue _other) {
		if (_State == Variable && _other._State == Variable) {
			for (int _idx = 0; _idx < P_Number; _idx++)
				if (_Counting_Table[_idx] != _other._Counting_Table[_idx]) return _Counting_Table[_idx] < _other._Counting_Table[_idx];
			return true;
		}
		return _State == Variable ? _other._State == Maximum : _other._State == Variable ? _State == Minimum : _State <= _other._State;
	}
	bool operator>(PriorityValue _other) {
		if (_State == Variable && _other._State == Variable) {
			for (int _idx = 0; _idx < P_Number; _idx++)
				if (_Counting_Table[_idx] != _other._Counting_Table[_idx]) return _Counting_Table[_idx] > _other._Counting_Table[_idx];
			return false;
		}
		return _State == Variable ? _other._State == Minimum : _other._State == Variable ? _State == Maximum : _State > _other._State;
	}
	bool operator>=(PriorityValue _other) {
		if (_State == Variable && _other._State == Variable) {
			for (int _idx = 0; _idx < P_Number; _idx++)
				if (_Counting_Table[_idx] != _other._Counting_Table[_idx]) return _Counting_Table[_idx] > _other._Counting_Table[_idx];
			return true;
		}
		return _State == Variable ? _other._State == Minimum : _other._State == Variable ? _State == Maximum : _State >= _other._State;
	}
	bool operator==(PriorityValue _other) {
		if (_State == Variable && _other._State == Variable) {
			for (int _idx = 0; _idx < P_Number; _idx++)
				if (_Counting_Table[_idx] != _other._Counting_Table[_idx]) return false;
			return true;
		}
		return _State == _other._State;
	}


	// 산술 연산자
	vector<int> Arithmetic_Counting_Table(vector<int>& _ct1, vector<int>& _ct2, Arithmetic _arithmetic_type) {
		if (_arithmetic_type == Addition) return { _ct1[0] + _ct2[0],_ct1[1] + _ct2[1],_ct1[2] + _ct2[2],_ct1[3] + _ct2[3],_ct1[4] + _ct2[4] };
		else if (_arithmetic_type == Subtraction) return { _ct1[0] - _ct2[0],_ct1[1] - _ct2[1],_ct1[2] - _ct2[2],_ct1[3] - _ct2[3],_ct1[4] - _ct2[4] };
		else if (_arithmetic_type == Multiplication) return { _ct1[0] * _ct2[0],_ct1[1] * _ct2[1],_ct1[2] * _ct2[2],_ct1[3] * _ct2[3],_ct1[4] * _ct2[4] };
		else if (_arithmetic_type == Division) return { _ct1[0] / _ct2[0],_ct1[1] / _ct2[1],_ct1[2] / _ct2[2],_ct1[3] / _ct2[3],_ct1[4] / _ct2[4] };
		return { _ct1[0] % _ct2[0],_ct1[1] % _ct2[1],_ct1[2] % _ct2[2],_ct1[3] % _ct2[3],_ct1[4] % _ct2[4] };
	}
	PriorityValue operator+(PriorityValue _other) {
		return _State == Variable && _other._State == Variable ? PriorityValue(Variable, Arithmetic_Counting_Table(_Counting_Table, _other._Counting_Table, Addition))
			: _State == Variable ? PriorityValue(_other._State)
			: _other._State == Variable ? PriorityValue(_State)
			: _State == _other._State ? PriorityValue(_State) : PriorityValue();
	}
	PriorityValue operator-(PriorityValue _other) {
		return _State == Variable && _other._State == Variable ? PriorityValue(Variable, Arithmetic_Counting_Table(_Counting_Table, _other._Counting_Table, Subtraction))
			: _State == Variable ? PriorityValue(_other._State == Minimum ? Maximum : Minimum)
			: _other._State == Variable ? PriorityValue(_State)
			: _State == _other._State ? PriorityValue() : PriorityValue(_State);
	}
	PriorityValue operator*(PriorityValue _other) {
		return _State == Variable && _other._State == Variable ? PriorityValue(Variable, Arithmetic_Counting_Table(_Counting_Table, _other._Counting_Table, Multiplication))
			: _State == Variable ? PriorityValue(_other._State)
			: _other._State == Variable ? PriorityValue(_State)
			: _State == _other._State ? PriorityValue(Maximum) : PriorityValue(Minimum);
	}
	PriorityValue operator/(PriorityValue _other) {
		return _State == Variable && _other._State == Variable ? PriorityValue(Variable, Arithmetic_Counting_Table(_Counting_Table, _other._Counting_Table, Division))
			: _State == Variable ? PriorityValue()
			: _other._State == Variable ? PriorityValue(_State)
			: _State == _other._State ? PriorityValue(Maximum) : PriorityValue(Minimum);
	}
	PriorityValue& operator+=(PriorityValue _other) {
		if (_State == Variable && _other._State == Variable) for (int i = 0; i < P_Number; i++) _Counting_Table[i] += _other._Counting_Table[i];
		else if (_State == Variable) { _State = _other._State; fill(_Counting_Table.begin(), _Counting_Table.end(), 0); }
		else if (_other._State == Variable) {}
		else if (_State == _other._State) {}
		else _State = Variable;
		return *this;
	}
	PriorityValue& operator-=(PriorityValue _other) {
		if (_State == Variable && _other._State == Variable) for (int i = 0; i < P_Number; i++) _Counting_Table[i] -= _other._Counting_Table[i];
		else if (_State == Variable) { _State = (_other._State == Minimum ? Maximum : Minimum); fill(_Counting_Table.begin(), _Counting_Table.end(), 0); }
		else if (_other._State == Variable) {}
		else if (_State == _other._State) _State = Variable;
		else {}
		return *this;
	}
	PriorityValue& operator*=(PriorityValue _other) {
		if (_State == Variable && _other._State == Variable) for (int i = 0; i < P_Number; i++) _Counting_Table[i] *= _other._Counting_Table[i];
		else if (_State == Variable) { _State = _other._State; fill(_Counting_Table.begin(), _Counting_Table.end(), 0); }
		else if (_other._State == Variable) {}
		else if (_State == _other._State) _State = Maximum;
		else _State = Minimum;
		return *this;
	}
	PriorityValue& operator/=(PriorityValue _other) {
		if (_State == Variable && _other._State == Variable) for (int i = 0; i < P_Number; i++) _Counting_Table[i] /= _other._Counting_Table[i];
		else if (_State == Variable) { _State = Variable; fill(_Counting_Table.begin(), _Counting_Table.end(), 0); }
		else if (_other._State == Variable) {}
		else if (_State == _other._State) _State = Maximum;
		else _State = Minimum;
		return *this;
	}
};
