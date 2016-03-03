struct State{
	string s;
};
State Start(){ return {"ABCD"}; }
bool operator<(const State & l, const State & r){
	return l.s < r.s;
}
bool operator==(const State & l, const State & r){
	return l.s == r.s;
}
ostream & operator<<(ostream & str, const State & st){
	return str<<st.s;
}

template <typename T>
void swap3(T & x1, T & x2, T & x3){
	T x = x1;
	x1 = x2;
	x2 = x3;
	x3 = x;
}

#define ROTATE(name,x,y,z)\
State name(const State & st_in){\
	State st = st_in;\
	swap3(st.s[x],st.s[y],st.s[z]);\
	return move(st);\
}

ROTATE(RA,1,2,3);
ROTATE(LA,3,2,1);
ROTATE(LB,0,2,3);
ROTATE(RB,3,2,0);
ROTATE(RC,0,1,3);
ROTATE(LC,3,1,0);
ROTATE(LD,0,1,2);
ROTATE(RD,2,1,0);

#define SYM(name,x,y)\
State name(const State & st_in){\
	State st = st_in;\
	swap(st.s[x],st.s[y]);\
	return move(st);\
}

SYM(SAB,2,3)
SYM(SAC,1,3)
SYM(SAD,1,2)
SYM(SBC,0,3)
SYM(SBD,0,2)
SYM(SCD,0,1)

typedef State (*SimpleOperation)(const State &);
typedef vector<pair<const char *,SimpleOperation>> InitArr;
InitArr init(){
	InitArr a;
	a.push_back(make_pair("RA",RA));
	a.push_back(make_pair("LA",LA));
	a.push_back(make_pair("LB",LB));
	a.push_back(make_pair("RB",RB));
	a.push_back(make_pair("RC",RC));
	a.push_back(make_pair("LC",LC));
	a.push_back(make_pair("LD",LD));
	a.push_back(make_pair("RD",RD));

	a.push_back(make_pair("SAB",SAB));
	a.push_back(make_pair("SAC",SAC));
	a.push_back(make_pair("SAD",SAD));
	a.push_back(make_pair("SBC",SBC));
	a.push_back(make_pair("SBD",SBD));
	a.push_back(make_pair("SCD",SCD));
	return a;
}

//#define FIND_EQUATIONS
bool search_mode = false;
State searched;

