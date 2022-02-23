// ------------------------------------------------------------
// -----              R3BSofTofWOnlineSpectra             -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofTofWOnlineSpectra.h"
#include "R3BEventHeader.h"
#include "R3BMwpcCalData.h"
#include "R3BSofSciSingleTcalData.h"
#include "R3BSofTofWHitData.h"
#include "R3BSofTofWMappedData.h"
#include "R3BSofTofWSingleTcalData.h"
#include "R3BSofTofWTcalData.h"
#include "R3BTwimHitData.h"
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

R3BSofTofWOnlineSpectra::R3BSofTofWOnlineSpectra()
    : FairTask("SofTofWOnlineSpectra", 1)
    , fMappedItemsTofW(NULL)
    , fTcalItemsTofW(NULL)
    , fSingleTcalItemsTofW(NULL)
    , fHitItemsTofW(NULL)
    , fSingleTcalItemsSci(NULL)
    , fHitItemsTwim(NULL)
    , fCalItemsMwpc(NULL)
    , fTwimTofRangeMax(-65.)
    , fTwimTofRangeMin(-87.)
    , fIdSofSciCaveC(1)
    , fNEvents(0)
{
}

R3BSofTofWOnlineSpectra::R3BSofTofWOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMappedItemsTofW(NULL)
    , fTcalItemsTofW(NULL)
    , fSingleTcalItemsTofW(NULL)
    , fHitItemsTofW(NULL)
    , fSingleTcalItemsSci(NULL)
    , fHitItemsTwim(NULL)
    , fCalItemsMwpc(NULL)
    , fTwimTofRangeMax(-65.)
    , fTwimTofRangeMin(-87.)
    , fIdSofSciCaveC(1)
    , fNEvents(0)
{
}

R3BSofTofWOnlineSpectra::~R3BSofTofWOnlineSpectra()
{
    LOG(INFO) << "R3BSofTofWOnlineSpectra::Delete instance";
    if (fMappedItemsTofW)
        delete fMappedItemsTofW;
    if (fTcalItemsTofW)
        delete fTcalItemsTofW;
    if (fSingleTcalItemsTofW)
        delete fSingleTcalItemsTofW;
    if (fHitItemsTofW)
        delete fHitItemsTofW;
    if (fSingleTcalItemsSci)
        delete fSingleTcalItemsSci;
    if (fHitItemsTwim)
        delete fHitItemsTwim;
    if (fCalItemsMwpc)
        delete fCalItemsMwpc;
}

InitStatus R3BSofTofWOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofTofWOnlineSpectra::Init ";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTofWOnlineSpectra::Init FairRootManager not found";
    // header = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // --- ------------------------------------- --- //
    // --- get access to mapped data of the TofW --- //
    // --- ------------------------------------- --- //
    fMappedItemsTofW = (TClonesArray*)mgr->GetObject("SofTofWMappedData");
    if (!fMappedItemsTofW)
    {
        return kFATAL;
    }

    // --- ----------------------------------- --- //
    // --- get access to tcal data of the TofW --- //
    // --- ----------------------------------- --- //
    fTcalItemsTofW = (TClonesArray*)mgr->GetObject("SofTofWTcalData");
    if (!fTcalItemsTofW)
    {
        return kFATAL;
    }

    // --- ------------------------------------------ --- //
    // --- get access to single tcal data of the TofW --- //
    // --- ------------------------------------------ --- //
    fSingleTcalItemsTofW = (TClonesArray*)mgr->GetObject("SofTofWSingleTcalData");
    if (!fSingleTcalItemsTofW)
    {
        LOG(WARNING) << "R3BSofTofWOnlineSpectra: SofTofWSingleTcalData not found";
    }

    // --- ------------------------------------------ --- //
    // --- get access to hit data of the TofW         --- //
    // --- ------------------------------------------ --- //
    fHitItemsTofW = (TClonesArray*)mgr->GetObject("TofWHitData");
    if (!fHitItemsTofW)
    {
        LOG(WARNING) << "R3BSofTofWOnlineSpectra: TofWHitData not found";
    }

    // --- ----------------------------------------- --- //
    // --- get access to single tcal data of the Sci --- //
    // --- ----------------------------------------- --- //
    fSingleTcalItemsSci = (TClonesArray*)mgr->GetObject("SofSciSingleTcalData");
    if (!fSingleTcalItemsSci)
    {
        return kFATAL;
    }

    // get access to hit data of the TWIM
    fHitItemsTwim = (TClonesArray*)mgr->GetObject("TwimHitData");
    if (!fHitItemsTwim)
        LOG(WARNING) << "R3BSofTofWOnlineSpectra: TwimHitData not found";

    // get access to cal data of the MWPC3
    fCalItemsMwpc = (TClonesArray*)mgr->GetObject("Mwpc3CalData");
    if (!fCalItemsMwpc)
        LOG(WARNING) << "R3BSofTofWOnlineSpectra: Mwpc3CalData not found";

    // --- ------------------------------- --- //
    // --- Create histograms for detectors --- //
    // --- ------------------------------- --- //
    char Name1[255];
    char Name2[255];

    cTofWMult = new TCanvas("TofW_mult", "TofW_mult", 0, 0, 1000, 500);
    cTofWMult->Divide(2, 1);

    for (Int_t j = 0; j < NbChs; j++)
    {

        // === MULT AT MAPPED LEVEL === //
        sprintf(Name1, "SofTofW_Pmt%i_MultPerPlastic", j + 1);
        fh2_mult[j] = new TH2I(Name1, Name1, NbDets + 2, -0.5, NbDets + 1.5, 5, -0.5, 4.5);
        fh2_mult[j]->GetXaxis()->SetTitle("Plastic number [1-28]");
        fh2_mult[j]->GetYaxis()->SetTitle("Multiplicity per plastic");
        fh2_mult[j]->GetYaxis()->SetTitleOffset(0.9);
        fh2_mult[j]->GetXaxis()->CenterTitle(true);
        fh2_mult[j]->GetYaxis()->CenterTitle(true);
        cTofWMult->cd(j + 1);
        fh2_mult[j]->Draw("COL");

        // === FINE TIME === //
        sprintf(Name1, "SofTofW_FineTime_Pmt%i", j + 1);
        cTofWFineTime[j] = new TCanvas(Name1, Name1, 10, 10, 1000, 900);
        cTofWFineTime[j]->Divide(7, 4);
        for (Int_t i = 0; i < NbDets; i++)
        {
            sprintf(Name1, "SofTofW%i_FineTimePmt%i", i + 1, j + 1);
            fh1_finetime[i * NbChs + j] = new TH1I(Name1, Name1, 1000, 0, 1000);
            cTofWFineTime[j]->cd(i + 1);
            fh1_finetime[i * NbChs + j]->Draw("");
        }
    }

    // === RAW POSITION === //
    sprintf(Name1, "SofTofW_RawPos_AtTcal_Mult1");
    cTofWRawPos = new TCanvas(Name1, Name1, 10, 10, 1000, 900);
    cTofWRawPos->Divide(7, 4);
    sprintf(Name1, "SofTofW_RawPos_AtSingleTcal");
    cTofWRawPosST = new TCanvas(Name1, Name1, 10, 10, 1000, 900);
    cTofWRawPosST->Divide(7, 4);
    for (Int_t i = 0; i < NbDets; i++)
    {
        sprintf(Name1, "SofTofW%i_RawPosAtTcal_Mult1", i + 1);
        fh1_RawPos_AtTcalMult1[i] = new TH1F(Name1, Name1, 40000, -20, 20);
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->SetTitle("Raw position [ns with one bin/ps]");
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->SetTitle("Counts per bin");
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->CenterTitle(true);
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->CenterTitle(true);
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_RawPos_AtTcalMult1[i]->GetYaxis()->SetTitleSize(0.045);
        cTofWRawPos->cd(i + 1);
        fh1_RawPos_AtTcalMult1[i]->Draw("");

        sprintf(Name1, "SofTofW%i_RawPosAtSingleTcal", i + 1);
        fh1_RawPos_AtSingleTcal[i] = new TH1F(Name1, Name1, 40000, -20, 20);
        fh1_RawPos_AtSingleTcal[i]->GetXaxis()->SetTitle("Raw position [ns with one bin/ps]");
        fh1_RawPos_AtSingleTcal[i]->GetYaxis()->SetTitle("Counts per bin");
        fh1_RawPos_AtSingleTcal[i]->GetXaxis()->CenterTitle(true);
        fh1_RawPos_AtSingleTcal[i]->GetYaxis()->CenterTitle(true);
        fh1_RawPos_AtSingleTcal[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_RawPos_AtSingleTcal[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_RawPos_AtSingleTcal[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_RawPos_AtSingleTcal[i]->GetYaxis()->SetTitleSize(0.045);
        cTofWRawPosST->cd(i + 1);
        fh1_RawPos_AtSingleTcal[i]->Draw("");
    }

    // === ENE RAW === //
    for (Int_t i = 0; i < NbChs; i++)
    {
        sprintf(Name1, "SofTofW_EneRaw_pmt%i", i + 1);
        cTofWEneRaw[i] = new TCanvas(Name1, Name1, 10, 10, 1000, 900);
        cTofWEneRaw[i]->Divide(7, 4);

        for (Int_t j = 0; j < NbDets; j++)
        {
            sprintf(Name1, "SofTofW%i_EneRaw_pmt%i", j + 1, i + 1);
            fh1_EneRaw[j * NbChs + i] = new TH1D(Name1, Name1, 5000, 0, 5000);
            fh1_EneRaw[j * NbChs + i]->GetXaxis()->SetTitle("Raw Energy [channel]");
            fh1_EneRaw[j * NbChs + i]->GetYaxis()->SetTitle("Counts per bin");
            fh1_EneRaw[j * NbChs + i]->GetXaxis()->CenterTitle(true);
            fh1_EneRaw[j * NbChs + i]->GetYaxis()->CenterTitle(true);
            fh1_EneRaw[j * NbChs + i]->GetXaxis()->SetLabelSize(0.045);
            fh1_EneRaw[j * NbChs + i]->GetXaxis()->SetTitleSize(0.045);
            fh1_EneRaw[j * NbChs + i]->GetYaxis()->SetLabelSize(0.045);
            fh1_EneRaw[j * NbChs + i]->GetYaxis()->SetTitleSize(0.045);
            cTofWEneRaw[i]->cd(j + 1);
            fh1_EneRaw[j * NbChs + i]->Draw("");
        }
    }

    // === RAW TIME-OF-Flight === //
    for (Int_t i = 0; i < NbDets; i++)
    {
        sprintf(Name1, "SofTofW_P%i_RawTof_AtTcal_Mult1", i + 1);
        cTofWRawTof[i] = new TCanvas(Name1, Name1, 10, 10, 1000, 900);
        cTofWRawTof[i]->Divide(1, 2);
        sprintf(Name1, "SofTofW%i_RawTofAtTcal_Mult1", i + 1);
        fh1_RawTof_AtTcalMult1[i] = new TH1D(Name1, Name1, 100000, -1000, 1000);
        fh1_RawTof_AtTcalMult1[i]->GetXaxis()->SetTitle("Raw time-of-flight [ns with one bin/ps]");
        fh1_RawTof_AtTcalMult1[i]->GetYaxis()->SetTitle("Counts per bin");
        fh1_RawTof_AtTcalMult1[i]->GetXaxis()->CenterTitle(true);
        fh1_RawTof_AtTcalMult1[i]->GetYaxis()->CenterTitle(true);
        fh1_RawTof_AtTcalMult1[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_RawTof_AtTcalMult1[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_RawTof_AtTcalMult1[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_RawTof_AtTcalMult1[i]->GetYaxis()->SetTitleSize(0.045);
        cTofWRawTof[i]->cd(1);
        fh1_RawTof_AtTcalMult1[i]->Draw("");
        sprintf(Name1, "SofTofW%i_RawTofAtSingleTcal", i + 1);
        fh1_RawTof_AtSingleTcal[i] = new TH1D(Name1, Name1, 100000, -1000, 1000);
        fh1_RawTof_AtSingleTcal[i]->GetXaxis()->SetTitle("Raw time-of-flight [ns, 1ps/bin]");
        fh1_RawTof_AtSingleTcal[i]->GetYaxis()->SetTitle("Counts per bin");
        fh1_RawTof_AtSingleTcal[i]->GetXaxis()->CenterTitle(true);
        fh1_RawTof_AtSingleTcal[i]->GetYaxis()->CenterTitle(true);
        fh1_RawTof_AtSingleTcal[i]->GetXaxis()->SetLabelSize(0.045);
        fh1_RawTof_AtSingleTcal[i]->GetXaxis()->SetTitleSize(0.045);
        fh1_RawTof_AtSingleTcal[i]->GetYaxis()->SetLabelSize(0.045);
        fh1_RawTof_AtSingleTcal[i]->GetYaxis()->SetTitleSize(0.045);
        cTofWRawTof[i]->cd(2);
        fh1_RawTof_AtSingleTcal[i]->Draw("");
    }

    // === Twim vs TIME-OF-Flight raw === //
    for (Int_t i = 0; i < NbDets; i++)
    {
        sprintf(Name1, "Twim_vs_Tof_Plastic_%i", i + 1);
        cTwimvsTof[i] = new TCanvas(Name1, Name1, 10, 10, 1000, 900);
        sprintf(Name1, "fh2_Twim_vs_ToF_Plastic_%i", i + 1);
        sprintf(Name2, "Twim vs ToF for plastic %i", i + 1);
        fh2_Twim_Tof[i] = new TH2F(Name1, Name2, 7000, fTwimTofRangeMin, fTwimTofRangeMax, 1000, 5, 95);
        fh2_Twim_Tof[i]->GetXaxis()->SetTitle("Raw time-of-flight [ns with one bin/ps]");
        fh2_Twim_Tof[i]->GetYaxis()->SetTitle("Charge Z");
        fh2_Twim_Tof[i]->GetXaxis()->CenterTitle(true);
        fh2_Twim_Tof[i]->GetYaxis()->CenterTitle(true);
        fh2_Twim_Tof[i]->GetXaxis()->SetLabelSize(0.045);
        fh2_Twim_Tof[i]->GetXaxis()->SetTitleSize(0.045);
        fh2_Twim_Tof[i]->GetYaxis()->SetLabelSize(0.045);
        fh2_Twim_Tof[i]->GetYaxis()->SetTitleSize(0.045);
        cTwimvsTof[i]->cd();
        fh2_Twim_Tof[i]->Draw("col");
    }

    sprintf(Name1, "Mwpc3X_vs_ToF_Plastic");
    sprintf(Name2, "Mwpc3X vs ToF-Plastic number");
    cMwpc3XvsTof = new TCanvas(Name1, Name2, 10, 10, 1000, 900);
    sprintf(Name1, "fh2_Mwpc3X_vs_ToF_Plastic");
    sprintf(Name2, "Mwpc3X vs ToF for plastic number");
    fh2_Mwpc3X_Tof = new TH2F(Name1, Name2, 28 * 8, 0.5, 28.5, 288 * 8, 0.5, 288.5);
    fh2_Mwpc3X_Tof->GetXaxis()->SetTitle("TofW-Plastic number [1-28]");
    fh2_Mwpc3X_Tof->GetYaxis()->SetTitle("MWPC3-X [pads]");
    fh2_Mwpc3X_Tof->GetXaxis()->CenterTitle(true);
    fh2_Mwpc3X_Tof->GetYaxis()->CenterTitle(true);
    fh2_Mwpc3X_Tof->GetXaxis()->SetLabelSize(0.045);
    fh2_Mwpc3X_Tof->GetXaxis()->SetTitleSize(0.045);
    fh2_Mwpc3X_Tof->GetYaxis()->SetLabelSize(0.045);
    fh2_Mwpc3X_Tof->GetYaxis()->SetTitleSize(0.045);
    cMwpc3XvsTof->cd();
    fh2_Mwpc3X_Tof->Draw("col");

    for (Int_t i = 0; i < NbDets; i++)
    {
        sprintf(Name1, "Mwpc3Y_vs_PosToF_Plastic%i", i + 1);
        cMwpc3YvsPosTof[i] = new TCanvas(Name1, Name1, 10, 10, 1000, 900);
        sprintf(Name1, "fh2_Mwpc3Y_vs_PosToF_Plastic%i", i + 1);
        fh2_Mwpc3Y_PosTof[i] = new TH2F(Name1, Name1, 1000, -20, 20, 240, 0, 120);
        fh2_Mwpc3Y_PosTof[i]->GetXaxis()->SetTitle("Pos TofW-Plastic [ps]");
        fh2_Mwpc3Y_PosTof[i]->GetYaxis()->SetTitle("MWPC3-Y [pads]");
        fh2_Mwpc3Y_PosTof[i]->GetXaxis()->CenterTitle(true);
        fh2_Mwpc3Y_PosTof[i]->GetYaxis()->CenterTitle(true);
        fh2_Mwpc3Y_PosTof[i]->GetXaxis()->SetLabelSize(0.045);
        fh2_Mwpc3Y_PosTof[i]->GetXaxis()->SetTitleSize(0.045);
        fh2_Mwpc3Y_PosTof[i]->GetYaxis()->SetLabelSize(0.045);
        fh2_Mwpc3Y_PosTof[i]->GetYaxis()->SetTitleSize(0.045);
        cMwpc3YvsPosTof[i]->cd();
        fh2_Mwpc3Y_PosTof[i]->Draw("col");
    }

    // Hit ToFW data
    sprintf(Name1, "Tof-hit_vs_Plastic");
    sprintf(Name2, "ToF-hit vs Plastic number");
    cTofvsplas = new TCanvas(Name1, Name2, 10, 10, 1000, 900);
    sprintf(Name1, "fh2_ToF-hit_vs_Plastic");
    sprintf(Name2, "ToF-hit vs plastic number");
    fh2_Tof_hit = new TH2F(Name1, Name2, 28 * 8, 0.5, 28.5, 5000, 0., 50.);
    fh2_Tof_hit->GetXaxis()->SetTitle("TofW-Plastic number [1-28]");
    fh2_Tof_hit->GetYaxis()->SetTitle("ToF-hit [ns]");
    fh2_Tof_hit->GetXaxis()->CenterTitle(true);
    fh2_Tof_hit->GetYaxis()->CenterTitle(true);
    fh2_Tof_hit->GetXaxis()->SetLabelSize(0.045);
    fh2_Tof_hit->GetXaxis()->SetTitleSize(0.045);
    fh2_Tof_hit->GetYaxis()->SetLabelSize(0.045);
    fh2_Tof_hit->GetYaxis()->SetTitleSize(0.045);
    cTofvsplas->cd();
    fh2_Tof_hit->Draw("col");

    sprintf(Name1, "Pos-hit_vs_Plastic");
    sprintf(Name2, "Pos-hit vs Plastic number");
    cPosvsplas = new TCanvas(Name1, Name2, 10, 10, 1000, 900);
    sprintf(Name1, "fh2_Pos-hit_vs_Plastic");
    sprintf(Name2, "Pos-hit vs plastic number");
    fh2_Pos_hit = new TH2F(Name1, Name2, 28 * 8, 0.5, 28.5, 6000, -300., 300.);
    fh2_Pos_hit->GetXaxis()->SetTitle("TofW-Plastic number [1-28]");
    fh2_Pos_hit->GetYaxis()->SetTitle("Pos-hit [mm]");
    fh2_Pos_hit->GetXaxis()->CenterTitle(true);
    fh2_Pos_hit->GetYaxis()->CenterTitle(true);
    fh2_Pos_hit->GetXaxis()->SetLabelSize(0.045);
    fh2_Pos_hit->GetXaxis()->SetTitleSize(0.045);
    fh2_Pos_hit->GetYaxis()->SetLabelSize(0.045);
    fh2_Pos_hit->GetYaxis()->SetTitleSize(0.045);
    cPosvsplas->cd();
    fh2_Pos_hit->Draw("col");

    // --- --------------- --- //
    // --- MAIN FOLDER-TofW --- //
    // --- --------------- --- //
    TFolder* mainfolTofW = new TFolder("SOFTOFW", "SOFTOFW info");
    for (Int_t j = 0; j < NbChs; j++)
    {
        mainfolTofW->Add(cTofWFineTime[j]);
        mainfolTofW->Add(cTofWEneRaw[j]);
    }
    mainfolTofW->Add(cTofWMult);
    mainfolTofW->Add(cTofWRawPos);
    mainfolTofW->Add(cTofWRawPosST);
    mainfolTofW->Add(cMwpc3XvsTof);

    // --- --------------- ---
    // --- FOLDER-TofW raw data
    // --- --------------- ---
    TFolder* folTofWRawTof = new TFolder("RawTof", "TOFW Raw info");
    for (Int_t i = 0; i < NbDets; i++)
        folTofWRawTof->Add(cTofWRawTof[i]);

    // --- --------------- ---
    // --- FOLDER-TofW hit data
    // --- --------------- ---
    TFolder* folTofWhit = new TFolder("HitTof", "TOFW Hit info");
    if (fHitItemsTofW)
    {
        folTofWhit->Add(cTofvsplas);
        folTofWhit->Add(cPosvsplas);
    }
    // --- --------------- ---
    // --- FOLDER-Twim vs TofW
    // --- --------------- ---
    TFolder* folTwimvsToF = new TFolder("Twim_vs_TofW", "Twim vs TOFW info");
    for (Int_t i = 0; i < NbDets; i++)
        folTwimvsToF->Add(cTwimvsTof[i]);

    // --- --------------- ---
    // --- FOLDER-MWPC3Y  vs ÃPosition TofW
    // --- --------------- ---
    TFolder* folMWPC3YvsToFPos = new TFolder("MWPC3Y_vs_ToFPos", "MWPC3Y vs TOFW position");
    for (Int_t i = 0; i < NbDets; i++)
        folMWPC3YvsToFPos->Add(cMwpc3YvsPosTof[i]);

    mainfolTofW->Add(folTofWRawTof);
    if (fHitItemsTofW)
        mainfolTofW->Add(folTofWhit);
    mainfolTofW->Add(folTwimvsToF);
    mainfolTofW->Add(folMWPC3YvsToFPos);

    run->AddObject(mainfolTofW);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_SOFTOFW_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofTofWOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofTofWOnlineSpectra::Reset_Histo";
    for (Int_t j = 0; j < NbChs; j++)
    {
        // === MULT === //
        fh2_mult[j]->Reset();
        for (Int_t i = 0; i < NbDets; i++)
        {
            // === FINE TIME === //
            fh1_finetime[i * NbChs + j]->Reset();
            fh1_EneRaw[i * NbChs + j]->Reset();
        }
    }

    for (Int_t i = 0; i < NbDets; i++)
    {
        // === RAW POSITION === //
        fh1_RawPos_AtTcalMult1[i]->Reset();
        fh1_RawPos_AtSingleTcal[i]->Reset();
        // === RAW TIME-OF-FLIGHT === //
        fh1_RawTof_AtTcalMult1[i]->Reset();
        fh1_RawTof_AtSingleTcal[i]->Reset();
    }
    for (UShort_t i = 0; i < NbDets; i++)
    {
        fh2_Twim_Tof[i]->Reset();
        fh2_Mwpc3Y_PosTof[i]->Reset();
    }

    fh2_Mwpc3X_Tof->Reset();

    if (fHitItemsTofW)
    {
        fh2_Tof_hit->Reset();
        fh2_Pos_hit->Reset();
    }
}

void R3BSofTofWOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofTofWOnlineSpectra::Exec FairRootManager not found";

    Int_t nHits;
    UShort_t iDet; // 0-bsed
    UShort_t iCh;  // 0-based
    Double_t iRawTimeNs[NbDets * 2];
    UShort_t mult[NbDets * NbChs];

    // --- -------------- --- //
    // --- initialisation --- //
    // --- -------------- --- //
    for (UShort_t i = 0; i < NbDets; i++)
    {
        for (UShort_t j = 0; j < NbChs; j++)
        {
            mult[i * NbChs + j] = 0;
        }
    }

    if (fSingleTcalItemsTofW && fSingleTcalItemsTofW->GetEntriesFast())
    {
        // --- ------------------------- --- //
        // --- loop over singe tcal data --- //
        // --- ------------------------- --- //
        nHits = fSingleTcalItemsTofW->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTofWSingleTcalData* hitST = (R3BSofTofWSingleTcalData*)fSingleTcalItemsTofW->At(ihit);
            fh1_RawPos_AtSingleTcal[hitST->GetDetector() - 1]->Fill(hitST->GetRawPosNs());
            fh1_RawTof_AtSingleTcal[hitST->GetDetector() - 1]->Fill(hitST->GetRawTofNs());
        } // end of loop over the singletcal data
    }

    // Get ToFW hit data
    if (fHitItemsTofW && fHitItemsTofW->GetEntriesFast() > 0)
    {
        nHits = fHitItemsTofW->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTofWHitData* hit = (R3BSofTofWHitData*)fHitItemsTofW->At(ihit);
            if (!hit)
                continue;
            fh2_Tof_hit->Fill(hit->GetPaddle() + gRandom->Uniform(-0.49, 0.49), hit->GetTof());
            fh2_Pos_hit->Fill(hit->GetPaddle() + gRandom->Uniform(-0.49, 0.49), hit->GetY());
        }
    }

    if (fMappedItemsTofW && fMappedItemsTofW->GetEntriesFast() && fTcalItemsTofW && fTcalItemsTofW->GetEntriesFast())
    {

        // --- --------------------- --- //
        // --- loop over mapped data --- //
        // --- --------------------- --- //
        nHits = fMappedItemsTofW->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTofWMappedData* hitmapped = (R3BSofTofWMappedData*)fMappedItemsTofW->At(ihit);
            if (!hitmapped)
                continue;
            iDet = hitmapped->GetDetector() - 1;
            iCh = hitmapped->GetPmt() - 1;
            mult[iDet * NbChs + iCh]++;
            fh1_finetime[iDet * NbChs + iCh]->Fill(hitmapped->GetTimeFine());
            fh1_EneRaw[iDet * NbChs + iCh]->Fill(hitmapped->GetEnergy());
        }

        // --- ------------------- --- //
        // --- loop over tcal data --- //
        // --- ------------------- --- //
        nHits = fTcalItemsTofW->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTofWTcalData* hittcal = (R3BSofTofWTcalData*)fTcalItemsTofW->At(ihit);
            if (!hittcal)
                continue;
            if (hittcal->GetPmt() == 3)
                continue;
            iDet = hittcal->GetDetector() - 1;
            iCh = hittcal->GetPmt() - 1;
            iRawTimeNs[iDet * 2 + iCh] = hittcal->GetRawTimeNs();
            ;
        }

        // --- ----------------------------------- --- //
        // --- Get the Time at the Start detectors --- //
        // --- ----------------------------------- --- //
        Double_t TrawStart = -1000000.;
        if (fSingleTcalItemsSci)
        {
            for (Int_t i = 0; i < fSingleTcalItemsSci->GetEntriesFast(); i++)
            {
                R3BSofSciSingleTcalData* hitsingletcalsci = (R3BSofSciSingleTcalData*)fSingleTcalItemsSci->At(i);
                if (hitsingletcalsci && hitsingletcalsci->GetDetector() == fIdSofSciCaveC)
                {
                    TrawStart = hitsingletcalsci->GetRawTimeNs();
                }
            } // end of if mult=1 in the Start
        }

        // Get hit data Twim
        Double_t twimZ = 0.;
        if (fHitItemsTwim && fHitItemsTwim->GetEntriesFast() > 0)
        {
            nHits = fHitItemsTwim->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BTwimHitData* hit = (R3BTwimHitData*)fHitItemsTwim->At(ihit);
                if (!hit)
                    continue;
                twimZ = hit->GetZcharge();
            }
        }

        // Get cal data MWPC3
        Double_t mwpc3x = -1., qmax = -100.;
        Double_t mwpc3y = -1., qmay = -100.;
        if (fCalItemsMwpc && fCalItemsMwpc->GetEntriesFast() > 0)
        {
            nHits = fCalItemsMwpc->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BMwpcCalData* hit = (R3BMwpcCalData*)fCalItemsMwpc->At(ihit);
                if (!hit)
                    continue;
                if (hit->GetQ() > qmax && hit->GetPlane() == 1)
                {
                    mwpc3x = hit->GetPad();
                    qmax = hit->GetQ();
                }
                if (hit->GetQ() > qmay && hit->GetPlane() == 3)
                {
                    mwpc3y = hit->GetPad();
                    qmay = hit->GetQ();
                }
            }
        }

        // --- ----------------------------------------- --- //
        // --- filling some histogramms outside the loop --- //
        // --- ----------------------------------------- --- //
        Double_t tofw = 0.;
        Double_t tofpos = -1000;
        for (UShort_t i = 0; i < NbDets; i++)
        {
            for (UShort_t j = 0; j < NbChs; j++)
            {
                fh2_mult[j]->Fill(i + 1, mult[i * NbChs + j]);
            }
            if ((mult[i * NbChs] == 1) && (mult[i * NbChs + 1] == 1))
            {
                // Y position is increasing from down to up: PosRaw = TrawDown - TrawUp
                tofpos = (Double_t)(iRawTimeNs[i * NbChs] - iRawTimeNs[i * NbChs + 1]);
                fh1_RawPos_AtTcalMult1[i]->Fill(tofpos);
                if (mwpc3x > 0)
                {
                    fh2_Mwpc3X_Tof->Fill(i + 1 + gRandom->Uniform(-0.5, 0.5), mwpc3x + gRandom->Uniform(-0.5, 0.5));
                }
                if (mwpc3y > 0)
                {
                    fh2_Mwpc3Y_PosTof[i]->Fill(tofpos, mwpc3y + gRandom->Uniform(-0.5, 0.5));
                }
                if (TrawStart != -1000000.)
                {
                    tofw = (0.5 * (iRawTimeNs[i * 2 + 1] + iRawTimeNs[i * 2])) - TrawStart;
                    fh1_RawTof_AtTcalMult1[i]->Fill(tofw);
                    if (twimZ > 0)
                    {
                        fh2_Twim_Tof[i]->Fill(tofw, twimZ);
                    }
                }
            } // end of if mult=1 in the plastic
        }
    }

    fNEvents += 1;
}

void R3BSofTofWOnlineSpectra::FinishEvent()
{
    if (fMappedItemsTofW)
    {
        fMappedItemsTofW->Clear();
    }
    if (fTcalItemsTofW)
    {
        fTcalItemsTofW->Clear();
    }
    if (fSingleTcalItemsTofW)
    {
        fSingleTcalItemsTofW->Clear();
    }
    if (fHitItemsTofW)
    {
        fHitItemsTofW->Clear();
    }
    if (fSingleTcalItemsSci)
    {
        fSingleTcalItemsSci->Clear();
    }
    if (fHitItemsTwim)
    {
        fHitItemsTwim->Clear();
    }
    if (fCalItemsMwpc)
    {
        fCalItemsMwpc->Clear();
    }
}

void R3BSofTofWOnlineSpectra::FinishTask()
{

    if (fMappedItemsTofW)
    {
        cTofWMult->Write();
        for (UShort_t j = 0; j < NbChs; j++)
        {
            fh2_mult[j]->Write();
            cTofWFineTime[j]->Write();
            for (UShort_t i = 0; i < NbDets; i++)
            {
                fh1_finetime[i * NbChs + j]->Write();
                fh1_EneRaw[i * NbChs + j]->Write();
            }
        }
    }

    if (fTcalItemsTofW)
    {
        cTofWRawPos->Write();
        for (UShort_t i = 0; i < NbDets; i++)
        {
            fh1_RawPos_AtTcalMult1[i]->Write();
            if (fSingleTcalItemsSci)
            {
                if (fSingleTcalItemsTofW)
                    cTofWRawTof[i]->Write();
                fh1_RawTof_AtTcalMult1[i]->Write();
            }
        }
    }

    if (fSingleTcalItemsTofW)
    {
        cTofWRawPosST->Write();
        for (UShort_t i = 0; i < NbDets; i++)
        {
            fh1_RawPos_AtSingleTcal[i]->Write();
            fh1_RawTof_AtSingleTcal[i]->Write();
        }
    }

    if (fHitItemsTwim && fTcalItemsTofW)
    {
        for (UShort_t i = 0; i < NbDets; i++)
            cTwimvsTof[i]->Write();
    }

    if (fCalItemsMwpc && fTcalItemsTofW)
    {
        cMwpc3XvsTof->Write();
        for (Int_t i = 0; i < NbDets; i++)
            cMwpc3YvsPosTof[i]->Write();
    }

    if (fHitItemsTofW)
    {
        cTofvsplas->Write();
        cPosvsplas->Write();
    }
}

ClassImp(R3BSofTofWOnlineSpectra)
