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

//#define WRITE_GRAPH
//bool search_mode = false;
//State searched;

//typedef State (*SimpleOperation)(const State &);
//typedef vector<pair<const char *,SimpleOperation>> InitArr;

// =====================================================================
/* планы по распараллеливанию, а вообще с памятью надо бороться, м.б. btree заюзать, 
или вообще вместо Кэли Шраера использовать...
поиск
если не найдено, то требуется_записть, мне_требуется_запись
если требуется запись {
	sem_wait(pers_mut)
		int barrier_val;
		sem_getvalue(write_bar,&barrier_val);
		if ( 0 != barrier_val ) sem_wait(write_bar);
		if ( ++(*process_cnt) == ncores ) {
			*process_cnt=0;
			требуется_записть = false;
			добавленное = []
			sem_post(write_bar);
		}
	sem_post(pers_mut)
	sem_wait(write_bar)
		if(мне_требуется_запись){
			мне_требуется_запись = false;
			если добавленное не пусто
				пройтись по (добавленное || поиск) - что короче добавленное.size()||log2(map.size())
				и если там нет - добавить и добавить в добавленное
			иначе
				добавить и добавить в добавленное
		}
		sem_getvalue(pers_bar,&barrier_val);
		if ( 0 != barrier_val ) sem_wait(pers_bar);
		if ( ++(*process_cnt) == ncores ) {
			*process_cnt=0;
			требуется_записть = false;
			sem_post(pers_bar);
		}
	sem_post(write_bar)
	sem_wait(pers_bar)
	sem_post(pers_bar)
}

удаляется узел, когда все галочки проставлены
одновременная встречная простановка галочек:
sem_wait(flag_v_obrabotke)
если вычислен
	ставим флаг "в обработке"
	sem_post(flag_v_obrabotke)
	вычисляем, ищем
	sem_wait(vychislation)
	если найденный вычисляется
		sem_post(vychislation)
		sem_wait(vstrecha)
		если у себя не вычислено // мог поставить партнер
			у себя ставим вычислено в оба направления
			у партнера ставим вычислено в оба направления
		sem_post(vstrecha)
	иначе
		у найденного ставим вычислено
		sem_post(vychislation)
		у себя ставим вычислено
иначе
	sem_post(flag_v_obrabotke)
*/
State eq(const State & st){
	return st;
}

ostream & operator<<(ostream & str, const vector<const char * > & v){
	if(v.size()==0)	return str;
	auto it = v.begin();
	str<<*it;
	for(it++; it!=v.end(); it++)
		str<<"*"<<*it;
	return str;
}

int node_count = 1;

struct Node{
#ifndef WITHOUT_PATH
	vector<const char * > path;
#endif
	
	unsigned int flags = 0;

#ifdef WRITE_GRAPH	
	int n;
#endif

	Node()
#ifdef WRITE_GRAPH	
	:n(node_count++)
#endif
	{}

	Node(const Node & from, const char * name) 
#ifndef WITHOUT_PATH
	: path(from.path)
#ifdef WRITE_GRAPH	
	, n(node_count++)
#endif
#endif
	{
#ifndef WITHOUT_PATH
		path.push_back(name);
#endif
	}
};

map<State,Node> arr;
typedef map<State,Node>::iterator ArrIter;
deque<ArrIter> queue;

struct bits{
	int n;
	bits(int nn):n(nn){}
};
ostream & operator<<(ostream & str, bits b){
	for(int i=NUM_GENERATOR*2; i--;	)
		str<<(int)(bool)(b.n&(1<<i));
	return str;
}

int main(){
	ArrIter it = arr.insert(make_pair(start,Node())).first;
	queue.push_back(it);
#ifdef WRITE_GRAPH	
	cout << "#" << it->second.n << " " << it->first <<endl;
#endif		
	size_t deep = 0;
	while(queue.size()>0){
		it = queue.front(); queue.pop_front();
#ifndef WITHOUT_PATH
		if(it->second.path.size() > deep){
			deep = it->second.path.size();
			cerr << "глубина " << deep << ", осталось "<<arr.size() <<endl;
#ifdef MAX_DEEP
			if(deep==MAX_DEEP) goto end;
#endif
		}
#endif
		
		//clog << "start #"<<it->second.n<<":"<<bits(it->second.flags)<<endl;
		for(int i=0; i<NUM_GENERATOR; i++){
			int i2 = i*2;
			if(generators[i2].second==generators[i2+1].second){
				if( !(it->second.flags & 3<<i2) ){
					State st = generators[i2].second(it->first);
					ArrIter target = arr.find(st);
					if(target==arr.end()){
						target = arr.insert(make_pair(st,Node(it->second,generators[i2].first))).first;
						queue.push_back(target);
#ifdef WRITE_GRAPH	
						cout << "#" << target->second.n << " " << target->first <<endl;
#endif						
#ifndef WITHOUT_PATH
						if(search_mode && st==searched){
							cout << "### FOUND: " << start << "*" << target->second.path << " = "  << st <<endl;
							goto end;
						}
#endif
					}
						it->second.flags |= 3<<i2;
					target->second.flags |= 3<<i2;
					//clog << "i2 = " << i2 << "; 3<<i2 = " << bits(3<<i2) << endl;
					//clog << " #"<<it->second.n<<":"<<bits(it->second.flags)<<endl;
					//clog << " #"<<target->second.n<<":"<<bits(target->second.flags)<<endl;
#ifdef WRITE_GRAPH	
					cout << it->second.n << " " << target->second.n << " " << generators[i2].first << endl;
#endif						
				}
			}
			else{
				if( !(it->second.flags & 1<<i2) ){
					State st = generators[i2].second(it->first);
					ArrIter target = arr.find(st);
					if(target==arr.end()){
						target = arr.insert(make_pair(st,Node(it->second,generators[i2].first))).first;
						queue.push_back(target);
#ifdef WRITE_GRAPH	
						cout << "#" << target->second.n << " " << target->first <<endl;
#endif						
#ifndef WITHOUT_PATH
						if(search_mode && st==searched){
							cout << "### FOUND: " << start << "*" << target->second.path << " = "  << st <<endl;
							goto end;
						}
#endif						
					}
						it->second.flags |= 1<<i2;
					target->second.flags |= 2<<i2;
#ifdef WRITE_GRAPH	
					cout << it->second.n << " " << target->second.n << " " << generators[i2].first << endl;
#endif						
				}
				if( !(it->second.flags & 2<<i2) ){
					State st = generators[i2+1].second(it->first);
					ArrIter target = arr.find(st);
					if(target==arr.end()){
						target = arr.insert(make_pair(st,Node(it->second,generators[i2+1].first))).first;
						queue.push_back(target);
#ifdef WRITE_GRAPH	
						cout << "#" << target->second.n << " " << target->first <<endl;
#endif						
#ifndef WITHOUT_PATH
						if(search_mode && st==searched){
							cout << "### FOUND: " << start << "*" << target->second.path << " = "  << st <<endl;
							goto end;
						}
#endif						
					}
						it->second.flags |= 2<<i2;
					target->second.flags |= 1<<i2;
#ifdef WRITE_GRAPH	
					cout << it->second.n << " " << target->second.n << " " << generators[i2+1].first << endl;
#endif						
				}
			}
			//clog << "arr, queue: " <<arr.size() << " " << queue.size() <<endl;
			//cin.get();
		}//for
		//int tmp = it->second.n;
		arr.erase(it);
		//clog << "(erase #"<<tmp<<") arr, queue: " <<arr.size() << " " << queue.size() <<endl;
		//cin.get();
	}//while
	end:;
}