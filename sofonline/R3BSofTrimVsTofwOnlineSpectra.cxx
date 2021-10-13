/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofTrimVsTofwOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BSofTofWHitData.h"
#include "R3BSofTrimHitData.h"
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

R3BSofTrimVsTofwOnlineSpectra::R3BSofTrimVsTofwOnlineSpectra()
    : FairTask("SofTrimVsTofwOnlineSpectra", 1)
    , fTrimHit(NULL)
    , fTofwHit(NULL)
    , fNEvents(0)
    , fNumSections(3)
{
}

R3BSofTrimVsTofwOnlineSpectra::R3BSofTrimVsTofwOnlineSpectra(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fTrimHit(NULL)
    , fTofwHit(NULL)
    , fNEvents(0)
    , fNumSections(3)
{
}

R3BSofTrimVsTofwOnlineSpectra::~R3BSofTrimVsTofwOnlineSpectra()
{
    LOG(INFO) << "R3BSofTrimVsTofwOnlineSpectra::Delete instance";
    if (fTrimHit)
        delete fTrimHit;
    if (fTofwHit)
        delete fTofwHit;
}

InitStatus R3BSofTrimVsTofwOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofTrimVsTofwOnlineSpectra::Init() fNumSections = " << fNumSections;

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTrimVsTofwOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // === ===================== === //
    // === Triple-MUSIC HIT DATA === //
    // === ===================== === //
    fTrimHit = (TClonesArray*)mgr->GetObject("TrimHitData");
    if (!fTrimHit)
    {
        LOG(WARNING) << " R3BSofTrimVsTofwOnlineSpectra::Init(), TrimHitData not found ... is ok !";
    }

    // === ================= === //
    // === Tof-WALL HIT DATA === //
    // === ================= === //
    fTofwHit = (TClonesArray*)mgr->GetObject("TofWHitData");
    if (!fTofwHit)
    {
        LOG(WARNING) << " R3BSofTrimVsTofwOnlineSpectra::Init(), TofwHitData not found ... is ok !";
    }

    // === =============================== === //
    // === Create histograms for detectors === //
    // === =============================== === //
    char Name1[255];
    char Name2[255];
    if (fTrimHit && fTofwHit)
    {
        for (int section = 0; section < 3; section++)
        {
            sprintf(Name1, "ES%02d_vs_Tof", section + 1);
            c_TrimE_vs_TofCaveC[section] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
            c_TrimE_vs_TofCaveC[section]->cd();
            sprintf(Name1, "TrimE_S%02d_vs_Tof", section + 1);
            fh2_TrimE_vs_TofCaveC[section] = new TH2D(Name1, Name1, 1000, -5000, 5000, 1000, 10000, 60000);
            fh2_TrimE_vs_TofCaveC[section]->Draw("col");
        }
    }

    // === ================ === //
    // === MAIN FOLDER-Trim === //
    // === ================ === //

    TFolder* mainfol = new TFolder("TrimVsTofW", "Trim vs TofW info");
    if (fTrimHit && fTofwHit)
    {
        for (int section = 0; section < 3; section++)
        {
            mainfol->Add(c_TrimE_vs_TofCaveC[section]);
        }
    }
    run->AddObject(mainfol);

    // === Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_TRIMvsTOFW_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofTrimVsTofwOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofTrimVsTofwOnlineSpectra::Reset_Histo";

    // Hit data
    if (fTrimHit && fTofwHit)
    {
        for (int section = 0; section < 3; section++)
        {
            fh2_TrimE_vs_TofCaveC[section]->Reset();
        }
    }
}

void R3BSofTrimVsTofwOnlineSpectra::Exec(Option_t* option)
{

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTrimVsTofwOnlineSpectra::Exec FairRootManager not found";

    // === Tof-wall Hit data
    Int_t nHitsTofW = 0;
    Double_t Tof = 0;
    if (fTofwHit && fTofwHit->GetEntriesFast() > 0)
    {
        nHitsTofW = fTrimHit->GetEntriesFast();
        if (nHitsTofW == 1)
        {
            R3BSofTofWHitData* hit = (R3BSofTofWHitData*)fTofwHit->At(0);
            if (hit)
                Tof = hit->GetTof();
        }
    }

    // === Triple-MUSIC Hit data
    if (fTrimHit && fTrimHit->GetEntriesFast() > 0 && nHitsTofW == 1)
    {
        Int_t nHits = fTrimHit->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHitsTofW; ihit++)
        {
            R3BSofTrimHitData* hit = (R3BSofTrimHitData*)fTrimHit->At(ihit);
            if (!hit)
                continue;
            fh2_TrimE_vs_TofCaveC[hit->GetSecID() - 1]->Fill(Tof, hit->GetEnergyTheta());
        } // end of loop over the HitData TClonesArray
    }

    fNEvents += 1;
}

void R3BSofTrimVsTofwOnlineSpectra::FinishEvent()
{
    if (fTrimHit)
        fTrimHit->Clear();
    if (fTofwHit)
        fTofwHit->Clear();
}

void R3BSofTrimVsTofwOnlineSpectra::FinishTask()
{
    if (fTrimHit && fTofwHit)
    {
        for (int section = 0; section < 3; section++)
        {
            c_TrimE_vs_TofCaveC[section]->Write();
        }
    }
}

ClassImp(R3BSofTrimVsTofwOnlineSpectra)
