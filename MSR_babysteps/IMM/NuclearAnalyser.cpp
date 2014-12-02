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

#include "headers/TSCBasicInlineFunctions.h"
#include "headers/TSCKcodeTools.h"

using namespace std;


int main(){
  gStyle->SetOptStat(0);
  /*  
  TFile *_f=new TFile("2001_1001.root","RECREATE");

  IMM *I=new IMM("/tmp/pstudy/1001_2001/");
  TH2D* h1=I->Get_r_h_Map("92233.72c","92238.72c","2001","1001");
  TH2D* h2=I->Get_r_h_Map("92235.72c","92238.72c","2001","1001");
  TH2D* h3=I->Get_r_h_Map("94239.72c","92238.72c","2001","1001");
  TH2D* h4=I->Get_r_h_Map("94241.72c","92238.72c","2001","1001");
  TH2D* h5=I->Get_r_h_Map("92233.72c","90232.72c","2001","1001");
  TH2D* h6=I->Get_r_h_Map("92235.72c","90232.72c","2001","1001");
  TH2D* h7=I->Get_r_h_Map("94239.72c","90232.72c","2001","1001");
  TH2D* h8=I->Get_r_h_Map("94241.72c","90232.72c","2001","1001");
  _f->Write();
  _f->Close();

  TFile *_f1=new TFile("2002_1001.root","RECREATE");

  IMM *I=new IMM("/tmp/pstudy/1002_2001/");
  TH2D* h11=I->Get_r_h_Map("92233.72c","92238.72c","2001","1002");
  TH2D* h12=I->Get_r_h_Map("92235.72c","92238.72c","2001","1002");
  TH2D* h13=I->Get_r_h_Map("94239.72c","92238.72c","2001","1002");
  TH2D* h14=I->Get_r_h_Map("94241.72c","92238.72c","2001","1002");
  TH2D* h15=I->Get_r_h_Map("92233.72c","90232.72c","2001","1002");
  TH2D* h16=I->Get_r_h_Map("92235.72c","90232.72c","2001","1002");
  TH2D* h17=I->Get_r_h_Map("94239.72c","90232.72c","2001","1002");
  TH2D* h18=I->Get_r_h_Map("94241.72c","90232.72c","2001","1002");
  _f1->Write();
  _f1->Close();
  
  TFile *_f2=new TFile("2001_1003.root","RECREATE");

  IMM *I=new IMM("/tmp/pstudy/1003_2001/");
  TH2D* h21=I->Get_r_h_Map("92233.72c","92238.72c","2001","1003");
  TH2D* h22=I->Get_r_h_Map("92235.72c","92238.72c","2001","1003");
  TH2D* h23=I->Get_r_h_Map("94239.72c","92238.72c","2001","1003");
  TH2D* h24=I->Get_r_h_Map("94241.72c","92238.72c","2001","1003");
  TH2D* h25=I->Get_r_h_Map("92233.72c","90232.72c","2001","1003");
  TH2D* h26=I->Get_r_h_Map("92235.72c","90232.72c","2001","1003");
  TH2D* h27=I->Get_r_h_Map("94239.72c","90232.72c","2001","1003");
  TH2D* h28=I->Get_r_h_Map("94241.72c","90232.72c","2001","1003");
  _f2->Write();
  _f2->Close();
  */
  TFile *_f3=new TFile("2001_1004.root","RECREATE");

  IMM *I=new IMM("/tmp/pstudy/1004_2001/");
  TH2D* h31=I->Get_r_h_Map("92233.72c","92238.72c","2001","1004");
  TH2D* h32=I->Get_r_h_Map("92235.72c","92238.72c","2001","1004");
  TH2D* h33=I->Get_r_h_Map("94239.72c","92238.72c","2001","1004");
  TH2D* h34=I->Get_r_h_Map("94241.72c","92238.72c","2001","1004");
  TH2D* h35=I->Get_r_h_Map("92233.72c","90232.72c","2001","1004");
  TH2D* h36=I->Get_r_h_Map("92235.72c","90232.72c","2001","1004");
  TH2D* h37=I->Get_r_h_Map("94239.72c","90232.72c","2001","1004");
  TH2D* h38=I->Get_r_h_Map("94241.72c","90232.72c","2001","1004");
  _f3->Write();
  _f3->Close();

  TFile *_f4=new TFile("2001_1005.root","RECREATE");

  IMM *I=new IMM("/tmp/pstudy/1005_2001/");
  TH2D* h41=I->Get_r_h_Map("92233.72c","92238.72c","2001","1005");
  TH2D* h42=I->Get_r_h_Map("92235.72c","92238.72c","2001","1005");
  TH2D* h43=I->Get_r_h_Map("94239.72c","92238.72c","2001","1005");
  TH2D* h44=I->Get_r_h_Map("94241.72c","92238.72c","2001","1005");
  TH2D* h45=I->Get_r_h_Map("92233.72c","90232.72c","2001","1005");
  TH2D* h46=I->Get_r_h_Map("92235.72c","90232.72c","2001","1005");
  TH2D* h47=I->Get_r_h_Map("94239.72c","90232.72c","2001","1005");
  TH2D* h48=I->Get_r_h_Map("94241.72c","90232.72c","2001","1005");
  _f4->Write();
  _f4->Close();




}

int NuclearAnalyser(){
	return main();
}
