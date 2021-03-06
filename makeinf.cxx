//Makes inf files which tell the sizes of the pictures in my doctoral thesis.

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <typeinfo>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#define CAMBRIDGEPREFIX(contents) contents
#define CAMBRIDGEPREFIX2(content,content2) content ## _ ## content2
#include "lendefs.h"
#include "xmldata.hxx"
#include "cdxdata.hxx"
#include "enums.hxx"
#include "./generated/filestructure.hxx"
#include "./generated/internalstructure.hxx"
#include "parsecdxml_variables.hxx"

#include "ellipeq.h"
#include "quartic.hxx"
#include "definitionlist.h"
#include "janitor.hxx"
#include "graphics_variables.hxx"
#include "createsvg.hxx"
#include "hatch.hxx"

#include "xmlparse.hxx"


#include "makeinf.hxx"
int createsortiment(char sortiment_count,char * * sortimentstart)
{
	if (sortiment_count==0)
	{
		makeinf_sortimentcount=0;
		return 0;
	}
	makeinf_sortimentcount=1;
	makeinf_sortiment.sortiment_length=0;
	for (int ilv1=0;ilv1<sortiment_count;ilv1++)
	{
		makeinf_sortiment.sortiment_length++;
		makeinf_sortiment.sortiment[ilv1].number=atoi(sortimentstart[ilv1]);
		//TODO: calculate scaling...
	}
	return 1;
}
int main(int argc,char * * argv)
{
	FILE * infile;
	setvbuf(stdout,NULL,_IONBF,0);
	makeinf_frame_count=0;
	makeinf_sortimentcount=0;
	createsortiment(argc-3,argv+3);
	makeinf(argv[1],argv[2]);
}
