using namespace std;

#include <TROOT.h>


int main()
{
  gROOT->ProcessLine(".L JetScript.cpp");
  gROOT->ProcessLine("main1()");

  return 0;
}
