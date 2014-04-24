typedef struct element_
{
	int protons;
	int hasVE;
	char name[4];
	int maxbonds;
	int minfep;
}element_;
element_ element[]=
{
{0,1,{"."},1,0},
{0,1,{":"},1,0},
{1,1,{"H"},1,0},
{1,1,{"D"},1,0},
{1,1,{"T"},1,0},
{2,2,{"He"},0,0},
{3,1,{"Li"},1,0},
{4,2,{"Be"},2,0},
{5,3,{"B"},4,0},
{6,4,{""},4,0},
{6,4,{"C"},4,0},
{7,5,{"N"},4,0},
{8,6,{"O"},2,2},
{9,7,{"F"},1,3},
{10,8,{"Ne"},0,0},
{11,1,{"Na"},1,0},
{12,2,{"Mg"},2,0},
{13,3,{"Al"},4,0},
{14,4,{"Si"},4,0},
{15,5,{"P"},5,0},
{16,6,{"S"},6,0},
{17,7,{"Cl"},7,0},
{18,8,{"Ar"},0,0},
{19,1,{"K"},1,0},
{20,2,{"Ca"},2,0},
{21,3,{"Sc"},-1,0},
{22,4,{"Ti"},-1,0},
{23,5,{"V"},-1,0},
{24,6,{"Cr"},-1,0},
{25,7,{"Mn"},-1,0},
{26,8,{"Fe"},-1,0},
{27,9,{"Co"},-1,0},
{28,10,{"Ni"},-1,0},
{29,11,{"Cu"},-1,0},
{30,12,{"Zn"},2,0},
{31,3,{"Ga"},-1,0},
{32,4,{"Ge"},4,0},
{33,5,{"As"},5,0},
{34,6,{"Se"},6,0},
{35,7,{"Br"},7,0},
{36,8,{"Kr"},6,0},
{37,1,{"Rb"},1,0},
{38,2,{"Sr"},2,0},
{39,3,{"Y"},-1,0},
{40,4,{"Zr"},-1,0},
{41,5,{"Nb"},-1,0},
{42,6,{"Mo"},-1,0},
{43,7,{"Tc"},-1,0},
{44,8,{"Ru"},-1,0},
{45,9,{"Rh"},-1,0},
{46,10,{"Pd"},-1,0},
{47,11,{"Ag"},-1,0},
{48,12,{"Cd"},2,0},
{49,3,{"In"},-1,0},
{50,4,{"Sn"},-1,0},
{51,5,{"Sb"},5,0},
{52,6,{"Te"},6,0},
{53,7,{"I"},7,0},
{54,8,{"Xe"},8,0},
{55,1,{"Cs"},1,0},
{56,2,{"Ba"},2,0},
{57,3,{"La"},-1,0},
{58,4,{"Ce"},-1,0},
{59,5,{"Pr"},-1,0},
{60,6,{"Nd"},-1,0},
{61,7,{"Pm"},-1,0},
{62,8,{"Sm"},-1,0},
{63,9,{"Eu"},-1,0},
{64,10,{"Gd"},-1,0},
{65,11,{"Tb"},-1,0},
{66,12,{"Dy"},-1,0},
{67,13,{"Ho"},-1,0},
{68,14,{"Er"},-1,0},
{69,15,{"Tm"},-1,0},
{70,16,{"Yb"},-1,0},
{71,17,{"Lu"},-1,0},
{72,4,{"Hf"},-1,0},
{73,5,{"Ta"},-1,0},
{74,6,{"W"},-1,0},
{75,7,{"Re"},-1,0},
{76,8,{"Os"},-1,0},
{77,9,{"Ir"},-1,0},
{78,10,{"Pt"},-1,0},
{79,11,{"Au"},-1,0},
{80,12,{"Hg"},2,0},
{81,3,{"Tl"},-1,0},
{82,4,{"Pb"},-1,0},
{83,5,{"Bi"},-1,0},
{84,6,{"Po"},-1,0},
{85,7,{"At"},-1,0},
{86,8,{"Rn"},-1,0},
{87,1,{"Fr"},-1,0},
{88,2,{"Ra"},-1,0},
{89,3,{"Ac"},-1,0},
{90,4,{"Th"},-1,0},
{91,5,{"Pa"},-1,0},
{92,6,{"U"},-1,0},
{93,7,{"Np"},-1,0},
{94,8,{"Pu"},-1,0},
{95,-1,{"Am"},-1,0},
{96,-1,{"Cm"},-1,0},
{97,-1,{"Bk"},-1,0},
{98,-1,{"Cf"},-1,0},
{99,-1,{"Es"},-1,0},
{100,-1,{"Fm"},-1,0},
{101,-1,{"Md"},-1,0},
{102,-1,{"No"},-1,0},
{103,-1,{"Lr"},-1,0},
{104,-1,{"Rf"},-1,0},
};
