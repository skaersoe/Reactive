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
private:
	ifstream infile;
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
		if(index<0||index>LastLineIndex)return 0;
		if(line[index].size()==0)return 0;
		char *dummychar=new char[line[index].size()+1];
		dummychar[line[index].size()]=0;
		memcpy(dummychar,line[index].c_str(),line[index].size());
		int i=0;
		int j=0;
		int k=0;
		for(int i=0;i<line[index].size();i++){
			if((int)dummychar[i]==32){
				linesegment[k][j]=0;
				linesegment[k][j+1]=0;
				if(j>0){
					linesegment[k][j+1]=0;
					k++;
				}
				j=0;
				continue;
			}
			linesegment[k][j]=dummychar[i];
			j++;
		}
		if(j>0){
			linesegment[k][j]=0;
			linesegment[k][j+1]=0;
		}
		k++;
		return k;
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
		sprintf(infilename,filename);
		infile.open(filename);
		__file_exist=false;
		if(!infile){if(writefilewarning)cout<<"ERROR::KcodeReader:: no file loaded!"<<endl;return;}
		__file_exist=true;
		LoadLines(GetNLines(filename));
	}
	//--------------------------------------------------
	//--------------------------------------------------
	// destructor
	//--------------------------------------------------
	//--------------------------------------------------	
	~KcodeReader(){
		if(__file_exist)infile.close();
	}

	int GetLineLength(int linenumber){
		
		return line[linenumber].length();
	}
	char __GetLineCharArray[1000];
	char *GetLine(int linenumber){
		return (char*)line[linenumber].c_str();
	}
	string GetLineString(int linenumber){
		return line[linenumber];
	}
	char *GetLine(char *searchword,int linenumber){ // fetches line from after searchword
		int lengthofword;
		for(lengthofword=0;lengthofword<1000;lengthofword++){if(searchword[lengthofword]==0)break;}
		int dummyint=this->FindLine(searchword,(double)linenumber);
		if(dummyint<=0)return 0;
		if(this->GetLineLength(linenumber)>=dummyint+lengthofword)
			return (char*)(this->GetLine(linenumber)+dummyint+lengthofword);
		return 0;
	}
	char *GetLineClean(int linenumber){ // get new line where spaces in end have been cleaned
		char *dummychar=GetLine(linenumber);
		int lengthofline;
		for(lengthofline=0;lengthofline<1000;lengthofline++){if(dummychar[lengthofline]==0)break;}
			while(lengthofline>=0&&(dummychar[lengthofline]==32||dummychar[lengthofline]==0)){
				dummychar[lengthofline]=0;
				lengthofline--;
			}
		return (char*)dummychar;
	}
	char *GetLineClean(char *searchword,int linenumber){ // fetches line from after searchword, and cleans spaces in ind
		int lengthofword;
		for(lengthofword=0;lengthofword<1000;lengthofword++){if(searchword[lengthofword]==0)break;}
		int dummyint=this->FindLine(searchword,(double)linenumber);
		if(dummyint<=0)return 0;
		if(this->GetLineLength(linenumber)>=dummyint+lengthofword){
			char *dummychar=(char*)(this->GetLine(linenumber)+dummyint+lengthofword);
			int lengthofline;
			for(lengthofline=0;lengthofline<1000;lengthofline++){if(dummychar[lengthofline]==0)break;}
			while(lengthofline>=0&&(dummychar[lengthofline]==32||dummychar[lengthofline]==0)){
				dummychar[lengthofline]=0;
				lengthofline--;
			}
			return (char*)dummychar;
		}
		return 0;
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

	double GetCaptureFromStandartOutoutfile__Result;
	double GetCaptureFromStandartOutoutfile(){
		int resultline=this->FindLine("neutron creation","neutron loss",-(this->LastLineIndex-1));
		if(resultline==-1){
			GetCaptureFromStandartOutoutfile__Result=0;
			return 0;
		}
		int captuerline=this->FindLine("photonuclear","capture",resultline);
		if(captuerline==-1)return 0;
		GetCaptureFromStandartOutoutfile__Result=this->ReadNumberInLine(captuerline,-5,false);
		return GetCaptureFromStandartOutoutfile__Result;
	}
	double GetFissionFromStandartOutoutfile__Result;
	double GetFissionFromStandartOutoutfile(){
		int resultline=this->FindLine("neutron creation","neutron loss",-(this->LastLineIndex-1));
		if(resultline==-1){
			GetCaptureFromStandartOutoutfile__Result=0;
			return 0;
		}
		int fissionline=this->FindLine("prompt fission","loss to fission",resultline);
		if(fissionline==-1)return 0;
		GetFissionFromStandartOutoutfile__Result=this->ReadNumberInLine(fissionline,-5,false);
		return GetFissionFromStandartOutoutfile__Result;
	}
	double GetEscapeFromStandartOutoutfile__Result;
	double GetEscapeFromStandartOutoutfile(){
		int resultline=this->FindLine("neutron creation","neutron loss",-(this->LastLineIndex-1));
		if(resultline==-1){
			GetCaptureFromStandartOutoutfile__Result=0;
			return 0;
		}
		int escapeline=this->FindLine("source","escape",resultline);
		if(escapeline==-1)return 0;
		GetEscapeFromStandartOutoutfile__Result=this->ReadNumberInLine(escapeline,-5,false);
		return GetEscapeFromStandartOutoutfile__Result;
	}
	double GetKeffFromStandartOutoutfile__Error;
	double GetKeffFromStandartOutoutfile__Result;
	double GetKeffFromStandartOutoutfile(){
		int resultline=this->FindLine("final result",-(this->LastLineIndex-1));
		if(resultline==-1){
			GetKeffFromStandartOutoutfile__Error=0;
			GetKeffFromStandartOutoutfile__Result=0;
			return 0;
		}
		GetKeffFromStandartOutoutfile__Error=this->ReadNumberInLine(resultline,-2,false);
		GetKeffFromStandartOutoutfile__Result=this->ReadNumberInLine(resultline,-1,false);
		//cout<<GetKeffFromStandartOutoutfile__Result<<" pm "<<GetKeffFromStandartOutoutfile__Error<<endl;
		return GetKeffFromStandartOutoutfile__Result;
	}



	int Get_ListOfNumbers__arraylength;
	double *Get_ListOfNumbers(int linenumber){
		int numberosegments=this->ReadLineIntoSegments(linenumber);
		Get_ListOfNumbers__arraylength=0;
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
		double *the_array=new double[NumberOfNumbers];//(double*)malloc(NumberOfNumbers*sizeof(double));
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

};

class Decay{
public:
	int ZAID;
	int DecayZAID; // zaid
	int DecayZAID2; // zaid
	double Thalf;
	double Prob1;
	double ProbSF;
	bool has2nddecaymode;
	Decay(int zaid, double thalf,int DecayZaid){
		ZAID=zaid;
		if(DecayZaid>1){
			DecayZAID=DecayZaid;
		} else if(DecayZaid==-4){ // alpha
			DecayZAID=zaid-2004;
		} else if(DecayZaid==-1){ // beta
			DecayZAID=zaid+1000;
		} else if(DecayZaid==-2){ //beta+ or e-cap
			DecayZAID=zaid-1000;
		}
		Thalf=thalf;
		Prob1=1;
		ProbSF=0;
		DecayZAID2=0;
		has2nddecaymode=false;
	}
	void AddSecundDecay(double probSF, double prob2=0,int DecayZaid2=-1){
		ProbSF=probSF;
		Prob1-=probSF;
		Prob1-=prob2;
		if(DecayZaid2>1){
			DecayZAID2=DecayZaid2;
		} else if(DecayZaid2==-4){ // alpha
			DecayZAID2=ZAID-2004;
		} else if(DecayZaid2==-1){ // beta
			DecayZAID2=ZAID+1000;
		} else if(DecayZaid2==-2){ //beta+ or e-cap
			DecayZAID2=ZAID-1000;
		}
		has2nddecaymode=true;
	}
	double lambda(){
		return log10(2.)/Thalf;
	}
	double DecayRate(double timeunitinsecunds=1.){ //  N(t=1)/N_0
		return exp(-timeunitinsecunds*log10(2.)/Thalf);
	}
	double DecaysPerTime(double timeunitinsecunds=1.){ //  as a fraction
		return 1-DecayRate(timeunitinsecunds);
	}

};

class DecayList{
public:
	Decay *D[10000];
	// prob2 = 1-prob1-probsf
	DecayList(){
		int itr=0;
		double m=60.;
		double h=60.*m;
		double d=24.*h;
		double y=365.242*d;
		double ky=1.e3*y;
		double My=1.e6*y;
		double Gy=1.e9*y;
		//Missing decay data for isotope with zaid: 96247
		//Missing decay data for isotope with zaid: 96248
		

		D[itr]=new Decay(90227,		18.68	*d	,-4);itr++;
		D[itr]=new Decay(90228,		1.9116	*y	,-4);itr++;
		D[itr]=new Decay(90229,		7.34	*ky	,-4);itr++;
		D[itr]=new Decay(90230,		75.38	*ky	,-4);itr++;
		D[itr]=new Decay(90231,		25.52	*h	,-1);itr++;
		D[itr]=new Decay(90232,		14.05	*Gy	,-4);itr++;
		D[itr]=new Decay(90233,		21.83	*m	,-1);itr++;
		D[itr]=new Decay(90234,		24.1	*d	,-1);itr++;
		D[itr]=new Decay(90235,		7.2		*m	,-1);itr++;

		D[itr]=new Decay(91229,		1.5		*d	,-4);itr++;
		D[itr]=new Decay(91230,		17.4	*d	,-2);
			D[itr]->AddSecundDecay(0,0.084,-1);		 itr++;
		D[itr]=new Decay(91231,		231.04	*ky	,-4);itr++;
		D[itr]=new Decay(91232,		1.31	*d	,-1);itr++;
		D[itr]=new Decay(91233,		26.975	*d	,-1);itr++;
		D[itr]=new Decay(91234,		6.7		*h	,-1);itr++;
		D[itr]=new Decay(91235,		24.44	*m	,-1);itr++;
		
		D[itr]=new Decay(92230,		20.8	*d	,-4);itr++;
		D[itr]=new Decay(92231,		4.2		*d	,-2);itr++;
		D[itr]=new Decay(92232,		68.9	*y	,-4);itr++;
		D[itr]=new Decay(92233,		159.2	*ky	,-4);itr++;
		D[itr]=new Decay(92234,		245.4	*ky	,-4);itr++;
		D[itr]=new Decay(92235,		740		*My	,-4);itr++;
		D[itr]=new Decay(92236,		23.42	*My	,-4);itr++;
		D[itr]=new Decay(92237,		6.75	*d	,-1);itr++;
		D[itr]=new Decay(92238,		4.468	*Gy	,-4);itr++;
		D[itr]=new Decay(92239,		23.45	*m	,-1);itr++;
		D[itr]=new Decay(92240,		14.1	*h	,-1);itr++;

		D[itr]=new Decay(93235,		396.1	*d	,-2);itr++;
		D[itr]=new Decay(93236,		154.	*ky	,-2);
			D[itr]->AddSecundDecay(0,0.125,-4);		 itr++;
		D[itr]=new Decay(93237,		2.144	*My	,-4);itr++;
		D[itr]=new Decay(93238,		2.117	*d	,-1);itr++;
		D[itr]=new Decay(93239,		2.356	*d	,-1);itr++;
		D[itr]=new Decay(93240,		61.9	*m	,-1);itr++;

		D[itr]=new Decay(94236,		2.858	*y	,-4);itr++;
		D[itr]=new Decay(94237,		45.2	*d	,-2);itr++;
		D[itr]=new Decay(94238,		87.7	*y	,-4);itr++;
		D[itr]=new Decay(94239,		24.11	*ky	,-4);itr++;
		D[itr]=new Decay(94240,		6.561	*ky	,-4);itr++;
		D[itr]=new Decay(94241,		14.29	*y	,-1);itr++;
		D[itr]=new Decay(94242,		375		*ky	,-4);itr++;
		D[itr]=new Decay(94243,		4.956	*h	,-1);itr++;
		D[itr]=new Decay(94244,		800.	*My	,-4);itr++;
		D[itr]=new Decay(94245,		10.5	*h	,-1);itr++;
		D[itr]=new Decay(94246,		10.84	*d	,-1);itr++;
		
		D[itr]=new Decay(95240,		50.8	*h	,-2);itr++;
		D[itr]=new Decay(95241,		432.2	*y	,-4);itr++;
		D[itr]=new Decay(95242,		16.02	*h	,-1);
			D[itr]->AddSecundDecay(0,0.173,-2);		 itr++;
		D[itr]=new Decay(95243,		7.370	*ky	,-4);itr++;
		D[itr]=new Decay(95244,		10.1	*h	,-1);itr++;
		D[itr]=new Decay(95245,		2.05	*h	,-1);itr++;
		D[itr]=new Decay(95246,		39		*m	,-1);itr++;

		D[itr]=new Decay(96241,		32.8	*d	,-2);itr++;
		D[itr]=new Decay(96242,		162.8	*d	,-4);itr++;
		D[itr]=new Decay(96243,		29.1	*y	,-4);itr++;
		D[itr]=new Decay(96244,		18.1	*y	,-4);itr++;
		D[itr]=new Decay(96245,		8.5		*ky	,-4);itr++;
		D[itr]=new Decay(96246,		4.76	*ky	,-4);itr++;
		D[itr]=0;
	}
	int itr(int zaid){
		int jtr=0;
	while(D[jtr]!=0){
			if(D[jtr]->ZAID==zaid){
				return jtr;
			}
			jtr++;
		}
		return -1;
	}
	Decay *GetDecay(int zaid){
		if(itr(zaid)!=-1){
			return (Decay*)D[itr(zaid)];
		}
		return 0;
	}
	double GetDecayFraction(int zaid, int timeunitinsecunds=1){
		if(D[itr(zaid)])return D[itr(zaid)]->DecaysPerTime(timeunitinsecunds);
	return 0;
	}
	double GetDecayProduct(int zaid, bool getsecundary){
		if(getsecundary){
			if(D[itr(zaid)]){
				if(D[itr(zaid)]->has2nddecaymode)return D[itr(zaid)]->DecayZAID2;
			}
			cout<<"ERROR::DecayList::GetDecayProduct:: operation not possible"<<endl;
			return 0;
		}
		if(D[itr(zaid)]){
			if(!D[itr(zaid)]->has2nddecaymode)return D[itr(zaid)]->DecayZAID;
			return -D[itr(zaid)]->DecayZAID;
		}
		return 0;
	}
};

class OmitList{
public:
	int List[10000];
	OmitList(){
		int i=0;
		List[i]=18039;i++;
		List[i]=18041;i++;
		List[i]=18042;i++;
		List[i]=18043;i++;
		List[i]=19042;i++;
		List[i]=19043;i++;
		List[i]=19044;i++;
		List[i]=19045;i++;
		List[i]=19046;i++;
		List[i]=20041;i++;
		List[i]=76188;i++;
		List[i]=78192;i++;

		List[i]=20049;i++;
		List[i]=30064;i++;
		List[i]=76189;i++;
		List[i]=76191;i++;
		List[i]=77192;i++;
		List[i]=78193;i++;

		List[i]=11023;i++;
		List[i]=28058;i++;
		List[i]=74182;i++;
		List[i]=74183;i++;
		List[i]=74184;i++;
		List[i]=74186;i++;
		
		List[i]=6012;i++;
		List[i]=6013;i++;
		List[i]=6014;i++;
		List[i]=7016;i++;
		List[i]=8018;i++;
		List[i]=8019;i++;
		List[i]=9018;i++;
		List[i]=10021;i++;
		List[i]=10022;i++;
		List[i]=20045;i++;
		List[i]=20047;i++;
		List[i]=21047;i++;
		List[i]=21044;i++;
		List[i]=21046;i++;
		List[i]=21048;i++;
		List[i]=22045;i++;
		List[i]=22051;i++;
		List[i]=23047;i++;
		List[i]=23048;i++;
		List[i]=23049;i++;
		List[i]=23050;i++;
		List[i]=23051;i++;
		List[i]=23052;i++;
		List[i]=23053;i++;
		List[i]=23054;i++;
		List[i]=24049;i++;
		List[i]=24051;i++;
		List[i]=24055;i++;
		List[i]=24056;i++;
		List[i]=25051;i++;
		List[i]=25052;i++;
		List[i]=25053;i++;
		List[i]=25054;i++;
		List[i]=25056;i++;
		List[i]=25057;i++;
		List[i]=25058;i++;
		List[i]=26053;i++;
		List[i]=26055;i++;
		List[i]=26059;i++;
		List[i]=26060;i++;
		List[i]=26061;i++;
		List[i]=27057;i++;
		List[i]=27060;i++;
		List[i]=27061;i++;
		List[i]=27062;i++;
		List[i]=27063;i++;
		List[i]=27064;i++;
		List[i]=28057;i++;
		List[i]=28063;i++;
		List[i]=28065;i++;
		List[i]=29062;i++;
		List[i]=29064;i++;
		List[i]=29066;i++;
		List[i]=30065;i++;
		List[i]=30066;i++;
		List[i]=30067;i++;
		List[i]=30068;i++;
		List[i]=30069;i++;
		List[i]=30070;i++;
		List[i]=31070;i++;
		List[i]=32071;i++;
		List[i]=32075;i++;
		List[i]=33072;i++;
		List[i]=33073;i++;
		List[i]=34075;i++;
		List[i]=34081;i++;
		List[i]=35080;i++;
		List[i]=36079;i++;
		List[i]=36081;i++;
		List[i]=38085;i++;
		List[i]=39086;i++;
		List[i]=39087;i++;
		List[i]=39088;i++;
		List[i]=39092;i++;
		List[i]=39093;i++;
		List[i]=40088;i++;
		List[i]=40089;i++;
		List[i]=40097;i++;
		List[i]=41091;i++;
		List[i]=41092;i++;
		List[i]=41096;i++;
		List[i]=41097;i++;
		List[i]=41098;i++;
		List[i]=41099;i++;
		List[i]=41100;i++;
		List[i]=42091;i++;
		List[i]=42093;i++;
		List[i]=42101;i++;
		List[i]=43097;i++;
		List[i]=43098;i++;
		List[i]=44097;i++;
		List[i]=45104;i++;
		List[i]=45106;i++;
		List[i]=45107;i++;
		List[i]=45108;i++;
		List[i]=45109;i++;
		List[i]=45110;i++;
		List[i]=45111;i++;
		List[i]=46103;i++;
		List[i]=46109;i++;
		List[i]=46111;i++;
		List[i]=46112;i++;
		List[i]=46113;i++;
		List[i]=46114;i++;
		List[i]=47106;i++;
		List[i]=47108;i++;
		List[i]=47110;i++;
		List[i]=48107;i++;
		List[i]=48109;i++;
		List[i]=48115;i++;
		List[i]=49114;i++;
		List[i]=49116;i++;
		List[i]=49117;i++;
		List[i]=49118;i++;
		List[i]=49119;i++;
		List[i]=49120;i++;
		List[i]=49121;i++;
		List[i]=49122;i++;
		List[i]=49123;i++;
		List[i]=49124;i++;
		List[i]=49125;i++;
		List[i]=50121;i++;
		List[i]=51122;i++;
		List[i]=52121;i++;
		List[i]=52127;i++;
		List[i]=52129;i++;
		List[i]=53128;i++;
		List[i]=53132;i++;
		List[i]=53133;i++;
		List[i]=53134;i++;
		List[i]=54125;i++;
		List[i]=54127;i++;
		List[i]=56131;i++;
		List[i]=58137;i++;
		List[i]=60149;i++;
		List[i]=61145;i++;
		List[i]=61146;i++;
		List[i]=62145;i++;
		List[i]=62146;i++;
		List[i]=64150;i++;
		List[i]=64151;i++;
		List[i]=64159;i++;
		List[i]=66157;i++;
		List[i]=66159;i++;
		List[i]=67163;i++;
		List[i]=67164;i++;
		List[i]=67166;i++;
		List[i]=68163;i++;
		List[i]=68165;i++;
		List[i]=68169;i++;
		List[i]=69166;i++;
		List[i]=69167;i++;
		List[i]=69168;i++;
		List[i]=69169;i++;
		List[i]=69170;i++;
		List[i]=69171;i++;
		List[i]=69172;i++;
		List[i]=69173;i++;
		List[i]=70168;i++;
		List[i]=70169;i++;
		List[i]=70170;i++;
		List[i]=70171;i++;
		List[i]=70172;i++;
		List[i]=70173;i++;
		List[i]=70174;i++;
		List[i]=70175;i++;
		List[i]=70176;i++;
		List[i]=71173;i++;
		List[i]=71174;i++;
		List[i]=71177;i++;
		List[i]=72175;i++;
		List[i]=72181;i++;
		List[i]=72182;i++;
		List[i]=72183;i++;
		List[i]=73179;i++;
		List[i]=73180;i++;
		List[i]=73183;i++;
		List[i]=73184;i++;
		List[i]=73185;i++;
		List[i]=73186;i++;
		List[i]=74179;i++;
		List[i]=74180;i++;
		List[i]=74181;i++;
		List[i]=74185;i++;
		List[i]=74187;i++;
		List[i]=74188;i++;
		List[i]=75185;i++;
		List[i]=75186;i++;
		List[i]=75184;i++;
		List[i]=75186;i++;
		List[i]=75188;i++;
		List[i]=76185;i++;
		List[i]=76186;i++;
		List[i]=76187;i++;
		List[i]=77188;i++;
		List[i]=77189;i++;
		List[i]=77190;i++;
		List[i]=78190;i++;
		List[i]=78191;i++;
		List[i]=86221;i++;
		List[i]=86222;i++;
		List[i]=87222;i++;
		List[i]=87223;i++;
		List[i]=86219;i++;
		List[i]=86220;i++;
		List[i]=87221;i++;
		List[i]=88222;i++;
		List[i]=88227;i++;
		List[i]=89223;i++;
		List[i]=89224;i++;
		List[i]=89228;i++;
		List[i]=90231;i++;
		List[i]=91229;i++;
		List[i]=91230;i++;
		List[i]=92229;i++;
		List[i]=92230;i++;
		List[i]=92231;i++;
		List[i]=94245;i++;
		List[i]=95240;i++;
		List[i]=99249;i++;
		List[i]=99250;i++;
		List[i]=-1;
	}
	bool Omit(int zaid){
		int i=0;
		while(i<10000){
			if(zaid==List[i])return true;
			i++;
			if(List[i]==-1)break;
		}
		return false;
	}
	void PrintOmitCard(int Material){
		int i=0;
		while(i<10000){
			i++;
			if(List[i]==-1)break;
		}
		cout<<"       Omit="<<Material<<","<<i<<","<<endl;
		i=0;
		cout<<"       "<<List[i];
		int j=1;
		i++;
		while(i<10000){
			if(j==4){cout<<endl<<"       ";j=0;}
			cout<<","<<List[i];
			j++;
			i++;
			if(List[i]==-1)return;
		}
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
		if(__This_KcodeReader_id!=id)if(KKR[0])delete KKR[0];

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
			if(KKR[0]->__file_exist){
				return 0;
			}
			cout<<"IMM::Case:: It looks like the last case in the run was: "<<this->GetCaseName(id,"")<<endl;
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
		string case1="Keff";
		string case2="FissionFrac";
		string case3="CaptureFrac";
		string case4="EscapeFrac";
		if(case1.find(FoM)<100)return 1;
		if(case2.find(FoM)<100)return 2;
		if(case3.find(FoM)<100)return 3;
		if(case4.find(FoM)<100)return 4;
		cout<<"ERROR::IMM::GetFoMindex:: MapFoM = \""<<FoM<<"\" is not a known FoM"<<endl;
		return 0;
	}
	char *GetFoMAxisName(int GetFoMindex){
		char *dummy=new char[100];
		sprintf(dummy,"\0");
		if(GetFoMindex==1)sprintf(dummy,"Keff\0");
		if(GetFoMindex==2)sprintf(dummy,"Fission fraction\0");
		if(GetFoMindex==3)sprintf(dummy,"Neutron capture fraction\0");
		if(GetFoMindex==4)sprintf(dummy,"Neutron escape fraction\0");
		return (char*)dummy;
	}
	double __GetFoM_error;
	double __GetFoM;
	double GetFoM(int FoMindex){
		if(FoMindex==1){  // Keff case
			__GetFoM=Case(__This_KcodeReader_id)->GetKeffFromStandartOutoutfile();
			__GetFoM_error=Case(__This_KcodeReader_id)->GetKeffFromStandartOutoutfile__Error;
		}
		if(FoMindex==2){  // FissionFrac case
			__GetFoM=Case(__This_KcodeReader_id)->GetFissionFromStandartOutoutfile();
			__GetFoM_error=0;
		}
		if(FoMindex==3){  // CaptureFrac case
			__GetFoM=Case(__This_KcodeReader_id)->GetCaptureFromStandartOutoutfile();
			__GetFoM_error=0;
		}
		if(FoMindex==4){  // EscapeFrac case
			__GetFoM=Case(__This_KcodeReader_id)->GetEscapeFromStandartOutoutfile();
			__GetFoM_error=0;
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
		Initialize();
		int itr=1;
		while(!Case(itr)){
			if(EndOfCases){
				cout<<"ERROR::IMM::Get2dMap:: Could not find a valid outp file to read from"<<endl;
				return 0;
			}
			itr++;
		}
		cout<<"Initilization done."<<endl;
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
			MapFoM=(char*)Case(itr)->GetLineClean(" MapFoM ",MapFoMindex);

			//			int dummyint=Case(itr)->FindLine(" MapFoM ",(double)MapFoMindex);
			//			if(Case(itr)->GetLineLength(MapFoMindex)>=dummyint+8)
			//				char *MapFoM = (char*)(Case(itr)->GetLine(MapFoMindex)+dummyint+8);
			if(!MapFoM||MapFoM[0]==0){cout<<"ERROR::IMM::Get2dMap:: You did not enter a MapFoM - try:  Get2dMap(\"Keff\");"<<endl; return 0;}
		} else if(MapFoMindex!=-1){
			char *dummychar=(char*)Case(itr)->GetLineClean(" MapFoM ",MapFoMindex);
			if(dummychar&&dummychar[0]!=1)
				cout<<"WARNING::IMM::Get2dMap:: You entered a FoM, but there is also a FoM in the file, using the entered FoM"<<endl;
		}
		int FoMindex=GetFoMindex(MapFoM);
		if(!FoMindex){cout<<"ERROR::IMM::Get2dMap:: Unknown FoM"<<endl; return 0;}
		// the following instruction-keywords must be written on an IMM line must be in the outp file
		int BinsXindex=this->GetPstudyInstructionLine(" BinsX "); // line must end on three numbers: nBins Min Max 
		int BinsYindex=this->GetPstudyInstructionLine(" BinsY "); // line must end on three numbers: nBins Min Max
		int ThisXindex=this->GetPstudyInstructionLine(" ThisX "); // line must end with current x value
		int ThisYindex=this->GetPstudyInstructionLine(" ThisY "); // line must end with current y value

		// optional IMM inputs:
		int HistTitleindex=this->GetPstudyInstructionLine(" HistTitle ");
		int XaxisNameindex=this->GetPstudyInstructionLine(" XaxisName ");
		int YaxisNameindex=this->GetPstudyInstructionLine(" YaxisName ");

		// Initilization is over...
		// testing validity:
		if(BinsXindex==-1){cout<<"ERROR::IMM::Get2dMap:: Did not find \" BinsX \" in case "<<__This_KcodeReader_id<<"'s outp"<<endl; return 0;}
		if(BinsYindex==-1){cout<<"ERROR::IMM::Get2dMap:: Did not find \" BinsY \" in case "<<__This_KcodeReader_id<<"'s outp"<<endl; return 0;}
		if(ThisXindex==-1){cout<<"ERROR::IMM::Get2dMap:: Did not find \" ThisX \" in case "<<__This_KcodeReader_id<<"'s outp"<<endl; return 0;}
		if(ThisYindex==-1){cout<<"ERROR::IMM::Get2dMap:: Did not find \" ThisY \" in case "<<__This_KcodeReader_id<<"'s outp"<<endl; return 0;}

		if(HistTitleindex==-1){cout<<"WARNING::IMM::Get2dMap:: Did not find \" HistTitle \" in case "<<__This_KcodeReader_id<<"'s outp"<<endl;}
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
			, nBinsX+1, xMin-(xMax-xMin)/nBinsX/2., xMax+(xMax-xMin)/nBinsX/2.
			, nBinsY+1, yMin-(yMax-yMin)/nBinsY/2., yMax+(yMax-yMin)/nBinsY/2.);

		// fetching names:
		char *theZaxisName=GetFoMAxisName(FoMindex);

		char *theXaxisName=0;
		if(XaxisNameindex!=-1){
			theXaxisName = (char*)Case(itr)->GetLineClean(" XaxisName ",XaxisNameindex);
		}
		char *theYaxisName=0;
		if(YaxisNameindex!=-1){
			theYaxisName = (char*)Case(itr)->GetLineClean(" YaxisName ",YaxisNameindex);
		}
		char *HistNameTitle=0;
		if(HistTitleindex!=-1){
			HistNameTitle = (char*)Case(itr)->GetLineClean(" HistTitle ",HistTitleindex);
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
		while(!EndOfCases){
			if(itr%100==0)cout<<"Now loading: "<<itr<<endl;
			if(Case(itr)==0){
				if(EndOfCases)break;
				itr++;
				continue;
			}
			if(MapTag){
				if(Case(itr)->FindLine(" MapTag ",MapTag,(double)MapTagindex)==-1){itr++;continue;}
			}
			//fetching x bin
			dummydouble=Case(itr)->Get_ListOfNumbers(ThisXindex);
			this_x=dummydouble[Case(itr)->Get_ListOfNumbers__arraylength-1];
			//fetching y bin
			dummydouble=Case(itr)->Get_ListOfNumbers(ThisYindex);
			this_y=dummydouble[Case(itr)->Get_ListOfNumbers__arraylength-1];

			// finding bin:
			binx=returnhist->GetXaxis()->FindBin(this_x);
			biny=returnhist->GetYaxis()->FindBin(this_y);

			// filling histogram:
			if(returnhist->GetBinContent(binx,biny)){cout<<"Error::IMM::Get2dMap: x,y-bin dublicate! Returning Histogram prematurely..."<<endl;return returnhist;}
			returnhist->SetBinContent(binx,biny,GetFoM(FoMindex));
			returnhist->SetBinError(binx,biny,__GetFoM_error);
			itr++;
		}
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
			keff_err=Case(itr)->GetKeffFromStandartOutoutfile__Error;
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
	~Logbook(){

	}
	void WriteLine(char *input, int line, bool doprint=false){
		if(line>=10000){cout<<"ERROR::Logbook::WriteLine:: Too many lines in Logbook"<<endl;return ;}
		Line[line]=input;
		if(doprint)cout<<" logbook: "<<Line[line]<<endl;
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


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

class ZAID{
private:
	int _zaid;
	double _temperature;
	char _tmp_tmptag[6];
	char _tmptag[5];
	char _ZAIDtag[7];
	char _ZAIDcard[11];

protected:
	void Initialize(double inzaid){
		_zaid=inzaid/1;
		if(GetZ()>100){cout<<"WARNING::ZAID::Initialize:: zaid is very high Z>=100"<<endl;}
		if(!((int)(inzaid*100)%10))_temperature=2.5301E-08;
		else _temperature=(((int)(inzaid*100)%10)+1)*2.5852e-008;
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
	//Avaliable variables:

	//Constructors
	ZAID(ZAID *inZAID){
		ZAID(inZAID->GetZAID(),inZAID->GetTemp());
	}
	ZAID(char *zaidcard){
		if(TSC::isNumberFromChar(zaidcard)<0){cout<<"ERROR::isotope_ZAID::isotope_ZAID:: could not read zaidcard"<<endl;return;}
		Initialize(TSC::GetNumberFromChar(zaidcard));
	}
	ZAID(int inzaid, double intemperature=0.){
		_temperature=intemperature;
		if(intemperature>1){
			cout<<"WARNING::isotope_ZAID::isotope_ZAID M Temperature converted from Kelvin ("<<intemperature<<" K) to energy ("<<T2E(intemperature)<<" MeV) "<<endl;
			_temperature=T2E(intemperature);
		}
		Initialize(inzaid+tmpkernelindendifier(_temperature));
	}
	ZAID(int inz,int ina, double intemperature=0.){
		ZAID(inz*1000+ina,intemperature);
	}

	//functions:
	void SetTemp(double intemp){
		_temperature=intemp;
	}
	void SetZAID(int inZAID){
		_zaid=inZAID;
	}
	int GetZAID(){
		return _zaid;
	}
	double GetTemp(){
		return _temperature;
	}
	int GetZ(){
		return (int)_zaid/1000;
	}
	int GetA(){
		return (int)_zaid%1000;
	}
	int GetN(){
		return (int)(GetA()-GetZ());
	}
	char *GetZAIDcard(){
		if(GetTempTag()!=0){
			sprintf(_ZAIDcard,"%s%s\0",GetZAIDtag(),GetTempTag());
		} else {
			sprintf(_ZAIDcard,"%s\0",GetZAIDtag());
		}
		return _ZAIDcard;
	}
	char *GetZAIDtag(){
		if(GetA()<10)sprintf(_ZAIDtag,"%d00%d\0",GetZ(),GetA());
		else if(GetA()<100)sprintf(_ZAIDtag,"%d0%d\0",GetZ(),GetA());
		else sprintf(_ZAIDtag,"%d%d\0",GetZ(),GetA());
		return _ZAIDtag;
	}
	char *GetTempTag(){
		if(_temperature==0)_tmp_tmptag[1]=0;
		else sprintf(_tmp_tmptag,"%1.2fC\0",tmpkernelindendifier(_temperature));
		for(int i=0;i<5;i++)_tmptag[i]=_tmp_tmptag[i+1];
		return _tmptag;
	}
};

class Isotope{
private :
	//basics:
	ZAID *_zaid;
	double _massfrac;
	double _atomfrac;
	double _atomdensity;
	double _spec_activity;
	// advanced:
//	 no.   zaid     (n,gamma)    error   (n,fission)    error (n,nu*fission)   error (n,fission*Q)    error
	double _n_gamma;
	double _n_gamma_err;
	double _fission;
	double _fission_err;
	double _nufission;
	double _nufission_err;
	double _fissionQ;
	double _fissionQ_err;
//	 no.   zaid        (n,2n)    error        (n,3n)    error     (n,alpha)    error         (n,p)    error
	double _n_2n;
	double _n_2n_err;
	double _n_3n;
	double _n_3n_err;
	double _n_alpha;
	double _n_alpha_err;
	double _n_p;
	double _n_p_err;
protected:
public :
	Isotope(int inzaid, double inatomfrac, double inmassfrac=0, double inatomdensity=0, double inspec_activity=0, double temperature=0){
		_zaid=new ZAID(inzaid,temperature);
		_atomfrac=inatomfrac;
		_massfrac=inmassfrac;
		_atomdensity=inatomdensity;
		_spec_activity=inspec_activity;
	}
	Isotope(string burnupmaterialline, double temperature=0){
		stringstream stream(burnupmaterialline);
		double toomany_numbers=-1;
		_massfrac=-1;
		int zaiddummy;
		double dummy;
//		no. zaid     mass      activity   sp. act.  atom den.   atom fr.   mass fr.
		stream >> dummy >> zaiddummy >> dummy >> dummy >> _spec_activity >> _atomdensity >> _atomfrac >> _massfrac >> toomany_numbers;
		if(toomany_numbers!=-1||_massfrac==-1)cout<<"WARNING::Isotope::Isotope:: string format incorrect!!"<<endl;
		_zaid=new ZAID(zaiddummy,temperature);
	}
	~Isotope(){
		if(_zaid)delete _zaid;
	}

	// add information
	void SetNeutronics_ngammaLine(double n_gamma, double n_gamma_err, double fission, double fission_err, double nufission, double nufission_err, double fissionQ, double fissionQ_err){
		_n_gamma=n_gamma;
		_n_gamma_err=n_gamma_err;
		_fission=fission;
		_fission_err=fission_err;
		_nufission=nufission;
		_nufission_err=nufission_err;
		_fissionQ=fissionQ;
		_fissionQ_err=fissionQ_err;
	}
	void SetNeutronics_ngammaLine(string Neutronics_ngammaLine){
		stringstream stream1(Neutronics_ngammaLine);
		double toomany_numbers=-1;
		int zaiddummy;
		double dummy;
		_fissionQ_err=-1;
		//	 no.   zaid     (n,gamma)    error   (n,fission)    error (n,nu*fission)   error (n,fission*Q)    error
		stream1 >> dummy >> zaiddummy >> _n_gamma >> _n_gamma_err >> _fission >> _fission_err >> _nufission >> _nufission_err >> _fissionQ >> _fissionQ_err >> toomany_numbers;
		if(toomany_numbers!=-1||_fissionQ_err==-1)cout<<"WARNING::Isotope::SetNeutronics:: Neutronics_ngammaLine format incorrect!"<<endl;
		if(_zaid->GetZAID()!=zaiddummy)cout<<"WARNING::Isotope::SetNeutronics:: Wrong zaid detected 1"<<endl;
	}
	void SetNeutronics_n2nLine(double n_2n, double n_2n_err, double n_3n, double n_3n_err, double n_alpha, double n_alpha_err, double n_p, double n_p_err){
		_n_2n=n_2n;
		_n_2n_err=n_2n_err;
		_n_3n=n_3n;
		_n_3n_err=n_3n_err;
		_n_alpha=n_alpha;
		_n_alpha_err=n_alpha_err;
		_n_p=n_p;
		_n_p_err=n_p_err;
	}
	void SetNeutronics_n2nLine(string Neutronics_n2nLine){
		double toomany_numbers=-1;
		int zaiddummy;
		double dummy;
		_n_p_err=-1;
		stringstream stream2(Neutronics_n2nLine);
		//	 no.   zaid        (n,2n)    error        (n,3n)    error     (n,alpha)    error         (n,p)    error
		stream2 >> dummy >> zaiddummy >> _n_2n >> _n_2n_err >> _n_3n >> _n_3n_err >> _n_alpha >> _n_alpha_err >> _n_p >> _n_p_err >> toomany_numbers;
		if(toomany_numbers!=-1||_n_p_err==-1)cout<<"WARNING::Isotope::SetNeutronics:: Neutronics_n2nLine format incorrect!"<<endl;
		if(_zaid->GetZAID()!=zaiddummy)cout<<"WARNING::Isotope::SetNeutronics:: Wrong zaid detected 2"<<endl;
	}
	
	void SetNeutronics(string Neutronics_ngammaLine,string Neutronics_n2nLine){
		SetNeutronics_ngammaLine(Neutronics_ngammaLine);
		SetNeutronics_n2nLine(Neutronics_n2nLine);
	}
	// functions:
	void ScaleFraction(double scale){

		_massfrac*=scale;
		_atomfrac*=scale;
		_atomdensity*=scale;
		_spec_activity*=scale;
		_n_gamma*=scale;
		_n_gamma_err*=scale;
		_fission*=scale;
		_fission_err*=scale;
		_nufission*=scale;
		_nufission_err*=scale;
		_fissionQ*=scale;
		_fissionQ_err*=scale;
		_n_2n*=scale;
		_n_2n_err*=scale;
		_n_3n*=scale;
		_n_3n_err*=scale;
		_n_alpha*=scale;
		_n_alpha_err*=scale;
		_n_p*=scale;
		_n_p_err*=scale;
	}
	// get Information:
	ZAID *GetZAIDPtr(){
		return (ZAID*)_zaid;
	}
	int GetZAID(){
		return _zaid->GetZAID();
	}
	int GetTemp(){
		return _zaid->GetTemp();
	}
	int GetZ(){
		return _zaid->GetZ();
	}
	int GetA(){
		return _zaid->GetA();
	}
	int GetN(){
		return _zaid->GetN();
	}
	double GetMassfrac(){
		return _massfrac;
	}
	void SetMassfrac(double newFrac){
		_massfrac=newFrac;
	}
	double GetAtomfrac(){
		return _atomfrac;
	}
	double GetAtomdensity(){
		return _atomdensity;
	}
	double GetSpecActivity(){
		return _spec_activity;
	}

	double GetNGamma(){
		return _n_gamma;
	}
	double GetNGamma_err(){
		return _n_gamma_err;
	}
	double GetFission(){
		return _fission;
	}
	double GetFission_err(){
		return _fission_err;
	}
	double GetNuFission(){
		return _nufission;
	}
	double GetNuFission_err(){
		return _nufission_err;
	}
	double GetFissionQ(){
		return _fissionQ;
	}
	double GetFissionQ_err(){
		return _fissionQ_err;
	}
	double GetN2n(){
		return _n_2n;
	}
	double GetN2n_err(){
		return _n_2n_err;
	}
	double GetN3n(){
		return _n_3n;
	}
	double GetN3n_err(){
		return _n_3n_err;
	}
	double GetNAlpha(){
		return _n_alpha;
	}
	double GetNAlpha_err(){
		return _n_alpha;

	}
	double GetNP(){
		return _n_p;
	}
	double GetNP_err(){
		return _n_p_err;
	}
	double GetNeutronsPerNeutron(){
		double absorbtions=GetNeutronsAbsorbed();
		if(absorbtions==0)return 0;
		return GetNeutronsCreated()/(absorbtions);
	}
	double GetNeutronEconomy(){
		return GetNeutronsCreated()-GetNeutronsAbsorbed();
	}
	double GetNeutronsAbsorbed(){
		return GetNGamma()+GetNAlpha()+GetNP()+GetFission()+GetN2n()+GetN3n();
	}
	double GetNeutronsCapture(){
		return GetNGamma()+GetNAlpha()+GetNP();
	}
	double GetNeutronsCreated(){
		return GetNuFission()+GetN2n()*2+GetN3n()*3;
	}

};

class Material{
private :
	OmitList *Omit;
	double _mat_id;
	double _mass;
	double _volume;
	double _temperature;
	
protected:
	Isotope *_iso[110][20];
	void Initilize(){
		for(int i=0;i<110;i++)
			for(int j=0;j<20;j++)
				_iso[i][j]=0;
		tmp_iso=0;
	}
	int GetIsotopeId(int zaid){
		if(zaid/1000<110)
		for(int i=0;i<20;i++){
			if(_iso[zaid/1000][i]->GetZAID()==zaid){return i;}
		}
		return -1;
	}
	int GetNextEmptyIsotopeId(int zaid){
		if(zaid/1000<110)
		for(int i=0;i<20;i++){
			if(!_iso[zaid/1000][i])return i;
			if(_iso[zaid/1000][i]->GetZAID()==zaid){cout<<"ERROR::Material::GetNextIsotopeId:: zaid="<<zaid<<" already exists"<<endl;return -1;}
		}
		cout<<"ERROR::Material::GetNextIsotopeId:: Too many isotopes (21) with z="<<zaid/1000<<endl;
		return -1;
	}
public :
	Isotope *tmp_iso;
	Material(int inmat_id=0, double inmass=0, double intemperature=0,double involume=0){
		Initilize();
		_mat_id=inmat_id;
		_mass=inmass;
		_temperature=intemperature;
		_volume=involume;
		tmp_iso=0;
		Omit=new OmitList();
	}
	~Material(){
		for(int i=0;i<110;i++)
			for(int j=0;j<20;j++)
				if(_iso[i][j])delete _iso[i][j];
	}
	// load directly from a a standart Burnup output file information:
	void SetMass(double NewMass){
		_mass=NewMass;
	}
	int LoadFromBurnupOutput(KcodeReader *outp, int step, bool noNonActinides=false, bool doprintstatus=true){ // if the defined material in this class is 0 then "sum af materials" is loaded, and no neutronics
		char Material_indendifier[30];
		char Material_indendifier2[30];
		char Step_indendifier[30];
		bool doskipneutronics=false;
		int zaiddummy;
		double dummy;
		stringstream stream;
		if(_mat_id){
			sprintf(Material_indendifier,"material\0");
			if(_mat_id<1000)sprintf(Material_indendifier,"%s \0",Material_indendifier);
			if(_mat_id<100)sprintf(Material_indendifier,"%s \0",Material_indendifier);
			if(_mat_id<10)sprintf(Material_indendifier,"%s \0",Material_indendifier);
			sprintf(Material_indendifier,"%s%d\0",Material_indendifier,_mat_id);
		} else {
			sprintf(Material_indendifier,"sum of materials\0");
		}
		if(_mat_id){
			sprintf(Material_indendifier2,"Material #: \0");
			if(_mat_id<1000)sprintf(Material_indendifier2,"%s \0",Material_indendifier2);
			if(_mat_id<100)sprintf(Material_indendifier2,"%s \0",Material_indendifier2);
			if(_mat_id<10)sprintf(Material_indendifier2,"%s \0",Material_indendifier2);
			sprintf(Material_indendifier2,"%s%d\0",Material_indendifier2,_mat_id);
		}
		if(step<10)sprintf(Step_indendifier,"step  %d\0",step); // two spaces
		else if(step<100)sprintf(Step_indendifier,"step %d\0",step);
		else sprintf(Step_indendifier,"step%d\0",step);
		if(doprintstatus)cout<<"Preparing to load "<<Material_indendifier<<" for step "<<step<<"..."<<endl;
		// fetching volumn:
		int volumeline=-1;
		if(_mat_id)volumeline=outp->FindLine(" nuclide data ",Material_indendifier,-outp->LastLineIndex+1);
		else volumeline=outp->FindLine(" nuclide data","summed over all materials",-outp->LastLineIndex+1);
		_volume=0;
		if(volumeline==-1){
			cout<<"WARNING::Material::LoadFromBurnupOutput:: could not find volumn related to material - volume will be 0"<<endl;
		} else {
			double *dummydoubleptr=outp->Get_ListOfNumbers(volumeline);
			if(outp->Get_ListOfNumbers__arraylength>2)_volume=dummydoubleptr[outp->Get_ListOfNumbers__arraylength-2];
		}
		
		// finding the nonactinides:
		if(doprintstatus)cout<<"Listing file pointers..."<<endl;
		int FirstNonActinideMaterialLine=0;
		if(!noNonActinides){
			FirstNonActinideMaterialLine=outp->FindLine(" nonactinide ",Material_indendifier,-outp->LastLineIndex+1);
			FirstNonActinideMaterialLine=outp->FindLine(Material_indendifier,Step_indendifier,-FirstNonActinideMaterialLine);
			if(FirstNonActinideMaterialLine<1){cout<<"ERROR::Material::LoadFromBurnupOutput:: could not find step or material (1)"<<endl;return 0;}
			if(outp->FindLine(" nonactinide ",(double)FirstNonActinideMaterialLine)==-1){cout<<"ERROR::Material::LoadFromBurnupOutput:: could not find step or material (2)"<<endl;return 0;}
			FirstNonActinideMaterialLine+=4;
		}

		// finding the actinides (continued search):
		int FirstActinideMaterialLine=0;
		FirstActinideMaterialLine=outp->FindLine(" actinide ",Material_indendifier,-outp->LastLineIndex+1);
		FirstActinideMaterialLine=outp->FindLine(Material_indendifier,Step_indendifier,-FirstActinideMaterialLine);
		if(FirstActinideMaterialLine<1){cout<<"ERROR::Material::LoadFromBurnupOutput:: could not find step or material (3)"<<endl;return 0;}
		if(outp->FindLine(" actinide ",(double)FirstActinideMaterialLine)==-1){cout<<"ERROR::Material::LoadFromBurnupOutput:: could not find step or material (4)"<<endl;return 0;}
		FirstActinideMaterialLine+=4;

		// looking for decayline - which must not b there!
		int Nextdecayonlyline=1;
		Nextdecayonlyline=outp->FindLine(" Decay only calc ",Nextdecayonlyline+1);
		if(Nextdecayonlyline!=-1){
			cout<<"decay only step found - will skip all neutronics data."<<endl;
			doskipneutronics=true;
		}
		
		int FirstngammaLine=0;
		int Firstn2nLine=0;
		if(_mat_id&&!doskipneutronics){
			// Looping forward through (n,gamma) lines:
			for(int i=0;i<2*step+1;i++){
				while(true){
					FirstngammaLine=outp->FindLine(Material_indendifier2,FirstngammaLine+1);
					if(outp->FindLine("(n,gamma)",(double)(FirstngammaLine+2)))break;
					if(FirstngammaLine<1){cout<<"ERROR::Material::LoadFromBurnupOutput:: could not find (n,gamma) line for step or material (1)"<<endl;return 0;}
				}
				if(FirstngammaLine<1){cout<<"ERROR::Material::LoadFromBurnupOutput:: could not find (n,gamma) line for step or material (2)"<<endl;return 0;}
			}
			FirstngammaLine+=3;
			// n2n line (continued search):
			Firstn2nLine=outp->FindLine("(n,2n)",FirstngammaLine+1);
			
			if(Firstn2nLine<FirstngammaLine||Firstn2nLine>FirstngammaLine+2500){cout<<"ERROR::Material::LoadFromBurnupOutput:: could not find (n,2n) line for step or material"<<endl;return 0;}
			Firstn2nLine++;
		}
		
		// --------------------------------------
		// start loading the isotopes:
		// --------------------------------------
		// if noactinides then skipping through nonactinides to actinides
		if(noNonActinides&&!doskipneutronics){
			while(true){
				stream.str(outp->GetLine(FirstngammaLine));
				stream >> dummy >> zaiddummy ;
				if(109<zaiddummy/1000||zaiddummy/1000<1){cout<<"ERROR::Material::LoadFromBurnupOutput:: Strange isotope (1) Zaid="<<zaiddummy<<endl;return 0;}
				if(zaiddummy/1000>89)break;
				FirstngammaLine++;
				Firstn2nLine++;
			}
		}
		// finding last non-actinide:
		int enditr1=0;
		if(!noNonActinides)enditr1=outp->FindLine(FirstNonActinideMaterialLine)-FirstNonActinideMaterialLine-1;
		// finding last actinide:
		int enditr2=outp->FindLine(FirstActinideMaterialLine)-FirstActinideMaterialLine-1; // not loading total line (-1)
		// load non actinides
		if(!noNonActinides)if(doprintstatus)cout<<"Loading non-actinides..."<<endl;
		for(int itr=0;itr<enditr1;itr++){
			this->NewIsotope(outp,FirstNonActinideMaterialLine);
			FirstNonActinideMaterialLine++;
		}
		// load actinides
		if(doprintstatus)cout<<"Loading actinides..."<<endl;
		for(int itr=0;itr<enditr2;itr++){
			this->NewIsotope(outp,FirstActinideMaterialLine);
			FirstActinideMaterialLine++;
		}
		if(!noNonActinides){
			double *dumdub=outp->Get_ListOfNumbers(FirstActinideMaterialLine);
			this->_mass=dumdub[0];
			dumdub=outp->Get_ListOfNumbers(FirstNonActinideMaterialLine);
			this->_mass+=dumdub[0];
		} else {
			double *dumdub=outp->Get_ListOfNumbers(FirstActinideMaterialLine);
			this->_mass=dumdub[0];
		}

		if(doskipneutronics)return 1;
		// load neutronics
		if(doprintstatus)cout<<"Loading neutronics..."<<endl;
		enditr1=outp->FindLine(FirstngammaLine)-FirstngammaLine;
		for(int itr=0;itr<enditr1;itr++){
			double *zaid=outp->Get_ListOfNumbers(FirstngammaLine);
			int zaiddummy=-1;
			if(outp->Get_ListOfNumbers__arraylength>2)
				zaiddummy=zaid[1];
			if(!this->GetIsotopePtr(zaiddummy)){
				cout<<"WARNING::Material::LoadFromBurnupOutput:: zaid="<<zaiddummy<<" was not listed in m"<<_mat_id<<" skipping neutronics"<<endl;
				FirstngammaLine++;
				Firstn2nLine++;
				continue;
				//this->NewIsotope(zaiddummy,0,0,0,0);
			}

			this->AddNeutronics(outp,FirstngammaLine,Firstn2nLine);
			FirstngammaLine++;
			Firstn2nLine++;
		}
		return 1;
	}


	// add stuff;
	int NewIsotope(KcodeReader *outp,int burnupmateriallineindex){
		double *res=outp->Get_ListOfNumbers(burnupmateriallineindex);
		if(outp->Get_ListOfNumbers__arraylength!=8)cout<<"ERROR::Material::NewIsotope:: Could not read line! (wrong number of numbers)"<<endl;

//		no. zaid     mass      activity   sp. act.  atom den.   atom fr.   mass fr.
		int zaiddummy=res[1];
		this->_mass+=res[2];
		if(109<zaiddummy/1000||zaiddummy/1000<1){cout<<"ERROR::Material::NewIsotope:: Strange isotope (1) zaid="<<zaiddummy<<endl;return 0;}
		int iddummy=GetNextEmptyIsotopeId(res[1]);
		if(iddummy==-1){cout<<"ERROR::Material::AddIsotope:: could not add a new isotope"<<endl;return 0;}
		_iso[zaiddummy/1000][iddummy]=new Isotope(res[1],res[6],res[7],res[5],res[4],_temperature);

	}
	int NewIsotope(string burnupmaterialline){
		stringstream stream(burnupmaterialline);
		int zaiddummy;
		double dummy;
		double massdummy;		
//		no. zaid     mass      activity   sp. act.  atom den.   atom fr.   mass fr.
		stream >> dummy >> zaiddummy >> massdummy;
		this->_mass+=massdummy;
		if(109<zaiddummy/1000||zaiddummy/1000<1){cout<<"ERROR::Material::NewIsotope:: Strange isotope (1) zaid="<<zaiddummy<<endl;return 0;}
		int iddummy=GetNextEmptyIsotopeId(zaiddummy);
		if(iddummy==-1){cout<<"ERROR::Material::AddIsotope:: could not add a new isotope"<<endl;return 0;}
		_iso[zaiddummy/1000][iddummy]=new Isotope((string)burnupmaterialline,_temperature);
		return 1;
	}
	int NewIsotope(int zaid,double atomfrac,double massfrac=0, double atomdensity=0, double spec_activity=0){
		if(109<zaid/1000||zaid/1000<1){cout<<"ERROR::Material::NewIsotope:: Strange isotope (2)"<<endl;return 0;}
		int iddummy=GetNextEmptyIsotopeId(zaid);
		if(iddummy==-1){cout<<"ERROR::Material::AddIsotope:: could not add a new isotope"<<endl;return 0;}
		_iso[zaid/1000][iddummy]=new Isotope(zaid,atomfrac,massfrac,atomdensity,spec_activity,_temperature);
		return 1;
	}
	int AddNeutronics(string Neutronics_ngammaLine,string Neutronics_n2nLine){
		stringstream stream(Neutronics_ngammaLine);
		int zaiddummy;
		double dummy;
		stream >> dummy >> zaiddummy ;
		if(109<zaiddummy/1000||zaiddummy/1000<1){cout<<"ERROR::Material::AddNeutronics:: Strange isotope"<<endl;return 0;}
		if(!GetIsotopePtr(zaiddummy)){cout<<"ERROR::Material::AddNeutronics:: Isotope zaid="<<zaiddummy<<" not defined"<<endl;return 0;}
		GetIsotopePtr(zaiddummy)->SetNeutronics((string)Neutronics_ngammaLine,(string)Neutronics_n2nLine);
		return 1;
	}
	int AddNeutronics(KcodeReader *outp,int Neutronics_ngammaLineIndex,int Neutronics_n2nLineIndex){
		double *res=outp->Get_ListOfNumbers(Neutronics_ngammaLineIndex);
		if(outp->Get_ListOfNumbers__arraylength!=10)cout<<"ERROR::Material::AddNeutronics:: Could not read line (1)! (wrong number of numbers)"<<endl;
		//	 no.   zaid        (n,2n)    error        (n,3n)    error     (n,alpha)    error         (n,p)    error
		int zaiddummy=res[1];
		if(109<zaiddummy/1000||zaiddummy/1000<1){cout<<"ERROR::Material::AddNeutronics:: very strange isotope zaid="<<zaiddummy<<endl;return 0;}
		GetIsotopePtr(zaiddummy)->SetNeutronics_ngammaLine(res[2],res[3],res[4],res[5],res[6],res[7],res[8],res[9]);
		res=outp->Get_ListOfNumbers(Neutronics_n2nLineIndex);
		if(outp->Get_ListOfNumbers__arraylength!=10){cout<<"ERROR::Material::AddNeutronics:: Could not read line (1)! (wrong number of numbers)"<<endl;return 0;}
		if(res[1]*1!=zaiddummy){cout<<"ERROR::Material::AddNeutronics:: Zaids are not identical: "<<res[1]<<" and "<<zaiddummy<<endl;return 0;}
		GetIsotopePtr(zaiddummy)->SetNeutronics_n2nLine(res[2],res[3],res[4],res[5],res[6],res[7],res[8],res[9]);
	}

	// get stuff:
	Isotope *GetIsotopePtr(int zaid){
		if(zaid/1000<0){
			if(zaid/1000==0){
				cout<<"WARNING::Material::GetIsotope:: You are looking for isotope with z=0. Which is absurd - please stop that! (1)"<<endl;
			} else {
				cout<<"ERROR::Material::GetIsotope:: Mega ERROR detected! (1)"<<endl;
				return 0;
			}
		}		// in the case that this isotope was just loaded:
		if(tmp_iso)
			if(tmp_iso->GetZAID()==zaid)
				return tmp_iso;
		// looking for the isotope amongst already defined isotopes:
		if(zaid/1000<110)
		for(int i=0;i<20;i++){
			// in the case that it is not found:
			if(!_iso[zaid/1000][i])return 0;
			// in the case that it is already there:
			if(_iso[zaid/1000][i]->GetZAID()==zaid){tmp_iso=_iso[zaid/1000][i];return tmp_iso;}
		}
		cout<<"ERROR::Material::GetIsotope:: Too many isotopes (21) with z="<<zaid/1000<<endl;
		tmp_iso=0;
		return 0;
	}
	Isotope *GetIsotopePtr(int z, int itr){
		if(z<0||itr<0){
			if(z==0){
				cout<<"WARNING::Material::GetIsotope:: You are looking for isotope with z=0. Which is absurd - please stop that! (2)"<<endl;
			} else {
				cout<<"ERROR::Material::GetIsotope:: Mega ERROR detected! (2)"<<endl;
				return 0;
			}
		}
		if(z<110&&itr<20)if(_iso[z][itr]){
			tmp_iso=_iso[z][itr];
			return tmp_iso;
		}
		tmp_iso=0;
		return 0;
	}
	double GetMat_id(){return _mat_id;}
	double GetMass(){return _mass;}
	double GetVolume(){return _volume;}
	double GetTemp(){return _temperature;}

	// functions:
	double GetMassIsotope(int zaid){
		if(GetIsotopePtr(zaid))
			return _mass*GetIsotopePtr(zaid)->GetMassfrac()/this->GetMassFracAllIsotopes();
		return 0;
	}
	double GetMassElement(int Z){
		double sumdummy=0;
		for(int i=0;i<20;i++){
			if(this->GetIsotopePtr(Z,i))sumdummy+=GetMassIsotope(this->GetIsotopePtr(Z,i)->GetZAID());
		}
		return sumdummy;
	}
	double GetMassAllIsotopes(int low=1, int high=109){
		double sumdummy=0;
		for(int i=low;i<high+1;i++){
			sumdummy+=GetMassElement(i);
		}
		return sumdummy;
	}
	double GetNeutronEconomyIsotope(int zaid){
		if(GetIsotopePtr(zaid))
			return GetIsotopePtr(zaid)->GetNeutronEconomy();
		return 0;
	}
	double GetNeutronEconomyElement(int Z){
		double sumdummy=0;
		for(int i=0;i<20;i++){
			if(this->GetIsotopePtr(Z,i))sumdummy+=GetNeutronEconomyIsotope(this->GetIsotopePtr(Z,i)->GetZAID());
		}
		return sumdummy;
	}
	double GetNeutronEconomyAllIsotopes(int low=1, int high=109){
		double sumdummy=0;
		for(int i=low;i<high+1;i++){
			sumdummy+=GetNeutronEconomyElement(i);
		}
		return sumdummy;
	}
	double GetFissionQIsotope(int zaid){
		if(GetIsotopePtr(zaid))
			return GetIsotopePtr(zaid)->GetFissionQ();
		return 0;
	}
	double GeFissionQElement(int Z){
		double sumdummy=0;
		for(int i=0;i<20;i++){
			if(this->GetIsotopePtr(Z,i))sumdummy+=GetFissionQIsotope(this->GetIsotopePtr(Z,i)->GetZAID());
		}
		return sumdummy;
	}
	double GetFissionQAllIsotopes(int low=1, int high=109){
		double sumdummy=0;
		for(int i=low;i<high+1;i++){
			sumdummy+=GeFissionQElement(i);
		}
		return sumdummy;
	}
	double GetMassFracIsotope(int zaid){
		if(GetIsotopePtr(zaid))
			return GetIsotopePtr(zaid)->GetMassfrac();
		return 0;
	}
	double GetMassFracElement(int Z){
		double sumdummy=0;
		for(int i=0;i<20;i++){
			if(this->GetIsotopePtr(Z,i))sumdummy+=GetMassFracIsotope(this->GetIsotopePtr(Z,i)->GetZAID());
		}
		return sumdummy;
	}
	double GetMassFracAllIsotopes(int low=1, int high=109){
		double sumdummy=0;
		for(int i=low;i<high+1;i++){
			sumdummy+=GetMassFracElement(i);
		}
		return sumdummy;
	}
	double GetNeutronsAbsorbedIsotope(int zaid){
		if(GetIsotopePtr(zaid))
			return GetIsotopePtr(zaid)->GetNeutronsAbsorbed();
		return 0;
	}
	double GetNeutronsAbsorbedElement(int Z){
		double sumdummy=0;
		for(int i=0;i<20;i++){
			if(this->GetIsotopePtr(Z,i))sumdummy+=GetNeutronsAbsorbedIsotope(this->GetIsotopePtr(Z,i)->GetZAID());
		}
		return sumdummy;
	}
	double GetNeutronsAbsorbedAll(int low=1, int high=109){
		double sumdummy=0;
		for(int i=low;i<high+1;i++){
			sumdummy+=GetNeutronsAbsorbedElement(i);
		}
		return sumdummy;
	}
	
	double GetNeutronsCaptureIsotope(int zaid){
		if(GetIsotopePtr(zaid))
			return GetIsotopePtr(zaid)->GetNeutronsCapture();
		return 0;
	}
	double GetNeutronsCaptureElement(int Z){
		double sumdummy=0;
		for(int i=0;i<20;i++){
			if(this->GetIsotopePtr(Z,i))sumdummy+=GetNeutronsCaptureIsotope(this->GetIsotopePtr(Z,i)->GetZAID());
		}
		return sumdummy;
	}
	double GetNeutronsCaptureAll(int low=1, int high=109){
		double sumdummy=0;
		for(int i=low;i<high+1;i++){
			sumdummy+=GetNeutronsCaptureElement(i);
		}
		return sumdummy;
	}
	double GetNeutronsCreatedIsotope(int zaid){
		if(GetIsotopePtr(zaid))
			return GetIsotopePtr(zaid)->GetNeutronsCreated();
		return 0;
	}
	double GetNeutronsCreatedElement(int Z){
		double sumdummy=0;
		for(int i=0;i<20;i++){
			if(this->GetIsotopePtr(Z,i))sumdummy+=GetNeutronsCreatedIsotope(this->GetIsotopePtr(Z,i)->GetZAID());
		}
		return sumdummy;
	}
	double GetNeutronsCreatedAll(int low=1, int high=109){
		double sumdummy=0;
		for(int i=low;i<high+1;i++){
			sumdummy+=GetNeutronsCreatedElement(i);
		}
		return sumdummy;
	}
	double GetNeutronsPerNeutronIsotope(int zaid){
		if(GetIsotopePtr(zaid))
			return GetIsotopePtr(zaid)->GetNeutronsPerNeutron();
		return 0;
	}
	double GetNeutronsPerNeutronElement(int Z){
		return GetNeutronsCreatedElement(Z)/GetNeutronsAbsorbedElement(Z);
	}
	double GetNeutronsPerNeutronAll(int low=1, int high=109){
		return GetNeutronsCreatedAll(low,high)/GetNeutronsAbsorbedAll(low,high);
	}
	double GetAtomFracIsotope(int zaid){
		if(GetIsotopePtr(zaid))
			return GetIsotopePtr(zaid)->GetAtomfrac();
		return 0;
	}
	double GetAtomFracElement(int Z){
		double sumdummy=0;
		for(int i=0;i<20;i++){
			if(this->GetIsotopePtr(Z,i))sumdummy+=GetAtomFracIsotope(this->GetIsotopePtr(Z,i)->GetZAID());
		}
		return sumdummy;
	}
	double GetAtomFracAllIsotopes(int low=1, int high=109){
		double sumdummy=0;
		for(int i=low;i<high+1;i++){
			sumdummy+=GetAtomFracElement(i);
		}
		return sumdummy;
	}
	void ScaleIsotope(int zaid, double scale){
		if(GetIsotopePtr(zaid))GetIsotopePtr(zaid)->ScaleFraction(scale);
	}
	void ScaleElement(int Z, double scale){
		for(int i=0;i<20;i++){
			if(GetIsotopePtr(Z,i))ScaleIsotope(this->GetIsotopePtr(Z,i)->GetZAID(),scale);
		}
	}
	void ScaleAllIsotopes(double scale,int low=1, int high=109){
		for(int i=low;i<high+1;i++){
			ScaleElement(i,scale);
		}
	}
	void NormalizeMaterialFractions(double TotalFraction=1.){
		ScaleAllIsotopes(TotalFraction/GetAtomFracAllIsotopes());
	}
	void PrintMaterial(){
		OmitList O;
		cout<<"m"<<(int)_mat_id<<"   ";
		for(int Z=1;Z<110;Z++){
			for(int i=0;i<20;i++){
				if(GetIsotopePtr(Z,i)){
					if(GetIsotopePtr(Z,i)->GetAtomfrac()!=0){
						if(O.Omit(GetIsotopePtr(Z,i)->GetZAID()))cout<<"c";
						cout<<"             "<<GetIsotopePtr(Z,i)->GetZAIDPtr()->GetZAIDcard()<<" "<<GetIsotopePtr(Z,i)->GetAtomfrac()<<endl;
					}
				}
			}
		}
	}
	void PrintMaterial(Logbook *L,bool doprintstatus=false){
		char dummy[100];
		sprintf(dummy,"m%d     \0",_mat_id);L->WriteLine(dummy,doprintstatus);
		for(int Z=1;Z<110;Z++){
			for(int i=0;i<20;i++){
				if(GetIsotopePtr(Z,i)){
					if(GetIsotopePtr(Z,i)->GetAtomfrac()!=0){
						sprintf(dummy,"\0");
						if(Omit->Omit(GetIsotopePtr(Z,i)->GetZAID())||GetIsotopePtr(Z,i)->GetMassfrac()<0){
							sprintf(dummy,"c\0",_mat_id);
							if(GetIsotopePtr(Z,i)->GetMassfrac()<0)cout<<"WARNING::Material::PrintMaterial:: MassFrac<0 for "<<GetIsotopePtr(Z,i)->GetZAID()<<" - omitting"<<endl;
						}
						sprintf(dummy,"%s        %s  %.12E\0",dummy,GetIsotopePtr(Z,i)->GetZAIDPtr()->GetZAIDcard(),GetIsotopePtr(Z,i)->GetAtomfrac());
						L->WriteLine(dummy,doprintstatus);
					}
				}
			}
		}
	}
	double CleanUpOmits_Advanced(int low=1,int high=109){
		// will remove the isotopes, but will not change the mass!
		if(GetIsotopePtr(90231))
			if(GetIsotopePtr(90231)->GetAtomfrac())
				if(GetIsotopePtr(91231))
					if(GetIsotopePtr(91231)->GetAtomfrac()){
						cout<<"CleanUpOmits:: Isotope: 90231 (with "<<this->GetMassIsotope(90231)
							<<" g) was dacayed into 91231."<<endl;
						_mass-=this->GetMassIsotope(90231);
						this->AddMass(91231,this->GetMassIsotope(90231),false,false);
						this->ScaleIsotope(90231,0);
					}
		double mass=0;
		for(int z=low;z<high+1;z++){
			for(int i=0;i<20;i++){
				if(GetIsotopePtr(z,i)){
					if(Omit->Omit(GetIsotopePtr(z,i)->GetZAID())){
						if(GetIsotopePtr(z,i)->GetAtomfrac()==0)continue;
						cout<<"CleanUpOmits:: Isotope: "<<GetIsotopePtr(z,i)->GetZAID()<<" (with "<<_mass*GetIsotopePtr(z,i)->GetMassfrac()<<" g) was on Omit list and has been removed."<<endl;
						mass+=_mass*GetIsotopePtr(z,i)->GetMassfrac();
						this->GetIsotopePtr(z,i)->ScaleFraction(0);
					}
				}
			}
		}
		return mass;
	}
	double CleanUpOmits(int low=1,int high=109){
		// will remove the isotopes, but will not change the mass!
		double mass=0;
		for(int z=low;z<high+1;z++){
			for(int i=0;i<20;i++){
				if(GetIsotopePtr(z,i)){
					if(Omit->Omit(GetIsotopePtr(z,i)->GetZAID())){
						cout<<"CleanUpOmits:: Isotope: "<<GetIsotopePtr(z,i)->GetZAID()<<" (with "<<_mass*GetIsotopePtr(z,i)->GetMassfrac()<<" g) was on Omit list and has been removed."<<endl;
						mass+=_mass*GetIsotopePtr(z,i)->GetMassfrac();
						this->GetIsotopePtr(z,i)->ScaleFraction(0);
					}
				}
			}
		}
		return mass;
	}

	void AddMass(int zaid, double mass,bool normalize=true,bool printwarnings=true){
		Isotope *I=this->GetIsotopePtr(zaid);
		if(!I){if(printwarnings)cout<<"WARNING::Material::AddMass:: cannot add mass to nonexisting isotope: "<<zaid<<endl;return;}
		// assuming that frac sum = 1, one could normalize before running this function
		double totalfrac=this->GetMassFracAllIsotopes();
		double frac0=I->GetMassfrac();

		double fracchange=(_mass*frac0/totalfrac+mass)*(totalfrac-frac0)/(_mass*frac0-_mass*frac0*frac0/totalfrac);
		if(_mass+mass/I->GetMassfrac()*this->GetMassFracAllIsotopes()<0){cout<<"ERROR::Material::AddMass:: You are trying to remove more mass than there is of isotope: "<<zaid<<endl;return;}
		I->ScaleFraction(fracchange);
		_mass+=mass;
		if(normalize)this->NormalizeMaterialFractions();
	}
	void AddMass(Material *M,double mass,bool RescaleAfterOmitted=false, bool normalize=true,bool printwarnings=true){
		double syncronizablemass=0;
		double InputTotalMassFrac=M->GetMassFracAllIsotopes();
		double ThisTotalMassFrac=this->GetMassFracAllIsotopes();
		for(int z=1;z<109;z++){
			for(int i=0;i<20;i++){
				if(M->GetIsotopePtr(z,i)){
					if(this->GetIsotopePtr(M->GetIsotopePtr(z,i)->GetZAID())){
						if(this->GetIsotopePtr(M->GetIsotopePtr(z,i)->GetZAID())->GetAtomfrac()!=0){
							syncronizablemass+=mass*M->GetIsotopePtr(z,i)->GetMassfrac()/InputTotalMassFrac;
							continue;
						}
						if(printwarnings)cout<<"WARNING::Material::AddMass:: Non scaleable isotope: "<<M->GetIsotopePtr(z,i)->GetZAID()
					<<" with mass frac: "<<this->GetIsotopePtr(z,i)->GetMassfrac()<<" will be omitted."<<endl;
					if(RescaleAfterOmitted)InputTotalMassFrac-=M->GetIsotopePtr(z,i)->GetMassfrac();
					}
				}
			}
		}
		double masstoadd=mass;
		if(RescaleAfterOmitted)masstoadd=mass*mass/syncronizablemass;
		for(int z=1;z<110;z++){
			for(int i=0;i<20;i++){
				if(M->GetIsotopePtr(z,i)){
					if(this->GetIsotopePtr(M->GetIsotopePtr(z,i)->GetZAID())){
						if(this->GetIsotopePtr(M->GetIsotopePtr(z,i)->GetZAID())->GetAtomfrac()!=0){
//							cout<<"adding "<<masstoadd*M->GetIsotopePtr(z,i)->GetMassfrac()/InputTotalMassFrac<<" to isotope: "<<M->GetIsotopePtr(z,i)->GetZAID()<<endl;
							AddMass(M->GetIsotopePtr(z,i)->GetZAID(),masstoadd*M->GetIsotopePtr(z,i)->GetMassfrac()/InputTotalMassFrac,false,printwarnings);
						}
					}
				}
			}
		}
		if(normalize)this->NormalizeMaterialFractions();
	}

};

class EconomyCalculator{
	Material *M;
	DecayList *D;
	double Leagkage;
	char RC[180];
public:
	EconomyCalculator(Material *inM,double leakage=0){
		D=new DecayList();
		M=inM;
		Leagkage=leakage;
	}

	void PrintEconomyTable(Logbook *L=0){
		sprintf(RC,"zaid\t\t\tnu\teta\tfiss\tn,gam\tn,2n\tn,3n\tdecay\tproduct\t\t2nd product\0");					if(L)L->WriteLine(RC,false);cout<<RC<<endl;
		Isotope *I;
		for(int z=90;z<109;z++){
					cout<<endl;

			for(int i=0;i<20;i++){
					I=M->GetIsotopePtr(z,i);
				if(!I)continue;
				if(!D->GetDecay(I->GetZAID())){
					cout<<"Missing decay data for isotope with zaid: "<<I->GetZAID()<<endl;
					continue;
				}
				double N=I->GetAtomdensity()*M->GetVolume()*1e24;
				double ND=D->GetDecayFraction(I->GetZAID())*N;
				double NR=(I->GetNeutronsAbsorbed()+ND)/100;
				double nu=0;
				char Dky2[12];
				char Dky1[12];
				sprintf(Dky1,"%d\t\0",D->GetDecay(I->GetZAID())->DecayZAID);
				sprintf(Dky2,"NA\t\0");
				int decay2=D->GetDecay(I->GetZAID())->DecayZAID2;
				if(decay2){
					sprintf(Dky1,"%d (%1.2f%%)\0",D->GetDecay(I->GetZAID())->DecayZAID,D->GetDecay(I->GetZAID())->Prob1*100);
					sprintf(Dky2,"%d (%1.2f%%)\0",D->GetDecay(I->GetZAID())->DecayZAID2,100-D->GetDecay(I->GetZAID())->Prob1*100);
				}

				if(I->GetFission())nu=I->GetNuFission()/I->GetFission();
				sprintf(RC,"%d\t\t\t%1.3f\t%1.3f\t%1.2f\t%1.2f\t%1.2f\t%1.2f\t%1.2f\t%s\t%s\0"
					,I->GetZAID()
					,nu
					,I->GetNeutronsPerNeutron()
					,I->GetFission()/NR
					,I->GetNGamma()/NR
					,I->GetN2n()/NR
					,I->GetN3n()/NR
					,D->GetDecayFraction(I->GetZAID())*1e24*M->GetVolume()*I->GetAtomdensity()/NR
					,Dky1
					,Dky2
					,I->GetNP()/NR
					,I->GetNAlpha()/NR
					);					if(L)L->WriteLine(RC,false);cout<<RC<<endl;
			}
		}
	}


	void PrintThCycle(Logbook *L=0){
		double fiss_LT=0.01;
		double ng_LT=0.01;
		double n2n_LT=0.01;
		double dec_LT=0.01;
		
		
		
		Isotope *I;

		sprintf(RC,"Zaid\t\tnu\t\tfiss\tngamma\tn2n\tdecay(s)\0");if(L)L->WriteLine(RC,false);cout<<RC<<endl;
		for(int z=90;z<109;z++){
			for(int i=0;i<20;i++){
				I=M->GetIsotopePtr(z,i);
				if(!I)continue;
				if(!D->GetDecay(I->GetZAID())){
					cout<<"Missing decay data for isotope with zaid: "<<I->GetZAID()<<endl;
					continue;
				}
				double N=I->GetAtomdensity()*M->GetVolume()*1e24;
				double ND=D->GetDecayFraction(I->GetZAID())*N;
				double NR=(I->GetNeutronsAbsorbed()+ND)/100;
				if(NR==0)continue;

				double nu=0;
				double fiss=I->GetFission()/NR;
				if(fiss)nu=I->GetNuFission()/I->GetFission();
				double ngamma=I->GetNGamma()/NR;
				double n2n=I->GetN2n()/NR;
				double dec=ND/NR;
				double prob1=D->GetDecay(I->GetZAID())->Prob1;
				double prob2=1-prob1;
				int D1=D->GetDecay(I->GetZAID())->DecayZAID;
				int D2=D->GetDecay(I->GetZAID())->DecayZAID2;


				char Cfiss[10];
				sprintf(Cfiss,"\t\0");
				char Cnu[10];
				sprintf(Cnu,"\t\0");
				if(fiss>fiss_LT){
					sprintf(Cfiss,"%.3f\t\0",fiss);
					sprintf(Cnu,"%.3f\t\0",nu);
				}
				char Cngamma[10];
				sprintf(Cngamma,"\t\0");
				if(ngamma>ng_LT)sprintf(Cngamma,"%.2f\t\0",ngamma);

				char Cn2n[10];
				sprintf(Cn2n,"\t\0");
				if(n2n>n2n_LT)sprintf(Cn2n,"%.2f\t\0",n2n);

				char Cdec[25];
				sprintf(Cdec,"\t\0");
				if(dec>dec_LT){
					if(prob1>.99)sprintf(Cdec,"%.2f (%d)\0",dec,D1);
					else sprintf(Cdec,"%.2f (%d) %.2f (%d)\0",dec*prob1,D1,dec*prob2,D2);
				}

				sprintf(RC,"%d\t\t%s\t%s%s%s%s\0"
					,I->GetZAID()
					,Cnu
					,Cfiss
					,Cngamma
					,Cn2n
					,Cdec);					if(L)L->WriteLine(RC,false);cout<<RC<<endl;
				
			}
		}
	}

};


class BurnupStep {
private:
	int _step; 
	double _duration;
	double _time;
	double _power;
	double _keff;
	double _flux;
	double _ave_nu;
	double _ave_q;
	double _burnup;
	double _source;
public:
	BurnupStep(int instep, double induration, double intime, double inpower, double inkeff,
		double influx, double inave_nu, double inave_q, double inburnup, double insource){
		_step=instep; 
		_duration=induration;
		_time=intime;
		_power=inpower;
		_keff=inkeff;
		_flux=influx;
		_ave_nu=inave_nu;
		_ave_q=inave_q;
		_burnup=inburnup;
		_source=insource;
	}
	BurnupStep(string NeutronicsLine){
		stringstream stream(NeutronicsLine);
		_source=-1;
		double toomany_numbers=-1;
		stream >> _step >> _duration >> _time >> _power >> _keff >> _flux >> _ave_nu >> _ave_q >> _burnup >> _source >> toomany_numbers;
		if(_source==-1||toomany_numbers!=-1)cout<<"ERROR::Isotope_KcodeNeutronics::Isotope_KcodeNeutronics:: string format incorrect!!"<<endl;
	}
	BurnupStep(BurnupStep *KNL){
		_step=KNL->GetStep(); 
		_duration=KNL->GetTime_thisstep_days();
		_time=KNL->GetTime_sincet0_days();
		_power=KNL->GetPower();
		_keff=KNL->GetKeff();
		_flux=KNL->GetFlux();
		_ave_nu=KNL->GetAve_nu();
		_ave_q=KNL->GetAve_q();
		_burnup=KNL->GetBurnup();
		_source=KNL->GetSource();
	}
	~BurnupStep(){
	}
	// basic values
	int GetStep(){return _step;} 
//	double duration(){return _duration;} this would just be confusing...
//	double time(){return _time;} this would just be confusing...
	double GetPower(){return _power;}
	double GetKeff(){return _keff;}
	double GetFlux(){return _flux;}
	double GetAve_nu(){return _ave_nu;}
	double GetAve_q(){return _ave_q;}
	double GetBurnup(){return _burnup;}
	double GetSource(){return _source;}
	// derived quantities
	double GetTime_thisstep(){
		return _duration*86400;
	}
	double GetTime_thisstep_days(){
		return _duration;
	}
	double GetTime_sincet0(){
		return _time*86400;
	}
	double GetTime_sincet0_days(){
		return _time;
	}


};




class FullBurnupRun{
private:
	double _leakage[100];
	int nsteps;
	int nmaterials;
	BurnupStep *_step[100];
	Material *_mat[100][100];
protected:
public:


	int GetStepId(int step){
		for(int i=0;i<nsteps;i++)
			if(_step[i])
				if(_step[i]->GetStep()==step)return  i;
		return -1;
	}
	int GetMaterialId(int material){
		int step;
		bool hasstep=false;
		for(step=0;step<nsteps;step++)
			if(_step[step]){hasstep=true;break;}
		if(!hasstep){cout<<"ERROR::FullBurnupRun::GetMaterialId:: No valid steps..."<<endl;return -1;}
		for(int i=0;i<nmaterials;i++)
			if(_mat[step][i])
				if(_mat[step][i]->GetMat_id()==material)return  i;
		return -1;
	}
	void Initialize(){
		nsteps=0;
		nmaterials=0;
		for(int i=1;i<100;i++){
			_leakage[i]=0;
			_step[i]=0;
			for(int j=1;j<100;j++){
				_mat[i][j]=0;
			}
		}
		
	}

	FullBurnupRun(KcodeReader *outp, double temperature,int LoadOnlyThisStep=-1, int LoadOnlyThisMaterial=-1,bool loadsumofmaterials=false,bool dowritestatus=true){
		if(dowritestatus)cout<<"Initializing..."<<endl;
		Initialize();
		// locating neutronics table:
		if(dowritestatus)cout<<"Locating file pointers..."<<endl;
		int neutronicsline=outp->FindLine("neutronics",-outp->LastLineIndex+1)+4;
		int endneutronicsitr=outp->FindLine(neutronicsline);
		if(LoadOnlyThisStep!=-1){
			neutronicsline+=LoadOnlyThisStep;
			endneutronicsitr=neutronicsline+1;
		}
		// locating materials
		int materialsnumbers[100];
		materialsnumbers[0]=0;
		int dummyitr=1;
		if(LoadOnlyThisMaterial!=-1){
			materialsnumbers[1]=LoadOnlyThisMaterial;
			materialsnumbers[2]=-1;
		} else 
		for(int i=1;i<100;i++){
			if(dummyitr<1)break;
			materialsnumbers[i]=-1;
			bool docontinue=true;
			while(true){
				dummyitr=outp->FindLine("nuclide data "," material",dummyitr+1);
				if(dummyitr<0)break;
				double *dummydouble=outp->Get_ListOfNumbers(dummyitr);
				if(outp->Get_ListOfNumbers__arraylength==3){
					for(int j=1;j<i;j++){
						if(materialsnumbers[j]==1*dummydouble[0]){
							docontinue=false;
							break;
						}
					}
				} else continue;
				if(!docontinue)continue; // that is counterintuitive notation...
				materialsnumbers[i]=1*dummydouble[0];
				break;
			}
		}
		if(dowritestatus)cout<<"Will store the following materials:"<<endl;
		if(loadsumofmaterials)cout<<" - Sum of materials (m0)"<<endl; 
		for(int j=1;j<100;j++){
			if(materialsnumbers[j]==-1)break;
			if(dowritestatus)cout<<" - Material: "<<materialsnumbers[j]<<endl;
		}

		if(dowritestatus)cout<<"For each of the following steps:"<<endl;
		for(int i=neutronicsline;i<endneutronicsitr;i++){
			outp->printsection(i,1);
		}
		// start loading...
		for(int i=neutronicsline;i<endneutronicsitr;i++){
			if(dowritestatus)cout<<"Loading step: ";outp->GetLine(i);
			_step[nsteps]=new BurnupStep(outp->GetLine(i));
		// start loading materials...
			if(dowritestatus)cout<<"Loading materials for this step..."<<endl;
			if(loadsumofmaterials){
				_mat[nsteps][0]=new Material(0,0,temperature);
				_mat[nsteps][0]->LoadFromBurnupOutput(outp,_step[nsteps]->GetStep(),false,dowritestatus);
			}
			if(nsteps==0)nmaterials++;
			for(int j=1;j<100;j++){
				if(materialsnumbers[j]==-1)break;
				_mat[nsteps][j]=new Material(materialsnumbers[j],0,temperature);
				_mat[nsteps][j]->LoadFromBurnupOutput(outp,_step[nsteps]->GetStep(),false,dowritestatus);
				if(nsteps==0)nmaterials++;
			}
			nsteps++;
		}
	}
	~FullBurnupRun(){
		for(int i=1;i<100;i++){
			if(_step[i])delete _step[i];
			for(int j=1;j<100;j++){
				if(_mat[i][j])delete _mat[i][j];
			}
		}
	}

	// fetchers:
	BurnupStep *GetStepPtr(int step){
		int stepid=GetStepId(step);
		if(stepid!=-1)
			return (BurnupStep*)_step[stepid];
		return 0;
	}
	Material *GetMaterialPtr(int material, int step){
		int stepid=GetStepId(step);
		int matid=GetMaterialId(material);
		if(stepid!=-1&&matid!=-1)
			return (Material*)_mat[stepid][matid];
		return 0;
	}

	double GetNeutronsPerNeutronAllMaterials(int step,int low=1, int high=109){
		if(GetNeutronsAbsorptionAllMaterials(step, low, high))return GetNeutronsCreatedAllMaterials(step, low, high)/GetNeutronsAbsorptionAllMaterials(step, low, high);
		return 0;
	}
	double GetNeutronsCapturedAllMaterials(int step,int low=1, int high=109){
		double sumCap=0;
		for(int i=1;i<nmaterials;i++){
			if(this->GetStepId(step)!=-1){
				if(_mat[this->GetStepId(step)][i]){
					sumCap+=_mat[this->GetStepId(step)][i]->GetNeutronsCaptureAll(low,high);
				}
			}
		}
		return sumCap;
	}
	double GetNeutronsCreatedAllMaterials(int step,int low=1, int high=109){
		double sumCre=0;
		for(int i=1;i<nmaterials;i++){
			if(this->GetStepId(step)!=-1){
				if(_mat[this->GetStepId(step)][i]){
					sumCre+=_mat[this->GetStepId(step)][i]->GetNeutronsCreatedAll(low,high);
				}
			}
		}
		return sumCre;
	}
	double GetNeutronsAbsorptionAllMaterials(int step,int low=1, int high=109){
		double sumAbs=0;
		for(int i=1;i<nmaterials;i++){
			if(this->GetStepId(step)!=-1){
				if(_mat[this->GetStepId(step)][i]){
					sumAbs+=_mat[this->GetStepId(step)][i]->GetNeutronsAbsorbedAll(low,high);
				}
			}
		}
		return sumAbs;
	}
	double GetCalculatedLeakageFactor(int step, int material=0){
		double leakage=0;
		if(!material){
			leakage=(GetNeutronsPerNeutronAllMaterials(step)-GetStepPtr(step)->GetKeff())/GetStepPtr(step)->GetKeff();
		} else {
			leakage=(this->GetMaterialPtr(material,step)->GetNeutronsPerNeutronAll()-GetStepPtr(step)->GetKeff())/GetStepPtr(step)->GetKeff();
		}
		return leakage;
	}
	int GetNMaterials(){
		return this->nmaterials;
	}
	int GetNSteps(){
		return this->nsteps;
	}

	double PredictKeff(double Leakagefactor, Material *fuel, Material *moderator=0){
		double NCreMod=0;
		if(moderator)NCreMod=moderator->GetNeutronsCreatedAll();
		double NCreFuel=fuel->GetNeutronsCreatedAll();
		double NCreSys=NCreMod+NCreFuel;
		double NAbsMod=0;
		if(moderator)NAbsMod=moderator->GetNeutronsAbsorbedAll();
		double NAbsFuel=fuel->GetNeutronsAbsorbedAll();
		double NAbsSys=NAbsMod+NAbsFuel;
		double N2NSystem=NCreSys/NAbsSys;
		return N2NSystem/(1+Leakagefactor);
	}

	void PrintEssentials(int step,int fuelid=1001,int moderatorid=2000){
		
		if(!this->GetStepPtr(step))return;
		cout<<"|||||||||||||||||||||||||||||||||||||||||||||||||||||"<<endl;
		cout<<"||||||||||||Essential information in step: "<<step<<"||||||||"<<endl;
		cout<<"|||||||||||||||||||||||||||||||||||||||||||||||||||||"<<endl;
		cout<<"Moderator: m"<<moderatorid<<" had a total volume of: "<<this->GetMaterialPtr(moderatorid,step)->GetVolume()<<" cm3"
			<<" and a total mass of: "<<this->GetMaterialPtr(moderatorid,step)->GetMass()<<" g"<<endl;
		cout<<"Fuel: m"<<fuelid<<" had a total volume of: "<<this->GetMaterialPtr(fuelid,step)->GetVolume()
			<<" and a total mass of: "<<this->GetMaterialPtr(fuelid,step)->GetMass()<<endl;
		cout<<"The last burn step ("<<step<<") has burned with a power of: "<<this->GetStepPtr(step)->GetPower()<<" MW"
			<<" for "<<this->GetStepPtr(step)->GetTime_sincet0_days()<<" days (since t0)"
			<<" this corresponts to a burnup of: "<<this->GetStepPtr(step)->GetBurnup()<<" GWd/tHM"<<endl;

		cout<<endl<<"<<<<<<<<<<<<< Keff and overall reactor economy<<<<<<<<<"<<endl;
		cout<<"Keff is "<<this->GetStepPtr(step)->GetKeff();
		if(GetStepPtr(0))
			cout<<" this is a change of: "<<this->GetStepPtr(step)->GetKeff()-GetStepPtr(0)->GetKeff()<<" since t0 Keff_t0=("<<GetStepPtr(0)->GetKeff()<<").";
		cout<<endl;

		cout<<"It is estimated that the reactor leaks ~"<<this->GetCalculatedLeakageFactor(step)*100<<"% of its neutrons"<<endl;
		double absM=this->GetMaterialPtr(moderatorid,step)->GetNeutronsAbsorbedAll();
		double absF=this->GetMaterialPtr(fuelid,step)->GetNeutronsAbsorbedAll();
		double absT=absM+absF;
		cout<<"   and that ~"<<absM/absT*100<<"% of its neutrons is lost in the moderator"<<endl;
		cout<<"   the average neutrons per neutron in the fuel is: "<<this->GetMaterialPtr(fuelid,step)->GetNeutronsPerNeutronAll()<<endl;



		if(GetStepPtr(0)){
			double Abs=this->GetMaterialPtr(fuelid,step)->GetNeutronsAbsorbedAll(1,109);
			double AbsAc=this->GetMaterialPtr(fuelid,step)->GetNeutronsAbsorbedAll(90,109);
			double AbsLight=this->GetMaterialPtr(fuelid,step)->GetNeutronsAbsorbedAll(1,11);
			double AbsFP=this->GetMaterialPtr(fuelid,step)->GetNeutronsAbsorbedAll(12,89);

			double Abs0=this->GetMaterialPtr(fuelid,0)->GetNeutronsAbsorbedAll(1,109);
			double AbsAc0=this->GetMaterialPtr(fuelid,0)->GetNeutronsAbsorbedAll(90,109);
			double AbsLight0=this->GetMaterialPtr(fuelid,0)->GetNeutronsAbsorbedAll(1,11);
			double AbsFP0=this->GetMaterialPtr(fuelid,0)->GetNeutronsAbsorbedAll(12,89);

			double leakage0=this->GetCalculatedLeakageFactor(0)*100;
			double leakage=this->GetCalculatedLeakageFactor(step)*100;



			cout<<"The keff change of "<<this->GetStepPtr(step)->GetKeff()-GetStepPtr(0)->GetKeff()<<", is expected from:";
			cout<<"Leakage : "<<leakage-leakage0<<" which is a change of: "<<leakage-leakage0<<" (from t0)"<<endl;
			cout<<"Absorbtion:"<<endl;
			cout<<"Light element:   "<<AbsLight/Abs*100<<"% which is a change of: "<<AbsLight/Abs*100-AbsLight0/Abs0*100<<"% (from t0)"<<endl;
			cout<<"Fission product: "<<AbsFP/Abs*100   <<"% which is a change of: "<<AbsFP/Abs*100-AbsFP0/Abs0*100<<"% (from t0)"<<endl;
			cout<<"Actinides:       "<<AbsAc/Abs*100   <<"% which is a change of: "<<AbsAc/Abs*100-AbsAc0/Abs0*100<<"% (from t0)"<<endl;

			double NpN=this->GetMaterialPtr(fuelid,step)->GetNeutronsPerNeutronAll();
			double NpNAc=this->GetMaterialPtr(fuelid,step)->GetNeutronsPerNeutronAll(90,109);
			double NpNPu=this->GetMaterialPtr(fuelid,step)->GetNeutronsPerNeutronAll(94,94);
			double NpNFP=this->GetMaterialPtr(fuelid,step)->GetNeutronsPerNeutronAll(1,89);

			double NpN0=this->GetMaterialPtr(fuelid,0)->GetNeutronsPerNeutronAll();
			double NpNAc0=this->GetMaterialPtr(fuelid,0)->GetNeutronsPerNeutronAll(90,109);
			double NpNPu0=this->GetMaterialPtr(fuelid,0)->GetNeutronsPerNeutronAll(94,94);
			double NpNFP0=this->GetMaterialPtr(fuelid,0)->GetNeutronsPerNeutronAll(1,89);

			cout<<"FP quality (N per N):    "<<NpNFP<<" which is a change of: "<<NpNFP-NpNFP0<<" (from t0)"<<endl;
			cout<<"Ac quality (N per N):    "<<NpNAc<<" which is a change of: "<<NpNAc-NpNAc0<<" (from t0)"<<endl;
			cout<<" - Pu quality (N per N): "<<NpNPu<<" which is a change of: "<<NpNPu-NpNPu0<<" (from t0)"<<endl;

			double  Pu241Abs=this->GetMaterialPtr(fuelid,0)->GetNeutronsAbsorbedIsotope(94241);
			double  Pu241Cre=this->GetMaterialPtr(fuelid,0)->GetNeutronsCreatedIsotope(94241);
			double  PuxxxAbs=0;
			double  PuxxxCre=0;
			for(int i=230;i<250;i++){
				if(this->GetMaterialPtr(fuelid,0)->GetIsotopePtr(94000+i)){
					if(i==241)continue;
					PuxxxAbs+=this->GetMaterialPtr(fuelid,0)->GetIsotopePtr(94000+i)->GetNeutronsAbsorbed();
					PuxxxCre+=this->GetMaterialPtr(fuelid,0)->GetIsotopePtr(94000+i)->GetNeutronsCreated();
				}
			}
			double Pu241decayfraction=TMath::Power(.5,this->GetStepPtr(step)->GetTime_sincet0_days()/365/14.4);
			cout<<"||"<<Pu241decayfraction<<"||"<<endl;
			Pu241Abs*=Pu241decayfraction;
			Pu241Cre*=Pu241decayfraction;
			PuxxxCre+=Pu241Cre;
			PuxxxAbs+=Pu241Abs;
			cout<<"the expected Pu quallity reduction of from Pu241 decay alone: "<<PuxxxCre/PuxxxAbs-NpNPu0<<endl;
		}



		cout<<endl<<"<<<<<<<<<<<<< Fission information: <<<<<<<<<<<<<<<<<<<<"<<endl;
		cout<<"The average Q value per fission is: "<<this->GetStepPtr(step)->GetAve_q()<<" MeV with <nu>="<<this->GetStepPtr(step)->GetAve_nu()
			<<" - the fission energy comes from:"<<endl;
		double totalfisq=this->GetMaterialPtr(fuelid,step)->GetFissionQAllIsotopes();
		cout<<"Th:     "<<this->GetMaterialPtr(fuelid,step)->GeFissionQElement(90)/totalfisq*100<<"%"<<endl;
		cout<<"U:      "<<this->GetMaterialPtr(fuelid,step)->GeFissionQElement(92)/totalfisq*100<<"%"
			<<"   ( where "<<this->GetMaterialPtr(fuelid,step)->GetFissionQIsotope(92233)/totalfisq*100<<"% was from U233)"
			<<endl;
		cout<<"Pu:     "<<this->GetMaterialPtr(fuelid,step)->GeFissionQElement(94)/totalfisq*100<<"%"
			<<"   ( where "<<this->GetMaterialPtr(fuelid,step)->GetFissionQIsotope(94241)/totalfisq*100<<"% was from Pu241)"
			<<endl;
		cout<<"Others: "<<(totalfisq-this->GetMaterialPtr(fuelid,step)->GeFissionQElement(90)
			-this->GetMaterialPtr(fuelid,step)->GeFissionQElement(92)
			-this->GetMaterialPtr(fuelid,step)->GeFissionQElement(94))/totalfisq*100<<"%"<<endl;





		cout<<endl<<"<<<<<<<<<<<<< Fuel salt composition: <<<<<<<<<<<<<<<<<<"<<endl;
		double Acfrac=this->GetMaterialPtr(fuelid,step)->GetAtomFracAllIsotopes(90,109);
		double NonAcfrac=this->GetMaterialPtr(fuelid,step)->GetAtomFracAllIsotopes(1,89);
		double Lightfrac=this->GetMaterialPtr(fuelid,step)->GetAtomFracAllIsotopes(1,23);
		double Lifrac=this->GetMaterialPtr(fuelid,step)->GetAtomFracAllIsotopes(3,3);
		double Ffrac=this->GetMaterialPtr(fuelid,step)->GetAtomFracAllIsotopes(19,19);
		double FPfrac=this->GetMaterialPtr(fuelid,step)->GetAtomFracAllIsotopes(23,89);
		double Thfrac=this->GetMaterialPtr(fuelid,step)->GetAtomFracAllIsotopes(90,90);
		double Pafrac=this->GetMaterialPtr(fuelid,step)->GetAtomFracAllIsotopes(91,91);
		double Ufrac=this->GetMaterialPtr(fuelid,step)->GetAtomFracAllIsotopes(92,92);
		double Pufrac=this->GetMaterialPtr(fuelid,step)->GetAtomFracAllIsotopes(94,94);
		double Trfrac=this->GetMaterialPtr(fuelid,step)->GetAtomFracAllIsotopes(93,109)+this->GetMaterialPtr(fuelid,step)->GetAtomFracAllIsotopes(91,91);;
		double Otherfrac=Trfrac-Pufrac;
		double nonFfrac=1-Ffrac;
		double Li6vs7=this->GetMaterialPtr(fuelid,step)->GetAtomFracIsotope(3006)/Lifrac;

		cout<<"The 6Li concentration in the Li is now "<<Li6vs7<<"a% "<<endl;

		double Acmfrac=this->GetMaterialPtr(fuelid,step)->GetMassFracAllIsotopes(90,109);
		double NonAcmfrac=this->GetMaterialPtr(fuelid,step)->GetMassFracAllIsotopes(1,89);
		double Lightmfrac=this->GetMaterialPtr(fuelid,step)->GetMassFracAllIsotopes(1,23);
		double Limfrac=this->GetMaterialPtr(fuelid,step)->GetMassFracAllIsotopes(3,3);
		double Fmfrac=this->GetMaterialPtr(fuelid,step)->GetMassFracAllIsotopes(19,19);
		double FPmfrac=this->GetMaterialPtr(fuelid,step)->GetMassFracAllIsotopes(23,89);
		double Thmfrac=this->GetMaterialPtr(fuelid,step)->GetMassFracAllIsotopes(90,90);
		double Pamfrac=this->GetMaterialPtr(fuelid,step)->GetMassFracAllIsotopes(91,91);
		double Umfrac=this->GetMaterialPtr(fuelid,step)->GetMassFracAllIsotopes(92,92);
		double Pumfrac=this->GetMaterialPtr(fuelid,step)->GetMassFracAllIsotopes(94,94);
		double Trmfrac=this->GetMaterialPtr(fuelid,step)->GetMassFracAllIsotopes(93,109)+this->GetMaterialPtr(fuelid,step)->GetMassFracAllIsotopes(91,91);;
		double Othermfrac=Trmfrac-Pumfrac;

		
		if(GetStepPtr(0)){

		}


	}

};


class Reprocessor{
private:
protected:
public:
	KcodeReader *KKR;
	Material *RefuelMat;
	Material *LiMat;
	Logbook *log;
	double Chemistry[110];
	double Bubbeler[110];
	double ChemistryPerDay_liters;
	//double LiFraction;
	//double AcFraction;
	double Refueling_Iterations;
	double Refueling_StartingAmmount;
	double RemoveUranium;
	DecayList *Decayer;
	Reprocessor(char *filename, KcodeReader *FuelAndLiInfromationOutp, int materialstep=-1, bool dowritestatus=true){
		Decayer=new DecayList();
		log=new Logbook();
		if(dowritestatus)cout<<"Initializing Reprocessor"<<endl; 
		KKR=0;
		KKR=new KcodeReader(filename);
		double *dummydouble;
		int zaiddummy=-1;
		double atomfracdummy=-1;
		int Zdummy=-1;
		double fracdummy=-1;
		RemoveUranium=0;
		Refueling_Iterations=3;
		Refueling_StartingAmmount=25000;
		if(dowritestatus)cout<<"Loading refuel vector..."<<endl; 
		int lineindex=KKR->FindLine("RPI:","Refuel Vector");
		if(lineindex==-1){cout<<"ERROR::Reprocessor::Reprocessor:: could not fine \"Refuel Vector\" line in "<<filename<<endl;return;}
		dummydouble=KKR->Get_ListOfNumbers(lineindex+1);
		if(KKR->Get_ListOfNumbers__arraylength<2){cout<<"ERROR::Reprocessor::Reprocessor:: No refuel vector";return;}
		RefuelMat=new Material(dummydouble[0],1,T2E(1000),1);
		if(materialstep==-1){
			RefuelMat->LoadFromBurnupOutput(FuelAndLiInfromationOutp,dummydouble[1],false,dowritestatus);
		} else {
			RefuelMat->LoadFromBurnupOutput(FuelAndLiInfromationOutp,materialstep,false,dowritestatus);
		}
		Refueling_Iterations=3;
		Refueling_StartingAmmount=10000;
		if(KKR->Get_ListOfNumbers__arraylength>2){
			RemoveUranium=dummydouble[2];
			if(KKR->Get_ListOfNumbers__arraylength>3){
				Refueling_Iterations=dummydouble[3];
				if(KKR->Get_ListOfNumbers__arraylength>4){
					Refueling_StartingAmmount=dummydouble[4];
				}
			}
		}
		// fixing omits
		RefuelMat->CleanUpOmits_Advanced();
			//keep 1 percent of fission products
		RefuelMat->ScaleAllIsotopes(.01,1,2);
		RefuelMat->ScaleAllIsotopes(.01,4,8);
		RefuelMat->ScaleAllIsotopes(.01,10,89);
		// remove nobel gasses and oxygen:
		RefuelMat->ScaleAllIsotopes(0.,2,2); 
		RefuelMat->ScaleAllIsotopes(0.,8,8); 
		RefuelMat->ScaleAllIsotopes(0.,10,10); 
		RefuelMat->ScaleAllIsotopes(0.,18,18); 
		RefuelMat->ScaleAllIsotopes(0.,36,36); 
		RefuelMat->ScaleAllIsotopes(0.,54,54); 
		RefuelMat->ScaleAllIsotopes(0.,86,86);
		// removing Uranium with 1e-7 Ac contamination:
		RefuelMat->ScaleAllIsotopes(4./94.,92,92);
		RefuelMat->ScaleAllIsotopes(1.-4./94.*1e-5,93,109);
		RefuelMat->ScaleAllIsotopes(1.-4./94.*1e-5,90,91);
		// secund round
		RefuelMat->ScaleAllIsotopes(.25,92,92);
		// fixing Li and F into 78LiF-22AcF4
		RefuelMat->ScaleAllIsotopes(
			RefuelMat->GetAtomFracAllIsotopes(90,109)/22.*78./RefuelMat->GetAtomFracAllIsotopes(3,3)
			,3,3);
		RefuelMat->ScaleAllIsotopes(
			RefuelMat->GetAtomFracAllIsotopes(90,109)/22.*166./RefuelMat->GetAtomFracAllIsotopes(9,9)
			,9,9);
		RefuelMat->NormalizeMaterialFractions(1);

		
		if(dowritestatus)cout<<"Loading Lithium vector..."<<endl;
		lineindex=KKR->FindLine("RPI:","Li Vector");
		if(lineindex==-1){cout<<"ERROR::Reprocessor::Reprocessor:: could not fine \"Li Vector\" line in "<<filename<<endl;return;}
		dummydouble=KKR->Get_ListOfNumbers(lineindex+1);
		if(KKR->Get_ListOfNumbers__arraylength<2){cout<<"ERROR::Reprocessor::Reprocessor:: No Li vector";return;}
		LiMat=new Material(dummydouble[0],1,0,1);
		LiMat->LoadFromBurnupOutput(FuelAndLiInfromationOutp,dummydouble[1],false,dowritestatus);
		/*
		if(dowritestatus)cout<<"Loading LiF Fraction parameters..."<<endl; 
		lineindex=KKR->FindLine("RPI:","LiF Fraction");
		if(lineindex==-1){cout<<"ERROR::Reprocessor::Reprocessor:: could not fine \"LiF Fraction\" line in "<<filename<<endl;return;}
		dummydouble=KKR->Get_ListOfNumbers(lineindex+1);
		if(KKR->Get_ListOfNumbers__arraylength!=1){cout<<"ERROR::Reprocessor::Reprocessor:: Wrong format of \"LiF Fraction\" "<<endl;return;}
		LiFraction=dummydouble[0];
		
		if(dowritestatus)cout<<"Loading AcF4 Fraction parameters..."<<endl; 
		lineindex=KKR->FindLine("RPI:","AcF4 Fraction");
		if(lineindex==-1){cout<<"ERROR::Reprocessor::Reprocessor:: could not fine \"AcF4 Fraction\" line in "<<filename<<endl;return;}
		dummydouble=KKR->Get_ListOfNumbers(lineindex+1);
		if(KKR->Get_ListOfNumbers__arraylength!=1){cout<<"ERROR::Reprocessor::Reprocessor:: Wrong format of \"AcF4 Fraction\" "<<endl;return;}
		AcFraction=dummydouble[0];
		*/
		if(dowritestatus)cout<<"Loading flame reactor parameters..."<<endl; 
		lineindex=KKR->FindLine("RPI:","Reprocessor volume");
		if(lineindex==-1){cout<<"ERROR::Reprocessor::Reprocessor:: could not fine \"Reprocessor volume\" line in "<<filename<<endl;return;}
		dummydouble=KKR->Get_ListOfNumbers(lineindex+1);
		if(KKR->Get_ListOfNumbers__arraylength!=1){cout<<"ERROR::Reprocessor::Reprocessor:: Wrong format of \"Reprocessor volume\" "<<endl;return;}
		ChemistryPerDay_liters=dummydouble[0];
		lineindex=KKR->FindLine("RPI:","Reprocessor amount");
		if(lineindex==-1){cout<<"ERROR::Reprocessor::Reprocessor:: could not fine \"Reprocessor amount\" line in "<<filename<<endl;return;}
		int lineitr=lineindex+1;
		for(int i=0;i<110;i++)Chemistry[i]=0.;
		while(true){
			dummydouble=KKR->Get_ListOfNumbers(lineitr);
			if(KKR->Get_ListOfNumbers__arraylength<2)break;
			Zdummy=dummydouble[0];
			fracdummy=dummydouble[1];
			if(Zdummy<1||Zdummy>109){cout<<"ERROR::Reprocessor::Reprocessor:: Invalid Z in \"Reprocessor amount\""<<endl;return;}
			Chemistry[Zdummy]=fracdummy;
			lineitr++;
		}
		if(dowritestatus)cout<<"Loading helium bubbeler parameters..."<<endl; 
		lineindex=KKR->FindLine("RPI:","Helium Bubbeler");
		if(lineindex==-1){cout<<"ERROR::Reprocessor::Reprocessor:: could not fine \"Helium Bubbeler\" line in "<<filename<<endl;return;}
		lineitr=lineindex+1;
		for(int i=0;i<110;i++)Bubbeler[i]=0.;
		while(true){
			dummydouble=KKR->Get_ListOfNumbers(lineitr);
			if(KKR->Get_ListOfNumbers__arraylength<2)break;
			Zdummy=dummydouble[0];
			fracdummy=dummydouble[1];
			if(Zdummy<1||Zdummy>109){cout<<"ERROR::Reprocessor::Reprocessor:: Invalid Z in \"Helium Bubbeler\""<<endl;return;}
			Bubbeler[Zdummy]=fracdummy;
			lineitr++;
		}
	}
	~Reprocessor(){

		if(KKR)delete KKR;
		delete log;
	}
	
	double RunReprocessor(int whichstep, FullBurnupRun *BR, double WantedKeff, double InputMass=0, int fuelsaltid=1001, int modid=2000, char *file=0, bool doprint=false){
		char strdummy[1000];
		if(!BR->GetStepPtr(whichstep)){cout<<"ERROR::Reprocessor::RunReprocessor:: Step does not exist!"<<endl;return 0;}
		Material *Fuel=BR->GetMaterialPtr(fuelsaltid,whichstep);
		Material *Mod=BR->GetMaterialPtr(modid,whichstep);
		if(!Fuel||!Mod){cout<<"ERROR::Reprocessor::RunReprocessor:: Moderator or fuel does not exist!"<<endl;return 0;}



		// variables for later use:
		double Li6frac=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetAtomFracIsotope(3006)/BR->GetMaterialPtr(fuelsaltid,whichstep)->GetAtomFracAllIsotopes(3);
		double ModAbs=BR->GetMaterialPtr(modid,whichstep)->GetNeutronsAbsorbedAll();
		double FuelAbs=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsAbsorbedAll();
		double LiFAbs=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsAbsorbedAll(3,3)
			+BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsAbsorbedAll(9,9);
		double FpAbs=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsAbsorbedAll(1,89)
			-LiFAbs;
		double AcAbs=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsAbsorbedAll(90,109);
		double TotalAbs=FuelAbs+ModAbs;

		double ModCre=BR->GetMaterialPtr(modid,whichstep)->GetNeutronsCreatedAll();
		double FuelCre=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsCreatedAll();
		double LiFCre=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsCreatedAll(3,3)
			+BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsCreatedAll(9,9);
		double FpCre=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsCreatedAll(1,89)
			-LiFCre;
		double AcCre=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsCreatedAll(90,109);
		double TotalCre=FuelCre+ModCre;


		double TotalMass=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(1,109);
		double density=TotalMass/BR->GetMaterialPtr(fuelsaltid,whichstep)->GetVolume();
		double leak=BR->GetCalculatedLeakageFactor(whichstep,fuelsaltid);
		double AcInv=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(90,109);
		double UInv=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(92,92);
		double ThInv=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(90,90);
		double PuInv=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(94,94);
		double FInv=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(9,9);
		double LiInv=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(3,3);
		double LiEnr=1;
		if((BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassIsotope(3007)+BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassIsotope(3006)))
			LiEnr=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassIsotope(3007)/(BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassIsotope(3007)+BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassIsotope(3006));
		double FPInv=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(1,89)-FInv-LiInv;

		double Li6frac0=BR->GetMaterialPtr(fuelsaltid,0)->GetAtomFracIsotope(3006)/BR->GetMaterialPtr(fuelsaltid,0)->GetAtomFracAllIsotopes(3);
		double TotalMass0=BR->GetMaterialPtr(fuelsaltid,0)->GetMassAllIsotopes(1,109);
		double density0=TotalMass/BR->GetMaterialPtr(fuelsaltid,0)->GetVolume();
		double leak0=BR->GetCalculatedLeakageFactor(0,fuelsaltid);
		double AcInv0=BR->GetMaterialPtr(fuelsaltid,0)->GetMassAllIsotopes(90,109);
		double UInv0=BR->GetMaterialPtr(fuelsaltid,0)->GetMassAllIsotopes(92,92);
		double ThInv0=BR->GetMaterialPtr(fuelsaltid,0)->GetMassAllIsotopes(90,90);
		double PuInv0=BR->GetMaterialPtr(fuelsaltid,0)->GetMassAllIsotopes(94,94);
		double FInv0=BR->GetMaterialPtr(fuelsaltid,0)->GetMassAllIsotopes(9,9);
		double LiInv0=BR->GetMaterialPtr(fuelsaltid,0)->GetMassAllIsotopes(3,3);
		double LiEnr0=1;
		if((BR->GetMaterialPtr(fuelsaltid,0)->GetMassIsotope(3007)+BR->GetMaterialPtr(fuelsaltid,0)->GetMassIsotope(3006)))
			LiEnr0=BR->GetMaterialPtr(fuelsaltid,0)->GetMassIsotope(3007)/(BR->GetMaterialPtr(fuelsaltid,0)->GetMassIsotope(3007)+BR->GetMaterialPtr(fuelsaltid,0)->GetMassIsotope(3006));
		double FPInv0=BR->GetMaterialPtr(fuelsaltid,0)->GetMassAllIsotopes(1,89)-FInv0-LiInv0;

		double ModAbs0=BR->GetMaterialPtr(modid,0)->GetNeutronsAbsorbedAll();
		double FuelAbs0=BR->GetMaterialPtr(fuelsaltid,0)->GetNeutronsAbsorbedAll();
		double LiFAbs0=BR->GetMaterialPtr(fuelsaltid,0)->GetNeutronsAbsorbedAll(3,3)
			+BR->GetMaterialPtr(fuelsaltid,0)->GetNeutronsAbsorbedAll(9,9);
		double FpAbs0=BR->GetMaterialPtr(fuelsaltid,0)->GetNeutronsAbsorbedAll(1,89)
			-LiFAbs0;
		double AcAbs0=BR->GetMaterialPtr(fuelsaltid,0)->GetNeutronsAbsorbedAll(90,109);
		double TotalAbs0=FuelAbs0+ModAbs0;

		double ModCre0=BR->GetMaterialPtr(modid,0)->GetNeutronsCreatedAll();
		double FuelCre0=BR->GetMaterialPtr(fuelsaltid,0)->GetNeutronsCreatedAll();
		double LiFCre0=BR->GetMaterialPtr(fuelsaltid,0)->GetNeutronsCreatedAll(3,3)
			+BR->GetMaterialPtr(fuelsaltid,0)->GetNeutronsCreatedAll(9,9);
		double FpCre0=BR->GetMaterialPtr(fuelsaltid,0)->GetNeutronsCreatedAll(1,89)
			-LiFCre0;
		double AcCre0=BR->GetMaterialPtr(fuelsaltid,0)->GetNeutronsCreatedAll(90,109);
		double TotalCre0=FuelCre0+ModCre0;


		double Keff_0=BR->PredictKeff(leak,Fuel);


		//normalizing
		BR->GetMaterialPtr(fuelsaltid,whichstep)->NormalizeMaterialFractions();

		// Running routine;
		// Bubbler
		this->RunBubbler(whichstep,BR,fuelsaltid,doprint);
		// Chemestry
		this->RunChemistry(whichstep,BR,fuelsaltid,doprint);

		//normalizing
		BR->GetMaterialPtr(fuelsaltid,whichstep)->NormalizeMaterialFractions();


		// Salt corrector itr 1
		this->CorrectCarrierSaltConcentration(whichstep,BR,fuelsaltid,doprint);


		if(!InputMass)
			InputMass=BR->GetMaterialPtr(fuelsaltid,0)->GetMass();
		// refueler irt 1

		if(this-RunRefueler(whichstep,BR,leak,WantedKeff,InputMass,fuelsaltid,doprint)==0){
			sprintf(strdummy,"ERROR:: Something is wrong in the refueler - fix it! Ending...");log->WriteLine(strdummy,doprint);
			return 0;
		}



		// renormalizing 
		BR->GetMaterialPtr(fuelsaltid,whichstep)->NormalizeMaterialFractions();
		/*
		// refueler irt 2
		if(this-RunRefueler(whichstep,BR,leak,WantedKeff,InputMass,fuelsaltid,doprint)==0){
			sprintf(strdummy,"ERROR:: Something is wrong in the refueler - fix it! Ending...");log->WriteLine(strdummy,doprint);
			return 0;
		}
		// renormalizing 
		BR->GetMaterialPtr(fuelsaltid,whichstep)->NormalizeMaterialFractions();
		*/


		double ModAbs1=BR->GetMaterialPtr(modid,whichstep)->GetNeutronsAbsorbedAll();
		double FuelAbs1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsAbsorbedAll();
		double LiFAbs1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsAbsorbedAll(3,3)
			+BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsAbsorbedAll(9,9);
		double FpAbs1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsAbsorbedAll(1,89)
			-LiFAbs1;
		double AcAbs1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsAbsorbedAll(90,109);
		double TotalAbs1=FuelAbs1+ModAbs1;

		double ModCre1=BR->GetMaterialPtr(modid,whichstep)->GetNeutronsCreatedAll();
		double FuelCre1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsCreatedAll();
		double LiFCre1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsCreatedAll(3,3)
			+BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsCreatedAll(9,9);
		double FpCre1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsCreatedAll(1,89)
			-LiFCre1;
		double AcCre1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetNeutronsCreatedAll(90,109);
		double TotalCre1=FuelCre1+ModCre1;

		double TotalMass1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(1,109);
		double density1=TotalMass/BR->GetMaterialPtr(fuelsaltid,whichstep)->GetVolume();
		double AcInv1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(90,109);
		double UInv1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(92,92);
		double ThInv1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(90,90);
		double PuInv1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(94,94);
		double FInv1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(9,9);
		double LiInv1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(3,3);
		double LiEnr1=1;
		if((BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassIsotope(3007)+BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassIsotope(3006)))
			LiEnr1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassIsotope(3007)/(BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassIsotope(3007)+BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassIsotope(3006));
		double FPInv1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetMassAllIsotopes(1,89)-FInv1-LiInv1;
		double Li6frac1=BR->GetMaterialPtr(fuelsaltid,whichstep)->GetAtomFracIsotope(3006)/BR->GetMaterialPtr(fuelsaltid,whichstep)->GetAtomFracAllIsotopes(3);
		

		sprintf(strdummy,"||||||||||||||||||||||||||||||||||||||||||");log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"||||||||||||Burn step summary:||||||||||||");log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"||||||||||||||||||||||||||||||||||||||||||");log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Step Duration: %f days. Step Power: %f MW. Step Burnup: %f GWd/tHM"
			,BR->GetStepPtr(whichstep)->GetTime_sincet0_days()
			,BR->GetStepPtr(whichstep)->GetPower()
			,BR->GetStepPtr(whichstep)->GetBurnup());log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Keff at t0 was: %f at t=%f days it was %f - which is a change of %f."
			,BR->GetStepPtr(0)->GetKeff()
			,BR->GetStepPtr(whichstep)->GetTime_sincet0_days()
			,BR->GetStepPtr(whichstep)->GetKeff()
			,BR->GetStepPtr(whichstep)->GetKeff()-BR->GetStepPtr(0)->GetKeff());log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Neutron losses, creation and Keff:");log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"                 \tt=0 \tt=%f days\t after reprocessor",BR->GetStepPtr(whichstep)->GetTime_sincet0_days());log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Leakage:         \t%f\t%f\tNA"
			,leak0
			,leak);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Moderaotor abs.: \t%f\t%f\t%f"
			,ModAbs0/TotalAbs0
			,ModAbs/TotalAbs
			,ModAbs1/TotalAbs1);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Fiss. prod. abs.:\t%f\t%f\t%f"
			,FpAbs0/TotalAbs0
			,FpAbs/TotalAbs
			,FpAbs1/TotalAbs1);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Carry salt abs.: \t%f\t%f\t%f"
			,LiFAbs0/TotalAbs0
			,LiFAbs/TotalAbs
			,LiFAbs1/TotalAbs1);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Neutron Prod.:   \t%f\t%f\t%f"
			,TotalCre0/TotalAbs0
			,TotalCre/TotalAbs
			,TotalCre1/TotalAbs1);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Predicted Keff:  \t%f\t%f\t%f"
			,BR->PredictKeff(leak0,BR->GetMaterialPtr(fuelsaltid,0))
			,Keff_0
			,BR->PredictKeff(leak,BR->GetMaterialPtr(fuelsaltid,whichstep)));log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Keff:            \t%f\t%f\tNA"
			,BR->GetStepPtr(0)->GetKeff()
			,BR->GetStepPtr(whichstep)->GetKeff());log->WriteLine(strdummy,doprint);
		
		sprintf(strdummy,"|||||||||||Inventory inforamtion||||||||||");log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Average daily changes [g/day]: Annihilated\tCreated\tRefueled");log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"FP: %f\t\t%f\t\t%f"
			,TotalMass0/1000.
			,TotalMass/1000.
			,TotalMass1/1000.);log->WriteLine(strdummy,doprint);


		sprintf(strdummy,"Inventory [kg]      \tt=0 \tt=%f days\t after reprocessor",BR->GetStepPtr(whichstep)->GetTime_sincet0_days());log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Total mass:         \t%f\t%f\t%f"
			,TotalMass0/1000.
			,TotalMass/1000.
			,TotalMass1/1000.);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Actinide mass:      \t%f\t%f\t%f"
			,AcInv0/1000.
			,AcInv/1000.
			,AcInv1/1000.);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Th mass:            \t%f\t%f\t%f"
			,ThInv0/1000.
			,ThInv/1000.
			,ThInv1/1000.);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"U mass:             \t%f\t%f\t%f"
			,UInv0/1000.
			,UInv/1000.
			,UInv1/1000.);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Pu mass:            \t%f\t%f\t%f"
			,PuInv0/1000.
			,PuInv/1000.
			,PuInv1/1000.);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"FP mass:            \t%f\t%f\t%f"
			,FPInv0/1000.
			,FPInv/1000.
			,FPInv1/1000.);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"F mass:            \t%f\t%f\t%f"
			,FInv0/1000.
			,FInv/1000.
			,FInv1/1000.);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Li mass:            \t%f\t%f\t%f"
			,LiInv0/1000.
			,LiInv/1000.
			,LiInv1/1000.);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Li6 richness:       \t%f%%\t%f%%\t%f%%"
			,Li6frac0*100.
			,Li6frac*100.
			,Li6frac1*100.);log->WriteLine(strdummy,doprint);

		sprintf(strdummy,"The actinide vector (percent) \tt=0 \tt=%f days\t after reprocessor",BR->GetStepPtr(whichstep)->GetTime_sincet0_days());log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Th:      \t%f\t%f\t%f"
			,100*ThInv0/AcInv0
			,100*ThInv/AcInv
			,100*ThInv1/AcInv1);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"U:      \t%f\t%f\t%f"
			,100*UInv0/AcInv0
			,100*UInv/AcInv
			,100*UInv1/AcInv1);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Pu:      \t%f\t%f\t%f"
			,100*(AcInv0-ThInv0-UInv0)/AcInv0
			,100*(AcInv-ThInv-UInv)/AcInv
			,100*(AcInv1-ThInv1-UInv1)/AcInv1);log->WriteLine(strdummy,doprint);
		
		sprintf(strdummy,"Add and removed by the reprocessor...");log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Th:         %f kg",(ThInv1-ThInv)/1000.);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Fresh fuel: %f kg",((AcInv1-ThInv1)-(AcInv-ThInv))/1000.);log->WriteLine(strdummy,doprint);
		sprintf(strdummy," - here of: %f kg Pu",(PuInv1-PuInv)/1000.);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"fiss. prod: %f kg",(FPInv1-FPInv)/1000.);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Flouride:   %f kg",(FInv1-FInv)/1000.);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Lithium:    %f kg",(LiInv1-LiInv)/1000.);log->WriteLine(strdummy,doprint);

//		sprintf(strdummy,"Neutron economy diagram",(LiInv1-LiInv)/1000.);log->WriteLine(strdummy,doprint);
//		sprintf(strdummy,"TH231 <- ",(LiInv1-LiInv)/1000.);log->WriteLine(strdummy,doprint);

		sprintf(strdummy,"Summary of Actinide isotopes");log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"isotope\tMass\ta frac\tm frac\tX-s\tNperN");log->WriteLine(strdummy,doprint);
		Isotope *I=0;
		double totalAbs=Fuel->GetNeutronsAbsorbedAll();
		for(int z=90;z<110;z++){
			for(int i=0;i<20;i++){
				I=Fuel->GetIsotopePtr(z,i);
				if(!I)continue;
				if(!I->GetAtomfrac())continue;
			sprintf(strdummy,"%d\t\t%.5e\t\t%.5e\t\t%.5e\t\t%.5e\t\t%.5e"
				,I->GetZAID()
				,Fuel->GetMassIsotope(I->GetZAID())
				,I->GetAtomfrac()
				,I->GetMassfrac()
				,I->GetNeutronsAbsorbed()/totalAbs
				,I->GetNeutronsPerNeutron());log->WriteLine(strdummy,doprint);
			}
		}
		EconomyCalculator *EC=new EconomyCalculator(Fuel,leak);
		EC->PrintThCycle(log);
		//		element		xs		decay		fiss		n, 	n2n		n3n		n,p		n,alpha


		//		31				32				33					34					35
		//		TH	(n,y)->


		//			-	
		//
		//		Th232->(n,y)
		//				
		//
		//
		//
		//Th					7%fiss->90%->	.1%fiss->0.1%	->	Th
		//						1.9				1.9					
		//										Beta99%
		//Pa									5%fiss->N,Y10%
		//										Beta
		//									
		//U										93%fiss


		if(file)log->WriteLogfile(file);
	}
	
	void RunBubbler(int whichstep, FullBurnupRun *BR, int fuelsaltid=1001, bool doprint=false){
		// removes 100% of the elements listed in the bubbeler list in the reprocessor file and returns ammount removed (<0).
		char strdummy[1000];
		double TotalRemovedMass=0;
		BurnupStep *step=0;
		Material *fuel=0;
		if(BR->GetStepPtr(whichstep))
			step=BR->GetStepPtr(whichstep);
		if(BR->GetMaterialPtr(fuelsaltid,whichstep))
			fuel=BR->GetMaterialPtr(fuelsaltid,whichstep);
		if(!step||!fuel){cout<<"ERROR::Reprocessor::RunBubbler:: Material or step did not exist"<<endl;return;}
		double duration=step->GetTime_sincet0_days();
		sprintf(strdummy,"Helium bubbler running online for %f days\0",duration);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Element (Z) - mass removed (g)\0");log->WriteLine(strdummy,doprint);

		double remove=0;
		for(int i=1;i<110;i++){
			if(Bubbeler[i]&&fuel->GetMassElement(i)){
				remove=1-Bubbeler[i];
				double massremoved=(1-remove)*fuel->GetMassElement(i);
				TotalRemovedMass+=massremoved;
				sprintf(strdummy,"%d  %f \0",i,massremoved);log->WriteLine(strdummy,doprint);
				fuel->ScaleAllIsotopes(remove,i,i);
			}
		}
		sprintf(strdummy,"This correspond to a removal of %.5e grams per day\0",TotalRemovedMass/duration);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"\0");log->WriteLine(strdummy,doprint);
		fuel->SetMass(fuel->GetMass()-TotalRemovedMass);
		return ;
	}

	void RunChemistry(int whichstep, FullBurnupRun *BR, int fuelsaltid=1001, bool doprint=false){
		// Runs iteratively through time per day and removes FPs and adds according to FP production, then returns removed mass (<0).
		char strdummy[1000];
		double TotalRemovedMass=0;
		BurnupStep *step=0;
		Material *fuel=0;
		BurnupStep *step0=0;
		Material *fuel0=0;
		if(BR->GetStepPtr(whichstep))
			step=BR->GetStepPtr(whichstep);
		if(BR->GetMaterialPtr(fuelsaltid,whichstep))
			fuel=BR->GetMaterialPtr(fuelsaltid,whichstep);
		if(BR->GetStepPtr(0))
			step0=BR->GetStepPtr(0);
		if(BR->GetMaterialPtr(fuelsaltid,0))
			fuel0=BR->GetMaterialPtr(fuelsaltid,0);
		if(!step||!fuel||!step0||!fuel0){cout<<"ERROR::Reprocessor::RunChemistry:: Material or step did not exist (step 0 required)"<<endl;return;}
		double duration=step->GetTime_sincet0_days();
		sprintf(strdummy,"Flame reactor running online for %f days\0",duration);log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"NOTE:: Flame reactor Assumes constant creation rate throughout step\0");log->WriteLine(strdummy,doprint);
		sprintf(strdummy,"Element (Z) - mass at t0 - mass before reprocessor - mass after reprocessor -  mass removed (g)\0");log->WriteLine(strdummy,doprint);
		for(int i=1;i<110;i++){
			if(Chemistry[i]>0&&fuel->GetMassElement(i)){
				double remove=0;
				double currentmass=0;
				double removemass=0;
				double mass0=0;
				double mass=fuel->GetMassElement(i);
				if(fuel0->GetMassElement(i))mass0=fuel0->GetMassElement(i);
				currentmass=mass0;
				double massproduction_per_day=(fuel->GetMassElement(i)-currentmass)/duration;
				double massremoved=0;
				for(int j=0;j<duration;j++){
					removemass=currentmass*Chemistry[i]*this->ChemistryPerDay_liters*1e3/fuel->GetVolume();
					massremoved+=removemass;
					currentmass-=removemass;
					currentmass+=massproduction_per_day;
				}
				fuel->ScaleAllIsotopes(currentmass/mass,i,i);
				TotalRemovedMass+=massremoved;
				sprintf(strdummy,"%d  %f  %f  %f  %f \0",i,mass0,mass,currentmass,massremoved);log->WriteLine(strdummy,doprint);

			}
		}
		sprintf(strdummy,"Average mass removed %f g/day, total mass removed %f g\0",TotalRemovedMass/duration,TotalRemovedMass);log->WriteLine(strdummy,doprint);
		sprintf(strdummy," \0");log->WriteLine(strdummy,doprint);
		fuel->SetMass(fuel->GetMass()-TotalRemovedMass);
	}
	
	void CorrectCarrierSaltConcentration(int whichstep, FullBurnupRun *BR ,int fuelsaltid=1001, bool doprint=false){
		// looks at the LiF to AcF4 ratios and refuels with Li anf F, then returns the refueled mass (<0 if mass was removed)
		char strdummy[1000];
		//LiMat->GetAtomFracIsotope(3007);
		sprintf(strdummy,"Initializing carrier salt balancer...");log->WriteLine(strdummy,doprint);
		if((LiMat->GetAtomFracIsotope(3007)+LiMat->GetAtomFracIsotope(3006)))
			sprintf(strdummy,"Corecting Li (using %f Li7) and F fractions to match Ac fractions",LiMat->GetAtomFracIsotope(3007)/(LiMat->GetAtomFracIsotope(3007)+LiMat->GetAtomFracIsotope(3006)));log->WriteLine(strdummy,doprint);

		double TotalMassAdded=0;
		BurnupStep *step=0;
		Material *fuel=0;
		if(BR->GetStepPtr(whichstep))
			step=BR->GetStepPtr(whichstep);
		if(BR->GetMaterialPtr(fuelsaltid,whichstep))
			fuel=BR->GetMaterialPtr(fuelsaltid,whichstep);
		if(!step||!fuel){cout<<"ERROR::Reprocessor::Chemistry:: Material or step did not exist (step 0 required)"<<endl;return;}


		double AcFrac=fuel->GetAtomFracAllIsotopes(90,109);
		double LiFrac=fuel->GetAtomFracAllIsotopes(3,3);
		double FFrac=fuel->GetAtomFracAllIsotopes(9,9);
		cout<<"LiFrac "<<LiFrac<<endl;
		cout<<"AcFrac "<<AcFrac<<endl;
		double LiFracNeeded=(.78/.22*AcFrac);
		double FFracNeeded=(.78/.22*AcFrac+4.*AcFrac);
		cout<<"LiFracNeeded "<<LiFracNeeded<<endl;

		double mass0=fuel->GetMass();
		double Fmasstoadd=(FFracNeeded-FFrac)/LiFrac*mass0*fuel->GetMassFracAllIsotopes(9,9);
		double Limasstoadd=(LiFracNeeded-LiFrac)/LiFrac*mass0*fuel->GetMassFracAllIsotopes(3,3);

		if(Fmasstoadd>0){
			sprintf(strdummy,"Adding %f g F",Fmasstoadd);log->WriteLine(strdummy,doprint);
			fuel->AddMass(9019,Fmasstoadd,false,doprint);
		}
		if(Fmasstoadd<0){
			sprintf(strdummy,"Removing %f g F",Fmasstoadd);log->WriteLine(strdummy,doprint);
			fuel->AddMass(9019,Fmasstoadd,false,doprint);
		}
		

		cout<<"Limasstoadd "<<Limasstoadd<<endl;
		if(Limasstoadd>0){
			sprintf(strdummy,"Adding %f g Li",Limasstoadd);log->WriteLine(strdummy,doprint);
			fuel->AddMass(LiMat,Limasstoadd,false,doprint);
		}
		if(Limasstoadd<0){
			sprintf(strdummy,"Removing %f g Li",Limasstoadd);log->WriteLine(strdummy,doprint);
			double totalLifrac=fuel->GetMassFracIsotope(3006)+fuel->GetMassFracIsotope(3007);
			cout<<" That is: "<<Limasstoadd*fuel->GetMassFracIsotope(3006)/totalLifrac<<" g of Li6 and "
				<<Limasstoadd*fuel->GetMassFracIsotope(3007)/totalLifrac<<" g og Li7"<<endl;
				fuel->AddMass(3006,Limasstoadd*fuel->GetMassFracIsotope(3006)/totalLifrac,false,doprint);
				fuel->AddMass(3007,Limasstoadd*fuel->GetMassFracIsotope(3007)/totalLifrac,false,doprint);
		}

	}

	int RunRefueler(int whichstep, FullBurnupRun *BR, double leakage, double wantedkeff, double wantedtotalmass, int fuelsaltid=1001, bool doprint=false){
		char strdummy[1000];
		double TotalRemovedMass=0;
		BurnupStep *step=0;
		Material *fuel=0;
		Material *mod=0;
		if(BR->GetStepPtr(whichstep))
			step=BR->GetStepPtr(whichstep);
		if(BR->GetMaterialPtr(fuelsaltid,whichstep))
			fuel=BR->GetMaterialPtr(fuelsaltid,whichstep);
		if(!step||!fuel){cout<<"ERROR::Reprocessor::RunRefueler:: Material or step did not exist (step 0 required)"<<endl;return 0;}

		sprintf(strdummy,"Initiating refuel procedure: Keff before refueling is predicted to be: %f - refueler aims for %f\0",BR->PredictKeff(leakage,fuel),wantedkeff);log->WriteLine(strdummy,doprint);

		double Keff0=BR->PredictKeff(leakage,fuel);
		double masschange=wantedtotalmass-fuel->GetMass();
		// ----------------------------
		// initializing first iteration
		// ----------------------------
		
		int lastcase=-1;
		if(masschange==0&&(Keff0>wantedkeff*.999&&Keff0<wantedkeff*1.001)){
			// Case Null: trivial case
			sprintf(strdummy,"Fuel mass and keff looks ok (keff with 1 permil of asked value) - ending refuel procedure.\0");log->WriteLine(strdummy,doprint);
			return 1;
		}else if(masschange==0){
			sprintf(strdummy,"Fuel mass looks ok, continueing to iterator...\0");log->WriteLine(strdummy,doprint);
			lastcase=0;
			// case 0: just continue...
		} else if(masschange>0&&Keff0<wantedkeff){
			// case 1: add fuel 
			sprintf(strdummy,"Case 1: Trying to add %f g of fresh fuel \0",masschange);log->WriteLine(strdummy,doprint);
			AddFreshFuel(fuel,masschange,doprint);
			lastcase=1;
		}else if(masschange<0&&Keff0<wantedkeff){
			// case 2: remove Th 
			sprintf(strdummy,"Case 2: Trying to remove %f g of Th \0",masschange);log->WriteLine(strdummy,doprint);
			RemoveFuelTh(fuel,masschange,doprint);
			lastcase=2;
		}else if(masschange>0&&Keff0>wantedkeff){
			//case 3: add thorium
			sprintf(strdummy,"Case 3: Trying to add %f g of Th \0",masschange);log->WriteLine(strdummy,doprint);
			AddTh232(fuel,masschange,doprint);
			lastcase=3;
		}else if(masschange<0&&Keff0>wantedkeff){
			//case 4: remove actinides
			sprintf(strdummy,"Case 4: Trying to remove %f g of Transuranics (plus Pa) \0",masschange);log->WriteLine(strdummy,doprint);
			RemoveTransuranics(fuel,masschange,doprint);
			lastcase=4;
		}else if(masschange<0&&Keff0==wantedkeff){
			// case 5: remove Th and actinides
			sprintf(strdummy,"ERROR:: Case 5: does not know how to handle this case \0");log->WriteLine(strdummy,doprint);
			cout<<"ERROR::Reprocessor::RunRefueler:: cannot handle this case 5.."<<endl;
			return 0;
		}else if(masschange>0&&Keff0==wantedkeff){
			//case 6: adding thorium and Pu 
			sprintf(strdummy,"ERROR:: Case 6: does not know how to handle this case \0");log->WriteLine(strdummy,doprint);
			cout<<"ERROR::Reprocessor::RunRefueler:: cannot handle this case 6.."<<endl;
			return 0;
		} else {
			// case -1: the non-existing case
			cout<<"ERROR::Reprocessor::RunRefueler:: this case does not exist.."<<endl;
			sprintf(strdummy,"ERROR:: Case -1: case is non-existent \0");log->WriteLine(strdummy,doprint);
			return 0;
		}
		
		// ----------------------------
		// initializing secund iteration round
		// ----------------------------
		double Keff1=BR->PredictKeff(leakage,fuel);
		if(Keff0>wantedkeff*.999&&Keff0<wantedkeff*1.001){ // withine 1 percent of the wished Keff
			sprintf(strdummy,"Case 2: You are withine 1 permil of the wanted Keff at keff=%f, refueler ending... \0",Keff1);log->WriteLine(strdummy,doprint);	
			return 1;
		} else if(Keff1<wantedkeff){
			// the straight forward case
			if(lastcase==1||lastcase==2||lastcase==0){
				double trytoadd=this->Refueling_StartingAmmount;
				for(int itr=0;itr<this->Refueling_Iterations;itr++){
					AddFreshFuel(fuel,trytoadd,doprint);
					RemoveFuelTh(fuel,-trytoadd,doprint);
					double Keff2=BR->PredictKeff(leakage,fuel);
					AddFreshFuel(fuel,-trytoadd,doprint);
					RemoveFuelTh(fuel,trytoadd,doprint);
					if(Keff1!=Keff2)
						trytoadd*=(((Keff1-wantedkeff)/(Keff1-Keff2)));
					else trytoadd=(trytoadd+1)*100;
				}
				AddFreshFuel(fuel,trytoadd,doprint);
				RemoveFuelTh(fuel,-trytoadd,doprint);
				double Keff3=BR->PredictKeff(leakage,fuel);
				sprintf(strdummy,"After %d iterations Keff converged to %f with %f g of transuranics removed and %f g of Th232 added\0"
					,Refueling_Iterations,Keff3,(lastcase==1)*masschange+trytoadd,(lastcase==2)*-masschange+trytoadd);log->WriteLine(strdummy,doprint);
				return 1;
			} else {
				// the inbetween case
				sprintf(strdummy,"ERROR:: This subcase is untested(1)\0");log->WriteLine(strdummy,doprint);
				cout<<"ERROR::Reprocessor::RunRefueler:: This subcase is untested(1)"<<endl;
				return 0;
				// this is the function which sould be implemented
				double trytoadd=masschange/2.;
				for(int itr=0;itr<this->Refueling_Iterations;itr++){
					double Keff2;
					if(lastcase==3){
						AddFreshFuel(fuel,trytoadd,doprint);
						AddTh232(fuel,-trytoadd,doprint);
						Keff2=BR->PredictKeff(leakage,fuel);
						AddFreshFuel(fuel,-trytoadd,doprint);
						AddTh232(fuel,trytoadd,doprint);
					}else if(lastcase==4){
						RemoveTransuranics(fuel,trytoadd,doprint);
						RemoveFuelTh(fuel,-trytoadd,doprint);
						Keff2=BR->PredictKeff(leakage,fuel);
						RemoveTransuranics(fuel,-trytoadd,doprint);
						RemoveFuelTh(fuel,trytoadd,doprint);
					} else {
						sprintf(strdummy,"ERROR:: Unknown subcase (1)\0");log->WriteLine(strdummy,doprint);
						cout<<"ERROR::Reprocessor::RunRefueler:: Unknown subcase (1).."<<endl;
						return 0;
					}
					if(Keff1!=Keff2)
						trytoadd*=(((Keff1-wantedkeff)/(Keff1-Keff2)));
					else trytoadd=(trytoadd+1)*100;
					if(trytoadd>masschange)trytoadd=masschange;
				}
				AddFreshFuel(fuel,trytoadd,doprint);
				RemoveFuelTh(fuel,-trytoadd,doprint);
				double Keff3=BR->PredictKeff(leakage,fuel);
				if(lastcase==3)
					sprintf(strdummy,"After %d iterations Keff converged to %f with %f g of Fresh Fuel added and %f g of Th232 added\0"
						,Refueling_Iterations,Keff3,(lastcase==1)*masschange+trytoadd,(lastcase==2)*-masschange+trytoadd);
				if(lastcase==4)
					sprintf(strdummy,"After %d iterations Keff converged to %f with %f g of transuranics removed and %f g of fuel grade Th removed\0"
						,Refueling_Iterations,Keff3,(lastcase==1)*masschange+trytoadd,(lastcase==2)*-masschange+trytoadd);
				log->WriteLine(strdummy,doprint);
				return 1;
			} 
		} else if(Keff1>wantedkeff){
			// the straight forward case
			if(lastcase==3||lastcase==4||lastcase==0){
				double trytoadd=this->Refueling_StartingAmmount;
				for(int itr=0;itr<this->Refueling_Iterations;itr++){
					AddTh232(fuel,trytoadd,doprint);
					RemoveTransuranics(fuel,-trytoadd,doprint);
					double Keff2=BR->PredictKeff(leakage,fuel);
					AddTh232(fuel,-trytoadd,doprint);
					RemoveTransuranics(fuel,trytoadd,doprint);
					if(Keff1!=Keff2)
						trytoadd*=(((Keff1-wantedkeff)/(Keff1-Keff2)));
//					else trytoadd=(trytoadd+1)*100;
				}
				AddTh232(fuel,trytoadd,doprint);
				RemoveTransuranics(fuel,-trytoadd,doprint);
				double Keff3=BR->PredictKeff(leakage,fuel);
				sprintf(strdummy,"After %d iterations Keff converged to %f with %f g of transuranics removed and %f g of Th232 added\0"
					,Refueling_Iterations,Keff3,(lastcase==4)*-masschange+trytoadd,(lastcase==3)*masschange+trytoadd);log->WriteLine(strdummy,doprint);
				return 1;
			} else {
			// the inbetween case
				// add Th and remove fresh fuel.
				double trytoadd=masschange/2.;
				for(int itr=0;itr<this->Refueling_Iterations;itr++){
					AddTh232(fuel,trytoadd,doprint);
					AddFreshFuel(fuel,-trytoadd,doprint);
					double Keff2=BR->PredictKeff(leakage,fuel);
					AddTh232(fuel,-trytoadd,doprint);
					AddFreshFuel(fuel,trytoadd,doprint);
					if(Keff1!=Keff2)
						trytoadd*=(((Keff1-wantedkeff)/(Keff1-Keff2)));
					if(trytoadd>masschange||trytoadd<0)trytoadd=masschange/2.;
				}
				AddTh232(fuel,trytoadd,doprint);
				AddFreshFuel(fuel,-trytoadd,doprint);
				double Keff3=BR->PredictKeff(leakage,fuel);
				sprintf(strdummy,"After %d iterations Keff converged to %f with %f g fresh fuel added and %f g of Th232 added\0"
					,Refueling_Iterations,Keff3,-masschange+trytoadd,masschange+trytoadd);log->WriteLine(strdummy,doprint);
				return 1;
			} 
		} else {
			// case -1: the non-existing case
			cout<<"ERROR::Reprocessor::RunRefueler:: this case does not exist (2).."<<endl;
			sprintf(strdummy,"ERROR:: Case -1: case is non-existent (2) \0");log->WriteLine(strdummy,doprint);
			return 0;
		}
		cout<<"ERROR::Reprocessor::RunRefueler:: this function should never reach its end.."<<endl;
		sprintf(strdummy,"ERROR:: this function should never reach its end \0");log->WriteLine(strdummy,doprint);
		return 0;

	}
	void AddFreshFuel(Material *fuel,double masschange,bool doprint){
			fuel->AddMass(RefuelMat,masschange,true,true,doprint);
	}
	void RemoveFuelTh(Material *fuel,double masschange,bool doprint){
		for(int i=0;i<20;i++){
			Isotope *I=fuel->GetIsotopePtr(90,i);
			if(I){
				if(I->GetAtomfrac()){
					fuel->AddMass(I->GetZAID(),masschange*I->GetMassfrac()/fuel->GetMassFracAllIsotopes(90,90),false,doprint);
				}
			}
		}
		fuel->NormalizeMaterialFractions();
	}
	void AddTh232(Material *fuel,double masschange,bool doprint){
		fuel->AddMass(90232,masschange,true,doprint);
	}
	void RemoveTransuranics(Material *fuel,double masschange,bool doprint){
		double TrMassfrac=fuel->GetMassFracAllIsotopes(91,91)+fuel->GetMassFracAllIsotopes(93,109);
		double syncronizablemass=0;
		for(int z=91;z<110;z++){
			if(z==92)continue;
			for(int i=0;i<20;i++){
				Isotope *I=fuel->GetIsotopePtr(z,i);
				if(I){
					if(I->GetAtomfrac()){
						fuel->AddMass(I->GetZAID(),masschange*I->GetMassfrac()/TrMassfrac,false,doprint);
					}
				}
			}
		}
		fuel->NormalizeMaterialFractions();
	}

};




class DeckMatReplacer{
public:
	Logbook *Log;
	DeckMatReplacer(Material *Mat,KcodeReader *Deck,char *filename,bool doprintstatus=false){
		Log=new Logbook();
		char dummy[10];
		int matline=0;
		sprintf(dummy,"m%d\0",Mat->GetMat_id());
		while(true){
			matline=Deck->FindLine(dummy,matline);
			if(matline>=0)
				if(Deck->FindLine(dummy,(double)matline)==0)break;
			if(matline==-1){cout<<"ERROR::DeckMatReplacer::DeckMatReplacer:: could not find material"<<endl;return;}
		}
		for(int i=0;i<matline;i++)
			Log->WriteLine(Deck->GetLine(i),doprintstatus);
		Mat->PrintMaterial(Log,doprintstatus);
		matline++;
		while(true){
			char *chardummy=Deck->GetLine(matline);
			if(chardummy){
				if(chardummy[0]==32||chardummy[0]==67||chardummy[0]==99){
					matline++;
					continue;
				}
			}
			break;
		}
		for(int i=matline;i<Deck->LastLineIndex;i++)
			Log->WriteLine(Deck->GetLine(i),doprintstatus);
		Log->WriteLogfile(filename);
	}
	

};
