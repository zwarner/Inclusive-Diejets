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
#include "fastjet/ClusterSequence.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"
#include "fastjet/tools/Subtractor.hh"

using namespace std;
using namespace fastjet;

//Jet.cpp
//This scipt first clusters stable partiles in to jets using fasjet
//This scipt will analyzes data produced by POWHEG  and showerd with PYTHIA8
//The events generate were p p > j j  [QCD](13TEV).Analysis was done at NLO
//LHAPDF 303400 nlo was used for the PDF


// a declaration of a function that pretty prints a list of jets
void print_jets (const vector<PseudoJet> &);
TH1D *h1 = new TH1D("pt1", "Dijets", 15,0,250);
TH1D *h2 = new TH1D("pt2", "Dijets", 15,0,150);
TH1D *h3 = new TH1D("Detal Phi", "Dijets",8,0,3.2);
TH1D *h4 = new TH1D("Delta Eta", "Dijets",10,0,10);
TH1D *h5 = new TH1D("Multiplicity","Dijets",50,0,50);
TH1D *h6 = new TH1D("Number of Charged Particles","Dijets",20,0,200);




int main1()
{
  double px, py,pz,E;
  double temp_px,temp_py;
  double temp_pt;
  double temp_deltaphi;
  int jetnum;

  double R = 0.4;
  double ptmin = 40.0;
  double ymax = 4.7;
  double ymin = 1.4;
  JetDefinition jet_def(antikt_algorithm, R, E_scheme,Best);
  int tracker = 0;
  double xsec = 6696983174; // cross section in units of pb
  double lumi = 0.66; // luminosity in units of pb^-1
  int  numEvents = 350000; 
  int chargeCount;

  Class_NLO3 t;

  for(int i = 0; i<numEvents; i++) 
    {
      t.GetEntry(i);
      vector<PseudoJet> particles;
    
      for(int j = 0; j < t.kMaxParticle; j++)//loops over stable particles
	{
	 if(t.Particle_Status[j] == 1) // makes sure the particle is stable and creates a PseudoJet vector
           {
	      temp_px = t.Particle_Px[j];
	      temp_py = t.Particle_Py[j];

	      if(sqrt(temp_px*temp_px + temp_py*temp_py) > .0001 && sqrt(temp_px*temp_px + temp_py*temp_py) < 13000 )//makes sure values are reasonable (real)
		 {
		     px = t.Particle_Px[j];
		     py = t.Particle_Py[j];
		     pz = t.Particle_Pz[j];
		     E =  t.Particle_E[j];
		     particles.push_back(PseudoJet(px,py,pz,E));
		  }
	     }
	  }

      
	 ClusterSequence cs(particles, jet_def);
	 vector<PseudoJet> jets =sorted_by_pt(cs.inclusive_jets(ptmin));

	 
	 //all jets
	 /*
	 for(int j = 0; j<jets.size(); j++)
	   {
	     h1->Fill(jets[j].pt());
	     //tracker = tracker + 1;
	   }
	 */
	 
	 
	  if(jets.size() > 1)
	    {
	      if(abs(jets[0].eta()) < ymax && abs(jets[1].eta()) < ymax) 
		{
		  if(abs(jets[0].eta()) > ymin && abs(jets[1].eta()) > ymin)
		   {
		       if( jets[0].eta()*jets[1].eta() < 0.0)
		      	{
			  h1->Fill(jets[0].pt());
			  h2->Fill(jets[1].pt());
			  h4->Fill(abs(jets[0].eta()-jets[1].eta()));

			  //loops over stable particles to count number of charged partilces with |eta| < 1.0 & pt > 200 MeV (0.2 Gev)
			  chargeCount = 0;
			  for(int j = 0; j < t.kMaxParticle; j++)
			    {
			      if(t.Particle_Status[j] == 1)
				  if(t.Particle_Charge[j] == 1 || t.Particle_Charge[j] == -1)
				    if(abs(t.Particle_Eta[j]) < 1.0 && t.Particle_PT[j] > 0.2)
				       chargeCount = chargeCount + 1;
			        
			    }
			  h6->Fill(chargeCount);
		      	}
		     }
		}
	    }
	 
	 /*
	  if(jets.size() > 1)
	    {
	      if(abs(jets[0].rap()) < ymax)
		{
		  h1->Fill(jets[0].pt());
		}
	    }
	 */
	 /*
	 //two leading jets
	  if(jets.size()>1)
	  {
	    // if(abs(jets[0].rap()) < 5.0)
	    // h1->Fill(jets[0].rap());
	    // h1->Fill(jets[0].rap()-jets[1].rap());
	    //  tracker = tracker + 1;
	     // h1->Fill((jets[0].pt() + jets[1].pt())/2);
	  }
	 */

	 
	 //Delta phi
	  if(jets.size()>1)
	  {
	    if(abs(jets[0].eta()) < ymax && abs(jets[1].eta()) < ymax)
	      {
		if(abs(jets[0].eta()) > ymin && abs(jets[1].eta()) > ymin)
		  {
		   if( jets[0].eta()*jets[1].eta() < 0.0 )
		     {
			 if(abs(jets[0].phi()-jets[1].phi()) < M_PI)
			   h3->Fill(abs(jets[0].phi()-jets[1].phi()));
			 else
			   h3->Fill(2*M_PI - abs(jets[0].phi()-jets[1].phi()));
		     }
		  }
	      }
	     
	  }
	  
	  
	 /*
	  //Delta y @ small delta phi 
	  temp_deltaphi = 0;
	   if(jets.size()>1)
	  {
	    if(abs(jets[0].rap()) < ymax) 
	      {
		if(abs(jets[0].phi()-jets[1].phi()) < M_PI)
		 temp_deltaphi=abs(jets[0].phi()-jets[1].phi());
		else
		  temp_deltaphi=(2*M_PI - abs(jets[0].phi()-jets[1].phi()));
		
		if(temp_deltaphi < 0.4)
		  h1->Fill(jets[0].rap()-jets[1].rap());
		
	      }
	     
	  }

	 */
	   

	 h5->Fill(jets.size());
  
     }
      
 

  //print info and jets
  cout << "Clustering with " << jet_def.description() << endl;  
  cout << "Number of pt1 jets: " << h1->Integral() << endl;
  cout << "Number of pt2 jets: " << h2->Integral() << endl;
  cout << "Number of #Delta#Phi jets: " << h3->Integral() << endl;
  cout << "Number of #Delta#eta jets: " << h4->Integral() << endl;


   // normalizes the data to the cross section, integrated luminosty, and number of jets in the data
 
   h1->Scale(xsec*lumi/numEvents);
   h2->Scale(xsec*lumi/numEvents);
   h3->Scale(xsec*lumi/numEvents);
   h4->Scale(xsec*lumi/numEvents);
   
 
  auto legend1 = new TLegend();
  auto legend2 = new TLegend();
  auto legend3 = new TLegend();
  auto legend4 = new TLegend();
  TLatex cmslabel;
  TLatex infolabel;

  auto A = new TCanvas();

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);
  
  
   h1->GetXaxis()->SetTitle("#scale[1.75]{Pt_{1}}");
   h1->GetYaxis()->SetTitle("#lower[-0.2]{#scale[1.75]{#frac{d#sigma}{dPt_{1}}}}");
   h1->SetLineColor(kRed);
   h1->Draw("histE1 pfc");

   cmslabel.DrawLatex(0.5,0.44,"pt > 40 Gev");
   cmslabel.DrawLatex(0.5,0.44,"R = 0.4");
   cmslabel.DrawLatex(0.5,0.44,"1.4 < |#eta| < 4.7");

   auto B = new TCanvas();

   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(kFALSE);
  
   h2->GetXaxis()->SetTitle("#scale[1.75]{Pt_{2}}");
   h2->GetYaxis()->SetTitle("#lower[-0.2]{#scale[1.75]{#frac{d#sigma}{dPt_{2}}}}");
   h2->SetLineColor(kRed);
   h2->Draw("histE1 pfc");

   cmslabel.DrawLatex(0.5,0.44,"pt > 40 Gev");
   cmslabel.DrawLatex(0.5,0.44,"R = 0.4");
   cmslabel.DrawLatex(0.5,0.44,"1.4 < |#eta| < 4.7");

   auto C = new TCanvas();

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(kFALSE);
  
   
   h3->GetXaxis()->SetTitle("#scale[1.75]{#Delta#Phi_{jj}}");
   h3->GetYaxis()->SetTitle("#lower[-0.3]{#scale[1.75]{#frac{d#sigma}{d#Delta#Phi_{jj}}}}");
   h3->SetLineColor(kRed);
   h3->Draw("histE1 pfc");

   cmslabel.DrawLatex(0.5,0.44,"pt > 40 Gev");
   cmslabel.DrawLatex(0.5,0.44,"R = 0.4");
   cmslabel.DrawLatex(0.5,0.44," 1.4 < |#eta| < 4.7 ");

   auto D = new TCanvas();

   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(kFALSE);
  
   h4->GetXaxis()->SetTitle("#scale[1.75]{|#Delta#eta_{jj}|}");
   h4->GetYaxis()->SetTitle("#lower[-0.1]{#scale[1.75]{#frac{d#sigma}{d#Delta#eta_{jj}}}}");
   h4->SetLineColor(kRed);
   h4->Draw("histE1 pfc");

   cmslabel.DrawLatex(0.5,0.44,"pt > 40 Gev");
   cmslabel.DrawLatex(0.5,0.44,"R = 0.4");
   cmslabel.DrawLatex(0.5,0.44,"1.4 < |#eta| < 4.7");

    auto F = new TCanvas();

   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(kFALSE);
  
   h5->GetXaxis()->SetTitle("#scale[1.75]{N_{jets}}");
   h5->GetYaxis()->SetTitle("");
   h5->SetLineColor(kRed);
   h5->Draw("histE1 pfc");

   cmslabel.DrawLatex(0.5,0.44,"pt > 40 Gev");
   cmslabel.DrawLatex(0.5,0.44,"R = 0.4");
   cmslabel.DrawLatex(0.5,0.44,"1.4 < |#eta| < 4.7");

    auto G = new TCanvas();

   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(kFALSE);
  
   h6->GetXaxis()->SetTitle("#scale[1.75]{N_{cp}}");
   h6->GetYaxis()->SetTitle("");
   h6->SetLineColor(kRed);
   h6->Draw("histE1 pfc");

   cmslabel.DrawLatex(0.5,0.44,"PT_{j} > 40 Gev");
   cmslabel.DrawLatex(0.5,0.44,"R = 0.4");
   cmslabel.DrawLatex(0.5,0.44,"1.4 < |#eta_{j}| < 4.7");
   cmslabel.DrawLatex(0.5,0.55,"PT_{cp} > 200 MeV");  
   cmslabel.DrawLatex(0.5,0.55,"|#eta_{cp}| < 1.0");  
  
  return 0;

}
//----------------------------------------------------------------------
/// a function that pretty prints a list of jets
void print_jets (const vector<PseudoJet> & jets)
{

  // sort jets into increasing pt
  vector<PseudoJet> sorted_jets = sorted_by_pt(jets);  

  // label the columns
  printf("%5s %15s %15s %15s %15s\n","jet #", "rapidity", 
	 "phi", "pt", "n constituents");
  
  // print out the details for each jet
  for (unsigned int i = 0; i < sorted_jets.size(); i++) {
    // the following is not super efficient since it creates an
    // intermediate constituents vector
    int n_constituents = sorted_jets[i].constituents().size();
    printf("%5u %15.8f %15.8f %15.8f %8u\n",
	   i, sorted_jets[i].rap(), sorted_jets[i].phi(),
	   sorted_jets[i].perp(), n_constituents);
  }

  }
