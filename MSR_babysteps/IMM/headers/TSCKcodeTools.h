#pragma once
#include <math.h>
#include <iostream>
#include <sstream>  
#include<algorithm>
#include<iterator>
#include <string>
#include <fstream>
#include <cstring>
#include "TTree.h"
#include "TFile.h"
#include "TF1.h"
#include "TF2.h"
#include "TF3.h"
#include "TMath.h"
#include <cmath>
#include "TSCBasicInlineFunctions.h"
#include "TSCMathFunctions.h"
using namespace std;




//static const int __KcodeReader_MaxStringLength=524288;
//static const int __KcodeReader_MaxStringLength=2000;
class KcodeReader {
private :
	ifstream infile;
	size_t locater;
	TFile *f;
	TTree *curT;
	TTree *T;
	char outfilename[100];
	char infilename[100];
	unsigned int GetNLines(char *filename){
		if(!this->__file_exist)return 0;
		unsigned int nLines=0;
		ifstream afile;
		afile.open(filename);
		//afile.unsetf(ios_base::skipws);
		string dummystring;
		while (getline(afile, dummystring))
			nLines++;
		//cout<<"nlines:: "<<nLines<<endl;
		afile.close();
		return nLines;
	}

	void LoadLines(int nlines){
		int iter=0;
		line=new string[nlines+1];
		// cout<<"loading file..."<<endl;
		while(getline(infile, line[iter])){
			iter++;
			if(iter>nlines){cout<<"ERROR::KcodeReader::LoadLines:: File too big"<<endl;break;}
		}
		LastLineIndex=iter;
//		cout<<"File loaded: "<<LastLineIndex<<" lines"<<endl;
	}
	char linesegment[100][25];
	int ReadLineIntoSegments(int index){
		if(index<0||index>LastLineIndex)return 0;
		if(line[index].size()==0)return 0;
		char *dummychar=new char[line[index].size()+1];
		dummychar[line[index].size()]=0;
		memcpy(dummychar,line[index].c_str(),line[index].size());
		// resettiing old line segment array
//		for(int j=0;j<100;j++)for(int k=0;k<25;k++)linesegment[j][k]=0;
		int i=0;
		int j=0;
		int k=0;
		for(int i=0;i<=line[index].size();i++){
			if((int)dummychar[i]==32){linesegment[k][j]=0;if(j>0)k++;j=0;continue;}
			linesegment[k][j]=dummychar[i];
			j++;
		}
		k++;
		return k;
	}
	int MakeRootFile(char *filename){
		if(f){
			cout<<"WARNING::KcodeReader::MakeRootFile:: root file already existed: "<<outfilename<<".root, closing,"<<outfilename<<".root and creating new file named: "<<filename<<".root"<<endl;
			f->Close();
			f=0;
		}
		sprintf(outfilename,"%s",filename);
		f=new TFile(TSC::CHAR(filename,".root"),"RECREATE");
		curT=new TTree("T","T");
		curT->Branch("NT_isactive",&this->T_NT_isactive,"NT_isactive/I");
		curT->Branch("NT_material",this->T_NT_material,"material[NT_isactive]/I");
		curT->Branch("NT_step",this->T_NT_step,"step[NT_isactive]/I");
		curT->Branch("NT_duration",this->T_NT_duration,"duration[NT_isactive]/F");
		curT->Branch("NT_time",this->T_NT_time,"time[NT_isactive]/F");
		curT->Branch("NT_power",this->T_NT_power,"power[NT_isactive]/F");
		curT->Branch("NT_powerfraction",this->T_NT_powerfraction,"powerfraction[NT_isactive]/F");
		curT->Branch("NT_keff",this->T_NT_keff,"keff[NT_isactive]/F");
		curT->Branch("NT_flux",this->T_NT_flux,"flux[NT_isactive]/F");
		curT->Branch("NT_avenu",this->T_NT_avenu,"avenu[NT_isactive]/F");
		curT->Branch("NT_aveq",this->T_NT_aveq,"aveq[NT_isactive]/F");
		curT->Branch("NT_burnup",this->T_NT_burnup,"burnup[NT_isactive]/F");
		curT->Branch("NT_source",this->T_NT_source,"source[NT_isactive]/F");

		curT->Branch("NBT_isactive",&this->T_NBT_isactive,"NBT_isactive/I");
		curT->Branch("NBT_material",this->T_NBT_material,"material[NBT_isactive]/I");
		curT->Branch("NBT_step",this->T_NBT_step,"step[NBT_isactive]/I");
		curT->Branch("NBT_time",this->T_NBT_time,"time[NBT_isactive]/F");
		curT->Branch("NBT_power",this->T_NBT_power,"power[NBT_isactive]/F");
		curT->Branch("NBT_index",this->T_NBT_index,"index[NBT_isactive]/I");
		curT->Branch("NBT_zaid",this->T_NBT_zaid,"zaid[NBT_isactive]/I");
		curT->Branch("NBT_Z",this->T_NBT_Z,"Z[NBT_isactive]/I");
		curT->Branch("NBT_A",this->T_NBT_A,"A[NBT_isactive]/I");
		curT->Branch("NBT_mass",this->T_NBT_mass,"mass[NBT_isactive]/F");
		curT->Branch("NBT_activity",this->T_NBT_activity,"activity[NBT_isactive]/F");
		curT->Branch("NBT_sp_activity",this->T_NBT_sp_activity,"sp_activity[NBT_isactive]/F");
		curT->Branch("NBT_atom_density",this->T_NBT_atom_density,"atom_density[NBT_isactive]/F");
		curT->Branch("NBT_atom_fraction",this->T_NBT_atom_fraction,"atom_fraction[NBT_isactive]/F");
		curT->Branch("NBT_mass_fraction",this->T_NBT_mass_fraction,"mass_fraction[NBT_isactive]/F");

		curT->Branch("NCAT_isactive",&this->T_NCAT_isactive,"NCAT_isactive/I");
		curT->Branch("NCAT_step",this->T_NCAT_step,"NCAT_step[NCAT_isactive]/I");
		curT->Branch("NCAT_material",this->T_NCAT_material,"NCAT_material[NCAT_isactive]/I");
		curT->Branch("NCAT_index",this->T_NCAT_index,"NCAT_index[NCAT_isactive]/I");
		curT->Branch("NCAT_zaid",this->T_NCAT_zaid,"NCAT_zaid[NCAT_isactive]/I");
		curT->Branch("NCAT_Z",this->T_NCAT_Z,"NCAT_Z[NCAT_isactive]/I");
		curT->Branch("NCAT_A",this->T_NCAT_A,"NCAT_A[NCAT_isactive]/I");
		curT->Branch("NCAT_n_gamma",this->T_NCAT_n_gamma,"NCAT_n_gamma[NCAT_isactive]/F");
		curT->Branch("NCAT_n_gamma_e",this->T_NCAT_n_gamma_e,"NCAT_n_gamma_e[NCAT_isactive]/F");
		curT->Branch("NCAT_n_fission",this->T_NCAT_n_fission,"NCAT_n_fission[NCAT_isactive]/F");
		curT->Branch("NCAT_n_fission_e",this->T_NCAT_n_fission_e,"NCAT_n_fission_e[NCAT_isactive]/F");
		curT->Branch("NCAT_n_nuxfission",this->T_NCAT_n_nuxfission,"NCAT_n_nuxfission[NCAT_isactive]/F");
		curT->Branch("NCAT_n_nuxfission_e",this->T_NCAT_n_nuxfission_e,"NCAT_n_nuxfission_e[NCAT_isactive]/F");
		curT->Branch("NCAT_n_fissionxq",this->T_NCAT_n_fissionxq,"NCAT_n_fissionxq[NCAT_isactive]/F");
		curT->Branch("NCAT_n_fissionxq_e",this->T_NCAT_n_fissionxq_e,"NCAT_n_fissionxq_e[NCAT_isactive]/F");
		curT->Branch("NCAT_n_2n",this->T_NCAT_n_2n,"NCAT_n_2n[NCAT_isactive]/F");
		curT->Branch("NCAT_n_2n_e",this->T_NCAT_n_2n_e,"NCAT_n_2n_e[NCAT_isactive]/F");
		curT->Branch("NCAT_n_3n",this->T_NCAT_n_3n,"NCAT_n_3n[NCAT_isactive]/F");
		curT->Branch("NCAT_n_3n_e",this->T_NCAT_n_3n_e,"NCAT_n_3n_e[NCAT_isactive]/F");
		curT->Branch("NCAT_n_alpha",this->T_NCAT_n_alpha,"NCAT_n_alpha[NCAT_isactive]/F");
		curT->Branch("NCAT_n_alpha_e",this->T_NCAT_n_alpha_e,"NCAT_n_alpha_e[NCAT_isactive]/F");
		curT->Branch("NCAT_n_p",this->T_NCAT_n_p,"NCAT_n_p[NCAT_isactive]/F");
		curT->Branch("NCAT_n_p_e",this->T_NCAT_n_p_e,"NCAT_n_p_e[NCAT_isactive]/F");
		curT->Branch("NCAT_creation",this->T_NCAT_creation,"NCAT_creation[NCAT_isactive]/F");
		curT->Branch("NCAT_absorption",this->T_NCAT_absorption,"NCAT_absorption[NCAT_isactive]/F");
		curT->Branch("NCAT_neutroneconomy",this->T_NCAT_neutroneconomy,"NCAT_neutroneconomy[NCAT_isactive]/F");

		T_NCAT_isactive=0;
		T_NT_isactive=0;
		T_NBT_isactive=0;
		return 1;
	}



	int T_NCAT_isactive;
	int T_NT_isactive;
	int T_NBT_isactive;

	int T_NCAT_index[1], T_NCAT_zaid[1], T_NCAT_Z[1], T_NCAT_A[1];
	float T_NCAT_n_gamma[1],T_NCAT_n_gamma_e[1],
		T_NCAT_n_fission[1],T_NCAT_n_fission_e[1],
		T_NCAT_n_nuxfission[1],T_NCAT_n_nuxfission_e[1],
		T_NCAT_n_fissionxq[1],T_NCAT_n_fissionxq_e[1];
	float T_NCAT_n_2n[1],T_NCAT_n_2n_e[1],
		T_NCAT_n_3n[1],T_NCAT_n_3n_e[1],
		T_NCAT_n_alpha[1],T_NCAT_n_alpha_e[1],
		T_NCAT_n_p[1],T_NCAT_n_p_e[1];
	int T_NCAT_step[1],T_NCAT_material[1];
	float T_NCAT_creation[1],T_NCAT_absorption[1],T_NCAT_neutroneconomy[1];

	int T_NT_step[1],T_NT_material[1];
	float T_NT_duration[1],T_NT_time[1],T_NT_power[1],T_NT_keff[1],T_NT_flux[1],T_NT_avenu[1],T_NT_aveq[1],T_NT_burnup[1],T_NT_source[1],T_NT_powerfraction[1];

	int T_NBT_material[1],T_NBT_step[1],T_NBT_index[1],T_NBT_zaid[1],T_NBT_Z[1],T_NBT_A[1];
	float T_NBT_time[1],T_NBT_power[1],T_NBT_mass[1],T_NBT_activity[1],T_NBT_sp_activity[1],T_NBT_atom_density[1],T_NBT_atom_fraction[1],T_NBT_mass_fraction[1];


	int ActivateBranchesNeutronicsTable(){
		T_NCAT_isactive=0;
		T_NT_isactive=1;
		T_NBT_isactive=0;
		return 1;
	}
	int DeactivateBranchesNeutronicsTable(){
		T_NCAT_isactive=0;
		T_NT_isactive=0;
		T_NBT_isactive=0;
		return 1;
	}
	int ActivateBranchesNeutronCreationAndAnihilationTable(){
		T_NCAT_isactive=1;
		T_NT_isactive=0;
		T_NBT_isactive=0;
		return 1;
	}
	int DeactivateBranchesNeutronCreationAndAnihilationTable(){
		T_NCAT_isactive=0;
		T_NT_isactive=0;
		T_NBT_isactive=0;
		return 1;
	}
	int ActivateBranchesNuclideBurnupTable(){
		T_NCAT_isactive=0;
		T_NT_isactive=0;
		T_NBT_isactive=1;
		return 1;
	}
	int DeactivateBranchesNuclideBurnupTable(){
		T_NCAT_isactive=0;
		T_NT_isactive=0;
		T_NBT_isactive=0;
		return 1;
	}

public :
	int LastLineIndex;
	string *line;
	//--------------------------------------------------
	//--------------------------------------------------
	// constructor
	//--------------------------------------------------
	//--------------------------------------------------
	bool __file_exist;
	KcodeReader(char *filename, bool writefilewarning=true){
		T_NBT_isactive=0;
		T_NT_isactive=0;
		sprintf(infilename,filename);
		infile.open(filename);
		if(!infile){if(writefilewarning)cout<<"ERROR::KcodeReader:: no file loaded!"<<endl;__file_exist=false;return;}
		__file_exist=true;
		LoadLines(GetNLines(filename));
		f=0;
		curT=0;
		DeckSetLength=0;
	}
	//--------------------------------------------------
	//--------------------------------------------------
	// destructor
	//--------------------------------------------------
	//--------------------------------------------------	
	~KcodeReader(){
		infile.close();
		if(curT)curT->SetBranchStatus("*",1);
		if(f)f->Write();
		if(f)f->Close();
	}
	



	// findes line containing the search word, and returns the line number
	// starting from line "fromline", if negative the search is backwards, double for search in one specific line	
	int FindLine(char *searchword, int fromline=0){
		if(fromline<0){		// searching backwards
			for(int i=-fromline;i>=0;i--){
				if(line[i].find(searchword)<10000)return i;
			}
		} else {			// searching
			for(int i=fromline;i<LastLineIndex;i++){
				if(line[i].find(searchword)<10000)return i;
			}
		}
		// nothing was found
		return -1;
	}
	// searches for 2 words in from line
	int FindLine(char *searchword1, char *searchword2, int fromline=0){
		if(fromline<0){		// searching backwards
			for(int i=-fromline;i>=0;i--){
				if(line[i].find(searchword1)<10000)
					if(line[i].find(searchword2)<10000)return i;
			}
		} else {			// searching
			for(int i=fromline;i<LastLineIndex;i++){
				if(line[i].find(searchword1)<10000)
					if(line[i].find(searchword2)<10000)return i;
			}
		}
		// nothing was found
		return -1;
	}
	// searches for word in specific line, and return the words location in the line
	int FindLine(char *searchword, double linenumber){
		if(line[(int)linenumber].find(searchword)<10000)return line[(int)linenumber].find(searchword);
		return -1;
	}
	// searches for 2 words in specific line, and return the words location in the line
	int FindLine(char *searchword1, char *searchword2, double linenumber){
				if(line[(int)linenumber].find(searchword1)<10000)
					if(line[(int)linenumber].find(searchword2)<10000)return line[(int)linenumber].find(searchword2);
		return -1;
	}
	// finds next empty line
	int FindLine(int fromline=0){
		if(fromline<0){		// searching backwards
			for(int i=-fromline;i>=0;i--){
				if(line[i].length()==0)return i;
			}
		} else {			// searching
			for(int i=fromline;i<LastLineIndex;i++){
				if(line[i].length()==0)return i;
			}
		}
		// nothing was found
		return -1;
	}
	
	int FindNumberOfLineWhere(char *searchword,char *searchword2=0, int fromline=0){
		int numberoflines=-1;
		while(fromline!=-1){
			numberoflines++;
			if(searchword2)fromline=this->FindLine(searchword,searchword2,fromline+1);
			else fromline=this->FindLine(searchword,fromline+1);
		}
		return numberoflines;
	}
	int PrintLinesWhere(char *searchword,char *searchword2=0, int fromline=0){
		int numberoflines=-1;
		while(fromline!=-1){
			numberoflines++;
			if(searchword2)fromline=this->FindLine(searchword,searchword2,fromline+1);
			else fromline=this->FindLine(searchword,fromline+1);
			this->printsection(fromline,1);
		}
		return numberoflines;
	}

	// prints nlines in a section, if nlines<0, prints until -nlines with length 0 have been printed
	// if doprint=false then this algoritm is just analternative use of FindLine 
	int printsection(int fromline, int nlines, bool doprint=true){
		int i=fromline;
		if(nlines>0){
			for(i=fromline;i<fromline+nlines;i++){
				if(i==LastLineIndex)break;
				if(doprint)cout<<i<<" ::|"<<line[i]<<endl;
			}
		} else {
			int n=-nlines;
			
			while(n>=0){
				if(i==LastLineIndex)break;
				if(doprint)cout<<i<<" ::|"<<line[i]<<endl;
				i++;
				if(line[i].length()==0)n--;
			}
		}
		if(i==LastLineIndex)return -1;
		return i;
	}
	// prints until a line with searchword is found
	int printsection(int fromline, char *searchword, bool doprint=true){
		int i=fromline;
		while(FindLine(searchword,i*1.)==-1){
			if(i==LastLineIndex)break;
			if(doprint)cout<<i<<" ::|"<<line[i]<<endl;
			i++;
		}
		if(i==LastLineIndex)return -1;
		return i;
	}

	int GetNumberOfSegments(int line){	// returns the number of linesegments in #line
		return ReadLineIntoSegments(line);
	}

	char The_Segment_From__GetSegment[25];
	char *GetSegment(int line, int segment, bool write_errors=true){	// returns a char* for linesegment number #segment in #line
		if(ReadLineIntoSegments(line)<segment){
			sprintf(The_Segment_From__GetSegment,"%s",linesegment[segment]);
			return (char*)The_Segment_From__GetSegment;
		}
		if(write_errors)cout<<"ERROR::KcodeReader::GetSegment:: No such segment"<<endl;
		return 0;
	}

	double ReadNumberInLine(int line,int segment, bool write_errors=true){	// returns the number written in #segment from #line. if #segment<0 then it #reads the -#segment'th number found in the line	

		if(line<0||line>this->LastLineIndex){if(write_errors)cout<<"ERROR::KcodeReader::ReadNumber:: Line does not exist (index too high)"<<endl;return 0;}
		int numberofsegments=ReadLineIntoSegments(line);
		if(!numberofsegments){if(write_errors)cout<<"ERROR::KcodeReader::ReadNumber:: Line is empty"<<endl;return 0;}
		if(segment>=0){
			if(TSC::isNumberFromChar(linesegment[segment])>=0){
				return (double)TSC::GetNumberFromChar(linesegment[segment]);
			} else {if(write_errors)cout<<"ERROR::KcodeReader::ReadNumber:: segment is not a number"<<endl;return 0;}
		} else {
			int currentnumberofnumbers=0;
			if(numberofsegments<-segment){if(write_errors)cout<<"ERROR::KcodeReader::ReadNumber:: segment higher than number of line segments"<<endl;return 0;}
			for(int i=0;i<numberofsegments;i++){
				if(TSC::isNumberFromChar(linesegment[i])>=0){
					currentnumberofnumbers++;
					if(currentnumberofnumbers==-segment)return (double)TSC::GetNumberFromChar(linesegment[i]);
				}
			}
		}
		if(write_errors)cout<<"ERROR::KcodeReader::ReadNumber:: No such number found"<<endl;
		return 0;
	}
	double GetKeffFromStandartOutoutfile__keffError;
	double GetKeffFromStandartOutoutfile__keff;
	double GetKeffFromStandartOutoutfile(bool getErrorinstead=false){ // returns keff (0 - error 1) from a standart MCNPX output file, error stored in: this->GetKeffFromStandartOutoutfile__keffError

		int resultline=this->FindLine("final result");

		// performing some basic checkups
		if(resultline-4!=this->FindLine("keff","68%",resultline-4)
			||
			this->ReadLineIntoSegments(resultline)!=13
			)
			{cout<<"ERROR::KcodeReader::GetKeffFromStandartOutoutfile:: Wrong file formate 1"<<endl;return 0;}
		GetKeffFromStandartOutoutfile__keffError=this->ReadNumberInLine(resultline,-2);
		GetKeffFromStandartOutoutfile__keff=this->ReadNumberInLine(resultline,-1);
		if(GetKeffFromStandartOutoutfile__keff==0||GetKeffFromStandartOutoutfile__keffError==0)
			{cout<<"ERROR::KcodeReader::GetKeffFromStandartOutoutfile:: Wrong file formate 2"<<endl;return 0;}
		if(getErrorinstead)return GetKeffFromStandartOutoutfile__keffError;
		return GetKeffFromStandartOutoutfile__keff;
	}
	int Get_ListOfNumbers__arraylength;
	double *Get_ListOfNumbers(int linenumber){
		int numberosegments=this->ReadLineIntoSegments(linenumber);
		int firstsegment=0;
		int NumberOfNumbers=0;
		bool firstnumber=true;
		for(int i=0;i<100&&i<numberosegments;i++){
			if(TSC::isNumberFromChar(linesegment[i])>=0){
				if(firstnumber)firstsegment=i;
				NumberOfNumbers++;
				firstnumber=false;
			}
			
		}
		if(!NumberOfNumbers)return 0;
		Get_ListOfNumbers__arraylength=NumberOfNumbers;
		double *the_array=(double*)malloc(NumberOfNumbers*sizeof(double));
		int index_i=0;
		for(int i=firstsegment;i<100&&i<numberosegments;i++){
			if(TSC::isNumberFromChar(linesegment[i])>=0){
				if(index_i>=NumberOfNumbers){cout<<"ERROR::KcodeReader::Get_ListOfNumbers:: something strange is going on - really strange..."<<endl;return 0;}
				the_array[index_i]=TSC::GetNumberFromChar(linesegment[i]);
				index_i++;
			}
		}
		return (double*)the_array;
	}
	double *Get_ListOfNumbers(char *searchword,int startindex=0){ // returns the numbers from a line in a double array
		int linenumber;
			if(startindex)linenumber=this->FindLine(searchword,startindex);
			else linenumber=this->FindLine(searchword);
		if(linenumber<=0){cout<<"ERROR::KcodeReader::Get_ListOfNumbers:: Could not find a r-binning line (check your searchword: \""<<searchword<<"\")"<<endl;return 0;}
		return (double*)Get_ListOfNumbers(linenumber);
	}
	double *Get_ListOfNumbers(char *searchword,char *searchword2,int startindex=0){ // returns the numbers from a line in a double array
		int linenumber;
			if(searchword2)linenumber=this->FindLine(searchword,searchword2,startindex);
			else linenumber=this->FindLine(searchword);
		if(linenumber<=0){cout<<"ERROR::KcodeReader::Get_ListOfNumbers:: Could not find a r-binning line (check your searchword: \""<<searchword<<"\")"<<endl;return 0;}
		return (double*)Get_ListOfNumbers(linenumber);
	}

	int CreateOutPutFile(char *filename){
		return MakeRootFile(filename);
	}

	// Reads the neutronics table indexed "tablenumber" into a root tree in the file "f", minus is backwards index, if "-1" the last table is read
	int ReadNeutronicsTable(){
		cout<<"Reading neutronics table"<<endl;
		if(!f){
			cout<<"WARNING::KcodeReader::ReadNeutronicsTable:: no root file existed, autogenerating: KcodeReaderOutputFile.root"<<endl;
			MakeRootFile("KcodeReaderOutputFile");
		}
		int lastfound=FindLine("neutronics",-LastLineIndex+1);
		if(lastfound==-1){cout<<"WARNING::KcodeReader::ReadNeutronicsTable:: no table found..."<<endl; return 0;}
		// lastfound is not equal to the entry where reading should start..
		int endmark=printsection(lastfound,"nuclide",0);
		int startindex=lastfound+3;
		ActivateBranchesNeutronicsTable();
		int currentmaterial=0;
		float powervalues[1000]={0};
		int numberofsegments;
		//printsection(startindex,endmark-startindex);
		for(int i=startindex;i<endmark;i++){
			numberofsegments=ReadLineIntoSegments(i);
			if(numberofsegments==3&&TSC::isNumberFromChar(linesegment[2])>=0){
					currentmaterial=(int)TSC::GetNumberFromChar(linesegment[2]);
					continue;
			} else if(numberofsegments==10
				&&TSC::isNumberFromChar(linesegment[0])>=0&&TSC::isNumberFromChar(linesegment[1])>=0
				&&TSC::isNumberFromChar(linesegment[2])>=0&&TSC::isNumberFromChar(linesegment[3])>=0
				&&TSC::isNumberFromChar(linesegment[4])>=0&&TSC::isNumberFromChar(linesegment[5])>=0
				&&TSC::isNumberFromChar(linesegment[6])>=0&&TSC::isNumberFromChar(linesegment[7])>=0
				&&TSC::isNumberFromChar(linesegment[8])>=0&&TSC::isNumberFromChar(linesegment[9])>=0
					){
				T_NT_material[0]=0;
				T_NT_step[0]=(int)TSC::GetNumberFromChar(linesegment[0]);
				T_NT_duration[0]=(float)TSC::GetNumberFromChar(linesegment[1]);
				T_NT_time[0]=(float)TSC::GetNumberFromChar(linesegment[2]);
				powervalues[T_NT_step[0]]=(float)TSC::GetNumberFromChar(linesegment[3]);
				T_NT_power[0]=powervalues[T_NT_step[0]];
				T_NT_powerfraction[0]=1.;
				T_NT_keff[0]=(float)TSC::GetNumberFromChar(linesegment[4]);
				T_NT_flux[0]=(float)TSC::GetNumberFromChar(linesegment[5]);
				T_NT_avenu[0]=(float)TSC::GetNumberFromChar(linesegment[6]);
				T_NT_aveq[0]=(float)TSC::GetNumberFromChar(linesegment[7]);
				T_NT_burnup[0]=(float)TSC::GetNumberFromChar(linesegment[8]);
				T_NT_source[0]=(float)TSC::GetNumberFromChar(linesegment[9]);
			} else if(numberofsegments==5
				&&TSC::isNumberFromChar(linesegment[0])>=0&&TSC::isNumberFromChar(linesegment[1])>=0
				&&TSC::isNumberFromChar(linesegment[2])>=0&&TSC::isNumberFromChar(linesegment[3])>=0
				&&TSC::isNumberFromChar(linesegment[4])>=0
				){
				T_NT_material[0]=currentmaterial;
				T_NT_step[0]=(int)TSC::GetNumberFromChar(linesegment[0]);
				T_NT_duration[0]=(float)TSC::GetNumberFromChar(linesegment[1]);
				T_NT_time[0]=(float)TSC::GetNumberFromChar(linesegment[2]);
				T_NT_powerfraction[0]=(float)TSC::GetNumberFromChar(linesegment[3]);
				T_NT_power[0]=(float)(T_NT_powerfraction[0]*powervalues[T_NT_step[0]]);
				T_NT_keff[0]=0;
				T_NT_flux[0]=0;
				T_NT_avenu[0]=0;
				T_NT_aveq[0]=0;
				T_NT_burnup[0]=(float)TSC::GetNumberFromChar(linesegment[4]);
				T_NT_source[0]=0;
			} else if(numberofsegments==3&&(FindLine("(days)",(double)i)==8||FindLine("Burnup",(double)i)==21)){
				//cout<<"(days) :"<<FindLine("(days)",(double)i)<<endl;
				//if(FindLine("Burnup",(double)i))cout<<"(days) :"<<FindLine("Burnup",(double)i)<<endl;
				// burnup line
				continue;
			} else if(numberofsegments==6&&FindLine("burnup",(double)i)==46){
				//cout<<"burnupline 6 :"<<FindLine("burnup",(double)i)<<endl;
				// material table header line
				continue;
			} else if(numberofsegments==5&&FindLine("(days)",(double)i)==9){
				//cout<<"header 5 :"<<FindLine("(days)",(double)i)<<endl;
				// main table header line
				continue;
			} else if(numberofsegments==0){
				// empty line
				continue;
			} else {
				cout<<"ERROR:KcodeReader::ReadNeutronicsTable:: ReadFail, does not understand this line!"<<endl;
				printsection(i,1);
				return 0;
			}
			curT->Fill();
		}
		DeactivateBranchesNeutronicsTable();
		return 1;
	}

	int ReadNuclideBurnupTable(){
		cout<<"Reading Nucleid burnup table"<<endl;
		if(!f){
			cout<<"WARNING::KcodeReader::ReadNuclideBurnupTable:: no root file existed, autogenerating: KcodeReaderOutputFile.root"<<endl;
			MakeRootFile("KcodeReaderOutputFile");
		}
		int lastfound=FindLine("neutronics",-(LastLineIndex-1));
		lastfound=FindLine("nuclide",lastfound+1);
		if(lastfound==-1){cout<<"WARNING::KcodeReader::ReadNuclideBurnupTable:: no table found..."<<endl; return 0;}
		int endmark=0;printsection(lastfound,"nuclide",0);
		int startindex=lastfound+2;

		ActivateBranchesNuclideBurnupTable();
		
		//printsection(startindex,1000);

		int i=startindex-1;
		int numberofsegments;
		int material=0;
		int step=-1;
		float time=-1;
		float power=-1;
		for(int i=startindex;i<LastLineIndex;i++){
			numberofsegments=ReadLineIntoSegments(i);
			if(numberofsegments==0){
				continue;
			} else if(numberofsegments==17
				&&TSC::isNumberFromChar(linesegment[10])>=0
				&&TSC::isNumberFromChar(linesegment[12])>=0
				&&TSC::isNumberFromChar(linesegment[15])>=0
				){
				if(FindLine("actinide inventory",(double)i)<0){
					cout<<"ERROR:KcodeReader::ReadNuclideBurnupTable:: ReadFail, does not understand this line!"<<endl;
					return 0;
				}
				material=0;
				step=TSC::GetNumberFromChar(linesegment[10]);
				time=TSC::GetNumberFromChar(linesegment[12]);
				power=TSC::GetNumberFromChar(linesegment[15]);
				continue;
			} else if(numberofsegments==15
				&&TSC::isNumberFromChar(linesegment[3])>=0
				&&TSC::isNumberFromChar(linesegment[8])>=0
				&&TSC::isNumberFromChar(linesegment[10])>=0
				&&TSC::isNumberFromChar(linesegment[13])>=0
				){
				if(FindLine("actinide inventory",(double)i)<0){
					cout<<"ERROR:KcodeReader::ReadNuclideBurnupTable:: ReadFail, does not understand this line!"<<endl;
					return 0;
				}
				material=TSC::GetNumberFromChar(linesegment[3]);
				step=TSC::GetNumberFromChar(linesegment[8]);
				time=TSC::GetNumberFromChar(linesegment[10]);
				power=TSC::GetNumberFromChar(linesegment[13]);
				continue;
			} else if(numberofsegments==8
				&&TSC::isNumberFromChar(linesegment[0])>=0&&TSC::isNumberFromChar(linesegment[1])>=0
				&&TSC::isNumberFromChar(linesegment[2])>=0&&TSC::isNumberFromChar(linesegment[3])>=0
				&&TSC::isNumberFromChar(linesegment[4])>=0&&TSC::isNumberFromChar(linesegment[5])>=0
				&&TSC::isNumberFromChar(linesegment[6])>=0&&TSC::isNumberFromChar(linesegment[7])>=0
				){	
				
				T_NBT_material[0]=material;
				T_NBT_step[0]=step;
				T_NBT_time[0]=time;
				T_NBT_power[0]=power;
				T_NBT_index[0]=(int)TSC::GetNumberFromChar(linesegment[0]);
				T_NBT_zaid[0]=(int)TSC::GetNumberFromChar(linesegment[1]);
				T_NBT_Z[0]=M::zaid2Z(T_NBT_zaid[0]);
				T_NBT_A[0]=M::zaid2A(T_NBT_zaid[0]);
				T_NBT_mass[0]=(float)TSC::GetNumberFromChar(linesegment[2]);
				T_NBT_activity[0]=(float)TSC::GetNumberFromChar(linesegment[3]);
				T_NBT_sp_activity[0]=(float)TSC::GetNumberFromChar(linesegment[4]);
				T_NBT_atom_density[0]=(float)TSC::GetNumberFromChar(linesegment[5]);
				T_NBT_atom_fraction[0]=(float)TSC::GetNumberFromChar(linesegment[6]);
				T_NBT_mass_fraction[0]=(float)TSC::GetNumberFromChar(linesegment[7]);
			} else if(numberofsegments==7
				&&FindLine("totals",(double)i)
				&&TSC::isNumberFromChar(linesegment[1])>=0
				&&TSC::isNumberFromChar(linesegment[2])>=0&&TSC::isNumberFromChar(linesegment[3])>=0
				&&TSC::isNumberFromChar(linesegment[4])>=0&&TSC::isNumberFromChar(linesegment[5])>=0
				&&TSC::isNumberFromChar(linesegment[6])>=0&&TSC::isNumberFromChar(linesegment[7])>=0
				){	
					T_NBT_material[0]=material;
					T_NBT_step[0]=step;
					T_NBT_time[0]=time;
					T_NBT_power[0]=power;
					T_NBT_index[0]=0;
					T_NBT_zaid[0]=0;
					T_NBT_Z[0]=0;
					T_NBT_A[0]=0;
					T_NBT_mass[0]=(float)TSC::GetNumberFromChar(linesegment[1]);
					T_NBT_activity[0]=(float)TSC::GetNumberFromChar(linesegment[2]);
					T_NBT_sp_activity[0]=(float)TSC::GetNumberFromChar(linesegment[3]);
					T_NBT_atom_density[0]=(float)TSC::GetNumberFromChar(linesegment[4]);
					T_NBT_atom_fraction[0]=(float)TSC::GetNumberFromChar(linesegment[5]);
					T_NBT_mass_fraction[0]=(float)TSC::GetNumberFromChar(linesegment[6]);
			} else if(numberofsegments==12
				&&FindLine("no. zaid     mass      activity   sp. act.  atom den.   atom fr.   mass fr.",(double)i)
				){
					continue;
			} else if(numberofsegments==11
				&&FindLine("no.  zaid    mass      activity  spec.act.  atom den.   atom fr.   mass fr.",(double)i)
				){
					continue;
			} else if(numberofsegments==4
				&&FindLine("(gm)        (Ci)     (Ci/gm)    (a/b-cm)",(double)i)
				){
					continue;
			} else if(numberofsegments==5
				&&FindLine("(gm)        (Ci)     (Ci/gm)    (a/b-cm)",(double)i)
				){
					continue;
			} else if(FindLine("summary table summed over all materials",(double)i)
				){
					continue;
			}else {
					cout<<"--------------------------------"<<endl;
					cout<<"--------------------------------"<<endl;
					cout<<"--------------------------------"<<endl;
					cout<<numberofsegments<<endl;
					printsection(i,1);
					cout<<"--------------------------------"<<endl;
					cout<<"--------------------------------"<<endl;
					cout<<"--------------------------------"<<endl;
					
					break;
			}
			
			curT->Fill();
		}
		DeactivateBranchesNuclideBurnupTable();
		return 1;
	}

	int ReadNeutronCreationAndAnihilationTable(){
		cout<<"Reading Neutron Creation And Anihilation table"<<endl;
		if(!f){
			cout<<"WARNING::KcodeReader::ReadNeutronCreationAndAnihilationTable:: no root file existed, autogenerating: KcodeReaderOutputFile.root"<<endl;
			MakeRootFile("KcodeReaderOutputFile");
		}

		int numberofsegments;
		int startindex=0;
		int step_counter=0;
		int step=0;
		int material=0;
		for(int i=0;i<LastLineIndex;i++){
			int dummyi=FindLine("(n,gamma)",i);
			if(dummyi<0){
				i=FindLine("(n,fission)",i);
			} else {
			  i=TMath::Min(dummyi,FindLine("(n,fission)",i));
			}
			if(i<0){break;}
			if(FindLine("Total Collision Rates",(double)i-5)>=0){
					step_counter++;
					step=((int)(step_counter/2.))*(-1+2*(step_counter%2));
					cout<<" ... Writing time step: "<<step<<endl;
			}
			numberofsegments=ReadLineIntoSegments(i-2);
			if(TSC::isNumberFromChar(linesegment[2])>=0&&FindLine("Negative Material",(double)i-2)<0){
				material=TSC::GetNumberFromChar(linesegment[2]);
			} else if((TSC::isNumberFromChar(linesegment[2])>=0||TSC::isNumberFromChar(linesegment[3])>=0)&&FindLine("Negative Material",(double)i-2)>=0){
				if(TSC::isNumberFromChar(linesegment[3])>=0)material=TSC::GetNumberFromChar(linesegment[3]);
				if(TSC::isNumberFromChar(linesegment[2])>=0)material=TSC::GetNumberFromChar(linesegment[2]);
				continue;
			} else {
				printsection(i-2,1);
					cout<<"ERROR::KcodeReader::ReadNeutronCreationAndAnihilationTable:: Something was wrong with a line (01)."<<endl;
				return 0;
			}
			int startmark=i+1;
			int endmark=FindLine(startmark);
			//printsection(endmark,10);
		//printsection(startmark-5,10);
			ActivateBranchesNeutronCreationAndAnihilationTable();
			T_NCAT_material[0]=material;
			T_NCAT_step[0]=step;
			for(int j=startmark;j<endmark;j++){
				numberofsegments=ReadLineIntoSegments(j);
				if(!(numberofsegments==10
					&&TSC::isNumberFromChar(linesegment[0])>=0
					&&TSC::isNumberFromChar(linesegment[1])>=0
					&&TSC::isNumberFromChar(linesegment[2])>=0
					&&TSC::isNumberFromChar(linesegment[3])>=0
					&&TSC::isNumberFromChar(linesegment[4])>=0
					&&TSC::isNumberFromChar(linesegment[5])>=0
					&&TSC::isNumberFromChar(linesegment[6])>=0
					&&TSC::isNumberFromChar(linesegment[7])>=0
					&&TSC::isNumberFromChar(linesegment[8])>=0
					&&TSC::isNumberFromChar(linesegment[9])>=0
					)){
						cout<<"ERROR::KcodeReader::ReadNeutronCreationAndAnihilationTable:: Something was wrong with a line (02)."<<endl;
						return 0;
				}
				T_NCAT_index[0]=TSC::GetNumberFromChar(linesegment[0]);
				T_NCAT_zaid[0]=TSC::GetNumberFromChar(linesegment[1]);
				T_NCAT_Z[0]=M::zaid2Z(T_NCAT_zaid[0]);
				T_NCAT_A[0]=M::zaid2A(T_NCAT_zaid[0]);
				T_NCAT_n_gamma[0]=TSC::GetNumberFromChar(linesegment[2]);
				T_NCAT_n_gamma_e[0]=TSC::GetNumberFromChar(linesegment[3]);
				T_NCAT_n_fission[0]=TSC::GetNumberFromChar(linesegment[4]);
				T_NCAT_n_fission_e[0]=TSC::GetNumberFromChar(linesegment[5]);
				T_NCAT_n_nuxfission[0]=TSC::GetNumberFromChar(linesegment[6]);
				T_NCAT_n_nuxfission_e[0]=TSC::GetNumberFromChar(linesegment[7]);
				T_NCAT_n_fissionxq[0]=TSC::GetNumberFromChar(linesegment[8]);
				T_NCAT_n_fissionxq_e[0]=TSC::GetNumberFromChar(linesegment[9]);
	
				numberofsegments=ReadLineIntoSegments(j+endmark-startmark+2);
				if(!(numberofsegments==10
					&&TSC::isNumberFromChar(linesegment[0])>=0
					&&TSC::isNumberFromChar(linesegment[1])>=0
					&&TSC::isNumberFromChar(linesegment[2])>=0
					&&TSC::isNumberFromChar(linesegment[3])>=0
					&&TSC::isNumberFromChar(linesegment[4])>=0
					&&TSC::isNumberFromChar(linesegment[5])>=0
					&&TSC::isNumberFromChar(linesegment[6])>=0
					&&TSC::isNumberFromChar(linesegment[7])>=0
					&&TSC::isNumberFromChar(linesegment[8])>=0
					&&TSC::isNumberFromChar(linesegment[9])>=0
					)){
						cout<<"ERROR::KcodeReader::ReadNeutronCreationAndAnihilationTable:: Something was wrong with a line (03)."<<endl;
						return 0;
				}
				if(!(T_NCAT_index[0]==TSC::GetNumberFromChar(linesegment[0])
					&&T_NCAT_zaid[0]==TSC::GetNumberFromChar(linesegment[1]))){
						cout<<"ERROR::KcodeReader::ReadNeutronCreationAndAnihilationTable:: Something was wrong with a line (04)."<<endl;
						return 0;
				}
				T_NCAT_n_2n[0]=TSC::GetNumberFromChar(linesegment[2]);
				T_NCAT_n_2n_e[0]=TSC::GetNumberFromChar(linesegment[3]);
				T_NCAT_n_3n[0]=TSC::GetNumberFromChar(linesegment[4]);
				T_NCAT_n_3n_e[0]=TSC::GetNumberFromChar(linesegment[5]);
				T_NCAT_n_alpha[0]=TSC::GetNumberFromChar(linesegment[6]);
				T_NCAT_n_alpha_e[0]=TSC::GetNumberFromChar(linesegment[7]);
				T_NCAT_n_p[0]=TSC::GetNumberFromChar(linesegment[8]);
				T_NCAT_n_p_e[0]=TSC::GetNumberFromChar(linesegment[9]);
				
				// deriving stuff
				if(T_NCAT_n_fission[0]!=0){
				T_NCAT_creation[0]=
					T_NCAT_n_2n[0]
					+T_NCAT_n_3n[0]*2
					+(T_NCAT_n_nuxfission[0]/T_NCAT_n_fission[0]-1)*T_NCAT_n_fission[0];
				} else {
				T_NCAT_creation[0]=
					T_NCAT_n_2n[0]
					+T_NCAT_n_3n[0]*2;
				}
				T_NCAT_absorption[0]=
					T_NCAT_n_alpha[0]
					+T_NCAT_n_p[0]
					+T_NCAT_n_gamma[0];
				if(T_NCAT_absorption[0]!=0){T_NCAT_neutroneconomy[0]=T_NCAT_creation[0]/T_NCAT_absorption[0];
				} else {T_NCAT_neutroneconomy[0]=0;}
				curT->Fill();
			}
		}
		DeactivateBranchesNeutronCreationAndAnihilationTable();
		return 1;
	}

	string *DeckSet;
	int DeckSetLength;
	int ReadDeckSet(){
		DeckSetLength=0;
		int i=FindLine("1-",1);
		int j=FindLine(i);
		int dummycounter=0;
		if(j-i<=0){
			cout<<"ERROR::KcodeReader::ReadDeckSet:: inconsistency in the deck - it ends before it begins"<<endl;
			return 0;
		}
		if(j-i>=9999){
			cout<<"ERROR::KcodeReader::ReadDeckSet:: inconsistency in the deck - too long (>10k lines)"<<endl;
			return 0;
		}
		DeckSetLength=j-i-1;
		DeckSet=new string[DeckSetLength];
		for(i=i;i<j;i++){
			int numberofsegments=ReadLineIntoSegments(i);
			if(!TSC::isNumberFromChar(linesegment[0])==0)continue;
			if(dummycounter!=TSC::GetNumberFromChar(linesegment[0])-1){
				cout<<"ERROR::KcodeReader::ReadDeckSet:: inconsistency in the deck (expected line number dosnt match) around line:"<<i<<" (see below)"<<endl;
				printsection(i-4,10);
				return 0;
			}
			//??? DOES NOT WORK...???
			DeckSet[dummycounter]=(char*)(((char*)line[i].c_str())+13);
			dummycounter++;
		}
		return DeckSetLength;
	}

	int Read2Root(){
		cout<<"Processing..."<<endl;
		if(!f){
			cout<<"WARNING::KcodeReader::ReadNuclideBurnupTable:: no root file existed, autogenerating: KcodeReaderOutputFile.root"<<endl;
			MakeRootFile("KcodeReaderOutputFile");
		}
		if(!ReadNeutronCreationAndAnihilationTable())return 0;
		if(!ReadNuclideBurnupTable())return 0;
		if(!ReadNeutronicsTable())return 0;
		return 1;
	}
};

class IMM{
private :
	int lognumberofcases;
	int __GetPstudyInstruction_index;
	KcodeReader *KKR[10000];
	char WorkingDirectory[250];
	char lasecasename[260];
	char *GetCaseName(int id,char *a=0){
		sprintf(lasecasename,"%scase\0",WorkingDirectory);
		int orderofid=(int)TMath::Log10(id);
		for(int i=0;i<lognumberofcases-orderofid;i++){
			sprintf(lasecasename,"%s0\0",lasecasename);
		}
		if(a){
			sprintf(lasecasename,"%s%d/%s\0",lasecasename,id,a);
		} else {
			sprintf(lasecasename,"%s%d/outp\0",lasecasename,id);
		}
		return lasecasename;
	}
	int GetLogNCases(){
		ifstream infile;
		lognumberofcases=0;
		for(int i=0;i<10;i++){
			infile.open(GetCaseName(1));
			if(infile)break;
			lognumberofcases++;
			if(i==9)cout<<"ERROR::IMM::GetLogNCases:: could not find any case in directory: "<<WorkingDirectory<<" || check that a file outp exist in a dir named caseX"<<endl;
		}
		//if(lognumberofcases>4)cout<<"ERROR::IMM::GetLogNCases:: too many cases"<<endl;
		infile.close();
		return lognumberofcases;
	}
	KcodeReader *Case(int id,char *alternativeoutputname=0){
		if(id>=10000||id<0){cout<<"ERROR::IMM::GetCase:: Cannot handle this many/few cases (max 10000)"<<endl;return 0;}
		if(alternativeoutputname){
			if(KKR[id])delete KKR[id];
		//	cout<<"Loading"<<this->GetCaseName(id,alternativeoutputname)<<endl;
			KKR[id]=new KcodeReader((char*)this->GetCaseName(id,alternativeoutputname),false);
		} else if (!KKR[id]){
		//	cout<<"Loading"<<this->GetCaseName(id)<<endl;
			KKR[id]=new KcodeReader((char*)this->GetCaseName(id),false);
		}
			if(!KKR[id]->__file_exist)
				return 0;
		
		return (KcodeReader*)KKR[id];
	}
public :
	IMM(char *maindir=0){
		if(maindir){sprintf(WorkingDirectory,"%s\0",maindir);
		} else {sprintf(WorkingDirectory,"\0");}
		Initialize();
	}
	~IMM(){}
	double *xaxis_bins;
	double *yaxis_bins;
	int xaxis_Nbins;
	int yaxis_Nbins;
	char xaxis_name[100];
	char yaxis_name[100];

	void Initialize(){
		cout<<"Initializing new IMM reader"<<endl;
		sprintf(xaxis_name,"Radius (r) [cm]");
		sprintf(yaxis_name,"Height (h) [cm]");
		for(int i=0;i<10000;i++)KKR[i]=0;
		lognumberofcases=GetLogNCases();
		if(lognumberofcases<0||lognumberofcases>5){cout<<"ERROR::IMM::Initialize:: Could not initialize!!!"<<endl;return;}
		__GetPstudyInstruction_index=0;
		cout<<"IMM reader initialized"<<endl;
	}
	int GetNumberOfPstudyInstructions(bool doprint=false){ //returns the number of lines with the lable " IMM "
		if(doprint)return Case(1)->PrintLinesWhere(" IMM ");
		return Case(1)->FindNumberOfLineWhere(" IMM ");
	}
	int GetPstudyInstructionLine(char *searchword=0){ // returns linenumber of the IMM instruction labled searchword, if searchword=0 then reads next instruction
		int linenumber;
		if(searchword){
			linenumber=Case(1)->FindLine(" IMM ",searchword);
		} else {
			linenumber=Case(1)->FindLine(" IMM ",__GetPstudyInstruction_index+1);
		}
		if(linenumber!=-1)__GetPstudyInstruction_index=linenumber;
		return linenumber;
	}
	TH2D *Get_r_h_Map(char *fissiletag,char *fertiletag){
		int index_fissile=this->GetPstudyInstructionLine(" FissileMaterials ");
		int index_fertile=this->GetPstudyInstructionLine(" FertileMaterials ");
		if(index_fissile==-1){cout<<"ERROR::IMM::Get_r_h_Map:: unable to read fissile material list"<<endl;return 0;}
		if(index_fertile==-1){cout<<"ERROR::IMM::Get_r_h_Map:: unable to read fertile material list"<<endl;return 0;}
		if(Case(1)->FindLine("FissileMaterials",fissiletag,(double)index_fissile)<1){cout<<"ERROR::IMM::Get_r_h_Map:: Could not find this fissile tag \""<<index_fissile<<"\""<<endl;return 0;}
		if(Case(1)->FindLine("FertileMaterials",fertiletag,(double)index_fertile)<1){cout<<"ERROR::IMM::Get_r_h_Map:: Could not find this fertile tag \""<<index_fertile<<"\""<<endl;return 0;}
		
		int index_heightbins=this->GetPstudyInstructionLine(" HeightBins ");
		int index_radiusbins=this->GetPstudyInstructionLine(" RadiusBins ");
		if(index_heightbins==-1){cout<<"ERROR::IMM::Get_r_h_Map:: unable to read height bins material list"<<endl;return 0;}
		if(index_radiusbins==-1){cout<<"ERROR::IMM::Get_r_h_Map:: unable to read radius bins material list"<<endl;return 0;}
		if(Case(1)->FindLine("HeightBins","uniform",(double)index_heightbins)<1){cout<<"ERROR::IMM::Get_r_h_Map:: Height axis is not \"uniform\""<<endl;return 0;}
		if(Case(1)->FindLine("RadiusBins","uniform",(double)index_radiusbins)<1){cout<<"ERROR::IMM::Get_r_h_Map:: Radius axis is not \"uniform\""<<endl;return 0;}

 
		Case(1)->printsection(index_fissile,1);
		Case(1)->printsection(index_fertile,1);
		Case(1)->printsection(index_heightbins,1);
		Case(1)->printsection(index_radiusbins,1);

		double *RadiusBins=Case(1)->Get_ListOfNumbers(index_radiusbins);
		if(Case(1)->Get_ListOfNumbers__arraylength!=4)cout<<"WARNING:IMM::Get_r_h_Map:: Strange number of radius bins..."<<endl;
		int nRbins=(int)RadiusBins[Case(1)->Get_ListOfNumbers__arraylength-3];
		double lowR=RadiusBins[Case(1)->Get_ListOfNumbers__arraylength-2];
		double highR=RadiusBins[Case(1)->Get_ListOfNumbers__arraylength-1];

		double *HeightBins=Case(1)->Get_ListOfNumbers(index_heightbins);
		if(Case(1)->Get_ListOfNumbers__arraylength!=4)cout<<"WARNING:IMM::Get_r_h_Map:: Strange number of height bins..."<<endl;
		int nHbins=(int)HeightBins[Case(1)->Get_ListOfNumbers__arraylength-3];
		double lowH=HeightBins[Case(1)->Get_ListOfNumbers__arraylength-2];
		double highH=HeightBins[Case(1)->Get_ListOfNumbers__arraylength-1];

		TH2D *dummyhist=new TH2D(TSC::uname(),TSC::CHAR(fissiletag,", ",fertiletag)
//			,nHbins,lowH-(highH-lowH)/nHbins, highH+(highH-lowH)/nHbins
//			, nRbins, lowR-(highR-lowR)/nRbins, highR+(highR-lowR)/nRbins);
			, 100,lowH-(highH-lowH)/nHbins, highH+(highH-lowH)/nHbins
			, 100, lowR-(highR-lowR)/nRbins, highR+(highR-lowR)/nRbins);

		// starting the filling prodecure

		int this_radius_index=this->GetPstudyInstructionLine(" ThisRadius ");
		int this_height_index=this->GetPstudyInstructionLine(" ThisHeight ");
		int this_fissile_index=this->GetPstudyInstructionLine(" ThisFissile ");
		int this_fertile_index=this->GetPstudyInstructionLine(" ThisFertile ");
		
		int itr=1;
		double keff;
		double keff_err;
			
		double this_heightbin;
		double this_radiusbin;
		double *dummydouble;

		while(Case(itr)){
			// fetching radius
			if(Case(itr)->FindLine(" ThisFissile ",fissiletag,(double)this_fissile_index)==-1){itr++;continue;}
			if(Case(itr)->FindLine(" ThisFertile ",fertiletag,(double)this_fertile_index)==-1){itr++;continue;}
			//Case(itr)->printsection(this_fissile_index,1);
			//Case(itr)->printsection(this_fertile_index,1);
			dummydouble=Case(itr)->Get_ListOfNumbers(this_radius_index);
			if(Case(itr)->Get_ListOfNumbers__arraylength!=2){
				if(!Case(itr,"outq")){cout<<"ERROR::IMM::Get_r_h_Map something is wrong (a)"<<endl;return 0;}
				continue;
			}
			this_radiusbin=dummydouble[1];
			// fetching height
			dummydouble=Case(itr)->Get_ListOfNumbers(this_height_index);
			if(Case(itr)->Get_ListOfNumbers__arraylength!=2){
				if(!Case(itr,"outq")){cout<<"ERROR::IMM::Get_r_h_Map something is wrong (b)"<<endl;return 0;}
				continue;
			}
			this_heightbin=dummydouble[1];
			// fetching keff
			keff=Case(itr)->GetKeffFromStandartOutoutfile();
			keff_err=Case(itr)->GetKeffFromStandartOutoutfile(true);
			if(keff==0||keff_err==0){
				if(!Case(itr,"outq")){cout<<"ERROR::IMM::Get_r_h_Map something is wrong (c)"<<endl;return 0;}
				continue;
			}
			dummyhist->Fill(this_radiusbin,this_heightbin);
			/*Case(itr)->printsection(this_height_index,1);
			Case(itr)->printsection(this_radius_index,1);
			cout<<" joink::"<<this_radiusbin<<"::"<<this_heightbin<<"::"<<dummyhist->GetBin(this_radiusbin,this_heightbin)<<endl;
			*/
			/*
			dummyhist->SetBinContent(dummyhist->GetBin(this_radiusbin,this_heightbin),1);
			dummyhist->SetBinError(dummyhist->GetBin(this_radiusbin,this_heightbin),0);
			*/
			//dummyhist->Fill();
			itr++;
		}
//		cout<<"number of cases found was "<<itr<<endl;	
		return dummyhist;
	}
};


class isotope{
private :

public :
	// generics:
	int zaid;
	int Z;
	int A;
	int N;
	
	// from
	double afrac;
	double mfrac;
	double mass;
	double adensity;
	double act;			// activity
	double act_sp;		// specific activity
	
	// from activity tables
	double xs_ng;		//n,gamma
	double xs_ng_e;
	double xs_nf;		//n,fission
	double xs_nf_e; 
	double xs_nuxnf;	// nu times n,fission
	double xs_nuxnf_e;
	double xs_qxnf;		// q times n,fission
	double xs_qxnf_e;
	double xs_n2n;
	double xs_n2n_e;
	double xs_n3n;
	double xs_n3n_e;
	double xs_na;		//n,alpha
	double xs_na_e;
	double xs_p;		//n,p
	double xs_p_e;


	isotope(TTree *T){ // tree produced using Read2Root

	}
	~isotope(){}

};








class Reprocessor{
private :
	


public :
	Reprocessor(){};
	~Reprocessor(){};
};

class kcode2mcnpx{
private :
	TFile *OutPutFile;
	TTree *OutPutTree;
	char OutPutFileName[100];
	char OutPutTreeName[100];
	char DeckName_output;
	KcodeReader *Reader;

public :
	kcode2mcnpx(char *outqName,char *KcodeTreeName=0){
		Reader=new KcodeReader(outqName);
		sprintf(OutPutTreeName,"%s",outqName);
		if(KcodeTreeName){
			sprintf(OutPutFileName,"%s",KcodeTreeName);
		} else {
			sprintf(OutPutTreeName,"tmp_kcodetomcnpxhandeler_outfilefortree");
			Reader->CreateOutPutFile(OutPutTreeName);
			Reader->Read2Root();
		}
		OutPutTree=(TTree*)TSC::TreeLoader(OutPutFileName);
		Reader->ReadDeckSet();
	}
	~kcode2mcnpx(){
		delete OutPutTree;
		delete OutPutFile;
	}




	
};

/*TH1 **GetIsotopeHistogram(char *infile){
	TTree *T=TSC::TreeLoader(infile);
	TH1D *dummyhist=TSC::FastDraw1d(T,"NBT_step","1000,-.5,999.5");
	TH1D *StepToTimeHist=TSC::FastDraw1d(T,"NBT_step","1000,-.5,999.5","NBT_time");
	StepToTimeHist->Divide(dummyhist);
	int laststepindex;
	for(laststepindex=1;laststepindex<1001;laststepindex++){
		if(dummyhist->GetBinContent(laststepindex)==0){
			laststepindex-=1;
			break;
		}
	}
	int steps=laststepindex-1;
	TH1** ResultMaxtix
	double timeinlaststep=StepToTimeHist->GetBinContent(laststepindex);
	cout<<"Number of steps (-1): "<<laststepindex<<" latest time: "<<timeinlaststep<<endl;

	char stepandmaterialcut[25];
	sprintf(stepandmaterialcut,"(NBT_zaid>0&&NBT_step==%d &&NBT_material==%d)",step,material);

	TH1D *hZ_plot=TSC::FastDraw1d(T,"NBT_Z","100,-.5,99.5",TSC::CHAR(stepandmaterialcut,"*NBT_atom_fraction"));
	TH1D *hA=TSC::FastDraw1d(T,"NBT_zaid","100000,-.5,99999.5",TSC::CHAR(stepandmaterialcut,"*NBT_A"));
	TH1D *hZ=TSC::FastDraw1d(T,"NBT_zaid","100000,-.5,99999.5",TSC::CHAR(stepandmaterialcut,"*NBT_Z"));
	TH1D *hm=TSC::FastDraw1d(T,"NBT_zaid","100000,-.5,99999.5",TSC::CHAR(stepandmaterialcut,"*NBT_mass"));
	TH1D *hf=TSC::FastDraw1d(T,"NBT_zaid","100000,-.5,99999.5",TSC::CHAR(stepandmaterialcut,"*NBT_atom_fraction"));
	TH1D *hzaid=TSC::FastDraw1d(T,"NBT_zaid","100000,-.5,99999.5",TSC::CHAR(stepandmaterialcut,"*NBT_zaid"));

	
	TSC::NameHist(hZ_plot,1,"40GWd/tHM burnup (3 years), 17x17 assemply","Element number (Z)","Mass (m) [g]");
	TCanvas *c1=new TCanvas();
	hZ_plot->Draw();
	cout<<"\tZ\tA\tmass\t\tatom fraction"<<endl;
	cout<<"----------------------------"<<endl;
	double fsum;
	for(int i=1;i<100001;i++){
		if(hm->GetBinContent(i)<1e-9)continue;
		cout<<"\t"<<hZ->GetBinContent(i)<<"\t"<<hA->GetBinContent(i)<<"\t"<<hm->GetBinContent(i)<<"     \t"<<hf->GetBinContent(i)<<endl;
		fsum+=hf->GetBinContent(i);
	}
	cout<<"total: "<<fsum<<endl;


	for(int i=1;i<100001;i++){
		if(hm->GetBinContent(i)<1e-9)continue;
		cout<<"        "<<hzaid->GetBinContent(i)<<".73c  1"<<endl; //<<hf->GetBinContent(i)<<endl;
		fsum+=hf->GetBinContent(i);
		
	}
	cout<<"total: "<<fsum<<endl;



	return 1;
	*/

