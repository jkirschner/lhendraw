//This generates the hxx file with the filestructure object definitions from filestructure.draft.
//Because this renders objects somehow obsolete, The main routine theoretically could be converted to ansi c99.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define stringlength 256
#define maxunits 256
#define bufferlength 65536
#ifdef BITMODE64
#define intl long long
#endif
#ifdef BITMODE32
#define intl long
#endif
char contents[maxunits][stringlength+1];
char properties[maxunits][stringlength+1];
char name[stringlength+1];
int namelength=0;
int properties_count=0;
int contents_count=0;
int contents_length[maxunits];
int properties_length[maxunits];
char properties_types[maxunits][stringlength+1];
int properties_type_nrs[maxunits];
char enums_registered[maxunits][stringlength+1];
int enums_registered_count=0;
char helpbuffer[65536];
char resetmode_outline[][31]={{"%1$n"},{"%2$s=0;\n%1$n"},{"clear_%3$s(%2$s);\n%1$n"},{"%2$s.a=0;\n%1$n"},{"%2$s=1;\n%1$n"},{"%2$s=0;\n%1$n"},{"%2$s.count=0;\n%1$n"},{"%2$s=0;\n%1$n"}};//very problematic: each operation must tell its length!
//ATTENTION: the second matrix array size is critical: if it is too low, the strings are simply cut off!
char * helpbufferpos;
char datablockstring[255];
char datablockstring2[255];
int internalmode=0;//switches between multilist-contents and buffer-contents. only set once in a runtime of filestructure_maker.
char linemode;//tells if the line is a buffer entry
#define uniread \
({\
	__label__ tlback;\
	unsigned char tlfirst=ihv1;\
	int tlucs;\
	int tlmask=0x3F;\
	unsigned char tlscan=0x40;\
	tlucs=tlfirst;\
	tlback:\
	fread(&ihv1,1,1,infile);\
	tlmask=(tlmask<<5)|0x1F;\
	tlscan=tlscan>>1;\
	tlucs=(tlucs<<6)|(ihv1&0x3F);\
	if (tlfirst & tlscan)\
	{\
		goto tlback;\
	}\
	tlucs&=tlmask;\
})

char register_enum(const char * input)
{
	int ilv1;
	for (int ilv1=0;ilv1<enums_registered_count;ilv1++)
	{
		if (strcmp(enums_registered[ilv1],input)==0)
		{
			return 0;
		}
	}
	strcpy(enums_registered[enums_registered_count],input);
	enums_registered_count++;
	return 1;
}

int main(int argc,char * * argv)
{
	FILE * infile,*outfile,*initfile,*propertyfile,*directoryfile,*multilistlistfile;
	char ihv1;
	int helpbufferreturnvalue;
	int propertynumber_global;//TODO: when multiple files are processed, take it from the former one
	int usage;
	propertynumber_global=0;
	if (strncmp(argv[1],"-m",2)==0)
	{
		internalmode=1;//multilistreference;
	}
	if (strncmp(argv[1],"-l",2)==0)
	{
		internalmode=2;//index to contentlist
	}
	usage=atoi(argv[1]+2);
	infile=fopen(argv[2],"r");
	outfile=fopen(argv[3],"w");
	initfile=fopen(argv[4],"a");
	multilistlistfile=fopen("generated/multilistlistfile.hxx","a");
	if (internalmode&2)
	{
		propertyfile=fopen(argv[6],"a");
		directoryfile=fopen(argv[7],"a");
	}
	datablockstring[0]=0;
	datablockstring2[0]=0;
	if (argc>=6)
	{
		if (argv[5][0]!=0)
		{
			sprintf(datablockstring,"%s_",argv[5]);
			strcpy(datablockstring2,datablockstring);
		}
		else
		{
			sprintf(datablockstring2,"CAMBRIDGE_");
		}
	}
	helpbufferpos=&helpbuffer[0];
	helpbuffer[0]=0;
	lineback:
	namelength=0;
	properties_count=0;
	contents_count=0;
	linemode=0;
	symbolback1:
	fread(&(name[namelength]),1,1,infile);
	if (feof(infile))
	{
		goto done;
	}
	if (namelength==0)
	{
		if (name[0]=='!')
		{
			linemode=1;
			goto symbolback1;
		}
	}
	if (name[namelength]!=';')
	{
		namelength++;
		goto symbolback1;
	}
	name[namelength]=0;
	contentsback:
	contents_length[contents_count]=0;
	symbolback2:
	fread(&ihv1,1,1,infile);
	if (ihv1==';')
	{
		goto contentsdone;
	}
	contents[contents_count][contents_length[contents_count]]=ihv1;
	if (contents[contents_count][contents_length[contents_count]]!=' ')
	{
		contents_length[contents_count]++;
		goto symbolback2;
	}
	contents[contents_count][contents_length[contents_count]]=0;
	contents_count++;
	goto contentsback;
	contentsdone:
	propertiesback:
	strcpy(properties_types[properties_count],"_i32");
	properties_type_nrs[properties_count]=0;
	fread(&ihv1,1,1,infile);
	if (ihv1==';')
	{
		goto propertiesdone;
	}
	switch(ihv1)
	{
		case 's' : strcpy(properties_types[properties_count],"cdx_String");properties_type_nrs[properties_count]=2;break;
		case 'S' : strcpy(properties_types[properties_count],"cdx_Buffered_String");properties_type_nrs[properties_count]=2;break;
		case 'c' : strcpy(properties_types[properties_count],"_i8");properties_type_nrs[properties_count]=1;break;
		case 'C' : strcpy(properties_types[properties_count],"_x8");properties_type_nrs[properties_count]=1;break;
		case '0' : strcpy(properties_types[properties_count],"_i32");properties_type_nrs[properties_count]=1;break;
		case '1' : strcpy(properties_types[properties_count],"float");properties_type_nrs[properties_count]=1;break;
		case '.' : strcpy(properties_types[properties_count],"cdx_Point1D");properties_type_nrs[properties_count]=2;break;
		case '2' : strcpy(properties_types[properties_count],"cdx_Point2D");properties_type_nrs[properties_count]=2;break;
		case '3' : strcpy(properties_types[properties_count],"cdx_Point3D");properties_type_nrs[properties_count]=2;break;
		case '4' : strcpy(properties_types[properties_count],"cdx_Rectangle");properties_type_nrs[properties_count]=2;break;
		case '5' : strcpy(properties_types[properties_count],"cdx_Coordinate");properties_type_nrs[properties_count]=1;break;
		case '#' : strcpy(properties_types[properties_count],"_i32");properties_type_nrs[properties_count]=4;break;
		case '~' : strcpy(properties_types[properties_count],"cdx_Bezierpoints");properties_type_nrs[properties_count]=6;break;
		case '!' : strcpy(properties_types[properties_count],"_i32");properties_type_nrs[properties_count]=5;break;//an ENUM
		case ':' : strcpy(properties_types[properties_count],":");properties_type_nrs[properties_count]=0;break;//actually a content
		case 'i' : strcpy(properties_types[properties_count],"bkchem_id");properties_type_nrs[properties_count]=1;break;//bkchem type id(sucks)
		case '\\' : 
		;
		int thisnamelength=0;
		propertiesname_back:
		fread(&properties_types[properties_count][thisnamelength],1,1,infile);
		if (properties_types[properties_count][thisnamelength]!=' ')
		{
			thisnamelength++;
			goto propertiesname_back;
		}
		properties_types[properties_count][thisnamelength]=0;properties_type_nrs[properties_count]=0;
		goto start_symbolreading3;
		default :
		;
		if (!(ihv1 & 128))
		{
			unknownType:
			{printf("Something went wrong defining %s - unknown symbol%c! ",name,ihv1);exit(1);};
		}
		int tlbackvalue=uniread;
		switch (tlbackvalue)
		{
			case 0x203c: strcpy(properties_types[properties_count],"_i32");properties_type_nrs[properties_count]=7;break;//an ENUM with two possibilities
			case 0xe7: strcpy(properties_types[properties_count],"_i8");properties_type_nrs[properties_count]=5;break;//8 bit enum
			case 0xA2: strcpy(properties_types[properties_count],"arcfloat");properties_type_nrs[properties_count]=2;break;//bin as Integer
			case 0xB0: strcpy(properties_types[properties_count],"cdx_ObjectIDArray");properties_type_nrs[properties_count]=2;break;
			default:
			printf("Something went wrong defining %s - unknown symbol 0x%04X! ",name,tlbackvalue);exit(1);
		}
	}
	fread(&ihv1,1,1,infile);//should be a blank
	if (ihv1!=' ') {printf("Something went wrong defining %s - no space after type! ",name);exit(1);};
	start_symbolreading3:
	properties_length[properties_count]=0;
	symbolback3:
	fread(&ihv1,1,1,infile);
	if (ihv1==';')
	{
		if (properties_length[properties_count]>0)
		{
			printf("Something went wrong defining %s - no space after typename!",name);exit(1);
		}
		goto propertiesdone;
	}
	properties[properties_count][properties_length[properties_count]]=ihv1;
	if (properties[properties_count][properties_length[properties_count]]!=' ')
	{
		properties_length[properties_count]++;
		goto symbolback3;
	}
	properties[properties_count][properties_length[properties_count]]=0;
	properties_count++;
	goto propertiesback;
	propertiesdone:
	fprintf(outfile,"#define STRUCTUREDEFINED_%s%s_instance\nstruct %s%s_instance:%s\n{\n        static inline const char * INTERNALgetname(){return \"%s%s\";}\n        const char * getName(){return INTERNALgetname()+%i;}\n        const char * getFullName(){return INTERNALgetname();}\n",datablockstring,name,datablockstring,name,(linemode)?"TELESCOPE_element":((internalmode&1)?"basic_instance_nested":"basic_instance"),datablockstring,name,(int)strlen(datablockstring));
	fprintf(outfile,"	const static int INTERNALPropertycount=%i;\n",properties_count);
	fprintf(outfile,"	int getINTERNALPropertycount(){return INTERNALPropertycount;}\n");
	if (internalmode&1)
	{
		fprintf(outfile,"	_u32 INTERNALPropertyexistflags;\n	virtual _u32 * getINTERNALPropertyexistflags(){return &INTERNALPropertyexistflags;}\n");
	}
	if (internalmode&1)
	{
		for (int ilv1=0;ilv1<contents_count;ilv1++)
		{
			fprintf(outfile,"        basicmultilistreference * %s;\n",contents[ilv1]);
		}
	}
	for (int ilv1=0;ilv1<properties_count;ilv1++)
	{
		fprintf(outfile,"        %s %s;\n",(properties_type_nrs[ilv1]!=0)?properties_types[ilv1]:"//",properties[ilv1]);
	}
	
	if (!linemode)
	{
		if ((internalmode&1)==0)
		{
			fprintf(outfile,"        static AUTOSTRUCT_cstyle_vtable INTERNAL_cstyle_vtable;\n      static superconstellation properties[];\n      AUTOSTRUCT_cstyle_PROPERTY_ROUTINE(%s%s_instance,%i);\n       %s%s_instance();\n        ~%s%s_instance(){}\n};\nAUTOSTRUCT_cstyle_vtable %s%s_instance::INTERNAL_cstyle_vtable={%s%s_instance::properties,%i,NULL,0,\"%s%s\",\"%s\"};\n",datablockstring,name,properties_count,datablockstring,name,datablockstring,name,datablockstring,name,datablockstring,name,properties_count,datablockstring,name,name);
		}
		if (internalmode&1)
		{
			fprintf(outfile,"        static AUTOSTRUCT_cstyle_vtable INTERNAL_cstyle_vtable;\n      AUTOSTRUCT_GET_ROUTINE(contents,%i)\n        AUTOSTRUCT_PROPERTY_ROUTINE(%i)\n        %s%s_instance();\n        ~%s%s_instance(){}\n};\nAUTOSTRUCT_cstyle_vtable %s%s_instance::INTERNAL_cstyle_vtable={%s%s_instance::properties,%i,%s%s_instance::contents,%i,\"%s%s\",\"%s\"};\nsuperconstellation %s%s_instance::contents[]={\n",contents_count,properties_count,datablockstring,name,datablockstring,name,datablockstring,name,datablockstring,name,properties_count,datablockstring,name,contents_count,datablockstring,name,name,datablockstring,name);
			for (int ilv1=0;ilv1<contents_count;ilv1++)
			{
				fprintf(outfile,"{\"%s\",offsetof(%s%s_instance,%s)}%s\n",contents[ilv1],datablockstring,name,contents[ilv1],(ilv1==contents_count-1) ? "" : ",");
			}
			fprintf(outfile,"};\n");
		}
	}
	else
	{
		fprintf(outfile,"        static AUTOSTRUCT_cstyle_vtable INTERNAL_cstyle_vtable;\n	static superconstellation properties[];\n      %s%s_instance();\n        ~%s%s_instance(){}\n};\nAUTOSTRUCT_cstyle_vtable %s%s_instance::INTERNAL_cstyle_vtable={%s%s_instance::properties,%i,NULL,0,\"%s%s\",\"%s\"};\n",datablockstring,name,datablockstring,name,datablockstring,name,datablockstring,name,properties_count,datablockstring,name,name);
	}
	for (int ilv1=0;ilv1<properties_count;ilv1++)
	{
		if ((properties_type_nrs[ilv1]==5) || (properties_type_nrs[ilv1]==4) || (properties_type_nrs[ilv1]==7))
		{
			if (register_enum(properties[ilv1]))
			{
				fprintf(outfile,"#ifndef FUNCTIONDEFINED_AUTOMATICALLY_CDXMLREAD_ENUM_%s%s\n#define FUNCTIONDEFINED_AUTOMATICALLY_CDXMLREAD_ENUM_%s%s\nint __attribute__((sysv_abi))CDXMLREAD_ENUM_%s%s(char * input,void * output)\n{\n        \
	*((_i32 *)output)=get_bienum%s(%s%s,input,%s%s_max);\n	return 0;\n}\n\
int __attribute__((sysv_abi))CDXMLWRITE_ENUM_%s%s(char * input,void * output)\n{\n        \
	return set_bienum%s(%s%s,(FILE*)output,%s%s_max,*((_i32 *)input));\n}\n#endif\n",datablockstring2,properties[ilv1],datablockstring2,properties[ilv1],datablockstring2,properties[ilv1],(properties_type_nrs[ilv1]==7)?"_multi":"",datablockstring2,properties[ilv1],datablockstring2,properties[ilv1],datablockstring2,properties[ilv1],(properties_type_nrs[ilv1]==7)?"_multi":"",datablockstring2,properties[ilv1],datablockstring2,properties[ilv1]);
			}
		}
	}
	fprintf(outfile,"superconstellation %s%s_instance::properties[]={\n",datablockstring,name);
	if (linemode)
	{
		fprintf(directoryfile,"{%i,%i,\"%s\"},\n",propertynumber_global,propertynumber_global+properties_count,name);//TODO: make this consistent to the structure type numbers
		propertynumber_global+=properties_count;
	}
	for (int ilv1=0;ilv1<properties_count;ilv1++)
	{
		FILE * thisfile;
		if ((internalmode&2)&&(linemode))
		{
			thisfile=propertyfile;
		}
		else
		{
			thisfile=outfile;
		}
		if (strcmp(properties_types[ilv1],":")==0)
		{
			fprintf(thisfile,"{\"%s\",offsetof(%s%s_instance,%s),0,CDXMLREAD_INVALID,CDXMLWRITE_INVALID,CDXMLREAD_BIN_%s,CDXMLWRITE_BIN_%s},\n",properties[ilv1],datablockstring,name,properties[ilv1],properties[ilv1],properties[ilv1]);
		}
		else
		if ((properties_type_nrs[ilv1]==5) || (properties_type_nrs[ilv1]==4) || (properties_type_nrs[ilv1]==7))
		{
			fprintf(thisfile,"{\"%s\",offsetof(%s%s_instance,%s),sizeof(%s),CDXMLREAD_ENUM_%s%s,CDXMLWRITE_ENUM_%s%s,CDXMLREAD_BIN__i32,CDXMLWRITE_BIN__i16},\n",properties[ilv1],datablockstring,name,properties[ilv1],properties_types[ilv1],datablockstring2,properties[ilv1],datablockstring2,properties[ilv1]);
		}
		else
		{
			fprintf(thisfile,"{\"%s\",offsetof(%s%s_instance,%s),sizeof(%s),CDXMLREAD_%s,CDXMLWRITE_%s,CDXMLREAD_BIN_%s,CDXMLWRITE_BIN_%s},\n",properties[ilv1],datablockstring,name,properties[ilv1],properties_types[ilv1],(properties_type_nrs[ilv1]!=0)?properties_types[ilv1]:"_i32",(properties_type_nrs[ilv1]!=0)?properties_types[ilv1]:"_i32",properties_types[ilv1],properties_types[ilv1]);
		}
	}
	fprintf(outfile,"};\n");
	sprintf(helpbufferpos,"%s%s_instance::%s%s_instance()\n{\n%n",datablockstring,name,datablockstring,name,&helpbufferreturnvalue);
	helpbufferpos+=helpbufferreturnvalue;
	(*helpbufferpos)=0;
	if (internalmode&1)
	{
		sprintf(helpbufferpos,"	INTERNALPropertyexistflags=0;\n%n",&helpbufferreturnvalue);
		helpbufferpos+=helpbufferreturnvalue;
		(*helpbufferpos)=0;
	}
	if (internalmode&1)
	{
		for (int ilv1=0;ilv1<contents_count;ilv1++)
		{
			sprintf(helpbufferpos,"        %s=new(multilistreference<%s%s_instance>);\n%n",contents[ilv1],datablockstring,contents[ilv1],&helpbufferreturnvalue);//TODO mem: leaks
			helpbufferpos+=helpbufferreturnvalue;
			(*helpbufferpos)=0;
		}
	}
	if ((internalmode&2) && (!(linemode)))
	{
		sprintf(helpbufferpos,"	TELESCOPE_buffer * ibuffer;\n	getbufferfromstructure(retrievemultilist<%s%s_instance>(),&ibuffer);\n	pos_in_buffer=(*ibuffer).count;\n%n",datablockstring,name,&helpbufferreturnvalue);
		helpbufferpos+=helpbufferreturnvalue;
		(*helpbufferpos)=0;
	}
	for (int ilv1=0;ilv1<properties_count;ilv1++)
	{
		sprintf(helpbufferpos,"        %n",&helpbufferreturnvalue);
		helpbufferpos+=helpbufferreturnvalue;
		(*helpbufferpos)=0;
		sprintf(helpbufferpos,resetmode_outline[properties_type_nrs[ilv1]],&helpbufferreturnvalue,properties[ilv1],properties_types[ilv1]);
		helpbufferpos+=helpbufferreturnvalue;
		(*helpbufferpos)=0;
	}
	if (!linemode)
	{
		sprintf(helpbufferpos,"	_=&INTERNAL_cstyle_vtable;\n%n",&helpbufferreturnvalue);
		helpbufferpos+=helpbufferreturnvalue;
	}
	sprintf(helpbufferpos,"}\n%n",&helpbufferreturnvalue);
	helpbufferpos+=helpbufferreturnvalue;
	(*helpbufferpos)=0;
	if (linemode!=1)
	{
		sprintf(helpbufferpos,"multilist<%s%s_instance> globstat_%s%s_multilist=multilist<%s%s_instance>();\n%n",datablockstring,name,datablockstring,name,datablockstring,name,&helpbufferreturnvalue);//TODO mem: leaks
		helpbufferpos+=helpbufferreturnvalue;
		sprintf(helpbufferpos,"multilist<%s%s_instance> * glob_%s%s_multilist=&globstat_%s%s_multilist;\n%n",datablockstring,name,datablockstring,name,datablockstring,name,&helpbufferreturnvalue);
		helpbufferpos+=helpbufferreturnvalue;
	}
//	fprintf(initfile,"glob_%s%s_multilist=new multilist<%s%s_instance>;\n",datablockstring,name,datablockstring,name);
	(*helpbufferpos)=0;
	if (fread(&ihv1,1,1,infile)==0){goto done;};
	if (ihv1!='\n') {if(!feof(infile)){printf("no linebreak");exit(1);}else goto done;}
	if (linemode!=1)
	fprintf(multilistlistfile,"{\"%s%s\",&globstat_%s%s_multilist,%i},\n",datablockstring,name,datablockstring,name,usage);
	if (!feof(infile))
	{
		goto lineback;
	}
	done:
	fprintf(outfile,"%s",helpbuffer);
	fclose(infile);
	fclose(outfile);
	fclose(initfile);
	fclose(multilistlistfile);
	if (internalmode&2)
	{
		fclose(propertyfile);
		fclose(directoryfile);
	}
	return 0;
}
