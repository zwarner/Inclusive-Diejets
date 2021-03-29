using namespace std;

#include <TROOT.h>


int main()
{
  gROOT->ProcessLine(".L Class.C");
  gROOT->ProcessLine(".L MakeJets.cpp");
  gROOT->ProcessLine("main1()");

  return 0;
}
