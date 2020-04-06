/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofTrimOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofTrimMappedData.h"
#include "THttpServer.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "TCanvas.h"
#include "TFolder.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TVector3.h"

#include "TClonesArray.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TMath.h"
#include "TRandom.h"
#include <array>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

R3BSofTrimOnlineSpectra::R3BSofTrimOnlineSpectra()
    : FairTask("SofTrimOnlineSpectra", 1)
    , fMappedItemsTrim(NULL)
    , fNEvents(0)
{
}

R3BSofTrimOnlineSpectra::R3BSofTrimOnlineSpectra(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMappedItemsTrim(NULL)
    , fNEvents(0)
{
}

R3BSofTrimOnlineSpectra::~R3BSofTrimOnlineSpectra()
{
    LOG(INFO) << "R3BSofTrimOnlineSpectra::Delete instance";
    if (fMappedItemsTrim)
        delete fMappedItemsTrim;
}

InitStatus R3BSofTrimOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofTrimOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTrimOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // === get access to mapped data of the TRIPLE-MUSIC
    fMappedItemsTrim = (TClonesArray*)mgr->GetObject("TrimMappedData");
    if (!fMappedItemsTrim)
    {
        return kFATAL;
    }

    // === Create histograms for detectors
    char Name1[255];
    char Name2[255];

    // --- TRIM: 1-D MAPPED data for ENERGY, DRIFT-TIME and MULTIPLICITY
    for (Int_t i = 0; i < NbSectionsTrim; i++){
      sprintf(Name1, "Trim_Emap_Sec_%d",i+1);
      sprintf(Name2, "Section %d",i+1);
      cTrimMap_E[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
      cTrimMap_E[i]->Divide(2,3); // 2 col and 3 raws (if triangular anodes: one raw per pair)

      sprintf(Name1, "Trim_DTmap_Sec_%d",i+1);
      sprintf(Name2, "Section %d",i+1);
      cTrimMap_DT[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
      cTrimMap_DT[i]->Divide(2,3);

      for (Int_t j = 0; j < NbAnodesTrim; j++){
	sprintf(Name1, "fh1_trim_Emap_sec%d_a%d", i+1, j+1);
	sprintf(Name2, "Sec %d:Anode %d", i+1, j+1);
	fh1_trimmap_E[i][j] = new TH1F(Name1, Name2, 8192, 0, 8192);
	fh1_trimmap_E[i][j]->GetXaxis()->SetTitle("Raw Energy [channels], if mult==1");
	fh1_trimmap_E[i][j]->GetYaxis()->SetTitle("Counts");
	fh1_trimmap_E[i][j]->GetYaxis()->SetTitleOffset(1.1);
	fh1_trimmap_E[i][j]->GetXaxis()->CenterTitle(true);
	fh1_trimmap_E[i][j]->GetYaxis()->CenterTitle(true);
	fh1_trimmap_E[i][j]->GetXaxis()->SetLabelSize(0.045);
	fh1_trimmap_E[i][j]->GetXaxis()->SetTitleSize(0.045);
	fh1_trimmap_E[i][j]->GetYaxis()->SetLabelSize(0.045);
	fh1_trimmap_E[i][j]->GetYaxis()->SetTitleSize(0.045);
	fh1_trimmap_E[i][j]->SetFillColor(31);
	cTrimMap_E[i]->cd(j+1);
	fh1_trimmap_E[i][j]->Draw("");

	sprintf(Name1, "fh1_trim_DTmap_sec%d_a%d", i+1, j+1);
	sprintf(Name2, "Sec %d:Anode %d", i+1, j+1);
	fh1_trimmap_DT[i][j] = new TH1F(Name1, Name2, 40000, 0, 40000);
	fh1_trimmap_DT[i][j]->GetXaxis()->SetTitle("Drift time [channels, 100ps TDC resolution], if mult==1");
	fh1_trimmap_DT[i][j]->GetYaxis()->SetTitle("Counts");
	fh1_trimmap_DT[i][j]->GetYaxis()->SetTitleOffset(1.1);
	fh1_trimmap_DT[i][j]->GetXaxis()->CenterTitle(true);
	fh1_trimmap_DT[i][j]->GetYaxis()->CenterTitle(true);
	fh1_trimmap_DT[i][j]->GetXaxis()->SetLabelSize(0.045);
	fh1_trimmap_DT[i][j]->GetXaxis()->SetTitleSize(0.045);
	fh1_trimmap_DT[i][j]->GetYaxis()->SetLabelSize(0.045);
	fh1_trimmap_DT[i][j]->GetYaxis()->SetTitleSize(0.045);
	fh1_trimmap_DT[i][j]->SetFillColor(31);
	cTrimMap_DT[i]->cd(j+1);
	fh1_trimmap_DT[i][j]->Draw("");
      }

      sprintf(Name1, "Trim_multiplicity_Sec_%d", i + 1);
      sprintf(Name2, "TRIM: Multiplicity per anode in section %d", i + 1);
      cTrimMap_Mult[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);

      sprintf(Name1, "fh1_trim_mult_sec_%d", i+1);
      sprintf(Name2, "Trim: Multiplicity section %d", i + 1);
      fh1_trimmap_Mult[i] = new TH1F(Name1, Name2, NbAnodesTrim+NbTrefTrim+NbTtrigTrim, 0, NbAnodesTrim+NbTrefTrim+NbTtrigTrim);
      fh1_trimmap_Mult[i]->GetXaxis()->SetTitle("Anode");
      fh1_trimmap_Mult[i]->GetYaxis()->SetTitle("Counts");
      fh1_trimmap_Mult[i]->GetYaxis()->SetTitleOffset(1.1);
      fh1_trimmap_Mult[i]->GetXaxis()->CenterTitle(true);
      fh1_trimmap_Mult[i]->GetYaxis()->CenterTitle(true);
      fh1_trimmap_Mult[i]->GetXaxis()->SetLabelSize(0.045);
      fh1_trimmap_Mult[i]->GetXaxis()->SetTitleSize(0.045);
      fh1_trimmap_Mult[i]->GetYaxis()->SetLabelSize(0.045);
      fh1_trimmap_Mult[i]->GetYaxis()->SetTitleSize(0.045);
      fh1_trimmap_Mult[i]->SetFillColor(31);
      fh1_trimmap_Mult[i]->Draw("");
    }// end of for(i, NbSectionsTrim) 1D-MAPPED


    // --- TRIM: 1-D MAPPED data for Delta T = Tref - Ttrig for each section 
    cTrimMap_DeltaTrefTtrig = new TCanvas("Trim_TrefTtrig", "Delta T (Tref-Trig)", 10, 10, 800, 700);
    cTrimMap_DeltaTrefTtrig->Divide(NbSectionsTrim,1);

    for (Int_t i = 0; i < NbSectionsTrim; i++){
      sprintf(Name1, "fh1_trim_Sec%d_DeltaTrefTtrig", i+1);
      sprintf(Name2, "Delta T (Tref-Trig) in section %d [channels, 100ps TDC resolution] 1ns/bin",i+1);
      fh1_trimmap_DeltaTrefTtrig[i] = new TH1F(Name1, Name2, 8000, -40000, 40000);
      cTrimMap_DeltaTrefTtrig->cd(i+1);
      fh1_trimmap_DeltaTrefTtrig[i]->Draw("");
    }

    // --- TRIM: 2-D MAPPED data for ENERGY vs DRIFT-TIME and DRIFT-TIME vs DRIFT-TIME
    for (Int_t i = 0; i < NbSectionsTrim; i++){
      sprintf(Name1, "Trim_EvsDT_Sec_%d", i + 1);
      sprintf(Name2, "Section %d", i + 1);
      cTrimMap_EvsDT[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);

      cTrimMap_EvsDT[i]->Divide(2,3);
      sprintf(Name1, "Trim_DTvsDT_Sec_%d", i + 1);
      sprintf(Name2, "DTvsDT Section %d", i + 1);
      cTrimMap_DTvsDT[i] = new TCanvas(Name1, Name2, 10, 10, 800, 700);
      cTrimMap_DTvsDT[i]->Divide(2,3);

      for (Int_t j = 0; j < NbAnodesTrim; j++){
	sprintf(Name1, "fh1_trim_EvsDT_sec%d_a%d", i+1, j+1);
	sprintf(Name2, "Sec %d:Anode %d", i+1, j+1);
	fh2_trimmap_EvsDT[i][j] = new TH2F(Name1, Name2, 512, 0, 8192, 800, 0, 40000);
	fh2_trimmap_EvsDT[i][j]->GetXaxis()->SetTitle("Energy [channels, 8k ADC resolution], if mult==1");
	fh2_trimmap_EvsDT[i][j]->GetYaxis()->SetTitle("Drift time [channels, 100ps TDC resolution], if mult==1");
	fh2_trimmap_EvsDT[i][j]->GetYaxis()->SetTitleOffset(1.1);
	fh2_trimmap_EvsDT[i][j]->GetXaxis()->CenterTitle(true);
	fh2_trimmap_EvsDT[i][j]->GetYaxis()->CenterTitle(true);
	fh2_trimmap_EvsDT[i][j]->GetXaxis()->SetLabelSize(0.045);
	fh2_trimmap_EvsDT[i][j]->GetXaxis()->SetTitleSize(0.045);
	fh2_trimmap_EvsDT[i][j]->GetYaxis()->SetLabelSize(0.045);
	fh2_trimmap_EvsDT[i][j]->GetYaxis()->SetTitleSize(0.045);
	cTrimMap_EvsDT[i]->cd(j+1);
	fh2_trimmap_EvsDT[i][j]->Draw("col");
      }

      for (Int_t j = 0; j < NbAnodesTrim-1; j++){
	sprintf(Name1, "fh2_trimmap_DTvsDT_sec%d_a%d", i + 1, j);
	sprintf(Name2, "Sec %d: DT_%d vs DT_%d", i + 1, j + 1, j + 2);
	fh2_trimmap_DTvsDT[i][j] = new TH2F(Name1, Name2, 800, 0, 40000, 800, 0, 40000);
	sprintf(Name1, "Drift time %d [channels, 100ps TDC resolution]", j+1);
	sprintf(Name2, "Drift time %d [channels, 100ps TDC resolution]", j+2);
	fh2_trimmap_DTvsDT[i][j]->GetXaxis()->SetTitle(Name1);
	fh2_trimmap_DTvsDT[i][j]->GetYaxis()->SetTitle(Name2);
	fh2_trimmap_DTvsDT[i][j]->GetYaxis()->SetTitleOffset(1.1);
	fh2_trimmap_DTvsDT[i][j]->GetXaxis()->CenterTitle(true);
	fh2_trimmap_DTvsDT[i][j]->GetYaxis()->CenterTitle(true);
	fh2_trimmap_DTvsDT[i][j]->GetXaxis()->SetLabelSize(0.045);
	fh2_trimmap_DTvsDT[i][j]->GetXaxis()->SetTitleSize(0.045);
	fh2_trimmap_DTvsDT[i][j]->GetYaxis()->SetLabelSize(0.045);
	fh2_trimmap_DTvsDT[i][j]->GetYaxis()->SetTitleSize(0.045);
	cTrimMap_DTvsDT[i]->cd(j + 1);
	fh2_trimmap_DTvsDT[i][j]->Draw("col");
      }
    }// end of for (i, NbSectionsTrim) 2-D MAPPED

    // === MAIN FOLDER-Trim
    TFolder* mainfolTrim = new TFolder("TRIM", "TRIM info");
    for (Int_t i = 0; i < NbSectionsTrim; i++) mainfolTrim->Add(cTrimMap_E[i]);
    for (Int_t i = 0; i < NbSectionsTrim; i++) mainfolTrim->Add(cTrimMap_DT[i]);
    for (Int_t i = 0; i < NbSectionsTrim; i++) mainfolTrim->Add(cTrimMap_Mult[i]);
    mainfolTrim->Add(cTrimMap_DeltaTrefTtrig);
    for (Int_t i = 0; i < NbSectionsTrim; i++){
        mainfolTrim->Add(cTrimMap_EvsDT[i]);
        mainfolTrim->Add(cTrimMap_DTvsDT[i]);
    }
    run->AddObject(mainfolTrim);

    // === Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_TRIM_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofTrimOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofTrimOnlineSpectra::Reset_Histo";

    // Mapped data
    for (Int_t i = 0; i < NbSectionsTrim; i++){
      fh1_trimmap_DeltaTrefTtrig[i]->Reset();
      fh1_trimmap_Mult[i]->Reset();
      for (Int_t j = 0; j < NbAnodesTrim; j++){
	fh1_trimmap_E[i][j]->Reset();
	fh1_trimmap_DT[i][j]->Reset();
	fh2_trimmap_EvsDT[i][j]->Reset();
      }
      for (Int_t j = 0; j < NbAnodesTrim-1; j++){
	fh2_trimmap_DTvsDT[i][j]->Reset();
      }
    }
}

void R3BSofTrimOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTrimOnlineSpectra::Exec FairRootManager not found";

    // === MAPPED data : Section ID: GetSecID() returns 1-based section number
    // ===               Anode ID: GetAnodeID returns 1-based anode number
    // ===               For Anode ID [1:6] : Eraw, Traw
    // ===               For Anode ID 7     : 0,    Traw of the ref signal
    // ===               For Anode ID 8     : 0,    Traw of the trig signal
    if (fMappedItemsTrim && fMappedItemsTrim->GetEntriesFast() > 0){

      Double_t Eraw[NbSectionsTrim][NbAnodesTrim];
      Double_t Traw[NbSectionsTrim][NbAnodesTrim + NbTrefTrim + NbTtrigTrim];
      UInt_t mult[NbSectionsTrim][NbAnodesTrim + NbTrefTrim + NbTtrigTrim];
      for (Int_t j = 0; j < NbSectionsTrim; j++)
	for (Int_t i = 0; i < NbAnodesTrim; i++)
	  Eraw[j][i] = 0.; // attention, only first hit
      for (Int_t j = 0; j < NbSectionsTrim; j++)
	for (Int_t i = 0; i < NbAnodesTrim + NbTrefTrim + NbTtrigTrim; i++){
	  Traw[j][i] = -1.; // attention, only first hit
	  mult[j][i] = 0;
	}

      Int_t nHits = fMappedItemsTrim->GetEntriesFast();
      for (Int_t ihit = 0; ihit < nHits; ihit++){
	R3BSofTrimMappedData* hit = (R3BSofTrimMappedData*)fMappedItemsTrim->At(ihit);
	if (!hit) continue;
	fh1_trimmap_Mult[hit->GetSecID()-1]->Fill(hit->GetAnodeID());
	mult[hit->GetSecID()-1][hit->GetAnodeID()-1]++;
	// ATTENTION: take into accound only the first hit
	if (mult[hit->GetSecID()-1][hit->GetAnodeID()-1]==1){
	  if (hit->GetEnergy()<8192 && hit->GetEnergy()>0)  //Eraw=0 for Tref and Ttrig
	    Eraw[hit->GetSecID()-1][hit->GetAnodeID()-1] = hit->GetEnergy();
	  Traw[hit->GetSecID()-1][hit->GetAnodeID()-1] = hit->GetTime(); 
	}
      }

      for (Int_t s=0; s<NbSectionsTrim; s++){
	if (mult[s][NbAnodesTrim]==1 && mult[s][NbAnodesTrim+1]==1) 
	  fh1_trimmap_DeltaTrefTtrig[s]->Fill(Traw[s][NbAnodesTrim]-Traw[s][NbAnodesTrim+1]);
	for(Int_t a=0; a<NbAnodesTrim;a++){
	  if (mult[s][a]==1){
	    fh1_trimmap_E[s][a]->Fill(Eraw[s][a]);
	    if (mult[s][NbAnodesTrim]==1){
	      fh1_trimmap_DT[s][a]->Fill(Traw[s][a]-Traw[s][NbAnodesTrim]);
	      fh2_trimmap_EvsDT[s][a]->Fill(Traw[s][a]-Traw[s][NbAnodesTrim],Eraw[s][a]);
	    }
	  }
	}
	for(Int_t a=0; a<NbAnodesTrim-1;a++)
	  if (mult[s][a]==1 && mult[s][a+1]==1 && mult[s][NbAnodesTrim]==1)
	    fh2_trimmap_DTvsDT[s][a]->Fill(Traw[s][a]-Traw[s][NbAnodesTrim],Traw[s][a+1]-Traw[s][NbAnodesTrim]);
      }// end of for(NbSectionsTrim)

    }// end of if (MappedItemsTrim)

    fNEvents += 1;
}

void R3BSofTrimOnlineSpectra::FinishEvent()
{
    if (fMappedItemsTrim)     fMappedItemsTrim->Clear();
}

void R3BSofTrimOnlineSpectra::FinishTask()
{
  if (fMappedItemsTrim){
    cTrimMap_DeltaTrefTtrig->Write();
    for (Int_t i = 0; i < NbSectionsTrim; i++){
      cTrimMap_E[i]->Write();
      cTrimMap_DT[i]->Write();
      cTrimMap_Mult[i]->Write();
      cTrimMap_EvsDT[i]->Write();
      cTrimMap_DTvsDT[i]->Write();
    }
  }
}

ClassImp(R3BSofTrimOnlineSpectra)
