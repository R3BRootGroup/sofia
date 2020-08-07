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
    , fNbDetectors(1)
    , fNbChannels(3)
    , fIdS2(0)
    , fIdS8(0)
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
    , fNbDetectors(1)
    , fNbChannels(3)
    , fIdS2(0)
    , fIdS8(0)
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

    cSciMult = new TCanvas*[fNbDetectors];
    fh1_finetime = new TH1I*[fNbDetectors * fNbChannels];
    fh2_mult = new TH2I*[fNbDetectors];
    cSciRawPos = new TCanvas*[fNbDetectors];
    fh1_RawPos_AtTcalMult1 = new TH1F*[fNbDetectors];
    fh1_RawPos_AtSingleTcal = new TH1F*[fNbDetectors];
    cMusicZvsRawPos = new TCanvas*[fNbDetectors];
    fh2_MusZvsRawPos = new TH2F*[fNbDetectors];
    for (Int_t i = 0; i < fNbDetectors; i++)
    {
        // === FINE TIME AND MULT === //
        sprintf(Name1, "SofSci%i_MultAndFt", i + 1);
        cSciMult[i] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
        cSciMult[i]->Divide(2, 2);
        for (Int_t j = 0; j < fNbChannels; j++)
        {
            sprintf(Name1, "SofSci%i_FineTimeCh%i", i + 1, j + 1);
            fh1_finetime[i * fNbChannels + j] = new TH1I(Name1, Name1, 1000, 0, 1000);
            cSciMult[i]->cd(j + 1);
            fh1_finetime[i * fNbChannels + j]->Draw("");
        }
        sprintf(Name1, "SofSci%i_MultPerChannel", i + 1);
        fh2_mult[i] = new TH2I(Name1, Name1, fNbChannels, 0.5, fNbChannels + 0.5, 20, -0.5, 19.5);
        fh2_mult[i]->GetXaxis()->SetTitle("channel: 1=PMT R,    2=PMT L,    3=COMMON REF");
        fh2_mult[i]->GetYaxis()->SetTitle("multiplicity per channel");
        cSciMult[i]->cd(4);
        fh2_mult[i]->Draw("COL");

        // === RAW POSITION AT TCAL LEVEL, MULT==1 === //
        sprintf(Name1, "SofSci%i_RawPos", i + 1);
        cSciRawPos[i] = new TCanvas(Name1, Name1, 10, 10, 500, 500);
        cSciRawPos[i]->Divide(1, 2);
        sprintf(Name1, "SofSci%i_RawPosAtTcal_Mult1", i + 1);
        fh1_RawPos_AtTcalMult1[i] = new TH1F(Name1, Name1, 20000, -10, 10);
        fh1_RawPos_AtTcalMult1[i]->GetXaxis()->SetTitle(
            "(RIGHT,Wix. side) -->  raw position [ns, 1ps/bin] --> (LEFT,Mes. side) -->");
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
        sprintf(Name1, "SofSci%i_RawPosAtSingleTcal", i + 1);
        fh1_RawPos_AtSingleTcal[i] = new TH1F(Name1, Name1, 20000, -10, 10);
        fh1_RawPos_AtSingleTcal[i]->GetXaxis()->SetTitle(
            "(RIGHT,Wix. side) -->  raw position [ns, 1ps/bin] --> (LEFT,Mes. side) -->");
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
        fh2_MusZvsRawPos[i] = new TH2F(Name1, Name1, 2000, -10, 10, 1200, 1, 41);
        fh2_MusZvsRawPos[i]->GetXaxis()->SetTitle(
            "(RIGHT,Wix. side) -->  raw position [ns, 1ps/bin] --> (LEFT,Mes. side) -->");
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
    cMwpc0vsRawPos = new TCanvas("Mwpc0_vs_RawPosAtTcal_Mult1", "Mwpc0-X vs RawPosAtTcal_Mult1", 10, 10, 800, 700);
    fh2_Mwpc0vsRawPos = new TH2F("fh2_Mwpc_vs_RawPos", "Mwpc0-X vs RawPosAtTcal_Mult1", 1400, -7, 7, 258, 0.5, 64.5);
    fh2_Mwpc0vsRawPos->GetXaxis()->SetTitle(
        "(RIGHT,Wix. side) -->  raw position [ns, 10ps/bin] --> (LEFT,Mes. side) -->");
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
    cMusicDTvsRawPos =
        new TCanvas("MUSIC_DT_vs_RawPosAtTcal_Mult1", "Music FT vs RawPosAtTcal_Mult1", 10, 10, 800, 700);
    fh2_MusDTvsRawPos =
        new TH2F("fh2_MusDT_vs_RawPos", "R3B-MUSIC DT vs RawPosAtTcal_Mult1", 1400, -7, 7, 800, -20, 20);
    fh2_MusDTvsRawPos->GetXaxis()->SetTitle(
        "(RIGHT,Wix. side) -->  raw position [ns, 10ps/bin] --> (LEFT,Mes. side) -->");
    fh2_MusDTvsRawPos->GetYaxis()->SetTitle("Drift Time (mm)");
    fh2_MusDTvsRawPos->GetYaxis()->SetTitleOffset(1.1);
    fh2_MusDTvsRawPos->GetXaxis()->CenterTitle(true);
    fh2_MusDTvsRawPos->GetYaxis()->CenterTitle(true);
    fh2_MusDTvsRawPos->GetXaxis()->SetLabelSize(0.045);
    fh2_MusDTvsRawPos->GetXaxis()->SetTitleSize(0.045);
    fh2_MusDTvsRawPos->GetYaxis()->SetLabelSize(0.045);
    fh2_MusDTvsRawPos->GetYaxis()->SetTitleSize(0.045);
    fh2_MusDTvsRawPos->Draw("col");

    // === RAW TOF FROM S2 AT TCAL AND SINGLE TCAL LEVELS === //
    cSciRawTof_FromS2 = new TCanvas*[fNbDetectors];
    fh1_RawTof_FromS2_AtTcalMult1 = new TH1D*[fNbDetectors];
    fh1_RawTof_FromS2_AtTcalMult1_wTref = new TH1D*[fNbDetectors];
    fh1_RawTof_FromS2_AtSingleTcal_wTref = new TH1D*[fNbDetectors];
    cMusicZvsRawTof_FromS2 = new TCanvas*[fNbDetectors];
    fh2_MusZvsRawTof_FromS2 = new TH2F*[fNbDetectors];
    cSciRawTof_FromS8 = new TCanvas*[fNbDetectors];
    fh1_RawTof_FromS8_AtTcalMult1 = new TH1D*[fNbDetectors];
    fh1_RawTof_FromS8_AtTcalMult1_wTref = new TH1D*[fNbDetectors];
    fh1_RawTof_FromS8_AtSingleTcal_wTref = new TH1D*[fNbDetectors];
    cMusicZvsRawTof_FromS8 = new TCanvas*[fNbDetectors];
    fh2_MusZvsRawTof_FromS8 = new TH2F*[fNbDetectors];
    for (Int_t dstop = 0; dstop < fNbDetectors; dstop++)
    {
        if (fIdS2 > 0)
        {
            sprintf(Name1, "RawTof_Sci%02d_to_Sci%02d", fIdS2, dstop + 1);
            sprintf(Name2, "Raw time-of-flight from Sci%02d to Sci%02d", fIdS2, dstop + 1);
            cSciRawTof_FromS2[dstop] = new TCanvas(Name1, Name2, 10, 10, 800, 800);
            cSciRawTof_FromS2[dstop]->Divide(1, 3);
            sprintf(Name1, "RawTofNs_m1_Sci%02d_to_Sci%02d", fIdS2, dstop + 1);
            fh1_RawTof_FromS2_AtTcalMult1[dstop] = new TH1D(Name1, Name1, 100000, -50000, 50000);
            fh1_RawTof_FromS2_AtTcalMult1[dstop]->GetXaxis()->SetTitle("Raw Tof [ns]");
            fh1_RawTof_FromS2_AtTcalMult1[dstop]->GetYaxis()->SetTitle("Counts per bin");
            sprintf(Name1, "RawTofNs_m1_wTref_Sci%02d_to_Sci%02d", fIdS2, dstop + 1);
            fh1_RawTof_FromS2_AtTcalMult1_wTref[dstop] = new TH1D(Name1, Name1, 400000, -2000, 2000);
            fh1_RawTof_FromS2_AtTcalMult1_wTref[dstop]->GetXaxis()->SetTitle("Raw Tof [ns]");
            fh1_RawTof_FromS2_AtTcalMult1_wTref[dstop]->GetYaxis()->SetTitle("Counts per bin");
            sprintf(Name1, "RawTofNs_wTref_Sci%02d_to_Sci%02d", fIdS2, dstop + 1);
            fh1_RawTof_FromS2_AtSingleTcal_wTref[dstop] = new TH1D(Name1, Name1, 400000, -2000, 2000);
            fh1_RawTof_FromS2_AtSingleTcal_wTref[dstop]->GetXaxis()->SetTitle("Raw Tof [ns]");
            fh1_RawTof_FromS2_AtSingleTcal_wTref[dstop]->GetYaxis()->SetTitle("Counts per bin");
            cSciRawTof_FromS2[dstop]->cd(1);
            fh1_RawTof_FromS2_AtTcalMult1[dstop]->Draw("");
            cSciRawTof_FromS2[dstop]->cd(2);
            fh1_RawTof_FromS2_AtTcalMult1_wTref[dstop]->Draw("");
            cSciRawTof_FromS2[dstop]->cd(3);
            fh1_RawTof_FromS2_AtSingleTcal_wTref[dstop]->Draw("");
        }
        if (fIdS8 > 0)
        {
            sprintf(Name1, "RawTof_Sci%02d_to_Sci%02d", fIdS8, dstop + 1);
            sprintf(Name2, "Raw time-of-flight from Sci%02d to Sci%02d", fIdS8, dstop + 1);
            cSciRawTof_FromS8[dstop] = new TCanvas(Name1, Name2, 10, 10, 800, 800);
            cSciRawTof_FromS8[dstop]->Divide(1, 3);
            sprintf(Name1, "RawTofNs_m1_Sci%02d_to_Sci%02d", fIdS8, dstop + 1);
            fh1_RawTof_FromS8_AtTcalMult1[dstop] = new TH1D(Name1, Name1, 100000, -50000, 50000);
            fh1_RawTof_FromS8_AtTcalMult1[dstop]->GetXaxis()->SetTitle("Raw Tof [ns]");
            fh1_RawTof_FromS8_AtTcalMult1[dstop]->GetYaxis()->SetTitle("Counts per bin");
            sprintf(Name1, "RawTofNs_m1_wTref_Sci%02d_to_Sci%02d", fIdS8, dstop + 1);
            fh1_RawTof_FromS8_AtTcalMult1_wTref[dstop] = new TH1D(Name1, Name1, 400000, -2000, 2000);
            fh1_RawTof_FromS8_AtTcalMult1_wTref[dstop]->GetXaxis()->SetTitle("Raw Tof [ns]");
            fh1_RawTof_FromS8_AtTcalMult1_wTref[dstop]->GetYaxis()->SetTitle("Counts per bin");
            sprintf(Name1, "RawTofNs_wTref_Sci%02d_to_Sci%02d", fIdS8, dstop + 1);
            fh1_RawTof_FromS8_AtSingleTcal_wTref[dstop] = new TH1D(Name1, Name1, 400000, -2000, 2000);
            fh1_RawTof_FromS8_AtSingleTcal_wTref[dstop]->GetXaxis()->SetTitle("Raw Tof [ns]");
            fh1_RawTof_FromS8_AtSingleTcal_wTref[dstop]->GetYaxis()->SetTitle("Counts per bin");
            cSciRawTof_FromS8[dstop]->cd(1);
            fh1_RawTof_FromS8_AtTcalMult1[dstop]->Draw("");
            cSciRawTof_FromS8[dstop]->cd(2);
            fh1_RawTof_FromS8_AtTcalMult1_wTref[dstop]->Draw("");
            cSciRawTof_FromS8[dstop]->cd(3);
            fh1_RawTof_FromS8_AtSingleTcal_wTref[dstop]->Draw("");
        }

        // === MUSIC HIT DATA VERSUS SCI-RAW TOF
        if (fIdS2 > 0)
        {
            sprintf(Name1, "MUSIC_Z_vs_RawTof_Sci%02d_to_Sci%02d", fIdS2, dstop + 1);
            cMusicZvsRawTof_FromS2[dstop] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
            sprintf(Name1, "MusZ_vs_RawTof_Sci%02d_to_Sci%02d", fIdS2, dstop + 1);
            fh2_MusZvsRawTof_FromS2[dstop] = new TH2F(Name1, Name1, 1500, 500, 2000, 1200, 1, 41);
            fh2_MusZvsRawTof_FromS2[dstop]->GetXaxis()->SetTitle("Raw-ToF-Cave-C [ns]");
            fh2_MusZvsRawTof_FromS2[dstop]->GetYaxis()->SetTitle("Charge (Z)");
            fh2_MusZvsRawTof_FromS2[dstop]->GetYaxis()->SetTitleOffset(1.1);
            fh2_MusZvsRawTof_FromS2[dstop]->GetXaxis()->CenterTitle(true);
            fh2_MusZvsRawTof_FromS2[dstop]->GetYaxis()->CenterTitle(true);
            fh2_MusZvsRawTof_FromS2[dstop]->GetXaxis()->SetLabelSize(0.045);
            fh2_MusZvsRawTof_FromS2[dstop]->GetXaxis()->SetTitleSize(0.045);
            fh2_MusZvsRawTof_FromS2[dstop]->GetYaxis()->SetLabelSize(0.045);
            fh2_MusZvsRawTof_FromS2[dstop]->GetYaxis()->SetTitleSize(0.045);
            fh2_MusZvsRawTof_FromS2[dstop]->Draw("colz");
        }
        if (fIdS8 > 0)
        {
            sprintf(Name1, "MUSIC_Z_vs_RawTof_Sci%02d_to_Sci%02d", fIdS8, dstop + 1);
            cMusicZvsRawTof_FromS8[dstop] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
            sprintf(Name1, "MusZ_vs_RawTof_Sci%02d_to_Sci%02d", fIdS8, dstop + 1);
            fh2_MusZvsRawTof_FromS8[dstop] = new TH2F(Name1, Name1, 1500, 500, 2000, 1200, 1, 41);
            fh2_MusZvsRawTof_FromS8[dstop]->GetXaxis()->SetTitle("Raw-ToF-Cave-C [ns]");
            fh2_MusZvsRawTof_FromS8[dstop]->GetYaxis()->SetTitle("Charge (Z)");
            fh2_MusZvsRawTof_FromS8[dstop]->GetYaxis()->SetTitleOffset(1.1);
            fh2_MusZvsRawTof_FromS8[dstop]->GetXaxis()->CenterTitle(true);
            fh2_MusZvsRawTof_FromS8[dstop]->GetYaxis()->CenterTitle(true);
            fh2_MusZvsRawTof_FromS8[dstop]->GetXaxis()->SetLabelSize(0.045);
            fh2_MusZvsRawTof_FromS8[dstop]->GetXaxis()->SetTitleSize(0.045);
            fh2_MusZvsRawTof_FromS8[dstop]->GetYaxis()->SetLabelSize(0.045);
            fh2_MusZvsRawTof_FromS8[dstop]->GetYaxis()->SetTitleSize(0.045);
            fh2_MusZvsRawTof_FromS8[dstop]->Draw("colz");
        }
    }

    // === HIT DATA AoverQ VERSUS Q === //
    if (fIdS2 > 0)
    {
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
    }

    // --- --------------- --- //
    // --- MAIN FOLDER-Sci --- //
    // --- --------------- --- //
    TFolder* mainfolSci = new TFolder("SofSci", "SofSci info");
    for (Int_t i = 0; i < fNbDetectors; i++)
    {
        mainfolSci->Add(cSciMult[i]);
        mainfolSci->Add(cSciRawPos[i]);
        if (fIdS2 > 0)
            mainfolSci->Add(cSciRawTof_FromS2[i]);
        if (fIdS8 > 0)
            mainfolSci->Add(cSciRawTof_FromS8[i]);
    }
    mainfolSci->Add(cMusicDTvsRawPos);
    mainfolSci->Add(cMwpc0vsRawPos);
    run->AddObject(mainfolSci);

    TFolder* mainfolID = new TFolder("SofSCI-Music", "SofSCI-Music Incoming info");
    for (UShort_t d = 0; d < fNbDetectors; d++)
    {
        mainfolID->Add(cMusicZvsRawPos[d]);
        if (fIdS2 > 0)
            mainfolID->Add(cMusicZvsRawTof_FromS2[d]);
        if (fIdS8 > 0)
            mainfolID->Add(cMusicZvsRawTof_FromS8[d]);
    }
    if (fIdS2 > 0)
        mainfolID->Add(cAqvsq);
    run->AddObject(mainfolID);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_SOFSCI_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofSciOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofSciOnlineSpectra::Reset_Histo";
    for (Int_t i = 0; i < fNbDetectors; i++)
    {
        // === MULT AND FINE TIME === //
        fh2_mult[i]->Reset();
        for (Int_t j = 0; j < fNbChannels; j++)
        {
            fh1_finetime[i * fNbChannels + j]->Reset();
        }
        // === RAW POSITION === //
        fh1_RawPos_AtTcalMult1[i]->Reset();
        fh1_RawPos_AtSingleTcal[i]->Reset();
        // === R3BMUSIC === //
        fh2_MusZvsRawPos[i]->Reset();

        if (fIdS2 > 0)
        {
            // === RAW TIME_OF_FLIGHT === //
            fh1_RawTof_FromS2_AtTcalMult1[i]->Reset();
            fh1_RawTof_FromS2_AtTcalMult1_wTref[i]->Reset();
            fh1_RawTof_FromS2_AtSingleTcal_wTref[i]->Reset();
            // === R3BMUSIC === //
            fh2_MusZvsRawTof_FromS2[i]->Reset();
        }
        if (fIdS8 > 0)
        {
            // === RAW TIME_OF_FLIGHT === //
            fh1_RawTof_FromS8_AtTcalMult1[i]->Reset();
            fh1_RawTof_FromS8_AtTcalMult1_wTref[i]->Reset();
            fh1_RawTof_FromS8_AtSingleTcal_wTref[i]->Reset();
            // === R3BMUSIC === //
            fh2_MusZvsRawTof_FromS8[i]->Reset();
        }
    }

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
    UShort_t multMapSci[fNbDetectors * fNbChannels];
    // SofSci Tcal data
    Double_t iRawTimeNs[fNbDetectors * fNbChannels];

    // --- -------------- --- //
    // --- initialisation --- //
    // --- -------------- --- //
    for (UShort_t i = 0; i < fNbDetectors; i++)
    {
        for (UShort_t j = 0; j < fNbChannels; j++)
        {
            multMapSci[i * fNbChannels + j] = 0;
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
            multMapSci[iDet * fNbChannels + iCh]++;
            fh1_finetime[iDet * fNbChannels + iCh]->Fill(hitmapped->GetTimeFine());
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
                iRawTimeNs[iDet * fNbChannels + iCh] = hittcal->GetRawTimeNs();
            }
        }

        // --- ------------------------------ --- //
        // --- loop over sci single tcal data --- //
        // --- ------------------------------ --- //
        Double_t xs2 = -10000.;
        Double_t toff = -10000.;
        double Beta_S2_Cave, Gamma_S2_Cave, Brho_S2_Cave;
        double slope_calib = -5.8; // only for the s467, for S2 SofSci
        Int_t d, t;
        if (fSingleTcalItemsSci)
        {
            nHits = fSingleTcalItemsSci->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BSofSciSingleTcalData* hitsingletcal = (R3BSofSciSingleTcalData*)fSingleTcalItemsSci->At(ihit);
                if (!hitsingletcal)
                    continue;
                d = hitsingletcal->GetDetector() - 1;
                fh1_RawPos_AtSingleTcal[d]->Fill(hitsingletcal->GetRawPosNs());
                if (fIdS2 > 0)
                    fh1_RawTof_FromS2_AtSingleTcal_wTref[d]->Fill(hitsingletcal->GetRawTofNs_FromS2());
                if (fIdS8 > 0)
                    fh1_RawTof_FromS8_AtSingleTcal_wTref[d]->Fill(hitsingletcal->GetRawTofNs_FromS8());
                if (d == fIdS2 - 1)
                    xs2 = hitsingletcal->GetRawPosNs() * slope_calib;
                if ((d == fNbDetectors - 1) && (fIdS2 > 0))
                    toff = hitsingletcal->GetRawTofNs_FromS2();
                if (MusicZ > 0)
                {
                    fh2_MusZvsRawPos[d]->Fill(hitsingletcal->GetRawPosNs(), MusicZ);
                    if (fIdS2 > 0)
                        fh2_MusZvsRawTof_FromS2[d]->Fill(hitsingletcal->GetRawTofNs_FromS2(), MusicZ);
                    if (fIdS8 > 0)
                        fh2_MusZvsRawTof_FromS8[d]->Fill(hitsingletcal->GetRawTofNs_FromS8(), MusicZ);
                }
                if (d == fNbDetectors - 1)
                {
                    fh2_MusDTvsRawPos->Fill(hitsingletcal->GetRawPosNs(), MusicDT); // at Cave C
                }
            } // end of loop over the SingleTcalItems

            if (MusicZ > 0 && xs2 != -10000. && toff != -10000.)
            {
                Beta_S2_Cave = 15424.3 / (toff + 675. - 1922.) / 29.9999; // After run 336
                Gamma_S2_Cave = 1. / (TMath::Sqrt(1. - (Beta_S2_Cave) * (Beta_S2_Cave)));
                Brho_S2_Cave = 9.048 * (1 + xs2 / 726.); //+mwpc0x/10./2000);
                fh2_Aqvsq->Fill(Brho_S2_Cave / (3.10716 * Gamma_S2_Cave * Beta_S2_Cave), MusicZ);
            }
        }
        // Get cal data MWPC0
        Double_t mwpc0padx = -1., qmax = -100.;
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
                    mwpc0padx = hit->GetPad();
                    qmax = hit->GetQ();
                }
            }
        }

        // --- ----------------------------------------- --- //
        // --- filling some histogramms outside the loop --- //
        // --- ----------------------------------------- --- //
        Double_t possci = 0.;
        xs2 = -10000.;
        for (UShort_t i = 0; i < fNbDetectors; i++)
        {
            for (UShort_t j = 0; j < fNbChannels; j++)
            {
                fh2_mult[i]->Fill(j + 1, multMapSci[i * fNbChannels + j]);
            }
            if ((multMapSci[i * fNbChannels] == 1) && (multMapSci[i * fNbChannels + 1] == 1))
            {
                // TrawRIGHT-TrawLEFT = 5*(CCr-CCl)+(FTl-FTr) : x is increasing from RIGHT to LEFT
                possci = iRawTimeNs[i * fNbChannels] - iRawTimeNs[i * fNbChannels + 1];
                fh1_RawPos_AtTcalMult1[i]->Fill(possci);
                if (i == 1)
                    xs2 = possci;

                if (mwpc0padx > 0 && possci > -10. && possci < 10. && i == 3)
                {
                    fh2_Mwpc0vsRawPos->Fill(possci, mwpc0padx);
                }
            }
        }

        Double_t iRawTof_wTref;
        Double_t iRawTof;
        for (UShort_t dstop = 0; dstop < fNbDetectors; dstop++)
        {
            if (fIdS2 > 0)
            {
                if ((multMapSci[(fIdS2 - 1) * fNbChannels] == 1) && (multMapSci[(fIdS2 - 1) * fNbChannels + 1] == 1) &&
                    (multMapSci[dstop * fNbChannels] == 1) && (multMapSci[dstop * fNbChannels + 1] == 1))
                {
                    iRawTof = 0.5 * (iRawTimeNs[dstop * fNbChannels] + iRawTimeNs[dstop * fNbChannels + 1]) -
                              0.5 * (iRawTimeNs[(fIdS2 - 1) * fNbChannels] + iRawTimeNs[(fIdS2 - 1) * fNbChannels + 1]);
                    fh1_RawTof_FromS2_AtTcalMult1[dstop]->Fill(iRawTof);

                    if (multMapSci[(fIdS2 - 1) * fNbChannels + 2] == 1 && multMapSci[dstop * fNbChannels + 2] == 1)
                    {
                        iRawTof_wTref =
                            iRawTof + iRawTimeNs[(fIdS2 - 1) * fNbChannels + 2] - iRawTimeNs[dstop * fNbChannels + 2];
                        fh1_RawTof_FromS2_AtTcalMult1_wTref[dstop]->Fill(iRawTof_wTref);
                    } // end of if mult==1 also for the Tref
                }     // end of if mult==1 in the left and right pmts
            }         // end of if(SofSci at S2)

            if (fIdS8 > 0)
            {
                if ((multMapSci[(fIdS8 - 1) * fNbChannels] == 1) && (multMapSci[(fIdS8 - 1) * fNbChannels + 1] == 1) &&
                    (multMapSci[dstop * fNbChannels] == 1) && (multMapSci[dstop * fNbChannels + 1] == 1))
                {
                    iRawTof = 0.5 * (iRawTimeNs[dstop * fNbChannels] + iRawTimeNs[dstop * fNbChannels + 1]) -
                              0.5 * (iRawTimeNs[(fIdS8 - 1) * fNbChannels] + iRawTimeNs[(fIdS8 - 1) * fNbChannels + 1]);
                    fh1_RawTof_FromS8_AtTcalMult1[dstop]->Fill(iRawTof);

                    if (multMapSci[(fIdS8 - 1) * fNbChannels + 2] == 1 && multMapSci[dstop * fNbChannels + 2] == 1)
                    {
                        iRawTof_wTref =
                            iRawTof + iRawTimeNs[(fIdS8 - 1) * fNbChannels + 2] - iRawTimeNs[dstop * fNbChannels + 2];
                        fh1_RawTof_FromS8_AtTcalMult1_wTref[dstop]->Fill(iRawTof_wTref);
                    } // end of if mult==1 also for the Tref
                }     // end of if mult==1 in the left and right pmts
            }         // end of if(SofSci at S8)

        } // end of for (dstop)
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
        for (UShort_t i = 0; i < fNbDetectors; i++)
        {
            fh2_mult[i]->Write();
            cSciMult[i]->Write();
            for (UShort_t j = 0; j < fNbChannels; j++)
            {
                fh1_finetime[i * fNbChannels + j]->Write();
            }
        }
    }

    if (fTcalItemsSci)
    {
        for (UShort_t i = 0; i < fNbDetectors; i++)
        {
            fh1_RawPos_AtTcalMult1[i]->Write();
            cSciRawPos[i]->Write();
            if (fIdS2 > 0)
            {
                fh1_RawTof_FromS2_AtTcalMult1[i]->Write();
                fh1_RawTof_FromS2_AtTcalMult1_wTref[i]->Write();
                cSciRawTof_FromS2[i]->Write();
            }
            if (fIdS8 > 0)
            {
                fh1_RawTof_FromS8_AtTcalMult1[i]->Write();
                fh1_RawTof_FromS8_AtTcalMult1_wTref[i]->Write();
                cSciRawTof_FromS8[i]->Write();
            }
        }
        if (fMusHitItems)
        {
            for (UShort_t d = 0; d < fNbDetectors; d++)
                fh2_MusZvsRawPos[d]->Write();
        }
        if (fMusCalItems)
        {
            for (UShort_t d = 0; d < fNbDetectors; d++)
            {
                if (fIdS2 > 0)
                    fh2_MusZvsRawTof_FromS2[d]->Write();
                if (fIdS8 > 0)
                    fh2_MusZvsRawTof_FromS8[d]->Write();
            }
            fh2_MusDTvsRawPos->Write();
        }
        if (fCalItemsMwpc0)
            fh2_Mwpc0vsRawPos->Write();
    }
}

ClassImp(R3BSofSciOnlineSpectra)
