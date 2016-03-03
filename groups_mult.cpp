#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <assert.h>
#include <utility>
#include <algorithm>
using namespace std;

//#include "triangle.hpp"
//#include "tetraedr.hpp"
#include "rubik-s-cubik-2x2x2.hpp"

//#define WRITE_TABLE
//bool search_mode = false;
//State searched;

// =====================================================================
State start = Start();
State eq(const State & st){
	return st;
}

typedef State (*SimpleOperation)(const State &);
typedef vector<pair<const char *,SimpleOperation>> Algorithm;
ostream & operator<<(ostream & str, const Algorithm & alg){
	auto it = alg.begin();
	assert(it!=alg.end());
	str << it->first;
	for(it++; it!=alg.end(); it++)
		str << '*' << it->first;
	return str;
}
Algorithm make_alg(const char * name, SimpleOperation op) {
	//Algorithm alg;
	//alg.push_back(make_pair(name,op));
	//return move(alg);
	return move(Algorithm({make_pair(name,op)}));
}

struct Operation{
	Algorithm algorithm;
	bool pending = true;
#ifdef WRITE_TABLE
	vector<pair<const void * , const void * >> equations; // Algorithm
	size_t n;
#endif
	// -----
	State 
	do_it(State state){
		//clog << "do " << algorithm << " for " << state <<endl;
		for(auto it = algorithm.begin(); it!=algorithm.end(); it++){
			state = it->second(state);
			//clog << "*" << it->first << " = " << state <<endl;
		}
		//clog << "done: " << state <<endl;
		return move(state);
	}
	Operation(Algorithm alg) 
		: algorithm(alg) {
#ifdef WRITE_TABLE
		//equations.push_back(algorithm);
#endif		
	}
	Algorithm multiply(const Operation &r_op) {
		Algorithm l_alg = this->algorithm; 
		for(auto it = r_op.algorithm.begin(); it != r_op.algorithm.end(); it++)
			l_alg.push_back(*it);
		return move(l_alg);
	}
};

map<State,Operation> operations;

typedef typename map<State,Operation>::iterator OperationIter;
OperationIter eq_it;
deque<OperationIter> pending_queue;
OperationIter push_pending(State st, Algorithm alg) {
	auto X = operations.insert(make_pair(st,Operation(alg)));
	assert(X.second);
	OperationIter it = X.first;
	it->second.pending = true;
	pending_queue.push_back(it);
	return it;
}
OperationIter pop_pending() {
	OperationIter it = pending_queue.front();
	pending_queue.pop_front();
	it->second.pending = false;
	return it;
}

bool search(OperationIter l_it, OperationIter r_it){
	//clog << start <<endl;
	//clog << start << " * " << l_it->second.algorithm << " = " << l_it->first <<endl;
	State st = r_it->second.do_it(l_it->first);
	OperationIter found = operations.find(st);
	Algorithm alg = l_it->second.multiply(r_it->second);
	//clog << start << " * " << l_it->second.algorithm << " * " << r_it->second.algorithm << " = " << st << " : ";
	if(found!=operations.end()){
		//clog << "найдено " << found->second.algorithm <<endl;
		if(found->second.algorithm.size() > alg.size())
			found->second.algorithm = alg;
#ifdef WRITE_TABLE
		found->second.equations.push_back(make_pair(&*l_it,&*r_it));//alg
#endif
	}
	else{
		//clog << "не найдено "<<endl;
		found = push_pending(st,alg);
#ifdef WRITE_TABLE
		found->second.equations.push_back(make_pair(&*l_it,&*r_it));//alg
#endif
		if(search_mode && st == searched)
			return true;
	}
	return false;
}

int declen(size_t q){
	int len=1;
	while(q>=10){
		q/=10;
		len++;
	}
	return len;
}

void print_all(){
	clog << "=== итого: "<<operations.size()<<" ===" << endl;
#ifdef WRITE_TABLE
	int len = declen(operations.size()-1);
	// двумерный массив OperationIter-ов
	typedef pair<State,Operation> * OperationPointer;
	OperationIter table[operations.size()][operations.size()];
	// и заполнить n у каждого Operation
	size_t i=0;
	for(auto it = operations.begin(); it!=operations.end(); it++)
		it->second.n = i++;
#endif
	for(auto it = operations.begin(); it!=operations.end(); it++){
#ifdef WRITE_TABLE
		// по n заполнить массив
		for(auto it_alg = it->second.equations.begin(); it_alg != it->second.equations.end(); it_alg++)
			table[((OperationPointer)it_alg->first)->second.n][((OperationPointer)it_alg->second)->second.n] = it;
		cout  <<setfill('0') <<setw(len) << it->second.n << ") ";
#endif
		cout << it->first << ": " << it->second.algorithm;
#if defined(WRITE_TABLE) && 1
		sort(it->second.equations.begin(), it->second.equations.end());
		it->second.equations.erase(
			unique(it->second.equations.begin(), it->second.equations.end()),
			it->second.equations.end()
		);
		for(auto it_alg = it->second.equations.begin(); it_alg != it->second.equations.end(); it_alg++)
			cout << "\t=" << ((OperationPointer)it_alg->first)->second.algorithm << " * "
					 << ((OperationPointer)it_alg->second)->second.algorithm ;
#endif
		if(it->second.pending)
			cout << " pending";
		cout << endl;
	}
#ifdef WRITE_TABLE
	// вывести массив: 
	for(i=0; i<operations.size(); i++){
		for(size_t j=0; j<operations.size(); j++)
			cout  <<setfill('0') <<setw(len) << (i==0 ? j : (j==0 ? i : table[i][j]->second.n)) << "  ";
		cout << endl;
	}
#endif
}

int main(){
	push_pending(eq(start),make_alg("eq",eq));
	eq_it = operations.find(start);

	auto in = init();
	clog << "стартовали, start= " << start << endl;
	for(auto it = in.begin(); it!=in.end(); it++)
		push_pending(it->second(start),make_alg(it->first, it->second));
	clog << "загрузили" << endl;
	
	int tmp=0;
	while(pending_queue.size()){
		cerr << "pending: " << pending_queue.size() << " delta: " << (signed int)pending_queue.size()-tmp <<endl;
			tmp=pending_queue.size();
		//print_all();  cin.get();
		
		OperationIter cur_it = pop_pending();
		//clog << endl << "достал " << cur_it->second.algorithm << endl;
		if(eq_it==cur_it) continue;
		// слева из массива, справа текущий
		for(auto it = operations.begin(); it!=operations.end(); it++){
			//clog << "умножаю на " << it->second.algorithm << endl;
			if(eq_it==it || it->second.pending)	continue;
			if(search(cur_it,it) || search(it,cur_it))
				goto br;
		}
	}
br:
	print_all();
}
