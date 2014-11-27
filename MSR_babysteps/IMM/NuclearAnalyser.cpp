// **************************************************************************
// **************************************************************************
// ********************By Troels Schönfeldt, 2014****************************
// **************************************************************************
// **************************************************************************
#include <iostream>
#include <fstream>
#include <math.h>
#include "TTree.h"
#include "TFile.h"
#include "TF1.h"
#include "TF2.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TBox.h"
#include "TLine.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TRandom2.h"
#include "THnSparse.h"
#include <string>

#include "c:/WorkDir/TSC_Headers/TSCBasicInlineFunctions.h"
#include "c:/WorkDir/TSC_Headers/TSCKcodeTools.h"

using namespace std;


void WriteRootFrom(char *inname,char *outname=0){
	KcodeReader *Reader=new KcodeReader(inname);
	if(outname)Reader->CreateOutPutFile(outname);
//	int i=0;
//	int lastfound=0;
	Reader->Read2Root();
	delete Reader;
}

int ListFissionProducts(char *infile,int material,int step=-1){
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
	if(step=-1)step=laststepindex-1;
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
}



int PlotActinides(char *infile,int material){
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
	double timeinlaststep=StepToTimeHist->GetBinContent(laststepindex);
	cout<<"Number of steps (-1): "<<laststepindex<<" latest time: "<<timeinlaststep<<endl;

	char timesteps[100];
	sprintf(timesteps,"(50,0,50)");



	return 1;
}



int main(){
	gStyle->SetOptStat(0);

	IMM *I=new IMM("data/");
	TH2D *h1=I->Get_r_h_Map("92235.72c","92238.72c");
//	h1->Fill(2,2);
	h1->Draw("colz");
	return 0;

	//KcodeReader *H=new KcodeReader("case426");

	/*
	cout<<"keff:  "<<H->GetKeffFromStandartOutoutfile()<<endl;
	cout<<"error: "<<H->GetKeffFromStandartOutoutfile__keffError<<endl;
	*/
	/*
	double *K=H->Get_ListOfNumbers("radius all");

	for(int i=0;i<H->Get_ListOfNumbers__arraylength;i++){
		cout<<K[i]<<" ";
	}
	cout<<endl;
	*/
	/*	
	KcodeReader *H=new KcodeReader("outp");
	int resultline=H->FindLine("final result");
	cout<<"Result: "<<H->ReadNumberInLine(resultline,-1)<<endl;
	cout<<"Error:  "<<H->ReadNumberInLine(resultline,3)<<endl;
	*/
	//kcode2mcnpx *handler=new kcode2mcnpx("cfc_u_longer.o","u45_longer");
	//WriteRootFrom("cfc_u45_summed.o","u45_longer");
	//WriteRootFrom("cfc_th_longer.o","th_longer");
	//WriteRootFrom("cfc_u_longer.o","u_longer");
	//ListFissionProducts("u_longer",1200,20);
	//PlotActinides("th_longer",1200);
	return 1;
}

int NuclearAnalyser(){
	return main();
}