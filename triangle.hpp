struct State{
	string s;
};
bool operator<(const State & l, const State & r){
	return l.s < r.s;
}
bool operator==(const State & l, const State & r){
	return l.s == r.s;
}
ostream & operator<<(ostream & str, const State & st){
	return str<<st.s;
}

State symA(const State & st_in){
	State st = st_in;
	swap(st.s[1],st.s[2]);
	return move(st);
}
State symB(const State & st_in){
	State st = st_in;
	swap(st.s[0],st.s[2]);
	return move(st);
}
State symC(const State & st_in){
	State st = st_in;
	swap(st.s[1],st.s[0]);
	return move(st);
}

typedef State (*SimpleOperation)(const State &);
#define NUM_GENERATOR 3
pair<const char *,SimpleOperation> generators[NUM_GENERATOR*2] = {
	{"SA",symA},	{"SA",symA},
	{"SB",symB},	{"SB",symB},
	{"SC",symC},	{"SC",symC},
};
State start = {"ABC"};

//#define WRITE_TABLE
#define WRITE_GRAPH
bool search_mode = false;
State searched;

