// ------------------------------------------------------------
// -----                  R3BSofSciOnlineSpectra           -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

/*
 * This task should fill histograms with SOFIA online data
 */

#include "R3BSofSciOnlineSpectra.h"
#include "R3BEventHeader.h"
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

#define DeltaClockTrefRight_S2 0
#define DeltaClockTrefLeft_S2 0
#define DeltaClockTrefRight_CaveC 50
#define DeltaClockTrefLeft_CaveC 47

R3BSofSciOnlineSpectra::R3BSofSciOnlineSpectra()
    : FairTask("SofSciOnlineSpectra", 1)
    , fEventHeader(nullptr)
    , fMapped(NULL)
    , fTcal(NULL)
    , fSingleTcal(NULL)
    , fCal(NULL)
    , fNEvents(0)
    , fNbDetectors(2)
    , fNbChannels(3)
    , fIdS2(1)
    , fIdS8(0)
{
    fCalTofS2min = new TArrayF(4);
    fCalTofS2max = new TArrayF(4);
    fCalTofS8min = new TArrayF(2);
    fCalTofS8max = new TArrayF(2);
}

R3BSofSciOnlineSpectra::R3BSofSciOnlineSpectra(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fEventHeader(nullptr)
    , fMapped(NULL)
    , fTcal(NULL)
    , fSingleTcal(NULL)
    , fCal(NULL)
    , fNEvents(0)
    , fNbDetectors(2)
    , fNbChannels(3)
    , fIdS2(1)
    , fIdS8(0)
{
    fCalTofS2min = new TArrayF(4);
    fCalTofS2max = new TArrayF(4);
    fCalTofS8min = new TArrayF(2);
    fCalTofS8max = new TArrayF(2);
}

R3BSofSciOnlineSpectra::~R3BSofSciOnlineSpectra()
{
    LOG(INFO) << "R3BSofSciOnlineSpectra::Delete instance";
    if (fEventHeader)
        delete fEventHeader;
    if (fMapped)
        delete fMapped;
    if (fTcal)
        delete fTcal;
    if (fSingleTcal)
        delete fSingleTcal;
    if (fCal)
        delete fCal;
}

InitStatus R3BSofSciOnlineSpectra::Init()
{

    LOG(INFO) << "R3BSofSciOnlineSpectra::Init()";

    // try to get a handle on the EventHeader. EventHeader may not be
    // present though and hence may be null. Take care when using.

    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofSciOnlineSpectra::Init FairRootManager not found";

    fEventHeader = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");
    if (!fEventHeader)
        fEventHeader = (R3BEventHeader*)mgr->GetObject("EventHeader.");

    FairRunOnline* run = FairRunOnline::Instance();
    run->GetHttpServer()->Register("", this);

    // --- ------------------------- --- //
    // --- get access to mapped data --- //
    // --- ------------------------- --- //
    fMapped = (TClonesArray*)mgr->GetObject("SofSciMappedData");
    if (!fMapped)
    {
        return kFATAL;
    }

    // --- ------------------------ --- //
    // --- get access to tcal data  --- //
    // --- ------------------------ --- //
    fTcal = (TClonesArray*)mgr->GetObject("SofSciTcalData");
    if (!fTcal)
    {
        return kFATAL;
    }

    // --- ------------------------------ --- //
    // --- get access to single tcal data --- //
    // --- ------------------------------ --- //
    fSingleTcal = (TClonesArray*)mgr->GetObject("SofSciSingleTcalData");
    if (!fSingleTcal)
    {
        return kFATAL;
    }

    // --- ---------------------- --- //
    // --- get access to cal data --- //
    // --- ---------------------- --- //
    fCal = (TClonesArray*)mgr->GetObject("SofSciCalData");
    if (!fSingleTcal)
    {
        return kFATAL;
    }

    // --- ------------------------------ --- //
    // --- declare TCanvas and Histograms --- //
    // --- ------------------------------ --- //

    // === 1D-MULTIPLICITY without condition on TPAT === //
    fh1_multMap = new TH1I*[fNbDetectors * fNbChannels];
    fh1_multTcal = new TH1I*[fNbDetectors * fNbChannels];
    fh1_multSingleTcal = new TH1I*[fNbDetectors];
    fh1_multCal = new TH1I*[fNbDetectors];

    // === 1D-MULTIPLICITY with condition on TPAT === //
    fh1_multMap_condTpat = new TH1I*[fNbDetectors * fNbChannels];
    fh1_multTcal_condTpat = new TH1I*[fNbDetectors * fNbChannels];
    fh1_multSingleTcal_condTpat = new TH1I*[fNbDetectors];
    // fh1_multCal_condTpat = new TH1I*[fNbDetectors];

    // === MAPPED LEVEL: FINE TIME AND MULT PER EVENT === //
    cMapped = new TCanvas*[fNbDetectors];
    fh1_finetime = new TH1I*[fNbDetectors * fNbChannels];
    fh2_mult = new TH2I*[fNbDetectors];
    fh2_mult_RvsL = new TH2I*[fNbDetectors];
    fh2_mult_RvsL_condTpat = new TH2I*[fNbDetectors];

    // === POSITION AT TCAL, SINGLE TCAL AND CAL LEVELS === //
    cPos = new TCanvas*[fNbDetectors];
    cRawPosVsCalPos = new TCanvas*[fNbDetectors];
    fh1_RawPos_TcalMult1 = new TH1F*[fNbDetectors];
    fh1_RawPos_SingleTcal = new TH1F*[fNbDetectors];
    fh1_CalPos = new TH1F*[fNbDetectors];
    fh2_RawPosVsCalPos = new TH2F*[fNbDetectors];

    if (fIdS2 > 0)
    {
        // === TIME-OF-FLIGHT FROM S2 AT TCAL, SINGLE TCAL AND CAL LEVELS === //
        cTofFromS2 = new TCanvas*[fNbDetectors - fIdS2];
        fh1_RawTofFromS2_TcalMult1 = new TH1D*[fNbDetectors - fIdS2];
        fh1_RawTofFromS2_SingleTcal = new TH1D*[fNbDetectors - fIdS2];
        fh1_CalTofFromS2 = new TH1D*[fNbDetectors - fIdS2];

        // === BETA CALCULATED FROM ToF S2 CAL DATA === //
        cBetaFromS2 = new TCanvas*[fNbDetectors - fIdS2];
        fh1_BetaFromS2 = new TH1D*[fNbDetectors - fIdS2];

        // === POS vs ToF FROM S2 at CAL LEVEL === //
        fh2_PosVsTofS2 = new TH2D*[2 * (fNbDetectors - fIdS2)];
    }

    if (fIdS8 > 0)
    {
        // === TIME-OF-FLIGHT FROM S8 AT TCAL, SINGLE TCAL AND CAL LEVELS === //
        cTofFromS8 = new TCanvas*[fNbDetectors - fIdS8];
        fh1_RawTofFromS8_TcalMult1 = new TH1D*[fNbDetectors - fIdS8];
        fh1_RawTofFromS8_SingleTcal = new TH1D*[fNbDetectors - fIdS8];
        fh1_CalTofFromS8 = new TH1D*[fNbDetectors - fIdS8];

        // === BETA CALCULATED FROM ToF S8 CAL DATA === //
        cBetaFromS8 = new TCanvas*[fNbDetectors - fIdS8];
        fh1_BetaFromS8 = new TH1D*[fNbDetectors - fIdS8];

        // === POS vs ToF FROM S8 at CAL LEVEL === //
        fh2_PosVsTofS8 = new TH2D*[2 * (fNbDetectors - fIdS8)];
    }

    // === 2D multiplicity at Mapped level: Tref vs PMT === //
    fh2_mult_TrefVsPmt = new TH2I*[fNbDetectors * (fNbChannels - 1)];
    fh2_mult_TrefVsPmt_condTpat = new TH2I*[fNbDetectors * (fNbChannels - 1)];

    // --- ----------------------------- --- //
    // --- create TCanvas and Histograms --- //
    // --- ------------------------------ --- //
    char Name1[255];
    char Name2[255];

    // === TCanvas: 1D-MultMap === //
    sprintf(Name1, "MultMap");
    cMultMap = new TCanvas(Name1, Name1, 10, 10, 800, 700);
    cMultMap->Divide(fNbChannels, fNbDetectors);

    // === TCanvas: 1D-MultTcal === //
    sprintf(Name1, "MultTcal");
    cMultTcal = new TCanvas(Name1, Name1, 10, 10, 800, 700);
    cMultTcal->Divide(fNbChannels, fNbDetectors);

    // === TCanvas: 1D-MultSingleTcal === //
    sprintf(Name1, "MultSingleTcal");
    cMultSingleTcal = new TCanvas(Name1, Name1, 10, 10, 800, 700);
    cMultSingleTcal->Divide(1, fNbDetectors);

    // === TCanvas: 1D-MultCal === //
    sprintf(Name1, "MultCal");
    cMultCal = new TCanvas(Name1, Name1, 10, 10, 800, 700);
    cMultCal->Divide(1, fNbDetectors);

    // === TCanvas: 2D-MultMap === //
    sprintf(Name1, "MultMap2D");
    cMultMap2D = new TCanvas(Name1, Name1, 10, 10, 800, 700);
    cMultMap2D->Divide(4, fNbDetectors);

    // === TCanvas: 2D-MultMap === //
    sprintf(Name1, "MultMap2D_RvsL");
    cMultMap2D_RvsL = new TCanvas(Name1, Name1, 10, 10, 800, 700);
    cMultMap2D_RvsL->Divide(2, fNbDetectors);

    // === TCanvas: Delta Clock Per Sci === //
    sprintf(Name1, "DeltaClockPerSci");
    cDeltaClockPerSci = new TCanvas(Name1, Name1, 10, 10, 800, 700);
    cDeltaClockPerSci->Divide(2, fNbDetectors);
    fh1_deltaClockPerSci = new TH1F*[fNbDetectors * 2];
    fh1_deltaClockPerSci_condTpat = new TH1F*[fNbDetectors * 2];
    for (int d = 0; d < fNbDetectors; d++)
    {
        // with condition on TPAT
        sprintf(Name1, "DeltaClock_Sci%i_TrefRight", d + 1);
        sprintf(Name2, "DeltaClock_Sci%i_TrefRight (blue no condition on TPAT and red TPAT = 1 or 2)", d + 1);
        fh1_deltaClockPerSci[2 * d] = new TH1F(Name1, Name2, 3000, -1500.5, 1499.5);
        fh1_deltaClockPerSci[2 * d]->SetLineColor(kBlue);
        fh1_deltaClockPerSci[2 * d]->SetLineWidth(2);
        cDeltaClockPerSci->cd(2 * d + 1);
        fh1_deltaClockPerSci[2 * d]->Draw();

        sprintf(Name1, "DeltaClock_Sci%i_TrefLeft", d + 1);
        sprintf(Name2, "DeltaClock_Sci%i_TrefLeft (blue no condition on Tpat and ref TPAT = 1 or 2)", d + 1);
        fh1_deltaClockPerSci[2 * d + 1] = new TH1F(Name1, Name2, 3000, -1500.5, 1499.5);
        fh1_deltaClockPerSci[2 * d + 1]->SetLineColor(kBlue);
        fh1_deltaClockPerSci[2 * d + 1]->SetLineWidth(2);
        cDeltaClockPerSci->cd(2 * d + 2);
        fh1_deltaClockPerSci[2 * d + 1]->Draw();

        // with condition on TPAT
        sprintf(Name1, "DeltaClock_Sci%i_TrefRight_wCondTpat", d + 1);
        fh1_deltaClockPerSci_condTpat[2 * d] = new TH1F(Name1, Name1, 3000, -1500.5, 1499.5);
        fh1_deltaClockPerSci_condTpat[2 * d]->SetLineColor(kRed);
        fh1_deltaClockPerSci_condTpat[2 * d]->SetLineWidth(1);
        cDeltaClockPerSci->cd(2 * d + 1);
        fh1_deltaClockPerSci_condTpat[2 * d]->Draw("sames");

        sprintf(Name1, "DeltaClock_Sci%i_TrefLeft_wCondTpat", d + 1);
        fh1_deltaClockPerSci_condTpat[2 * d + 1] = new TH1F(Name1, Name1, 3000, -1500.5, 1499.5);
        fh1_deltaClockPerSci_condTpat[2 * d + 1]->SetLineColor(kRed);
        fh1_deltaClockPerSci_condTpat[2 * d + 1]->SetLineWidth(1);
        cDeltaClockPerSci->cd(2 * d + 2);
        fh1_deltaClockPerSci_condTpat[2 * d + 1]->Draw("sames");
    }

    if (fNbDetectors > 1)
    {
        // === Delta Tref at TCAL === //
        cDeltaTref = new TCanvas("DeltaTref", "DeltaTref", 10, 10, 800, 700);
        cDeltaTref->Divide(1, fNbDetectors - 1);
        fh1_DeltaTref = new TH1D*[fNbDetectors - 1];
        for (int d = 0; d < fNbDetectors - 1; d++)
        {
            sprintf(Name1, "DeltaTref_Sci%02d_to_SciCaveC", d + 1);
            fh1_DeltaTref[d] = new TH1D(Name1, Name1, 45000, -20000, 25000);
            cDeltaTref->cd(d + 1);
            fh1_DeltaTref[d]->Draw();
        }
    }

    for (Int_t i = 0; i < fNbDetectors; i++)
    {
        // === TH1I: 1D-mult at SingleTcal level === //
        sprintf(Name1, "Sci%i_MultPerEvent_SingleTcal", i + 1);
        sprintf(Name2, "Sci%i_MultPerEvent_SingleTcal (blue no Tpat condition, red: Tpat=1 or 2)", i + 1);
        fh1_multSingleTcal[i] = new TH1I(Name1, Name2, 20, -0.5, 19.5);
        fh1_multSingleTcal[i]->GetXaxis()->SetTitle("Multiplicity per event");
        fh1_multSingleTcal[i]->GetYaxis()->SetTitle("Counts");
        fh1_multSingleTcal[i]->GetXaxis()->CenterTitle(true);
        fh1_multSingleTcal[i]->GetYaxis()->CenterTitle(true);
        fh1_multSingleTcal[i]->GetXaxis()->SetLabelSize(0.05);
        fh1_multSingleTcal[i]->GetXaxis()->SetTitleSize(0.05);
        fh1_multSingleTcal[i]->GetYaxis()->SetLabelSize(0.05);
        fh1_multSingleTcal[i]->GetYaxis()->SetTitleSize(0.05);
        fh1_multSingleTcal[i]->SetLineColor(kBlue);
        fh1_multSingleTcal[i]->SetLineWidth(2);
        cMultSingleTcal->cd(i + 1);
        gPad->SetLogy();
        fh1_multSingleTcal[i]->Draw("");

        sprintf(Name1, "Sci%i_MultPerEvent_SingleTcal_condTpat", i + 1);
        sprintf(
            Name2, "Sci%i_MultPerEvent_SingleTcal_condTpat (blue no condition on Tpat, red for Tpat=1 or 2)", i + 1);
        fh1_multSingleTcal_condTpat[i] = new TH1I(Name1, Name2, 20, -0.5, 19.5);
        fh1_multSingleTcal_condTpat[i]->GetXaxis()->SetTitle("Multiplicity per event");
        fh1_multSingleTcal_condTpat[i]->GetYaxis()->SetTitle("Counts");
        fh1_multSingleTcal_condTpat[i]->GetXaxis()->CenterTitle(true);
        fh1_multSingleTcal_condTpat[i]->GetYaxis()->CenterTitle(true);
        fh1_multSingleTcal_condTpat[i]->GetXaxis()->SetLabelSize(0.05);
        fh1_multSingleTcal_condTpat[i]->GetXaxis()->SetTitleSize(0.05);
        fh1_multSingleTcal_condTpat[i]->GetYaxis()->SetLabelSize(0.05);
        fh1_multSingleTcal_condTpat[i]->GetYaxis()->SetTitleSize(0.05);
        fh1_multSingleTcal_condTpat[i]->SetLineColor(kRed);
        fh1_multSingleTcal_condTpat[i]->SetLineWidth(1);
        cMultSingleTcal->cd(i + 1);
        gPad->SetLogy();
        fh1_multSingleTcal_condTpat[i]->Draw("sames");

        // === TH1I: 1D-mult at cal level === //
        sprintf(Name1, "Sci%i_MultPerEvent_Cal", i + 1);
        fh1_multCal[i] = new TH1I(Name1, Name1, 20, -0.5, 19.5);
        fh1_multCal[i]->GetXaxis()->SetTitle("Multiplicity per event");
        fh1_multCal[i]->GetYaxis()->SetTitle("Counts");
        fh1_multCal[i]->GetXaxis()->CenterTitle(true);
        fh1_multCal[i]->GetYaxis()->CenterTitle(true);
        fh1_multCal[i]->GetXaxis()->SetLabelSize(0.05);
        fh1_multCal[i]->GetXaxis()->SetTitleSize(0.05);
        fh1_multCal[i]->GetYaxis()->SetLabelSize(0.05);
        fh1_multCal[i]->GetYaxis()->SetTitleSize(0.05);
        cMultCal->cd(i + 1);
        gPad->SetLogy();
        fh1_multCal[i]->Draw("");

        // === TCanvas: mapped level === //
        sprintf(Name1, "Sci%i_MultAndFt", i + 1);
        cMapped[i] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
        cMapped[i]->Divide(2, 2);

        // === TCanvas: position === //
        sprintf(Name1, "Sci%i_Pos", i + 1);
        cPos[i] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
        cPos[i]->Divide(1, 3);

        // === TCanvas: PosRaw vs PosCal === //
        sprintf(Name1, "Sci%i_RawPosVsCalPos", i + 1);
        cRawPosVsCalPos[i] = new TCanvas(Name1, Name1, 10, 10, 800, 700);

        for (Int_t j = 0; j < fNbChannels; j++)
        {
            // === TH1I: 1D-mult at map level === //
            sprintf(Name1, "Sci%i_Pmt%i_MultPerEvent_Mapped", i + 1, j + 1);
            sprintf(Name2,
                    "Sci%i_Pmt%i_MultPerEvent_Mapped (blue no condition on TPAT, red condition on TPAT = 1 or 2)",
                    i + 1,
                    j + 1);
            fh1_multMap[i * fNbChannels + j] = new TH1I(Name1, Name1, 70, -0.5, 69.5);
            fh1_multMap[i * fNbChannels + j]->GetXaxis()->SetTitle("Multiplicity per event");
            fh1_multMap[i * fNbChannels + j]->GetYaxis()->SetTitle("Counts");
            fh1_multMap[i * fNbChannels + j]->GetXaxis()->CenterTitle(true);
            fh1_multMap[i * fNbChannels + j]->GetYaxis()->CenterTitle(true);
            fh1_multMap[i * fNbChannels + j]->GetXaxis()->SetLabelSize(0.05);
            fh1_multMap[i * fNbChannels + j]->GetXaxis()->SetTitleSize(0.05);
            fh1_multMap[i * fNbChannels + j]->GetYaxis()->SetLabelSize(0.05);
            fh1_multMap[i * fNbChannels + j]->GetYaxis()->SetTitleSize(0.05);
            fh1_multMap[i * fNbChannels + j]->SetLineWidth(2);
            fh1_multMap[i * fNbChannels + j]->SetLineColor(kBlue);
            cMultMap->cd(i * fNbChannels + j + 1);
            gPad->SetLogy();
            fh1_multMap[i * fNbChannels + j]->Draw("");

            sprintf(Name1, "Sci%i_Pmt%i_MultPerEvent_Mapped_condTpat", i + 1, j + 1);
            fh1_multMap_condTpat[i * fNbChannels + j] = new TH1I(Name1, Name1, 70, -0.5, 69.5);
            fh1_multMap_condTpat[i * fNbChannels + j]->GetXaxis()->SetTitle("Multiplicity per event");
            fh1_multMap_condTpat[i * fNbChannels + j]->GetYaxis()->SetTitle("Counts");
            fh1_multMap_condTpat[i * fNbChannels + j]->GetXaxis()->CenterTitle(true);
            fh1_multMap_condTpat[i * fNbChannels + j]->GetYaxis()->CenterTitle(true);
            fh1_multMap_condTpat[i * fNbChannels + j]->GetXaxis()->SetLabelSize(0.05);
            fh1_multMap_condTpat[i * fNbChannels + j]->GetXaxis()->SetTitleSize(0.05);
            fh1_multMap_condTpat[i * fNbChannels + j]->GetYaxis()->SetLabelSize(0.05);
            fh1_multMap_condTpat[i * fNbChannels + j]->GetYaxis()->SetTitleSize(0.05);
            fh1_multMap_condTpat[i * fNbChannels + j]->SetLineWidth(1);
            fh1_multMap_condTpat[i * fNbChannels + j]->SetLineColor(kRed);
            cMultMap->cd(i * fNbChannels + j + 1);
            gPad->SetLogy();
            fh1_multMap_condTpat[i * fNbChannels + j]->Draw("sames");

            // === TH1I: 1D-mult at tcal level === //
            sprintf(Name1, "Sci%i_Pmt%i_MultPerEvent_Tcal", i + 1, j + 1);
            sprintf(Name2,
                    "Sci%i_Pmt%i_MultPerEvent_Tcal (blue no condition on Tpat, red condition on TPAT = 1 or 2)",
                    i + 1,
                    j + 1);
            fh1_multTcal[i * fNbChannels + j] = new TH1I(Name1, Name1, 70, -0.5, 69.5);
            fh1_multTcal[i * fNbChannels + j]->GetXaxis()->SetTitle("Multiplicity per event");
            fh1_multTcal[i * fNbChannels + j]->GetYaxis()->SetTitle("Counts");
            fh1_multTcal[i * fNbChannels + j]->GetXaxis()->CenterTitle(true);
            fh1_multTcal[i * fNbChannels + j]->GetYaxis()->CenterTitle(true);
            fh1_multTcal[i * fNbChannels + j]->GetXaxis()->SetLabelSize(0.05);
            fh1_multTcal[i * fNbChannels + j]->GetXaxis()->SetTitleSize(0.05);
            fh1_multTcal[i * fNbChannels + j]->GetYaxis()->SetLabelSize(0.05);
            fh1_multTcal[i * fNbChannels + j]->GetYaxis()->SetTitleSize(0.05);
            fh1_multTcal[i * fNbChannels + j]->SetLineWidth(2);
            fh1_multTcal[i * fNbChannels + j]->SetLineColor(kBlue);
            cMultTcal->cd(i * fNbChannels + j + 1);
            gPad->SetLogy();
            fh1_multTcal[i * fNbChannels + j]->Draw("");

            sprintf(Name1, "Sci%i_Pmt%i_MultPerEvent_Tcal_condTpat", i + 1, j + 1);
            fh1_multTcal_condTpat[i * fNbChannels + j] = new TH1I(Name1, Name1, 70, -0.5, 69.5);
            fh1_multTcal_condTpat[i * fNbChannels + j]->GetXaxis()->SetTitle("Multiplicity per event");
            fh1_multTcal_condTpat[i * fNbChannels + j]->GetYaxis()->SetTitle("Counts");
            fh1_multTcal_condTpat[i * fNbChannels + j]->GetXaxis()->CenterTitle(true);
            fh1_multTcal_condTpat[i * fNbChannels + j]->GetYaxis()->CenterTitle(true);
            fh1_multTcal_condTpat[i * fNbChannels + j]->GetXaxis()->SetLabelSize(0.05);
            fh1_multTcal_condTpat[i * fNbChannels + j]->GetXaxis()->SetTitleSize(0.05);
            fh1_multTcal_condTpat[i * fNbChannels + j]->GetYaxis()->SetLabelSize(0.05);
            fh1_multTcal_condTpat[i * fNbChannels + j]->GetYaxis()->SetTitleSize(0.05);
            fh1_multTcal_condTpat[i * fNbChannels + j]->SetLineWidth(1);
            fh1_multTcal_condTpat[i * fNbChannels + j]->SetLineColor(kRed);
            cMultTcal->cd(i * fNbChannels + j + 1);
            gPad->SetLogy();
            fh1_multTcal_condTpat[i * fNbChannels + j]->Draw("sames");

            // === TH1F: fine time === //
            sprintf(Name1, "Sci%i_FineTimeCh%i", i + 1, j + 1);
            fh1_finetime[i * fNbChannels + j] = new TH1I(Name1, Name1, 1000, 0, 1000);
            fh1_finetime[i * fNbChannels + j]->GetXaxis()->SetTitle("Fine time");
            fh1_finetime[i * fNbChannels + j]->GetYaxis()->SetTitle("Counts per bin");
            fh1_finetime[i * fNbChannels + j]->GetXaxis()->CenterTitle(true);
            fh1_finetime[i * fNbChannels + j]->GetYaxis()->CenterTitle(true);
            fh1_finetime[i * fNbChannels + j]->GetXaxis()->SetLabelSize(0.05);
            fh1_finetime[i * fNbChannels + j]->GetXaxis()->SetTitleSize(0.05);
            fh1_finetime[i * fNbChannels + j]->GetYaxis()->SetLabelSize(0.05);
            fh1_finetime[i * fNbChannels + j]->GetYaxis()->SetTitleSize(0.05);
            cMapped[i]->cd(j + 1);
            fh1_finetime[i * fNbChannels + j]->Draw("");
        }

        for (Int_t pmt = 0; pmt < fNbChannels - 1; pmt++)
        {
            sprintf(Name1, "SofSci%i_MultMap_TrefVsPmt%i", i + 1, pmt + 1);
            fh2_mult_TrefVsPmt[i * (fNbChannels - 1) + pmt] = new TH2I(Name1, Name1, 70, -0.5, 69.5, 5, -0.5, 4.5);
            fh2_mult_TrefVsPmt[i * (fNbChannels - 1) + pmt]->GetXaxis()->SetTitle("Mult Pmt");
            fh2_mult_TrefVsPmt[i * (fNbChannels - 1) + pmt]->GetYaxis()->SetTitle("Mult Tref");
            fh2_mult_TrefVsPmt[i * (fNbChannels - 1) + pmt]->GetXaxis()->CenterTitle(true);
            fh2_mult_TrefVsPmt[i * (fNbChannels - 1) + pmt]->GetYaxis()->CenterTitle(true);
            fh2_mult_TrefVsPmt[i * (fNbChannels - 1) + pmt]->GetXaxis()->SetLabelSize(0.05);
            fh2_mult_TrefVsPmt[i * (fNbChannels - 1) + pmt]->GetXaxis()->SetTitleSize(0.05);
            fh2_mult_TrefVsPmt[i * (fNbChannels - 1) + pmt]->GetYaxis()->SetLabelSize(0.05);
            fh2_mult_TrefVsPmt[i * (fNbChannels - 1) + pmt]->GetYaxis()->SetTitleSize(0.05);
            cMultMap2D->cd(i * (fNbChannels - 1) + pmt + 1);
            fh2_mult_TrefVsPmt[i * (fNbChannels - 1) + pmt]->Draw("col");

            sprintf(Name1, "SofSci%i_MultMap_TrefVsPmt%ii_condTpat", i + 1, pmt + 1);
            sprintf(Name2, "SofSci%i_MultMap_TrefVsPmt%i for Tpat = 1 or 2", i + 1, pmt + 1);
            fh2_mult_TrefVsPmt_condTpat[i * (fNbChannels - 1) + pmt] =
                new TH2I(Name1, Name2, 70, -0.5, 69.5, 5, -0.5, 4.5);
            fh2_mult_TrefVsPmt_condTpat[i * (fNbChannels - 1) + pmt]->GetXaxis()->SetTitle("Mult Pmt");
            fh2_mult_TrefVsPmt_condTpat[i * (fNbChannels - 1) + pmt]->GetYaxis()->SetTitle("Mult Tref");
            fh2_mult_TrefVsPmt_condTpat[i * (fNbChannels - 1) + pmt]->GetXaxis()->CenterTitle(true);
            fh2_mult_TrefVsPmt_condTpat[i * (fNbChannels - 1) + pmt]->GetYaxis()->CenterTitle(true);
            fh2_mult_TrefVsPmt_condTpat[i * (fNbChannels - 1) + pmt]->GetXaxis()->SetLabelSize(0.05);
            fh2_mult_TrefVsPmt_condTpat[i * (fNbChannels - 1) + pmt]->GetXaxis()->SetTitleSize(0.05);
            fh2_mult_TrefVsPmt_condTpat[i * (fNbChannels - 1) + pmt]->GetYaxis()->SetLabelSize(0.05);
            fh2_mult_TrefVsPmt_condTpat[i * (fNbChannels - 1) + pmt]->GetYaxis()->SetTitleSize(0.05);
            cMultMap2D->cd(fNbChannels + i * (fNbChannels - 1) + pmt + 2);
            fh2_mult_TrefVsPmt_condTpat[i * (fNbChannels - 1) + pmt]->Draw("col");
        }

        // === TH1F: multiplicity per event and channel at mapped level === //
        sprintf(Name1, "Sci%i_MultPerEvent", i + 1);
        fh2_mult[i] = new TH2I(Name1, Name1, fNbChannels, 0.5, fNbChannels + 0.5, 20, -0.5, 19.5);
        fh2_mult[i]->GetXaxis()->SetTitle("channel: 1=PMT R,    2=PMT L,    3=COMMON REF");
        fh2_mult[i]->GetYaxis()->SetTitle("multiplicity per event");
        fh2_mult[i]->GetXaxis()->CenterTitle(true);
        fh2_mult[i]->GetYaxis()->CenterTitle(true);
        fh2_mult[i]->GetXaxis()->SetLabelSize(0.05);
        fh2_mult[i]->GetXaxis()->SetTitleSize(0.05);
        fh2_mult[i]->GetYaxis()->SetLabelSize(0.05);
        fh2_mult[i]->GetYaxis()->SetTitleSize(0.05);
        cMapped[i]->cd(4);
        fh2_mult[i]->Draw("COL");

        // === TH1F: multiplicity per event and channel at mapped level === //
        sprintf(Name1, "Sci%i_MultPerEvent_RvsL", i + 1);
        fh2_mult_RvsL[i] = new TH2I(Name1, Name1, 40, -1.5, 38.5, 40, -1.5, 38.5);
        fh2_mult_RvsL[i]->GetXaxis()->SetTitle("Multiplicity per event on the Left Pmt");
        fh2_mult_RvsL[i]->GetYaxis()->SetTitle("Multiplicity per event on the Right Pmt");
        fh2_mult_RvsL[i]->GetXaxis()->CenterTitle(true);
        fh2_mult_RvsL[i]->GetYaxis()->CenterTitle(true);
        fh2_mult_RvsL[i]->GetXaxis()->SetLabelSize(0.05);
        fh2_mult_RvsL[i]->GetXaxis()->SetTitleSize(0.05);
        fh2_mult_RvsL[i]->GetYaxis()->SetLabelSize(0.05);
        fh2_mult_RvsL[i]->GetYaxis()->SetTitleSize(0.05);
        cMultMap2D_RvsL->cd(i + 1);
        fh2_mult_RvsL[i]->Draw("COL");

        // === TH1F: multiplicity per event and channel at mapped level === //
        sprintf(Name1, "Sci%i_MultPerEvent_RvsL_condTpat", i + 1);
        fh2_mult_RvsL_condTpat[i] = new TH2I(Name1, Name1, 40, -1.5, 38.5, 40, -1.5, 38.5);
        fh2_mult_RvsL_condTpat[i]->GetXaxis()->SetTitle("Multiplicity per event on the Left Pmt");
        fh2_mult_RvsL_condTpat[i]->GetYaxis()->SetTitle("Multiplicity per event on the Right Pmt");
        fh2_mult_RvsL_condTpat[i]->GetXaxis()->CenterTitle(true);
        fh2_mult_RvsL_condTpat[i]->GetYaxis()->CenterTitle(true);
        fh2_mult_RvsL_condTpat[i]->GetXaxis()->SetLabelSize(0.05);
        fh2_mult_RvsL_condTpat[i]->GetXaxis()->SetTitleSize(0.05);
        fh2_mult_RvsL_condTpat[i]->GetYaxis()->SetLabelSize(0.05);
        fh2_mult_RvsL_condTpat[i]->GetYaxis()->SetTitleSize(0.05);
        cMultMap2D_RvsL->cd(fNbDetectors + i + 1);
        fh2_mult_RvsL_condTpat[i]->Draw("COL");

        // === TH1F: raw position at tcal level if mult=1 === //
        sprintf(Name1, "SofSci%i_RawPos_Tcal_Mult1", i + 1);
        fh1_RawPos_TcalMult1[i] = new TH1F(Name1, Name1, 200000, -100, 100);
        fh1_RawPos_TcalMult1[i]->GetXaxis()->SetTitle(
            "(RIGHT,Wix. side) -->  raw position [ns, 1ps/bin] --> (LEFT,Mes. side) -->");
        fh1_RawPos_TcalMult1[i]->GetYaxis()->SetTitle("Counts per bin");
        fh1_RawPos_TcalMult1[i]->GetXaxis()->CenterTitle(true);
        fh1_RawPos_TcalMult1[i]->GetYaxis()->CenterTitle(true);
        fh1_RawPos_TcalMult1[i]->GetXaxis()->SetLabelSize(0.05);
        fh1_RawPos_TcalMult1[i]->GetXaxis()->SetTitleSize(0.05);
        fh1_RawPos_TcalMult1[i]->GetYaxis()->SetLabelSize(0.05);
        fh1_RawPos_TcalMult1[i]->GetYaxis()->SetTitleSize(0.05);
        cPos[i]->cd(1);
        fh1_RawPos_TcalMult1[i]->Draw("");

        // === TH1F: raw position at single tcal level === //
        sprintf(Name1, "SofSci%i_RawPos_SingleTcal", i + 1);
        fh1_RawPos_SingleTcal[i] = new TH1F(Name1, Name1, 100000, -50, 50);
        fh1_RawPos_SingleTcal[i]->GetXaxis()->SetTitle(
            "(RIGHT,Wix. side) -->  raw position [ns, 1ps/bin] --> (LEFT,Mes. side) -->");
        fh1_RawPos_SingleTcal[i]->GetYaxis()->SetTitle("Counts per bin");
        fh1_RawPos_SingleTcal[i]->GetXaxis()->CenterTitle(true);
        fh1_RawPos_SingleTcal[i]->GetYaxis()->CenterTitle(true);
        fh1_RawPos_SingleTcal[i]->GetXaxis()->SetLabelSize(0.05);
        fh1_RawPos_SingleTcal[i]->GetXaxis()->SetTitleSize(0.05);
        fh1_RawPos_SingleTcal[i]->GetYaxis()->SetLabelSize(0.05);
        fh1_RawPos_SingleTcal[i]->GetYaxis()->SetTitleSize(0.05);
        cPos[i]->cd(2);
        fh1_RawPos_SingleTcal[i]->Draw("");

        // === TH1F: calibrated position in mm at cal level === //
        sprintf(Name1, "SofSci%i_CalPos_mm", i + 1);
        if (i == (fNbDetectors - 1))
            fh1_CalPos[i] = new TH1F(Name1, Name1, 5000, -25, 25);
        else
            fh1_CalPos[i] = new TH1F(Name1, Name1, 30000, -150, 150);
        fh1_CalPos[i]->GetXaxis()->SetTitle("(RIGHT, Wix. side) -->  x position [mm] --> (LEFT,Mes. side) -->");
        fh1_CalPos[i]->GetYaxis()->SetTitle("Counts per bin");
        fh1_CalPos[i]->GetXaxis()->CenterTitle(true);
        fh1_CalPos[i]->GetYaxis()->CenterTitle(true);
        fh1_CalPos[i]->GetXaxis()->SetLabelSize(0.05);
        fh1_CalPos[i]->GetXaxis()->SetTitleSize(0.05);
        fh1_CalPos[i]->GetYaxis()->SetLabelSize(0.05);
        fh1_CalPos[i]->GetYaxis()->SetTitleSize(0.05);
        cPos[i]->cd(3);
        fh1_CalPos[i]->Draw("");

        // +++ TH2F: raw position versus calibrated position === //
        sprintf(Name1, "SofSci%i_RawPosVsCalPos", i + 1);
        if (i == (fNbDetectors - 1))
            fh2_RawPosVsCalPos[i] = new TH2F(Name1, Name1, 500, -25, 25, 500, -25, 25);
        else
            fh2_RawPosVsCalPos[i] = new TH2F(Name1, Name1, 500, -100, 100, 500, -10, 10);
        fh2_RawPosVsCalPos[i]->GetXaxis()->SetTitle("Calculated X position [mm]");
        fh2_RawPosVsCalPos[i]->GetYaxis()->SetTitle(
            "(RIGHT, Wixhausen side) --->  Raw X position [ns]  ---> (LEFT, Messel side)");
        fh2_RawPosVsCalPos[i]->GetXaxis()->CenterTitle(true);
        fh2_RawPosVsCalPos[i]->GetYaxis()->CenterTitle(true);
        fh2_RawPosVsCalPos[i]->GetXaxis()->SetLabelSize(0.05);
        fh2_RawPosVsCalPos[i]->GetYaxis()->SetLabelSize(0.05);
        fh2_RawPosVsCalPos[i]->GetXaxis()->SetTitleSize(0.05);
        fh2_RawPosVsCalPos[i]->GetYaxis()->SetTitleSize(0.05);
        cRawPosVsCalPos[i]->cd();
        fh2_RawPosVsCalPos[i]->Draw("col");
    } // end of loop over fNbDetectors

    if (fIdS2)
    {
        // === TCanvas: position verus Tof from S2 at cal level ===//
        cPosVsTofS2 = new TCanvas("PosVsTofS2", "PosVsTofS2", 10, 10, 800, 700);
        cPosVsTofS2->Divide(2, fNbDetectors - fIdS2);
        Float_t minS2 = 0, maxS2 = 1;

        for (Int_t i = fIdS2; i < fNbDetectors; i++)
        {
            // === TCanvas: time of flight from S2 calculated with Tref signal === //
            sprintf(Name1, "Tof_S2_Sci%d", i + 1);
            cTofFromS2[i - fIdS2] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
            cTofFromS2[i - fIdS2]->Divide(1, 3);

            // === TCanvas: beta Calculated from ToF from S2 === //
            sprintf(Name1, "Beta_S2_Sci%d", i + 1);
            cBetaFromS2[i - fIdS2] = new TCanvas(Name1, Name1, 10, 10, 800, 700);

            // === TH1D: raw ToF from S2 at tcal level if mult==1 for the 4 PMTs + 2 Tref signals === //
            sprintf(Name1, "RawTofNs_Tcal_m1_wTref_S2_to_Sci%02d", i + 1);
            fh1_RawTofFromS2_TcalMult1[i - fIdS2] = new TH1D(Name1, Name1, 1000000, -50000, 50000);
            fh1_RawTofFromS2_TcalMult1[i - fIdS2]->GetXaxis()->SetTitle("Raw Tof [ns]");
            fh1_RawTofFromS2_TcalMult1[i - fIdS2]->GetYaxis()->SetTitle("Counts per bin");
            fh1_RawTofFromS2_TcalMult1[i - fIdS2]->GetXaxis()->CenterTitle(true);
            fh1_RawTofFromS2_TcalMult1[i - fIdS2]->GetYaxis()->CenterTitle(true);
            fh1_RawTofFromS2_TcalMult1[i - fIdS2]->GetXaxis()->SetLabelSize(0.05);
            fh1_RawTofFromS2_TcalMult1[i - fIdS2]->GetXaxis()->SetTitleSize(0.05);
            fh1_RawTofFromS2_TcalMult1[i - fIdS2]->GetYaxis()->SetLabelSize(0.05);
            fh1_RawTofFromS2_TcalMult1[i - fIdS2]->GetYaxis()->SetTitleSize(0.05);
            cTofFromS2[i - fIdS2]->cd(1);
            fh1_RawTofFromS2_TcalMult1[i - fIdS2]->Draw("");

            // === TH1D: raw ToF from S2 at single tcal level === //
            sprintf(Name1, "RawTofNs_SingleTcal_wTref_S2_to_Sci%02d", i + 1);
            fh1_RawTofFromS2_SingleTcal[i - fIdS2] = new TH1D(Name1, Name1, 800000, -4000, 4000);
            fh1_RawTofFromS2_SingleTcal[i - fIdS2]->GetXaxis()->SetTitle("Raw Tof [ns]");
            fh1_RawTofFromS2_SingleTcal[i - fIdS2]->GetYaxis()->SetTitle("Counts per bin");
            fh1_RawTofFromS2_SingleTcal[i - fIdS2]->GetXaxis()->CenterTitle(true);
            fh1_RawTofFromS2_SingleTcal[i - fIdS2]->GetYaxis()->CenterTitle(true);
            fh1_RawTofFromS2_SingleTcal[i - fIdS2]->GetXaxis()->SetLabelSize(0.05);
            fh1_RawTofFromS2_SingleTcal[i - fIdS2]->GetXaxis()->SetTitleSize(0.05);
            fh1_RawTofFromS2_SingleTcal[i - fIdS2]->GetYaxis()->SetLabelSize(0.05);
            fh1_RawTofFromS2_SingleTcal[i - fIdS2]->GetYaxis()->SetTitleSize(0.05);
            cTofFromS2[i - fIdS2]->cd(2);
            fh1_RawTofFromS2_SingleTcal[i - fIdS2]->Draw("");

            // === TH1D: calibrated ToF from S2 at cal level === //
            sprintf(Name1, "CalTofNs_S2_to_Sci%02d", i + 1);
            fh1_CalTofFromS2[i - fIdS2] = new TH1D(Name1, Name1, 100000, 0, 1000);
            fh1_CalTofFromS2[i - fIdS2]->GetXaxis()->SetTitle("Calibrated Tof [ns]");
            fh1_CalTofFromS2[i - fIdS2]->GetYaxis()->SetTitle("Counts per bin");
            fh1_CalTofFromS2[i - fIdS2]->GetXaxis()->CenterTitle(true);
            fh1_CalTofFromS2[i - fIdS2]->GetYaxis()->CenterTitle(true);
            fh1_CalTofFromS2[i - fIdS2]->GetXaxis()->SetLabelSize(0.05);
            fh1_CalTofFromS2[i - fIdS2]->GetXaxis()->SetTitleSize(0.05);
            fh1_CalTofFromS2[i - fIdS2]->GetYaxis()->SetLabelSize(0.05);
            fh1_CalTofFromS2[i - fIdS2]->GetYaxis()->SetTitleSize(0.05);
            cTofFromS2[i - fIdS2]->cd(3);
            fh1_CalTofFromS2[i - fIdS2]->Draw("");

            // === TH1D: beta at cal level calculated from ToF from S2 === //
            sprintf(Name1, "CalBeta_S2_to_Sci%02d", i + 1);
            fh1_BetaFromS2[i - fIdS2] = new TH1D(Name1, Name1, 30000, 0.6, 0.9);
            fh1_BetaFromS2[i - fIdS2]->GetXaxis()->SetTitle("beta = v/c");
            fh1_BetaFromS2[i - fIdS2]->GetYaxis()->SetTitle("Counts per bin");
            fh1_BetaFromS2[i - fIdS2]->GetXaxis()->CenterTitle(true);
            fh1_BetaFromS2[i - fIdS2]->GetYaxis()->CenterTitle(true);
            fh1_BetaFromS2[i - fIdS2]->GetXaxis()->SetLabelSize(0.05);
            fh1_BetaFromS2[i - fIdS2]->GetXaxis()->SetTitleSize(0.05);
            fh1_BetaFromS2[i - fIdS2]->GetYaxis()->SetLabelSize(0.05);
            fh1_BetaFromS2[i - fIdS2]->GetYaxis()->SetTitleSize(0.05);
            cBetaFromS2[i - fIdS2]->cd();
            fh1_BetaFromS2[i - fIdS2]->Draw("");

            // === TH2D: posS2 vs tof from S2  === //
            sprintf(Name1, "PosS2_vs_Tof_S2_Sci%02d", i + 1);
            minS2 = GetTofS2min(i - fIdS2);
            maxS2 = GetTofS2max(i - fIdS2);
            fh2_PosVsTofS2[(i - fIdS2) * 2] =
                new TH2D(Name1, Name1, 50. * (maxS2 - minS2), minS2, maxS2, 1000, -100, 100);
            sprintf(Name1, "Tof S2 - Sci%02d [ns]", i + 1);
            fh2_PosVsTofS2[(i - fIdS2) * 2]->GetXaxis()->SetTitle(Name1);
            fh2_PosVsTofS2[(i - fIdS2) * 2]->GetYaxis()->SetTitle("Pos at S2 [mm]");
            fh2_PosVsTofS2[(i - fIdS2) * 2]->GetXaxis()->CenterTitle(true);
            fh2_PosVsTofS2[(i - fIdS2) * 2]->GetYaxis()->CenterTitle(true);
            fh2_PosVsTofS2[(i - fIdS2) * 2]->GetXaxis()->SetLabelSize(0.05);
            fh2_PosVsTofS2[(i - fIdS2) * 2]->GetXaxis()->SetTitleSize(0.05);
            fh2_PosVsTofS2[(i - fIdS2) * 2]->GetYaxis()->SetLabelSize(0.05);
            fh2_PosVsTofS2[(i - fIdS2) * 2]->GetYaxis()->SetTitleSize(0.05);
            cPosVsTofS2->cd((i - fIdS2) * 2 + 1);
            fh2_PosVsTofS2[(i - fIdS2) * 2]->Draw("col");

            // === TH2D: pos SofSci stop vs tof from S2  === //
            sprintf(Name1, "PosSci%02d_vs_Tof_S2_Sci%02d", i + 1, i + 1);
            fh2_PosVsTofS2[(i - fIdS2) * 2 + 1] =
                new TH2D(Name1, Name1, 50 * (maxS2 - minS2), minS2, maxS2, 1000, -25, 25);
            sprintf(Name1, "Tof S2 - Sci%02d [ns]", i + 1);
            fh2_PosVsTofS2[(i - fIdS2) * 2 + 1]->GetXaxis()->SetTitle(Name1);
            sprintf(Name1, "Pos at Sci%02d [mm]", i + 1);
            fh2_PosVsTofS2[(i - fIdS2) * 2 + 1]->GetYaxis()->SetTitle(Name1);
            fh2_PosVsTofS2[(i - fIdS2) * 2 + 1]->GetXaxis()->CenterTitle(true);
            fh2_PosVsTofS2[(i - fIdS2) * 2 + 1]->GetYaxis()->CenterTitle(true);
            fh2_PosVsTofS2[(i - fIdS2) * 2 + 1]->GetXaxis()->SetLabelSize(0.05);
            fh2_PosVsTofS2[(i - fIdS2) * 2 + 1]->GetXaxis()->SetTitleSize(0.05);
            fh2_PosVsTofS2[(i - fIdS2) * 2 + 1]->GetYaxis()->SetLabelSize(0.05);
            fh2_PosVsTofS2[(i - fIdS2) * 2 + 1]->GetYaxis()->SetTitleSize(0.05);
            cPosVsTofS2->cd((i - fIdS2) * 2 + 2);
            fh2_PosVsTofS2[(i - fIdS2) * 2 + 1]->Draw("col");
        }
    } // end of if SofSci at S2

    if (fIdS8)
    {
        // === TCanvas: position verus Tof from S8 at cal level ===//
        cPosVsTofS8 = new TCanvas("PosVsTofS8", "PosVsTofS28", 10, 10, 800, 700);
        cPosVsTofS8->Divide(2, fNbDetectors - fIdS8);
        Int_t minS8 = 0, maxS8 = 1;

        for (Int_t i = fIdS8; i < fNbDetectors; i++)
        {
            // === TCanvas: time of flight from S8 calculated with Tref signal === //
            sprintf(Name1, "Tof_S8_Sci%d", i + 1);
            cTofFromS8[i - fIdS8] = new TCanvas(Name1, Name1, 10, 10, 800, 700);
            cTofFromS8[i - fIdS8]->Divide(1, 3);
            minS8 = GetTofS8min(i - fIdS8);
            maxS8 = GetTofS8max(i - fIdS8);

            // === TCanvas: beta Calculated from ToF from S8 === //
            sprintf(Name1, "Beta_S8_Sci%d", i + 1);
            cBetaFromS8[i - fIdS8] = new TCanvas(Name1, Name1, 10, 10, 800, 700);

            // === TH1D: raw ToF from S8 at tcal level if mult==1 for the 4 PMTs + 2 Tref signals === //
            sprintf(Name1, "RawTofNs_Tcal_m1_wTref_S8_to_Sci%02d", i + 1);
            fh1_RawTofFromS8_TcalMult1[i - fIdS8] = new TH1D(Name1, Name1, 800000, -4000, 4000);
            fh1_RawTofFromS8_TcalMult1[i - fIdS8]->GetXaxis()->SetTitle("Raw Tof [ns]");
            fh1_RawTofFromS8_TcalMult1[i - fIdS8]->GetYaxis()->SetTitle("Counts per bin");
            fh1_RawTofFromS8_TcalMult1[i - fIdS8]->GetXaxis()->CenterTitle(true);
            fh1_RawTofFromS8_TcalMult1[i - fIdS8]->GetYaxis()->CenterTitle(true);
            fh1_RawTofFromS8_TcalMult1[i - fIdS8]->GetXaxis()->SetLabelSize(0.05);
            fh1_RawTofFromS8_TcalMult1[i - fIdS8]->GetXaxis()->SetTitleSize(0.05);
            fh1_RawTofFromS8_TcalMult1[i - fIdS8]->GetYaxis()->SetLabelSize(0.05);
            fh1_RawTofFromS8_TcalMult1[i - fIdS8]->GetYaxis()->SetTitleSize(0.05);
            cTofFromS8[i - fIdS8]->cd(1);
            fh1_RawTofFromS8_TcalMult1[i - fIdS8]->Draw("");

            // === TH1D: raw ToF from S8 at single tcal level === //
            sprintf(Name1, "RawTofNs_SingleTcal_wTref_S8_to_Sci%02d", i + 1);
            fh1_RawTofFromS8_SingleTcal[i - fIdS8] = new TH1D(Name1, Name1, 800000, -4000, 4000);
            fh1_RawTofFromS8_SingleTcal[i - fIdS8]->GetXaxis()->SetTitle("Raw Tof [ns]");
            fh1_RawTofFromS8_SingleTcal[i - fIdS8]->GetYaxis()->SetTitle("Counts per bin");
            fh1_RawTofFromS8_SingleTcal[i - fIdS8]->GetXaxis()->CenterTitle(true);
            fh1_RawTofFromS8_SingleTcal[i - fIdS8]->GetYaxis()->CenterTitle(true);
            fh1_RawTofFromS8_SingleTcal[i - fIdS8]->GetXaxis()->SetLabelSize(0.05);
            fh1_RawTofFromS8_SingleTcal[i - fIdS8]->GetXaxis()->SetTitleSize(0.05);
            fh1_RawTofFromS8_SingleTcal[i - fIdS8]->GetYaxis()->SetLabelSize(0.05);
            fh1_RawTofFromS8_SingleTcal[i - fIdS8]->GetYaxis()->SetTitleSize(0.05);
            cTofFromS8[i - fIdS8]->cd(2);
            fh1_RawTofFromS8_SingleTcal[i - fIdS8]->Draw("");

            // === TH1D: calibrated ToF from S8 at cal level === //
            sprintf(Name1, "CalTofNs_S8_to_Sci%02d", i + 1);
            fh1_CalTofFromS8[i - fIdS8] = new TH1D(Name1, Name1, 100000, 0, 1000);
            fh1_CalTofFromS8[i - fIdS8]->GetXaxis()->SetTitle("Calibrated Tof [ns]");
            fh1_CalTofFromS8[i - fIdS8]->GetYaxis()->SetTitle("Counts per bin");
            fh1_CalTofFromS8[i - fIdS8]->GetXaxis()->CenterTitle(true);
            fh1_CalTofFromS8[i - fIdS8]->GetYaxis()->CenterTitle(true);
            fh1_CalTofFromS8[i - fIdS8]->GetXaxis()->SetLabelSize(0.05);
            fh1_CalTofFromS8[i - fIdS8]->GetXaxis()->SetTitleSize(0.05);
            fh1_CalTofFromS8[i - fIdS8]->GetYaxis()->SetLabelSize(0.05);
            fh1_CalTofFromS8[i - fIdS8]->GetYaxis()->SetTitleSize(0.05);
            cTofFromS8[i - fIdS8]->cd(3);
            fh1_CalTofFromS8[i - fIdS8]->Draw("");

            // === TH1D: beta at cal level calculated from ToF from S8 === //
            sprintf(Name1, "CalBeta_S8_to_Sci%02d", i + 1);
            fh1_BetaFromS8[i - fIdS8] = new TH1D(Name1, Name1, 30000, 0.6, 0.9);
            fh1_BetaFromS8[i - fIdS8]->GetXaxis()->SetTitle("beta = v/c");
            fh1_BetaFromS8[i - fIdS8]->GetYaxis()->SetTitle("Counts per bin");
            fh1_BetaFromS8[i - fIdS8]->GetXaxis()->CenterTitle(true);
            fh1_BetaFromS8[i - fIdS8]->GetYaxis()->CenterTitle(true);
            fh1_BetaFromS8[i - fIdS8]->GetXaxis()->SetLabelSize(0.05);
            fh1_BetaFromS8[i - fIdS8]->GetXaxis()->SetTitleSize(0.05);
            fh1_BetaFromS8[i - fIdS8]->GetYaxis()->SetLabelSize(0.05);
            fh1_BetaFromS8[i - fIdS8]->GetYaxis()->SetTitleSize(0.05);
            cBetaFromS8[i - fIdS8]->cd();
            fh1_BetaFromS8[i - fIdS8]->Draw("");

            // === TH2D: posS8 vs tof from S8  === //
            sprintf(Name1, "PosS8_vs_Tof_S8-Sci%02d", i + 1);
            fh2_PosVsTofS8[(i - fIdS8) * 2] = new TH2D(Name1, Name1, 50 * (maxS8 - minS8), minS8, maxS8, 400, -10, 10);
            sprintf(Name1, "Tof S8 - Sci%02d [ns]", i + 1);
            fh2_PosVsTofS8[(i - fIdS8) * 2]->GetXaxis()->SetTitle(Name1);
            fh2_PosVsTofS8[(i - fIdS8) * 2]->GetYaxis()->SetTitle("Pos at S8 [mm]");
            fh2_PosVsTofS8[(i - fIdS8) * 2]->GetXaxis()->CenterTitle(true);
            fh2_PosVsTofS8[(i - fIdS8) * 2]->GetYaxis()->CenterTitle(true);
            fh2_PosVsTofS8[(i - fIdS8) * 2]->GetXaxis()->SetLabelSize(0.05);
            fh2_PosVsTofS8[(i - fIdS8) * 2]->GetXaxis()->SetTitleSize(0.05);
            fh2_PosVsTofS8[(i - fIdS8) * 2]->GetYaxis()->SetLabelSize(0.05);
            fh2_PosVsTofS8[(i - fIdS8) * 2]->GetYaxis()->SetTitleSize(0.05);
            cPosVsTofS8->cd((i - fIdS8) * 2 + 1);
            fh2_PosVsTofS8[(i - fIdS8) * 2]->Draw("col");

            // === TH2D: pos SofSci stop vs tof from S8  === //
            sprintf(Name1, "PosSci%02d_vs_Tof_S8-Sci%02d", i + 1, i + 1);
            fh2_PosVsTofS8[(i - fIdS8) * 2 + 1] =
                new TH2D(Name1, Name1, 50 * (maxS8 - minS8), minS8, maxS8, 400, -10, 10);
            sprintf(Name1, "Tof S8 - Sci%02d [ns]", i + 1);
            fh2_PosVsTofS8[(i - fIdS8) * 2 + 1]->GetXaxis()->SetTitle(Name1);
            sprintf(Name1, "Pos at Sci%02d [mm]", i + 1);
            fh2_PosVsTofS8[(i - fIdS8) * 2 + 1]->GetYaxis()->SetTitle(Name1);
            fh2_PosVsTofS8[(i - fIdS8) * 2 + 1]->GetXaxis()->CenterTitle(true);
            fh2_PosVsTofS8[(i - fIdS8) * 2 + 1]->GetYaxis()->CenterTitle(true);
            fh2_PosVsTofS8[(i - fIdS8) * 2 + 1]->GetXaxis()->SetLabelSize(0.05);
            fh2_PosVsTofS8[(i - fIdS8) * 2 + 1]->GetXaxis()->SetTitleSize(0.05);
            fh2_PosVsTofS8[(i - fIdS8) * 2 + 1]->GetYaxis()->SetLabelSize(0.05);
            fh2_PosVsTofS8[(i - fIdS8) * 2 + 1]->GetYaxis()->SetTitleSize(0.05);
            cPosVsTofS8->cd((i - fIdS8) * 2 + 2);
            fh2_PosVsTofS8[(i - fIdS8) * 2 + 1]->Draw("col");
        }
    } // end of if SofSci at S8

    // --- --------------- --- //
    // --- MAIN FOLDER-Sci --- //
    // --- --------------- --- //
    TFolder* mainfolSciMult = new TFolder("SofSciMult", "SofSci Mult info");
    mainfolSciMult->Add(cMultMap);
    mainfolSciMult->Add(cMultTcal);
    mainfolSciMult->Add(cMultSingleTcal);
    mainfolSciMult->Add(cMultCal);
    mainfolSciMult->Add(cMultMap2D);
    mainfolSciMult->Add(cMultMap2D_RvsL);

    TFolder* mainfolSci = new TFolder("SofSci", "SofSci info");
    mainfolSci->Add(cDeltaClockPerSci);
    for (Int_t i = 0; i < fNbDetectors; i++)
    {
        mainfolSci->Add(cMapped[i]);
        mainfolSci->Add(cPos[i]);
        mainfolSci->Add(cRawPosVsCalPos[i]);
    }
    if (fNbDetectors > 1)
        mainfolSci->Add(cDeltaTref);
    if (fIdS2 > 0)
    {
        for (Int_t i = fIdS2; i < fNbDetectors; i++)
        {
            mainfolSci->Add(cTofFromS2[i - fIdS2]);
            mainfolSci->Add(cBetaFromS2[i - fIdS2]);
        }
        mainfolSci->Add(cPosVsTofS2);
    }
    if (fIdS8 > 0)
    {
        for (Int_t i = fIdS8; i < fNbDetectors; i++)
        {
            mainfolSci->Add(cTofFromS8[i - fIdS8]);
            mainfolSci->Add(cBetaFromS8[i - fIdS8]);
        }
        mainfolSci->Add(cPosVsTofS8);
    }
    run->AddObject(mainfolSciMult);
    run->AddObject(mainfolSci);

    // Register command to reset histograms
    run->GetHttpServer()->RegisterCommand("Reset_SOFSCI_HIST", Form("/Objects/%s/->Reset_Histo()", GetName()));

    return kSUCCESS;
}

void R3BSofSciOnlineSpectra::Reset_Histo()
{
    LOG(INFO) << "R3BSofSciOnlineSpectra::Reset_Histo";
    for (Int_t d = 0; d < fNbDetectors; d++)
        for (Int_t p = 0; p < 2; p++)
        {
            fh1_deltaClockPerSci[2 * d + p]->Reset();
            fh1_deltaClockPerSci_condTpat[2 * d + p]->Reset();
        }
    for (Int_t i = 0; i < fNbDetectors; i++)
    {
        // === MULTIPLICITY === //
        fh1_multMap[i]->Reset();
        fh1_multTcal[i]->Reset();
        fh1_multSingleTcal[i]->Reset();
        fh1_multCal[i]->Reset();
        fh1_multMap_condTpat[i]->Reset();
        fh1_multTcal_condTpat[i]->Reset();
        fh1_multSingleTcal_condTpat[i]->Reset();
        // fh1_multCal_condTpat[i]->Reset();
        fh2_mult[i]->Reset();
        fh2_mult_RvsL[i]->Reset();
        fh2_mult_RvsL_condTpat[i]->Reset();
        for (Int_t j = 0; j < fNbChannels - 1; j++)
        {
            fh2_mult_TrefVsPmt[i * (fNbChannels - 1) + j]->Reset();
            fh2_mult_TrefVsPmt_condTpat[i * (fNbChannels - 1) + j]->Reset();
        }

        // === FINE TIME === //
        for (Int_t j = 0; j < fNbChannels; j++)
            fh1_finetime[i * fNbChannels + j]->Reset();

        // === POSITION === //
        fh1_RawPos_TcalMult1[i]->Reset();
        fh1_RawPos_SingleTcal[i]->Reset();
        fh1_CalPos[i]->Reset();
        fh2_RawPosVsCalPos[i]->Reset();
    }

    if (fNbDetectors > 1)
    {
        for (int d = 0; d < fNbDetectors - 1; d++)
            fh1_DeltaTref[d]->Reset();
    }
    // === TIME OF FLIGHT AND BETA === //
    if (fIdS2 > 0)
    {
        for (Int_t i = fIdS2; i < fNbDetectors; i++)
        {
            fh1_RawTofFromS2_TcalMult1[i - fIdS2]->Reset();
            fh1_RawTofFromS2_SingleTcal[i - fIdS2]->Reset();
            fh1_CalTofFromS2[i - fIdS2]->Reset();
            fh1_BetaFromS2[i - fIdS2]->Reset();
            fh2_PosVsTofS2[(i - fIdS2) * 2]->Reset();
            fh2_PosVsTofS2[(i - fIdS2) * 2 + 1]->Reset();
        }
    }
    if (fIdS8 > 0)
    {
        for (Int_t i = fIdS8; i < fNbDetectors; i++)
        {
            fh1_RawTofFromS8_TcalMult1[i - fIdS8]->Reset();
            fh1_RawTofFromS8_SingleTcal[i - fIdS8]->Reset();
            fh1_CalTofFromS8[i - fIdS8]->Reset();
            fh1_BetaFromS8[i - fIdS8]->Reset();
            fh2_PosVsTofS8[(i - fIdS8) * 2]->Reset();
            fh2_PosVsTofS8[(i - fIdS8) * 2 + 1]->Reset();
        }
    }
}

void R3BSofSciOnlineSpectra::Exec(Option_t* option)
{
    FairRootManager* mgr = FairRootManager::Instance();
    if (NULL == mgr)
        LOG(FATAL) << "R3BSofSciOnlineSpectra::Exec FairRootManager not found";

    // --- -------------- --- //
    // --- TPAT CONDITION --- //
    // --- -------------- --- //
    Bool_t BeamOrFission = kFALSE;
    if (fEventHeader->GetTpat() > 0)
    {
        if ((fEventHeader->GetTpat() & 0x1) == 1 || // beam
            (fEventHeader->GetTpat() & 0x2) == 2)   // fission
            BeamOrFission = kTRUE;
    }

    // --- --------------- --- //
    // --- local variables --- //
    // --- --------------- --- //

    Int_t nHits;
    Int_t iDet; // 0-based
    Int_t iCh;  // 0-based
    Float_t iRawPos;
    Double_t iRawTof;
    Double_t TofS2[fNbDetectors - fIdS2];
    Double_t TofS8[fNbDetectors - fIdS8];
    Float_t RawPos[fNbDetectors];
    Float_t CalPos[fNbDetectors];

    Int_t multMap[fNbDetectors * fNbChannels];
    Int_t multTcal[fNbDetectors * fNbChannels];
    Int_t multSTcal[fNbDetectors];
    Int_t multCal[fNbDetectors];
    Double_t iRawTimeNs[fNbDetectors * fNbChannels];
    Float_t iClock[fNbDetectors * fNbChannels];
    Float_t iCoarse[fNbDetectors * fNbChannels];

    for (Int_t i = 0; i < fNbDetectors; i++)
    {
        for (Int_t j = 0; j < fNbChannels; j++)
        {
            multMap[i * fNbChannels + j] = 0;
            multTcal[i * fNbChannels + j] = 0;
            iRawTimeNs[i * fNbChannels + j] = 0;
            iClock[i * fNbChannels + j] = 0;
            iCoarse[i * fNbChannels + j] = 0;
        }
        multSTcal[i] = 0;
        multCal[i] = 0;
        RawPos[i] = -100000.;
        CalPos[i] = -100000.;
    }

    if (fMapped && fMapped->GetEntriesFast())
    {
        // --- ---------------- --- //
        // --- read mapped data --- //
        // --- ---------------- --- //
        nHits = fMapped->GetEntriesFast();
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofSciMappedData* hitmapped = (R3BSofSciMappedData*)fMapped->At(ihit);
            if (!hitmapped)
                continue;
            iDet = hitmapped->GetDetector() - 1;
            iCh = hitmapped->GetPmt() - 1;
            multMap[iDet * fNbChannels + iCh]++;
            fh1_finetime[iDet * fNbChannels + iCh]->Fill(hitmapped->GetTimeFine());
            iCoarse[iDet * fNbChannels + iCh] = (Float_t)hitmapped->GetTimeCoarse();
        } // end of loop over mapped data

        if (fTcal && fTcal->GetEntriesFast())
        {
            // --- -------------- --- //
            // --- read tcal data --- //
            // --- -------------- --- //
            nHits = fTcal->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++)
            {
                R3BSofSciTcalData* hittcal = (R3BSofSciTcalData*)fTcal->At(ihit);
                if (!hittcal)
                    continue;
                iDet = hittcal->GetDetector() - 1;
                iCh = hittcal->GetPmt() - 1;
                multTcal[iDet * fNbChannels + iCh]++;
                iRawTimeNs[iDet * fNbChannels + iCh] = hittcal->GetRawTimeNs();
                iClock[iDet * fNbChannels + iCh] = (Float_t)hittcal->GetCoarseTime();
                if (multMap[iDet * fNbChannels + iCh] == 1 &&
                    iCoarse[iDet * fNbChannels + iCh] != iClock[iDet * fNbChannels + iCh])
                {
                    std::cout << "iCoarse[" << iDet * fNbChannels + iCh << "] = " << iCoarse[iDet * fNbChannels + iCh]
                              << ", iClock[" << iDet * fNbChannels + iCh << "]" << iClock[iDet * fNbChannels + iCh]
                              << std::endl;
                }
            } // --- end of loop over Tcal data --- //

            if (fNbDetectors > 1)
            {
                for (int d = 0; d < fNbDetectors - 1; d++)
                {
                    if (multMap[(fNbDetectors - 1) * fNbChannels + 2] == 1 && multMap[d * fNbChannels + 2] == 1)
                        fh1_DeltaTref[d]->Fill(iRawTimeNs[d * fNbChannels + 2] -
                                               iRawTimeNs[(fNbDetectors - 1) * fNbChannels + 2]);
                    // std::cout << "index first tref : " << d*fNbChannels+2 << ", index cave C Tref = " <<
                    // (fNbDetectors-1)*fNbChannels+2 << std::endl;
                }
            }

            // --- --------------------- --- //
            // --- read single tcal data --- //
            // --- --------------------- --- //
            if (fSingleTcal && fSingleTcal->GetEntries())
            {
                nHits = fSingleTcal->GetEntriesFast();
                for (Int_t ihit = 0; ihit < nHits; ihit++)
                {
                    R3BSofSciSingleTcalData* hitstcal = (R3BSofSciSingleTcalData*)fSingleTcal->At(ihit);
                    if (!hitstcal)
                        continue;
                    iDet = hitstcal->GetDetector() - 1;
                    multSTcal[iDet]++;
                    RawPos[iDet] = hitstcal->GetRawPosNs();
                    fh1_RawPos_SingleTcal[iDet]->Fill(hitstcal->GetRawPosNs());
                    if (fIdS2 > 0 && hitstcal->GetDetector() > fIdS2)
                        fh1_RawTofFromS2_SingleTcal[iDet - fIdS2]->Fill(hitstcal->GetRawTofNs_FromS2());
                    if (fIdS8 > 0 && hitstcal->GetDetector() > fIdS8)
                        fh1_RawTofFromS8_SingleTcal[iDet - fIdS8]->Fill(hitstcal->GetRawTofNs_FromS8());
                } // --- end of loop over SingleTcal data --- //

                if (fCal && fCal->GetEntries())
                {
                    // --- ------------- --- //
                    // --- read cal data --- //
                    // --- ------------- --- //
                    for (Int_t i = fIdS2; i < fNbDetectors; i++)
                        TofS2[i - fIdS2] = 0;
                    for (Int_t i = fIdS8; i < fNbDetectors; i++)
                        TofS8[i - fIdS8] = 0;
                    nHits = fCal->GetEntriesFast();
                    for (Int_t ihit = 0; ihit < nHits; ihit++)
                    {
                        R3BSofSciCalData* hitcal = (R3BSofSciCalData*)fCal->At(ihit);
                        if (!hitcal)
                            continue;
                        iDet = hitcal->GetDetector() - 1;
                        multCal[iDet]++;
                        fh1_CalPos[iDet]->Fill(hitcal->GetPosMm());
                        CalPos[iDet] = hitcal->GetPosMm();
                        if (fIdS2 > 0 && hitcal->GetDetector() > fIdS2)
                        {
                            fh1_CalTofFromS2[iDet - fIdS2]->Fill(hitcal->GetTofNs_S2());
                            fh1_BetaFromS2[iDet - fIdS2]->Fill(hitcal->GetBeta_S2());
                            TofS2[iDet - fIdS2] = hitcal->GetTofNs_S2();
                            fh2_PosVsTofS2[2 * (iDet - fIdS2) + 1]->Fill(hitcal->GetTofNs_S2(), hitcal->GetPosMm());
                        }
                        if (fIdS8 > 0 && hitcal->GetDetector() > fIdS8)
                        {
                            fh1_CalTofFromS8[iDet - fIdS8]->Fill(hitcal->GetTofNs_S8());
                            fh1_BetaFromS8[iDet - fIdS8]->Fill(hitcal->GetBeta_S8());
                            TofS8[iDet - fIdS8] = hitcal->GetTofNs_S8();
                            fh2_PosVsTofS8[2 * (iDet - fIdS8) + 1]->Fill(TofS8[iDet - fIdS8], hitcal->GetPosMm());
                        }
                    } // --- end of loop over Cal data --- //
                }     // --- end of if Cal data --- //
            }         // --- end of if SingleTcal data --- //
        }             //--- end of if Tcal data --- //

        // --- ----------------------------------------- --- //
        // --- filling some histogramms outside the loop --- //
        // --- ----------------------------------------- --- //
        Float_t delta = 0;
        for (Int_t i = 0; i < fNbDetectors; i++)
        {
            fh2_mult_RvsL[i]->Fill(multMap[i * fNbChannels + 1], multMap[i * fNbChannels]);
            for (Int_t j = 0; j < (fNbChannels - 1); j++)
                fh2_mult_TrefVsPmt[i * (fNbChannels - 1) + j]->Fill(multMap[i * fNbChannels + j],
                                                                    multMap[i * fNbChannels + 2]);
            if (BeamOrFission == kTRUE)
            {
                fh2_mult_RvsL_condTpat[i]->Fill(multMap[i * fNbChannels + 1], multMap[i * fNbChannels]);
                for (Int_t j = 0; j < (fNbChannels - 1); j++)
                    fh2_mult_TrefVsPmt_condTpat[i * (fNbChannels - 1) + j]->Fill(multMap[i * fNbChannels + j],
                                                                                 multMap[i * fNbChannels + 2]);
            }

            for (Int_t j = 0; j < fNbChannels; j++)
            {
                fh2_mult[i]->Fill(j + 1, multMap[i * fNbChannels + j]);
                fh1_multMap[i * fNbChannels + j]->Fill(multMap[i * fNbChannels + j]);
                fh1_multTcal[i * fNbChannels + j]->Fill(multTcal[i * fNbChannels + j]);
                if (BeamOrFission == kTRUE)
                {
                    fh1_multMap_condTpat[i * fNbChannels + j]->Fill(multMap[i * fNbChannels + j]);
                    fh1_multTcal_condTpat[i * fNbChannels + j]->Fill(multTcal[i * fNbChannels + j]);
                }
            }
            fh1_multSingleTcal[i]->Fill(multSTcal[i]);
            fh1_multCal[i]->Fill(multCal[i]);
            if (BeamOrFission == kTRUE)
            {
                fh1_multSingleTcal_condTpat[i]->Fill(multSTcal[i]);
                // fh1_multCal_condTpat[i]->Fill(multCal[i]);
            }
            if ((multTcal[i * fNbChannels] == 1) && (multTcal[i * fNbChannels + 1] == 1))
            {
                // TrawRIGHT-TrawLEFT = 5*(CCr-CCl)+(FTl-FTr) : x is increasing from RIGHT to LEFT
                iRawPos = iRawTimeNs[i * fNbChannels] - iRawTimeNs[i * fNbChannels + 1];
                fh1_RawPos_TcalMult1[i]->Fill(iRawPos);
                if (multTcal[i * fNbChannels + 2] == 1)
                {
                    if (fNbDetectors > 1 && fIdS2 > 0)
                    {
                        if (i == 0)
                        {
                            fh1_deltaClockPerSci[2 * i]->Fill(
                                (Float_t)(iClock[i * fNbChannels + 2] - iClock[i * fNbChannels]) -
                                (Float_t)(DeltaClockTrefRight_S2));
                            fh1_deltaClockPerSci[2 * i + 1]->Fill(
                                (Float_t)(iClock[i * fNbChannels + 2] - iClock[i * fNbChannels + 1]) -
                                (Float_t)(DeltaClockTrefLeft_S2));
                            if (BeamOrFission == kTRUE)
                            {
                                fh1_deltaClockPerSci_condTpat[2 * i]->Fill(
                                    (Float_t)(iClock[i * fNbChannels + 2] - iClock[i * fNbChannels]) -
                                    (Float_t)(DeltaClockTrefRight_S2));
                                fh1_deltaClockPerSci_condTpat[2 * i + 1]->Fill(
                                    (Float_t)(iClock[i * fNbChannels + 2] - iClock[i * fNbChannels + 1]) -
                                    (Float_t)(DeltaClockTrefLeft_S2));
                            }
                        }
                        else if (i == 1)
                        {
                            fh1_deltaClockPerSci[2 * i]->Fill(
                                (Float_t)(iClock[i * fNbChannels + 2] - iClock[i * fNbChannels]) -
                                (Float_t)(DeltaClockTrefRight_CaveC));
                            fh1_deltaClockPerSci[2 * i + 1]->Fill(
                                (Float_t)(iClock[i * fNbChannels + 2] - iClock[i * fNbChannels + 1]) -
                                (Float_t)(DeltaClockTrefLeft_CaveC));
                            if (BeamOrFission == kTRUE)
                            {
                                fh1_deltaClockPerSci_condTpat[2 * i]->Fill(
                                    (Float_t)(iClock[i * fNbChannels + 2] - iClock[i * fNbChannels]) -
                                    (Float_t)(DeltaClockTrefRight_CaveC));
                                fh1_deltaClockPerSci_condTpat[2 * i + 1]->Fill(
                                    (Float_t)(iClock[i * fNbChannels + 2] - iClock[i * fNbChannels + 1]) -
                                    (Float_t)(DeltaClockTrefLeft_CaveC));
                            }
                        }
                    }
                    else if (fNbDetectors == 1)
                    {
                        fh1_deltaClockPerSci[2 * i]->Fill(
                            (Float_t)(iClock[i * fNbChannels + 2] - iClock[i * fNbChannels]) -
                            (Float_t)(DeltaClockTrefRight_CaveC));
                        fh1_deltaClockPerSci[2 * i + 1]->Fill(
                            (Float_t)(iClock[i * fNbChannels + 2] - iClock[i * fNbChannels + 1]) -
                            (Float_t)(DeltaClockTrefLeft_CaveC));
                        if (BeamOrFission == kTRUE)
                        {
                            fh1_deltaClockPerSci_condTpat[2 * i]->Fill(
                                (Float_t)(iClock[i * fNbChannels + 2] - iClock[i * fNbChannels]) -
                                (Float_t)(DeltaClockTrefRight_CaveC));
                            fh1_deltaClockPerSci_condTpat[2 * i + 1]->Fill(
                                (Float_t)(iClock[i * fNbChannels + 2] - iClock[i * fNbChannels + 1]) -
                                (Float_t)(DeltaClockTrefLeft_CaveC));
                        }
                    }
                }
            }
            fh2_RawPosVsCalPos[i]->Fill(CalPos[i], RawPos[i]);
        }

        if (fIdS2 > 0)
        {
            for (Int_t dstop = fIdS2; dstop < fNbDetectors; dstop++)
            {
                if ((multTcal[(fIdS2 - 1) * fNbChannels] == 1) &&     // SofSci at S2: PMT RIGHT
                    (multTcal[(fIdS2 - 1) * fNbChannels + 1] == 1) && // SofSci at S2: PMT LEFT
                    (multTcal[(fIdS2 - 1) * fNbChannels + 2] == 1) && // Tref of SofSci at S2
                    (multTcal[dstop * fNbChannels] == 1) &&           // SofSci stop: PMT RIGHT
                    (multTcal[dstop * fNbChannels + 1] == 1) &&       // SofSci stop: PMT LEFT
                    (multTcal[dstop * fNbChannels + 2] == 1))         // Tref of SofSci stop
                {
                    iRawTof =
                        0.5 * (iRawTimeNs[dstop * fNbChannels] + iRawTimeNs[dstop * fNbChannels + 1]) -
                        0.5 * (iRawTimeNs[(fIdS2 - 1) * fNbChannels] + iRawTimeNs[(fIdS2 - 1) * fNbChannels + 1]) +
                        iRawTimeNs[(fIdS2 - 1) * fNbChannels + 2] - iRawTimeNs[dstop * fNbChannels + 2];
                    fh1_RawTofFromS2_TcalMult1[dstop - fIdS2]->Fill(iRawTof);
                }
                fh2_PosVsTofS2[2 * (dstop - fIdS2)]->Fill(TofS2[dstop - fIdS2], CalPos[fIdS2 - 1]);
            }
        } // --- end of if SofSci at S2 --- //

        if (fIdS8 > 0)
        {
            for (Int_t dstop = fIdS8; dstop < fNbDetectors; dstop++)
            {
                if ((multMap[(fIdS8 - 1) * fNbChannels] == 1) &&     // SofSci at S8: PMT RIGHT
                    (multMap[(fIdS8 - 1) * fNbChannels + 1] == 1) && // SofSci at S8: PMT LEFT
                    (multMap[(fIdS8 - 1) * fNbChannels + 2] == 1) && // Tref of SofSci at S8
                    (multMap[dstop * fNbChannels] == 1) &&           // SofSci stop: PMT RIGHT
                    (multMap[dstop * fNbChannels + 1] == 1) &&       // SofSci stop: PMT LEFT
                    (multMap[dstop * fNbChannels + 2] == 1))         // Tref of SofSci stop
                {
                    iRawTof =
                        0.5 * (iRawTimeNs[dstop * fNbChannels] + iRawTimeNs[dstop * fNbChannels + 1]) -
                        0.5 * (iRawTimeNs[(fIdS8 - 1) * fNbChannels] + iRawTimeNs[(fIdS8 - 1) * fNbChannels + 1]) +
                        iRawTimeNs[(fIdS8 - 1) * fNbChannels + 2] - iRawTimeNs[dstop * fNbChannels + 2];
                    fh1_RawTofFromS8_TcalMult1[dstop - fIdS8]->Fill(iRawTof);
                }
                fh2_PosVsTofS8[2 * (dstop - fIdS8)]->Fill(TofS8[dstop - fIdS8], CalPos[fIdS8 - 1]);
            }
        } // --- end of if SofSci at S8 --- //
    }     // --- end of if Mapped data --- //

    fNEvents++;
}

// -----   Public method Reset   ------------------------------------------------
void R3BSofSciOnlineSpectra::Reset() {}

// -----   Public method Finish   -----------------------------------------------
void R3BSofSciOnlineSpectra::FinishEvent()
{
    if (fMapped)
    {
        fMapped->Clear();
    }
    if (fTcal)
    {
        fTcal->Clear();
    }
    if (fSingleTcal)
    {
        fSingleTcal->Clear();
    }
    if (fCal)
    {
        fCal->Clear();
    }
}

void R3BSofSciOnlineSpectra::FinishTask()
{

    for (Int_t i = 0; i < fNbDetectors; i++)
    {
        if (fMapped)
        {
            cMapped[i]->Write();
            fh2_mult[i]->Write();
            fh2_mult_RvsL[i]->Write();
            fh2_mult_RvsL_condTpat[i]->Write();
            for (Int_t j = 0; j < (fNbChannels - 1); j++)
            {
                fh2_mult_TrefVsPmt[i * (fNbChannels - 1) + j]->Write();
                fh2_mult_TrefVsPmt_condTpat[i * (fNbChannels - 1) + j]->Write();
            }
        }

        if (fTcal)
        {
            cPos[i]->Write();
            fh1_RawPos_TcalMult1[i]->Write();
            cDeltaClockPerSci->Write();
        }

        if (fSingleTcal)
        {
            fh1_RawPos_SingleTcal[i]->Write();
            fh1_multSingleTcal[i]->Write();
        }

        if (fCal)
        {
            fh1_CalPos[i]->Write();
            fh1_multCal[i]->Write();
            cRawPosVsCalPos[i]->Write();
            fh2_RawPosVsCalPos[i]->Write();
        }

        for (Int_t j = 0; j < fNbChannels; j++)
        {
            if (fMapped)
            {
                fh1_finetime[i * fNbChannels + j]->Write();
                fh1_multMap[i * fNbChannels + j]->Write();
            }
            if (fTcal)
            {
                fh1_multTcal[i * fNbChannels + j]->Write();
            }
        }
    } // end of loop over fNbDetectors

    if (fNbDetectors > 1 && fTcal)
    {
        cDeltaTref->Write();
    }
    if (fIdS2 > 0)
    {
        for (Int_t i = fIdS2; i < fNbDetectors; i++)
        {
            cTofFromS2[i - fIdS2]->Write();
            if (fTcal)
            {
                fh1_RawTofFromS2_TcalMult1[i - fIdS2]->Write();
            }
            if (fSingleTcal)
            {
                fh1_RawTofFromS2_SingleTcal[i - fIdS2]->Write();
            }
            if (fCal)
            {
                cBetaFromS2[i - fIdS2]->Write();
                fh1_CalTofFromS2[i - fIdS2]->Write();
                fh2_PosVsTofS2[2 * (i - fIdS2)]->Write();
                fh2_PosVsTofS2[2 * (i - fIdS2) + 1]->Write();
            }
        }
        // cPosVsTofS2->Write();
    } // end of SofSci at S2

    if (fIdS8 > 0)
    {
        for (Int_t i = fIdS8; i < fNbDetectors; i++)
        {
            cTofFromS8[i - fIdS8]->Write();
            if (fTcal)
            {
                fh1_RawTofFromS8_TcalMult1[i - fIdS8]->Write();
            }
            if (fSingleTcal)
            {
                fh1_RawTofFromS8_SingleTcal[i - fIdS8]->Write();
            }
            if (fCal)
            {
                fh1_CalTofFromS8[i - fIdS8]->Write();
                fh2_PosVsTofS8[2 * (i - fIdS8)]->Write();
                fh2_PosVsTofS8[2 * (i - fIdS8) + 1]->Write();
            }
            // cPosVsTofS8->Write();
        }
    } // end of SofSci at S8
}
