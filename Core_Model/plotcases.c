#include "../TSC_headers/TSCBasicInlineFunctions.h"
#include "../TSC_headers/TSCKcodeTools.h"

int plotcases(){
  IMM *I=new IMM("bigrun/");

  TH2D *h1=I->Get2dMap()->Clone(); // or TH2D *h1=I->Get2dMap("Keff");
  TH2D *h2=I->Get2dMap("CaptureFrac")->Clone();
  TH2D *h3=I->Get2dMap("EscapeFrac")->Clone();
  TH2D *h4=I->Get2dMap("FissionFrac")->Clone();

  TCanvas *c1=new TCanvas();
  h1->Draw("colz");
  TCanvas *c2=new TCanvas();
  h2->Draw("colz");
  TCanvas *c3=new TCanvas();
  h3->Draw("colz");
  TCanvas *c4=new TCanvas();
  h4->Draw("colz");


}
