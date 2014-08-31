typedef struct namelist_
{
	char input[5];
	char output[5];
}namelist_;
namelist_ list_greeklist[]={
{"A","Α"},
{"B","Β"},
{"C","Χ"},
{"D","Δ"},
{"E","Ε"},
{"F","Φ"},
{"G","Γ"},
{"H","Η"},
{"I","Ι"},
{"J","ϑ"},
{"K","Κ"},
{"L","Λ"},
{"M","Μ"},
{"N","Ν"},
{"O","Ο"},
{"P","Π"},
{"Q","Θ"},
{"R","Ρ"},
{"S","Σ"},
{"T","Τ"},
{"U","Υ"},
{"V","ς"},
{"W","Ω"},
{"X","Ξ"},
{"Y","Ψ"},
{"Z","Ζ"},
{"a","α"},
{"b","β"},
{"c","χ"},
{"d","δ"},
{"e","ε"},
{"f","ϕ"},
{"g","γ"},
{"h","η"},
{"i","ι"},
{"j","φ"},
{"k","κ"},
{"l","λ"},
{"m","μ"},
{"n","ν"},
{"o","ο"},
{"p","π"},
{"q","θ"},
{"r","ρ"},
{"s","σ"},
{"t","τ"},
{"u","υ"},
{"v","ϖ"},
{"w","ω"},
{"x","ξ"},
{"y","ψ"},
{"z","ζ"},
};
struct list_bookstavecode
{
	char name[12];
	char unicode[8];
};
list_bookstavecode list_xml[]
{
	{{"lt"},{"<"}},
	{{"gt"},{">"}},
	{{"amp"},{"&"}},
	{{"apos"},{"'"}},
	{{"quot"},{"\""}},
};
_small list_greeklist_size=sizeof(list_greeklist)/sizeof(list_greeklist[0]);
char * get_list_greeklist(const char * input)
{
	for (int ilv1=0;ilv1<list_greeklist_size;ilv1++)
	{
		if (strcmp(list_greeklist[ilv1].input,input)==0)
		{
			return list_greeklist[ilv1].output;
		}
	}
	return 0;
}
bienum CDXML_objectcodes_List[]={
{"CDXML",0x8000},
{"page",0x8001},
{"group",0x8002},
{"fragment",0x8003},
{"n",0x8004},
{"b",0x8005},
{"t",0x8006},
{"s",0xDEADBEEF},
{"colortable",0xDEADBEEF},
{"color",0xDEADBEEF},
{"fonttable",0xDEADBEEF},
{"font",0xDEADBEEF},
{"arrow",0x8021},
{"graphic",0x8007},
{"annotation",0x802B},
{"curve",0x8008},
{"LENHACK_ANNOTATION",0},
{"moleculefill",0},
{"EmbeddedObject",0x8009},
{"Spectrum",0x8010},
{"TLCPlate",0x8023},
{"TLCLane",0x8024},
{"TLCSpot",0x8025},
};
int CDXML_objectcodes_ListSize=sizeof(CDXML_objectcodes_List)/sizeof(CDXML_objectcodes_List[0]);
bienum CDXML_propertycodes_List[]={
{"Comment",0x9},
{"Z",0xA},
{"Warning",0x10},
{"represent",0xE},
{"p",0x200},
{"xyz",0x201},
{"BoundingBox",0x204},
{"Head3D",0x207},
{"Tail3D",0x208},
{"Center3D??",0x20D},
{"MajorAxisEnd3D??",0x20E},
{"MinorAxisEnd3D??",0x20F},
{"colortable",0x300},
{"color",0x301},
{"Charge",0x421},
{"Geometry",0x430},
{"BS",0x437},
{"NumHydrogens",0x42B},
{"s",0x700},

{"NodeElement",0x402},

{"Order",0x600},
{"Display",0x601},
{"Display2",0x602},
{"DoublePosition",0x603},
{"B",0x604},
{"E",0x605},
{"BS",0x60A},
{"CircularBondOrdering",0x60B},

{"GraphicType",0xA00},
{"LineType",0xA01},
{"ArrowType",0xA02},
{"RectangleType",0xA03},
{"OvalType",0xA04},
{"OrbitalType",0xA05},
{"SymbolType",0xA07},
{"CurveType",0xA08},
{"HeadSize",0xA20},
{"AngularSize",0xA21},
{"BracketSize",0xA22},
{"BracketUsage",0xA24},
{"ArrowheadHead??",0xA2F},
{"ArrowheadTail??//TODO",0},
{"ArrowheadType??",0xA35},
{"FillType??",0xA37},
{"TLCRf",0xAB0},
{"TLCTail",0xAB1},

{"SpectrumXSpacing",0xA80},
{"SpectrumXLow",0xA81},
{"SpectrumDataPoint",0xA86},
{"SpectrumClass",0xA87},
{"LabelStyle",0x80A},
{"CaptionStyle",0x80B},

{"Keyword",0x1500},
{"Content",0x1501},
};
int CDXML_propertycodes_ListSize=sizeof(CDXML_propertycodes_List)/sizeof(CDXML_propertycodes_List[0]);
