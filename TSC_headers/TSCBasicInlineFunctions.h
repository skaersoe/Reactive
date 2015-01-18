#pragma once
#include <iostream>
#include "TTree.h"
#include "TFile.h"
#include <math.h>
#include "TMath.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "THnSparse.h"
#include "TGaxis.h"
#include "TLatex.h"
#include "TText.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TRandom2.h"
#include <time.h>
#include "TSCMathFunctions.h"

using namespace std;
/*
class DB_controler{
public :
	static bool DB_isActive;

};
 bool DB_isActive=false;


class DB{
	char ThisDbName[50];
public :
	DB(char *name=__func__){
		if(!DB_controler::DB_isActive)return;
		if(name){
			sprintf(ThisDbName,"%s",name);
		} else {
			sprintf(ThisDbName,"NoName");
		}
	}
};
*/

	
class DataReader{
public :
	double *__Global_DataReader_dataline;
	char ReadChar[100];
	bool commaispoint;
	FILE *infile;
	int LineSeperator;
	int RowSeperator;
	DataReader(char *filename){
		commaispoint=true;
		LineSeperator=9;
		RowSeperator=10;
		__Global_DataReader_dataline=new double[128];
		if(!(infile=fopen(filename,"r")))cout<<"Could not open file"<<filename<<endl;
		cout<<"DataReader constructed, file "<<filename<<" was loaded."<<
			endl<<" - Initially values will be seperated by tab, and rows by enter."<<
			endl<<" -  - modefy LineSeperator and RowSeperator to change this"<<endl;
	}
	~DataReader(){
		fclose(infile);
	}

	double *readline(){
		int d_index=0;
		int colindex=0;
		while(!feof(infile)){
			if(d_index>32){cout<<"ERROR - DataReader::readline - Exeptionally lond data entry: nothing read";return 0;}
			ReadChar[d_index]=fgetc(infile);
			if(commaispoint)if(ReadChar[d_index]==44)ReadChar[d_index]=46;
			if(ReadChar[d_index]==LineSeperator){
				ReadChar[d_index]=NULL;
				d_index=-1;
				__Global_DataReader_dataline[colindex]=strtod(ReadChar,NULL);
				colindex++;
			} else if(ReadChar[d_index]==RowSeperator||feof(infile)){
				if(ReadChar[0]==RowSeperator)ReadChar[d_index]=0;
				d_index=0;
				__Global_DataReader_dataline[colindex]=strtod(ReadChar,NULL);
				return __Global_DataReader_dataline;
			}
			d_index++;
		}
	}
};
class SSW{
private:
	static char RX[150];
	static char RY[150];
	static char RZ[150];
	static char RWX[150];
	static char RWY[150];
	static char RWZ[150];
public:
	static double history,id,x,y,z,wx,wy,k,energy,time,weight;

	static double i;
	static double JGP;
	static double JC;
	static double IPT;
	static double surface;
		
	static double wz;
	static double lambda;
	static double l;
	static double e;
	static double loge;
	static double logl;
	static double ModeratorHight;
	static double rsqr;
	static double radtodeg;
	static double r;
	static double xsign;
	static double ysign;
	static double theta;
	static double ycenter;
	static double BigRsqr;
	static double BigR;
	static double phi;
	static double wr;
	static double wxsign;
	static double wtheta0;
	static double wthetaDiff;
	static double wtheta;
	static double wphi;
	static void Rotation1Axis(double deg,char axis /*"x" "y","z"*/);
	static void ResetRotation();
	static void SetSSWAliases(TTree *T);
	static double ci(double id){
		return TMath::Nint(TMath::Abs(id/1E+6));
	}

	static double cJGP(double id){return -TMath::Nint(ci(id)/200.0);}
	static double cJC(double id){return TMath::Nint(ci(id)/100.0) + 2*cJGP(id);}
	static double cIPT(double id){return ci(id)-100*cJC(id)+200*cJGP(id);}
	static double csurface(double id){return TMath::Abs(id)-1000000;}
	
	static double cwz(double wx,double wy,double id){return TMath::Sqrt(TMath::Max((double)0, (double)1-wx*wx-wy*wy)) * (-2.*(id<0)+1.);}

	static double clambda(double energy){return 0.000286299/sqrt(energy);}
	static double cl(double energy){return 0.000286299/sqrt(energy);}
	static double ce(double energy){return energy;}
	static double cloge(double energy){return log10(energy*1.);}
	static double clogl(double energy){return log10(clambda(energy)*1.);}
	static double cr(double rx,double rz){return sqrt(rx*rx+rz*rz);}
	static double cxsign(double rx){return (-2.*(rx<0)+1.);}
	static double cysign(double ry){return (-2.*(ry<0)+1.);}
	static double ctheta(double rx,double rz){return (rx<0)*360.+cxsign(rx)*(acos(rz/cr(rx,rz))*SSW::radtodeg);}
	static double cBigR(double rx,double ry,double rz){return sqrt(rx*rx+ry*ry+rz*rz);}
	static double cphi(double rx,double ry,double rz){return asin(ry/cBigR(rx,ry,rz))*SSW::radtodeg;}
	static double cwr(double rwx,double rwz){return sqrt(rwx*rwx+rwz*rwz);}
	static double cwxsign(double rwx){return (-2.*(rwx<0)+1.);}
	static double cwtheta0(double rwx,double rwz){return (rwx<0)*360.+cwxsign(rwx)*(acos(rwz/cwr(rwx,rwz))*SSW::radtodeg);}
	static double cwtheta(double rx,double rz,double rwx, double rwz){
		double wthetaDiff=(cwtheta0(rwx,rwz)-ctheta(rx,rz));
		return	(wthetaDiff<-180)*360-(wthetaDiff>180)*360+wthetaDiff;
	}
	static double cwphi(double rwy){return asin(rwy)*radtodeg;}

	static void LoadNeutron(double inhistory,double inid,double inx,double iny,double inz,double inwx,double inwy,double ink,double inenergy,double intime,double inweight);

};
class TSC {
public :
static unsigned int __GlobalUnameInteger;
static char __GlobalunameCharArray[1000];
//static char __GlobalCHARCharArray[1000];
static double TSCTitlesize;
static double TSCLablesize;
static double TSCTitleOffset;
static double TSCyTitlesize;
static double TSCyLablesize;
static double TSCyTitleOffset;
static double TSCNamesize;
static double TSCNameOffset;
static double TSCLinewidth;




static void Set_LENS_variables(TTree *T){
	T->SetAlias("C8w","V2L(5.26/C8/1e-6)");
	T->SetAlias("C8e","V2E(5.26/C8/1e-6)");
	T->SetAlias("C8k","V2K(5.26/C8/1e-6)");
	T->SetAlias("C8lk","log10(V2K(5.26/C8/1e-6))");
	T->SetAlias("C8lw","log10(V2L(5.26/C8/1e-6))");
	T->SetAlias("C8le","log10(V2E(5.26/C8/1e-6))");
	T->SetAlias("C12w","V2L(5.3/C12/1e-6)");
	T->SetAlias("C12e","V2E(5.3/C12/1e-6)");
	T->SetAlias("C12k","V2K(5.3/C12/1e-6)");
	T->SetAlias("C12lk","log10(V2K(5.3/C12/1e-6))");
	T->SetAlias("C12lw","log10(V2L(5.3/C12/1e-6))");
	T->SetAlias("C12le","log10(V2E(5.3/C12/1e-6))");
	T->SetAlias("V","5.26/C8/1e-6");
	T->SetAlias("DetectorEff","4.5e-4");
	T->SetAlias("DetectorEffC8","4.5e-4/C8w");
}

static int isNumberFromChar(char *input){
	int i=0;
	while((int)input[i]<48||(int)input[i]>57){
		if((int)input[i]==0||input[i]==13||input[i]==10)
			return -1;
/*			if((int)input[i]<47)
				if((int)input[i+1]>=48&&(int)input[i+1]<=57)
					break;
					*/
			i++;
	}
	//i--;
	if(i>0)
		if(input[i-1]==46)i--;
	if(i>0)
		if(input[i-1]==45)i--;
	return i;
}

static double GetNumberFromChar(char *input){
	int i=isNumberFromChar(input);
	if(i>=0){
		string a=input+i;
		//cout<<a<<endl;
		return (double)atof(a.c_str());
	}
	cout<<"WARNING::TSC::GetNumberFromChar, this array does not contain a number: returning 0"<<endl;
	return 0;
}

static double AvgIntegral(TH1D *h1,double a, double b,bool printresult=false){
		int lowbin=0;double lowlimit;
		int highbin=0;double highlimit;
		for(int k=0;k<h1->GetNbinsX()+2;k++){
			if(h1->GetBinLowEdge(k)<a){lowbin=k+1;lowlimit=h1->GetBinLowEdge(k+1);}
			if(h1->GetBinLowEdge(k)<b){highbin=k;highlimit=h1->GetBinLowEdge(k+1);}
		}
		if(printresult)cout<<lowbin<<" to "<<highbin<<"  ---  "<<lowlimit<<" to "<<highlimit<<"  ::  "<<h1->Integral(lowbin,highbin,"width")/(highlimit-lowlimit)<<endl;
		return h1->Integral(lowbin,highbin,"width")/(highlimit-lowlimit);
}
static char *CHAR(const char *c1,const char *c2=0, const char *c3=0, const char *c4=0, const char *c5=0, const char *c6=0, const char *c7=0){
	int i=0;
	int j;
	for(j=0;j<1000;j++)if(c1[j]==0){i+=j;break;}
	if(c2)for(j=0;j<1000;j++)if(c2[j]==0){i+=j;break;}
	if(c3)for(j=0;j<1000;j++)if(c3[j]==0){i+=j;break;}
	if(c4)for(j=0;j<1000;j++)if(c4[j]==0){i+=j;break;}
	if(c5)for(j=0;j<1000;j++)if(c5[j]==0){i+=j;break;}
	if(c6)for(j=0;j<1000;j++)if(c6[j]==0){i+=j;break;}
	if(c7)for(j=0;j<1000;j++)if(c7[j]==0){i+=j;break;}
	char *CharArray=new char[i+1];
	CharArray[i]=0;
	sprintf(CharArray,"%s\0",c1);
if(c2)sprintf(CharArray,"%s%s\0",c1,c2);
if(c3)sprintf(CharArray,"%s%s\0",CharArray,c3);
if(c4)sprintf(CharArray,"%s%s\0",CharArray,c4);
if(c5)sprintf(CharArray,"%s%s\0",CharArray,c5);
if(c6)sprintf(CharArray,"%s%s\0",CharArray,c6);
if(c7)sprintf(CharArray,"%s%s\0",CharArray,c7);
sprintf(CharArray,"%s\0",CharArray);
return CharArray;
}
static char *uname(){
	__GlobalUnameInteger++;
	sprintf(__GlobalunameCharArray,"uname%d\0",__GlobalUnameInteger);
	return __GlobalunameCharArray;
}
static char *uname(int dummynumber){
	sprintf(__GlobalunameCharArray,"uname_dummy%d\0",dummynumber);
	return __GlobalunameCharArray;
}

static void DrawFrameBox(double x1,double y1,double x2, double y2,int color=4,double width=0,int style=4){
	TLine *l1=new TLine(x1,y1,x2,y1);
	TLine *l2=new TLine(x1,y2,x2,y2);
	TLine *l3=new TLine(x1,y1,x1,y2);
	TLine *l4=new TLine(x2,y1,x2,y2);
	l1->SetLineColor(color);
	l2->SetLineColor(color);
	l3->SetLineColor(color);
	l4->SetLineColor(color);
	if(width!=0){
		l1->SetLineWidth(width);
		l2->SetLineWidth(width);
		l3->SetLineWidth(width);
		l4->SetLineWidth(width);
	}
	if(style!=0){
	l1->SetLineStyle(style);
	l2->SetLineStyle(style);
	l3->SetLineStyle(style);
	l4->SetLineStyle(style);
	}
	l1->Draw();
	l2->Draw();
	l3->Draw();
	l4->Draw();

}
static void NormalizeByBinSize_Reverse(TH1D *h){
	for(int i=1;i<h->GetNbinsX()+1;i++){
		h->SetBinContent(i,h->GetBinContent(i)*h->GetBinWidth(i));
	}
}
static void NormalizeByBinSize_Reverse(TH2D *h,bool renormalizeX=1,bool renormalizeY=1){
	if(renormalizeX||renormalizeY){
		for(int i=1;i<h->GetNbinsX()+1;i++){
			for(int j=1;j<h->GetNbinsY()+1;j++){
				if(renormalizeX)h->SetBinContent(i,j,h->GetBinContent(i,j)*h->GetXaxis()->GetBinWidth(i));
				if(renormalizeY)h->SetBinContent(i,j,h->GetBinContent(i,j)*h->GetYaxis()->GetBinWidth(j));
			}
		}
	} else {cout<<"Warining - TSC::NormalizeByBinSize no axis normalized!"<<endl;}
}
static void NormalizeByBinSize(TH1D *h){
//	h->Scale(1.,"widht");
//	return;
	for(int i=1;i<h->GetNbinsX()+1;i++){
		h->SetBinContent(i,h->GetBinContent(i)/(h->GetBinLowEdge(i+1)-h->GetBinLowEdge(i)));
		if(h->GetSumw2N())h->SetBinError(i,h->GetBinError(i)/(h->GetBinLowEdge(i+1)-h->GetBinLowEdge(i)));
	}
}
static void NormalizeByBinSize(TH2D *h,bool renormalizeX=1,bool renormalizeY=1){
	if(renormalizeX||renormalizeY){
		for(int i=1;i<h->GetNbinsX()+1;i++){
			for(int j=1;j<h->GetNbinsY()+1;j++){
				if(renormalizeX)h->SetBinContent(i,j,h->GetBinContent(i,j)/h->GetXaxis()->GetBinWidth(i));
				if(renormalizeY)h->SetBinContent(i,j,h->GetBinContent(i,j)/h->GetYaxis()->GetBinWidth(j));
				if(renormalizeX&&h->GetSumw2N()>0)h->SetBinError(i,j,h->GetBinError(i,j)/h->GetXaxis()->GetBinWidth(i));
				if(renormalizeY&&h->GetSumw2N())h->SetBinError(i,j,h->GetBinError(i,j)/h->GetYaxis()->GetBinWidth(j));
			}
		}
	} else {cout<<"Warining - TSC::NormalizeByBinSize no axis normalized!"<<endl;}
}
static void NormalizeByBinSize(TH1 *h,bool renormalizeX=1,bool renormalizeY=1){
	cout<<h->GetNbinsX()<<"  "<<h->GetNbinsY()<<endl;
	if(h->GetNbinsY()>1){
		NormalizeByBinSize((TH2D*)h,renormalizeX,renormalizeY);
	} else {
		NormalizeByBinSize((TH1D*)h);
	}
}
static void LambdaOnLogEnergy(TCanvas *incanvas, char *name=0, int writtenrootlength=0){
	incanvas->Update();
	double ymax=incanvas->GetUymax();
	double ymin=incanvas->GetUymin();
	double xmax=pow(10,incanvas->GetUxmax());
	double xmin=pow(10,incanvas->GetUxmin());
	if(!incanvas->GetLogx()){
		xmax=incanvas->GetUxmax();
		xmin=incanvas->GetUxmin();
	}
	if(incanvas->GetLogy()){
		ymax=pow(10,incanvas->GetUymax());
		ymin=pow(10,incanvas->GetUymin());
	}
//	cout<<ymax<<endl;
	double smalloffset=.2*(ymax-ymin)/16.;
	double medoffset=.3*(ymax-ymin)/16.;
	double bigoffset=.5*(ymax-ymin)/16.;
	if(incanvas->GetLogy()){
		smalloffset=.2*(incanvas->GetUxmax()-incanvas->GetUxmin())/16.*ymax;
		medoffset=.2*(incanvas->GetUxmax()-incanvas->GetUxmin())/16.*ymax;
		bigoffset=.5*(incanvas->GetUxmax()-incanvas->GetUxmin())/16.*ymax;
	}
cout<<smalloffset<<endl;
	
	double ofsethere;
	double textoffset=(log10(xmax)-log10(xmin))/120.;
	


	TLine *l0[10];
	TLine *l1[10];
	TLine *l2[10];
	TLine *l3[10];
	TLine *l4;

	char textchar[10];

	TText *t0[10];
	TText *t1[10];
	TText *t2[10];
	TText *t3[10];
	TText *t4;
	double lasttext=0;
	for(int i=1;i<10;i++){
		if(i==1){ofsethere=bigoffset;}else if(i==5){ofsethere=medoffset;}else{ofsethere=smalloffset;}
		if(L2E(i*.1)>xmin&&L2E(i*.1)<xmax){
			l0[i]=new TLine(L2E(i*.1),ymax,L2E(i*.1),ymax-ofsethere);
			l0[i]->Draw();
			sprintf(textchar,".%i\0",i);
			t0[i] = new TText(L2E(i*.1),ymax+smalloffset,textchar);
			lasttext=L2E(i*.1);
			t0[i]->SetTextSize(.03);
		} else {t0[i]=NULL;}
		if(L2E(i*1)>xmin&&L2E(i*1)<xmax){
			l1[i]=new TLine(L2E(i*1),ymax,L2E(i*1),ymax-ofsethere);
			l1[i]->Draw();
			sprintf(textchar,"%i\0",i);
			t1[i] = new TText(L2E(i*1),ymax+smalloffset,textchar);
			t1[i]->SetTextSize(.03);
		} else {t1[i]=NULL;} 
		if(L2E(i*10)>xmin&&L2E(i*10)<xmax){
			l2[i]=new TLine(L2E(i*10),ymax,L2E(i*10),ymax-ofsethere);
			l2[i]->Draw();
			sprintf(textchar,"%i\0",i*10);
			t2[i] = new TText(L2E(i*10),ymax+smalloffset,textchar);
			t2[i]->SetTextSize(.03);
		} else {t2[i]=NULL;}
		if(L2E(i*100)>xmin&&L2E(i*100)<xmax){
			l3[i]=new TLine(L2E(i*100),ymax,L2E(i*100),ymax-ofsethere);
			l3[i]->Draw();
			sprintf(textchar,"%i\0",i*100);
			t3[i] = new TText(L2E(i*1e-7),ymax+smalloffset,textchar);
			t3[i]->SetTextSize(.03);
		} else {t3[i]=NULL;}
	}
	if(L2E(1000)>xmin&&L2E(1000)<xmax){
		l4=new TLine(L2E(1000),ymax,L2E(1000),ymax-bigoffset);
		l4->Draw();
		t4 = new TText(L2E(1000),ymax+smalloffset,"1000");
		t4->SetTextSize(.03);
	} else {t4=NULL;}
	
	if(t0[1]!=NULL){t0[1]->SetTextAlign(21);t0[1]->Draw();}
	if(t1[1]!=NULL){t1[1]->SetTextAlign(21);t1[1]->Draw();}
	if(t2[1]!=NULL){t2[1]->SetTextAlign(21);t2[1]->Draw();}
	if(t3[1]!=NULL){t3[1]->SetTextAlign(21);t3[1]->Draw();}
	if(t0[5]!=NULL){t0[5]->SetTextAlign(21);t0[5]->Draw();}
	if(t1[5]!=NULL){t1[5]->SetTextAlign(21);t1[5]->Draw();}
	if(t2[5]!=NULL){t2[5]->SetTextAlign(21);t2[5]->Draw();}
	if(t3[5]!=NULL){t3[5]->SetTextAlign(21);t3[5]->Draw();}
	if(t0[2]!=NULL){t0[2]->SetTextAlign(21);t0[2]->Draw();}
	if(t1[2]!=NULL){t1[2]->SetTextAlign(21);t1[2]->Draw();}
	if(t2[2]!=NULL){t2[2]->SetTextAlign(21);t2[2]->Draw();}
	if(t3[2]!=NULL){t3[2]->SetTextAlign(21);t3[2]->Draw();}

	if(name){
		TLatex *labletext;
		if(L2E(0.01)<xmax){
			labletext=new TLatex(L2E(0.01),ymax+bigoffset,name);
		} else {
			labletext=new TLatex(xmax/*-length*1.6*textoffset*/,ymax+bigoffset,name);
		}
		labletext->SetTextAlign(31);
		labletext->SetTextSize(.03);
		labletext->Draw();
	//	cout<<name<<endl;
	}
}
static void EnergyOnLambda(TCanvas *incanvas, char *name=0, int writtenrootlength=0){
	incanvas->Update();
	double ymax=incanvas->GetUymax();
	double ymin=incanvas->GetUymin();
	double xmax=incanvas->GetUxmax();
	double xmin=incanvas->GetUxmin();
	double smalloffset=.2*(ymax-ymin)/16.;
	double medoffset=.3*(ymax-ymin)/16.;
	double bigoffset=.5*(ymax-ymin)/16.;
	double ofsethere;
	double textoffset=(xmax-xmin)/120.;

	TLine *l0[10];
	TLine *l1[10];
	TLine *l2[10];
	TLine *l3[10];
	TLine *l4;

	char textchar[10];

	TText *t0[10];
	TText *t1[10];
	TText *t2[10];
	TText *t3[10];
	TText *t4;

	for(int i=1;i<10;i++){
		if(i==1){ofsethere=bigoffset;}else if(i==5){ofsethere=medoffset;}else{ofsethere=smalloffset;}
		if(E2L(i*1e-10)>xmin&&E2L(i*1e-10)<xmax){
			l0[i]=new TLine(E2L(i*1e-10),ymax,E2L(i*1e-10),ymax-ofsethere);
			l0[i]->Draw();
			sprintf(textchar,".%i\0",i);
			t0[i] = new TText(E2L(i*1e-10),ymax+smalloffset,textchar);
			t0[i]->SetTextSize(.03);
		} else {t0[i]=NULL;}
		if(E2L(i*1e-9)>xmin&&E2L(i*1e-9)<xmax){
			l1[i]=new TLine(E2L(i*1e-9),ymax,E2L(i*1e-9),ymax-ofsethere);
			l1[i]->Draw();
			sprintf(textchar,"%i\0",i);
			t1[i] = new TText(E2L(i*1e-9),ymax+smalloffset,textchar);
			t1[i]->SetTextSize(.03);
		} else {t1[i]=NULL;} 
		if(E2L(i*1e-8)>xmin&&E2L(i*1e-8)<xmax){
			l2[i]=new TLine(E2L(i*1e-8),ymax,E2L(i*1e-8),ymax-ofsethere);
			l2[i]->Draw();
			sprintf(textchar,"%i\0",i*10);
			t2[i] = new TText(E2L(i*1e-8),ymax+smalloffset,textchar);
			t2[i]->SetTextSize(.03);
		} else {t2[i]=NULL;}
		if(E2L(i*1e-7)>xmin&&E2L(i*1e-7)<xmax){
			l3[i]=new TLine(E2L(i*1e-7),ymax,E2L(i*1e-7),ymax-ofsethere);
			l3[i]->Draw();
			sprintf(textchar,"%i\0",i*100);
			t3[i] = new TText(E2L(i*1e-7),ymax+smalloffset,textchar);
			t3[i]->SetTextSize(.03);
		} else {t3[i]=NULL;}
	}
	if(E2L(1000e-9)>xmin&&E2L(1000e-9)<xmax){
		l4=new TLine(E2L(1000e-9),ymax,E2L(1000e-9),ymax-bigoffset);
		l4->Draw();
		t4 = new TText(E2L(1e-6),ymax+smalloffset,"1000");
		t4->SetTextSize(.03);
	} else {t4=NULL;}
	
	if(t0[1]!=NULL){t0[1]->SetTextAlign(31);t0[1]->Draw();}
	if(t1[1]!=NULL){t1[1]->SetTextAlign(31);t1[1]->Draw();}
	if(t2[1]!=NULL){t2[1]->SetTextAlign(31);t2[1]->Draw();}
	if(t3[1]!=NULL){t3[1]->SetTextAlign(31);t3[1]->Draw();}

	if(t4)if(t4->GetX()+textoffset*6.5<t3[1]->GetX()){t4->SetTextAlign(31);t4->Draw();}
	int last0=1;
	int last1=1;
	int last2=1;
	int last3=1;
	for(int i=2;i<10;i++){
		if(t0[i]){
			if(t0[last0]){
				if(!t1[1]){
					if(t0[i]->GetX()+textoffset*3.5<t0[last0]->GetX()){
						t0[i]->SetTextAlign(31);t0[i]->Draw();last0=i;
					}
				} else if(t0[i]->GetX()+textoffset*3.5<t0[last0]->GetX()&&t0[i]->GetX()-textoffset*3.5>t1[1]->GetX()){
					t0[i]->SetTextAlign(31);t0[i]->Draw();last0=i;
				}
			} else {
				t0[i]->SetTextAlign(31);t0[i]->Draw();last0=i;
			}
		}
		if(t1[i]){
			if(t1[last1]){
				if(!t2[1]){
					if(t1[i]->GetX()+textoffset*3.5<t1[last1]->GetX()){
						t1[i]->SetTextAlign(31);t1[i]->Draw();last1=i;
					}
				} else if(t1[i]->GetX()+textoffset*3.5<t1[last1]->GetX()&&t1[i]->GetX()-textoffset*4.5>t2[1]->GetX()){
					t1[i]->SetTextAlign(31);t1[i]->Draw();last1=i;
				}
			} else {t1[i]->SetTextAlign(31);t1[i]->Draw();last1=i;}
		}
		if(t2[i]){
			if(t2[last2]){
				if(!t3[1]){
					if(t2[i]->GetX()+textoffset*4.5<t2[last2]->GetX()){
						t2[i]->SetTextAlign(31);t2[i]->Draw();last2=i;
					}
				} else if(t2[i]->GetX()+textoffset*4.5<t2[last2]->GetX()&&t2[i]->GetX()-textoffset*5.5>t3[1]->GetX()){
					t2[i]->SetTextAlign(31);t2[i]->Draw();last2=i;
				}
			} else {t2[i]->SetTextAlign(31);t2[i]->Draw();last2=i;}
		}
		if(t3[i]){
			if(t3[last3]){
				if(!t4){
					if(t3[i]->GetX()+textoffset*5.5<t3[last3]->GetX()){
						t3[i]->SetTextAlign(31);t3[i]->Draw();last3=i;
					}
				}else if(t3[i]->GetX()+textoffset*5.5<t3[last3]->GetX()&&t3[i]->GetX()-textoffset*6.5>t4->GetX()){
					t3[i]->SetTextAlign(31);t3[i]->Draw();last3=i;
				}
			} else {t3[i]->SetTextAlign(31);t3[i]->Draw();last3=i;}
		}
	}
	if(name){

		TLatex *labletext = new TLatex(xmax/*-length*1.6*textoffset*/,ymax+bigoffset,name);
		labletext->SetTextAlign(31);
		labletext->SetTextSize(.03);
		labletext->Draw();
		cout<<name<<endl;
	}
}
static void Log2Human(TAxis *axis){
	int bins = axis->GetNbins();
	double *new_bins = new double[bins + 1];
	for (int i=1;i<bins+2;i++){
		new_bins[i-1] = pow(10,axis->GetBinLowEdge(i));
	}
	axis->Set(bins, new_bins);
	delete new_bins;
}
static void Human2Log(TAxis *axis){
	int bins = axis->GetNbins();
	double *new_bins = new double[bins + 1];
	for (int i=1;i<bins+2;i++){
		new_bins[i-1] = log10(axis->GetBinLowEdge(i));
	}
	axis->Set(bins, new_bins);
	delete new_bins;
}
static void Log2Human(TH1D *h1){
	Log2Human(h1->GetXaxis());
}
static void Human2Log(TH1D *h1){
	Human2Log(h1->GetXaxis());
}
/*
static TH1D *E2L1d(TH1D *h1, bool doNormalizeAfterTransform=false){

	int bins = h1->GetNbinsX();
	//cout<<h1->GetBinLowEdge(2)<<endl;
	if(h1->GetBinLowEdge(1)<0){
		cout<<"WARNING TSC::E2L1d_ArbEBins Histogram is in log notation (-10,-9,-8...). The input histogram ("<<h1->GetName()<<") will be converted"<<endl;
		//Human2Log(h1);
	}
	bool NumberOfOddBins=false;
	double *new_bins = new double[bins+1];
	for (int i=2;i<bins+2;i++){
		//cout<<h1->GetBinLowEdge(bins-i+2)<<endl;
		if(h1->GetBinLowEdge(bins-i+2)<=0){
			//cout<<i<<"<<<<"<<h1->GetBinLowEdge(bins-i+2)<<endl;
			if(bins-i+2==0){cout<<"ERROR: TSC::E2L1d_ArbEBins: A bin ("<<bins-i+2<<" of "<<bins<<") was 0, null pointer returned"<<endl;return 0;}
			cout<<"WARNING: TSC::E2L1d_ArbEBins: Last bin upper edge is infinite, and will be interpreted as the overflow bin."<<endl;
			NumberOfOddBins=true;
		} else {
			new_bins[i-1]=E2L(h1->GetBinLowEdge(bins-i+2));
		}
	}
	new_bins[0]=0;
	if(NumberOfOddBins)cout<<"<<<<<"<<bins--<<endl;
	//for (int i=0;i<bins+1;i++)cout<<i<<"  "<<new_bins[i]<<endl;

	TH1D *h2=new TH1D(TSC::uname(),"",bins,new_bins);
	if(h1->GetSumw2N())h2->Sumw2();
	for (int i=0;i<bins+2;i++){
		h2->SetBinContent(i,h1->GetBinContent(bins+1-i));
		if(h1->GetSumw2N())h2->SetBinError(i,h1->GetBinError(bins+1-i));
	}
	if(doNormalizeAfterTransform)TSC::NormalizeByBinSize(h2);
	delete new_bins;
	return h2;
}*/
static TH1D *E2L1d(TH1D *h1, bool NormalizeAfterTransform=false, bool DeNormalizeBeforeTransform=false){
	if(DeNormalizeBeforeTransform)TSC::NormalizeByBinSize_Reverse(h1);
	int bins = h1->GetNbinsX();
	//cout<<h1->GetBinLowEdge(2)<<endl;
	if(h1->GetBinLowEdge(1)<0){
		cout<<"WARNING TSC::E2L1d Histogram is in log notation (-10,-9,-8...). The input histogram ("<<h1->GetName()<<") will be converted"<<endl;
		Log2Human(h1);
	}
	bool NumberOfOddBins=false;
	double *new_bins = new double[bins+1];
	for (int i=1;i<bins+2;i++){
		if(h1->GetBinLowEdge(bins-i+2)<=0){
			if(bins-i+2==0){cout<<"ERROR: TSC::E2L1d: A bin ("<<bins-i+2<<" of "<<bins<<") was strange, null pointer returned"<<endl;return 0;}
			cout<<"WARNING: TSC::E2L1d: Last bin upper edge is infinite, and will be interpreted as the overflow bin."<<endl;
			NumberOfOddBins=true;
		} else {
			new_bins[i-1]=E2L(h1->GetBinLowEdge(bins-i+2));
		}
	}
	new_bins[0]=0;
	if(NumberOfOddBins)bins=bins-1;


	TH1D *h2=new TH1D(TSC::uname(),"",bins,new_bins);
	if(h1->GetSumw2N())h2->Sumw2();
	for (int i=0;i<bins+2;i++){
//		cout<<i<<"  "<<h1->GetBinCenter(i)<<"  "<<h1->GetBinContent(i)<<endl;
//		h2->Fill(L2E(h1->GetBinCenter(i)),h1->GetBinContent(i));
//		if(h1->GetSumw2N())h2->SetBinError(i,h1->GetBinError(bins+1-i));
		h2->SetBinContent(i,h1->GetBinContent(bins+NumberOfOddBins+1-i));
		if(h1->GetSumw2N())h2->SetBinError(i,h1->GetBinError(bins+NumberOfOddBins+1-i));
	}
	if(NormalizeAfterTransform)TSC::NormalizeByBinSize(h2);
	delete new_bins;
	return h2;
}

static TH1D *L2E1d(TH1D *h1, bool NormalizeAfterTransform=false, bool DeNormalizeBeforeTransform=false){
	if(DeNormalizeBeforeTransform)TSC::NormalizeByBinSize_Reverse(h1);
	int bins = h1->GetNbinsX();

	double *new_bins = new double[bins + 1];
	for (int i=1;i<bins+2;i++){
		if(h1->GetBinLowEdge(bins-i+2)<=0){new_bins[i-1]=1e9;continue;}
		new_bins[i-1] = L2E(h1->GetBinLowEdge(bins-i+2));
	}
	TH1D *h2=new TH1D(TSC::uname(),"",bins,new_bins);
	if(h1->GetSumw2N())h2->Sumw2();
	for (int i=0;i<bins+2;i++){
		h2->SetBinContent(i,h1->GetBinContent(bins+1-i));
		if(h1->GetSumw2N())h2->SetBinError(i,h1->GetBinError(bins+1-i));
	}
	if(NormalizeAfterTransform)TSC::NormalizeByBinSize(h2);
	delete new_bins;
	return h2;
}
/*
static TH1D *U2E1d(TH1D *h1){
	TH1D *h2=(TH1D*)h1->Clone(TSC::uname());
	TSC::Log2Human(h2->GetXaxis());
	TSC::NormalizeByBinSize(h2);
	return h2;
}
static TH1D *E2U1d(TH1D *h1){
	TH1D *h2=(TH1D*)h1->Clone(TSC::uname());
	TSC::NormalizeByBinSize_Reverse(h2);
	TSC::Human2Log(h2->GetXaxis());
	return h2;
}
static TH1D *E2L1d(TH1D *h1){
	return TSC::U2L1d(E2U1d(h1));
}
static TH1D *L2U1d(TH1D *h1){
	return E2U1d(L2E1d(h1));
	//return L2E1d(h1);
}
*/
static void NormalizeSlizeviseX(TH2D *h2){
	TH2D *normalizer=(TH2D*)h2->Clone("NormalizeSlizeviseX_dummy");
	normalizer->Sumw2();
	for(int x=0;x<h2->GetNbinsX()+2;x++){
		double value=0;
		double squresum=0;
		for(int y=0;y<h2->GetNbinsY()+2;y++){
			value+=h2->GetBinContent(x,y);
			squresum+=h2->GetBinContent(x,y)*h2->GetBinContent(x,y);
		}
		squresum=sqrt(squresum);
		for(int y=0;y<h2->GetNbinsY()+2;y++){
			normalizer->SetBinContent(x,y,value);
			normalizer->SetBinError(x,y,squresum);
		}
	}
	h2->Divide(normalizer);
	delete normalizer;
return;
}
static void NormalizeSlizeviseY(TH2D *h2){
	TH2D *normalizer=(TH2D*)h2->Clone("NormalizeSlizeviseX_dummy");
	normalizer->Sumw2();
	for(int y=0;y<h2->GetNbinsY()+2;y++){
		double value=0;
		double squresum=0;
		for(int x=0;x<h2->GetNbinsX()+2;x++){
			value+=h2->GetBinContent(x,y);
			squresum+=(h2->GetBinContent(x,y)*h2->GetBinContent(x,y));
		}
		squresum=sqrt(squresum);
		for(int x=0;x<h2->GetNbinsX()+2;x++){
			normalizer->SetBinContent(x,y,value);
			normalizer->SetBinError(x,y,squresum);
		}
	}
	h2->Divide(normalizer);
	delete normalizer;
return;
}
static void DrawName(TH1 *h1,char *name=0){
   if(name==0){
     h1->SetTitle(h1->GetName());
   }
   else h1->SetTitle(name);
}
static int round(double x){
   return (int)(x+0.5);
}
static double FindMaximum(TH1 *a1,TH1 *a2=0,TH1 *a3=0,TH1 *a4=0,TH1 *a5=0,TH1 *a6=0,TH1 *a7=0,TH1 *a8=0,TH1 *a9=0){
	double maximum=a1->GetMaximum();
	if(a2!=0)maximum=TMath::Max(maximum,a2->GetMaximum());
	if(a3!=0)maximum=TMath::Max(maximum,a3->GetMaximum());
	if(a4!=0)maximum=TMath::Max(maximum,a4->GetMaximum());
	if(a5!=0)maximum=TMath::Max(maximum,a5->GetMaximum());
	if(a6!=0)maximum=TMath::Max(maximum,a6->GetMaximum());
	if(a7!=0)maximum=TMath::Max(maximum,a7->GetMaximum());
	if(a8!=0)maximum=TMath::Max(maximum,a8->GetMaximum());
	if(a9!=0)maximum=TMath::Max(maximum,a9->GetMaximum());
	return maximum;
}
static double FindMinimum(TH1 *a1,TH1 *a2=0,TH1 *a3=0,TH1 *a4=0,TH1 *a5=0,TH1 *a6=0,TH1 *a7=0,TH1 *a8=0,TH1 *a9=0){
	double minimum=a1->GetMinimum(1e-38);
	if(a2!=0)minimum=min(minimum,a2->GetMinimum(1e-38));
	if(a3!=0)minimum=min(minimum,a3->GetMinimum(1e-38));
	if(a4!=0)minimum=min(minimum,a4->GetMinimum(1e-38));
	if(a5!=0)minimum=min(minimum,a5->GetMinimum(1e-38));
	if(a6!=0)minimum=min(minimum,a6->GetMinimum(1e-38));
	if(a7!=0)minimum=min(minimum,a7->GetMinimum(1e-38));
	if(a8!=0)minimum=min(minimum,a8->GetMinimum(1e-38));
	if(a9!=0)minimum=min(minimum,a9->GetMinimum(1e-38));
	return minimum;
}
static TLine *XLine(int col,double x,double ymin,double ymax){
	TLine *newline=new TLine(x,ymin,x,ymax);
	newline->SetLineColor(col);
	newline->SetLineWidth(3);
	return newline;
}
static double Integrate1d(TH1D *h1,double from, double to){
	int i=1;
	while(h1->GetBinLowEdge(i)<from){
		i++;
		if(i>h1->GetNbinsX()+2){cout<<"TSC::Integrate1d: Nothing to Integrate, will retur 0"<<endl;return 0;}
	}
	if(i==1){cout<<"TSC::Integrate1d: Lower limit outside histogram scope"<<endl;}
	int j=i;
	while(h1->GetBinLowEdge(j)+h1->GetBinWidth(j)>to){
		j++;
		if(j>h1->GetNbinsX()+1){cout<<"TSC::Integrate1d: Upper limit outside histogram scope"<<endl;break;}
	}
	cout<<"TSC::Integrate1d: Integration limmits set to: ["<<h1->GetBinLowEdge(i)<<";"<<h1->GetBinLowEdge(j)+h1->GetBinWidth(j)<<"]"<<endl;
	return h1->Integral(i,j);
}
static TH1D *GetRatio(TH1D *nom,TH1D *denom, char *newAxisName=0){
	TH1D *tmp=(TH1D*)nom->Clone();
	tmp->Divide(denom);
	if(newAxisName)tmp->GetYaxis()->SetTitle(newAxisName);
	return tmp;
}
static TH1D *PowerScale(TH1D *h1,double power){
	TH1D *tmp=(TH1D*)h1->Clone();
	for(int i=1;i<=h1->GetNbinsX();i++){
		tmp->SetBinContent(i,pow(tmp->GetBinContent(i),power));
		tmp->SetBinError(i,fabs(tmp->GetBinError(i)*power*pow(tmp->GetBinContent(i),power-1)));
	}
	return tmp;
}

static TH1D *FastDraw1d(TTree *T,char *var,char *binning=0,char *cut=0,char *opt=0,long int entries=100000000000,long int start=0,TCanvas *dumcanvas=0){
	bool setDelete=true;
	if(dumcanvas){setDelete=false;
	} else {dumcanvas=new TCanvas();
	}
	char dummyhistname[100];
	sprintf(dummyhistname,"%s",uname());
	char setopt[100]={"\0"};
	if(opt)sprintf(setopt,"%s\0",opt);
	cout<<CHAR(var,">>",dummyhistname,"(",binning,")")<<endl;
	if(binning){T->Draw(CHAR(var,">>",dummyhistname,"(",binning,")"),cut,setopt,entries,start);
	} else {T->Draw(CHAR(var,">>",dummyhistname),cut,setopt,entries,start);
	}
	TH1D *hist=(TH1D*)(gDirectory->Get(dummyhistname)->Clone());
	((TH1D*)gDirectory->Get(dummyhistname))->Delete();
	if(setDelete)delete dumcanvas;
	return hist;
}
static TH2D *FastDraw2d(TTree *T,char* var,char*binning=0,char *cut=0,char *opt=0,long int entries=100000000000,long int start=0,TCanvas *dumcanvas=0){
	bool setDelete=true;
	if(dumcanvas){setDelete=false;
	} else {dumcanvas=new TCanvas();
	}
	char setopt[100];
	sprintf(setopt,"colz\0");
	if(opt)sprintf(setopt,"colz %s\0",opt);
	char dummyhistname[100];
	sprintf(dummyhistname,"%s\0",uname());
	cout<<CHAR(var,">>",dummyhistname,"(",binning,")")<<endl;
	if(binning){T->Draw(CHAR(var,">>",dummyhistname,"(",binning,")"),cut,setopt,entries,start);
	} else {T->Draw(CHAR(var,">>",dummyhistname),cut,setopt,entries,start);
	}
	cout<<"fetched"<<endl;
	TH2D *hist=(TH2D*)(gDirectory->Get(dummyhistname)->Clone());
	((TH2D*)gDirectory->Get(dummyhistname))->Delete();
	if(setDelete)delete dumcanvas;
	return hist;
}
static TTree *TreeLoader(TFile *f,const char *file,const char *path=""){
	cout<<"Loading: "<<CHAR(path,file,".root")<<" ..."<<endl;
	if(f= new TFile(CHAR(path,file,".root")))return (TTree*)f->Get("T")->Clone(uname());
	cout<<"ERROR! <<<<<<<<<<<<<<<<<<<<LOADING FAILED<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;return 0;
}
static TTree *TreeLoader(const char *file,const char *path=""){
	cout<<"WARNING: TSC::TreeLoader is loading ("<<CHAR(path,file)<<") into memory, and it will not be deleted - this will turn messy over time!!!"<<endl;
	TFile *f;
	if(path)return TreeLoader(f,file,path);
	return (TTree*)TreeLoader(f,file);
}
static TTree *TreeLoaderSSW(TFile *f,const char *file,const char *path=""){
	TTree *T;
	if(path){
		T=TreeLoader(f,file,path);
	} else if(path){
		T=TreeLoader(f,file,path);
	}
	SSW::SetSSWAliases(T);
	return (TTree*)T;
}
static TTree *TreeLoaderSSW(const char *file,const char *path=""){
	TFile *f;
	TTree *T;
	if(path){
		T=TreeLoader(f,file,path);
	} else if(path){
		T=TreeLoader(f,file,path);
	}
	SSW::SetSSWAliases(T);
	return (TTree*)T;
}
static void NameHist(TH1 *h,int color=0,char *n=0, char *a1=0,char *a2=0,char *a3=0){
	if(n!=0)h->SetTitle(n);
	h->SetTitleSize(TSCNamesize);
	if(a1)h->GetXaxis()->SetTitle(a1);
	if(a3)h->GetZaxis()->SetTitle(a3);
	if(a2)h->GetYaxis()->SetTitle(a2);
	if(color!=0)h->SetLineColor(color);
	h->GetXaxis()->SetLabelSize(TSCLablesize);
	h->GetXaxis()->SetTitleOffset(TSCTitleOffset);
	h->GetXaxis()->SetTitleSize(TSCTitlesize);
	h->GetYaxis()->SetLabelSize(TSCyLablesize);
	h->GetYaxis()->SetTitleOffset(TSCyTitleOffset);
	h->GetYaxis()->SetTitleSize(TSCyTitlesize);
	h->SetLineWidth(TSCLinewidth);
	h->SetLineStyle(1);
}
static void NameHist(TH1D *h,int color=0,char *n=0, char *a1=0,char *a2=0,TLegend *l=0,char *hn=0,char *legopt=0){
	if(n!=0)h->SetTitle(n);
	h->SetTitleSize(TSCNamesize);
	h->SetTitleSize(TSCNamesize);
	if(a1)h->GetXaxis()->SetTitle(a1);
	if(a2)h->GetYaxis()->SetTitle(a2);
	if(color!=0)h->SetLineColor(color);
	if(l){
		if(legopt)l->AddEntry(h, hn, legopt);
		if(!legopt)l->AddEntry(h, hn, "l");
	}
	h->GetXaxis()->SetLabelSize(TSCLablesize);
	h->GetXaxis()->SetTitleOffset(TSCTitleOffset);
	h->GetXaxis()->SetTitleSize(TSCTitlesize);
	h->GetYaxis()->SetLabelSize(TSCyLablesize);
	h->GetYaxis()->SetTitleOffset(TSCyTitleOffset);
	h->GetYaxis()->SetTitleSize(TSCyTitlesize);
	h->SetLineWidth(TSCLinewidth);
	h->SetLineStyle(1);
}
static void NameHist(TH1D *h,int color,TLegend *l=0,char *hn=0,char *legopt=0){
	NameHist(h,color,0, 0,0,l,hn,legopt);
}
static void NameHist(TH2D *h,int color=0,char *n=0, char *a1=0,char *a2=0,char *a3=0,TLegend *l=0,char *hn=0,char *legopt=0){
	h->SetTitle(n);
	h->SetTitleSize(TSCNamesize);
	h->SetTitleSize(TSCNamesize);
	if(a1)h->GetXaxis()->SetTitle(a1);
	if(a2)h->GetYaxis()->SetTitle(a2);
	if(a3)h->GetZaxis()->SetTitle(a3);
	if(color!=0)h->SetLineColor(color);
	if(l){
		if(legopt)l->AddEntry(h, hn, legopt);
		if(!legopt)l->AddEntry(h, hn, "l");		
	}
	h->GetXaxis()->SetLabelSize(TSCLablesize);
	h->GetXaxis()->SetTitleOffset(TSCTitleOffset);
	h->GetXaxis()->SetTitleSize(TSCTitlesize);
	h->GetYaxis()->SetLabelSize(TSCyLablesize);
	h->GetYaxis()->SetTitleOffset(TSCyTitleOffset);
	h->GetYaxis()->SetTitleSize(TSCyTitlesize);
	if(a3!=0){
		h->GetZaxis()->SetLabelSize(TSCyLablesize);
		h->GetZaxis()->SetTitleOffset(TSCyTitleOffset);
		h->GetZaxis()->SetTitleSize(TSCyTitlesize);
	}
}
static void NameHist(TH2D *h,int color,TLegend *l=0,char *hn=0,char *legopt=0){
	NameHist(h,color,0, 0,0,0,l,hn,legopt);
}
static void NameHist(TH3D *h,int color=0,char *n=0, char *a1=0,char *a2=0,char *a3=0,TLegend *l=0,char *hn=0,char *legopt=0){
	h->SetTitle(n);
	h->SetTitleSize(TSCNamesize);
	h->SetTitleSize(TSCNamesize);
	if(a1)h->GetXaxis()->SetTitle(a1);
	if(a2)h->GetYaxis()->SetTitle(a2);
	if(a3)h->GetZaxis()->SetTitle(a3);
	if(color!=0)h->SetLineColor(color);
	if(l){
		if(legopt)l->AddEntry(h, hn, legopt);
		if(!legopt)l->AddEntry(h, hn, "l");		
	}
	h->GetXaxis()->SetLabelSize(TSCLablesize);
	h->GetXaxis()->SetTitleOffset(TSCTitleOffset);
	h->GetXaxis()->SetTitleSize(TSCTitlesize);
	h->GetYaxis()->SetLabelSize(TSCLablesize);
	h->GetYaxis()->SetTitleOffset(TSCTitleOffset);
	h->GetYaxis()->SetTitleSize(TSCTitlesize);
	h->GetZaxis()->SetLabelSize(TSCLablesize);
	h->GetZaxis()->SetTitleOffset(TSCTitleOffset);
	h->GetZaxis()->SetTitleSize(TSCTitlesize);


}
static void NameHist(TH3D *h,int color,TLegend *l=0,char *hn=0,char *legopt=0){
	NameHist(h,color,0, 0,0,0,l,hn,legopt);
}
static TH2D *MakeTH2D(char *name,int xbins,double xmin,double xmax,double ybins,double ymin,double ymax
						,char *xaxisname=0,char *yaxisname=0,char *zaxisname=0, bool sumw2=1){
	TH2D *dummy=new TH2D(name,"",xbins,xmin,xmax,ybins,ymin,ymax);
	NameHist(dummy,1,"",xaxisname,yaxisname,zaxisname);
	if(sumw2)dummy->Sumw2();
	return dummy;

}
static TH1D *MakeTH1D(char *name,int xbins,double xmin,double xmax
						,char *xaxisname=0,char *yaxisname=0, bool sumw2=1){

	TH1D *dummy=new TH1D(name,"",xbins,xmin,xmax);
	NameHist(dummy,1,"",xaxisname,yaxisname);
	dummy->Sumw2();
	return dummy;

}

static TH1D *Ratio(TH1 *h1,TH1 *h2,int color=0, char *axisname=0){
	TH1D *hout=(TH1D*)h1->Clone();
	hout->Divide(h2);
	if(color)hout->SetLineColor(color);
	if(axisname)hout->GetYaxis()->SetTitle(axisname);
	return hout;
}


static TH1 *SparseLoader(const char *file,char *NSname,int projection){
	if(projection>=0){
		return (TH1*)(((THnSparseF*)SparseLoader(file,NSname))->Projection(projection)->Clone(uname()));
	} else 
		cout<<"TSC::SparseLoader >> projection<0 not implermented yet! returning 0"<<endl;
	return 0;
}
static TH1 *SparseLoader(const char *file,char *NSname,int projectionx,int projectiony){
		return (TH1*)(((THnSparseF*)SparseLoader(file,NSname))->Projection(projectionx,projectiony)->Clone(uname()));
}

static THnSparseF *SparseLoader(const char *file,char *NSname){
	TFile *f;
	cout<<"Loading: "<<CHAR(file,".root")<<" ..."<<endl;
	if(f= new TFile(CHAR(file,".root"))){
		THnSparseF* dummy=(THnSparseF*)f->Get(NSname);
		if(!dummy)return 0;
		return dummy;
	}
	cout<<"ERROR! <<<<<<<<<<<<<<<<<<<<LOADING FAILED<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
	return 0;
}


static TH1D *SparseLoaderLambda(const char *file,char *NSname){
	TH1D *dummy=(TH1D*)SparseLoader(file,NSname,6);
	return TSC::E2L1d((TH1D*)dummy);
}


};
//initializers

//char TSC::__GlobalCHARCharArray[1000];
char TSC::__GlobalunameCharArray[1000];
unsigned int TSC::__GlobalUnameInteger=0;
double TSC::TSCTitlesize=0.045;
double TSC::TSCLablesize=0.03;
double TSC::TSCTitleOffset=0.85;
double TSC::TSCyTitlesize=0.045;
double TSC::TSCyLablesize=0.03;
double TSC::TSCyTitleOffset=1.05;
double TSC::TSCNamesize=0.1;
double TSC::TSCNameOffset=1;
double TSC::TSCLinewidth=2;

class CollimatorMaker{
private :
	void FindCenterPosition(){
		x_center=(xmax+xmin)/2.;
		y_center=(ymax+ymin)/2.;
		z_center=(zmax+zmin)/2.;
	}
	void FindDirectionVector(){
		FindCenterPosition();
		double a=x_center-f5_x;
		double b=y_center-f5_y;
		double c=z_center-f5_z;
		double R=sqrt(a*a+b*b+c*c);
		x_dirvec=a/R;
		y_dirvec=b/R;
		z_dirvec=c/R;
	}
public :
	double currentF5angle;
	double averysmallnumber;
	double xmin,ymin,zmin;
	double xmax,ymax,zmax;
	double f5_x,f5_y,f5_z;
	double F5ExclusionSphereRadius;
	int SurfaceNumber,F5DetectorNumber;
	double x_center,y_center,z_center;
	double x_dirvec,y_dirvec,z_dirvec;
	char eBins[100];

	CollimatorMaker(double _xmin, double _xmax, double _ymin, double _ymax, double _zmin, double _zmax, double _f5x, double _f5y, double _f5z,int _SurfaceNumber=0,int _F5DetectorNumber=0){
		xmin=_xmin;
		xmax=_xmax;
		ymin=_ymin;
		ymax=_ymax;
		zmin=_zmin;
		zmax=_zmax;
		f5_x=_f5x;
		f5_y=_f5y;
		f5_z=_f5z;
		SurfaceNumber=_SurfaceNumber;
		F5DetectorNumber=_F5DetectorNumber;
		SurfaceNumber=0;
		F5DetectorNumber=0;
		averysmallnumber=0.01;
		F5ExclusionSphereRadius=0.1;
		currentF5angle=0;
		sprintf(eBins,"5.e-9 20.e-9 100.e-9 1.e-6 1.e-3 1. 1.e6");
	}
	~CollimatorMaker(){}

	void SetSurfaceNumber(int inSurfaceNumber){
		SurfaceNumber=inSurfaceNumber;
	}
	void SetF5Position(double inf5_x,double inf5_y,double inf5_z,int Fnumber5=0,double inF5ro=-1){
		if(inF5ro>0)F5ExclusionSphereRadius=inF5ro;
		f5_x=inf5_x;
		f5_y=inf5_y;
		f5_z=inf5_z;
	}
	void SetMinPosition(double inxmin,double inymin,double inzmin){
		xmin=inxmin;
		ymin=inymin;
		zmin=inzmin;
	}
	void SetMaxPosition(double inxmax,double inymax,double inzmax){
		xmax=inxmax;
		ymax=inymax;
		zmax=inzmax;
	}
	void RotateF5to(double angleindeg){ // rotation around the y-axis
		RotateF5(-currentF5angle+angleindeg);
		currentF5angle=angleindeg;
	}
	void RotateF5(double angleindeg){ // rotation around the y-axis
		if(angleindeg==0)return;
		currentF5angle=currentF5angle+angleindeg;
		double oldx=f5_x;
		f5_x=f5_x*cos(angleindeg*M::deg2rad)+f5_z*sin(angleindeg*M::deg2rad);
		f5_z=f5_z*cos(angleindeg*M::deg2rad)-oldx*sin(angleindeg*M::deg2rad);
	}
	void WriteF5(int _F5DetectorNumber=0){
		if(_F5DetectorNumber)F5DetectorNumber=_F5DetectorNumber;
		if(F5DetectorNumber>0){
			cout<<"F"<<F5DetectorNumber<<"5:n ";
		} else {
			cout<<"F5:n ";
		}
		cout<<f5_x<<" "<<f5_y<<" "<<f5_z<<" "<<F5ExclusionSphereRadius<<endl;
	}
	void WriteE(int _F5DetectorNumber=0){
		if(_F5DetectorNumber)F5DetectorNumber=_F5DetectorNumber;
		if(F5DetectorNumber>0){
			cout<<"E"<<F5DetectorNumber<<"5:n";
		} else {
			cout<<"E5";
		}
		cout<<" 5e-9 20e-9 100e-9 1e-6 1e-3 1. 1e3 1e6"<<endl;
	}
	void WriteFM(int _F5DetectorNumber=0){
		if(_F5DetectorNumber)F5DetectorNumber=_F5DetectorNumber;
		if(F5DetectorNumber>0){
			cout<<"FM"<<F5DetectorNumber<<"5 ";
		} else {
			cout<<"FM5 ";
		}
		if(GetCollimationSolidAngle()==0){
			cout<<"1"<<endl;
			cout<<"c -----------------WARNING - colimator angle is 0------------------"<<endl;
		}
		cout<<1.56037734e16/GetCollimationSolidAngle()<<endl;
	}
	double GetCollimationSolidAngle(){
		FindCenterPosition();
		double H=sqrt((x_center-f5_x)*(x_center-f5_x)+(y_center-f5_y)*(y_center-f5_y)+(z_center-f5_z)*(z_center-f5_z));
		double dist_fc=sqrt((x_center-f5_x)*(x_center-f5_x)+(y_center-f5_y)*(y_center-f5_y)+(z_center-f5_z)*(z_center-f5_z));
		double dist_fymin=sqrt((x_center-f5_x)*(x_center-f5_x)+(ymin-f5_y)*(ymin-f5_y)+(z_center-f5_z)*(z_center-f5_z));
		double dist_fymax=sqrt((x_center-f5_x)*(x_center-f5_x)+(ymax-f5_y)*(ymax-f5_y)+(z_center-f5_z)*(z_center-f5_z));
		double dist_fxzmin=sqrt((xmin-f5_x)*(xmin-f5_x)+(y_center-f5_y)*(y_center-f5_y)+(zmin-f5_z)*(zmin-f5_z));
		double dist_fxzmax=sqrt((xmax-f5_x)*(xmax-f5_x)+(y_center-f5_y)*(y_center-f5_y)+(zmax-f5_z)*(zmax-f5_z));
		double dist_cxzmin=sqrt((x_center-xmin)*(x_center-xmin)+(z_center-zmin)*(z_center-zmin));
		double dist_cxzmax=sqrt((x_center-xmax)*(x_center-xmax)+(z_center-zmax)*(z_center-zmax));
		double dist_cymin=sqrt((y_center-ymin)*(y_center-ymin));
		double dist_cymax=sqrt((y_center-ymax)*(y_center-ymax));
		double cosrel_xzmin=acos((-dist_cxzmin*dist_cxzmin+dist_fxzmin*dist_fxzmin+dist_fc*dist_fc)/(2.*dist_fc*dist_fxzmin));
		double cosrel_xzmax=acos((-dist_cxzmax*dist_cxzmax+dist_fxzmax*dist_fxzmax+dist_fc*dist_fc)/(2.*dist_fc*dist_fxzmax));
		double cosrel_ymin=acos((-dist_cymin*dist_cymin+dist_fymin*dist_fymin+dist_fc*dist_fc)/(2.*dist_fc*dist_fymin));
		double cosrel_ymax=acos((-dist_cymax*dist_cymax+dist_fymax*dist_fymax+dist_fc*dist_fc)/(2.*dist_fc*dist_fymax));
		return (cosrel_xzmin+cosrel_xzmax)*(sin(cosrel_ymin)+sin(cosrel_ymax));
	}
	void WriteCollimator(int SurfaceIndex=-1){
		if(SurfaceIndex>=0)SurfaceNumber=SurfaceIndex;
		FindCenterPosition();
		FindDirectionVector();
		double xmin_vec=-xmin+x_center;
		double ymin_vec=-ymin+y_center;
		double zmin_vec=-zmin+z_center;
		double xmax_vec=-xmax+x_center;
		double ymax_vec=-ymax+y_center;
		double zmax_vec=-zmax+z_center;
		double R_min=sqrt(xmin_vec*xmin_vec+ymin_vec*ymin_vec+zmin_vec*zmin_vec);
		double R_max=sqrt(xmax_vec*xmax_vec+ymax_vec*ymax_vec+zmax_vec*zmax_vec);
		xmin_vec/=R_min;
		ymin_vec/=R_min;
		zmin_vec/=R_min;
		xmax_vec/=R_max;
		ymax_vec/=R_max;
		zmax_vec/=R_max;
		double c_xmin=f5_x-xmin_vec*averysmallnumber-x_dirvec*averysmallnumber;
		double c_ymin=f5_y-ymin_vec*averysmallnumber-y_dirvec*averysmallnumber;
		double c_zmin=f5_z-zmin_vec*averysmallnumber-z_dirvec*averysmallnumber;
		double c_xmax=f5_x-xmax_vec*averysmallnumber-x_dirvec*averysmallnumber;
		double c_ymax=f5_y-ymax_vec*averysmallnumber-y_dirvec*averysmallnumber;
		double c_zmax=f5_z-zmax_vec*averysmallnumber-z_dirvec*averysmallnumber;

		cout<<"c -------------------Surface card (macro body):"<<endl;
		if(SurfaceNumber>=0){
			cout<<SurfaceNumber<<" arb "<<xmin<<" "<<ymin<<" "<<zmin<<" "<<xmin<<" "<<ymax<<" "<<zmin<<endl;
		}else {
			cout<<"XXX arb "<<xmin<<" "<<ymin<<" "<<zmin<<" "<<xmin<<" "<<ymax<<" "<<zmin<<endl;
		}
		cout<<"        "<<xmax<<" "<<ymin<<" "<<zmax<<" "<<xmax<<" "<<ymax<<" "<<zmax<<endl;
		cout<<"        "<<c_xmin<<" "<<c_ymin<<" "<<c_zmin<<" "<<c_xmin<<" "<<c_ymax<<" "<<c_zmin<<endl;
		cout<<"        "<<c_xmax<<" "<<c_ymin<<" "<<c_zmax<<" "<<c_xmax<<" "<<c_ymax<<" "<<c_zmax<<endl;
		cout<<"        1234 1256 3478 1357 2468 5678"<<endl;
		cout<<"c -------------------Solid angle: "<<GetCollimationSolidAngle()<<endl;
	}
	void WriteEverything(int surfaceindex=-1,int _F5DetectorNumber=0){
		cout<<"c -------------------------------------------------"<<endl;
		cout<<"c ---------------------------Collimator------------"<<endl;
		cout<<"c -------------------------------------------------"<<endl;
		WriteCollimator(surfaceindex);
		cout<<"c -------------------------------------------------"<<endl;
		cout<<"c ---------------------------F5--------------------"<<endl;
		cout<<"c -------------------------------------------------"<<endl;
		WriteF5(_F5DetectorNumber);
		WriteFM(_F5DetectorNumber);
		WriteE(_F5DetectorNumber);
	}
	void WriteEverythingInRange(int anglebins,double minangle,double maxangle,int surfaceprefix=0,int F5startindex=0,bool writeenergybinning=true){

		double minangle_dummy=minangle;
		cout<<"Writing list of "<<anglebins<<" cards from "<<minangle<<" to "<<maxangle<<"the list is: "<<endl;
		cout<<minangle_dummy;
		for(int i=0;i<anglebins;i++){
			minangle_dummy+=(maxangle-minangle)/(anglebins-1);
			cout<<", "<<minangle_dummy;
		}
		cout<<endl<<endl<<endl;

		cout<<"c -------------------------------------------------"<<endl;
		cout<<"c ---------------------------Cell -----------------"<<endl;
		cout<<"c ----------Include/exclude outside world and------"<<endl;
		cout<<"c ----------remember to put a probber celle number-"<<endl;
		cout<<"c -------------------------------------------------"<<endl;
		if(surfaceprefix)cout<<"66666    0 (-"<<surfaceprefix;
		else cout<<"66666    0 (-0";
		int subcounter=0;
		for(int i=1;i<anglebins;i++){
			if(subcounter==4){
				subcounter=0;
				cout<<endl<<"          ";
			}
			cout<<":-"<<surfaceprefix+i;
			subcounter++;
		}
		cout <<") #OutsideWorldCellNumber"<<endl;
		cout <<"       IMP:N,P,|,H,/,Z=01"<<endl;
		cout<<"c -------------------------------------------------"<<endl;
		cout<<"c ---------------------------Surfaces--------------"<<endl;
		cout<<"c -------------------------------------------------"<<endl;
		this->RotateF5to(0);
		this->RotateF5(minangle);
		for(int i=0;i<anglebins;i++){
			WriteCollimator(surfaceprefix+i);
			this->RotateF5((maxangle-minangle)/(anglebins-1));
		}
		cout<<"c -------------------------------------------------"<<endl;
		cout<<"c ---------------------------F5s-------------------"<<endl;
		cout<<"c -------------------------------------------------"<<endl;
		this->RotateF5to(0);
		this->RotateF5(minangle);
		for(int i=0;i<anglebins;i++){
			WriteF5(F5startindex+i);
			WriteFM(F5startindex+i);
			if(writeenergybinning)WriteE(F5startindex+i);
			this->RotateF5((maxangle-minangle)/(anglebins-1));
		}

		cout<<"// -------------------------------------------------"<<endl;
		cout<<"// ---------------------------for Root--------------"<<endl;
		cout<<"// --------------------------After mctal2root mctal-"<<endl;
		cout<<"// -------------------------------------------------"<<endl;
		cout<<"TH1D *cold_dist=(TH1D*)FetchCollimatorMaker(**filename**, "<<anglebins<<", "<<minangle<<", "<<maxangle<<", 1, "<<F5startindex<<"); // for cold neutrons"<<endl;
		cout<<"TH1D *thermal_dist=(TH1D*)FetchCollimatorMaker(**filename**, "<<anglebins<<", "<<minangle<<", "<<maxangle<<", 3, "<<F5startindex<<"); // for thermal neutrons"<<endl;
	}
};
class CollimatorMaker_yx{
private :
	void FindCenterPosition(){
		x_center=(xmax+xmin)/2.;
		y_center=(ymax+ymin)/2.;
		z_center=(zmax+zmin)/2.;
	}
	void FindDirectionVector(){
		FindCenterPosition();
		double a=x_center-f5_x;
		double b=y_center-f5_y;
		double c=z_center-f5_z;
		double R=sqrt(a*a+b*b+c*c);
		x_dirvec=a/R;
		y_dirvec=b/R;
		z_dirvec=c/R;
	}
public :
	double currentF5angle;
	double averysmallnumber;
	double xmin,ymin,zmin;
	double xmax,ymax,zmax;
	double f5_x,f5_y,f5_z;
	double F5ExclusionSphereRadius;
	int SurfaceNumber,F5DetectorNumber;
	double x_center,y_center,z_center;
	double x_dirvec,y_dirvec,z_dirvec;
	char eBins[100];

	CollimatorMaker_yx(double _xmin, double _xmax, double _ymin, double _ymax, double _zmin, double _zmax, double _f5x, double _f5y, double _f5z,int _SurfaceNumber=0,int _F5DetectorNumber=0){
		xmin=_xmin;
		xmax=_xmax;
		ymin=_ymin;
		ymax=_ymax;
		zmin=_zmin;
		zmax=_zmax;
		f5_x=_f5x;
		f5_y=_f5y;
		f5_z=_f5z;
		SurfaceNumber=_SurfaceNumber;
		F5DetectorNumber=_F5DetectorNumber;
		SurfaceNumber=0;
		F5DetectorNumber=0;
		averysmallnumber=0.01;
		F5ExclusionSphereRadius=0.1;
		currentF5angle=0;
		sprintf(eBins,"5.e-9 20.e-9 100.e-9 1.e-6 1.e-3 1. 1.e6");
	}
	~CollimatorMaker_yx(){}

	void SetSurfaceNumber(int inSurfaceNumber){
		SurfaceNumber=inSurfaceNumber;
	}
	void SetF5Position(double inf5_x,double inf5_y,double inf5_z,int Fnumber5=0,double inF5ro=-1){
		if(inF5ro>0)F5ExclusionSphereRadius=inF5ro;
		f5_x=inf5_x;
		f5_y=inf5_y;
		f5_z=inf5_z;
	}
	void SetMinPosition(double inxmin,double inymin,double inzmin){
		xmin=inxmin;
		ymin=inymin;
		zmin=inzmin;
	}
	void SetMaxPosition(double inxmax,double inymax,double inzmax){
		xmax=inxmax;
		ymax=inymax;
		zmax=inzmax;
	}
	void RotateF5to(double angleindeg){ // rotation around the y-axis
		RotateF5(-currentF5angle+angleindeg);
		currentF5angle=angleindeg;
	}
	void RotateF5(double angleindeg){ // rotation around the y-axis
		if(angleindeg==0)return;
		currentF5angle=currentF5angle+angleindeg;
		double oldx=f5_x;
		f5_x=f5_x*cos(angleindeg*M::deg2rad)+f5_y*sin(angleindeg*M::deg2rad);
		f5_y=f5_y*cos(angleindeg*M::deg2rad)-oldx*sin(angleindeg*M::deg2rad);
	}
	void WriteF5(int _F5DetectorNumber=0){
		if(_F5DetectorNumber)F5DetectorNumber=_F5DetectorNumber;
		if(F5DetectorNumber>0){
			cout<<"F"<<F5DetectorNumber<<"5:n ";
		} else {
			cout<<"F5:n ";
		}
		cout<<f5_x<<" "<<f5_y<<" "<<f5_z<<" "<<F5ExclusionSphereRadius<<endl;
	}
	void WriteE(int _F5DetectorNumber=0){
		if(_F5DetectorNumber)F5DetectorNumber=_F5DetectorNumber;
		if(F5DetectorNumber>0){
			cout<<"E"<<F5DetectorNumber<<"5:n";
		} else {
			cout<<"E5";
		}
		cout<<" 5e-9 20e-9 100e-9 1e-6 1e-3 1. 1e3 1e6"<<endl;
	}
	void WriteFM(int _F5DetectorNumber=0){
		if(_F5DetectorNumber)F5DetectorNumber=_F5DetectorNumber;
		if(F5DetectorNumber>0){
			cout<<"FM"<<F5DetectorNumber<<"5 ";
		} else {
			cout<<"FM5 ";
		}
		if(GetCollimationSolidAngle()==0){
			cout<<"1"<<endl;
			cout<<"c -----------------WARNING - colimator angle is 0------------------"<<endl;
		}
		cout<<1.56037734e16/GetCollimationSolidAngle()<<endl;
	}
	double GetCollimationSolidAngle(){
		FindCenterPosition();
		double H=sqrt((x_center-f5_x)*(x_center-f5_x)+(y_center-f5_y)*(y_center-f5_y)+(z_center-f5_z)*(z_center-f5_z));
		double dist_fc=sqrt((x_center-f5_x)*(x_center-f5_x)+(y_center-f5_y)*(y_center-f5_y)+(z_center-f5_z)*(z_center-f5_z));
		double dist_fymin=sqrt((x_center-f5_x)*(x_center-f5_x)+(ymin-f5_y)*(ymin-f5_y)+(z_center-f5_z)*(z_center-f5_z));
		double dist_fymax=sqrt((x_center-f5_x)*(x_center-f5_x)+(ymax-f5_y)*(ymax-f5_y)+(z_center-f5_z)*(z_center-f5_z));
		double dist_fxzmin=sqrt((xmin-f5_x)*(xmin-f5_x)+(y_center-f5_y)*(y_center-f5_y)+(zmin-f5_z)*(zmin-f5_z));
		double dist_fxzmax=sqrt((xmax-f5_x)*(xmax-f5_x)+(y_center-f5_y)*(y_center-f5_y)+(zmax-f5_z)*(zmax-f5_z));
		double dist_cxzmin=sqrt((x_center-xmin)*(x_center-xmin)+(z_center-zmin)*(z_center-zmin));
		double dist_cxzmax=sqrt((x_center-xmax)*(x_center-xmax)+(z_center-zmax)*(z_center-zmax));
		double dist_cymin=sqrt((y_center-ymin)*(y_center-ymin));
		double dist_cymax=sqrt((y_center-ymax)*(y_center-ymax));
		double cosrel_xzmin=acos((-dist_cxzmin*dist_cxzmin+dist_fxzmin*dist_fxzmin+dist_fc*dist_fc)/(2.*dist_fc*dist_fxzmin));
		double cosrel_xzmax=acos((-dist_cxzmax*dist_cxzmax+dist_fxzmax*dist_fxzmax+dist_fc*dist_fc)/(2.*dist_fc*dist_fxzmax));
		double cosrel_ymin=acos((-dist_cymin*dist_cymin+dist_fymin*dist_fymin+dist_fc*dist_fc)/(2.*dist_fc*dist_fymin));
		double cosrel_ymax=acos((-dist_cymax*dist_cymax+dist_fymax*dist_fymax+dist_fc*dist_fc)/(2.*dist_fc*dist_fymax));
		return (cosrel_xzmin+cosrel_xzmax)*(sin(cosrel_ymin)+sin(cosrel_ymax));
	}
	void WriteCollimator(int SurfaceIndex=-1){
		if(SurfaceIndex>=0)SurfaceNumber=SurfaceIndex;
		FindCenterPosition();
		FindDirectionVector();
		double xmin_vec=-xmin+x_center;
		double ymin_vec=-ymin+y_center;
		double zmin_vec=-zmin+z_center;
		double xmax_vec=-xmax+x_center;
		double ymax_vec=-ymax+y_center;
		double zmax_vec=-zmax+z_center;
		double R_min=sqrt(xmin_vec*xmin_vec+ymin_vec*ymin_vec+zmin_vec*zmin_vec);
		double R_max=sqrt(xmax_vec*xmax_vec+ymax_vec*ymax_vec+zmax_vec*zmax_vec);
		xmin_vec/=R_min;
		ymin_vec/=R_min;
		zmin_vec/=R_min;
		xmax_vec/=R_max;
		ymax_vec/=R_max;
		zmax_vec/=R_max;
		double c_xmin=f5_x-xmin_vec*averysmallnumber-x_dirvec*averysmallnumber;
		double c_ymin=f5_y-ymin_vec*averysmallnumber-y_dirvec*averysmallnumber;
		double c_zmin=f5_z-zmin_vec*averysmallnumber-z_dirvec*averysmallnumber;
		double c_xmax=f5_x-xmax_vec*averysmallnumber-x_dirvec*averysmallnumber;
		double c_ymax=f5_y-ymax_vec*averysmallnumber-y_dirvec*averysmallnumber;
		double c_zmax=f5_z-zmax_vec*averysmallnumber-z_dirvec*averysmallnumber;

		cout<<"c -------------------Surface card (macro body):"<<endl;
		if(SurfaceNumber>=0){
			cout<<SurfaceNumber<<" arb "<<xmin<<" "<<ymin<<" "<<zmin<<" "<<xmin<<" "<<ymax<<" "<<zmin<<endl;
		}else {
			cout<<"XXX arb "<<xmin<<" "<<ymin<<" "<<zmin<<" "<<xmin<<" "<<ymax<<" "<<zmin<<endl;
		}
		cout<<"        "<<xmax<<" "<<ymin<<" "<<zmax<<" "<<xmax<<" "<<ymax<<" "<<zmax<<endl;
		cout<<"        "<<c_xmin<<" "<<c_ymin<<" "<<c_zmin<<" "<<c_xmin<<" "<<c_ymax<<" "<<c_zmin<<endl;
		cout<<"        "<<c_xmax<<" "<<c_ymin<<" "<<c_zmax<<" "<<c_xmax<<" "<<c_ymax<<" "<<c_zmax<<endl;
		cout<<"        1234 1256 3478 1357 2468 5678"<<endl;
		cout<<"c -------------------Solid angle: "<<GetCollimationSolidAngle()<<endl;
	}
	void WriteEverything(int surfaceindex=-1,int _F5DetectorNumber=0){
		cout<<"c -------------------------------------------------"<<endl;
		cout<<"c ---------------------------Collimator------------"<<endl;
		cout<<"c -------------------------------------------------"<<endl;
		WriteCollimator(surfaceindex);
		cout<<"c -------------------------------------------------"<<endl;
		cout<<"c ---------------------------F5--------------------"<<endl;
		cout<<"c -------------------------------------------------"<<endl;
		WriteF5(_F5DetectorNumber);
		WriteFM(_F5DetectorNumber);
		WriteE(_F5DetectorNumber);
	}
	void WriteEverythingInRange(int anglebins,double minangle,double maxangle,int surfaceprefix=0,int F5startindex=0,bool writeenergybinning=true){

		double minangle_dummy=minangle;
		cout<<"Writing list of "<<anglebins<<" cards from "<<minangle<<" to "<<maxangle<<"the list is: "<<endl;
		cout<<minangle_dummy;
		for(int i=0;i<anglebins;i++){
			minangle_dummy+=(maxangle-minangle)/(anglebins-1);
			cout<<", "<<minangle_dummy;
		}
		cout<<endl<<endl<<endl;

		cout<<"c -------------------------------------------------"<<endl;
		cout<<"c ---------------------------Cell -----------------"<<endl;
		cout<<"c ----------Include/exclude outside world and------"<<endl;
		cout<<"c ----------remember to put a probber celle number-"<<endl;
		cout<<"c -------------------------------------------------"<<endl;
		if(surfaceprefix)cout<<"66666    0 (-"<<surfaceprefix;
		else cout<<"66666    0 (-0";
		int subcounter=0;
		for(int i=1;i<anglebins;i++){
			if(subcounter==4){
				subcounter=0;
				cout<<endl<<"          ";
			}
			cout<<":-"<<surfaceprefix+i;
			subcounter++;
		}
		cout <<") #OutsideWorldCellNumber"<<endl;
		cout <<"       IMP:N,P,|,H,/,Z=01"<<endl;
		cout<<"c -------------------------------------------------"<<endl;
		cout<<"c ---------------------------Surfaces--------------"<<endl;
		cout<<"c -------------------------------------------------"<<endl;
		this->RotateF5to(0);
		this->RotateF5(minangle);
		for(int i=0;i<anglebins;i++){
			WriteCollimator(surfaceprefix+i);
			this->RotateF5((maxangle-minangle)/(anglebins-1));
		}
		cout<<"c -------------------------------------------------"<<endl;
		cout<<"c ---------------------------F5s-------------------"<<endl;
		cout<<"c -------------------------------------------------"<<endl;
		this->RotateF5to(0);
		this->RotateF5(minangle);
		for(int i=0;i<anglebins;i++){
			WriteF5(F5startindex+i);
			WriteFM(F5startindex+i);
			if(writeenergybinning)WriteE(F5startindex+i);
			this->RotateF5((maxangle-minangle)/(anglebins-1));
		}

		cout<<"// -------------------------------------------------"<<endl;
		cout<<"// ---------------------------for Root--------------"<<endl;
		cout<<"// --------------------------After mctal2root mctal-"<<endl;
		cout<<"// -------------------------------------------------"<<endl;
		cout<<"TH1D *cold_dist=(TH1D*)FetchCollimatorMaker(**filename**, "<<anglebins<<", "<<minangle<<", "<<maxangle<<", 1, "<<F5startindex<<"); // for cold neutrons"<<endl;
		cout<<"TH1D *thermal_dist=(TH1D*)FetchCollimatorMaker(**filename**, "<<anglebins<<", "<<minangle<<", "<<maxangle<<", 3, "<<F5startindex<<"); // for thermal neutrons"<<endl;
	}
};
TH1D *FetchCollimatorMaker(char *filename, int anglebins,double minangle,double maxangle,int bintostore=1 /*by default 1 is cold 3 is thermal*/,int F5startindex=0){
	TH1D *hist=new TH1D(TSC::uname(),"",anglebins,minangle-(maxangle-minangle)/(anglebins-1)/2.,maxangle+(maxangle-minangle)/(anglebins-1)/2.);
	hist->Sumw2();
	char fname[10];
	if(F5startindex)sprintf(fname,"f%d5\0",F5startindex);
	else sprintf(fname,"f5\0");
	TH1D *S=(TH1D*)TSC::SparseLoader(filename,fname,6);
	int imax=0;
	while(S&&imax<anglebins+1){
		imax++;
		sprintf(fname,"f%d5\0",F5startindex+imax);
		delete S;
		S=(TH1D*)TSC::SparseLoader(filename,fname,6);
	}
	cout<<imax<<"-------------"<<endl;
	for(int i=0;i<imax;i++){
		if(F5startindex+i!=0)sprintf(fname,"f%d5\0",F5startindex+i);
		else sprintf(fname,"f5\0");
		S=(TH1D*)TSC::SparseLoader(filename,fname,6);
		for(int j=1;j<hist->GetNbinsX()+2;j++){	
			if(hist->GetBinLowEdge(j+1)>minangle+i*1.*(maxangle-minangle)/(imax-1.)){
				hist->SetBinContent(j,S->GetBinContent(bintostore));
				hist->SetBinError(j,S->GetBinError(bintostore));
				break;
			}
		}
		delete S;
	}
	return hist;
}


class TheMatrix {
public:
static char __NameTag[100];
static char __GlobalTheMatrixCharArray[1000];
static bool __GlobalTheMatrixBoolArray[1000];
static clock_t DidDoTheMatrixTime;
static clock_t DidStartTheMatrixTime; 
static clock_t Last10TheMatrixTimes[25];
static long int Last10TheMatrixEntry[25];
static double DoTheMatrixRate;
static int PrintLength;
static int CallCounter;
static int DummyIterator;
static int TimeCounter;
static int NumberItrLoopLength;

static void SetLoopLength(int i){
	NumberItrLoopLength=i;
	return;
}

static void NameTag(char *name){
	sprintf(__NameTag,"%s\0",name);
}

static void reset(){
	DidDoTheMatrixTime=clock();
	Last10TheMatrixTimes[0]=0;
	DummyIterator=0;
	DidStartTheMatrixTime=DidDoTheMatrixTime;
	CallCounter=0;
	for(DummyIterator=0;DummyIterator<25;DummyIterator++){
		Last10TheMatrixTimes[DummyIterator]=DidStartTheMatrixTime;
		Last10TheMatrixEntry[DummyIterator]=0;
		__NameTag[DummyIterator]=0;
		__NameTag[DummyIterator+25]=0;
		__NameTag[DummyIterator+50]=0;
		__NameTag[DummyIterator+75]=0;
	}
	DummyIterator=0;
}

static char *NewString(){
	for(int i=0;i<PrintLength;i++){
		if(__GlobalTheMatrixBoolArray[i])if(gRandom->Rndm()<=0.125)__GlobalTheMatrixBoolArray[i]=false;
		if(!__GlobalTheMatrixBoolArray[i])if(gRandom->Rndm()<=0.025)__GlobalTheMatrixBoolArray[i]=true;
		if(__GlobalTheMatrixBoolArray[i]){
			__GlobalTheMatrixCharArray[i]=(char)((gRandom->Rndm()*95+32.5));
		} else __GlobalTheMatrixCharArray[i]=32;
	}
	return __GlobalTheMatrixCharArray;
}

static void Next(long int entry=-1){
	CallCounter++;
	if(entry!=-1)entry=CallCounter;
	if(clock()-DidDoTheMatrixTime<DoTheMatrixRate)return;
	if(DidDoTheMatrixTime==0||clock()-DidDoTheMatrixTime>DoTheMatrixRate*100)reset();
	DidDoTheMatrixTime=clock();
	if(CallCounter==0){
		cout<<"The Matrix";
		if(__NameTag[0]!=0)cout<<" ("<<__NameTag<<")";
		cout<<" was initialized, with input "<<entry<<", at clock: "<<DidStartTheMatrixTime<<endl;
		return;
	}

	if(__NameTag[0]!=0)cout<<__NameTag<<"\t";
	cout<<(double)((int)((fabs((double)(DidStartTheMatrixTime-DidDoTheMatrixTime)))/CLOCKS_PER_SEC*10))/10.<<"s";
	if(NumberItrLoopLength>-1)if(entry<NumberItrLoopLength){
		Last10TheMatrixTimes[DummyIterator]=DidDoTheMatrixTime;
		Last10TheMatrixEntry[DummyIterator]=entry;
		DummyIterator++;
		if(DummyIterator>24)DummyIterator=0;
		cout<<"\t Done in: "<<
			(double)((int)(((fabs((double)(DidDoTheMatrixTime-Last10TheMatrixTimes[DummyIterator])))/CLOCKS_PER_SEC)
			/(entry-Last10TheMatrixEntry[DummyIterator])
			*(NumberItrLoopLength-entry))*10.)/10.
			<<"s("<<
			(double)((int)(((fabs((double)(DidDoTheMatrixTime-DidStartTheMatrixTime)))/CLOCKS_PER_SEC)
			/(entry)
			*(NumberItrLoopLength-entry))*10.)/10.
			<<"s)";
		}
	cout<<"\t n="<<entry<<" "<<NewString()<<endl;
	return;
}
};
char TheMatrix::__NameTag[100]={0};
clock_t TheMatrix::DidDoTheMatrixTime=0;
clock_t TheMatrix::DidStartTheMatrixTime=clock();
clock_t TheMatrix::Last10TheMatrixTimes[25]={0};
long int TheMatrix::Last10TheMatrixEntry[25]={0};
double TheMatrix::DoTheMatrixRate=333; // rate in milisecunds
char TheMatrix::__GlobalTheMatrixCharArray[1000]={0};
bool TheMatrix::__GlobalTheMatrixBoolArray[1000]={0};
int TheMatrix::PrintLength=70;
int TheMatrix::CallCounter=0;
int TheMatrix::DummyIterator=0;
int TheMatrix::NumberItrLoopLength=0;



class Neutron{
	private:
	int strange_extraction_index_warning_counter;
	public:
	bool SwichOff_DoYouKnowThatThisMightNotBeANeutron;
	bool UseAllParticles;
	double history,id,x,y,z,wx,wy,k,energy,time,weight;
	double i;
	double JGP;
	double JC;
	double IPT;
	double surface;
		
	double wz;
	double lambda;
	double l;
	double e;
	double loge;
	double logl;
	double ModeratorHight;
	double rsqr;
	double radtodeg;
	double r;
	double xsign;
	double ysign;
	double theta;
	double ycenter;
	double BigRsqr;
	double BigR;
	double phi;
	double wr;
	double wxsign;
	double wtheta0;
	double wthetaDiff;
	double wtheta;
	double wphi;
	int extractionindex;
	double wthetaDiff_center;
	double wtheta_center;
	void Reset(){
		extractionindex=-1;
		history=0;
		id=0;
		x=0;
		y=0;
		z=0;
		wx=0;
		wy=0;
		k=0;
		energy=0;
		time=0;
		weight=0;
		wthetaDiff_center=0;
		i=0;
		JGP=0;
		JC=0;
		IPT=0;
		surface=0;
		wz=0;
		lambda=0;
		l=0;
		e=0;
		loge=0;
		logl=0;
		ModeratorHight=0;
		rsqr=0;
		radtodeg=0;
		r=0;
		xsign=0;
		ysign=0;
		theta=0;
		ycenter=0;
		BigRsqr=0;
		BigR=0;
		phi=0;
		wr=0;
		wxsign=0;
		wtheta0=0;
		wthetaDiff=0;
		wtheta=0;
		wphi=0;
		wtheta_center=0;
		strange_extraction_index_warning_counter=10; // warns 10 times
	}
	void LoadNeutron(double inhistory,double inid,double inx,double iny,double inz,double inwx,double inwy,double ink,double inenergy,double intime,double inweight){

		history=inhistory;
		id=inid;
		x=inx;
		y=iny;
		z=inz;
		wx=inwx;
		wy=inwy;
		k=ink;
		energy=inenergy;
		time=intime;
		weight=inweight;


		i=SSW::ci(id);
		JGP=SSW::cJGP(id);
		JC=SSW::cJC(id);
		IPT=SSW::cIPT(id);
		surface=SSW::csurface(id);
		wz=SSW::cwz(wx,wy,id);


		lambda=SSW::clambda(energy);
		l=lambda;
		e=energy;
		loge=log10(energy);
		logl=log10(lambda);

		r=SSW::cr(x,z);
		theta=SSW::ctheta(x,z);
		BigR=SSW::cBigR(x,y,z);
		wr=SSW::cwr(x,z);
	
		wtheta0=SSW::cwtheta0(wx,wz);
		wtheta=SSW::cwtheta(x,z,wz,wz);
	
		if(IPT!=1){
			if(!UseAllParticles){
				if(SwichOff_DoYouKnowThatThisMightNotBeANeutron){
					return;
				} else {
					cout<<"WARNING: A non-neutron was used as a neutron, turn off this Warning:"<<endl;
					cout<<" - Neutron::SwichOff_DoYouKnowThatThisMightNotBeANeutron=true;"<<endl;
					cout<<" or set:"<<endl;cout<<" - Neutron::UseAllParticles=true"<<endl;
				}
			}
		}



	}
	double GetExtractionIndexLimit(int inindex){
		return inindex*5.+30.+(inindex>11)*5.+(inindex>23)*50.+(inindex>35)*5.;
	}
	Neutron(){
		SwichOff_DoYouKnowThatThisMightNotBeANeutron=false;
		UseAllParticles=false;
		Reset();
	}
	
	void UseAllIPT(bool inputbool=true){UseAllParticles=inputbool;}// switch if you dont want the warning!
	Neutron(double inhistory,double inid,double inx,double iny,double inz,double inwx,double inwy,double ink,double inenergy,double intime,double inweight){
		SwichOff_DoYouKnowThatThisMightNotBeANeutron=false;
		UseAllParticles=false;
		LoadNeutron(inhistory,inid,inx,iny,inz,inwx,inwy,ink,inenergy,intime,inweight);
	}
	~Neutron(){}
};


/* // old neutron class
class Neutron{
	private:
	int strange_extraction_index_warning_counter;
	public:
	bool SwichOff_DoYouKnowThatThisMightNotBeANeutron;
	bool UseAllParticles;
	double history,id,x,y,z,wx,wy,k,energy,time,weight;
	double i;
	double JGP;
	double JC;
	double IPT;
	double surface;
		
	double wz;
	double lambda;
	double l;
	double e;
	double loge;
	double logl;
	double ModeratorHight;
	double rsqr;
	double radtodeg;
	double r;
	double xsign;
	double ysign;
	double theta;
	double ycenter;
	double BigRsqr;
	double BigR;
	double phi;
	double wr;
	double wxsign;
	double wtheta0;
	double wthetaDiff;
	double wtheta;
	double wphi;
	int extractionindex;
	double wthetaDiff_center;
	double wtheta_center;
	void Reset(){
		extractionindex=-1;
		history=0;
		id=0;
		x=0;
		y=0;
		z=0;
		wx=0;
		wy=0;
		k=0;
		energy=0;
		time=0;
		weight=0;
		wthetaDiff_center=0;
		i=0;
		JGP=0;
		JC=0;
		IPT=0;
		surface=0;
		wz=0;
		lambda=0;
		l=0;
		e=0;
		loge=0;
		logl=0;
		ModeratorHight=0;
		rsqr=0;
		radtodeg=0;
		r=0;
		xsign=0;
		ysign=0;
		theta=0;
		ycenter=0;
		BigRsqr=0;
		BigR=0;
		phi=0;
		wr=0;
		wxsign=0;
		wtheta0=0;
		wthetaDiff=0;
		wtheta=0;
		wphi=0;
		wtheta_center=0;
		strange_extraction_index_warning_counter=10; // warns 10 times
	}
	void LoadNeutron(double inhistory,double inid,double inx,double iny,double inz,double inwx,double inwy,double ink,double inenergy,double intime,double inweight){
		id=inid;
		i=TSC::round(fabs(id/1E+6));
		JGP=TSC::round(i/200.0);
		JC=TSC::round(i/100.0) + 2*JGP;
		IPT=i-100*JC+200*JGP;

		if(IPT!=1){
			if(!UseAllParticles){
				if(SwichOff_DoYouKnowThatThisMightNotBeANeutron){
					return;
				} else {
					cout<<"WARNING: A non-neutron was used as a neutron, turn off this Warning:"<<endl;
					cout<<" - Neutron::SwichOff_DoYouKnowThatThisMightNotBeANeutron=true;"<<endl;
					cout<<" or set:"<<endl;cout<<" - Neutron::UseAllParticles=true"<<endl;
				}
			}
		}

		history=inhistory;
		x=inx;
		y=iny;
		z=inz;
		wx=inwx;
		wy=inwy;
		k=ink;
		energy=inenergy;
		time=intime;
		weight=inweight;


		surface=fabs(id)-1000000;
		//if(1-wx*wx-wy*wy<0)cout<<"ERROR TSC::SSW  ( 1-wx*wx-wy*wy<0 )"<<endl;
		wz=sqrt(1-wx*wx-wy*wy) * id/fabs(id);

		lambda=0.000286299/sqrt(energy);
		l=lambda;
		e=energy;
		loge=log10(energy);
		logl=log10(lambda);
		ModeratorHight=13.;
		rsqr=x*x+z*z;
		radtodeg=57.29577;
		r=sqrt(rsqr);
		xsign=(-2.*(x<0)+1.);
		ysign=(-2.*(y<0)+1.);
		theta=(x<0)*360.+xsign*(acos(z/r)*radtodeg);
		ycenter=ysign*ModeratorHight-y;
		BigRsqr=x*x+ycenter*ycenter+z*z;
		BigR=sqrt(x*x+ycenter*ycenter+z*z);
		phi=asin(ycenter/BigR)*radtodeg;
		wr=sqrt(wx*wx+wz*wz);
		wxsign=(-2.*(wx<0)+1.);
		wtheta0=(wx<0)*360.+wxsign*(acos(wz/wr)*radtodeg);
		wthetaDiff=(wtheta0-theta);
		wtheta=(wthetaDiff<-180)*360-(wthetaDiff>180)*360+wthetaDiff;
		wphi=asin(wy)*radtodeg;
		extractionindex=(theta-30)/5-(theta>90)-(theta>155)*10-(theta>265);
		wthetaDiff_center=(wtheta0-(extractionindex*5.+32.5+(extractionindex>11)*5.+(extractionindex>23)*50.+(extractionindex>35)*5.));
		//cout<<theta<<"  KK  "<<wtheta0<<" :: "<<wthetaDiff_center<<"  --  "<<wtheta<<endl;
		wtheta_center=(wthetaDiff_center<-180)*360-(wthetaDiff_center>180)*360+wthetaDiff_center;
		
		if(extractionindex<0||extractionindex>47){
			if(theta<=330.00001&&theta>=329){extractionindex=47;
			} else if(theta>=-0.00001&&theta<=1){
				if(strange_extraction_index_warning_counter>0){
					cout<<"strange extraction index : "<<extractionindex<<" "<<history<<" - theta="<<theta<<" - extractionindex set to 48 (this might cause a seq fault)"<<endl;
					cout<<" -- This warning will be repeated "<<10-strange_extraction_index_warning_counter<<" more times"<<endl;
					strange_extraction_index_warning_counter--;
				}
				extractionindex=48;
			}else {
				if(strange_extraction_index_warning_counter>0){
					cout<<"strange extraction index : "<<extractionindex<<" "<<history<<" - theta="<<theta<<" - extractionindex set to 48 (this might cause a seq fault)"<<endl;
					cout<<" -- This warning will be repeated "<<10-strange_extraction_index_warning_counter<<" more times"<<endl;
					strange_extraction_index_warning_counter--;
				}
				extractionindex=48;
				
			}
		}
	}
	double GetExtractionIndexLimit(int inindex){
		return inindex*5.+30.+(inindex>11)*5.+(inindex>23)*50.+(inindex>35)*5.;
	}
	Neutron(){
		SwichOff_DoYouKnowThatThisMightNotBeANeutron=false;
		UseAllParticles=false;
		Reset();
	}
	
	void UseAllIPT(bool inputbool=true){UseAllParticles=inputbool;}// switch if you dont want the warning!
	Neutron(double inhistory,double inid,double inx,double iny,double inz,double inwx,double inwy,double ink,double inenergy,double intime,double inweight){
		SwichOff_DoYouKnowThatThisMightNotBeANeutron=false;
		UseAllParticles=false;
		LoadNeutron(inhistory,inid,inx,iny,inz,inwx,inwy,ink,inenergy,intime,inweight);
	}
	~Neutron(){}
};
*/
void GetThoseNumbersGeometryLimits(double x,double z,double phi){
double XX=sin(phi*M::pi/180.)*2-x;
double ZZ=cos(phi*M::pi/180.)*2-z;
double R=sqrt(ZZ*ZZ+XX*XX);
cout<<"X: "<<.12*XX/R+x<<endl;
cout<<"Z: "<<.12*ZZ/R+z<<endl;
}
double ColdThermalThetaLimit(double *theta,double *par){
	double limx,limy,sign,cosang,rsqr,r;
	double R=2.;
	double Rsqr=4.;
	if(theta[0]<92.5){
			limx=.0891;
			limy=-.0128;
			sign=-1;
			rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
			cosang=cos(theta[0]/180*M::pi-acos(limy/r));
		} else if(theta[0]<180){
			limx=.026;
			limy=-.0862;
			sign=-1;
			rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
			cosang=cos(theta[0]/180*M::pi-acos(limy/r));
		} else if(theta[0]>267.5){
			limx=-.0891;
			limy=-.0128;
			sign=1;
			rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
			cosang=cos(theta[0]/180*M::pi-(2*M::pi-acos(limy/r)));
		} else {
			limx=-.026;
			limy=-.0862;
			sign=1;
			rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
			cosang=cos(theta[0]/180*M::pi-(2*M::pi-acos(limy/r)));
		}
		double over=Rsqr-r*R*cosang;
		double under=sqrt(rsqr+Rsqr-2*r*R*cosang)*R;
		return sign*acos(over/under)*180/M::pi;
	}
double ReflectorColdThetaLimit(double *theta,double *par){
	double limx,limy,sign,cosang,rsqr,r;
	double R=2.;
	double Rsqr=4.;
	if(theta[0]<92.5){
		limx=.0334;
		limy=.0836;
		sign=1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180*M::pi-acos(limy/r));
	} else if(theta[0]<180){
		limx=.0899;
		limy=.005;
		sign=1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180*M::pi-acos(limy/r));
	} else if(theta[0]>267.5){
		limx=-.0334;
		limy=.0836;
		sign=-1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180*M::pi-(2*M::pi-acos(limy/r)));
	} else {
		limx=-.0899;
		limy=.005;
		sign=-1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180*M::pi-(2*M::pi-acos(limy/r)));
	}
	double over=Rsqr-r*R*cosang;
	double under=sqrt(rsqr+Rsqr-2*r*R*cosang)*R;
	return sign*acos(over/under)*180/M::pi;
}
double ThermalReflectorThetaLimit(double *theta,double *par){
	double limx,limy,sign,cosang,rsqr,r;
	double R=2.;
	double Rsqr=4.;
	if(theta[0]<92.5){
		
		limx= .1603;
		limy= -.1055;
		sign=-1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180.*M::pi-acos(limy/r));
		//cosang=cos(x[0]/180*pi-asin(limx/r));
	} else if(theta[0]<180){
		limx= .0279;
		limy= -.1899;
		sign=-1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180.*M::pi-(2.*M::pi-acos(limy/r)));
	} else if(theta[0]>267.5){
		limx= -.1603;
		limy= -.1055;
		sign=1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180.*M::pi-(2.*M::pi-acos(limy/r)));
	} else {
		limx= -.0279;
		limy= -.1899;
		sign=1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180.*M::pi-acos(limy/r));
	}
	double over=Rsqr-r*R*cosang;
	double under=sqrt(rsqr+Rsqr-2*r*R*cosang)*R;
	return sign*acos(over/under)*180/M::pi;
}
double ThermalReflectorThetaUpperLimit(double *theta,double *par){
	double limx,limy,sign,cosang,rsqr,r;
	double R=2.;
	double Rsqr=4.;
	if(theta[0]<92.5){
		limx=.2801;
		limy= -.0986;
		sign=-1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180.*M::pi-acos(limy/r));
		//cosang=cos(x[0]/180*pi-asin(limx/r));
	} else if(theta[0]<180){
		limx= .02896;
		limy= -.29707;
		sign=-1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180.*M::pi-(2.*M::pi-acos(limy/r)));
	} else if(theta[0]>267.5){
		limx=-.2801;
		limy= -.0986;
		sign=1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180.*M::pi-(2.*M::pi-acos(limy/r)));
	} else {
		limx= -.02896;
		limy= -.29707;
		sign=1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180.*M::pi-acos(limy/r));
	}
	double over=Rsqr-r*R*cosang;
	double under=sqrt(rsqr+Rsqr-2*r*R*cosang)*R;
	return sign*acos(over/under)*180/M::pi;
}
double ReflectorColdThetaUpperLimit(double *theta,double *par){
	double limx,limy,sign,cosang,rsqr,r;
	double R=2.;
	double Rsqr=4.;
	if(theta[0]<92.5){
		limx=.0941;
		limy=.1871;
		sign=1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180*M::pi-acos(limy/r));
	} else if(theta[0]<180){
		limx=.2094;
		limy=.0063;
		sign=1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180*M::pi-acos(limy/r));
	} else if(theta[0]>267.5){
		limx=-.0941;
		limy=.1871;
		sign=-1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180*M::pi-(2*M::pi-acos(limy/r)));
	} else {
		limx=-.2094;
		limy=.0063;
		sign=-1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(theta[0]/180*M::pi-(2*M::pi-acos(limy/r)));
	}
	double over=Rsqr-r*R*cosang;
	double under=sqrt(rsqr+Rsqr-2*r*R*cosang)*R;
	return sign*acos(over/under)*180/M::pi;
}

double AlphaColdTherm(double *x,double *par){
	double limx,limy,sign,cosang,rsqr,r;
	double R=2.;
	double Rsqr=4.;
		if(x[0]<92.5){
		limx=.0891;
		limy=-.0128;
		sign=-1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(x[0]/180*M::pi-acos(limy/r));
	} else if(x[0]<180){
		limx=.026;
		limy=-.0862;
		sign=-1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(x[0]/180*M::pi-acos(limy/r));
	} else if(x[0]>267.5){
		limx=-.0891;
		limy=-.0128;
		sign=1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(x[0]/180*M::pi-(2*M::pi-acos(limy/r)));
	} else {
		limx=-.026;
		limy=-.0862;
		sign=1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(x[0]/180*M::pi-(2*M::pi-acos(limy/r)));
	}
	double over=Rsqr-r*R*cosang;
	double under=sqrt(rsqr+Rsqr-2*r*R*cosang)*R;
	return sign*acos(over/under)*180/M::pi;

}
double AlphaRefCold(double *x,double *par){
	double limx,limy,sign,cosang,rsqr,r;
	double R=2.;
	double Rsqr=4.;
		if(x[0]<92.5){
		limx=.0334;
		limy=.0836;
		sign=1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(x[0]/180*M::pi-acos(limy/r));
	} else if(x[0]<180){
		limx=.0899;
		limy=.005;
		sign=1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(x[0]/180*M::pi-acos(limy/r));
	} else if(x[0]>267.5){
		limx=-.0334;
		limy=.0836;
		sign=-1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(x[0]/180*M::pi-(2*M::pi-acos(limy/r)));
	} else {
		limx=-.0899;
		limy=.005;
		sign=-1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(x[0]/180*M::pi-(2*M::pi-acos(limy/r)));
	}
	double over=Rsqr-r*R*cosang;
	double under=sqrt(rsqr+Rsqr-2*r*R*cosang)*R;
	return sign*acos(over/under)*180/M::pi;

}
double AlphaRefTherm(double *x,double *par){
	double limx,limy,sign,cosang,rsqr,r;
	double R=2.;
	double Rsqr=4.;

	if(x[0]<92.5){
		limx= .1603;
		limy= -.1055;
		sign=-1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(x[0]/180.*M::pi-acos(limy/r));
		//cosang=cos(x[0]/180*pi-asin(limx/r));
	} else if(x[0]<180){
		limx= -.0279;
		limy= -.1899;
		sign=-1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(x[0]/180.*M::pi-acos(limy/r));
	} else if(x[0]>267.5){
		limx= -.1603;
		limy= -.1055;
		sign=1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(x[0]/180.*M::pi-(2.*M::pi-acos(limy/r)));
	} else {
		limx= .0279;
		limy= -.1899;
		sign=1;
		rsqr=limx*limx+limy*limy;r=sqrt(rsqr);
		cosang=cos(x[0]/180.*M::pi-(2.*M::pi-acos(limy/r)));
	}
	//cout<<acos(cosang)-x[0]/180*pi<<endl;
	double over=Rsqr-r*R*cosang;
	double under=sqrt(rsqr+Rsqr-2*r*R*cosang)*R;
	//cout<<cosang<<" :-: "<<over<<" :-: "<<under<<endl;
	return sign*acos(over/under)*180/M::pi;
	
	/*
	double xcosysin=limx*cos(x[0]/180.*3.pi)+limy*sin(x[0]/180.*pi);
	double R=2.;
	double Rsqr=4.;
	return sign*180/pi*acos((R-xcosysin)/sqrt(limx*limx+limy*limy-2*R*xcosysin+Rsqr));
*/
	//cout<<(2.-limy)<<"  ::  "<<sqrt(limx*limx+limy*limy)<<endl;
	//return x[0]-180/pi*acos((2.-limx)/sqrt(limx*limx+limy*limy));
	
}

bool hasrotation=false;
char SSW::RX[150];
char SSW::RY[150];
char SSW::RZ[150];
char SSW::RWX[150];
char SSW::RWY[150];
char SSW::RWZ[150];
void SSW::ResetRotation(){
	sprintf(SSW::RX,"x*1.\0");
	sprintf(SSW::RY,"y*1.\0");
	sprintf(SSW::RZ,"z*1.\0");
	sprintf(SSW::RWX,"wx*1.\0");
	sprintf(SSW::RWY,"wy*1.\0");
	sprintf(SSW::RWZ,"wz*1.\0");
	hasrotation=true;
}

void SSW::Rotation1Axis(double ang_deg,char axis /*"x" "y","z"*/){
	if(!hasrotation)ResetRotation();
	double angrad=ang_deg*M::deg2rad;
	double srot=sin(angrad);
	double crot=cos(angrad);
	char old_rx[150];
	char old_ry[150];
	char old_rz[150];
	char old_rwx[150];
	char old_rwy[150];
	char old_rwz[150];

	sprintf(old_rx,"%s\0",SSW::RX);
	sprintf(old_ry,"%s\0",SSW::RY);
	sprintf(old_rz,"%s\0",SSW::RZ);
	sprintf(old_rwx,"%s\0",SSW::RWX);
	sprintf(old_rwy,"%s\0",SSW::RWY);
	sprintf(old_rwz,"%s\0",SSW::RWZ);
	// Z=90, z=122
	if(axis==90||axis==122||axis==3)sprintf(SSW::RX,TSC::CHAR("%9f*",old_rx,"+%9f*",old_ry,"\0"),crot,-srot);
	if(axis==90||axis==122||axis==3)sprintf(SSW::RY,TSC::CHAR("%9f*",old_rx,"+%9f*",old_ry,"\0"),srot,crot);
	if(axis==89||axis==121||axis==2)sprintf(SSW::RX,TSC::CHAR("%9f*",old_rx,"+%9f*",old_rz,"\0"),crot,-srot);
	if(axis==89||axis==121||axis==2)sprintf(SSW::RZ,TSC::CHAR("%9f*",old_rx,"+%9f*",old_rz,"\0"),srot,crot);
	if(axis==88||axis==120||axis==1)sprintf(SSW::RY,TSC::CHAR("%9f*",old_ry,"+%9f*",old_rz,"\0"),crot,-srot);
	if(axis==88||axis==120||axis==1)sprintf(SSW::RZ,TSC::CHAR("%9f*",old_ry,"+%9f*",old_rz,"\0"),srot,crot);

	if(axis==90||axis==122||axis==3)sprintf(SSW::RWX,TSC::CHAR("%9f*",old_rwx,"+%9f*",old_rwy,"\0"),crot,-srot);
	if(axis==90||axis==122||axis==3)sprintf(SSW::RWY,TSC::CHAR("%9f*",old_rwx,"+%9f*",old_rwy,"\0"),srot,crot);
	if(axis==89||axis==121||axis==2)sprintf(SSW::RWX,TSC::CHAR("%9f*",old_rwx,"+%9f*",old_rwz,"\0"),crot,-srot);
	if(axis==89||axis==121||axis==2)sprintf(SSW::RWZ,TSC::CHAR("%9f*",old_rwx,"+%9f*",old_rwz,"\0"),srot,crot);
	if(axis==88||axis==120||axis==1)sprintf(SSW::RWY,TSC::CHAR("%9f*",old_rwy,"+%9f*",old_rwz,"\0"),crot,-srot);
	if(axis==88||axis==120||axis==1)sprintf(SSW::RWZ,TSC::CHAR("%9f*",old_rwy,"+%9f*",old_rwz,"\0"),srot,crot);
	
	cout<<">>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<"<<endl;
	cout<<">>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<"<<endl;
	cout<<"WARNING: this function was never tested!!!"<<endl;
	cout<<">>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<"<<endl;
	cout<<">>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<"<<endl;

}
void SSW::SetSSWAliases(TTree *T){


	T->SetAlias("i","SSW::ci(id*1)");
	T->SetAlias("JGP","SSW::cJGP(id*1)");
	T->SetAlias("JC","SSW::cJC(id*1)");
	T->SetAlias("IPT","SSW::cIPT(id*1)");
	T->SetAlias("surface","SSW::csurface(id*1)");
	T->SetAlias("wz","SSW::cwz(wx,wy,id*1)");

	if(!hasrotation)ResetRotation();
	
	T->SetAlias("rx",SSW::RX); // transformed coordinates
	T->SetAlias("ry",SSW::RY);
	T->SetAlias("rz",SSW::RZ);
	T->SetAlias("rwx",SSW::RWX);
	T->SetAlias("rwy",SSW::RWY);
	T->SetAlias("rwz",SSW::RWZ);

	T->SetAlias("lambda","SSW::clambda(energy*1.)");
	T->SetAlias("l","lambda*1.");
	T->SetAlias("e","energy*1.");
	T->SetAlias("loge","log10(energy*1.)");
	T->SetAlias("logl","log10(lambda*1.)");

	T->SetAlias("r","SSW::cr(rx*1.,rz*1.)");
	T->SetAlias("theta","SSW::ctheta(rx*1.,rz*1.)");
	T->SetAlias("BigR","SSW::cBigR(rx*1.,ry*1.,rz*1.)");
	T->SetAlias("wr","SSW::cwr(rx*1.,rz*1.)");
	
	T->SetAlias("wtheta0","SSW::cwtheta0(rwx*1.,rwz*1.)");
	T->SetAlias("wtheta","SSW::cwtheta(rx*1.,rz*1.,rwx*1.,rwz*1.)");
	

	/*
	T->SetAlias("i","TMath::Nint(TMath::Abs(id/1E+6))");
	T->SetAlias("JGP","-TMath::Nint(i/200.0)");
	T->SetAlias("JC","TMath::Nint(i/100.0) + 2*JGP");
	T->SetAlias("IPT","i-100*JC+200*JGP");
	T->SetAlias("surface","TMath::Abs(id)-1000000");
	T->SetAlias("wz","TMath::Sqrt(TMath::Max(0, 1-wx*wx-wy*wy)) * id/TMath::Abs(id)");


	T->SetAlias("shiftcyl_r","11.*1.");
	T->SetAlias("rx",SSW::RX); // transformed coordinates
	T->SetAlias("ry",SSW::RY);
	T->SetAlias("rz",SSW::RZ);
	T->SetAlias("rwx",SSW::RWX);
	T->SetAlias("rwy",SSW::RWY);
	T->SetAlias("rwz",SSW::RWZ);
	T->SetAlias("lambda","0.000286299/sqrt(energy*1.)");
	T->SetAlias("l","0.000286299/sqrt(energy*1.)");
	T->SetAlias("e","energy*1.");
	T->SetAlias("loge","log10(energy*1.)");
	T->SetAlias("logl","log10(lambda*1.)");
	T->SetAlias("ModeratorHight","13.*1");
	T->SetAlias("rsqr","rx*rx+rz*rz");
	T->SetAlias("radtodeg","1.*57.29577");
	T->SetAlias("r","sqrt(rsqr*1.)");
	T->SetAlias("xsign","(-2.*(rx<0)+1.)");
	T->SetAlias("ysign","(-2.*(ry<0)+1.)");
	T->SetAlias("theta","(rx<0)*360.+xsign*(acos(rz/r)*radtodeg)");
	T->SetAlias("ycenter","ysign*ModeratorHight-ry");
	T->SetAlias("BigRsqr","rx*rx+ycenter*ycenter+rz*rz");
	T->SetAlias("BigR","sqrt(rx*rx+ycenter*ycenter+rz*rz)");
	T->SetAlias("phi","asin(ycenter/BigR)*radtodeg");
	T->SetAlias("wr","sqrt(rwx*rwx+rwz*rwz)");
	T->SetAlias("wxsign","(-2.*(rwx<0)+1.)");
	T->SetAlias("wtheta0","(rwx<0)*360.+wxsign*(acos(rwz/wr)*radtodeg)");
	T->SetAlias("wthetaDiff","(wtheta0-theta)");
	T->SetAlias("wtheta","(wthetaDiff<-180)*360-(wthetaDiff>180)*360+wthetaDiff");
	T->SetAlias("wphi","asin(rwy)*radtodeg");

	T->SetAlias("shiftcyl_r","11.*1.");
	T->SetAlias("shift_Da","rwx*rwx+rwz*rwz");
	T->SetAlias("shift_Db","-2*(rwx*rx+rwz*rz)");
	T->SetAlias("shift_Dc","(rx*rx+rz*rz-shiftcyl_r*shiftcyl_r)");
	T->SetAlias("shift_Ddet","shift_Db*shift_Db-4.*shift_Da*shift_Dc");
	T->SetAlias("shift_Dm","(-shift_Db-sqrt(fabs(shift_Ddet*1.)))/2/shift_Da*(shift_Ddet>0)");
	T->SetAlias("shift_Dp","(-shift_Db+sqrt(fabs(shift_Ddet*1.)))/2/shift_Da*(shift_Ddet>0)");
	T->SetAlias("shift_qdist","shift_Dm-(shift_Dm>shift_Dp)*(shift_Dm-shift_Dp)");
	T->SetAlias("xshift","-shift_qdist*rwx+rx");
	T->SetAlias("zshift","-shift_qdist*rwz+rz");
	T->SetAlias("shift_xshiftsign","(-2.*(xshift<0)+1.)");
	T->SetAlias("thetashift","(xshift<0)*360.+shift_xshiftsign*acos(zshift/shiftcyl_r)*radtodeg");
	*/
}
void SSW::LoadNeutron(double inhistory,double inid,double inx,double iny,double inz,double inwx,double inwy,double ink,double inenergy,double intime,double inweight){
	history=inhistory;
	id=inid;
	x=inx;
	y=iny;
	z=inz;
	wx=inwx;
	wy=inwy;
	k=ink;
	energy=inenergy;
	time=intime;
	weight=inweight;

	i=ci(id);
	JGP=cJGP(id);
	JC=cJC(id);
	IPT=cIPT(id);
	surface=csurface(id);

	wz=cwz(wx,wy,id);

	lambda=clambda(energy);
//	l=lambda;
//	e=energy;
//	loge=log10(energy);
//	logl=log(lambda);

	r=cr(x,z);
	theta=ctheta(x,z);
	BigR=cBigR(x,y,z);
	wr=cwr(x,z);

	wtheta=cwtheta(x,z,wx,wz);
}

double SSW::history=0;
double SSW::id=0;
double SSW::x=0;
double SSW::y=0;
double SSW::z=0;
double SSW::wx=0;
double SSW::wy=0;
double SSW::k=0;
double SSW::energy=0;
double SSW::time=0;
double SSW::weight=0;

double SSW::i=0;
double SSW::JGP=0;
double SSW::JC=0;
double SSW::IPT=0;
double SSW::surface=0;
double SSW::wz=0;
double SSW::lambda=0;
double SSW::l=0;
double SSW::e=0;
double SSW::loge=0;
double SSW::logl=0;
double SSW::ModeratorHight=0;
double SSW::rsqr=0;
double SSW::radtodeg=M::rad2deg;
double SSW::r=0;
double SSW::xsign=0;
double SSW::ysign=0;
double SSW::theta=0;
double SSW::ycenter=0;
double SSW::BigRsqr=0;
double SSW::BigR=0;
double SSW::phi=0;
double SSW::wr=0;
double SSW::wxsign=0;
double SSW::wtheta0=0;
double SSW::wthetaDiff=0;
double SSW::wtheta=0;
double SSW::wphi=0;


double single_decay(double *x,double *par){
	return par[0]*exp(-x[0]/par[1]);
}

double dbl_decay(double *x,double *par){
	return fabs(par[0]*par[1]/par[3]/(par[3]-par[1])*(exp(-x[0]/par[1])-exp(-x[0]/par[3])))+par[2]*exp(-x[0]/par[3]);
}

void plotDblDecay(){
	TCanvas *c1=new TCanvas();
	c1->SetLogx();
	c1->SetLogy();
	TF1 *f_dbl_decay=new TF1("dbldcy",dbl_decay,0.001,1e6,4);
	TF1 *f_pu_decay=new TF1("pudcy",single_decay,0.001,1e6,2);
	TF1 *f_cm_decay=new TF1("cmdcy",single_decay,0.001,1e6,2);
	TF1 *f_dbl_0Pu_decay=new TF1("cmtopudcy",dbl_decay,0.001,1e6,4);
	//	par[0] = N_mother;
	//	Par[1] = Tau_N_mother;
	//	par[2] = N_daughter;
	//	par[3] = Tau_daughter;
	double par[4]={7,8500,8e3,14.2};
	f_dbl_0Pu_decay->SetParameters(par);
	f_dbl_0Pu_decay->SetParameter(2,0);
	f_dbl_decay->SetParameters(par);
	f_cm_decay->SetParameters(par);
	f_pu_decay->SetParameters(&par[2]);
	f_dbl_0Pu_decay->SetNpx(10000);
	f_dbl_decay->SetNpx(10000);
	f_pu_decay->SetNpx(10000);
	f_cm_decay->SetNpx(10000);


	f_dbl_0Pu_decay->SetLineColor(2);
	f_dbl_decay->SetLineColor(1);
	f_pu_decay->SetLineColor(6);
	f_cm_decay->SetLineColor(4);

	TLegend *l1=new TLegend(0.45,0.55,0.85,0.85);
	l1->AddEntry(f_dbl_decay,"Pu_{241}");
	l1->AddEntry(f_cm_decay,"Cm_{245}");
	l1->AddEntry(f_pu_decay,"Pu_{241} assuming no Cm_{245}");
	l1->AddEntry(f_dbl_0Pu_decay,"Pu_{241} assuming N_{Pu_{241}}^{0}=0");

	f_dbl_decay->SetTitle("Double decay (Cm_{245}^{8500years}->Pu_{241}^{14.2years})");
	f_dbl_decay->GetYaxis()->SetRangeUser(1e-3,1e5);
	f_dbl_decay->GetYaxis()->SetTitle("Weight[g]");
	f_dbl_decay->GetXaxis()->SetTitle("Time [years]");
	f_dbl_decay->Draw();
	f_cm_decay->Draw("same");
	f_pu_decay->Draw("same");
	f_dbl_0Pu_decay->Draw("same");
	f_dbl_decay->Draw("same"); // to put this on top
	l1->Draw();


	return;
}