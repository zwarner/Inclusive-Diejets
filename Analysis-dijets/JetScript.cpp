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

TH1D *h1 = new TH1D("pt1", "Dijets", 50,0,150);

TH1D *h2 = new TH1D("pt2", "Dijets", 50,0,150);

TH1D *h3 = new TH1D("pt3", "Dijets", 50,0,150);

TH1D *h4 = new TH1D("Avg Pt", "Dijets", 50,0,150);

TH1D *h5 = new TH1D("Detal Phi", "Dijet",21,0,3.2);

TH1D *h6 = new TH1D("Delta Eta", "Dijets",20,0,10);

TH1D *h7 = new TH1D("Multiplicity","Dijets",50,0,50);

TH1D *h8 = new TH1D("Number of Charged Particles","Dijets",40,0,200);

TH1D *h9 = new TH1D("Delta Eta @small Delta Phi","Dijets",15,0,10);


int main1()
{
 
  int jetnum;
  
  double ptmin = 40.0;
  int tracker = 0;
  double xsec = 5894500; // cross section in units of pb
  double xsec_error = 14018;    
  double lumi = 0.66; // luminosity in units of pb^-1
  int  numEvents = 4267893; 
  int chargeCount;
  
  double pt1[5000];
  double pt2[5000];
  double pt3[5000];
  double phi1[5000];
  double phi2[5000];
  double eta1[5000];
  double eta2[5000];
  double num_CP[5000];
  double num_jets[5000];

  double temp_DeltaPhi;

 
 

  ifstream inFile("jets_final.csv");
 
  int i = 0;

  if(inFile.is_open())
    {
       string line;
       string line1,line2,line3,line4,line5,line6,line7,line8,line9;
      while(!inFile.eof() && i < 3752 )// makes sure end of file is not reached
	{

	  if ( i < 3752 )
	    {
          getline(inFile,line1,',');
	  pt1[i] = stod(line1);
	  cout << pt1[i]  << " ";
	   
	  getline(inFile,line2,',');
	  pt2[i] = stod(line2);
	  cout << pt2[i] << " ";
	  
	  getline(inFile,line3,',');
	  eta1[i] =stod(line3);
	   cout << eta1[i] << " ";

	  getline(inFile,line4,',');
	  eta2[i] = stod(line4);
	    cout << eta2[i] << " ";
	  
	  getline(inFile,line5,',');
	  phi1[i] = stod(line5);
	  cout << phi1[i] << " ";
	  
	  getline(inFile,line6,',');
	  phi2[i] = stod(line6);
	  cout << phi2[i] << " ";
	  
	  getline(inFile,line7,',');
	  num_CP[i] = stod(line7);
	  cout << num_CP[i] << " ";

	  getline(inFile,line8,',');
	  pt3[i] = stod(line8);
	  cout << pt3[i] << " ";
	  
	  getline(inFile,line9,'\n');
	  num_jets[i] = stod(line9);
	  cout << num_jets[i] << " ";

	  cout << "i = " << i << endl;
	  i = i + 1;

	    }
	}
      
      inFile.close();
      
    }
  
  temp_DeltaPhi = 0;
  for (int j = 0; j < i-1; j++) //i-1 because last line is funky and adds extra space
     {
        h1->Fill(pt1[j]);
	h2->Fill(pt2[j]);
	h4->Fill( (pt1[j]+pt2[j]) / 2.0);
	h6->Fill(abs(eta1[j]-eta2[j]));
	
	if( abs(phi1[j]-phi2[j]) < M_PI)
	  {
	    h5->Fill(abs(phi1[j]-phi2[j]));
	    temp_DeltaPhi = abs(phi1[j]-phi2[j]); 
	  }
	else
	  {
	    h5->Fill(2*M_PI - abs(phi1[j]-phi2[j]));
	    temp_DeltaPhi =  2*M_PI - abs(phi1[j]-phi2[j]);
	  }

	if(temp_DeltaPhi < 0.4 )
	  h9->Fill(abs(eta1[j]-eta2[j]));
    
        
	h7->Fill(num_jets[j]);
	h8->Fill(num_CP[j]);
        
     }


   

  
   // normalizes the data to the cross section, integrated luminosty, and number of jets in the data
 
   h1->Scale(xsec*lumi/numEvents);
   h2->Scale(xsec*lumi/numEvents);
   // h3->Scale(xsec*lumi/numEvents);
   h4->Scale(xsec*lumi/numEvents);
   h5->Scale(xsec*lumi/numEvents);
   h6->Scale(xsec*lumi/numEvents);
   // h7->Scale(xsec*lumi/numEvents);
   h8->Scale(xsec*lumi/numEvents);
   h9->Scale(xsec*lumi/numEvents);
   
 
  auto legend1 = new TLegend();
  auto legend2 = new TLegend();
  auto legend3 = new TLegend();
  auto legend4 = new TLegend();
  TLatex cmslabel;
  TLatex infolabel;

  auto A = new TCanvas();

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);
  
  
   h1->GetXaxis()->SetTitle("#scale[1.75]{P_{T1} (GeV)}");
   h1->GetYaxis()->SetTitle("#lower[-0.2]{#scale[1.75]{#frac{d#sigma}{dP_{T1}}}}");
   
   h1->SetLineColor(kRed);
   h1->SetMarkerColor(kBlack);
   h1->SetMarkerStyle(22);

   h1->Draw("histE1");
   //h1_up->Draw("same");
   //h1_down->Draw("same");

   cmslabel.DrawLatex(0.5,0.44,"p_{Tj} > 40 GeV");
   cmslabel.DrawLatex(0.5,0.44,"R = 0.4");
   cmslabel.DrawLatex(0.5,0.44,"1.4 < |#eta_{1,2}| < 4.7");
   cmslabel.DrawLatex(0.5,0.44,"#eta_{1} #times #eta_{2} < 0");
   cmslabel.DrawLatex(0.5,0.44,"#scale[.75]{POWHEG+PYTHIA8}");

   auto B = new TCanvas();

   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(kFALSE);
  
   h2->GetXaxis()->SetTitle("#scale[1.7]{P_{T2} (GeV)}");
   h2->GetYaxis()->SetTitle("#lower[-0.1]{#scale[1.75]{#frac{d#sigma}{dP_{T2}}}}");
   h2->SetLineColor(kRed);
   h2->SetMarkerColor(kBlack);
   h2->SetMarkerStyle(22);
   h2->Draw("histE1");

   cmslabel.DrawLatex(0.5,0.44,"p_{Tj} > 40 GeV");
   cmslabel.DrawLatex(0.5,0.44,"R = 0.4");
   cmslabel.DrawLatex(0.5,0.44,"1.4 < |#eta_{1,2}| < 4.7");
   cmslabel.DrawLatex(0.5,0.44,"#eta_{1} #times #eta_{2} < 0");
   cmslabel.DrawLatex(0.5,0.44,"#scale[.75]{POWHEG+PYTHIA8}");


   auto H = new TCanvas();

   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(kFALSE);
  
   h4->GetXaxis()->SetTitle("#scale[1.7]{P_{T-avg} (GeV)}");
   h4->GetYaxis()->SetTitle("#lower[-0.2]{#scale[1.75]{#frac{d#sigma}{dP_{T-avg}}}}");
   h4->SetLineColor(kRed);
   h4->SetMarkerColor(kBlack);
   h4->SetMarkerStyle(22);
   h4->Draw("histE1");
   
   cmslabel.DrawLatex(0.5,0.44,"p_{Tj} > 40 GeV");
   cmslabel.DrawLatex(0.5,0.44,"R = 0.4");
   cmslabel.DrawLatex(0.5,0.44,"1.4 < |#eta_{1,2}| < 4.7");
   cmslabel.DrawLatex(0.5,0.44,"#eta_{1} #times #eta_{2} < 0");
   cmslabel.DrawLatex(0.5,0.44,"#scale[.75]{POWHEG+PYTHIA8}");

   auto C = new TCanvas();

   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(kFALSE);
  
  
   h5->GetXaxis()->SetTitle("#scale[1.7]{#Delta#Phi_{jj}}");
   h5->GetYaxis()->SetTitle("#lower[-0.1]{#scale[1.75]{#frac{d#sigma}{d#Delta#Phi_{jj}}}}");
   h5->SetLineColor(kRed);
   h5->SetMarkerColor(kBlack);
   h5->SetMarkerStyle(22);
   h5->Draw("histE1");

   cmslabel.DrawLatex(0.5,0.44,"p_{Tj} > 40 GeV");
   cmslabel.DrawLatex(0.5,0.44,"R = 0.4");
   cmslabel.DrawLatex(0.5,0.44," 1.4 < |#eta_{1,2}| < 4.7 ");
   cmslabel.DrawLatex(0.5,0.44,"#eta_{1} #times #eta_{2} < 0");
   cmslabel.DrawLatex(0.5,0.44,"#scale[.75]{POWHEG+PYTHIA8}");

   auto D = new TCanvas();

   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(kFALSE);
  
   h6->GetXaxis()->SetTitle("#lower[0.2]{#scale[1.7]{|#Delta#eta_{jj}|}}");
   h6->GetYaxis()->SetTitle("#lower[-0.1]{#scale[1.75]{#frac{d#sigma}{d#Delta#eta_{jj}}}}");
   h6->SetLineColor(kRed);
   h6->SetMarkerColor(kBlack);
   h6->SetMarkerStyle(22);
   h6->Draw("histE1");

   cmslabel.DrawLatex(0.5,0.44,"p_{Tj} > 40 GeV");
   cmslabel.DrawLatex(0.5,0.44,"R = 0.4");
   cmslabel.DrawLatex(0.5,0.44,"1.4 < |#eta_{1,2}| < 4.7");
   cmslabel.DrawLatex(0.5,0.44,"#eta_{1} #times #eta_{2} < 0");
   cmslabel.DrawLatex(0.5,0.44,"#scale[.75]{POWHEG+PYTHIA8}");

   auto F = new TCanvas();

   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(kFALSE);
  
   h7->GetXaxis()->SetTitle("#scale[1.7]{N_{jets}}");
   h7->GetYaxis()->SetTitle("#scale[1.5]{Number of Monte Carlo Events}");
   h7->SetLineColor(kRed);
   h7->SetMarkerColor(kBlack);
   h7->SetMarkerStyle(22);
   h7->Draw("histE1");

   cmslabel.DrawLatex(0.5,0.44,"p_{Tj} > 40 GeV");
   cmslabel.DrawLatex(0.5,0.44,"R = 0.4");
   cmslabel.DrawLatex(0.5,0.44,"1.4 < |#eta_{1,2}| < 4.7");
   cmslabel.DrawLatex(0.5,0.44,"#eta_{1} #times #eta_{2} < 0");
   cmslabel.DrawLatex(0.5,0.44,"#scale[.75]{POWHEG+PYTHIA8}");

   auto G = new TCanvas();

   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(kFALSE);
  
   h8->GetXaxis()->SetTitle("#scale[1.7]{N_{cp}}");
   h8->GetYaxis()->SetTitle("#scale[1.5]{Number of Monte Carlo Events}");
   h8->SetLineColor(kRed);
   h8->SetMarkerColor(kRed);
   h8->SetMarkerStyle(22);
   h8->Draw("histE1");

   cmslabel.DrawLatex(0.5,0.44,"P_{Tj} > 40 GeV");
   cmslabel.DrawLatex(0.5,0.44,"R = 0.4");
   cmslabel.DrawLatex(0.5,0.44,"1.4 < |#eta_{1,2}| < 4.7");
   cmslabel.DrawLatex(0.5,0.44,"#eta_{1} #times #eta_{2} < 0");
   cmslabel.DrawLatex(0.5,0.55,"P_{Tcp} > 1.0 GeV");
   cmslabel.DrawLatex(0.5,0.55,"|#eta_{cp}| < 1.0");
   cmslabel.DrawLatex(0.5,0.44,"#scale[.75]{POWHEG+PYTHIA8}");

   auto I = new TCanvas();

   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(kFALSE);
  
   h9->GetXaxis()->SetTitle("#lower[0.2]{#scale[1.7]{|#Delta#eta_{jj}|}}");
   h9->GetYaxis()->SetTitle("#lower[-0.1]{#scale[1.75]{#frac{d#sigma}{d#Delta#eta_{jj}}}}");
   h9->SetLineColor(kRed);
   h9->SetMarkerColor(kBlack);
   h9->SetMarkerStyle(22);
   h9->Draw("histE1");

   cmslabel.DrawLatex(0.5,0.44,"p_{Tj} > 40 GeV");
   cmslabel.DrawLatex(0.5,0.44,"R = 0.4");
   cmslabel.DrawLatex(0.5,0.44,"1.4 < |#eta_{1,2}| < 4.7");
   cmslabel.DrawLatex(0.5,0.44,"#eta_{1} #times #eta_{2} < 0");
   cmslabel.DrawLatex(0.5,0.44,"#Delta#phi_{jj} < 0.4");
   cmslabel.DrawLatex(0.5,0.44,"#scale[.75]{POWHEG+PYTHIA8}");
   
   
  
  
  return 0;

}

