#include <math.h>
typedef struct framestatus_
{
	int number;
	int posx,posy;
}framestatus_;
typedef struct frameinfo_
{
	int number;
	float posx,posy;
	float endx,endy;
	float width,height;
}frameinfo_;
typedef struct makeinf_sortiment_
{
	int sortiment_length;
	float height;
	float width;
	framestatus_ sortiment[255];
} makeinf_sortiment_;
makeinf_sortiment_ makeinf_sortiment;
makeinf_sortiment_ loadedmakeinf_sortiment;
FILE * INFfile;
FILE * infile;
frameinfo_ makeinf_frame[255];
int makeinf_frame_count;
int makeinf_sortimentcount;
float makeinf_width;
float makeinf_height;
long long makeinf_fileend;
char comparemakeinf_sortiment(makeinf_sortiment_ & in1, makeinf_sortiment_ & in2)
{
	if (in1.sortiment_length!=in2.sortiment_length)
	{
		return 0;
	}
	for (int ilv1=0;ilv1<in1.sortiment_length;ilv1++)
	{
		for (int ilv2=0;ilv2<in2.sortiment_length;ilv2++)
		{
			if (in1.sortiment[ilv1].number==in2.sortiment[ilv2].number)
			{
				goto ifound;
			}
		}
		return 0;
		ifound:
		;
	}
	return 1;
}
void addframe(cdx_Rectangle & irect,int nr)
{
	makeinf_frame[makeinf_frame_count].number=nr;
	makeinf_frame[makeinf_frame_count].posx=irect.left;
	makeinf_frame[makeinf_frame_count].posy=irect.top;
	makeinf_frame[makeinf_frame_count].endx=irect.right;
	makeinf_frame[makeinf_frame_count].endy=irect.bottom;
	makeinf_frame[makeinf_frame_count].width=irect.right-irect.left;
	makeinf_frame[makeinf_frame_count].height=irect.bottom-irect.top;
	makeinf_frame_count++;
}
#ifndef CDXMAKEINF_READONLY
void getframes()
{
	char isfound;
	int iframenr;
	for (int ilv1=0;ilv1<(*glob_graphic_multilist).filllevel;ilv1++)
	{
		if ((*glob_graphic_multilist).bufferlist[ilv1].GraphicType==3)
		{
			multilistreference<annotation_instance> * tlannotationmultilistreference=dynamic_cast<multilistreference<annotation_instance>*>((*glob_graphic_multilist).bufferlist[ilv1].annotation);
			isfound=0;
			for (int ilv2=0;ilv2<(*tlannotationmultilistreference).count_in_it;ilv2++)
			{
				annotation_instance * tlannotation=&((*glob_annotation_multilist).bufferlist[(*tlannotationmultilistreference).start_in_it+ilv2]);
				if (strcmp((*tlannotation).Keyword.a,"Name")==0)
				{
					if ((strncmp((*tlannotation).Content.a,"Frame",5))==0)
					{
						iframenr=atoi((*tlannotation).Content.a+5);
						isfound=1;
					}
				}
			}
			if (isfound==1)
			{
				cdx_Rectangle * iBBX=&(((*glob_graphic_multilist).bufferlist[ilv1]).BoundingBox);
				canonicalizeboundingbox(iBBX);
				addframe(*iBBX,iframenr);
			}
		}
	}
}
void svg_prescan()
{
	getcaptions(&SVG_width,&SVG_height,&SVG_ileft,&SVG_itop);
	SVG_ileft-=10;
	SVG_itop-=10;
	SVG_width+=20;
	SVG_height+=20;
	makeinf_width=SVG_width-SVG_ileft;
	makeinf_height=SVG_height-SVG_itop;
	getframes();
}

void scanfortext(FILE * fileforthatpurpose,float x,float y,float x2,float y2)
{
	for (int ilv1=0;ilv1<(*glob_t_multilist).filllevel;ilv1++)
	{
		t_instance tl_t_instance=(*glob_t_multilist).bufferlist[ilv1];
		if (text_actual_node[ilv1].owner==-1)
		{
			printf("OHO");
			if ((tl_t_instance.p.x>x) &&
			(tl_t_instance.p.y>y) &&
			(tl_t_instance.p.x<x2) &&
			(tl_t_instance.p.y<y2))
			{
				printf("IHI");
				multilistreference<s_instance> * tl_s_multilistreference=dynamic_cast<multilistreference<s_instance>*>(tl_t_instance.s);
				for (int ilv2=(*tl_s_multilistreference).start_in_it;ilv2<(*tl_s_multilistreference).start_in_it+(*tl_s_multilistreference).count_in_it;ilv2++)
				{
					fprintf(fileforthatpurpose,"%s",(*glob_s_multilist).bufferlist[ilv2].PCTEXT.a);
				}
			}
		}
	}
}
void scanfortext_frame(FILE * fileforthatpurpose,int inumber)
{
	frameinfo_ tlframeinfo;
	tlframeinfo=makeinf_frame[inumber];
	scanfortext(fileforthatpurpose,tlframeinfo.posx,tlframeinfo.posy,tlframeinfo.endx,tlframeinfo.endy);
}
void scanfortext_picture(FILE * fileforthatpurpose)
{
	for (int ilv1=0;ilv1<makeinf_sortiment.sortiment_length;ilv1++)
	{
		frameinfo_ tlframeinfo;
		for (int ilv2=0;ilv2<makeinf_frame_count;ilv2++)
		{
			if (makeinf_frame[ilv2].number==makeinf_sortiment.sortiment[ilv1].number)
			{
				tlframeinfo=makeinf_frame[ilv2];
			}
		}
		scanfortext(fileforthatpurpose,tlframeinfo.posx,tlframeinfo.posy,tlframeinfo.endx,tlframeinfo.endy);
	}
}
#endif
char truefilename[stringlength];
/*char tetrify()
{
	float currenty,currentmaxx;
	int rows;
	int currentnr;
	currenty=0;
	currentmaxx=0;
	rows=0;
	for (int ilv1=0;ilv1<makeinf_sortiment.sortiment_length;ilv1++)
	{
		makeinf_sortiment.sortiment[ilv1].posx=currentmaxx;
		makeinf_sortiment.sortiment[ilv1].posy=currenty;
		currentnr=makeinf_sortiment.sortiment[ilv1].number;
		for (int ilv2=0;ilv2<makeinf_frame_count;ilv2++)
		{
			if (makeinf_frame[ilv2]==currentnr)
			{
				currentnr=ilv2;
				goto ifertig;
			}
		}
		ifertig:
		currenty+=makeinf_frame[ilv1].height;
		maxup(currentmaxx,makeinf_frame[ilv1].width);
	}
}*/
void makeinf(const char * cdxname,const char * name)
{
	char tldowriteatend=makeinf_sortimentcount;
	strcpy(truefilename,name);
	strcat(truefilename,".inf");
	INFfile=fopen(truefilename,"r");
	if (INFfile!=NULL)
	{
		printf("Im RAAAAVING");
		fread(&makeinf_frame_count,4,1,INFfile);
		fread(&makeinf_sortimentcount,4,1,INFfile);
		fread(&makeinf_width,sizeof(float),1,INFfile);
		fread(&makeinf_height,sizeof(float),1,INFfile);
		fread(&(makeinf_frame[0]),sizeof(frameinfo_),makeinf_frame_count,INFfile);//frameinfos are always read in.
		makeinf_fileend=8+makeinf_frame_count*sizeof(frameinfo_);
		for (int ilv1=0;ilv1<makeinf_sortimentcount;ilv1++)
		{
			fread(&loadedmakeinf_sortiment.sortiment_length,4,1,INFfile);
			fread(&loadedmakeinf_sortiment.width,sizeof(float),1,INFfile);
			fread(&loadedmakeinf_sortiment.height,sizeof(float),1,INFfile);
			makeinf_fileend+=4+2*sizeof(float);
			for (int ilv2=0;ilv2<loadedmakeinf_sortiment.sortiment_length;ilv2++)
			{
				fread(&(loadedmakeinf_sortiment.sortiment[ilv2]),sizeof(framestatus_),1,INFfile);
				makeinf_fileend+=sizeof(framestatus_);
			}
			if (comparemakeinf_sortiment(loadedmakeinf_sortiment,makeinf_sortiment))
			{
				tldowriteatend=0;
			}
		}
		fclose(INFfile);
	}
	#ifndef CDXMAKEINF_READONLY
	else
	{
		infile=fopen(cdxname,"r+");
		input_fsm(infile);
		fclose(infile);
		svg_findaround();
		getframes();
		getatoms();
		makeinf_width=SVG_width-SVG_ileft;
		makeinf_height=SVG_height-SVG_itop;
		printf("w%fŋ",makeinf_width);
		strcpy(truefilename,name);
		strcat(truefilename,".inf");
		INFfile=fopen(truefilename,"w");
		fwrite(&makeinf_frame_count,4,1,INFfile);
		fwrite(&makeinf_sortimentcount,4,1,INFfile);
		fwrite(&makeinf_width,sizeof(float),1,INFfile);
		fwrite(&makeinf_height,sizeof(float),1,INFfile);
		FILE * scraptext;
		if (makeinf_frame_count>0)
		{
			for (int ilv1=0;ilv1<makeinf_frame_count;ilv1++)
			{
				strcpy(truefilename,name);
				strcat(truefilename,"(");
				sprintf(truefilename+strlen(truefilename),"%i",makeinf_frame[ilv1].number);
				strcat(truefilename,")");
				strcat(truefilename,".txt");
				scraptext=fopen(truefilename,"w");
				scanfortext_frame(scraptext,ilv1);
				fclose(scraptext);
			}
		}
		else
		{
			strcpy(truefilename,name);
			strcat(truefilename,".txt");
			scraptext=fopen(truefilename,"w");
			scanfortext(scraptext,-INFINITY,-INFINITY,INFINITY,INFINITY);
			fclose(scraptext);
		}
		for (int ilv1=0;ilv1<makeinf_frame_count;ilv1++)
		{
			fwrite(&(makeinf_frame[ilv1]),sizeof(frameinfo_),1,INFfile);
			makeinf_fileend+=sizeof(frameinfo_);
		}
		fclose(INFfile);
	}
	#endif
	if (tldowriteatend==1)
	{
		strcpy(truefilename,name);
		strcat(truefilename,".inf");
		INFfile=fopen(truefilename,"r+");
		fseek(INFfile,0,SEEK_END);
		fwrite(&makeinf_sortiment.sortiment_length,4,1,INFfile);
		fwrite(&makeinf_sortiment.width,sizeof(float),1,INFfile);
		fwrite(&makeinf_sortiment.height,sizeof(float),1,INFfile);
		makeinf_fileend+=12;
		for (int ilv2=0;ilv2<makeinf_sortiment.sortiment_length;ilv2++)
		{
			fwrite(&(makeinf_sortiment.sortiment[ilv2]),sizeof(framestatus_),1,INFfile);
			makeinf_fileend+=sizeof(framestatus_);
		}
		fseek(INFfile,4L,SEEK_SET);
		makeinf_sortimentcount++;
		fwrite(&makeinf_sortimentcount,4,1,INFfile);
	}
}

void main_multisvg(void)
{
	//tetrify
}