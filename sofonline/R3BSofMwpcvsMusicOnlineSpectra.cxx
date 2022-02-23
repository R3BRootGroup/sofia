// ------------------------------------------------------------
// -----        R3BSofMwpcvsMusicOnlineSpectra            -----
// -----    Created 16/02/20  by J.L. Rodriguez-Sanchez   -----
// -----       Fill mwpc vs music online histograms       -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with mwpc and music online data
 */

#include "R3BSofMwpcvsMusicOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BMusicHitData.h"
#include "R3BMusicMappedData.h"
#include "R3BMwpcCalData.h"
#include "R3BMwpcHitData.h"
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

#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom.h"
#include <array>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>

using namespace std;

R3BSofMwpcvsMusicOnlineSpectra::R3BSofMwpcvsMusicOnlineSpectra()
    : FairTask("SofMwpcvsMusicOnlineSpectra", 1)
    , fCalItemsMwpc(NULL)
    , fHitItemsMwpc(NULL)
    , fMappedItemsMus(NULL)
    , fHitItemsMus(NULL)
    , fNameDet1("Mwpc0")
    , fNEvents(0)
{
}

R3BSofMwpcvsMusicOnlineSpectra::R3BSofMwpcvsMusicOnlineSpectra(const TString& name,
                                                               Int_t iVerbose,
                                                               const TString& namedet1)
    : FairTask(name, iVerbose)
    , fCalItemsMwpc(NULL)
    , fHitItemsMwpc(NULL)
    , fMappedItemsMus(NULL)
    , fHitItemsMus(NULL)
    , fNameDet1(namedet1)
    , fNEvents(0)
{
}

R3BSofMwpcvsMusicOnlineSpectra::~R3BSofMwpcvsMusicOnlineSpectra()
{
    LOG(INFO) << "R3BSofMWPC" + fNameDet1 + "vsMusicOnlineSpectra::Delete instance";
    if (fCalItemsMwpc)
        delete fCalItemsMwpc;
    if (fHitItemsMwpc)
        delete fHitItemsMwpc;
    if (fMappedItemsMus)
        delete fMappedItemsMus;
    if (fHitItemsMus)
        delete fHitItemsMus;
}

InitStatus R3BSofMwpcvsMusicOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSof" + fNameDet1 + "vsMusicOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSof" + fNameDet1 + "vsMusicOnlineSpectra::FairRootManager not found";

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // get access to mapped data of the mwpc detector
    fCalItemsMwpc = (TClonesArray*)mgr->GetObject(fNameDet1 + "CalData");
    if (!fCalItemsMwpc)
    {
        return kFATAL;
    }

    // get access to hit data of mwpcs
    fHitItemsMwpc = (TClonesArray*)mgr->GetObject(fNameDet1 + "HitData");
    if (!fHitItemsMwpc)
        LOG(WARNING) << "R3BSof" + fNameDet1 + "vsMusicOnlineSpectra: " + fNameDet1 + "HitData not found";

    // get access to map data of the MUSIC
    fMappedItemsMus = (TClonesArray*)mgr->GetObject("MusicMappedData");
    if (!fMappedItemsMus)
        LOG(WARNING) << "R3BSof" + fNameDet1 + "vsMusicOnlineSpectra: MusicMappedData not found";

    // get access to hit data of the MUSIC
    fHitItemsMus = (TClonesArray*)mgr->GetObject("MusicHitData");
    if (!fHitItemsMus)
        LOG(WARNING) << "R3BSof" + fNameDet1 + "vsMusicOnlineSpectra: MusicHitData not found";

    // Create histograms for detectors
    char Name1[255];
    char Name2[255];

    cMusDTCorMwpc0 =
        new TCanvas("MusDT_" + fNameDet1, "Music: Correlation of DT with " + fNameDet1 + " (X,Y)", 10, 10, 800, 700);
    cMusDTCorMwpc0->Divide(2, 4);

    for (Int_t j = 0; j < NbAnodesMus; j++)
    {
        sprintf(Name1, "fh2_MusDT%d_vs_X0", j + 1);
        sprintf(Name2, "Music: DRIFT TIME ANODE %d vs ", j + 1);
        fh2_MusCorMwpc0_DTvsX0[j] = new TH2F(Name1, Name2 + fNameDet1 + "-X", 500, -20, 20, 1500, 0, 30000);
        fh2_MusCorMwpc0_DTvsX0[j]->GetXaxis()->SetTitle(fNameDet1 + "-X [mm]");
        fh2_MusCorMwpc0_DTvsX0[j]->GetYaxis()->SetTitle("Drift time [channels]");
        fh2_MusCorMwpc0_DTvsX0[j]->GetYaxis()->SetTitleOffset(1.1);
        fh2_MusCorMwpc0_DTvsX0[j]->GetXaxis()->CenterTitle(true);
        fh2_MusCorMwpc0_DTvsX0[j]->GetYaxis()->CenterTitle(true);
        fh2_MusCorMwpc0_DTvsX0[j]->GetXaxis()->SetLabelSize(0.045);
        fh2_MusCorMwpc0_DTvsX0[j]->GetXaxis()->SetTitleSize(0.045);
        fh2_MusCorMwpc0_DTvsX0[j]->GetYaxis()->SetLabelSize(0.045);
        fh2_MusCorMwpc0_DTvsX0[j]->GetYaxis()->SetTitleSize(0.045);
        cMusDTCorMwpc0->cd(j + 1);
        fh2_MusCorMwpc0_DTvsX0[j]->Draw("col");
    }

    cMusECorMwpc0 = new TCanvas("MusE_Mwpc0", "Music: Correlation with " + fNameDet1, 10, 10, 800, 700);
    cMusECorMwpc0->Divide(1, 2);
    fh2_MusCorMwpc0_EsumVsX0mm =
        new TH2F("fh2_Mus_Evs" + fNameDet1 + "X", "Music: Esum vs X in mm", 1000, -100, 100, 6400, 0, 64000);
    fh2_MusCorMwpc0_EsumVsX0mm->GetXaxis()->SetTitle("X [mm]");
    fh2_MusCorMwpc0_EsumVsX0mm->GetYaxis()->SetTitle("Raw Esum [channel]");
    fh2_MusCorMwpc0_EsumVsX0mm->GetXaxis()->CenterTitle(true);
    fh2_MusCorMwpc0_EsumVsX0mm->GetYaxis()->CenterTitle(true);
    fh2_MusCorMwpc0_EsumVsX0mm->GetXaxis()->SetLabelSize(0.045);
    fh2_MusCorMwpc0_EsumVsX0mm->GetXaxis()->SetTitleSize(0.045);
    fh2_MusCorMwpc0_EsumVsX0mm->GetYaxis()->SetLabelSize(0.045);
    fh2_MusCorMwpc0_EsumVsX0mm->GetYaxis()->SetTitleSize(0.045);
    cMusECorMwpc0->cd(1);
    fh2_MusCorMwpc0_EsumVsX0mm->Draw("col");
    fh2_MusCorMwpc0_EsumVsY0mm =
        new TH2F("fh2_Mus_Evs" + fNameDet1 + "Y", "Music: Esum vs Y in mm", 500, -50, 50, 6400, 0, 64000);
    fh2_MusCorMwpc0_EsumVsY0mm->GetXaxis()->SetTitle("Y [mm]");
    fh2_MusCorMwpc0_EsumVsY0mm->GetYaxis()->SetTitle("Raw Esum [channel]");
    fh2_MusCorMwpc0_EsumVsY0mm->GetXaxis()->CenterTitle(true);
    fh2_MusCorMwpc0_EsumVsY0mm->GetYaxis()->CenterTitle(true);
    fh2_MusCorMwpc0_EsumVsY0mm->GetXaxis()->SetLabelSize(0.045);
    fh2_MusCorMwpc0_EsumVsY0mm->GetXaxis()->SetTitleSize(0.045);
    fh2_MusCorMwpc0_EsumVsY0mm->GetYaxis()->SetLabelSize(0.045);
    fh2_MusCorMwpc0_EsumVsY0mm->GetYaxis()->SetTitleSize(0.045);
    cMusECorMwpc0->cd(2);
    fh2_MusCorMwpc0_EsumVsY0mm->Draw("col");

    // MAIN FOLDER
    TFolder* mainfol = new TFolder(fNameDet1 + "-Music", fNameDet1 + "-Music info");
    mainfol->Add(cMusECorMwpc0);
    mainfol->Add(cMusDTCorMwpc0);
    run->AddObject(mainfol);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_" + fNameDet1 + "-Music_HIST",
                                          Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofMwpcvsMusicOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSof" + fNameDet1 + "vsMusicOnlineSpectra::Reset_Histo";
    // Cal data
    for (Int_t j = 0; j < NbAnodesMus; j++)
    {
        fh2_MusCorMwpc0_DTvsX0[j]->Reset();
    }
    fh2_MusCorMwpc0_EsumVsX0mm->Reset();
    fh2_MusCorMwpc0_EsumVsY0mm->Reset();
    // Hit data
}

void R3BSofMwpcvsMusicOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSof" + fNameDet1 + "vsMusicOnlineSpectra::Exec FairRootManager not found";

    // Fill music mapped data
    if (fMappedItemsMus && fMappedItemsMus->GetEntriesFast() > 0)
    {
        Double_t e1 = 0., e2 = 0.;
        Double_t n1 = 0., n2 = 0.;
        for (Int_t i = 0; i < NbAnodesMus; i++)
            fE[i] = 0.; // mult=1 !!!
        for (Int_t i = 0; i < NbAnodesMus + 2; i++)
        {
            fT[i] = 0.; // mult=1 !!!
            multhit[i] = 0;
        }
        Int_t nHits = fMappedItemsMus->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BMusicMappedData* hit = (R3BMusicMappedData*)fMappedItemsMus->At(ihit);
            if (!hit)
                continue;
            multhit[hit->GetAnodeID()]++;
            if (fT[hit->GetAnodeID()] == 0)
                fT[hit->GetAnodeID()] = hit->GetTime(); // mult=1 !!!

            if (hit->GetAnodeID() < NbAnodesMus / 2)
            {
                e1 = e1 + hit->GetEnergy();
                n1++;
            }
            else if (hit->GetAnodeID() < NbAnodesMus)
            {
                e2 = e2 + hit->GetEnergy();
                n2++;
            }
        }

        // Fill data only if there are TREF signals
        if (multhit[NbAnodesMus] == 1)
        {

            if (fHitItemsMwpc && fHitItemsMwpc->GetEntriesFast() > 0)
            {
                for (Int_t ihit = 0; ihit < fHitItemsMwpc->GetEntriesFast(); ihit++)
                {
                    R3BMwpcHitData* hit = (R3BMwpcHitData*)fHitItemsMwpc->At(ihit);
                    if (!hit)
                        continue;
                    fh2_MusCorMwpc0_EsumVsX0mm->Fill(hit->GetX(), (e1 + e2) / (n1 + n2));
                    fh2_MusCorMwpc0_EsumVsY0mm->Fill(hit->GetY(), (e1 + e2) / (n1 + n2));
                    for (Int_t i = 0; i < NbAnodesMus; i++)
                    {
                        if (multhit[NbAnodesMus] == 1 && multhit[i] == 1)
                        {
                            fh2_MusCorMwpc0_DTvsX0[i]->Fill(hit->GetX(), fT[i] - fT[NbAnodesMus]);
                        }
                    }
                }
            }
        }
    }

    fNEvents += 1;
}

void R3BSofMwpcvsMusicOnlineSpectra::FinishEvent()
{
    if (fCalItemsMwpc)
    {
        fCalItemsMwpc->Clear();
    }
    if (fHitItemsMwpc)
    {
        fHitItemsMwpc->Clear();
    }
    if (fMappedItemsMus)
    {
        fMappedItemsMus->Clear();
    }
    if (fHitItemsMus)
    {
        fHitItemsMus->Clear();
    }
}

void R3BSofMwpcvsMusicOnlineSpectra::FinishTask()
{

    if (fCalItemsMwpc && fMappedItemsMus)
    {
    }
    if (fHitItemsMwpc && fHitItemsMus)
    {
    }
}

ClassImp(R3BSofMwpcvsMusicOnlineSpectra)
