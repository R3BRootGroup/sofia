// ---------------------------------------------------------------------
// -----         R3BSofTofWSingleTCal2HitPar source file           -----
// -----      Created 29/01/20  by J.L. Rodriguez-Sanchez          -----
// ---------------------------------------------------------------------

// ROOT headers
#include "TClonesArray.h"
#include "TF1.h"
#include "TGraph.h"
#include "TMath.h"
#include "TRandom.h"
#include "TVector3.h"

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// TofW headers
#include "R3BSofTofWHitPar.h"
#include "R3BSofTofWSingleTCal2HitPar.h"
#include "R3BSofTofWSingleTcalData.h"

#include <iomanip>

// R3BSofTofWSingleTCal2HitPar: Default Constructor --------------------------
R3BSofTofWSingleTCal2HitPar::R3BSofTofWSingleTCal2HitPar()
    : FairTask("R3BSof TofW Calibrator", 1)
    , fNumSci(28)
    , fMinStatistics(1000)
    , fLimit_left_tof(-100.)
    , fLimit_right_tof(100.)
    , fLimit_left_pos(-50.)
    , fLimit_right_pos(50)
    , fNbBinsTof(2000)
    , fNbBinsPos(2000)
    , fMaxSigma(200)
    , TofParams(NULL)
    , PosParams(NULL)
    , fHit_Par(NULL)
    , fTofCalDataCA(NULL)
{
}

// R3BSofTofWSingleTCal2HitParPar: Standard Constructor --------------------------
R3BSofTofWSingleTCal2HitPar::R3BSofTofWSingleTCal2HitPar(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNumSci(28)
    , fMinStatistics(1000)
    , fLimit_left_tof(-100.)
    , fLimit_right_tof(100.)
    , fLimit_left_pos(-50.)
    , fLimit_right_pos(50)
    , fNbBinsTof(2000)
    , fNbBinsPos(2000)
    , fMaxSigma(200)
    , TofParams(NULL)
    , PosParams(NULL)
    , fHit_Par(NULL)
    , fTofCalDataCA(NULL)
{
}

// Virtual R3BSofTofWSingleTCal2HitPar: Destructor
R3BSofTofWSingleTCal2HitPar::~R3BSofTofWSingleTCal2HitPar()
{
    LOG(INFO) << "R3BSofTofWSingleTCal2HitPar: Delete instance";
    if (fTofCalDataCA)
        delete fTofCalDataCA;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTofWSingleTCal2HitPar::Init()
{
    LOG(INFO) << "R3BSofTofWSingleTCal2HitPar: Init";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fTofCalDataCA = (TClonesArray*)rootManager->GetObject("SofTofWSingleTcalData");
    if (!fTofCalDataCA)
    {
        LOG(ERROR) << "R3BSofTofWSingleTCal2HitPar: SofTofWSingleTcalData not found";
        return kFATAL;
    }

    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        return kFATAL;
    }

    fHit_Par = (R3BSofTofWHitPar*)rtdb->getContainer("tofwHitPar");
    if (!fHit_Par)
    {
        LOG(ERROR) << "R3BSofTofWSingleTCal2HitPar:: Couldn't get handle on tofwHitPar container";
        return kFATAL;
    }

    // Define histograms
    char Name1[255];
    htof = new TH1F*[fNumSci];
    hpos = new TH1F*[fNumSci];
    fNbBinsTof = (int)(fLimit_right_tof - fLimit_left_tof) * 100.;
    fNbBinsPos = (int)(fLimit_right_pos - fLimit_left_pos) * 100.;
    for (Int_t i = 0; i < fNumSci; i++)
    {
        sprintf(Name1, "htof_%d", i + 1);
        htof[i] = new TH1F(Name1, Name1, fNbBinsTof, fLimit_left_tof, fLimit_right_tof);
        sprintf(Name1, "hpos_%d", i + 1);
        hpos[i] = new TH1F(Name1, Name1, fNbBinsPos, fLimit_left_pos, fLimit_right_pos);
    }

    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofTofWSingleTCal2HitPar::ReInit() { return kSUCCESS; }

// -----   Public method Execution   --------------------------------------------
void R3BSofTofWSingleTCal2HitPar::Exec(Option_t* option)
{
    // Reading the Input -- Cal Data --
    Int_t nHits = fTofCalDataCA->GetEntries();

    if (nHits == 0)
        return;

    R3BSofTofWSingleTcalData** calData = new R3BSofTofWSingleTcalData*[nHits];
    Int_t sciId = 0;
    for (Int_t i = 0; i < nHits; i++)
    {
        calData[i] = (R3BSofTofWSingleTcalData*)(fTofCalDataCA->At(i));
        sciId = calData[i]->GetDetector() - 1;
        htof[sciId]->Fill(calData[i]->GetRawTofNs());
        hpos[sciId]->Fill(calData[i]->GetRawPosNs());
    }
    if (calData)
        delete calData;

    return;
}

// -----   Protected method Finish   --------------------------------------------
void R3BSofTofWSingleTCal2HitPar::FinishEvent() {}

// -----   Public method Reset   ------------------------------------------------
void R3BSofTofWSingleTCal2HitPar::Reset() {}

void R3BSofTofWSingleTCal2HitPar::FinishTask()
{
    fHit_Par->SetNumSci(fNumSci);

    TF1* fit1 = new TF1("fit1", "gaus", fLimit_left_pos, fLimit_right_pos);
    TF1* fit2 = new TF1("fit2", "gaus", fLimit_left_tof, fLimit_right_tof);
    for (Int_t s = 0; s < fNumSci; s++)
    {
        if (htof[s]->GetEntries() > fMinStatistics && htof[s]->GetEntries() > fMinStatistics)
        {

            // std::cout << s<< " " << htof[s]->GetMaximum() <<std::endl;

            // Bins with a number of counts less than 20% of the maximum are set to zero
            for (Int_t k2 = 0; k2 < fNbBinsPos; k2++)
            {

                if (hpos[s]->GetBinContent(k2 + 1) < 0.2 * hpos[s]->GetMaximum())
                    hpos[s]->SetBinContent(k2 + 1, 0);
            }

            fHit_Par->SetInUse(1, s + 1);
            hpos[s]->Fit("fit1", "QR0");
            Double_t par1[3];
            fit1->GetParameters(&par1[0]);
            fHit_Par->SetPosPar(par1[1], s + 1);

            for (Int_t k2 = 0; k2 < fNbBinsTof; k2++)
            {

                if (htof[s]->GetBinContent(k2 + 1) < 0.2 * htof[s]->GetMaximum())
                    htof[s]->SetBinContent(k2 + 1, 0);
            }

            htof[s]->Fit("fit2", "QR0");
            Double_t par2[3];
            fit2->GetParameters(&par2[0]);
            fHit_Par->SetTofPar(par2[1], s + 1);
        }
        else
        {
            fHit_Par->SetInUse(0, s + 1);
        }
    }

    // Set parameters
    fHit_Par->setChanged();

    for (Int_t s = 0; s < fNumSci; s++)
    {
        hpos[s]->Write();
        htof[s]->Write();
    }
}

ClassImp(R3BSofTofWSingleTCal2HitPar)
