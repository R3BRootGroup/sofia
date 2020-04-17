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
#include "R3BSofSciCalData.h"
#include "R3BSofSciMappedData.h"
#include "R3BSofSciSingleTcalData.h"
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
    , fSingleTcalItemsSci(NULL)
    , fMusHitItems(NULL)
    , fMusCalItems(NULL)
    , fCalItemsMwpc0(NULL)
    , fTofwHitData(NULL)
    , fNEvents(0)
{
}

R3BSofSciOnlineSpectra::R3BSofSciOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMappedItemsSci(NULL)
    , fTcalItemsSci(NULL)
    , fSingleTcalItemsSci(NULL)
    , fMusHitItems(NULL)
    , fMusCalItems(NULL)
    , fCalItemsMwpc0(NULL)
    , fTofwHitData(NULL)
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
    if (fSingleTcalItemsSci)
        delete fSingleTcalItemsSci;
    if (fMusHitItems)
        delete fMusHitItems;
    if (fMusCalItems)
        delete fMusCalItems;
    if (fCalItemsMwpc0)
        delete fCalItemsMwpc0;
    if (fTofwHitData)
        delete fTofwHitData;
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

    // --- ----------------------------------------- --- //
    // --- get access to single tcal data of the SCI --- //
    // --- ----------------------------------------- --- //
    fSingleTcalItemsSci = (TClonesArray*)mgr->GetObject("SofSciSingleTcalData");
    if (!fSingleTcalItemsSci)
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

        // === RAW POSITION AT TCAL LEVEL, MULT==1 === //
        sprintf(Name1, "SofSci%i_RawPos", i + 1);
        cSciRawPos[i] = new TCanvas(Name1, Name1, 10, 10, 500, 500);
        cSciRawPos[i]->Divide(1, 2);
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
        cSciRawPos[i]->cd(1);
        fh1_RawPos_AtTcalMult1[i]->Draw("");

        // === RAW POSITION AT SINGLE TCAL LEVEL === //
        sprintf(Name1, "SofSci%i_RawPosAtiSingleTcal", i + 1);
        fh1_RawPos_AtSingleTcal[i] = new TH1F(Name1, Name1, 10000, -10, 10);
        fh1_RawPos_AtSingleTcal[i]->GetXaxis()->SetTitle("Raw position [ns with one bin/ps]");
        fh1_RawPos_AtSingleTcal[i]->GetYaxis()->SetTitle("Counts per bin");
        fh1_RawPos_AtSingleTcal[i]->GetXaxis()->CenterTitle(true);
        fh1_RawPos_AtSingleTcal[i]->GetYaxis()->CenterTitle(true);
        fh1_RawPos_AtSingleTcal[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_RawPos_AtSingleTcal[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_RawPos_AtSingleTcal[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_RawPos_AtSingleTcal[i]->GetYaxis()->SetTitleSize(0.045);
        cSciRawPos[i]->cd(2);
        fh1_RawPos_AtSingleTcal[i]->Draw("");

        // === R3B MUSIC CHARGE VERSUS RAW POSITION === //
        sprintf(Name1, "MUSIC_Z_vs_RawPos_Sci%02d", i + 1);
        cMusicZvsRawPos[i] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
        sprintf(Name1, "MusZ_vs_RawPos_Sci%02d", i + 1);
        fh2_MusZvsRawPos[i] = new TH2F(Name1, Name1, 2000, -10, 10, 1200, 6, 40);
        fh2_MusZvsRawPos[i]->GetXaxis()->SetTitle("Raw position [ns with one bin/ps]");
        fh2_MusZvsRawPos[i]->GetYaxis()->SetTitle("Charge (Z)");
        fh2_MusZvsRawPos[i]->GetYaxis()->SetTitleOffset(1.1);
        fh2_MusZvsRawPos[i]->GetXaxis()->CenterTitle(true);
        fh2_MusZvsRawPos[i]->GetYaxis()->CenterTitle(true);
        fh2_MusZvsRawPos[i]->GetXaxis()->SetLabelSize(0.045);
        fh2_MusZvsRawPos[i]->GetXaxis()->SetTitleSize(0.045);
        fh2_MusZvsRawPos[i]->GetYaxis()->SetLabelSize(0.045);
        fh2_MusZvsRawPos[i]->GetYaxis()->SetTitleSize(0.045);
        fh2_MusZvsRawPos[i]->Draw("colz");
    }

    // === Mwpc0 cal data vs SCI-RawPos === //
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

    // === Music Hit data vs SCI-RawPos === //
    TCanvas* cMusicDTvsRawPos =
        new TCanvas("MusicDT_vs_RawPosAtTcal_Mult1", "Music FT vs RawPosAtTcal_Mult1", 10, 10, 800, 700);
    fh2_MusDTvsRawPos = new TH2F("fh2_MusicDT_vs_RawPos", "Music DT vs RawPosAtTcal_Mult1", 1400, -7, 7, 800, -20, 20);
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

#ifdef NUMBER_OF_SOFSCI_TOF
    Int_t nTof = 0;
    Int_t TofMin[NbTof] = { 800, 800, 800, 1045, 1900, 855 };
    Int_t TofMax[NbTof] = { 900, 900, 900, 1090, 1950, 875 };
    for (Int_t dstart = 0; dstart < NUMBER_OF_SOFSCI_DETECTORS - 1; dstart++)
    {
        for (Int_t dstop = dstart + 1; dstop < NUMBER_OF_SOFSCI_DETECTORS; dstop++)
        {
            // === RAW TOF AT TCAL AND SINGLE TCAL LEVELS === //
            sprintf(Name1, "RawTof_Sci%02d_to_Sci%02d", dstart + 1, dstop + 1);
            sprintf(Name2, "Raw time-of-flight from Sci%02d to Sci%02d", dstart + 1, dstop + 1);
            cSciRawTof[nTof] = new TCanvas(Name1, Name2, 10, 10, 800, 800);
            cSciRawTof[nTof]->Divide(1, 3);
            sprintf(Name1, "RawTofNs_m1_Sci%02d_to_Sci%02d", dstart + 1, dstop + 1);
            fh1_RawTof_AtTcalMult1[nTof] = new TH1D(Name1, Name1, 100000, -50000, 50000);
            fh1_RawTof_AtTcalMult1[nTof]->GetXaxis()->SetTitle("Raw Tof [ns]");
            fh1_RawTof_AtTcalMult1[nTof]->GetYaxis()->SetTitle("Counts per bin");
            sprintf(Name1, "RawTofNs_m1_wTref_Sci%02d_to_Sci%02d", dstart + 1, dstop + 1);
            fh1_RawTof_AtTcalMult1_wTref[nTof] = new TH1D(Name1, Name1, 400000, -2000, 2000);
            fh1_RawTof_AtTcalMult1_wTref[nTof]->GetXaxis()->SetTitle("Raw Tof [ns]");
            fh1_RawTof_AtTcalMult1_wTref[nTof]->GetYaxis()->SetTitle("Counts per bin");
            sprintf(Name1, "RawTofNs_wTref_Sci%02d_to_Sci%02d", dstart + 1, dstop + 1);
            fh1_RawTof_AtSingleTcal_wTref[nTof] = new TH1D(Name1, Name1, 400000, -2000, 2000);
            fh1_RawTof_AtSingleTcal_wTref[nTof]->GetXaxis()->SetTitle("Raw Tof [ns]");
            fh1_RawTof_AtSingleTcal_wTref[nTof]->GetYaxis()->SetTitle("Counts per bin");
            cSciRawTof[nTof]->cd(1);
            fh1_RawTof_AtTcalMult1[nTof]->Draw("");
            cSciRawTof[nTof]->cd(2);
            fh1_RawTof_AtTcalMult1_wTref[nTof]->Draw("");
            cSciRawTof[nTof]->cd(3);
            fh1_RawTof_AtSingleTcal_wTref[nTof]->Draw("");

            // === MUSIC HIT DATA VERSUS SCI-RAW TOF
            sprintf(Name1, "MUSIC_Z_vs_RawTof_Sci%02d_to_Sci%02d", dstart + 1, dstop + 1);
            cMusicZvsRawTof[nTof] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
            sprintf(Name1, "MusZ_vs_RawTof_Sci%02d_to_Sci%02d", dstart + 1, dstop + 1);
            fh2_MusZvsRawTof[nTof] =
                new TH2F(Name1, Name1, 10 * (TofMax[nTof] - TofMin[nTof]), TofMin[nTof], TofMax[nTof], 1200, 6, 40);
            fh2_MusZvsRawTof[nTof]->GetXaxis()->SetTitle("Raw-ToF-Cave-C [ns]");
            fh2_MusZvsRawTof[nTof]->GetYaxis()->SetTitle("Charge (Z)");
            fh2_MusZvsRawTof[nTof]->GetYaxis()->SetTitleOffset(1.1);
            fh2_MusZvsRawTof[nTof]->GetXaxis()->CenterTitle(true);
            fh2_MusZvsRawTof[nTof]->GetYaxis()->CenterTitle(true);
            fh2_MusZvsRawTof[nTof]->GetXaxis()->SetLabelSize(0.045);
            fh2_MusZvsRawTof[nTof]->GetXaxis()->SetTitleSize(0.045);
            fh2_MusZvsRawTof[nTof]->GetYaxis()->SetLabelSize(0.045);
            fh2_MusZvsRawTof[nTof]->GetYaxis()->SetTitleSize(0.045);
            fh2_MusZvsRawTof[nTof]->Draw("colz");

            nTof++;
        }
    }

    // === HIT DATA AoverQ VERSUS Q === //
    cAqvsq = new TCanvas("FRSv_AoverQ_vs_Q", "A/q versus q 2D", 10, 10, 800, 700);
    fh2_Aqvsq = new TH2F("fh2v_Aq_vs_q_frs", "FRS: A/q vs q", 3000, 1., 3, 1300, 8, 39.5);
    fh2_Aqvsq->GetXaxis()->SetTitle("A/q");
    fh2_Aqvsq->GetYaxis()->SetTitle("Z [Charge units]");
    fh2_Aqvsq->GetYaxis()->SetTitleOffset(1.1);
    fh2_Aqvsq->GetXaxis()->CenterTitle(true);
    fh2_Aqvsq->GetYaxis()->CenterTitle(true);
    fh2_Aqvsq->GetXaxis()->SetLabelSize(0.045);
    fh2_Aqvsq->GetXaxis()->SetTitleSize(0.045);
    fh2_Aqvsq->GetYaxis()->SetLabelSize(0.045);
    fh2_Aqvsq->GetYaxis()->SetTitleSize(0.045);
    fh2_Aqvsq->Draw("colz");
#endif

    // --- --------------- --- //
    // --- MAIN FOLDER-Sci --- //
    // --- --------------- --- //
    TFolder* mainfolSci = new TFolder("SofSCI", "SOFSCI info");
    for (Int_t i = 0; i < NbDetectors; i++)
    {
        mainfolSci->Add(cSciMult[i]);
        mainfolSci->Add(cSciRawPos[i]);
    }
#ifdef NUMBER_OF_SOFSCI_TOF
    for (Int_t i = 0; i < NbTof; i++)
    {
        mainfolSci->Add(cSciRawTof[i]);
    }
#endif
    mainfolSci->Add(cMusicDTvsRawPos);
    mainfolSci->Add(cMwpc0vsRawPos);
    run->AddObject(mainfolSci);

    TFolder* mainfolID = new TFolder("SofSCI-Music", "SofSCI-Music Incoming info");
    for (UShort_t d = 0; d < NbDetectors; d++)
        mainfolID->Add(cMusicZvsRawPos[d]);
#ifdef NUMBER_OF_SOFSCI_TOF
    for (UShort_t t = 0; t < NbTof; t++)
        mainfolID->Add(cMusicZvsRawTof[t]);
    mainfolID->Add(cAqvsq);
#endif
    run->AddObject(mainfolID);

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
        fh1_RawPos_AtSingleTcal[i]->Reset();
        // === R3BMUSIC === //
        fh2_MusZvsRawPos[i]->Reset();
    }
#ifdef NUMBER_OF_SOFSCI_TOF
    for (Int_t i = 0; i < NbTof; i++)
    {
        // === RAW TIME_OF_FLIGHT === //
        fh1_RawTof_AtTcalMult1[i]->Reset();
        fh1_RawTof_AtTcalMult1_wTref[i]->Reset();
        fh1_RawTof_AtSingleTcal_wTref[i]->Reset();
        // === R3BMUSIC === //
        fh2_MusZvsRawTof[i]->Reset();
    }
#endif
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

    // --- ------------------------------------ --- //
    // --- variables while looping over the data --- //
    // --- ------------------------------------ --- //
    // SofSci Mapped data
    UShort_t multMapSci[NbDetectors * NbChannels];
    // SofSci Tcal data
    Double_t iRawTimeNs[NbDetectors * NbChannels];

    // --- -------------- --- //
    // --- initialisation --- //
    // --- -------------- --- //
    for (UShort_t i = 0; i < NbDetectors; i++)
    {
        for (UShort_t j = 0; j < NbChannels; j++)
        {
            multMapSci[i * NbChannels + j] = 0;
        }
    }
    // --- -------------- --- //
    // --- MUSIC Hit data --- //
    // --- -------------- --- //
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

    // --- -------------- --- //
    // --- MUSIC Cal data --- //
    // --- -------------- --- //
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

        // --- ------------------------- --- //
        // --- loop over sci mapped data --- //
        // --- ------------------------- --- //
        nHits = fMappedItemsSci->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofSciMappedData* hitmapped = (R3BSofSciMappedData*)fMappedItemsSci->At(ihit);
            if (!hitmapped)
                continue;
            iDet = hitmapped->GetDetector() - 1;
            iCh = hitmapped->GetPmt() - 1;
            multMapSci[iDet * NbChannels + iCh]++;
            fh1_finetime[iDet * NbChannels + iCh]->Fill(hitmapped->GetTimeFine());
        }

        // --- ----------------------- --- //
        // --- loop over sci tcal data --- //
        // --- ----------------------- --- //
        if (fTcalItemsSci)
        {
            nHits = fTcalItemsSci->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BSofSciTcalData* hittcal = (R3BSofSciTcalData*)fTcalItemsSci->At(ihit);
                if (!hittcal)
                    continue;
                iDet = hittcal->GetDetector() - 1;
                iCh = hittcal->GetPmt() - 1;
                iRawTimeNs[iDet * NbChannels + iCh] = hittcal->GetRawTimeNs();
            }
        }

        // --- ------------------------------ --- //
        // --- loop over sci single tcal data --- //
        // --- ------------------------------ --- //
        Double_t xs2 = -10000.;
        double toff, Beta_S2_Cave, Gamma_S2_Cave, Brho_S2_Cave;
        double slope_calib = -5.8; // only for the s467
        if (fSingleTcalItemsSci)
        {
            nHits = fSingleTcalItemsSci->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BSofSciSingleTcalData* hitsingletcal = (R3BSofSciSingleTcalData*)fSingleTcalItemsSci->At(ihit);
                if (!hitsingletcal)
                    continue;
                for (UShort_t d = 0; d < NbDetectors; d++)
                {
                    fh1_RawPos_AtSingleTcal[d]->Fill(hitsingletcal->GetRawPosNs(d + 1));
                    if (MusicZ > 0)
                        fh2_MusZvsRawPos[d]->Fill(hitsingletcal->GetRawPosNs(d + 1), MusicZ);
                    if (d == 3)
                        fh2_MusDTvsRawPos->Fill(hitsingletcal->GetRawPosNs(d + 1), MusicDT);
                    if (d == 1)
                        xs2 = hitsingletcal->GetRawPosNs(d + 1) * slope_calib;
                }
#ifdef NUMBER_OF_SOFSCI_TOF
                for (UShort_t t = 0; t < NbTof; t++)
                {
                    fh1_RawTof_AtSingleTcal_wTref[t]->Fill(hitsingletcal->GetRawTofNs(t));
                    if (MusicZ > 0)
                    {
                        fh2_MusZvsRawTof[t]->Fill(hitsingletcal->GetRawTofNs(t), MusicZ);
                        if (t == 3)
                        {
                            toff = hitsingletcal->GetRawTofNs(t + 1);
                            // std::cout <<"tof "<< toff << std::endl;
                            Beta_S2_Cave = 15424.3 / (toff + 675. - 1922.) / 29.9999; // After run 336
                            Gamma_S2_Cave = 1. / (TMath::Sqrt(1. - (Beta_S2_Cave) * (Beta_S2_Cave)));
                            Brho_S2_Cave = 9.048 * (1 + xs2 / 726.); //+mwpc0x/10./2000);
                            fh2_Aqvsq->Fill(Brho_S2_Cave / (3.10716 * Gamma_S2_Cave * Beta_S2_Cave), MusicZ);
                        } // end of A/Q for

                    } // end of if MUSICZ>0
                }
#endif
            }
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
        xs2 = -10000.;
        for (UShort_t i = 0; i < NbDetectors; i++)
        {
            for (UShort_t j = 0; j < NbChannels; j++)
            {
                fh2_mult[i]->Fill(j + 1, multMapSci[i * NbChannels + j]);
            }
            if ((multMapSci[i * NbChannels] > 0) && (multMapSci[i * NbChannels + 1] > 0))
            {
                // x position increases from left to right : TrawRIGHT - TrawLEFT
                possci = iRawTimeNs[i * NbChannels] - iRawTimeNs[i * NbChannels + 1];
                fh1_RawPos_AtTcalMult1[i]->Fill(possci);
                if (i == 1)
                    xs2 = possci;

                if (mwpc0x > 0 && possci > -10. && possci < 10. && i == 3)
                {
                    fh2_Mwpc0vsRawPos->Fill(possci, mwpc0x + gRandom->Uniform(-0.5, 0.5));
                }
            }
        }
#ifdef NUMBER_OF_SOFSCI_TOF
        Int_t indexTof = 0;
        Double_t iRawTof_wTref;
        Double_t iRawTof;
        for (UShort_t dstart = 0; dstart < NbDetectors - 1; dstart++)
        {
            for (UShort_t dstop = dstart + 1; dstop < NbDetectors; dstop++)
            {
                if ((multMapSci[dstart * NbChannels] == 1) && (multMapSci[dstart * NbChannels + 1] == 1) &&
                    (multMapSci[dstop * NbChannels] == 1) && (multMapSci[dstop * NbChannels + 1] == 1))
                {
                    iRawTof = 0.5 * (iRawTimeNs[dstop * NbChannels] + iRawTimeNs[dstop * NbChannels + 1]) -
                              0.5 * (iRawTimeNs[dstart * NbChannels] + iRawTimeNs[dstart * NbChannels + 1]);
                    fh1_RawTof_AtTcalMult1[indexTof]->Fill(iRawTof);

                    if (multMapSci[dstart * NbChannels + 2] == 1 && multMapSci[dstop * NbChannels + 2] == 1)
                    {
                        iRawTof_wTref =
                            iRawTof + iRawTimeNs[dstart * NbChannels + 2] - iRawTimeNs[dstop * NbChannels + 2];
                        fh1_RawTof_AtTcalMult1_wTref[indexTof]->Fill(iRawTof_wTref);
                    } // end of if mult==1 also for the Tref
                }     // end of if mult==1 in the left and right pmts
                indexTof++;
            } // end of for (dstop)
        }     // end of for (dstart)
#endif
    }
    fNEvents += 1;
}

// -----   Public method Reset   ------------------------------------------------
void R3BSofSciOnlineSpectra::Reset()
{
    LOG(DEBUG) << "Clearing TofWHitData Structure";
    if (fTofwHitData)
        fTofwHitData->Clear();
}

// -----   Public method Finish   -----------------------------------------------
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
#ifdef NUMBER_OF_SOFSCI_TOF
        for (UShort_t i = 0; i < NbTof; i++)
        {
            fh1_RawTof_AtTcalMult1[i]->Write();
            fh1_RawTof_AtTcalMult1_wTref[i]->Write();
            cSciRawTof[i]->Write();
        }
#endif
        if (fMusHitItems)
        {
            for (UShort_t d = 0; d < NbDetectors; d++)
                fh2_MusZvsRawPos[d]->Write();
        }
        if (fMusCalItems)
        {
#ifdef NUMBER_OF_SOFSCI_TOF
            for (UShort_t t = 0; t < NbTof; t++)
                fh2_MusZvsRawTof[t]->Write();
#endif
            fh2_MusDTvsRawPos->Write();
        }
        if (fCalItemsMwpc0)
            fh2_Mwpc0vsRawPos->Write();
    }
}

ClassImp(R3BSofSciOnlineSpectra)
