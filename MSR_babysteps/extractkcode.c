#include "TSCBasicInlineFunctions.h"
#include "TSCKcodeTools.h"

void extractkcode(){
  KcodeReader *H=new KcodeReader("exampleoutputfile.o");
  cout<<"keff:  "<<H->GetKeffFromStandartOutoutfile()<<endl;
  cout<<"error: "<<H->GetKeffFromStandartOutoutfile__keffError<<endl;
}

int main(){
  extractkcode();
  return 1;
}
