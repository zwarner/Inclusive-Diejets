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
TH1D *h1 = new TH1D("pt1", "Dijets", 15,0,150);
TH1D *h2 = new TH1D("pt2", "Dijets", 15,0,150);
TH1D *h3 = new TH1D("Detal Phi", "Dijets",8,0,3.2);
TH1D *h4 = new TH1D("Delta Eta", "Dijets",20,0,10);
TH1D *h5 = new TH1D("Multiplicity","Dijets",50,0,50);
TH1D *h6 = new TH1D("Number of Charged Particles","Dijets",20,0,200);




int main1()
{
  double px, py,pz,E;
  double temp_px,temp_py;
  double temp_pt;
  double temp_deltaphi;
  int numjets;

  double R = 0.4;
  double ptmin = 40.0;
  double ymax = 4.7;
  double ymin = 1.4;
  JetDefinition jet_def(antikt_algorithm, R, E_scheme,Best);

  double xsec = 6696983174; // cross section in units of pb
  double lumi = 0.66; // luminosity in units of pb^-1
  int  numEvents = 461650; 
  int chargeCount;

  Class t;
  numjets = 0;
  
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
	 
	   if(jets.size()>1)
	     {
	       if(abs(jets[0].eta()) < 5.0 && abs(jets[1].eta()) < 5.0)
		  numjets = numjets + 1;
	       }
	 
	 
	   cout << numjets << endl;
	  
  
     }
      
 

  //print info and jets
  cout << "jet events " << numjets << endl;  
  
  
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
