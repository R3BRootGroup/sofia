// ------------------------------------------------------------
// -----                  R3BSofSciOnlineSpectra           -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofSciOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BMusicCalData.h"
#include "R3BMusicHitData.h"
#include "R3BSofMwpcCalData.h"
#include "R3BSofSciMappedData.h"
#include "R3BSofSciTcalData.h"
#include "THttpServer.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRunOnline.h"
#include "FairRuntimeDb.h"
#include "TCanvas.h"
#include "TFolder.h"
#include "TH1.h"
#include "TH2.h"
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

R3BSofSciOnlineSpectra::R3BSofSciOnlineSpectra()
    : FairTask("SofSciOnlineSpectra", 1)
    , fMappedItemsSci(NULL)
    , fTcalItemsSci(NULL)
    , fMusHitItems(NULL)
    , fMusCalItems(NULL)
    , fCalItemsMwpc0(NULL)
    , fNEvents(0)
{
}

R3BSofSciOnlineSpectra::R3BSofSciOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMappedItemsSci(NULL)
    , fTcalItemsSci(NULL)
    , fMusHitItems(NULL)
    , fMusCalItems(NULL)
    , fCalItemsMwpc0(NULL)
    , fNEvents(0)
{
}

R3BSofSciOnlineSpectra::~R3BSofSciOnlineSpectra()
{
    LOG(INFO) << "R3BSofSciOnlineSpectra::Delete instance";
    if (fMappedItemsSci)
        delete fMappedItemsSci;
    if (fTcalItemsSci)
        delete fTcalItemsSci;
    if (fMusHitItems)
        delete fMusHitItems;
    if (fMusCalItems)
        delete fMusCalItems;
    if (fCalItemsMwpc0)
        delete fCalItemsMwpc0;
}

InitStatus R3BSofSciOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofSciOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofSciOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // --- ------------------------------------ --- //
    // --- get access to mapped data of the SCI --- //
    // --- ------------------------------------ --- //
    fMappedItemsSci = (TClonesArray*)mgr->GetObject("SofSciMappedData");
    if (!fMappedItemsSci)
    {
        return kFATAL;
    }

    // --- ---------------------------------- --- //
    // --- get access to tcal data of the SCI --- //
    // --- ---------------------------------- --- //
    fTcalItemsSci = (TClonesArray*)mgr->GetObject("SofSciTcalData");
    if (!fTcalItemsSci)
    {
        return kFATAL;
    }

    // get access to hit data of the MUSIC
    fMusHitItems = (TClonesArray*)mgr->GetObject("MusicHitData");
    if (!fMusHitItems)
        LOG(WARNING) << "R3BSofSciOnlineSpectra: MusicHitData not found";

    // get access to cal data of the MUSIC
    fMusCalItems = (TClonesArray*)mgr->GetObject("MusicCalData");
    if (!fMusCalItems)
        LOG(WARNING) << "R3BSofSciOnlineSpectra: MusicCalData not found";

    // get access to cal data of the MWPC0
    fCalItemsMwpc0 = (TClonesArray*)mgr->GetObject("Mwpc0CalData");
    if (!fCalItemsMwpc0)
        LOG(WARNING) << "R3BSofSciOnlineSpectra: Mwpc0CalData not found";

    // --- ------------------------------- --- //
    // --- Create histograms for detectors --- //
    // --- ------------------------------- --- //
    char Name1[255];
    char Name2[255];

    for (Int_t i = 0; i < NbDetectors; i++)
    {

        // === FINE TIME AND MULT === //
        sprintf(Name1, "SofSci%i_MultAndFt", i + 1);
        cSciMult[i] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
        cSciMult[i]->Divide(2, 2);
        for (Int_t j = 0; j < NbChannels; j++)
        {
            sprintf(Name1, "SofSci%i_FineTimeCh%i", i + 1, j + 1);
            fh1_finetime[i * NbChannels + j] = new TH1I(Name1, Name1, 1000, 0, 1000);
            cSciMult[i]->cd(j + 1);
            fh1_finetime[i * NbChannels + j]->Draw("");
        }
        sprintf(Name1, "SofSci%i_MultPerChannel", i + 1);
        fh2_mult[i] = new TH2I(Name1, Name1, NbChannels, 0.5, NbChannels + 0.5, 20, -0.5, 19.5);
        fh2_mult[i]->GetXaxis()->SetTitle("channel: 1=PMT R,    2=PMT L,    3=COMMON REF");
        fh2_mult[i]->GetYaxis()->SetTitle("multiplicity per channel");
        cSciMult[i]->cd(4);
        fh2_mult[i]->Draw("COL");

        // === RAW POSITION === //
        sprintf(Name1, "SofSci%i_RawPos", i + 1);
        cSciRawPos[i] = new TCanvas(Name1, Name1, 10, 10, 500, 500);
        sprintf(Name1, "SofSci%i_RawPosAtTcal_Mult1", i + 1);
        fh1_RawPos_AtTcalMult1[i] = new TH1F(Name1, Name1, 10000, -10, 10);
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->SetTitle("Raw position [ns with one bin/ps]");
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->SetTitle("Counts per bin");
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->CenterTitle(true);
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->CenterTitle(true);
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->SetTitleSize(0.045);
        cSciRawPos[i]->cd();
        fh1_RawPos_AtTcalMult1[i]->Draw("");
    }
  Int_t nTof=0;
  for (Int_t dstart = 0; dstart < NbDetectors-1 ; dstart++)
  {
    for(Int_t dstop = dstart+1; dstop < NbDetectors ; dstop++)
    {
      //1D - Raw time-of-flight from tcal data at mult=1
      sprintf(Name1,"RawTof_Sci%02d_to_Sci%02d",dstart+1,dstop+1);
      sprintf(Name2,"Raw time-of-flight from Sci%02d to Sci%02d",dstart+1, dstop+1);
      cSciRawTof[nTof] = new TCanvas(Name1,Name2,10,10,800,800);
      cSciRawTof[nTof]->Divide(1,2);
      sprintf(Name1,"RawTofNs_Sci%02d_to_Sci%02d",dstart+1,dstop+1);
      fh1_RawTof_AtTcalMult1[nTof] = new TH1D(Name1,Name1,200000,-10000,1000);
      fh1_RawTof_AtTcalMult1[nTof]->GetXaxis()->SetTitle("Raw Tof [ns]");
      fh1_RawTof_AtTcalMult1[nTof]->GetYaxis()->SetTitle("Counts per bin"); 
      sprintf(Name1,"RawTofNs_wTref_Sci%02d_to_Sci%02d",dstart+1,dstop+1);
      fh1_RawTof_AtTcalMult1_wTref[nTof] = new TH1D(Name1,Name1,200000,-10000,1000);
      fh1_RawTof_AtTcalMult1_wTref[nTof]->GetXaxis()->SetTitle("Raw Tof [ns]");
      fh1_RawTof_AtTcalMult1_wTref[nTof]->GetYaxis()->SetTitle("Counts per bin"); 
      cSciRawTof[nTof]->cd(1);
      fh1_RawTof_AtTcalMult1[nTof]->Draw("");
      cSciRawTof[nTof]->cd(2);
      fh1_RawTof_AtTcalMult1_wTref[nTof]->Draw("");
     
      // Raw Tof versus position in start and stop
      sprintf(Name1,"RawTof_Sci%02d_Sci%02d_vs_RawPos",dstart+1,dstop+1);
      sprintf(Name2,"Raw time-of-flight from Sci%02d to Sci%02d versus RawPos",dstart+1, dstop+1);
      cSciRawTofvsRawPos[nTof] = new TCanvas(Name1,Name2,10,10,800,800);
      cSciRawTofvsRawPos[nTof]->Divide(1,2);
      sprintf(Name1,"RawTofNs_Sci%02d_Sci%02d_vs_RawPos%02d",dstart+1,dstop+1,dstart+1);
      fh2_RawTof_vs_RawPosStart_AtTcalMult1[nTof] = new TH2D(Name1,Name1,1000,-10,10,2000,-10000,1000);
      fh2_RawTof_vs_RawPosStart_AtTcalMult1[nTof]->GetXaxis()->SetTitle("Raw Pos [ns]");
      fh2_RawTof_vs_RawPosStart_AtTcalMult1[nTof]->GetYaxis()->SetTitle("Raw Tof [ns]"); 
      sprintf(Name1,"RawTofNs_Sci%02d_Sci%02d_vs_RawPos%02d",dstart+1,dstop+1,dstop+1);
      fh2_RawTof_vs_RawPosStop_AtTcalMult1[nTof] = new TH2D(Name1,Name1,1000,-10,10,2000,-10000,1000);
      fh2_RawTof_vs_RawPosStop_AtTcalMult1[nTof]->GetXaxis()->SetTitle("Raw Pos [ns]");
      fh2_RawTof_vs_RawPosStop_AtTcalMult1[nTof]->GetYaxis()->SetTitle("Raw Tof [ns]"); 
      cSciRawTofvsRawPos[nTof]->cd(1);
      fh2_RawTof_vs_RawPosStart_AtTcalMult1[nTof]->Draw("col");
      cSciRawTofvsRawPos[nTof]->cd(2);
      fh2_RawTof_vs_RawPosStop_AtTcalMult1[nTof]->Draw("col");
      nTof++;
    }  
  }

    // Music Hit data vs SCI-RawPos
    TCanvas* cMusicZvsRawPos =
        new TCanvas("Musicchargez_vs_RawPosAtTcal_Mult1", "Music charge Z vs RawPosAtTcal_Mult1", 10, 10, 800, 700);
    fh2_MusZvsRawPos =
        new TH2F("fh2_Musicchargez_vs_RawPos", "Music charge Z vs RawPosAtTcal_Mult1", 10000, -7, 7, 400, 0, 40);
    fh2_MusZvsRawPos->GetXaxis()->SetTitle("Raw position [ns with one bin/ps]");
    fh2_MusZvsRawPos->GetYaxis()->SetTitle("Charge (Z)");
    fh2_MusZvsRawPos->GetYaxis()->SetTitleOffset(1.1);
    fh2_MusZvsRawPos->GetXaxis()->CenterTitle(true);
    fh2_MusZvsRawPos->GetYaxis()->CenterTitle(true);
    fh2_MusZvsRawPos->GetXaxis()->SetLabelSize(0.045);
    fh2_MusZvsRawPos->GetXaxis()->SetTitleSize(0.045);
    fh2_MusZvsRawPos->GetYaxis()->SetLabelSize(0.045);
    fh2_MusZvsRawPos->GetYaxis()->SetTitleSize(0.045);
    fh2_MusZvsRawPos->Draw("col");

    TCanvas* cMusicDTvsRawPos =
        new TCanvas("MusicDT_vs_RawPosAtTcal_Mult1", "Music FT vs RawPosAtTcal_Mult1", 10, 10, 800, 700);
    fh2_MusDTvsRawPos = new TH2F("fh2_MusicDT_vs_RawPos", "Music DT vs RawPosAtTcal_Mult1", 10000, -7, 7, 800, -20, 20);
    fh2_MusDTvsRawPos->GetXaxis()->SetTitle("Sci Raw position [ns with one bin/ps]");
    fh2_MusDTvsRawPos->GetYaxis()->SetTitle("Drift Time (mm)");
    fh2_MusDTvsRawPos->GetYaxis()->SetTitleOffset(1.1);
    fh2_MusDTvsRawPos->GetXaxis()->CenterTitle(true);
    fh2_MusDTvsRawPos->GetYaxis()->CenterTitle(true);
    fh2_MusDTvsRawPos->GetXaxis()->SetLabelSize(0.045);
    fh2_MusDTvsRawPos->GetXaxis()->SetTitleSize(0.045);
    fh2_MusDTvsRawPos->GetYaxis()->SetLabelSize(0.045);
    fh2_MusDTvsRawPos->GetYaxis()->SetTitleSize(0.045);
    fh2_MusDTvsRawPos->Draw("col");

    // Mwpc0 cal data vs SCI-RawPos
    TCanvas* cMwpc0vsRawPos =
        new TCanvas("Mwpc0_vs_RawPosAtTcal_Mult1", "Mwpc0-X vs RawPosAtTcal_Mult1", 10, 10, 800, 700);
    fh2_Mwpc0vsRawPos = new TH2F("fh2_Mwpc_vs_RawPos", "Mwpc0-X vs RawPosAtTcal_Mult1", 10000, -7, 7, 258, 0.5, 64.5);
    fh2_Mwpc0vsRawPos->GetXaxis()->SetTitle("Raw position [ns with one bin/ps]");
    fh2_Mwpc0vsRawPos->GetYaxis()->SetTitle("Mwpc0-X [pads]");
    fh2_Mwpc0vsRawPos->GetYaxis()->SetTitleOffset(1.1);
    fh2_Mwpc0vsRawPos->GetXaxis()->CenterTitle(true);
    fh2_Mwpc0vsRawPos->GetYaxis()->CenterTitle(true);
    fh2_Mwpc0vsRawPos->GetXaxis()->SetLabelSize(0.045);
    fh2_Mwpc0vsRawPos->GetXaxis()->SetTitleSize(0.045);
    fh2_Mwpc0vsRawPos->GetYaxis()->SetLabelSize(0.045);
    fh2_Mwpc0vsRawPos->GetYaxis()->SetTitleSize(0.045);
    fh2_Mwpc0vsRawPos->Draw("col");

    // --- --------------- --- //
    // --- MAIN FOLDER-Sci --- //
    // --- --------------- --- //
    TFolder* mainfolSci = new TFolder("SOFSCI", "SOFSCI info");
    for (Int_t i = 0; i < NbDetectors; i++)
    {
        mainfolSci->Add(cSciMult[i]);
        mainfolSci->Add(cSciRawPos[i]);
    }
    for(Int_t i = 0; i < NbTof ; i++)
    {
	mainfolSci->Add(cSciRawTof[i]);
    }
    mainfolSci->Add(cMusicZvsRawPos);
    mainfolSci->Add(cMusicDTvsRawPos);
    mainfolSci->Add(cMwpc0vsRawPos);
    run->AddObject(mainfolSci);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_SOFSCI_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofSciOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofSciOnlineSpectra::Reset_Histo";
    for (Int_t i = 0; i < NbDetectors; i++)
    {
        // === MULT AND FINE TIME === //
        fh2_mult[i]->Reset();
        for (Int_t j = 0; j < NbChannels; j++)
        {
            fh1_finetime[i * NbChannels + j]->Reset();
        }
        // === RAW POSITION === //
        fh1_RawPos_AtTcalMult1[i]->Reset();
    }
    for(Int_t i = 0; i<NbTof; i++)
    {
      // === RAW TIME_OF_FLIGHT === //
      fh1_RawTof_AtTcalMult1[i]->Reset();
      fh1_RawTof_AtTcalMult1_wTref[i]->Reset();
      fh2_RawTof_vs_RawPosStart_AtTcalMult1[i]->Reset();
      fh2_RawTof_vs_RawPosStop_AtTcalMult1[i]->Reset();
    }
    fh2_MusZvsRawPos->Reset();
    fh2_MusDTvsRawPos->Reset();
    fh2_Mwpc0vsRawPos->Reset();
}

void R3BSofSciOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofSciOnlineSpectra::Exec FairRootManager not found";

    Int_t nHits;
    UShort_t iDet; // 0-bsed
    UShort_t iCh;  // 0-based
    Double_t iRawTimeNs[NbDetectors * NbChannels];
    UShort_t mult[NbDetectors * NbChannels];

    // --- -------------- --- //
    // --- initialisation --- //
    // --- -------------- --- //
    for (UShort_t i = 0; i < NbDetectors; i++)
    {
        for (UShort_t j = 0; j < NbChannels; j++)
        {
            mult[i * NbChannels + j] = 0;
        }
    }

    // MUSIC Hit data
    Double_t MusicZ = 0.;
    Double_t MusicDT = -1000000.;
    if (fMusHitItems && fMusHitItems->GetEntriesFast() > 0)
    {
        nHits = fMusHitItems->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BMusicHitData* hit = (R3BMusicHitData*)fMusHitItems->At(ihit);
            if (!hit)
                continue;
            MusicZ = hit->GetZcharge();
        }
    }
    // MUSIC Cal data
    if (fMusCalItems && fMusCalItems->GetEntriesFast() > 0)
    {
        nHits = fMusCalItems->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BMusicCalData* hit = (R3BMusicCalData*)fMusCalItems->At(ihit);
            if (!hit)
                continue;
            if (hit->GetAnodeID() == 5)
                MusicDT = hit->GetDTime();
        }
    }

    if (fMappedItemsSci && fMappedItemsSci->GetEntriesFast() && fTcalItemsSci && fTcalItemsSci->GetEntriesFast())
    {

        // --- --------------------- --- //
        // --- loop over mapped data --- //
        // --- --------------------- --- //
        nHits = fMappedItemsSci->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofSciMappedData* hitmapped = (R3BSofSciMappedData*)fMappedItemsSci->At(ihit);
            if (!hitmapped)
                continue;
            iDet = hitmapped->GetDetector() - 1;
            iCh = hitmapped->GetPmt() - 1;
            mult[iDet * NbChannels + iCh]++;
            fh1_finetime[iDet * NbChannels + iCh]->Fill(hitmapped->GetTimeFine());
        }

        // --- ------------------- --- //
        // --- loop over tcal data --- //
        // --- ------------------- --- //
        nHits = fTcalItemsSci->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofSciTcalData* hittcal = (R3BSofSciTcalData*)fTcalItemsSci->At(ihit);
            if (!hittcal)
                continue;
            iDet = hittcal->GetDetector() - 1;
            iCh = hittcal->GetPmt() - 1;
            iRawTimeNs[iDet * NbChannels + iCh] = hittcal->GetRawTimeNs();
            ;
        }

        // Get cal data MWPC0
        Double_t mwpc0x = -1., qmax = -100.;
        if (fCalItemsMwpc0 && fCalItemsMwpc0->GetEntriesFast() > 0)
        {
            nHits = fCalItemsMwpc0->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BSofMwpcCalData* hit = (R3BSofMwpcCalData*)fCalItemsMwpc0->At(ihit);
                if (!hit)
                    continue;
                if (hit->GetQ() > qmax && hit->GetPlane() == 1)
                {
                    mwpc0x = hit->GetPad();
                    qmax = hit->GetQ();
                }
            }
        }

        // --- ----------------------------------------- --- //
        // --- filling some histogramms outside the loop --- //
        // --- ----------------------------------------- --- //
        Double_t possci = 0.;
        for (UShort_t i = 0; i < NbDetectors; i++)
        {
            for (UShort_t j = 0; j < NbChannels; j++)
            {
                fh2_mult[i]->Fill(j + 1, mult[i * NbChannels + j]);
            }
            if ((mult[i * NbChannels] == 1) && (mult[i * NbChannels + 1] == 1))
            {
                // x position increases from left to right : TrawRIGHT - TrawLEFT
                possci = iRawTimeNs[i * NbChannels] - iRawTimeNs[i * NbChannels + 1];
                fh1_RawPos_AtTcalMult1[i]->Fill(possci);
                if (MusicZ > 0.)
                    fh2_MusZvsRawPos->Fill(possci, MusicZ);

                if (MusicDT != -1000000.)
                    fh2_MusDTvsRawPos->Fill(possci, MusicZ);

                if (mwpc0x > 0 && possci > -10. && possci < 10.)
                {
                    fh2_Mwpc0vsRawPos->Fill(possci, mwpc0x + gRandom->Uniform(-0.5, 0.5));
                }
            }
        }
	Int_t nTof=0;
	Double_t iRawTof;
	for(UShort_t dstart=0; dstart<NbDetectors-1; dstart++)
	{
	  for(UShort_t dstop=dstart+1; dstop<NbDetectors; dstop++)
	  {
	    if( (mult[dstart*NbChannels] == 1) && (mult[dstart*NbChannels+1] == 1) &&
		(mult[dstop*NbChannels] == 1) && (mult[dstop*NbChannels+1] == 1) )
	    {
	      iRawTof = 0.5*(iRawTimeNs[dstop+NbChannels]+iRawTimeNs[dstop*NbChannels+1]) -
			0.5*(iRawTimeNs[dstart*NbChannels]+iRawTimeNs[dstart*NbChannels+1]) + 
		  iRawTimeNs[dstart*NbChannels+2] - iRawTimeNs[dstop*NbChannels+2];
	      fh1_RawTof_AtTcalMult1[nTof]->Fill(iRawTof); 
	      fh2_RawTof_vs_RawPosStart_AtTcalMult1[nTof]->Fill(iRawTimeNs[dstart*NbChannels]-iRawTimeNs[dstart*NbChannels+1],iRawTof);
	      fh2_RawTof_vs_RawPosStop_AtTcalMult1[nTof]->Fill(iRawTimeNs[dstop*NbChannels]-iRawTimeNs[dstop*NbChannels+1],iRawTof);
	      if( (mult[dstart*NbChannels+2]==1) && (mult[dstop*NbChannels+2]==1))
	      {
		fh1_RawTof_AtTcalMult1_wTref[nTof]->Fill( 
		  0.5*(iRawTimeNs[dstop+NbChannels]+iRawTimeNs[dstop*NbChannels+1]) - 
		  0.5*(iRawTimeNs[dstart*NbChannels]+iRawTimeNs[dstart*NbChannels+1]) + 
		  iRawTimeNs[dstart*NbChannels+2] - iRawTimeNs[dstop*NbChannels+2]
		  ); 
	      }
	      nTof++;
	    }
	  }
	}
    }
    fNEvents += 1;
}

void R3BSofSciOnlineSpectra::FinishEvent()
{
    if (fMappedItemsSci)
    {
        fMappedItemsSci->Clear();
    }
    if (fTcalItemsSci)
    {
        fTcalItemsSci->Clear();
    }
    if (fMusHitItems)
    {
        fMusHitItems->Clear();
    }
    if (fMusCalItems)
    {
        fMusCalItems->Clear();
    }
    if (fCalItemsMwpc0)
    {
        fCalItemsMwpc0->Clear();
    }
}

void R3BSofSciOnlineSpectra::FinishTask()
{

    if (fMappedItemsSci)
    {
        for (UShort_t i = 0; i < NbDetectors; i++)
        {
            fh2_mult[i]->Write();
            cSciMult[i]->Write();
            for (UShort_t j = 0; j < NbChannels; j++)
            {
                fh1_finetime[i * NbChannels + j]->Write();
            }
        }
    }

    if (fTcalItemsSci)
    {
        for (UShort_t i = 0; i < NbDetectors; i++)
        {
            fh1_RawPos_AtTcalMult1[i]->Write();
            cSciRawPos[i]->Write();
        }
	for (UShort_t i = 0; i<NbTof ; i++)
	{
	    fh1_RawTof_AtTcalMult1[i]->Write();
	    fh1_RawTof_AtTcalMult1_wTref[i]->Write();
	    cSciRawTof[i]->Write();
	    fh2_RawTof_vs_RawPosStart_AtTcalMult1[i]->Write();
	    fh2_RawTof_vs_RawPosStop_AtTcalMult1[i]->Write();
	    cSciRawTofvsRawPos[i];
	}
        if (fMusHitItems)
        {
            fh2_MusZvsRawPos->Write();
        }
        if (fMusCalItems)
            fh2_MusDTvsRawPos->Write();
        if (fCalItemsMwpc0)
            fh2_Mwpc0vsRawPos->Write();
    }
}

ClassImp(R3BSofSciOnlineSpectra)
