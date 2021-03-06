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
#include <fstream>
using std::ofstream;

using namespace std;
using namespace fastjet;

//MakeJets.cpp
//This scipt will analyzes data produced by POWHEG and showerd with PYTHIA8
//The events generate were p p > j j j [QCD](13TEV).Analysis was done at NLO
//NNPDF31_nlo was used for the PDF
//This scipt first clusters stable particles in to jets using fasjet
//These jets and their kinematics are then stored in an outputfile
// one with j-g-j cuts one without cuts

// a declaration of a function that pretty prints a list of jets
void print_jets (const vector<PseudoJet> &);



int main1()
{
  double px, py,pz,E;
  double temp_px,temp_py;
  double temp_pt;
  double temp_deltaphi;
  int jetnum;
  Class t;
  double R = 0.4; //jet cone radius
  double ptmin = 40.0;
  double ymin = 1.7; // minimum eta value for j-g-j
  double ymax = 4.7; // maximum eta value for j-j-j
  JetDefinition jet_def(antikt_algorithm, R, E_scheme,Best);
  int tracker = 0;
  double xsec = 38119802; // cross section in units of pb
  double lumi = 0.66; // luminosity in units of pb^-1
  int chargeCount; // number of charge particles counter
  int numEvents = 350000;

  int numCharge[10000]; 
  double jets_cut[2][3][10000]; // leading or subleading, kinematic (pt,eta,phi), event nunber
  double jet3_Pt[10000];
  double jet_multi[10000]; //jet multiplicity
  ofstream outdata;

  outdata.open("cut_jets.dat"); // opens the file
   if( !outdata ) { // file couldn't be opened
      cerr << "Error: file could not be opened" << endl;
      exit(1);
   }

  
  for(int i = 0; i<numEvents; i++) 
    {
      t.GetEntry(i);
      vector<PseudoJet> particles;
      jetnum = 0;
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
 	 
	 //leading and subleading jet kinematics & # of charged particles for j-g-j
	  if(jets.size() > 1)
	    {
	      if(abs(jets[0].eta()) < ymax && abs(jets[1].eta()) < ymax) 
		{
		  if(abs(jets[0].eta()) > ymin && abs(jets[1].eta()) > ymin)
		   {
		       if( jets[0].eta()*jets[1].eta() < 0.0)
		      	{
			  
			  jets_cut[0][0][tracker] = jets[0].pt();
			  jets_cut[0][1][tracker] = jets[0].eta();
			  jets_cut[0][2][tracker] = jets[0].phi();
			  jets_cut[1][0][tracker] = jets[1].pt();
			  jets_cut[1][1][tracker] = jets[1].eta();
			  jets_cut[1][2][tracker] = jets[1].phi();
			  jet3_Pt[tracker] = jets[2].pt();
			  jet_multi[tracker] = jets.size();
			  
		          //loops over stable particles to count number of charged partilces with |eta| < 1.0 & pt > 200 MeV (0.2 Gev)
			  chargeCount = 0;
			  for(int j = 0; j < t.kMaxParticle; j++)
			    {
			      if(t.Particle_Status[j] == 1)
				{
				  if(t.Particle_Charge[j] == 1 || t.Particle_Charge[j] == -1)
				    {
				      if(abs(t.Particle_Eta[j]) < 1.0)
					{
					  if(t.Particle_PT[j] > 0.2)
					    {
					      chargeCount = chargeCount + 1;
					    }  
					}
				      
				    } 
				}
			    }
			  numCharge[tracker] = chargeCount;
			  cout << " Number of charged particles in event = " << numCharge[tracker] << endl;
			  tracker = tracker + 1;
			    }
		     }
		}
	    }
	 

     }
      
  for(int k=0; k<tracker; k++)
    {
      for(int j=0; j<3; j++)
	{
	  for(int i=0; i<2; i++)
	    outdata << jets_cut[i][j][k] << " " ;
	}
      outdata << numCharge[k] << " " << jet3_Pt[k] << " " jet_multi[k];
      outdata << endl;
    }

  outdata.close();
  
  //print info and jets
  cout << "Clustering with " << jet_def.description() << endl;  
  cout << "Number of data points (jets): " << tracker << endl;


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
