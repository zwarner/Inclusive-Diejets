#include <TH1D.h>
#include <stdlib.h>
#include <TROOT.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <TLorentzVector.h>
#include <algorithm>
#include "TMath.h"
#include "TVector3.h"
#include "TRotation.h"
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>



using namespace std;


//Jet.cpp
//This scipt first clusters stable partiles in to jets using fasjet
//This scipt will analyzes data produced by POWHEG  and showerd with PYTHIA8
//The events generate were p p > j j  [QCD](13TEV).Analysis was done at NLO
//LHAPDF 303400 nlo was used for the PDF



//declar histograms and their upper/lower errors

TH1D *h1 = new TH1D("NCP 0.2 GeV","Dijets",40,0,200);
TH1D *h2 = new TH1D("NCP 0.5 GeV","Dijets",60,0,200);
TH1D *h3 = new TH1D("NCP 1.0 GeV","Dijets",70,0,200);


int main1()
{
 
  int jetnum;
  
  double ptmin = 40.0;
  int tracker = 0;
  double xsec = 5894500000; // cross section in units of pb
  double xsec_error = 14018000;    
  double lumi = 0.66; // luminosity in units of pb^-1
  int  numEvents = 4267893; 
  int chargeCount;
  

  double num_CP02[5000];
  double num_CP05[5000];
  double num_CP10[5000];


  ifstream inFile1("jets_pt02.csv");
  ifstream inFile2("jets_pt05.csv");
  ifstream inFile3("jets_pt10.csv");
 
  int i = 0;
  int k = 0;
  int l = 0;

  if(inFile1.is_open())
    {
      
       string line;
       string line1,line2,line3,line4,line5,line6,line7,line8,line9;
      while(!inFile1.eof() && i < 335 )// makes sure end of file is not reached
	{

	  if ( i < 335 )
	    {
          getline(inFile1,line1,',');	 
	   
	  getline(inFile1,line2,',');
	  	  
	  getline(inFile1,line3,',');       

	  getline(inFile1,line4,',');
        	  
	  getline(inFile1,line5,',');
	         
	  
	  getline(inFile1,line6,',');
	 	  
	  getline(inFile1,line7,',');
	  num_CP02[i] = stod(line7);
	  cout << num_CP02[i] << " ";

	  getline(inFile1,line8,',');

	  getline(inFile1,line9,'\n');

	  cout << "i = " << i << endl;
	  i = i + 1;

	    }
	}
      
      inFile1.close();
      
    }

   if(inFile2.is_open())
    {
       
       string line;
       string line1,line2,line3,line4,line5,line6,line7,line8,line9;
      while(!inFile2.eof() && k < 335 )// makes sure end of file is not reached
	{

	  if ( k < 335 )
	    {
          getline(inFile2,line1,',');	 
	   
	  getline(inFile2,line2,',');
	  	  
	  getline(inFile2,line3,',');       

	  getline(inFile2,line4,',');
        	  
	  getline(inFile2,line5,',');
	         
	  getline(inFile2,line6,',');
	 	  
	  getline(inFile2,line7,',');
	  num_CP05[i] = stod(line7);
	  cout << num_CP05[i] << " ";

	  getline(inFile2,line8,',');

	  getline(inFile2,line9,'\n');

	  cout << "k = " << k << endl;
	  k = k + 1;

	    }
	}
      
      inFile2.close();
      
    }
 
 if(inFile3.is_open())
    {
              
       string line;
       string line1,line2,line3,line4,line5,line6,line7,line8,line9;
      while(!inFile3.eof() && l < 335 )// makes sure end of file is not reached
	{

	  if ( l < 335 )
	    {
          getline(inFile3,line1,',');	 
	   
	  getline(inFile3,line2,',');
	  	  
	  getline(inFile3,line3,',');       

	  getline(inFile3,line4,',');
        	  
	  getline(inFile3,line5,',');
	         
	  
	  getline(inFile3,line6,',');
	 	  
	  getline(inFile3,line7,',');
	  num_CP10[i] = stod(line7);
	  cout << num_CP10[i] << " ";

	  getline(inFile3,line8,',');

	  getline(inFile3,line9,'\n');

	  cout << "l = " << l << endl;
	  l = l + 1;

	    }
	}
      
      inFile3.close();
      
    }
   

  for (int j = 0; j < i-1; j++) //i-1 because last line is funky and adds extra space
     {
       
	h1->Fill(num_CP02[j]);
	h2->Fill(num_CP05[j]);
	h3->Fill(num_CP10[j]);
        
     }


   

  
   // normalizes the data to the cross section, integrated luminosty, and number of jets in the data
 
  // h1->Scale(xsec*lumi/numEvents);
  // h2->Scale(xsec*lumi/numEvents);
  // h3->Scale(xsec*lumi/numEvents);
   
   

  auto legend1 = new TLegend(0.25,0.5,0.5,0.75);
  auto legend2 = new TLegend();
  auto legend3 = new TLegend();
  auto legend4 = new TLegend();
  TLatex cmslabel;
  TLatex infolabel;

  
   auto G = new TCanvas();

   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(kFALSE);
  
   h3->GetXaxis()->SetTitle("#scale[1.75]{N_{cp}}");
   h3->GetYaxis()->SetTitle("#scale[1.75]{N_{events}}");
   h3->GetYaxis()->SetTitle("");
   h1->SetLineColor(kBlack);
   h1->SetMarkerColor(kBlack);
   h1->SetMarkerStyle(2);
   h2->SetLineColor(kRed);
   h2->SetMarkerColor(kRed);
   h2->SetMarkerStyle(2);
   h3->SetLineColor(kBlue);
   h3->SetMarkerColor(kBlue);
   h3->SetMarkerStyle(2);
  
   h3->Draw("histE1");
   h2->Draw("same histeE1");
   h1->Draw("same histeE1");

   legend1->AddEntry(h1,"p_{T-CP} > 0.2 GeV","l");
   legend1->AddEntry(h2,"p_{T-CP} > 0.5 GeV","l");
   legend1->AddEntry(h3,"p_{T-CP} > 1.0 GeV","l");
   legend1->Draw();
   
   cmslabel.DrawLatex(0.5,0.44,"P_{Tj} > 40 Gev");
   cmslabel.DrawLatex(0.5,0.44,"R = 0.4");
   cmslabel.DrawLatex(0.5,0.44,"1.4 < |#eta_{1,2}| < 4.7");
   cmslabel.DrawLatex(0.5,0.55,"P_{Tcp} > 1.0 GeV");  
   cmslabel.DrawLatex(0.5,0.55,"|#eta_{cp}| < 1.0");
   cmslabel.DrawLatex(0.5,0.44,"#scale[.75]{POWHEG+PYTHIA8}");

  
  
  return 0;

}

