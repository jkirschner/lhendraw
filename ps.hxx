float ps_txposx,ps_txposy;
float ps_fontsize;
_u32 ps_linestyle=0;
float ps_currentx;
float ps_currenty;
float ps_color_red;
float ps_color_green;
float ps_color_blue;
void ps_express_text_tail()
{
	fprintf(outfile,"pop pop\n");
}
void ps_express_txinit(char ialignment,float iposx,float iposy,float iatomfontheight,float angle)
{
	SDL_txcursorx=0;
	SDL_txcursory=0;
	SDL_glyfstartx=iposx;
	SDL_glyfstarty=iposy;
	ps_txposx=iposx-4;
	ps_txposy=SVG_height-iposy-4;
	ps_fontsize=iatomfontheight;
	SDL_text_fallback=1;
	fprintf(outfile,"\nnewpath\n %f %f moveto\n0 0\n",ps_txposx,ps_txposy);
}
void ps_expressarc_enhanced(float centerx,float oldcentery,float radiusx,float radiusy,float startangle,float endangle,float tiltangle)
{
	fprintf(outfile,"\nnewpath\n");
	fprintf(outfile, "initmatrix\n");
	double centery=SVG_height-oldcentery;
	double tiltcos=cos(tiltangle);
	double tiltsin=sin(tiltangle);
	double xeffonx=tiltcos*radiusx;
	double xeffony=-tiltsin*radiusx;
	double yeffonx=tiltsin*radiusy;
	double yeffony=tiltcos*radiusy;
	double newcenterx=centerx*xeffonx+centery*yeffonx;
	double newcentery=centerx*xeffony+centery*yeffony;
	fprintf(outfile, "[%f %f %f %f %f %f] concat\n",xeffonx,xeffony,yeffonx,yeffony,centerx-newcenterx,centery-newcentery);
	fprintf(outfile,"%f %f 1 %f %f arc\n",centerx,centery,-startangle/Pi*180.0,-endangle/Pi*180.0);
	fprintf(outfile,"%1.6f %1.6f %1.6f setrgbcolor\n",ps_color_red,ps_color_green,ps_color_blue);
	if (ps_linestyle & 2)
	{
		fprintf(outfile,"fill\n");
	}
	else
	{
		fprintf(outfile,"%1.6f setlinewidth\n",(ps_linestyle&4)?0.1:0.025);
		fprintf(outfile,"stroke\n");
	}
	fprintf(outfile,"initmatrix\n");
}
void ps_expressarc(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle)
{
	ps_expressarc_enhanced(centerx,centery,radiusx,radiusy,startangle,endangle,0);
}
void ps_expressbezier(float x1,float y1,float x2,float y2,float x3,float y3,float x4,float y4)
{
}
void ps_expressbezier2(float x1,float y1,float x2,float y2,float x3,float y3)
{
}
void ps_expressbow(float ileft,float itop,float iright,float ibottom,float irelradius)
{
}
void ps_expressinfinityangle(int count)
{
	fprintf(outfile,"\nnewpath\n%f %f moveto\n",LHENDRAW_inficorn[0].x,SVG_height-LHENDRAW_inficorn[0].y);
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		fprintf(outfile,"\n%f %f lineto\n",LHENDRAW_inficorn[(ilv1+1)%count].x,SVG_height-LHENDRAW_inficorn[(ilv1+1)%count].y);
	}
	fprintf(outfile,"%1.6f %1.6f %1.6f setrgbcolor\n",ps_color_red,ps_color_green,ps_color_blue);
	if (ps_linestyle & 2)
	{
		fprintf(outfile,"fill\n");
	}
	else
	{
		fprintf(outfile,"%i setlinewidth\n",(ps_linestyle&4)?4:1);
		fprintf(outfile,"stroke\n");
	}
}
void ps_expressline(float ileft,float itop,float iright,float ibottom)
{
	itop=SVG_height-itop;
	ibottom=SVG_height-ibottom;
	fprintf(outfile,"newpath\n%f %f moveto\n%f %f lineto\n%1.6f %1.6f %1.6f setrgbcolor\n%i setlinewidth\nstroke\n",ileft,itop,iright,ibottom,ps_color_red,ps_color_green,ps_color_blue,(ps_linestyle&4)?4:1);
}
void ps_expressspinellipse(float ix,float iy,float irx,float iry,float iangle)
{
	ps_expressarc_enhanced(ix,iy,irx,iry,2*Pi,0,iangle);
}
void ps_expressellipse(float ix,float iy,float irx,float iry)
{
	ps_expressspinellipse(ix,iy,irx,iry,0);
}
void ps_expressxbezier(int icount,...)
{
}
int ps_get_colorstringv(int number)
{
	_u8 int_red=(number & 0xFF0000)>>16;
	_u8 int_green=(number & 0xFF00)>>8;
	_u8 int_blue=number & 0xFF;
	ps_color_red=int_red/255.0;
	ps_color_green=int_green/255.0;
	ps_color_blue=int_blue/255.0;
	return 0;
}
void ps_printformatted(const char * iinput,const char * parms,int imode,int start,int end)
{
	if (imode==0x20)
	{
		fprintf(outfile,"0 -6 rmoveto\n");
	}
	if (imode==0x40)
	{
		fprintf(outfile,"0 6 rmoveto\n");
	}
	fprintf(outfile,"pop\n");
	fprintf(outfile,"/Helvetica findfont\n12 scalefont\nsetfont\n(");
	for (int ilv1=start;ilv1<end;ilv1++)
	{
		char ihv1;
		ihv1=iinput[ilv1];
		if (strchr("\\)(	",ihv1)!=NULL)
		{
			fprintf(outfile,"\\%c",ihv1);goto ifertig;
		}
		if ((((_u8)ihv1) & 0xE0)==0xC0)
		{
			fprintf(outfile,"\\%03hho",(_u8)(((ihv1 & 0x3)<<6)|(iinput[ilv1+1] & 0x3F)));goto ifertig;
		}
		if ((((_u8)ihv1) & 0x80)!=0) goto ifertig;
		fwrite(&ihv1,1,1,outfile);
		ifertig:;
	}
	fprintf(outfile,") dup show stringwidth pop add 0\n");
	if (imode==0x20)
	{
		fprintf(outfile,"0 6 rmoveto\n");
	}
	if (imode==0x40)
	{
		fprintf(outfile,"0 -6 rmoveto\n");
	}
}
void ps_stylegenestring(int flags,unsigned int fillcolor=0)
{
	ps_linestyle=flags;
}
int ps_text_rewind(const _u8 * sizestring,int length,int imode)
{
	fprintf(outfile,"exch 0 exch sub exch rmoveto\n");
	fprintf(outfile,"(");
	fwrite(sizestring,1,length,outfile);
	fprintf(outfile,") stringwidth exch 0 exch sub exch rmoveto\n");
	fprintf(outfile,"0 0\n");
	return 0;
}
int __attribute__((warn_unused_result)) ps_expressgeometry_start(float left,float top,float right,float bottom)
{
	fprintf(outfile,"newpath\n");
	return 1;
}
void ps_expressgeometry_begin(float x,float y)
{
	fprintf(outfile,"%1.6f %1.6f moveto\n",x,SVG_height-y);
	gfx_geometry.startx=x;
	gfx_geometry.starty=y;
	ps_currentx=x;
	ps_currenty=y;
}
void ps_expressgeometry_arc_enhanced(float centerx,float centery,float radiusx,float radiusy,float startangle,float endangle,float tiltangle)
{
}
void ps_expressgeometry_end()
{
	fprintf(outfile,"%1.6f %1.6f %1.6f setrgbcolor\n",ps_color_red,ps_color_green,ps_color_blue);
	if (ps_linestyle & 2)
	{
		fprintf(outfile,"fill\n");
	}
	else
	{
		fprintf(outfile,"%i setlinewidth\n",(ps_linestyle&4)?4:1);
		fprintf(outfile,"stroke\n");
	}
}
void ps_expressgeometry_line(float x,float y)
{
	fprintf(outfile,"%f %f lineto\n",x,SVG_height-y);
	ps_currentx=x;
	ps_currenty=y;
}
void ps_expressgeometry_bezier2(float x1,float y1,float x2,float y2)
{
	double ax=(ps_currentx+x1*2)/3.0;
	double ay=(ps_currenty+y1*2)/3.0;
	double bx=(x2+x1*2)/3.0;
	double by=(y2+y1*2)/3.0;
	fprintf(outfile,"%1.9f %1.9f %1.9f %1.9f %1.9f %1.9f curveto\n",ax,SVG_height-ay,bx,SVG_height-by,x2,SVG_height-y2);
	ps_currentx=x2;
	ps_currenty=y2;
	if (((SVG_height-y2)==350.0) && (x2==0))
	{
		fprintf(outfile,"%%ursprungspunktfehler quadratisch\n");
	}
}
void ps_expressgeometry_bezier3(float x1,float y1,float x2,float y2,float x3,float y3)
{
	fprintf(outfile,"%1.6f %1.6f %1.6f %1.6f %1.6f %1.6f curveto\n",x1,SVG_height-y1,x2,SVG_height-y2,x3,SVG_height-y3);
	ps_currentx=x3;
	ps_currenty=y3;
	if (((SVG_height-y3)==350.0) && (x3==0))
	{
		fprintf(outfile,"%%ursprungspunktfehler kubisch\n");
	}
}
void ps_expressgeometry_backline()
{
	fprintf(outfile,"%1.6f %1.6f lineto\n",gfx_geometry.startx,SVG_height-gfx_geometry.starty);
}
