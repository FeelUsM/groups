#include <string.h>

struct State{
	char s[25];
};

State Start(){ 
	State st;
	strcpy(st.s,"111122334455223344556666");
	return st; 
}
bool operator<(const State & l, const State & r){
	return strcmp(l.s,r.s)<0;
}
bool operator==(const State & l, const State & r){
	return strcmp(l.s,r.s)==0;
}
ostream & operator<<(ostream & str, const State & st){
	return str<<st.s;
}

template <typename T>
void swap4(T & x1, T & x2, T & x3, T & x4){
	T x = x1;
	x1 = x2;
	x2 = x3;
	x3 = x4;
	x4 = x;
}

#define ROTATE(name, x1,y1,z1,q1, x2,y2,z2,q2, x3,y3,z3,q3)\
State name(const State & st_in){\
	State st = st_in;\
	swap4(st.s[x1],st.s[y1],st.s[z1],st.s[q1]);\
	swap4(st.s[x2],st.s[y2],st.s[z2],st.s[q2]);\
	swap4(st.s[x3],st.s[y3],st.s[z3],st.s[q3]);\
	return move(st);\
}
//	clog << #name " 0) " << st << endl; 


ROTATE(UL, 00,01,02,03, 04,06, 8,10, 05,07, 9,11);
ROTATE(UR, 03,02,01,00, 10, 8,06,04, 11, 9,07,05);
ROTATE(DL, 20,21,22,23, 12,14,16,18, 13,15,17,19);
ROTATE(DR, 23,22,21,20, 18,16,14,12, 19,17,15,13);
ROTATE(FL,  9, 8,16,17, 03,07,22,18, 02,15,23,10);
ROTATE(FR, 17,16, 8, 9, 18,22,07,03, 10,23,15,02);
ROTATE(BL, 05,13,12,04, 01,14,20,11, 00,06,21,19);
ROTATE(BR, 04,12,13,05, 11,20,14,01, 19,21,06,00);
ROTATE(RL, 07,06,14,15,  8,01,13,22, 16,02,05,21);
ROTATE(RR, 15,14,07,06, 22,13,01, 8, 21,05,02,16);
ROTATE(LL, 10,11,19,18, 17,03,04,20,  9,00,12,23);
ROTATE(LR, 18,19,11,10, 20,04,03,17, 23,12,00, 9);


typedef State (*SimpleOperation)(const State &);
#define NUM_GENERATOR 3
pair<const char *,SimpleOperation> generators[NUM_GENERATOR*2] = {
	{"UL",UL},	{"UR",UR},
	//{"DL",DL},	{"DR",DR},
	{"FL",FL},	{"FR",FR},
	//{"BL",BL},	{"BR",BR},
	{"RL",RL},	{"RR",RR},
	//{"LL",LL},	{"LR",LR},
};

State start = Start();


//#define WRITE_GRAPH
//#define FIND_EQUATIONS
#define MAX_DEEP 11
#define WITHOUT_PATH
bool search_mode = false;
State searched;

