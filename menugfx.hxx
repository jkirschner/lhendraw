//any graphics output which is not data of the document
void text_output_bitmap(int * posx,int * posy,fontpixinf_ * ifontpixinf)
{
	int ilv1,ilv2;
	int scanx,scany;
	int maxx,maxy;
	int iscreenskip=gfx_screensizex;
	unsigned int * iscreen=screen;
	int skip=(*ifontpixinf).sizex;
	char * mempos=(*ifontpixinf).memstart;
	scanx=(*posx)+(*ifontpixinf).pivotx;
	scany=(*posy)+(*ifontpixinf).pivoty;
	maxx=(*ifontpixinf).sizex+scanx;
	maxy=(*ifontpixinf).sizey+scany;
	if (scanx<0) {mempos-=scanx;scanx=0;}
	if (scany<0) {mempos-=scany*skip;scany=0;}
	if (maxx>=gfx_screensizex) maxx=gfx_screensizex-1;
	if (maxy>=gfx_screensizey) maxy=gfx_screensizey-1;
	skip-=(*ifontpixinf).sizex;
	(*posx)+=(*ifontpixinf).deltax;
	(*posy)+=(*ifontpixinf).deltay;
	if ((scanx>=maxx) || (scany>=maxy))
	{
		return;
	}
	iscreen+=iscreenskip*scany+scanx;
	iscreenskip-=(*ifontpixinf).sizex;
	for (ilv1=scany;ilv1<maxy;ilv1++)
	{
		for (ilv2=scanx;ilv2<maxx;ilv2++)
		{
			if (*(mempos) & 0x80)
			{
				*(iscreen)=SDL_color;
			}
			iscreen++;
			mempos++;
		}
		mempos+=skip;
		iscreen+=iscreenskip;
	}
}
void printmenutext(int posx,int posy,const char * iinput,const char * parms,int imode,int start,int end)
{
	int ilv4=start;
	char linebreak;
	int iposx=posx;
	int iposy=posy;
	thatwasatemporaryskip:
	linebreak=0;
	for (;ilv4<end;ilv4++)
	{
		if (iinput[ilv4]==10)
		{
			ilv4++;
			linebreak=1;
			goto skipfornow;
		}
		text_output_bitmap(&iposx,&iposy,&fontpixinf[indexfromunicode(((unsigned char)iinput[ilv4]))]);
	}
	skipfornow:
	if (linebreak) {if (ilv4<end) goto thatwasatemporaryskip;}//a line break;
}
int sdl_toolboxitemdraw(int posx,int posy,int gfxno,_u32 state)
{
	_u32 * ibutton=resources_bitmap_buttons[0][0];
	_u32 * ihgd=resources_bitmap_buttons[1][0];
	int idelta=(state & 0xD)>>1;
	for (int ilv2=0;ilv2<32-idelta;ilv2++)
	{
		for (int ilv3=0;ilv3<32-idelta;ilv3++)
		{
			_u32 tlgfx=resources_bitmap_buttons[selection_maxbuttons-gfxno][ilv2][ilv3];
			_u8 tlgfr=(tlgfx>>16) & 0xFF;
			_u8 tlgfg=(tlgfx>>8) & 0xFF;
			_u8 tlgfb=tlgfx & 0xFF;
			if ((state & 1)==1)
			{
				tlgfr=((1+(((tlgfx>>24) & 0xFF)))*tlgfr+((state & 0xFF000000)>>24)*(0xFF-((tlgfx>>24) & 0xFF)))>>8;
				tlgfg=((1+(((tlgfx>>24) & 0xFF)))*tlgfg+((state & 0xFF0000)>>16)*(0xFF-((tlgfx>>24) & 0xFF)))>>8;
				tlgfb=((1+(((tlgfx>>24) & 0xFF)))*tlgfb+((state & 0xFF00)>>8)*(0xFF-((tlgfx>>24) & 0xFF)))>>8;
			}
			screen[gfx_screensizex*(posy+ilv2+idelta)+posx+ilv3+idelta]=((tlgfx & 0xFF000000)==0x00000000) ? resources_bitmap_buttons[selection_maxbuttons-17][ilv2][ilv3]:((tlgfr<<16)+(tlgfg<<8)+(tlgfb));
		}
	}
	if (idelta>0)
	{
		for (int ilv1=0;ilv1<idelta;ilv1++)
		{
			for (int ilv2=0;ilv2<32;ilv2++)
			{
				screen[gfx_screensizex*(posy+ilv1)+posx+ilv2]=resources_bitmap_buttons[selection_maxbuttons-16][ilv1][ilv2];
				screen[gfx_screensizex*(posy+ilv2)+posx+ilv1]=resources_bitmap_buttons[selection_maxbuttons-16][ilv2][ilv1];
			}
		}
	}
	return 1;
}
int sdl_sliderdraw(int posx,int posy,int gfxno,int shift)
{
	_u32 * ibutton=resources_bitmap_buttons[selection_maxbuttons-gfxno][0];
	int xco,yco;
	if (shift>=0)
	{
		shift=shift%8;
	}
	else
	{
		shift=8-((-shift)%8);
	}
	for (int ilv1=0;ilv1<32;ilv1++)
	{
		if (ilv1<8)
		{
			yco=ilv1+8;
			if (yco>=12) yco=11;
		}
		else if (ilv1>=24)
		{
			yco=ilv1-16;
			if (yco<12) yco=12;
		}
		else
		{
			for (int ilv2=0;ilv2<gfx_canvassizex;ilv2++)
			{
				screen[gfx_screensizex*(posy+ilv1)+posx+ilv2]=*(ibutton+(ilv2+shift)%8);
			}
			goto ifertig;
		}
		for (int ilv2=0;ilv2<gfx_canvassizex;ilv2++)
		{
			if (ilv2<8)
			{
				xco=ilv2;
				if (xco>=4) xco=3;
			}
			else if (ilv2>=gfx_canvassizex-4)
			{
				xco=ilv2-(gfx_canvassizex-8);
				if (xco<4) xco=4;
			}
			screen[gfx_screensizex*(posy+ilv1)+posx+ilv2]=*(ibutton+yco*32+xco);
		}
		ifertig:;
	}
	return 1;
}
int quersum(_u32 input,int max=32)
{
	char wert=0;
	_u32 iinput=input;
	for (int ilv1=0;ilv1<max;ilv1++)
	{
		wert+=((iinput & 1)==1);
		iinput=iinput>>1;
	}
	return wert;
}
int sdl_textmenudraw(AUTOSTRUCT_PULLOUTLISTING_ * ilisting,int count,int xpos=0,int ypos=0)
{
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		switch (ilisting[ilv1].lmbmode)
		{
			case 5:
			{
				_u32 * iscreen=screen+(ypos+ilisting[ilv1].y*16)*gfx_screensizex+xpos+ilisting[ilv1].x*192;
				if ((*((_u32*)ilisting[ilv1].variable)) & (1<<ilv1))
				{
					SDL_color=0xFFFFFF;
					for (int ilv2=0;ilv2<16;ilv2++)
					{
						for (int ilv3=0;ilv3<192;ilv3++)
						{
							*(iscreen)=0;
							iscreen++;
						}
						iscreen+=gfx_screensizex-192;
					}
				}
				else
				{
					SDL_color=0x000000;
					for (int ilv2=0;ilv2<16;ilv2++)
					{
						for (int ilv3=0;ilv3<192;ilv3++)
						{
							*(iscreen)=0xFFFFFF;
							iscreen++;
						}
						iscreen+=gfx_screensizex-192;
					}
				}
			}
			break;
			default:
			SDL_color=0x000000;
		}
		printmenutext(ilisting[ilv1].x*192+xpos,ilisting[ilv1].y*16+ypos+12,ilisting[ilv1].name,NULL,0,0,strlen(ilisting[ilv1].name));
	}
}
int sdl_textbuttonmenudraw(AUTOSTRUCT_PULLOUTLISTING_ * ilisting,int count,int xpos=0,int ypos=0)
{
	SDL_color=0;
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		int left,top,right,bottom;
		left=xpos+ilisting[ilv1].x;
		top=ypos+ilisting[ilv1].y;
		right=xpos+ilisting[ilv1].maxx;
		bottom=ypos+ilisting[ilv1].maxy;
		_u32 * ibutton=resources_bitmap_buttons[selection_maxbuttons-34][0];
		int xco,yco;
		for (int ilv1=0;ilv1<32;ilv1++)
		{
			if (ilv1<8)
			{
				yco=ilv1+8;
				if (yco>=12) yco=11;
			}
			else if (ilv1>=24)
			{
				yco=ilv1-16;
				if (yco<12) yco=12;
			}
			else
			{
				yco==11;
			}
			for (int ilv2=0;ilv2<right-left;ilv2++)
			{
				if (ilv2<8)
				{
					xco=ilv2;
					if (xco>=4) xco=3;
				}
				else if (ilv2>=gfx_canvassizex-4)
				{
					xco=ilv2-(gfx_canvassizex-8);
					if (xco<4) xco=4;
				}
				screen[gfx_screensizex*(top+ilv1)+left+ilv2]=*(ibutton+yco*32+xco);
			}
		}
		printmenutext(left,top+12,ilisting[ilv1].name,NULL,0,0,strlen(ilisting[ilv1].name));
	}
}
int sdl_listmenudraw(AUTOSTRUCT_PULLOUTLISTING_ * ilisting,int count,int xpos=0,int ypos=0)
{
	int ilv2;
	structenum istructenum;
	int bgcolor=0;
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		istructenum=(*(structenum*)(ilisting[ilv1].variable));
		for (ilv2=0;ilv2*16+ypos+ilisting[ilv1].y+100<gfx_screensizey;ilv2++)//TODO: actual listlength
		{
			if (ilv2==istructenum.number)
			{
				bgcolor=0xFF;
			}
			else
			{
				bgcolor=0;
			}
			_u32 * iscreen=screen+((ypos+ilisting[ilv1].y+ilv2*16))*gfx_screensizex+xpos+ilisting[ilv1].x;
			SDL_color=ilisting[ilv1].bgcolor;
			for (int ilv3=0;ilv3<16;ilv3++)
			{
				for (int ilv4=0;ilv4<192;ilv4++)
				{
					*(iscreen)=bgcolor;
					iscreen++;
				}
				iscreen+=gfx_screensizex-192;
			}
			if (ilv2<istructenum.count)
			{
				printmenutext(ilisting[ilv1].x+xpos,(ilisting[ilv1].y+ilv2*16)+ypos+12,((char*)(istructenum.pointer))+istructenum.size*ilv2,NULL,0,0,strlen(((char*)(istructenum.pointer))+istructenum.size*ilv2));
			}
		}
	}
	return 1;
}
void sdl_colorpaldraw(int posx,int posy)
{
	_u32 icolor;
	for (int ilv1=0;ilv1<256;ilv1++)
	{
		for (int ilv2=0;ilv2<256;ilv2++)
		{
			if (control_usingmousebutton==SDL_BUTTON_LEFT)
			{
				icolor=control_drawproperties.color&(0xFFFF0000);
				icolor+=ilv2 & 0xFF;
				icolor+=(ilv1 & 0xFF)<<8;
			}
			else
			{
				icolor=control_drawproperties.color&(0xFFFF);
				icolor+=(ilv2 & 0xFF)<<16;
				icolor+=(ilv1 & 0xFF)<<24;
			}
			screen[posx+ilv2+(posy+ilv1)*gfx_screensizex]=icolor;
		}
	}
}
int sdl_buttonmenudraw(AUTOSTRUCT_PULLOUTLISTING_ * ilisting,int count,int xpos=0,int ypos=0)
{
	for (int ilv1=0;ilv1<count;ilv1++)
	{
		_u32 state=ilisting[ilv1].bgcolor;
		state=(*ilisting).bgcolor<<8;
		int tlvar=ilisting[ilv1].lmbmode;
		iagain:
		switch (tlvar)
		{
			case 1: if (control_tool==ilisting[ilv1].toolnr) state|=9;break;
			case 2: state|=(*((char*)(ilisting[ilv1].variable)) & 1); break;
			case 3: tlvar=ilisting[ilv1].rmbmode;if (tlvar!=3) goto iagain; break;
			case 0x111: state=(((*(int*)(ilisting[ilv1]).variable))<<8)|0x1;if (((ilisting+ilv1)==control_menuitem) && (control_mousestate & 0x20)) sdl_colorpaldraw(ilisting[ilv1].x*32+xpos+32,ilisting[ilv1].y*32+ypos+64);break;
			case 0x103: goto sliderdraw;break;
			case 4: 
			{
				int tlval=quersum(*(_u32*)(ilisting[ilv1].variable),STRUCTURE_OBJECTTYPE_ListSize);
				state=(((ilisting[ilv1].bgcolor & 0xFF)*tlval*256/STRUCTURE_OBJECTTYPE_ListSize)&0xFF00)+(((0xFF00)*(6-abs(tlval-4))*256/STRUCTURE_OBJECTTYPE_ListSize)&0xFF0000)+(((0xFF0000)*(8-tlval)*256/STRUCTURE_OBJECTTYPE_ListSize)&0xFF000000);
				state|=1*(tlval>0);state|=((((tlval*8)-1)/STRUCTURE_OBJECTTYPE_ListSize)&7)<<1; break;
			}
			case 6: state|=((*((_i32*)(ilisting[ilv1].variable)))==ilisting[ilv1].toolnr)?1:0;break;
		}
		sdl_toolboxitemdraw(ilisting[ilv1].x*32+xpos,ilisting[ilv1].y*32+ypos,ilisting[ilv1].picno,state);
		continue;
		sliderdraw:
		sdl_sliderdraw(ilisting[ilv1].x*32+xpos,ilisting[ilv1].y*32+ypos,ilisting[ilv1].picno,-control_menudragint);
		continue;
	}
		
	return 1;
}
int sdl_canvasframedraw()
{
	for (int ilv1=gfx_canvasminx;ilv1<gfx_canvasmaxx;ilv1++)
	{
		screen[(gfx_screensizex*gfx_canvasminy-1)+ilv1]=0;
	}
	for (int ilv1=gfx_canvasminx;ilv1<gfx_canvasmaxx;ilv1++)
	{
		screen[(gfx_screensizex*gfx_canvasmaxy)+ilv1]=0;
	}
	for (int ilv1=gfx_canvasminy;ilv1<gfx_canvasmaxy;ilv1++)
	{
		screen[(gfx_screensizex*ilv1-1)+gfx_canvasminx]=0;
	}
	for (int ilv1=gfx_canvasminy;ilv1<gfx_canvasmaxy;ilv1++)
	{
		screen[(gfx_screensizex*ilv1)+gfx_canvasmaxx]=0;
	}
}
structenum * searchreflectedstruct(const char * input);
int addmenu(const char * name,int type,int alignx=0,int aligny=0)
{
	structenum * tlstructenum;
	tlstructenum=searchreflectedstruct(name);
	if (tlstructenum)
	{
		menu_list[menu_list_count].type=type;
		menu_list[menu_list_count].what=*tlstructenum;
		menu_list[menu_list_count].alignx=alignx;
		menu_list[menu_list_count].aligny=aligny;
		menu_list_count++;
	}
}
extern int control_tool;
int sdl_filemenucommon()
{
	menu_list_count=0;
	addmenu("filedlg_buttons",4);
	addmenu("filedlg_lists",3);
}
int sdl_commonmenucommon()
{
	char tlstring[60];
	menu_list_count=0;
	addmenu("toolbox",0);
	for (int ilv1=0;ilv1<AUTOSTRUCT_PULLOUTLISTING_toolbox_Size;ilv1++)
	{
		if (AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].toolnr==control_tool)
		{
			snprintf(tlstring,60,"toolspecific_%s",AUTOSTRUCT_PULLOUTLISTING_toolbox[ilv1].name);
			tlstring[59]=0;
			addmenu(tlstring,0,gfx_screensizex-160,128);
		}
	}
	if (control_mousestate & 8)
	{
		sprintf(tlstring,"submenu_%s",menu_matrixsubmenuvariable);
		addmenu(tlstring,1,256,96);
	}
	if (control_mousestate & 0x10)
	{
		//TODO: insert ordinary popups here and tell between PSE
		menu_list[menu_list_count].type=2;
		menu_list[menu_list_count].alignx=160;
		menu_list[menu_list_count].aligny=160;
		menu_list_count++;
	}
}
int sdl_psedraw(int istartx,int istarty)
{
	for (int ilv1=0;ilv1<sizeof(element)/sizeof(element_);ilv1++)
	{
		int borderx=istartx+element[ilv1].PSEX*32;
		int bordery=istarty+element[ilv1].PSEY*48;
		int bordercolor=0;
		if (ilv1==control_drawproperties.Element)
		{
			bordercolor=0xFF9F00;
		}
		for (int ilv2=bordery;ilv2<bordery+48;ilv2++)
		{
			for (int ilv3=borderx;ilv3<borderx+32;ilv3++)
			{
				screen[ilv2*gfx_screensizex+ilv3]=0xFFFFFF;
			}
			screen[ilv2*gfx_screensizex+borderx+31]=bordercolor;
			screen[ilv2*gfx_screensizex+borderx]=bordercolor;
		}
		for (int ilv2=borderx;ilv2<borderx+32;ilv2++)
		{
			screen[bordery*gfx_screensizex+ilv2]=bordercolor;
			screen[(bordery+47)*gfx_screensizex+ilv2]=bordercolor;
		}
		printmenutext(istartx+element[ilv1].PSEX*32+10,istarty+element[ilv1].PSEY*48+24,element[ilv1].name,NULL,0,0,strlen(element[ilv1].name));
	}
}
int sdl_menudraw()
{
	for (int ilv1=0;ilv1<menu_list_count;ilv1++)
	{
		if (menu_list[ilv1].type==0)
		{
			sdl_buttonmenudraw((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,menu_list[ilv1].alignx,menu_list[ilv1].aligny);
		}
		if (menu_list[ilv1].type==1)
		{
			sdl_textmenudraw((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,menu_list[ilv1].alignx,menu_list[ilv1].aligny);
		}
		if (menu_list[ilv1].type==2)
		{
			sdl_psedraw(menu_list[ilv1].alignx,menu_list[ilv1].aligny);
		}
		if (menu_list[ilv1].type==3)
		{
			sdl_listmenudraw((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,menu_list[ilv1].alignx,menu_list[ilv1].aligny);
		}
		if (menu_list[ilv1].type==4)
		{
			sdl_textbuttonmenudraw((AUTOSTRUCT_PULLOUTLISTING_*)menu_list[ilv1].what.pointer,menu_list[ilv1].what.count,menu_list[ilv1].alignx,menu_list[ilv1].aligny);
		}
	}
}
int sdl_commonmenudraw()
{
	sdl_commonmenucommon();
	sdl_menudraw();
}
int sdl_filemenudraw()
{
	sdl_filemenucommon();
	sdl_menudraw();
}
extern int control_hotatom;
void draw_reticle()
{
	if (control_reticle)
	{
		if ((control_mousey<gfx_screensizey) && (control_mousey>=0))
		{
			for (int ilv1=0;ilv1<gfx_screensizex;ilv1++)
			{
				screen[gfx_screensizex*(control_mousey)+ilv1]^=0xFFFFFF;
			}
		}
		if ((control_mousex<gfx_screensizex) && (control_mousex>=0))
		{
			for (int ilv1=0;ilv1<gfx_screensizey;ilv1++)
			{
				screen[gfx_screensizex*(ilv1)+control_mousex]^=0xFFFFFF;
			}
		}
	}
}
int sdl_selectiondraw()
{
	_u32 icompare;
	int isize;
	char * ibufferpos;
	if (control_mousestate==1)
	{
		if ((control_tool==2) || (control_tool==5))
		{
			expressline(selection_frame.startx,selection_frame.starty,selection_frame.endx,selection_frame.starty);
			expressline(selection_frame.startx,selection_frame.starty,selection_frame.startx,selection_frame.endy);
			expressline(selection_frame.startx,selection_frame.endy,selection_frame.endx,selection_frame.endy);
			expressline(selection_frame.endx,selection_frame.starty,selection_frame.endx,selection_frame.endy);
		}
	}
	for (int ilv1=0;ilv1<STRUCTURE_OBJECTTYPE_ListSize;ilv1++)
	{
		icompare=1<<ilv1;
		int isize= STRUCTURE_OBJECTTYPE_List[ilv1].size;
		basicmultilist * tlmultilist=findmultilist(STRUCTURE_OBJECTTYPE_List[ilv1].name);
		if (tlmultilist==NULL) goto i_fertig;
		CDXMLREAD_functype tldummy;
		ibufferpos=(char*)((*tlmultilist).pointer);
		float tlpx,tlpy;
		SDL_color=0x00FF00;
		for (int ilv2=0;ilv2<(*tlmultilist).filllevel;ilv2++)
		{
			if ((selection_currentselection[ilv2]) & icompare)
			{
				if ((*((basic_instance*)(ibufferpos+isize*ilv2))).exist)
				{
					int ilv3=0;
					iback:
					if (retrievepoints_basic(((basic_instance*)(ibufferpos+isize*ilv2)),&tlpx,&tlpy,ilv3,ilv1)>0)
					{
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx+1,(tlpy-SDL_scrolly)*SDL_zoomy+1);
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx+2,(tlpy-SDL_scrolly)*SDL_zoomy);
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx-2,(tlpy-SDL_scrolly)*SDL_zoomy);
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx-1,(tlpy-SDL_scrolly)*SDL_zoomy+1);
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx+1,(tlpy-SDL_scrolly)*SDL_zoomy-1);
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx,(tlpy-SDL_scrolly)*SDL_zoomy+2);
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx,(tlpy-SDL_scrolly)*SDL_zoomy-2);
						putpixel((tlpx-SDL_scrollx)*SDL_zoomx-1,(tlpy-SDL_scrolly)*SDL_zoomy-1);
						ilv3++;
						goto iback;
					}
				}
			}
		}
		i_fertig:;
	}
		float tlpx,tlpy;
		SDL_color=0x3F3F00;
					if (control_hot[STRUCTURE_OBJECTTYPE_n]!=-1)
					{
					int ilv3=0;
					iback2:
						if (retrievepoints_basic(((basic_instance*)(((char*)(*glob_n_multilist).pointer)+sizeof(n_instance)*control_hot[STRUCTURE_OBJECTTYPE_n])),&tlpx,&tlpy,ilv3,STRUCTURE_OBJECTTYPE_n)>0)
						{
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx+1,(tlpy-SDL_scrolly)*SDL_zoomy+1);
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx+2,(tlpy-SDL_scrolly)*SDL_zoomy);
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx-2,(tlpy-SDL_scrolly)*SDL_zoomy);
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx-1,(tlpy-SDL_scrolly)*SDL_zoomy+1);
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx+1,(tlpy-SDL_scrolly)*SDL_zoomy-1);
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx,(tlpy-SDL_scrolly)*SDL_zoomy+2);
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx,(tlpy-SDL_scrolly)*SDL_zoomy-2);
							putpixel((tlpx-SDL_scrollx)*SDL_zoomx-1,(tlpy-SDL_scrolly)*SDL_zoomy-1);
							ilv3++;
							goto iback2;
						}
					}
}
