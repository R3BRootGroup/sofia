// ---------------------------------------------------------------------
// -----         R3BSofTwimMapped2CalPar source file               -----
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

// Twim headers
#include "R3BSofMwpcHitData.h"
#include "R3BSofTwimCalPar.h"
#include "R3BSofTwimMapped2CalPar.h"
#include "R3BSofTwimMappedData.h"

#include <iomanip>

// R3BSofTwimMapped2CalPar: Default Constructor --------------------------
R3BSofTwimMapped2CalPar::R3BSofTwimMapped2CalPar()
    : FairTask("R3B Twim Angle Calibrator", 1)
    , fNumSec(MAX_NB_TWIMSEC)
    , fNumAnodes(MAX_NB_TWIMANODE)   // 16 anodes
    , fNumAnodesRef(MAX_NB_TWIMTREF) // 1 anode for TREF
    , fMaxMult(MAX_MULT_TWIM_CAL)
    , fMinStadistics(1000)
    , fLimit_left(0)
    , fLimit_right(24000)
    , fNumParams(3)
    , fNumPosParams(2)
    , fMaxSigma(200)
    , CalParams(NULL)
    , PosParams(NULL)
    , fCal_Par(NULL)
    , fNameDetA("Mwpc1")
    , fPosMwpcA(0.)
    , fNameDetB("Mwpc2")
    , fPosMwpcB(0.)
    , fPosTwim(0.)
    , fTwimMappedDataCA(NULL)
    , fHitItemsMwpcA(NULL)
    , fHitItemsMwpcB(NULL)
{
}

// R3BSofTwimMapped2CalParPar: Standard Constructor --------------------------
R3BSofTwimMapped2CalPar::R3BSofTwimMapped2CalPar(const TString& name,
                                                 Int_t iVerbose,
                                                 const TString& namedeta,
                                                 const TString& namedetb)
    : FairTask(name, iVerbose)
    , fNumSec(MAX_NB_TWIMSEC)
    , fNumAnodes(MAX_NB_TWIMANODE)   // 16 anodes
    , fNumAnodesRef(MAX_NB_TWIMTREF) // 1 anode for TREF
    , fMaxMult(MAX_MULT_TWIM_CAL)
    , fMinStadistics(1000)
    , fLimit_left(0)
    , fLimit_right(24000)
    , fNumParams(3)
    , fNumPosParams(2)
    , fMaxSigma(200)
    , CalParams(NULL)
    , PosParams(NULL)
    , fCal_Par(NULL)
    , fNameDetA(namedeta)
    , fPosMwpcA(0.)
    , fNameDetB(namedetb)
    , fPosMwpcB(0.)
    , fPosTwim(0.)
    , fTwimMappedDataCA(NULL)
    , fHitItemsMwpcA(NULL)
    , fHitItemsMwpcB(NULL)
{
}

// Virtual R3BSofTwimMapped2CalPar: Destructor
R3BSofTwimMapped2CalPar::~R3BSofTwimMapped2CalPar()
{
    LOG(INFO) << "R3BSofTwimMapped2CalPar: Delete instance";
    if (fTwimMappedDataCA)
        delete fTwimMappedDataCA;
    if (fHitItemsMwpcA)
        delete fHitItemsMwpcA;
    if (fHitItemsMwpcB)
        delete fHitItemsMwpcB;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTwimMapped2CalPar::Init()
{
    LOG(INFO) << "R3BSofTwimMapped2CalPar: Init";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fTwimMappedDataCA = (TClonesArray*)rootManager->GetObject("TwimMappedData");
    if (!fTwimMappedDataCA)
    {
        LOG(ERROR) << "R3BSofTwimMapped2CalPar: TwimMappedData not found";
        return kFATAL;
    }

    // get access to hit data of mwpcs
    fHitItemsMwpcA = (TClonesArray*)rootManager->GetObject(fNameDetA + "HitData");
    if (!fHitItemsMwpcA)
    {
        LOG(ERROR) << "R3BSofTwimMapped2CalPar: " + fNameDetA + "HitData not found";
        return kFATAL;
    }

    fHitItemsMwpcB = (TClonesArray*)rootManager->GetObject(fNameDetB + "HitData");
    if (!fHitItemsMwpcB)
    {
        LOG(ERROR) << "R3BSofTwimMapped2CalPar: " + fNameDetB + "HitData not found";
        return kFATAL;
    }

    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        return kFATAL;
    }

    fCal_Par = (R3BSofTwimCalPar*)rtdb->getContainer("twimCalPar");
    if (!fCal_Par)
    {
        LOG(ERROR) << "R3BSofTwimMapped2CalPar:: Couldn't get handle on twimCalPar container";
        return kFATAL;
    }

    // Define TGraph for fits
    char Name1[255];
    fg_anode = new TGraph*[fNumSec * fNumAnodes];
    for (Int_t s = 0; s < fNumSec; s++)
        for (Int_t i = 0; i < fNumAnodes; i++)
        {
            fg_anode[s * fNumAnodes + i] = new TGraph();
            sprintf(Name1, "fg_sec%d_Anode_%d", s + 1, i + 1);
            fg_anode[s * fNumAnodes + i]->SetName(Name1);
            fg_anode[s * fNumAnodes + i]->SetTitle(Name1);
            fg_anode[s * fNumAnodes + i]->SetFillColor(1);
            fg_anode[s * fNumAnodes + i]->SetLineColor(0);
            fg_anode[s * fNumAnodes + i]->SetMarkerColor(4);
            fg_anode[s * fNumAnodes + i]->SetMarkerStyle(20);
            fg_anode[s * fNumAnodes + i]->SetMarkerSize(1.2);
        }

    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofTwimMapped2CalPar::ReInit() { return kSUCCESS; }

// -----   Public method Execution   --------------------------------------------
void R3BSofTwimMapped2CalPar::Exec(Option_t* option)
{
    // Reading the Input -- Mapped Data --
    Int_t nHits = fTwimMappedDataCA->GetEntries();
    Int_t nHitsA = fHitItemsMwpcA->GetEntries();
    Int_t nHitsB = fHitItemsMwpcB->GetEntries();
    if (nHits < 3 || nHitsA != 1 || nHitsB != 1)
        return;

    TVector3 PosMwpcA(0., 0., fPosMwpcA);
    TVector3 PosMwpcB(0., 0., fPosMwpcB);
    R3BSofMwpcHitData** hitMwAData = new R3BSofMwpcHitData*[nHitsA];
    for (Int_t i = 0; i < nHitsA; i++)
    {
        hitMwAData[i] = (R3BSofMwpcHitData*)(fHitItemsMwpcA->At(i));
        PosMwpcA.SetX(hitMwAData[i]->GetX());
        // LOG(INFO) <<hitMwAData[i]->GetX();
    }
    R3BSofMwpcHitData** hitMwBData = new R3BSofMwpcHitData*[nHitsB];
    for (Int_t i = 0; i < nHitsB; i++)
    {
        hitMwBData[i] = (R3BSofMwpcHitData*)(fHitItemsMwpcB->At(i));
        PosMwpcB.SetX(hitMwBData[i]->GetX());
        // LOG(INFO) <<hitMwBData[i]->GetX();
    }

    R3BSofTwimMappedData** mappedData = new R3BSofTwimMappedData*[nHits];
    Int_t secId = 0;
    Int_t anodeId = 0;

    for (Int_t s = 0; s < fNumSec; s++)
        for (Int_t i = 0; i < (fNumAnodes + fNumAnodesRef); i++)
        {
            mulanode[s][i] = 0;
            for (Int_t j = 0; j < fMaxMult; j++)
            {
                fE[s][j][i] = 0.;
                fDT[s][j][i] = 0.;
            }
        }

    for (Int_t i = 0; i < nHits; i++)
    {
        mappedData[i] = (R3BSofTwimMappedData*)(fTwimMappedDataCA->At(i));
        secId = mappedData[i]->GetSecID();
        anodeId = mappedData[i]->GetAnodeID();

        if (anodeId < fNumAnodes)
        {

            fE[secId][mulanode[secId][anodeId]][anodeId] = mappedData[i]->GetEnergy();
            fDT[secId][mulanode[secId][anodeId]][anodeId] = mappedData[i]->GetTime();
            mulanode[secId][anodeId]++;
        }
        else if (anodeId >= fNumAnodes)
        {                                                                             // Ref. Time
            fDT[secId][mulanode[secId][anodeId]][anodeId] = mappedData[i]->GetTime(); // Ref. Time
            mulanode[secId][anodeId]++;
        }
    }

    // Fill data only if there is TREF signal
    for (Int_t s = 0; s < fNumSec; s++)
        if (mulanode[s][fNumAnodes] == 1)
        {
            TF1* fa = new TF1("fa", "pol1", fPosMwpcA, fPosMwpcB);
            fa->SetParameter(0, PosMwpcA.X());
            fa->SetParameter(1, (PosMwpcB - PosMwpcA).X() / (fPosMwpcB - fPosMwpcA));
            for (Int_t i = 0; i < fNumAnodes; i++)
            {
                for (Int_t j = 0; j < mulanode[s][fNumAnodes]; j++)
                    for (Int_t k = 0; k < mulanode[s][i]; k++)
                    {
                        if (fE[s][k][i] > 0.)
                        { // Anode is 25mm, first anode is at -187.5mm with respect to the center of twim detector
                            fg_anode[s * fNumAnodes + i]->SetPoint(fg_anode[s * fNumAnodes + i]->GetN() + 1,
                                                                   fDT[s][k][i] - fDT[s][j][fNumAnodes],
                                                                   fa->Eval(fPosTwim - 187.5 + i * 25.0));
                        }
                    }
            }
        }
    if (mappedData)
        delete mappedData;
    if (hitMwAData)
        delete hitMwAData;
    if (hitMwBData)
        delete hitMwBData;
    return;
}

// -----   Protected method Finish   --------------------------------------------
void R3BSofTwimMapped2CalPar::FinishEvent() {}

// -----   Public method Reset   ------------------------------------------------
void R3BSofTwimMapped2CalPar::Reset() {}

void R3BSofTwimMapped2CalPar::FinishTask()
{
    fCal_Par->SetNumSec(fNumSec);
    fCal_Par->SetNumAnodes(fNumAnodes);
    fCal_Par->SetNumParamsEFit(fNumParams);
    fCal_Par->SetNumParamsPosFit(fNumPosParams);
    fCal_Par->GetAnodeCalParams()->Set(fNumSec * fNumParams * fNumAnodes);
    fCal_Par->GetPosParams()->Set(fNumSec * fNumPosParams * fNumAnodes);

    TF1* fit = new TF1("fit", "pol1", fLimit_left, fLimit_right);
    fit->SetLineColor(2);
    for (Int_t s = 0; s < fNumSec; s++)
        for (Int_t i = 0; i < fNumAnodes; i++)
        {
            if (fg_anode[s * fNumAnodes + i]->GetN() > fMinStadistics)
            {
                fCal_Par->SetInUse(1, s + 1, i + 1);
                fg_anode[s * fNumAnodes + i]->Fit("fit", "QR0");
                Double_t par[fNumPosParams];
                fit->GetParameters(&par[0]);
                fCal_Par->SetPosParams(par[0], i * fNumPosParams + s * fNumAnodes * fNumPosParams);
                fCal_Par->SetPosParams(par[1], i * fNumPosParams + s * fNumAnodes * fNumPosParams + 1);
            }
            else
                fCal_Par->SetAnodeCalParams(-1.0, i * fNumParams + s * fNumAnodes * fNumPosParams + 1);

            fg_anode[s * fNumAnodes + i]->Write();
        }
    fCal_Par->setChanged();
}

ClassImp(R3BSofTwimMapped2CalPar)
