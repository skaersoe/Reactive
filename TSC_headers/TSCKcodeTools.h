#pragma once
#include <math.h>
#include <iostream>
#include <sstream>  
#include<algorithm>
#include<iterator>
#include <string>
#include <fstream>
#include <sstream>
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
			if(iter>nlines){cout<<"WARNING::KcodeReader::LoadLines:: File too big - ending before EOF: nlines: "<<nlines<<" filename: "<<infilename<<endl;iter--;break;}
		}
		LastLineIndex=iter;
	}
	char linesegment[100][50];
	int ReadLineIntoSegments(int index){
//		this->printsection(index,1);
		
		if(index<0||index>LastLineIndex)return 0;
		if(line[index].size()==0)return 0;
		char *dummychar=new char[line[index].size()+1];
		dummychar[line[index].size()]=0;
		memcpy(dummychar,line[index].c_str(),line[index].size());
		int i=0;
		int j=0;
		int k=0;
		for(int i=0;i<line[index].size();i++){ 
			if((int)dummychar[i]==32){linesegment[k][j]=0;linesegment[k][j+1]=0;if(j>0)k++;j=0;continue;}
			linesegment[k][j]=dummychar[i];
			j++;
		}
		//k++;
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
	
	
	int GetLineLength(int linenumber){
		return line[linenumber].length();
	}
	char *GetLine(int linenumber){
		return (char*)line[linenumber].c_str();
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

/*	int GetNumberOfSegments(int line){	// returns the number of linesegments in #line
		return ReadLineIntoSegments(line);
	}
	*/
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
		if(resultline==-1){
			GetKeffFromStandartOutoutfile__keffError=0;
			GetKeffFromStandartOutoutfile__keff=0;
			return 0;
		}
		// performing some basic checkups
		if(resultline-4!=this->FindLine("keff","68%",resultline-4)
		//	||
		//	this->ReadLineIntoSegments(resultline)!=12
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
			//cout<<i<<" "<<linesegment[i]<<endl;
			if(TSC::isNumberFromChar(linesegment[i])>=0){
				//cout<<TSC::GetNumberFromChar(linesegment[i])<<endl;
				if(firstnumber)firstsegment=i;
				NumberOfNumbers++;
				firstnumber=false;
			}
		}
		if(!NumberOfNumbers)return 0;
		//cout<<NumberOfNumbers<<endl;
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
		//cout<<endl;
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
	KcodeReader *KKR[1];
	bool EndOfCases;
	int __This_KcodeReader_id;
	char WorkingDirectory[250];
	char lastcasename[260];
	char *GetCaseName(int id,char *a=0){
		sprintf(lastcasename,"%scase\0",WorkingDirectory);
		int orderofid=(int)TMath::Log10(id);
		for(int i=0;i<lognumberofcases-orderofid;i++){
			sprintf(lastcasename,"%s0\0",lastcasename);
		}
		if(a){
			sprintf(lastcasename,"%s%d/%s\0",lastcasename,id,a);
		} else {
			sprintf(lastcasename,"%s%d/outp\0",lastcasename,id);
		}
		return lastcasename;
	}
	int GetLogNCases(){
		cout<<"counting cases..."<<endl;
		ifstream infile;
		lognumberofcases=0;
		for(int i=0;i<10;i++){
			infile.open(GetCaseName(1,"inp"));
			if(infile)break;
			lognumberofcases++;
			if(i==9)cout<<"ERROR::IMM::GetLogNCases:: could not find any case in directory: "<<WorkingDirectory<<" || check that a file inp exist in case 1"<<endl;
		}
		//if(lognumberofcases>4)cout<<"ERROR::IMM::GetLogNCases:: too many cases"<<endl;
		infile.close();
		cout<<"Done"<<endl;
		return lognumberofcases;
	}
	KcodeReader *Case(int id,char *alternativeoutputname=0){
//		if(id>10000||id<0){cout<<"ERROR::IMM::GetCase:: Cannot handle this many/few cases (max 10000)"<<endl;return 0;}
		if(__This_KcodeReader_id!=id)delete KKR[0];

		if(alternativeoutputname){

		//	cout<<"Loading"<<this->GetCaseName(id,alternativeoutputname)<<endl;
			KKR[0]=new KcodeReader((char*)this->GetCaseName(id,alternativeoutputname),false);
		} else if (!KKR[0]){
		//	cout<<"Loading"<<this->GetCaseName(id)<<endl;
			KKR[0]=new KcodeReader((char*)this->GetCaseName(id),false);
		}
		if(!KKR[0]->__file_exist){
			delete KKR[0];
			KKR[0]=new KcodeReader((char*)this->GetCaseName(id,"inp"),false);
			if(KKR[0]->__file_exist)return 0;
			cout<<"IMM::Case:: It looks like the last case in the run is: "<<this->GetCaseName(id,"")<<endl;
			EndOfCases=true;
			return 0;
		}
		__This_KcodeReader_id=id;
		return (KcodeReader*)KKR[0];
	}	
	void Initialize(){
		cout<<"Initializing new IMM reader"<<endl;
		sprintf(xaxis_name,"Radius (r) [cm]");
		sprintf(yaxis_name,"Height (h) [cm]");
		//for(int i=0;i<10000;i++)KKR[i]=0;
		KKR[0]=0;;
		__This_KcodeReader_id=1;
		EndOfCases=false;
		lognumberofcases=GetLogNCases();
		if(lognumberofcases<0||lognumberofcases>5){cout<<"ERROR::IMM::Initialize:: Could not initialize!!!"<<endl;return;}
		__GetPstudyInstruction_index=0;
		cout<<"IMM reader initialized"<<endl;
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


	int GetNumberOfPstudyInstructions(bool doprint=false){ //returns the number of lines with the lable " IMM "
		if(doprint)return Case(__This_KcodeReader_id)->PrintLinesWhere(" IMM ");
		return Case(__This_KcodeReader_id)->FindNumberOfLineWhere(" IMM ");
	}
	int GetPstudyInstructionLine(char *searchword=0){ // returns linenumber of the IMM instruction labled searchword, if searchword=0 then reads next instruction
		int linenumber;
		if(searchword){
			linenumber=Case(__This_KcodeReader_id)->FindLine(" IMM ",searchword);
		} else {
			linenumber=Case(__This_KcodeReader_id)->FindLine(" IMM ",__GetPstudyInstruction_index+1);
		}
		if(linenumber!=-1)__GetPstudyInstruction_index=linenumber;
		return linenumber;
	}

	char __fissiletag2name[20];
	char *fissiletag2name(char *fissiletag){
		string U233="92233.72c";
		string U235="92235.72c";
		string Pu239="94239.72c";
		string Pu241="94241.72c";
		string PWRfuel="PRWfuel";
		string PWRfuel90="PRWfuel90";
		string inventory="inventory";
		if(inventory.find(fissiletag)==0){
			cout<<"WARNING::IMM::fissiletag2name:: Term \"inventory\" was used for fissile tag, assuming PWR fuel"<<endl;
			sprintf(__fissiletag2name,"PWR fuel\0");
			return __fissiletag2name;
		}
		if(U233.find(fissiletag)==0){sprintf(__fissiletag2name,"^{233}U\0");
		} else if(U235.find(fissiletag)==0){sprintf(__fissiletag2name,"^{235}U\0");
		} else if(Pu239.find(fissiletag)==0){sprintf(__fissiletag2name,"^{239}Pu\0");
		} else if(Pu241.find(fissiletag)==0){sprintf(__fissiletag2name,"^{241}Pu\0");
		} else if(PWRfuel.find(fissiletag)==0){sprintf(__fissiletag2name,"PWR fuel\0");
		} else if(PWRfuel90.find(fissiletag)==0){sprintf(__fissiletag2name,"PWR fuel 90 U removed\0");
		} else {cout<<"WARNING::IMM::fissiletag2name:: Unknown ´fissile"<<endl;return 0;}
		return __fissiletag2name;
	}
	char __fertiletag2name[10];
	char *fertiletag2name(char *fertiletag){
		string U238="92238.72c";
		string Th232="90232.72c";
		if(Th232.find(fertiletag)==0){sprintf(__fertiletag2name,"^{232}Th\0");
		} else if(U238.find(fertiletag)==0){sprintf(__fertiletag2name,"^{238}U\0");
		} else if(U238.find(fertiletag)==0){sprintf(__fertiletag2name,"^{238}U\0");
		} else {cout<<"WARNING::IMM::__fertiletag2name:: Unknown fertile"<<endl;return 0;}
		return __fertiletag2name;
	}
	char __moderatortag2name[20];
	char *moderatortag2name(char *moderatortag){
		string C="2001";
		string SiC="2002";
		string BeO="2003";
		string ZrH2="2004";
		string flibe="2010";
		if(C.find(moderatortag)==0){sprintf(__fertiletag2name,"Graphite\0");
		} else if(SiC.find(moderatortag)==0){sprintf(__fertiletag2name,"SiC\0");
		} else if(flibe.find(moderatortag)==0){sprintf(__fertiletag2name,"FLiBe\0");
		} else if(BeO.find(moderatortag)==0){sprintf(__fertiletag2name,"BeO\0");
		} else if(ZrH2.find(moderatortag)==0){sprintf(__fertiletag2name,"ZrH_{2}\0");
		} else {cout<<"WARNING::IMM::moderatortag2name:: Unknown moderator"<<endl;return 0;}
		return __fertiletag2name;
	}
	char __salttag2name[30];
	char *salttag2name(char *salttag){
		string LiBe="1001";
		string LiNaBe="1002";
		string RbZr="1003";
		string NaZr="1004";
		string LiNaZr="1005";
		string Li="1006";
		if(LiBe.find(salttag)==0){sprintf(__salttag2name,"^{7}LiF-BeF_{2}-AcF_{4}");
		} else if(LiNaBe.find(salttag)==0){sprintf(__salttag2name,"^{7}LiF-NaF-BeF_{2}-AcF_{4}\0");
		} else if(RbZr.find(salttag)==0){sprintf(__salttag2name,"RbF-ZrF_{4}-AcF_{4}\0");
		} else if(NaZr.find(salttag)==0){sprintf(__salttag2name,"NaF-ZrF_{4}-AcF_{4}\0");
		} else if(LiNaZr.find(salttag)==0){sprintf(__salttag2name,"^{7}LiF-NaF-ZrF_{4}-AcF_{4}\0");
		} else if(Li.find(salttag)==0){sprintf(__salttag2name,"^{7}LiF-AcF_{4}\0");
		} else {cout<<"WARNING::IMM::salttag2name:: Unknown salt"<<endl;return 0;}
		return __salttag2name;

	}
	int __GetFoMindex;
	int GetFoMindex(char *FoM){
		// list of known tags:
		string case1=" Keff ";
		if(case1.find(FoM)<100)return 1;
		cout<<"ERROR::IMM::GetFoMindex:: MapFoM = \""<<FoM<<"\" is not a known FoM"<<endl;
		return 0;
	}
	
	char *GetFoMAxisName(int GetFoMindex){
		char *dummy=new char[100];
		sprintf(dummy,"\0");
		if(GetFoMindex==1)sprintf(dummy,"Keff\0");
		return (char*)dummy;
	}
	double __GetFoM_error;
	double __GetFoM;
	double GetFoM(int FoMindex){
	  if(FoMindex==1){  // keff case
	    __GetFoM=Case(__This_KcodeReader_id)->GetKeffFromStandartOutoutfile();
	    __GetFoM_error=Case(__This_KcodeReader_id)->GetKeffFromStandartOutoutfile__keffError;
	  }
	  return __GetFoM;
	}

	TH2D *Get2dMap(char *MapFoM=0, char *MapTag=0){
		// **************************************************************************
		// MapName must be defined in GetFoM, if MapName=0, this function will find 
		// the FoM in the outp file "IMM MapName ".
		// MapTag can be used if more than two dimentions is analysed, then only
		// cases with a line labled " IMM MapTag " containing the string MapTag will
		// be entered into this map. MapTag MUST be unique and cannot be a substring
		// another MapTag!
		int itr=1;
		int MapTagindex=this->GetPstudyInstructionLine(" MapTag ");
		// fast-forwarding until first MapTag is found:
		if(MapTag){
			if(MapTagindex==-1){cout<<"ERROR::IMM::Get2dMap:: Did not find \" MapTag \" in case "<<__This_KcodeReader_id<<"'s outp"<<endl; return 0;}
			bool testdummy=false;
			while(Case(itr)){
				if(Case(itr)->FindLine(" MapTag ",MapTag,(double)MapTagindex)>=-1){testdummy=true;break;}
					itr++;
			}
			if(!testdummy){
				cout<<"ERROR::IMM::Get2dMap:: You have entered a MapTag which was not found..."<<endl; return 0;
			}
		}
		// checking MapFoM
		int MapFoMindex=this->GetPstudyInstructionLine(" MapFoM ");
		if(!MapFoM){
			if(MapFoMindex==-1){cout<<"ERROR::IMM::Get2dMap:: Did not find \" MapFoM \" in case "<<__This_KcodeReader_id<<"'s outp, try: Get2dMap(\"Keff\");"<<endl; return 0;}
			int dummyint=Case(itr)->FindLine(" MapFoM ",(double)MapFoMindex);
			if(Case(itr)->GetLineLength(MapFoMindex)>=dummyint+8)
				char *MapFoM = (char*)(Case(itr)->GetLine(MapFoMindex)+dummyint+8);
			else cout<<"ERROR::IMM::Get2dMap:: You did not enter a MapFoM - try:  Get2dMap(\"Keff\");"<<endl; return 0;
		} else if(MapFoMindex!=-1){
			int dummyint=Case(itr)->FindLine(" MapFoM ",(double)MapFoMindex);
			if(Case(itr)->GetLineLength(MapFoMindex)>=dummyint+8)
				cout<<"WARNING::IMM::Get2dMap:: You entered a FoM, but there is also a FoM in the file, using the entered FoM"<<endl;
		}
		int FoMindex=GetFoMindex(MapFoM);
		if(!FoMindex)cout<<"ERROR::IMM::Get2dMap:: Unknown FoM"<<endl; return 0;
		// the following instruction-keywords must be written on an IMM line must be in the outp file
		int BinsXindex=this->GetPstudyInstructionLine(" BinsX "); // line must end on three numbers: nBins Min Max 
		int BinsYindex=this->GetPstudyInstructionLine(" BinsY "); // line must end on three numbers: nBins Min Max
		int ThisXindex=this->GetPstudyInstructionLine(" ThisX "); // line must end with current x value
		int ThisYindex=this->GetPstudyInstructionLine(" ThisY "); // line must end with current y value

		// optional IMM inputs:
		int HistNameindex=this->GetPstudyInstructionLine(" HistName ");
		int XaxisNameindex=this->GetPstudyInstructionLine(" XaxisName ");
		int YaxisNameindex=this->GetPstudyInstructionLine(" YaxisName ");

		// Initilization is over...
		// testing validity:
		if(BinsXindex==-1){cout<<"ERROR::IMM::Get2dMap:: Did not find \" BinsX \" in case "<<__This_KcodeReader_id<<"'s outp"<<endl; return 0;}
		if(BinsYindex==-1){cout<<"ERROR::IMM::Get2dMap:: Did not find \" BinsY \" in case "<<__This_KcodeReader_id<<"'s outp"<<endl; return 0;}
		if(ThisXindex==-1){cout<<"ERROR::IMM::Get2dMap:: Did not find \" ThisX \" in case "<<__This_KcodeReader_id<<"'s outp"<<endl; return 0;}
		if(ThisYindex==-1){cout<<"ERROR::IMM::Get2dMap:: Did not find \" ThisY \" in case "<<__This_KcodeReader_id<<"'s outp"<<endl; return 0;}

		if(HistNameindex==-1){cout<<"WARNING::IMM::Get2dMap:: Did not find \" HistName \" in case "<<__This_KcodeReader_id<<"'s outp"<<endl;}
		if(XaxisNameindex==-1){cout<<"WARNING::IMM::Get2dMap:: Did not find \" XaxisName \" in case "<<__This_KcodeReader_id<<"'s outp"<<endl;}
		if(YaxisNameindex==-1){cout<<"WARNING::IMM::Get2dMap:: Did not find \" YaxisName \" in case "<<__This_KcodeReader_id<<"'s outp"<<endl;}

		// fetching x axis
		double *xBins=Case(itr)->Get_ListOfNumbers(BinsXindex);
		int nBinsX=(int)xBins[Case(itr)->Get_ListOfNumbers__arraylength-3];
		double xMin=xBins[Case(itr)->Get_ListOfNumbers__arraylength-2];
		double xMax=xBins[Case(itr)->Get_ListOfNumbers__arraylength-1];

		// fetching x axis
		double *yBins=Case(itr)->Get_ListOfNumbers(BinsYindex);
		int nBinsY=(int)yBins[Case(itr)->Get_ListOfNumbers__arraylength-3];
		double yMin=yBins[Case(itr)->Get_ListOfNumbers__arraylength-2];
		double yMax=yBins[Case(itr)->Get_ListOfNumbers__arraylength-1];

		TH2D *returnhist=new TH2D(TSC::uname(),""
			, nBinsX+1, xMin-(xMax-xMin)/nBinsX, xMax+(xMax-xMin)/nBinsX
			, nBinsY+1, yMin-(yMax-yMin)/nBinsY, yMax+(yMax-yMin)/nBinsY);

		char histname[150];
		sprintf(histname,"\0");

		// fetching names:
		char *theXaxisName=0;
		if(XaxisNameindex!=-1){
			int dummyint=Case(itr)->FindLine(" XaxisName ",(double)XaxisNameindex);
			if(Case(itr)->GetLineLength(XaxisNameindex)>=dummyint+11)
				theXaxisName = (char*)(Case(itr)->GetLine(XaxisNameindex)+dummyint+11);
		}

		char *theYaxisName=0;
		if(YaxisNameindex!=-1){
			int dummyint=Case(itr)->FindLine(" YaxisName ",(double)YaxisNameindex);
			if(Case(itr)->GetLineLength(YaxisNameindex)>=dummyint+11)
				theYaxisName = (char*)(Case(itr)->GetLine(YaxisNameindex)+dummyint+11);
		}
		char *theZaxisName=GetFoMAxisName(FoMindex);
		char *HistNameTitle=0;
		if(HistNameindex!=-1){
			int dummyint=Case(itr)->FindLine(" HistName ",(double)HistNameindex);
			if(Case(itr)->GetLineLength(HistNameindex)>=dummyint+10)
				HistNameTitle = (char*)(Case(itr)->GetLine(HistNameindex)+dummyint+10);
		}

		TSC::NameHist(returnhist,1,
			HistNameTitle,theXaxisName,theYaxisName,theZaxisName);
		returnhist->Sumw2();
		// starting the filling prodecure


		double this_x;
		double this_y;
		int binx;
		int biny;
		double *dummydouble;
		while(EndOfCases){
			if(itr%100==0)cout<<"Now loading: "<<itr<<endl;
			if(Case(itr)==0){
				if(EndOfCases)break;
				itr++;
				continue;
			}
			if(MapTag){
				if(Case(itr)->FindLine(" MapTag ",MapTag,(double)MapTagindex)>=-1){itr++;continue;}
			}
			//fetching x bin
			dummydouble=Case(itr)->Get_ListOfNumbers(ThisXindex);
			this_x=dummydouble[Case(itr)->Get_ListOfNumbers__arraylength];
			//fetching y bin
			dummydouble=Case(itr)->Get_ListOfNumbers(ThisYindex);
			this_y=dummydouble[Case(itr)->Get_ListOfNumbers__arraylength];

			// finding bin:
			binx=returnhist->GetXaxis()->FindBin(this_x);
			biny=returnhist->GetYaxis()->FindBin(this_y);

			// filling histogram:
			if(returnhist->GetBinContent(binx,biny)){cout<<"Error::IMM::Get2dMap: x,y-bin dublicate! Returning Histogram prematurely..."<<endl;return returnhist;}
			returnhist->SetBinContent(binx,biny,GetFoM(FoMindex));
			returnhist->SetBinError(binx,biny,__GetFoM_error);
			itr++;
		}

//		cout<<"number of cases found was "<<itr<<endl;	
		return returnhist;
	}
	

	TH2D *Get_r_h_Map(char *fissiletag=0,char *fertiletag=0,char *moderatortag=0,char *salttag=0, int startindex=0,int endindex=0){
		if(startindex==0)startindex=1;
		if(endindex==0)endindex=100000;
		if(!fissiletag){
			cout<<"WARNING::IMM::Get_r_h_Map:: You did not enter a fissile tag, will be skipped"<<endl;
		}
		if(!fertiletag){
			cout<<"WARNING::IMM::Get_r_h_Map:: You did not enter a fertile tag, will be skipped"<<endl;
		}
		if(!moderatortag){
			cout<<"WARNING::IMM::Get_r_h_Map:: You did not enter a moderator tag, will be skipped"<<endl;
		}
		if(!salttag){
			cout<<"WARNING::IMM::Get_r_h_Map:: You did not enter a salt tag, will be skipped"<<endl;
		}
		int this_radius_index=this->GetPstudyInstructionLine(" ThisRadius ");
		int this_height_index=this->GetPstudyInstructionLine(" ThisHeight ");
		int index_heightbins=this->GetPstudyInstructionLine(" HeightBins ");
		int index_radiusbins=this->GetPstudyInstructionLine(" RadiusBins ");

		int this_moderator_index=this->GetPstudyInstructionLine(" ThisModerator ");
		int this_salt_index=this->GetPstudyInstructionLine(" ThisSalt ");
		int this_fissile_index=this->GetPstudyInstructionLine(" ThisFissile ");
		int this_fertile_index=this->GetPstudyInstructionLine(" ThisFertile ");
		int index_fissile=this->GetPstudyInstructionLine(" FissileMaterials ");
		int index_fertile=this->GetPstudyInstructionLine(" FertileMaterials ");
		
		
		if(this_radius_index==-1){cout<<"ERROR::IMM::Get_r_h_Map:: unable to read ThisRadius"<<endl;return 0;}
		if(this_height_index==-1){cout<<"ERROR::IMM::Get_r_h_Map:: unable to read ThisHeight"<<endl;return 0;}
		if(index_heightbins==-1){cout<<"ERROR::IMM::Get_r_h_Map:: unable to read height bins material list"<<endl;return 0;}
		if(index_radiusbins==-1){cout<<"ERROR::IMM::Get_r_h_Map:: unable to read radius bins material list"<<endl;return 0;}
		
		if(fissiletag)if(this_fissile_index==-1){cout<<"ERROR::IMM::Get_r_h_Map:: unable to read ThisFissile"<<endl;return 0;}
		if(fertiletag)if(this_fertile_index==-1){cout<<"ERROR::IMM::Get_r_h_Map:: unable to read ThisFertile"<<endl;return 0;}
		if(moderatortag)if(this_moderator_index==-1){cout<<"ERROR::IMM::Get_r_h_Map:: unable to read ThisModerator"<<endl;return 0;}
		if(salttag)if(this_salt_index==-1){cout<<"ERROR::IMM::Get_r_h_Map:: unable to read ThisSalt"<<endl;return 0;}
		if(fissiletag)if(index_fissile==-1){cout<<"WARNING::IMM::Get_r_h_Map:: unable to read fissile material list"<<endl;}
		if(fertiletag)if(index_fertile==-1){cout<<"WARNING::IMM::Get_r_h_Map:: unable to read fertile material list"<<endl;}
	
		int itr=startindex;
		bool isSuccess=false;
		while(Case(itr)){
			if(itr>=endindex)break;
			if(fissiletag)if(Case(itr)->FindLine(" ThisFissile ",fissiletag,(double)this_fissile_index)==-1){itr++;continue;}
			if(fertiletag)if(Case(itr)->FindLine(" ThisFertile ",fertiletag,(double)this_fertile_index)==-1){itr++;continue;}
			if(moderatortag)if(Case(itr)->FindLine(" ThisModerator ",moderatortag,(double)this_moderator_index)==-1){itr++;continue;}
			if(salttag)if(Case(itr)->FindLine(" ThisSalt ",salttag,(double)this_salt_index)==-1){itr++;continue;}
			isSuccess=true;
			break;
		}
		if(!isSuccess){cout<<"ERROR:IMM::Get_r_h_Map:: Given Tag set was not found."<<endl;return 0;}
		Case(itr)->printsection(index_radiusbins,1);
		Case(itr)->printsection(index_heightbins,1);

		double *RadiusBins=Case(itr)->Get_ListOfNumbers(index_radiusbins);
		if(Case(itr)->Get_ListOfNumbers__arraylength!=5)cout<<"WARNING:IMM::Get_r_h_Map:: Strange number of radius bins..."<<endl;
		int nRbins=(int)RadiusBins[Case(itr)->Get_ListOfNumbers__arraylength-3];
		double lowR=RadiusBins[Case(itr)->Get_ListOfNumbers__arraylength-2];
		double highR=RadiusBins[Case(itr)->Get_ListOfNumbers__arraylength-1];

		double *HeightBins=Case(itr)->Get_ListOfNumbers(index_heightbins);
		if(Case(itr)->Get_ListOfNumbers__arraylength!=5)cout<<"WARNING:IMM::Get_r_h_Map:: Strange number of height bins..."<<endl;
		int nHbins=(int)HeightBins[Case(itr)->Get_ListOfNumbers__arraylength-3];
		double lowH=HeightBins[Case(itr)->Get_ListOfNumbers__arraylength-2];
		double highH=HeightBins[Case(itr)->Get_ListOfNumbers__arraylength-1];

		TH2D *dummyhist=new TH2D(TSC::uname(),""
			, nRbins+1, lowR-(highR-lowR)/nRbins, highR+(highR-lowR)/nRbins
			, nHbins+1,lowH-(highH-lowH)/nHbins, highH+(highH-lowH)/nHbins);

		char histname[150];
		sprintf(histname,"\0");
		if(salttag)
			if(salttag2name(salttag))
				sprintf(histname,"%s with \0",salttag2name(salttag));
		if(fissiletag)
			if(fissiletag2name(fissiletag))
				sprintf(histname,"%s%s\0",histname,fissiletag2name(fissiletag));
		if(fertiletag)
			if(fertiletag2name(fertiletag))
				sprintf(histname,"%s/%s\0",histname,fertiletag2name(fertiletag));
		if(moderatortag)
			if(moderatortag2name(moderatortag))
				sprintf(histname,"%s using %s\0",histname,moderatortag2name(moderatortag));

		TSC::NameHist(dummyhist,1,
			histname,xaxis_name,yaxis_name);
		dummyhist->Sumw2();
		// starting the filling prodecure

		double keff;
		double keff_err;
		double this_heightbin;
		double this_radiusbin;
		double *dummydouble;

		while(Case(itr)){
			if(itr%100==0)cout<<"Now loading: "<<itr<<endl;
			// fetching radius
			if(fissiletag)if(Case(itr)->FindLine(" ThisFissile ",fissiletag,(double)this_fissile_index)==-1){itr++;continue;}
			if(fertiletag)if(Case(itr)->FindLine(" ThisFertile ",fertiletag,(double)this_fertile_index)==-1){itr++;continue;}
			if(moderatortag)if(Case(itr)->FindLine(" ThisModerator ",moderatortag,(double)this_moderator_index)==-1){itr++;continue;}
			if(salttag)if(Case(itr)->FindLine(" ThisSalt ",salttag,(double)this_salt_index)==-1){itr++;continue;}
			dummydouble=Case(itr)->Get_ListOfNumbers(this_radius_index);
			if(Case(itr)->Get_ListOfNumbers__arraylength!=2){cout<<"ERROR::IMM::Get_r_h_Map something is wrong (a)"<<endl;return 0;}
			this_radiusbin=dummydouble[1];
			// fetching height
			dummydouble=Case(itr)->Get_ListOfNumbers(this_height_index);
			if(Case(itr)->Get_ListOfNumbers__arraylength!=2){cout<<"ERROR::IMM::Get_r_h_Map something is wrong (b)"<<endl;return 0;}

			this_heightbin=dummydouble[1];
			// fetching keff
			keff=Case(itr)->GetKeffFromStandartOutoutfile();
			keff_err=Case(itr)->GetKeffFromStandartOutoutfile__keff;
			if(keff==0||keff_err==0){
				cout<<"WARNING::IMM::Get_r_h_Map Did not find keff in outp in: "<<this->GetCaseName(itr)<<endl;
			}
			int binx=dummyhist->GetXaxis()->FindBin(this_radiusbin);
			int biny=dummyhist->GetYaxis()->FindBin(this_heightbin);

			if(keff&&dummyhist->GetBinContent(binx,biny)){cout<<"Error::IMM::Get_r_h_Map Bin dublicate"<<endl;return 0;}
			dummyhist->SetBinContent(binx,biny,keff);
			dummyhist->SetBinError(binx,biny,keff_err);

			//dummyhist->Fill();
			itr++;
			if(itr>=endindex)break;
		}

//		cout<<"number of cases found was "<<itr<<endl;	
		return dummyhist;
	}
	
};

class Logbook{
protected:
	string Line[10000];
	int lineitr;
public:
	Logbook(){
		lineitr=0;
	}
	~Logbook(){}
	void WriteLine(char *input, int line, bool doprint=false){
		if(line>=10000){cout<<"ERROR::Logbook::WriteLine:: Too many lines in Logbook"<<endl;return ;}
		Line[line] = string(input);
		if(doprint)cout<<Line[line]<<endl;

	}
	void WriteLine(char *input,bool doprint=false){
		WriteLine(input,lineitr,doprint);
			lineitr++;

	}
	int GetLineIndex(){
		return lineitr;
	}
	void GoToLineIndex(int lineindex){
		lineitr=lineindex;
	}
	void WriteLogfile(char *filename,int from=0,int to=0){
		if(lineitr<=0){cout<<"ERROR::Logbook::WriteLogfile:: No log to write"<<endl;return ;}
		if(to<=0)to=lineitr-1;
		ofstream outputfile;
		outputfile.open (filename);
		for(int i=from;i<to+1;i++){
			outputfile << Line[i] << "\n";
		}
		outputfile.close();
	}
};


class Isotope_KcodeNeutronics {
public:	
	int step; 
	double duration;
	double time;
	double power;
	double keff;
	double flux;
	double ave_nu;
	double ave_q;
	double burnup;
	double source;
	Isotope_KcodeNeutronics(int instep, double induration, double intime, double inpower, double inkeff,
		double influx, double inave_nu, double inave_q, double inburnup, double insource){
		step=instep; 
		duration=induration;
		time=intime;
		power=inpower;
		keff=inkeff;
		flux=influx;
		ave_nu=inave_nu;
		ave_q=inave_q;
		burnup=inburnup;
		source=insource;
	}
	Isotope_KcodeNeutronics(string NeutronicsLine){
		stringstream stream(NeutronicsLine);
		source=-1;
		double toomany_numbers=-1;
		stream >> step >> duration >> time >> power >> keff >> flux >> ave_nu >> ave_q >> burnup >> source >> toomany_numbers;

//		cout<<step<<" - "<<duration<<" - "<<time<<" - "<<power<<" - "<<keff<<
//			" - "<<flux<<" - "<<ave_nu<<" - "<<ave_q<<" - "<<burnup<<" - "<<source<<endl;
		if(source==-1||toomany_numbers!=-1)cout<<"ERROR::Isotope_KcodeNeutronics::Isotope_KcodeNeutronics:: string format incorrect!!"<<endl;
	}
	Isotope_KcodeNeutronics(Isotope_KcodeNeutronics *KNL){
		step=KNL->step; 
		duration=KNL->duration;
		time=KNL->time;
		power=KNL->power;
		keff=KNL->keff;
		flux=KNL->flux;
		ave_nu=KNL->ave_nu;
		ave_q=KNL->ave_q;
		burnup=KNL->burnup;
		source=KNL->source;
	}
	~Isotope_KcodeNeutronics(){}
};

class BurnupStep{
private :

protected:

public :
	Isotope_KcodeNeutronics *Neutronics;
	
	BurnupStep(){
		Neutronics=0;
	}

	~BurnupStep(){
		if(Neutronics)delete Neutronics;
	}
			// ------------------------------------------------------------------------------------------
	// ------------------------------------isotope_KcodeNeutronics-------------------------------
	// ------------------------------------------------------------------------------------------
	void SetNeutronics(int instep, double induration, double intime, double inpower, double inkeff,double influx, double inave_nu, double inave_q, double inburnup, double insource){
		if(Neutronics)delete Neutronics;
		Neutronics=new Isotope_KcodeNeutronics(instep, induration, intime, inpower, inkeff,influx,inave_nu,inave_q,inburnup,insource);
	}
	void SetNeutronics(string line){
		if(Neutronics)delete Neutronics;
		Neutronics=new Isotope_KcodeNeutronics((string)line);
	}
	void SetNeutronics(Isotope_KcodeNeutronics *KN){
		if(Neutronics)delete Neutronics;
		Neutronics=new Isotope_KcodeNeutronics((Isotope_KcodeNeutronics*)KN);
	}

};
class Isotope_ZAID{
private:
	char tmp_tmptag[6];
protected:	
	void Initialize(double inzaid){
		ZAID=inzaid/1;
		Z=inzaid/1000;
		if(Z>100){cout<<"ERROR::isotope_ZAID::isotope_ZAID:: cannot handle Z>=100"<<endl;return;}
		A=(int)inzaid%1000;
		N=A-Z;
		temperature=(((int)(inzaid*100)%10)+1)*2.5301E-08;
		// creating ZAID tag:
		if(A<10)sprintf(ZAIDtag,"%d00%d\0",Z,A);
		else if(A<100)sprintf(ZAIDtag,"%d0%d\0",Z,A);
		else sprintf(ZAIDtag,"%d%d\0",Z,A);

		// creating tmp tag:
		if(temperature==0)tmp_tmptag[1]=0;
		else sprintf(tmp_tmptag,"%1.2f\0",tmpkernelindendifier(temperature));
		for(int i=0;i<5;i++)tmptag[i]=tmp_tmptag[i+1];
		// assempeling ZAID card:
		if(tmptag[0]!=0){
			sprintf(ZAIDcard,"%s%s\0",ZAIDtag,tmptag);
		} else {
			sprintf(ZAIDcard,"%s\0",ZAIDtag);
		}
	}
	double tmpkernelindendifier(double temp){
		if(temp>2.1543E-07*1.5)cout<<"WARNING::isotope_ZAID::tmpkernelindendifier:: temperature is very high! (i.e. "<<E2T(temp)<<" K - assuming .74C)"<<endl;
		if(temp!=0&&temp<2.5301E-08*.95)cout<<"WARNING::isotope_ZAID::tmpkernelindendifier:: temperature is low! (i.e. "<<E2T(temp)<<"K - assuming .70C)"<<endl;
		if(temp<0) {
			cout<<"ERROR::isotope_ZAID::tmpkernelindendifier:: WTF: tmp="<<temp<<endl;
			return -1;
		}
		if(temp==0)return 0;
		else if(temp<(2.5301E-08+5.1704E-08)/2.)return .70;
		else if(temp<(5.1704E-08+7.7556E-08)/2.)return .71;
		else if(temp<(7.7556E-08+1.0341E-07)/2.)return .72;
		else if(temp<(1.0341E-07+2.1543E-07)/2.)return .73;
		return .74;
	};
public:
	//avaliable variables:
	int ZAID;
	int Z;
	int A;
	int N;
	double temperature;
	char ZAIDcard[15];
	char ZAIDtag[10];
	char tmptag[5];


	//functions:	
	Isotope_ZAID(Isotope_ZAID *inZAID){
		Isotope_ZAID(inZAID->ZAIDcard);
	}
	Isotope_ZAID(char *zaidcard){
		if(TSC::isNumberFromChar(zaidcard)<0){cout<<"ERROR::isotope_ZAID::isotope_ZAID:: could not read zaidcard"<<endl;return;}
		Initialize(TSC::GetNumberFromChar(zaidcard));
	}
	Isotope_ZAID(int inzaid, double intemperature=0.){
		temperature=intemperature;
		if(intemperature>1){
			cout<<"WARNING::isotope_ZAID::isotope_ZAID M Temperature converted from Kelvin ("<<intemperature<<" K) to energy ("<<T2E(intemperature)<<" MeV) "<<endl;
			temperature=T2E(intemperature);
		}
		Initialize(inzaid+tmpkernelindendifier(temperature));
	}
	Isotope_ZAID(int inz,int ina, double intemperature=0.){
		temperature=intemperature;
		if(intemperature>1){
			cout<<"WARNING::isotope_ZAID::isotope_ZAID M Temperature converted from Kelvin ("<<intemperature<<" K) to energy ("<<T2E(intemperature)<<" MeV) "<<endl;
			temperature=T2E(intemperature);
		}
		Initialize(inz+ina+tmpkernelindendifier(temperature));
	}


};

class Material{
	private :

	protected:

	public :
	
	double Temperature;

};

class Isotope{
private :

protected:

public :
// generics:
//	  no.  zaid    mass      activity  spec.act.  atom den.   atom fr.   mass fr.
//	   1  90231  0.000E+00  0.000E+00  0.000E+00  0.000E+00  0.000E+00  0.000E+00


	BurnupStep *BurnupStepPtr;
	Material *MaterialPtr;

	Isotope_ZAID *ZAID;

	int no;
	int zaid; // special case of ZAID
	double mass;
	double activity;
	double spec_act;
	double atom_den;
	double atom_fr;
	double mass_fr;

	Isotope(int inzaid, double inatom_fr, BurnupStep *inBurnupStepPtr=0, Material *inMaterialPtr=0){
		no=0;
		zaid=inzaid;
		mass=0;
		activity=0;
		spec_act=0;
		atom_den=0;
		atom_fr=inatom_fr;
		mass_fr=0;

		BurnupStepPtr=inBurnupStepPtr;  // this class pointer must be linked
		MaterialPtr=inMaterialPtr;      // this class pointer must be linked
		double temperature=0;
		if(MaterialPtr)temperature=MaterialPtr->Temperature;
		ZAID=new Isotope_ZAID(zaid,temperature);
	}

	Isotope(int inno, int inzaid, double inmass, double inactivity, double inspec_act, double inatom_den
		, double inatom_fr, double inmass_fr, BurnupStep *inBurnupStepPtr=0, Material *inMaterialPtr=0){
		no=inno;
		zaid=inzaid;
		mass=inmass;
		activity=inactivity;
		spec_act=inspec_act;
		atom_den=inatom_den;
		atom_fr=inatom_fr;
		mass_fr=inmass_fr;

		BurnupStepPtr=inBurnupStepPtr;  // this class pointer must be linked
		MaterialPtr=inMaterialPtr;      // this class pointer must be linked
		double temperature=0;
		if(MaterialPtr)temperature=MaterialPtr->Temperature;
		ZAID=new Isotope_ZAID(zaid,temperature);
	}

	Isotope(string InventoryLine, BurnupStep *inBurnupStepPtr=0, Material *inMaterialPtr=0){
		BurnupStepPtr=inBurnupStepPtr;  // this class pointer must be linked
		MaterialPtr=inMaterialPtr;      // this class pointer must be linked
		stringstream stream(InventoryLine);
		mass_fr=-1;
		double toomany_numbers=-1;
		stream >> no >> zaid >> mass >> activity >> spec_act >> atom_den >> atom_fr >> mass_fr >> toomany_numbers;
//		cout<<no<<" - "<<zaid<<" - "<<mass<<" - "<<activity<<" - "<<spec_act<<
//			" - "<<atom_den<<" - "<<atom_fr<<" - "<<mass_fr<<" - "<<mass_fr<<endl;
		if(mass_fr==-1||toomany_numbers!=-1)cout<<"ERROR::Isotope::Isotope:: string format incorrect!!"<<endl;
		double temperature=0;
		if(MaterialPtr)temperature=MaterialPtr->Temperature;
		ZAID=new Isotope_ZAID(zaid,temperature);
	}
	~Isotope(){
		if(ZAID)delete ZAID;
	}


	/*
	// ------------------------------------------------------------------------------------------
	// ------------------------------------ZAID--------------------------------------------------
	// ------------------------------------------------------------------------------------------
	void SetZAID(char *zaidcard){
		if(ZAID)delete ZAID;
		ZAID=new Isotope_ZAID(zaidcard);
		zaid=ZAID->ZAID;
	}
	void SetZAID(int inz,int ina, double intemperature=0.){
		if(ZAID)delete ZAID;
		ZAID=new Isotope_ZAID((int)inz,(int)ina,(double)intemperature);
		zaid=ZAID->ZAID;
	}
	void SetZAID(int inzaid, double intemperature=0.){
		if(ZAID)delete ZAID;
		ZAID=new Isotope_ZAID((int)inzaid,(double)intemperature);
		zaid=ZAID->ZAID;
	}
	void SetZAID(Isotope_ZAID *inZAID){
		if(ZAID)delete ZAID;
		ZAID=new Isotope_ZAID((Isotope_ZAID*)inZAID);
		zaid=ZAID->ZAID;
	}
	*/

};


class Reprocessor{
	char filename[150];
	int NeutronicsLinenumber;
	int Material_Linenumber;
private: 
	KcodeReader* KKR;
public:
	Reprocessor(char *infilename,char *outfile="Reprocessor_deck"){
		sprintf(filename,"%s\0",infilename);
		cout<<filename<<endl;
		KKR= new KcodeReader(filename);
		NeutronicsLinenumber=KKR->FindLine("neutronics and burnup",-KKR->LastLineIndex);
		if(NeutronicsLinenumber>0)
			;//KKR->printsection(NeutronicsLinenumber-5,25);
		else {cout<<"ERROR::Reprocessor::Reprocessor:: coould not find neutronic table"<<endl;return ;}
		Material_Linenumber=KKR->FindLine("nuclide",NeutronicsLinenumber);
		if(Material_Linenumber>0)
			KKR->printsection(Material_Linenumber-50,150);
		else {cout<<"ERROR::Reprocessor::Reprocessor:: coould not find neutronic table"<<endl;return ;}
	}
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

