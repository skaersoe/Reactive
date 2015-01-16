#include "/users/trsch/Reactive/TSC_headers/TSCBasicInlineFunctions.h"
#include "/users/trsch/Reactive/TSC_headers/TSCKcodeTools.h"

using namespace std;

int main(){
    gStyle->SetOptStat(0);

    KcodeReader *outp;
    // loading Material file
    KcodeReader *MaterialKKR=new KcodeReader("MaterialFile_5StepsPerYear");
    if(!MaterialKKR->__file_exist){cout<<"Did not find a valid material file... Ending"<<endl;return 0;}

    // checking for a reprocessor
    KcodeReader *reproc=new KcodeReader("example_reprocessor");
    if(!reproc->__file_exist){cout<<"Did not find a valid reprocessor input file... Ending"<<endl;return 0;}
    delete reproc;

    // loading deck0
    KcodeReader *Deck0=new KcodeReader("deck0");
    if(!Deck0->__file_exist){cout<<"Did not find a valid deck0 file... Ending"<<endl;return 0;}

    //loading outputfile
    int MaterialStep=0;
    char outpfilename[100];
    char nextdeckfilename[100];
    char logfilename[100];
    for(int i=100;i>=0;i--){
      sprintf(outpfilename,"run%d.o",i);
      //      cout<<outpfilename<<endl;
      KcodeReader *outp=new KcodeReader(outpfilename,false);
      if(!outp->__file_exist)continue;
      sprintf(logfilename,"LogFileForRun%d",i+1);
      sprintf(nextdeckfilename,"deck%d",i+1);
      MaterialStep=i+2;
      break;
    }
    if(!outp->__file_exist){cout<<"Did not find a valid MCNPX output file... Ending"<<endl;return 0;}
    cout<<"Will load: "<<outpfilename<<" and generate: "<<nextdeckfilename<<" and "<<logfilename<<endl;


    // building reprocessor
    Reprocessor *R=new Reprocessor("example_reprocessor",MaterialKKR,MaterialStep,true);

    // buliding FullBurnoutRun
    FullBurnupRun *Run=new FullBurnupRun(outp,T2E(800.),-1,-1,false,true);
    // running reprocessor
    R->RunReprocessor(2,Run,1.0005,23250000,1001,2000,logfilename,true);

    // generating new deck
    DeckMatReplacer *DMR=new DeckMatReplacer(Run->GetMaterialPtr(1001,2),Deck0,nextdeckfilename,false);

}

int GenerateNextDeck(){
  return main();
}
