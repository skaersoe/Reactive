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
  IMM *I=new IMM("data/");
  TH2D *h1=I->Get_r_h_Map("92235.72c","92238.72c");
  h1->Draw("colz");
  return 0;

}

int NuclearAnalyser(){
	return main();
}
