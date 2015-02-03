#include "/users/trsch/Reactive/TSC_headers/TSCBasicInlineFunctions.h"
#include "/users/trsch/Reactive/TSC_headers/TSCKcodeTools.h"

using namespace std;

int main(){
  gStyle->SetOptStat(0);

  TH1D *h104=(TH1D*)TSC::SparseLoader("mctal","f104",6);
  TSC::NameHist(h104,1,"","Lethargy (log(E)) [MeV]","Flux [cm^{-2}s^{-1}]");

  TH1D *h204=(TH1D*)TSC::SparseLoader("mctal","f204",6);
  TSC::NameHist(h204,2,"","Lethargy (log(E)) [MeV]","Flux [cm^{-2}s^{-1}]");

  TH1D *h304=(TH1D*)TSC::SparseLoader("mctal","f304",6);
  TSC::NameHist(h304,4,"","Lethargy (log(E)) [MeV]","Flux [cm^{-2}s^{-1}]");

  TLegend *l8=new TLegend(.65,.65,.9,.9);
  l8->AddEntry(h104,"Inner fuel","l");
  l8->AddEntry(h204,"Moderator","l");
  l8->AddEntry(h304,"Outer fuel","l");


  TCanvas *c8=new TCanvas();
  h104->GetXaxis()->SetRangeUser(1e-8,10);
  c8->SetLogx();
  h104->Draw("hist");
  h204->Draw("same hist");
  h304->Draw("same hist");
  l8->Draw();

  THnSparseF *S3=TSC::SparseLoader("mctal","f1");

  //  S3->GetAxis(10)->SetRangeUser(-200,200);
  TH2D *xy=(TH2D*)S3->Projection(9,8);
  TH1D *x=(TH1D*)S3->Projection(8);
  TH1D *y=(TH1D*)S3->Projection(9);

  TCanvas *c9=new TCanvas();
  xy->Draw("colz");

  TCanvas *c10=new TCanvas();
  x->Draw();

  TCanvas *c11=new TCanvas();
  y->Draw();





}


int Plot(){
  return main();
}
