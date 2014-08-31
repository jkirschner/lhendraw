//Processes the commandline parameters
char argmeanings[1000];//0: filename which is wanted 1: interpret this 2: Filename, to be read at start
char parametersthatwantfilename[]={"oslcnw"};
char parametersexecutedimmediately[]={"Ioh"};
char * parameter_filename;
char * parameter_filetype;
int parameter_argc;
char ** parameter_argv;
char parameter_cmd1[stringlength+1]="";
char parameter_cmd2[stringlength+1]="";
char * getparameter(int parameter,int ioffset,char * commandout_1,const char * commandout_2,int argc,char ** argv)//Takes a filename or a command from the commandline
{
	char * iarg;
	char *ichar,*ichar2,*ichar3;
	iarg=(argv[parameter]+ioffset);
	if (*iarg==0)
	{
		if (argc==parameter+1)
		{
			return NULL;
		}
		iarg=(argv[parameter+1]);
	}
	else
	{
		if (*iarg=='.')//attached and starting with a dot, that means its just the filetype
		{
			ichar3=NULL;
			ichar=iarg+1;
			ichar2=strchr(iarg,'=');
			if (ichar2)
			{
				iarg=ichar2+1;
			}
			else
			{
				if (argc==parameter+1)
				{
					return NULL;
				}
				iarg=(argv[parameter+1]);
			}
			goto filename_thatswhynobrackets;
		}
	}
	ichar=strchr(iarg,'(');//The other variant with brackets
	if (ichar!=NULL)
	{
		ichar2=strchr(ichar,')');
		if (ichar2!=NULL)
		{
			ichar[0]=0;
			ichar++;
			ichar2[0]=0;
			ichar3=strchr(ichar,',');
			if (ichar3==NULL)
			{
				ichar3[0]=0;
			}
			filename_thatswhynobrackets:
			strncpy(parameter_cmd1,ichar+1,stringlength);parameter_cmd1[stringlength]=0;
			if (ichar3!=NULL)
			{
				strncpy(parameter_cmd2,ichar3+1,stringlength);parameter_cmd2[stringlength]=0;
			}
		}
	}
	return iarg;
}
int executeparameter(const char which,int parameter,int posinparameter,int argc,char ** argv)
{
	posinparameter++;
	switch(which)
	{
		case 'I' :      control_GUI=0; control_interactive=0;control_force=1;break;
		case 'f' :      control_force|=1;break;
		case 'F' :      control_force|=0;break;
		case 'o' :      strncpy(control_filename,getparameter(parameter,posinparameter,parameter_filetype,NULL,argc,argv),stringlength-1);control_filename[stringlength-1]=0;strncpy(control_filetype,parameter_filetype,stringlength-1);control_filetype[stringlength-1]=0;control_saveuponexit=1;break;
//		case 'f' :      strncpy(control_filename,getparameter(parameter,posinparameter,parameter_filetype,NULL,argc,argv),stringlength-1);control_filename[stringlength-1]=0;strncpy(control_filetype,parameter_filetype,stringlength-1);control_filetype[stringlength-1]=0;break;
		case 'w' :      parameter_filetype=NULL;parameter_filename=getparameter(parameter,posinparameter,parameter_filetype,NULL,argc,argv);SAVE_TYPE(parameter_filename,parameter_filetype);break;
		case 'l' :      parameter_filetype=NULL;parameter_filename=getparameter(parameter,posinparameter,parameter_filetype,NULL,argc,argv);LOAD_TYPE(parameter_filename,parameter_filetype);break;
		case '5' :	WARN_HYPERC("","");
		case '-' :      if ((argv[parameter][posinparameter])!=0)
				{
				//TODO: add your multi-symbol commands here
				}//or it was a --, so not interpreted.
				break;
		case 'c' :      for (int ilv1=0;ilv1<REFLECTION_FUNCTION_ListSize;ilv1++)
				{
					if (strcmp(REFLECTION_FUNCTION_List[ilv1].name,getparameter(parameter,posinparameter,parameter_cmd1,parameter_cmd2,argc,argv))==0)
					{
						REFLECTION_FUNCTION_List[ilv1].function(parameter_cmd1,parameter_cmd2);
					}
				}
				break;
		case 'N' :      FILE_NEW("","");break;
		case 'n' :	parameter_filetype=NULL;parameter_filename=getparameter(parameter,posinparameter,parameter_filetype,NULL,argc,argv);
				LOAD_INTO_SEARCHBUF(parameter_filename,parameter_filetype);
				if (SEARCH("","")) {printf("%s\n",parameter_filename);}break;
		case 'h' :      printf(
"Usage:\n"
"lhendraw [-I] [-o output] [-<parameter> <parameter_filename>] (...) <filename> (...)\n"
"filenames and parameter_filename combinations may be mixed\n"
"the \"output\" file is always saved when the program exits\n"
"Its name is stored in the same register as the \"current\"\n"
"The -I command prevents loading the graphical user interface, making the program all command-line type.\n"
"All filenames stored outside the registers are read in additively, merging those documents\n"
"All other parameters are processed AFTERWARDS(sic)\n"
"\n"
"Parameters:\n"
"-I      : No gui. implies f\n"
"-o<fn>  : specify output-filename/filetype. Will be saved at end. Is stored as the current document's filename\n"
"-f      : run non-interactively\n"
"-F      : Run interactively. overrides the f of -I\n"
"Commands, processed in order:\n"
"-w<fn>  : saves to filename. Set no current filename\n"
"-l<fn>  : loads. Set no current filename\n"
"-e<fn>  : loads. Set current filename, overrides -o's filename\n"
"-q<fn>  : exits. saves if -o specified\n"
"-c<cmd>(<prm>,<prm>) : executes command in the form COMMAND(valuea,valueb)\n"
"     Example: -cSAVEAS(SUCKER.xml,.cdxml)\n"
"     saves the file as the name SUCKER.xml, but with the filetype .cdxml\n"
"--      : Interpret the following parameters as filenames, to be additively loaded at start\n"
"-N<fn>  : Un-loads the current file and un-sets the current filename, just as New File. Defeats -o.\n"
"-n<fn>  : searches, in the specified file, for the chemical pattern which is loaded before\n"
"          files with matches are printed on stdout. One match per file.\n"
"--search: searches, like -n, but in all follwing files. All following parameters are treated as input files.\n"
"\n"
"Example:\n"
"lhendraw -I -o.cdx shirt.cdx super.cdxml duper.cdx -ccheckupinconsistencies() -w.svg shirt.txt -cSELECT(true) -cROTATE(z,-90)\n"
"Superimposes super.cdxml and duper.cdx. concatenates all overlapping atoms.\n Saves a picture of the current state, as svg, into shirt.txt.\n Spins the result CCW at 90° and saves the spun result as shirt.cdx\n"
"lhendraw -I pattern.cdx --search FUJI0001.cdx FUJI0002.cdx FUJI0003.cdx FUJI0004.cdx FUJI0005.cdx FUJI0006.cdx LÖNNEBERG0001.cdxml LÖNNEBERG0002.cdxml\n"
"Searches the files FUJI.... and LÖNNE.... for the pattern in pattern.cdxml, then exits\n"
				);
				exit(0);
				break;
		case 'q' : exit(0);break;
	}
}

void cmdline(int argc,char ** argv)
{
	parameter_argc=argc;parameter_argv=argv;
	char wfn=0;//parameter wants filename
	char toomanyarguments=0;
	if (argc>1000) {toomanyarguments=1;argc=1000;}
	for (int ilv1=1;ilv1<argc;ilv1++)
	{
		argmeanings[ilv1]=0;
		if (wfn)
		{
			argmeanings[ilv1]=0;
			wfn&=2;
		}
		else
		{
			if (argv[ilv1][0]=='-')
			{
				argmeanings[ilv1]=1;
				if (argv[ilv1][1]=='-')
				{
					if (argv[ilv1][2]==0)
					{
						wfn=2;//want only filenames, forever
					}
					//TODO: add your multi-symbol commands here.
				}
				for (int ilv2=1;argv[ilv1][ilv2]!=0;ilv2++)
				{
					if (strchr(parametersthatwantfilename,argv[ilv1][ilv2]))
					{
						ilv2++;
						if (argv[ilv1][ilv2]!=0)
						{
							if (argv[ilv1][ilv2]=='.')
							{
								wfn=1;
								for (;argv[ilv1][ilv2]!=0;ilv2++)//resumed-loop
								{
									if ((argv[ilv1][ilv2]=='=') && (argv[ilv1][ilv2+1]!=0))
									{
										wfn=0;
									}
								}
							}
							else
							{
								wfn=0;
							}
						}
						else
						{
							wfn=1;
						}
					}
					if (strchr(parametersexecutedimmediately,argv[ilv1][ilv2]))
					{
						executeparameter(argv[ilv1][ilv2],ilv1,ilv2,argc,argv);
					}
					goto ifertig;
				}
				ifertig:;
			}
			else
			{
				argmeanings[ilv1]=2;//Filename, to be read at start
			}
		}
	}
	if (toomanyarguments)
	{
		if (control_force & 1)
		{
		}
		else
		{
			fprintf(stderr,"too many arguments");exit(10);
		}
	}
	for (int ilv1=1;ilv1<argc;ilv1++)
	{
		if (argmeanings[ilv1]==2)
		{
			LOAD_TYPE(argv[ilv1],"");
		}
	}
	for (int ilv1=1;ilv1<argc;ilv1++)
	{
		if (argmeanings[ilv1]==1)
		{
			for (int ilv2=0;argv[ilv1][ilv2]!=0;ilv2++)
			{
				if (strchr(parametersexecutedimmediately,argv[ilv1][ilv2])==NULL)
				{
					executeparameter(argv[ilv1][ilv2],ilv1,ilv2,argc,argv);
				}
				if (strchr(parametersthatwantfilename,argv[ilv1][ilv2]))
				{
					goto filenamefromnow;
				}
			}
			filenamefromnow:;
		}
	}
}
