// ----------------------------------------------------------------------
// -----                                                            -----
// -----                     R3BSofFragmentAnalysis                 -----
// -----             Created 09/02/20  by J.L. Rodriguez-Sanchez    -----
// ----------------------------------------------------------------------

// ROOT headers
#include "TClonesArray.h"
#include "TF1.h"
#include "TMath.h"
#include "TRandom.h"
#include "TVector3.h"

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include <iomanip>

// SOFIA headers
#include "R3BSofFragmentAnalysis.h"
#include "R3BSofMwpcHitData.h"
#include "R3BSofTofWHitData.h"
#include "R3BSofTrackingData.h"
#include "R3BSofTwimHitData.h"

Double_t const c = 29.9792458;
TVector3 v1;

// R3BSofFragmentAnalysis: Default Constructor --------------------------
R3BSofFragmentAnalysis::R3BSofFragmentAnalysis()
    : FairTask("R3BSof Tracking Analysis", 1)
    , fOffsetAq(0)
    , fOffsetZ(0)
    , fDist_mw3_tof(0.)
    , fDist_start_glad(0.)
    , fMwpcHitDataCA(NULL)
    , fTofWHitDataCA(NULL)
    , fTwimHitDataCA(NULL)
    , fTrackingDataCA(NULL)
    , fOnline(kFALSE)
{
}

// R3BSofFragmentAnalysisPar: Standard Constructor --------------------------
R3BSofFragmentAnalysis::R3BSofFragmentAnalysis(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fOffsetAq(0)
    , fOffsetZ(0)
    , fDist_mw3_tof(0.)
    , fDist_start_glad(0.)
    , fMwpcHitDataCA(NULL)
    , fTofWHitDataCA(NULL)
    , fTwimHitDataCA(NULL)
    , fTrackingDataCA(NULL)
    , fOnline(kFALSE)
{
}

// Virtual R3BSofFragmentAnalysis: Destructor
R3BSofFragmentAnalysis::~R3BSofFragmentAnalysis()
{
    LOG(INFO) << "R3BSofFragmentAnalysis: Delete instance";
    if (fMwpcHitDataCA)
    {
        delete fMwpcHitDataCA;
    }
    if (fTofWHitDataCA)
    {
        delete fTofWHitDataCA;
    }
    if (fTwimHitDataCA)
    {
        delete fTwimHitDataCA;
    }
    if (fTrackingDataCA)
    {
        delete fTrackingDataCA;
    }
}

void R3BSofFragmentAnalysis::SetParContainers()
{
    // Parameter Container
    // Reading softrackingAnaPar from FairRuntimeDb
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        LOG(ERROR) << "FairRuntimeDb not opened!";
    }
}

void R3BSofFragmentAnalysis::SetParameter()
{
    //--- Parameter Container ---
    frho_Cave = 7.0;
    fBfield_Glad = 4.0;
    fDist_mw3_tof = 72.0;
    fDist_start_glad = 65.5 + 163.4 + 118.;
    LOG(INFO) << "R3BSofFragmentAnalysis: Rho (Cave): " << frho_Cave;
    LOG(INFO) << "R3BSofFragmentAnalysis: B (Cave): " << fBfield_Glad;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofFragmentAnalysis::Init()
{
    LOG(INFO) << "R3BSofFragmentAnalysis: Init tracking analysis at Cave-C";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fMwpcHitDataCA = (TClonesArray*)rootManager->GetObject("Mwpc3HitData");
    if (!fMwpcHitDataCA)
    {
        return kFATAL;
    }

    fTwimHitDataCA = (TClonesArray*)rootManager->GetObject("TwimHitData");
    if (!fTwimHitDataCA)
    {
        return kFATAL;
    }

    fTofWHitDataCA = (TClonesArray*)rootManager->GetObject("TofWHitData");
    if (!fTofWHitDataCA)
    {
        return kFATAL;
    }

    // OUTPUT DATA
    fTrackingDataCA = new TClonesArray("R3BSofTrackingData", 2);
    if (!fOnline)
    {
        rootManager->Register("SofTrackingData", "GLAD Tracking Analysis", fTrackingDataCA, kTRUE);
    }
    else
    {
        rootManager->Register("SofTrackingData", "GLAD Tracking Analysis", fTrackingDataCA, kFALSE);
    }
    SetParameter();
    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofFragmentAnalysis::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofFragmentAnalysis::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    Double_t fZ = 0., fAq = 0.;
    Double_t Beta = 0., Brho_Cave = 0., Length = 0.;

    Int_t nHitMwpc = fMwpcHitDataCA->GetEntries();
    Int_t nHitTofW = fTofWHitDataCA->GetEntries();
    Int_t nHitTwim = fTwimHitDataCA->GetEntries();
    // LOG(INFO) << nHitMusic << " " << nHitSci << " " << nHitMwpc ;
    if (nHitMwpc < 1 || nHitTofW < 1 || nHitTwim < 1)
        return;

    R3BSofTofWHitData** HitTofW = new R3BSofTofWHitData*[nHitTofW];
    R3BSofTwimHitData** HitTwim = new R3BSofTwimHitData*[nHitTwim];
    R3BSofMwpcHitData** HitMwpc = new R3BSofMwpcHitData*[nHitMwpc];

    // Z from twim-music ------------------------------------
    Double_t countz = 0;
    for (Int_t i = 0; i < nHitTwim; i++)
    {
        HitTwim[i] = (R3BSofTwimHitData*)(fTwimHitDataCA->At(i));
        if (HitTwim[i]->GetZcharge() > 1)
        {
            fZ = fZ + HitTwim[i]->GetZcharge();
            countz++;
        }
    }
    if (countz > 0)
    {
        fZ = fZ / countz;
    }
    else
    {
        fZ = 0.;
    }

    // Added initial model
    Double_t ToF_Cave = 0.;
    Double_t mw3_x = 0., mw3_z = 0.;
    // Position at Cave-C behind GLAD: MWPC3
    for (Int_t i = 0; i < nHitMwpc; i++)
    {
        HitMwpc[i] = (R3BSofMwpcHitData*)(fMwpcHitDataCA->At(i));
        mw3_x = 243. - HitMwpc[i]->GetX() / 10. * cos(29. * TMath::DegToRad());         // cm
        mw3_z = 689. + HitMwpc[i]->GetX() / 10. * sin(29. * TMath::DegToRad()) - 163.4; // cm
    }
    // Time from TofW
    for (Int_t i = 0; i < nHitTofW; i++)
    {
        HitTofW[i] = (R3BSofTofWHitData*)(fTofWHitDataCA->At(i));
        ToF_Cave = HitTofW[i]->GetTime();
    }

    if (ToF_Cave > 0. && mw3_z > 0. && mw3_x < 0.)
    {

        v1.SetXYZ(mw3_x, 0., mw3_z);

        double rho = 94.6 / (2. * sin(v1.Theta() / 2.) * cos(14. * TMath::DegToRad() - v1.Theta() / 2.));
        Brho_Cave = 4.0 * 0.8 * rho * 0.01;
        Length = fDist_start_glad + sqrt(mw3_x * mw3_x + mw3_z * mw3_z) + fDist_mw3_tof;
        double vel = Length / ToF_Cave;
        Beta = vel / c;
        double gamma = 1. / sqrt(1. - Beta * Beta);
        fAq = Brho_Cave / (3.10716 * Beta * gamma);

        // Fill the data
        if (fZ > 1 && fAq > 1. && Brho_Cave > 0. && Beta > 0.)
            AddData(fZ + fOffsetZ, fAq + fOffsetAq, Beta, Length, Brho_Cave);
    }

    if (HitTofW)
        delete HitTofW;
    if (HitMwpc)
        delete HitMwpc;
    if (HitTwim)
        delete HitTwim;
    return;
}

// -----   Protected method Finish   --------------------------------------------
void R3BSofFragmentAnalysis::Finish() {}

// -----   Public method Reset   ------------------------------------------------
void R3BSofFragmentAnalysis::Reset()
{
    LOG(DEBUG) << "Clearing SofTrackingData Structure";
    if (fTrackingDataCA)
        fTrackingDataCA->Clear();
}

// -----   Private method AddData  --------------------------------------------
R3BSofTrackingData* R3BSofFragmentAnalysis::AddData(Double_t z,
                                                    Double_t aq,
                                                    Double_t beta,
                                                    Double_t length,
                                                    Double_t brho)
{
    // It fills the R3BSofTrackingData
    TClonesArray& clref = *fTrackingDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTrackingData(z, aq, beta, length, brho);
}
