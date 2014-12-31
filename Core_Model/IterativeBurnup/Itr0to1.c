#include "~Reactive/TSC_Headers/TSCBasicInlineFunctions.h"
#include "~Reactive/TSC_Headers/TSCKcodeTools.h"

using namespace std;

int main(){
    gStyle->SetOptStat(0);

    // loading Material file    
    KcodeReader *MaterialKKR=new KcodeReader("MaterialFile");
    // loading deck0
    KcodeReader *Deck0=new KcodeReader("deck0");
    //loading outputfile
    KcodeReader *outp=new KcodeReader("Run0.o");

    // building reprocessor
    Reprocessor *R=new Reprocessor("example_reprocessor",MaterialKKR);
    // buliding FullBurnoutRun
    FullBurnupRun *Run=new FullBurnupRun(outp,T2E(800.),-1,-1,false);
    
    // running reprocessor
    R->RunReprocessor(1,Run,1.02,1001,2000,"Run0to1Log",false);

    // generating new deck
    DeckMatReplacer *DMR=new DeckMatReplacer(Run->GetMaterialPtr(1001,3)->PrintMaterial(),Deck0,"deck1",true);
}

int Itr0to1(){

  return main();
}
