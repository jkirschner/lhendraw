//This unit does most processing of the internal data
struct drawproperties_
{
	_i32 bond_multiplicity;
	_i32 bond_Display1;
	_i32 attribute_tool;
	int color;
	_i32 Element;
};
drawproperties_ control_drawproperties={1,0,1,0,constants_Element_implicitcarbon};
int control_hot[32]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,};
int janitor_getmaxid(_u32 ino)
{
	_u32 iid=0;
	WORKIFIX_REGISTERED_TRADEMARK_workthrough_variables
	icompare=1<<ino;

	basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ino].name);
	if (tlmultilist==NULL) return -1;
	ibufferpos=(char*)((*tlmultilist).pointer);
	isize=STRUCTURE_OBJECTTYPE_List[ino].size;
	CDXMLREAD_functype tldummy;
	ioffset=(*tlmultilist).getproperties("id",&tldummy);
	if (ioffset<0) return -1;
	
	for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
	{
		if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
		{
			_u32 tlid=((*((_u32*)(ibufferpos+isize*ilv2+ioffset))));
			if (tlid>=iid)
			{
				iid=tlid+1;
			}
		}
	}
	(*tlmultilist).maxid=iid;
	return 0;
}
int edit_getnewZ()
{
	return 0;//TODO pretty urgent :)(!)
}
char getleftof(cdx_Point3D * istart,cdx_Point3D * iend,cdx_Point3D * ikink)
{
	float diff1x,diff1y,diff2x,diff2y;
	float iresult;
	diff1x=(*iend).x-(*istart).x;
	diff1y=(*iend).y-(*istart).y;
	diff2x=(*ikink).x-(*iend).x;
	diff2y=(*ikink).y-(*iend).y;
	iresult=(diff1x*diff2y-diff1y*diff2x);
	if (iresult==0) return 0;
	return (diff1x*diff2y-diff1y*diff2x>0) ? 1 : 2;//then, it is right of=> return1, otherwise, it is left of=>return2;
}



int get_bond_between(int inatom1, int inatom2)
{
	int imax=(*glob_b_multilist).filllevel;
	for (int ilv1=0;ilv1<imax;ilv1++)
	{
		if ((bond_actual_node[ilv1].start==inatom1) && (bond_actual_node[ilv1].end==inatom2))
		{
			return ilv1;
		}
		if ((bond_actual_node[ilv1].end==inatom1) && (bond_actual_node[ilv1].start==inatom2))
		{
			return ilv1;
		}
	}
}
basic_instance * gethot(int ino,int * nr=NULL)
{
	basicmultilist * tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ino].name);
	if ((control_hot[ino]>=0) && (control_hot[ino]<(*tl_multilist).filllevel))
	{
		if ((*(basic_instance*)(((char*)((*tl_multilist).pointer))+(control_hot[ino]*STRUCTURE_OBJECTTYPE_List[ino].size))).exist)
		{
			if (nr!=NULL)
			{
				(*nr)=control_hot[ino];
			}
		}
	}
	if (nr!=NULL)
	{
		*(nr)=-1;
	}
	return NULL;
}
int hit(n_instance * iinstance,float ix,float iy)
{
	if ((sqr(ix-(*iinstance).xyz.x)+sqr(iy-(*iinstance).xyz.y))<glob_clickradius)
	{
		return 1;
	}
	return 0;
}
int hit(b_instance * iinstance,float ix,float iy)
{
	n_instance *iinstance1=0;
	n_instance *iinstance2=0;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tlinstance=&((*glob_n_multilist).bufferlist[ilv1]);
		if ((*tlinstance).exist)
		{
			if ((*tlinstance).id==(*iinstance).B)
			{
				iinstance1=tlinstance;
			}
			if ((*tlinstance).id==(*iinstance).E)
			{
				iinstance2=tlinstance;
			}
		}
	}
	if (iinstance1==NULL) {return -3;}
	if (iinstance2==NULL) {return -3;}
	if ((sqr(ix-((*iinstance1).xyz.x+(*iinstance2).xyz.x)*0.5)+sqr(iy-((*iinstance1).xyz.y+(*iinstance2).xyz.y)*0.5))<40)
	{
		return 1;
	}
	return 0;
}
template <class thisinstance> inline int clickfor_template(float posx,float posy,int objecttype)
{
	char found=0;
	multilist<thisinstance> * imultilist=retrievemultilist<thisinstance>();
	for (int ilv1=0;ilv1<(*imultilist).filllevel;ilv1++)
	{
		thisinstance * tlinstance=&((*imultilist).bufferlist[ilv1]);
		if ((*tlinstance).exist)
		{
			if (hit(tlinstance,posx,posy)>0)
			{
				selection_clickselection[ilv1]|=(1<<objecttype);
				found=1;
			}
		}
	}
	return found;
}
inline int retrievepoints(n_instance * iinstance,float * ix,float * iy,int inumber)
{
	if (inumber>0) return 0;
	(*ix)=(*iinstance).xyz.x;
	(*iy)=(*iinstance).xyz.y;
	return 1;
}
inline int retrievepoints(b_instance * iinstance,float * ix,float * iy,int inumber)
{
	if (inumber>0) return 0;
	n_instance *iinstance1=0;
	n_instance *iinstance2=0;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tlinstance=&((*glob_n_multilist).bufferlist[ilv1]);
		if ((*tlinstance).exist)
		{
			if ((*tlinstance).id==(*iinstance).B)
			{
				iinstance1=tlinstance;
			}
			if ((*tlinstance).id==(*iinstance).E)
			{
				iinstance2=tlinstance;
			}
		}
	}
	if (iinstance1==NULL) {return -3;}
	if (iinstance2==NULL) {return -3;}
	(*ix)=((*iinstance1).xyz.x+(*iinstance2).xyz.x)*0.5;
	(*iy)=((*iinstance1).xyz.y+(*iinstance2).xyz.y)*0.5;
	return 1;
}

#define LOCALMACRO_1(whatabout) case STRUCTURE_OBJECTTYPE_ ## whatabout: return retrievepoints((whatabout ## _instance*)iinstance,ix,iy,inumber);break;
int retrievepoints_basic(basic_instance * iinstance,float * ix,float * iy,int inumber,int objecttype)
{
	switch (objecttype)
	{
		LOCALMACRO_1(n)
		LOCALMACRO_1(b)
	}
	return 0;
}
#undef LOCALMACRO_1
#define LOCALMACRO_1(whatabout) case STRUCTURE_OBJECTTYPE_ ## whatabout: return clickfor_template<whatabout ## _instance>(ix,iy,objecttype);break;
int clickfor(float ix,float iy,int objecttype,float iclickradius=constants_clickradius)
{
	glob_clickradius=iclickradius;
	switch (objecttype)
	{
		LOCALMACRO_1(n)
		LOCALMACRO_1(b)
	}
}
#undef LOCALMACRO_1

template <class whatabout> inline int retrievepoints_relay(whatabout * iinstance,float * ix,float * iy,int inumber)//template may become unnecessary
{
	return retrievepoints(iinstance,ix,iy,inumber);
}

n_instance * snapatom(float posx,float posy,_small * iatomnr=NULL)
{
	float best=2000000000;
	n_instance * bestatom=NULL;
	for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tlatom=&((*glob_n_multilist).bufferlist[ilv1]);
		if ((*tlatom).exist)
		{
			if (selection_clickselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_n))
			{
				float current=sqr(posx-(*tlatom).xyz.x)+sqr(posy-(*tlatom).xyz.y);
				if (current<best)
				{
					if (iatomnr!=NULL) *iatomnr=ilv1;
					best=current;
					bestatom=tlatom;
				}
			}
		}
	}
	return bestatom;
}
n_instance * snapatom_short(float iposx,float iposy,_small * iatomnr=NULL,int idistance=100)
{
	for (_small ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
	{
		n_instance * tlatom=&((*glob_n_multilist).bufferlist[ilv1]);
		if ((*tlatom).exist)
		{
			if ((sqr(iposx-(*tlatom).xyz.x)+sqr(iposy-(*tlatom).xyz.y))<idistance)
			{
				if (iatomnr!=NULL) *iatomnr=ilv1;
				return tlatom;
			}
		}
	}
	return NULL;
}

basic_instance * getclicked(int ino)
{
	_u32 compare=1<<ino;
	basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ino].name);
	for (int ilv1=0;ilv1<(*tlmultilist).filllevel;ilv1++)
	{
		if (selection_clickselection[ilv1] & compare)
		{
			basic_instance * tlinstance=(basic_instance*)(((char*)(*tlmultilist).pointer)+STRUCTURE_OBJECTTYPE_List[ino].size*ilv1);
			if ((*tlinstance).exist)
			{
				return tlinstance;
			}
		}
	}
	return NULL;
}

basic_instance * edit_locatebyid(_u32 ino,int iid,int * number=NULL)
{
	WORKIFIX_REGISTERED_TRADEMARK_workthrough_variables
	icompare=1<<ino;

	basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ino].name);
	if (tlmultilist==NULL) return NULL;
	ibufferpos=(char*)((*tlmultilist).pointer);
	isize=STRUCTURE_OBJECTTYPE_List[ino].size;
	CDXMLREAD_functype tldummy;
	ioffset=(*tlmultilist).getproperties("id",&tldummy);
	if (ioffset<0) return NULL;
	
	for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
	{
		if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
		{
			_u32 tlid=((*((_u32*)(ibufferpos+isize*ilv2+ioffset))));
			if (tlid==iid)
			{
				if (number!=NULL)
				{
					*number=ilv2;
				}
				return ((((basic_instance*)(ibufferpos+isize*ilv2))));
			}
		}
	}
	return NULL;
}

n_instance * summonatom(int * inr=NULL)
{
	if ((*glob_n_multilist).filllevel<bufferlistsize)
	{
		int tl_nr=-1;
		n_instance * tlinstance;
		if (janitor_getmaxid(STRUCTURE_OBJECTTYPE_n)==-1) return NULL;
		tl_nr=(*glob_n_multilist).filllevel;
		tlinstance=new(&((*glob_n_multilist).bufferlist[tl_nr])) n_instance;
		(*tlinstance).Element=constants_Element_implicitcarbon;
		(*tlinstance).color=control_drawproperties.color;
		selection_currentselection[tl_nr]&=(~(1<<STRUCTURE_OBJECTTYPE_n));
		atom_actual_node[tl_nr].bondcount=0;
		((*glob_n_multilist).filllevel)++;
		(*tlinstance).id=(*glob_n_multilist).maxid;
		(*glob_n_multilist).maxid++;
		if (inr!=NULL)
		{
			*inr=tl_nr;
		}
		return tlinstance;
		
	}
	return NULL;
}
arrow_instance * summonarrow(int * inr=NULL)
{
	if ((*glob_arrow_multilist).filllevel<bufferlistsize)
	{
		int tl_nr=-1;
		arrow_instance * tlinstance;
		tl_nr=(*glob_arrow_multilist).filllevel;
		tlinstance=new(&((*glob_arrow_multilist).bufferlist[tl_nr])) arrow_instance;
		selection_currentselection[tl_nr]&=(~(1<<STRUCTURE_OBJECTTYPE_arrow));
		(*tlinstance).color=control_drawproperties.color;
		((*glob_arrow_multilist).filllevel)++;
		if (inr!=NULL)
		{
			*inr=tl_nr;
		}
		return tlinstance;
	}
	return NULL;
}
b_instance * summonbond(int i_id_begin,int i_id_end,int i_nr_begin,int i_nr_end)
{
	if ((*glob_b_multilist).filllevel<bufferlistsize)
	{
		b_instance * tlinstance;
		tlinstance=new(&((*glob_b_multilist).bufferlist[(*glob_b_multilist).filllevel])) b_instance;
		selection_currentselection[(*glob_b_multilist).filllevel]&=(~(1<<STRUCTURE_OBJECTTYPE_b));
		(*tlinstance).B=i_id_begin;
		(*tlinstance).E=i_id_end;
		(*tlinstance).Display=control_drawproperties.bond_Display1;
		(*tlinstance).Order=0x10;
		(*tlinstance).color=control_drawproperties.color;
		atom_actual_node[i_nr_begin]+=((*glob_b_multilist).filllevel);
		atom_actual_node[i_nr_end]+=((*glob_b_multilist).filllevel);
		((*glob_b_multilist).filllevel)++;
		return tlinstance;
	}
	return NULL;
}
float edit_get_atom_best_free_side(int atomnr)
{
//	atom_actual_node//Should I really rely on them? better leave them as they were. But then it wont work for NEW atoms...
	float angle,angle2;
	float best=0;int bestnr=-1;float bestvalue=0;
	for (int ilv0=0;ilv0<atom_actual_node[atomnr].bondcount;ilv0++)
	{
		for (int ilv1=ilv0;ilv1<atom_actual_node[atomnr].bondcount;ilv1++)
		{
			int atomnr2=getother(atomnr,atom_actual_node[atomnr].bonds[ilv1]);
			int atomnr2b=getother(atomnr,atom_actual_node[atomnr].bonds[ilv0]);
			for (int ilv2=0;ilv2<2;ilv2++)
			{
				angle=fmod(0.5*(getangle((*glob_n_multilist).bufferlist[atomnr2].xyz.x-(*glob_n_multilist).bufferlist[atomnr].xyz.x,(*glob_n_multilist).bufferlist[atomnr2].xyz.y-(*glob_n_multilist).bufferlist[atomnr].xyz.y)+getangle((*glob_n_multilist).bufferlist[atomnr2b].xyz.x-(*glob_n_multilist).bufferlist[atomnr].xyz.x,(*glob_n_multilist).bufferlist[atomnr2b].xyz.y-(*glob_n_multilist).bufferlist[atomnr].xyz.y))+constants_angular_distance*(ilv2?1:-1)+4*Pi,2*Pi);
				float worst=2*Pi;int worstnr=-1;
				for (int ilv3=0;ilv3<atom_actual_node[atomnr].bondcount;ilv3++)
				{
					int atomnr3=getother(atomnr,atom_actual_node[atomnr].bonds[ilv3]);
					float angle2=fmod(getangle((*glob_n_multilist).bufferlist[atomnr3].xyz.x-(*glob_n_multilist).bufferlist[atomnr].xyz.x,(*glob_n_multilist).bufferlist[atomnr3].xyz.y-(*glob_n_multilist).bufferlist[atomnr].xyz.y)-angle+4*Pi,2*Pi);
					if (angle2>Pi)
					{
						angle2-=2*Pi;
					}
					angle2=fabs(angle2);
					if (angle2<worst)
					{
						worst=angle2;
						worstnr=ilv3;
					}
				}
				if (worst>best)
				{
					bestnr=worstnr;
					bestvalue=angle;
					best=worst;
				}
			}
		}
	}
	if (bestnr!=-1)
	{
		return bestvalue;
	}
	return (Pi-constants_angular_distance)/2;
}
int edit_errichten(int startatom)
{
	int atomnr2=-1;
	n_instance * tlatom=&((*glob_n_multilist).bufferlist[startatom]);
	if (tlatom==0) {return -1;}
	n_instance * tlatom2;
	b_instance * tlbond;
	if (atom_actual_node[startatom].bondcount==constants_maxbonds) return -1;
	float tlangle;
	tlangle=edit_get_atom_best_free_side(startatom);
	float posx=(*tlatom).xyz.x+constants_bondlength*cos(tlangle);
	float posy=(*tlatom).xyz.y+constants_bondlength*sin(tlangle);
	if (tlatom2=snapatom_short(posx,posy,&atomnr2,10))
	{
	}
	else
	{
		tlatom2=summonatom(&atomnr2);
		if (tlatom2)
		{
			(*tlatom2).xyz.x=posx;
			(*tlatom2).xyz.y=posy;
			(*tlatom2).xyz.z=(*tlatom).xyz.z;
			(*tlatom2).Z=(*tlatom).Z;
		}
	}
	if ((tlatom) && (tlatom2))
	{
		tlbond=summonbond((*tlatom).id,(*tlatom2).id,startatom,atomnr2);
		if (tlbond)
		{
			(*tlbond).Z=(*tlatom).Z;
		}
	}
	return atomnr2;
}
catalogized_command_funcdef(SPROUT2)
{
	int counter=0;
	if (selection_currentselection_found & (1<<STRUCTURE_OBJECTTYPE_n))
	{
		for (int ilv1=0;ilv1<(*glob_n_multilist).filllevel;ilv1++)
		{
			if (selection_currentselection[ilv1] & (1<<STRUCTURE_OBJECTTYPE_n))
			{
				if ((*glob_n_multilist).bufferlist[ilv1].exist)
				{ 
					counter++;
					edit_errichten(ilv1);
				}
			}
		}
		return counter;
	}
	int tlatom;
	gethot(STRUCTURE_OBJECTTYPE_n,&tlatom);
	if (tlatom!=-1)
	{
		int sproutcount=atoi(value);
		for (int ilv1=0;ilv1<sproutcount;ilv1++)
		{
			edit_errichten(ilv1);
		}
		return 1;
	}
	return 0;
}
catalogized_command_iterated_funcdef(SPROUT)
{
	if (edit_errichten(iindex)>=0) return 1; else return 0;
}
catalogized_command_iterated_funcdef(LABELTEXT)
{
	for (int ilv1=0;ilv1<sizeof(element)/sizeof(element_);ilv1++)
	{
		if (strcmp(value,element[ilv1].name)==0)
		{
			(*(n_instance*)iinstance).Element=ilv1;
			return 1;
		}
	}
	return 0;
}
catalogized_command_funcdef(FILE_NEW)
{
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tlmultilist)
		{
			(*tlmultilist).filllevel=0;
			//TODO: undo: buffer lists!
		}
	}
	return 1;
}
extern float control_coorsx;
extern float control_coorsy;
void checkupinconsistencies();
catalogized_command_funcdef(CHUP)
{
	checkupinconsistencies();
	return 1;
}
catalogized_command_funcdef(BLOT)
{
	int issueshiftstart();
	selection_clearselection(selection_clickselection);
	clickfor(control_coorsx,control_coorsy,STRUCTURE_OBJECTTYPE_n,1000);
	n_instance * iinstance=(n_instance*)getclicked(STRUCTURE_OBJECTTYPE_n);
	printf("|%i,%lli,%lli\n",(*glob_n_multilist).maxid,(*glob_n_multilist).filllevel,(*glob_b_multilist).filllevel);
	if (iinstance)
	{
		printf("%f,%f,%i\n",(*iinstance).xyz.x,(*iinstance).xyz.y,(*iinstance).id);
	}
	return 1;
}
void edit_scoopcolors(basic_instance * master)
{
	int ilv1,ilv2,ilv3;
	_u32 icolor;
	unsigned char r,g,b;
	CDXMLREAD_functype idummy;
	for (int ilv1=0;ilv1<8;ilv1++)
	{
		ADD_TO_MULTILISTREFERENCE(master,color);
	}
	for (ilv1=0;ilv1<8;ilv1++)//sortta explanation for da weird colortable order, roflz
	{
		r=ilv1%2;
		g=(ilv1>>1)%2;
		b=(ilv1>>2)%2;
		if (b) {r^=1;g^=r;r^=g;}else{r^=g;}
		(*glob_CAMBRIDGE_color_multilist).bufferlist[(ilv1+1)%8].b=b;
		(*glob_CAMBRIDGE_color_multilist).bufferlist[(ilv1+1)%8].g=g;
		(*glob_CAMBRIDGE_color_multilist).bufferlist[(ilv1+1)%8].r=r;
	}
	basicmultilist * tl_multilist;
	for (ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		tl_multilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tl_multilist!=NULL)
		{
			int icompare=(1<<ilv1);
			int tl_size=(*tl_multilist).itemsize;
			int ifilllevel=(*tl_multilist).filllevel;//separately, so it doesn't grow while executing the loop
			int ioffset=(*tl_multilist).getproperties("color",&idummy);
			if (ioffset!=-1)
			{
				for (ilv2=0;ilv2<ifilllevel;ilv2++)
				{
					if ((*tl_multilist)[ilv2].exist)
					{
						icolor=*(_u32*)(((char*)(&((*tl_multilist)[ilv2])))+ioffset);
						r=(icolor&0xFF0000)>>16;
						g=(icolor&0xFF00)>>8;
						b=(icolor&0xFF);
						for (ilv3=0;ilv3<(*glob_CAMBRIDGE_color_multilist).filllevel;ilv3++)
						{
							if ((((unsigned char)((*((CAMBRIDGE_color_instance*)&((*glob_CAMBRIDGE_color_multilist)[ilv3]))).r*255))==r) && 
							(((unsigned char)((*((CAMBRIDGE_color_instance*)&((*glob_CAMBRIDGE_color_multilist)[ilv3]))).g*255))==g) && 
							(((unsigned char)((*((CAMBRIDGE_color_instance*)&((*glob_CAMBRIDGE_color_multilist)[ilv3]))).b*255))==b))
							{
								goto idone;
							}
						}
						ADD_TO_MULTILISTREFERENCE(master,color);
						(*tl_CAMBRIDGE_color_instance).r=r/255.0;
						(*tl_CAMBRIDGE_color_instance).g=g/255.0;
						(*tl_CAMBRIDGE_color_instance).b=b/255.0;
						printf("r:%i,g:%i,b:%i\n",r,g,b);
						printf("r:%f,g:%f,b:%f\n",(*tl_CAMBRIDGE_color_instance).r,(*tl_CAMBRIDGE_color_instance).g,(*tl_CAMBRIDGE_color_instance).b);
						idone:;
					}
				}
			}
		}
	}
}
catalogized_command_funcdef(SAVE_TYPE)
{
	FILE * ifile=fopen(parameter,"w");
	fprintf(ifile,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
	fprintf(ifile,"<!DOCTYPE CDXML SYSTEM \"http://www.cambridgesoft.com/xml/cdxml.dtd\" >\n");
	for (int ilv1=0;ilv1<multilist_count;ilv1++)
	{
		if (multilistlist[ilv1].usage==1)
		{
			(*(multilistlist[ilv1].instance)).filllevel=0;
		}
	}
	CAMBRIDGE_colortable_instance i_CAMBRIDGE_colortable_instance=CAMBRIDGE_colortable_instance();
	(*glob_CAMBRIDGE_colortable_multilist).ADD(&i_CAMBRIDGE_colortable_instance);
	edit_scoopcolors(&i_CAMBRIDGE_colortable_instance);
	fprintf(ifile,"%s","<CDXML\n"
" CreationProgram=\"ChemDraw 12.0.2.1076\"\n"
" Name=\"whocares.cdxml\"\n"
" BoundingBox=\"5.5 155.04 724.05 736.75\"\n"
" WindowPosition=\"-1073741824 1073741824\"\n"
" WindowSize=\"0 0\"\n"
" FractionalWidths=\"yes\"\n"
" InterpretChemically=\"yes\"\n"
" ShowAtomQuery=\"yes\"\n"
" ShowAtomStereo=\"no\"\n"
" ShowAtomEnhancedStereo=\"yes\"\n"
" ShowAtomNumber=\"no\"\n"
" ShowBondQuery=\"yes\"\n"
" ShowBondRxn=\"yes\"\n"
" ShowBondStereo=\"no\"\n"
" ShowTerminalCarbonLabels=\"no\"\n"
" ShowNonTerminalCarbonLabels=\"no\"\n"
" HideImplicitHydrogens=\"no\"\n"
" LabelFont=\"3\"\n"
" LabelSize=\"18\"\n"
" LabelFace=\"96\"\n"
" CaptionFont=\"3\"\n"
" CaptionSize=\"18\"\n"
" HashSpacing=\"2.7\"\n"
" MarginWidth=\"2\"\n"
" LineWidth=\"1\"\n"
" BoldWidth=\"4\"\n"
" BondLength=\"30\"\n"
" BondSpacing=\"12\"\n"
" ChainAngle=\"120\"\n"
" LabelJustification=\"Auto\"\n"
" CaptionJustification=\"Left\"\n"
" AminoAcidTermini=\"NH2COOH\"\n"
" ShowSequenceTermini=\"yes\"\n"
" ShowSequenceBonds=\"yes\"\n"
" PrintMargins=\"10.71 12.93 10.71 12.93\"\n"
" MacPrintInfo=\"00030000025802580000000019641380FFCEFFE7199613D30367052803FC00020000025802580000000019641380000100640064000000010001010100000001270F000100010000000000000000000000000002001901900000000000600000000000000000000100000000000000000000000000000000\"\n"
" color=\"0\"\n"
" bgcolor=\"1\"\n"
"><colortable>\n");
for (int ilv1=0;ilv1<(*glob_CAMBRIDGE_color_multilist).filllevel;ilv1++)
{
	fprintf(ifile," <color r=\"%f\" g=\"%f\" b=\"%f\"/>\n",(*glob_CAMBRIDGE_color_multilist).bufferlist[ilv1].r,(*glob_CAMBRIDGE_color_multilist).bufferlist[ilv1].g,(*glob_CAMBRIDGE_color_multilist).bufferlist[ilv1].b);
}

fprintf(ifile,"%s","</colortable><fonttable>\n"
"<font id=\"3\" charset=\"windows-1258\" name=\"Arial\"/>\n"
"<font id=\"7\" charset=\"Unknown\" name=\"Symbol\"/>\n"
"</fonttable>");//TODO: font id's(!)
	//TODO: all subobjects of page must get filllevel=0 before add!
	CAMBRIDGE_page_instance i_CAMBRIDGE_page_instance=CAMBRIDGE_page_instance();
	i_CAMBRIDGE_page_instance.id=0;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,id);
	i_CAMBRIDGE_page_instance.BoundingBox.left=0;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,BoundingBox);
	i_CAMBRIDGE_page_instance.BoundingBox.top=0;
	i_CAMBRIDGE_page_instance.BoundingBox.right=28346.46;
	i_CAMBRIDGE_page_instance.BoundingBox.bottom=28346.46;
	i_CAMBRIDGE_page_instance.Width=28346.46;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,Width);
	i_CAMBRIDGE_page_instance.Height=28346.46;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,Height);
	i_CAMBRIDGE_page_instance.HeaderPosition=36;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,HeaderPosition);
	i_CAMBRIDGE_page_instance.FooterPosition=36;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,FooterPosition);
	i_CAMBRIDGE_page_instance.PageOverlap=0;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,PageOverlap);
	i_CAMBRIDGE_page_instance.PrintTrimMarks=1;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,PrintTrimMarks);
	i_CAMBRIDGE_page_instance.HeightPages=37;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,HeightPages);
	i_CAMBRIDGE_page_instance.WidthPages=48;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,WidthPages);
	i_CAMBRIDGE_page_instance.DrawingSpace=1;AUTOSTRUCT_EXISTS_SET_NAME(&i_CAMBRIDGE_page_instance,DrawingSpace);
	(*glob_CAMBRIDGE_page_multilist).ADD(&i_CAMBRIDGE_page_instance);
	CONVCAMBRIDGE_internaltomain(&i_CAMBRIDGE_page_instance);
	output_object(ifile,&i_CAMBRIDGE_page_instance);
	fprintf(ifile,"</CDXML>");
	fclose(ifile);
	return 1;
}
catalogized_command_funcdef(SAVEAS)
{
	SAVE_TYPE("Schmetzglut.cdxml","cdxml");
	printf("TODO***stub\n");
	return 1;
}
catalogized_command_funcdef(LOAD_INTO_SEARCHBUF)
{
	printf("TODO***stub\n");
}
catalogized_command_funcdef(LOAD_TYPE)
{
	FILE * infile;
	infile=fopen(parameter,"r");
	currentinstance=new(CAMBRIDGEPREFIX(Total_Document_instance));
	input_fsm(infile);
	fclose(infile);
	CAMBRIDGECONV_maintointernal();
	svg_findaround();
	getatoms();
}
catalogized_command_funcdef(LOADAS)
{
	printf("TODO***stub\n");
	return 1;
}
catalogized_command_funcdef(SAVE)
{
	printf("TODO***stub\n");
	return 1;
}
catalogized_command_funcdef(UNDO)
{
	restoreundo(~0,0);
	return 1;
}
catalogized_command_funcdef(REDO)
{
	printf("TODO***stub\n");
	return 1;
}
catalogized_command_funcdef(SELECTALL)
{
	selection_currentselection_found=0;
	_u32 icompare=0;
	int isize=0;
	for (int ilv1=1;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		icompare=1<<ilv1;
		basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		isize=STRUCTURE_OBJECTTYPE_List[ilv1].size;
		for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
		{
			if (value[0]=='1')
			{
				char tl_exbool=(*(basic_instance*)(((char*)(*tlmultilist).pointer)+ilv2*isize)).exist;
				selection_currentselection[ilv2]=icompare*tl_exbool+(selection_currentselection[ilv2]&(~icompare));
				selection_currentselection_found|=icompare*tl_exbool;
			}
			else
			{
				selection_currentselection[ilv2]=0;
			}
		}
	}
	return 1;
}
catalogized_command_funcdef(RESETDRAWTOOL)
{
	control_drawproperties.bond_multiplicity=1;
	control_drawproperties.bond_Display1=0;
	return 1;
}
catalogized_command_funcdef(TEST_SMASH)
{
	char zero=0;
	srand(counter1);
	TELESCOPE_aggressobject(glob_t_multilist,rand()%(*glob_t_multilist).filllevel);
	TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
	TELESCOPE_split(atoi(value),&zero,1);
	return 1;
}
extern int control_mousestate;
catalogized_command_funcdef(OPEN_PSE)
{
	control_mousestate=0x10;
	return 0;
}
catalogized_command_funcdef(HOME)
{
	SDL_zoomx=1;SDL_zoomy=1;SDL_scrollx=0;SDL_scrolly=0;
	return 1;
}
catalogized_command_funcdef(ONETOONE)
{
	SDL_scrollx+=(gfx_canvassizex/2)*((1/SDL_zoomx)-1);
	SDL_scrolly+=(gfx_canvassizey/2)*((1/SDL_zoomy)-1);
	SDL_zoomx=1;SDL_zoomy=1;
	return 1;
}
catalogized_command_funcdef(SEARCH)
{
	printf("TODO***stub\n");
	return 1;
}

struct edit_formatstruct
{
	_u32 color;
	_u8 face;
}__attribute__((packed));
char edit_scoop_atomstring[4];
int edit_scoop_numhydrogens;
int edit_scoop_charge;
edit_formatstruct edit_scoop_packedformats[6];
s_instance edit_scoop_formats[6];
_u8 edit_scoop_valids;//0: BIG 1: SMALL1 2: SMALL2 3: H 4: Nr 5: Charge
void processatomsymbol(int * fsm,char * pointer,s_instance * format)
{
	char ihv1;
	if ((*fsm)==6) return;
	if (pointer==NULL)
	{
		(*fsm)=6;
		return;
	}
	iback:
	if ((*pointer)==0) return;
	ihv1=(*pointer);
	if ((*fsm)==0)
	{
		if ((ihv1>='A') && (ihv1<='Z'))
		{
			edit_scoop_atomstring[0]=ihv1;
			edit_scoop_valids|=1<<0;
			edit_scoop_formats[0]=*format;
			(*fsm)=1;
			pointer++;
			goto iback;
		}
		if ((ihv1>='a') && (ihv1<='z'))//protons, neutrons or something
		{
			edit_scoop_atomstring[0]=ihv1;
			edit_scoop_valids|=1<<0;
			edit_scoop_formats[0]=*format;
			(*fsm)=5;
			pointer++;
			goto iback;
		}
		if ((ihv1=='.') || (ihv1==' ') || (ihv1==':'))
		{
			if (ihv1!=' ')
			{
				edit_scoop_atomstring[0]=ihv1;
			}
			edit_scoop_valids|=1<<0;
			edit_scoop_formats[0]=*format;
			(*fsm)=5;
			pointer++;
			goto iback;
		}
		(*fsm)=6;
		return;
	}
	if ((*fsm)==1)
	{
		if (ihv1=='H')
		{
			(*fsm)=3;
			edit_scoop_valids|=1<<3;
			edit_scoop_formats[3]=*format;
			edit_scoop_numhydrogens=1;
			pointer++;
			goto iback;
		}
		if ((ihv1>='a') && (ihv1<='z'))
		{
			edit_scoop_atomstring[2]=ihv1;
			edit_scoop_valids|=1<<1;
			edit_scoop_formats[1]=*format;
			(*fsm)=2;
			pointer++;
			goto iback;
		}
		(*fsm)=6;
		return;
	}
	if ((*fsm)==2)
	{
		if (ihv1=='H')//Some idea: would this be wise to do for fluorine?
		{
			(*fsm)=3;
			edit_scoop_valids|=1<<3;
			edit_scoop_formats[3]=*format;
			edit_scoop_numhydrogens=1;
			pointer++;
			goto iback;
		}
		if ((ihv1>='a') && (ihv1<='z'))
		{
			edit_scoop_atomstring[2]=ihv1;//Overwrites
			edit_scoop_valids|=1<<2;
			edit_scoop_formats[2]=*format;
			(*fsm)=2;
			pointer++;
			goto iback;
		}
		(*fsm)=6;
		return;
	}
	if ((*fsm)==3)
	{
		if ((ihv1>='0') && (ihv1<='9'))
		{
			edit_scoop_numhydrogens=ihv1-'0';
			edit_scoop_valids|=1<<4;
			edit_scoop_formats[4]=*format;
			(*fsm)=4;
			pointer++;
			goto iback;
		}
		(*fsm)=6;
		return;
	}
	if ((*fsm)==4)
	{
		if ((ihv1>='0') && (ihv1<='9'))
		{
			edit_scoop_numhydrogens=(edit_scoop_numhydrogens*10)+ihv1-'0';
			edit_scoop_valids|=1<<4;
			edit_scoop_formats[4]=*format;
			(*fsm)=4;
			pointer++;
			goto iback;
		}
		(*fsm)=6;
		return;
	}
	if ((*fsm)==5)
	{
		(*fsm)=6;
		return;
	}
	return;
}
int edit_interpretaselementwithimplicithydrogens(multilist<n_instance> * imultilist,int inumber)
{
	edit_scoop_valids=0;
	(*(_u32*)edit_scoop_atomstring)=0;
	edit_scoop_numhydrogens=0;
	char * ipointer;
	int i_backval=0;
	int fsm=0;//0: nothing 1: One big letter 2: One big Letter, and also small letters 3: Hydrogens-H 4: Number reached 5: other symbol, done 6: Invalid-cant interpret
	i_backval=TELESCOPE_aggressobject(imultilist,inumber);
	if (i_backval)
	{
		i_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
		if (i_backval) ipointer=(char*)TELESCOPE_getproperty_contents();
	}
	else
	{
		return 0;
	}
	iback:
	if (i_backval)
	{
		processatomsymbol(&fsm,ipointer,(s_instance*)TELESCOPE_getproperty());
/*		if ((((s_instance*)TELESCOPE_getproperty()).face & 0x60)!=0x60)
		{
			fsm=6;
		}*///This would be too strict, I think.
		i_backval=TELESCOPE_searchthroughobject_next(TELESCOPE_ELEMENTTYPE_s);
		if (i_backval)
		{
			ipointer=(char*)TELESCOPE_getproperty_contents();
			goto iback;
		}
	}
	if (fsm==6)
	{
		printf("Unknown:%s",edit_scoop_atomstring);
		return 0;
	}
	for (int ilv1=0;ilv1<sizeof(element)/sizeof(element_);ilv1++)
	{
		if (strcmp(edit_scoop_atomstring,element[ilv1].name)==0)
		{
			(*imultilist).bufferlist[inumber].Element=ilv1;
			goto yes_its_an_element;
		}
	}
	return 0;
	yes_its_an_element:;

	iback2://Then, we delete any s objects of the object
	TELESCOPE_aggressobject(imultilist,inumber);
	i_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s);
	if (i_backval)
	{
		TELESCOPE_clear_item();
		goto iback2;
	}
	TELESCOPE_aggressobject(imultilist,inumber);
	i_backval=TELESCOPE_searchthroughobject(TELESCOPE_ELEMENTTYPE_s_f);
	if (i_backval)
	{
		TELESCOPE_clear_item();
	}
	TELESCOPE_aggressobject(imultilist,inumber);
	_u8 cursor=1;
	_u8 fill=0;
	for (int ilv1=0;ilv1<6;ilv1++)
	{
		if (cursor & edit_scoop_valids)
		{
			edit_scoop_packedformats[fill].color=edit_scoop_formats[ilv1].color;
			edit_scoop_packedformats[fill].face=edit_scoop_formats[ilv1].face;
		}
		cursor<<1;
	}
	TELESCOPE_insertintoproperties(TELESCOPE_ELEMENTTYPE_s_f,(char*)&edit_scoop_packedformats,fill*sizeof(edit_formatstruct));
	(*imultilist).bufferlist[inumber].protons=edit_scoop_numhydrogens;
	(*imultilist).bufferlist[inumber].color=edit_scoop_formats[0].color;
	return 1;
}
