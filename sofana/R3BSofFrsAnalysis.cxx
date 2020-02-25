// ----------------------------------------------------------------------
// -----                                                            -----
// -----                     R3BSofFrsAnalysis                      -----
// -----             Created 23/01/20  by J.L. Rodriguez-Sanchez    -----
// ----------------------------------------------------------------------

// ROOT headers
#include "TClonesArray.h"
#include "TF1.h"
#include "TMath.h"
#include "TRandom.h"

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include <iomanip>

// R3B headers
#include "R3BMusicHitData.h"

// SOFIA headers
#include "R3BSofFrsAnaPar.h"
#include "R3BSofFrsAnalysis.h"
#include "R3BSofFrsData.h"
#include "R3BSofMwpcHitData.h"
#include "R3BSofSciHitData.h"

Double_t const c = 29.9792458; // Light velocity

// R3BSofFrsAnalysis: Default Constructor --------------------------
R3BSofFrsAnalysis::R3BSofFrsAnalysis()
    : FairTask("R3B-Sof Analysis for FRS", 1)
    , fOffsetAq(0)
    , fOffsetZ(0)
    , fFrs_Par(NULL)
    , fMwpcHitDataCA(NULL)
    , fSciHitDataCA(NULL)
    , fMusicHitDataCA(NULL)
    , fFrsDataCA(NULL)
    , fOnline(kFALSE)
{
}

// R3BSofFrsAnalysisPar: Standard Constructor --------------------------
R3BSofFrsAnalysis::R3BSofFrsAnalysis(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fOffsetAq(0)
    , fOffsetZ(0)
    , fFrs_Par(NULL)
    , fMwpcHitDataCA(NULL)
    , fSciHitDataCA(NULL)
    , fMusicHitDataCA(NULL)
    , fFrsDataCA(NULL)
    , fOnline(kFALSE)
{
}

// Virtual R3BSofFrsAnalysis: Destructor
R3BSofFrsAnalysis::~R3BSofFrsAnalysis()
{
    LOG(INFO) << "R3BSofFrsAnalysis: Delete instance";
    if (fMwpcHitDataCA)
    {
        delete fMwpcHitDataCA;
    }
    if (fSciHitDataCA)
    {
        delete fSciHitDataCA;
    }
    if (fMusicHitDataCA)
    {
        delete fMusicHitDataCA;
    }
    if (fFrsDataCA)
    {
        delete fFrsDataCA;
    }
}

void R3BSofFrsAnalysis::SetParContainers()
{
    // Parameter Container
    // Reading soffrsAnaPar from FairRuntimeDb
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        LOG(ERROR) << "FairRuntimeDb not opened!";
    }

    fFrs_Par = (R3BSofFrsAnaPar*)rtdb->getContainer("soffrsAnaPar");
    if (!fFrs_Par)
    {
        LOG(ERROR) << "R3BSofFrsAnalysisPar:: Couldn't get handle on soffrsAnaPar container";
    }
    else
    {
        LOG(INFO) << "R3BSofFrsAnalysisPar:: soffrsAnaPar container open";
    }
}

void R3BSofFrsAnalysis::SetParameter()
{
    //--- Parameter Container ---
    fMagS2Cave = fFrs_Par->GetMagnificationS2Cave();
    fDispS2 = fFrs_Par->GetDispersionS2();
    fPathS2Cave = fFrs_Par->GetPathS2Cave();
    fTOFS2Cave = fFrs_Par->GetTOFS2Cave();
    frho_S0_S2 = fFrs_Par->GetRhoS0S2();
    frho_S2_Cave = fFrs_Par->GetRhoS2Cave();
    fBfield_S0_S2 = fFrs_Par->GetBfiedS0S2();
    fBfield_S2_Cave = fFrs_Par->GetBfiedS2Cave();
    fPosFocalS2 = fFrs_Par->GetPosFocalS2();
    fPosFocalCave = fFrs_Par->GetPosFocalCave();

    LOG(INFO) << "R3BSofFrsAnalysis: Dispersion (S2): " << fDispS2 << " [m/per-cent]";
    LOG(INFO) << "R3BSofFrsAnalysis: Magnification (S2-Cave): " << fMagS2Cave;
    LOG(INFO) << "R3BSofFrsAnalysis: Path length (S2-Cave): " << fPathS2Cave << " [cm]";
    LOG(INFO) << "R3BSofFrsAnalysis: ToF correction (S2-Cave): " << fTOFS2Cave << " [ns]";
    LOG(INFO) << "R3BSofFrsAnalysis: Rho (S0-S2): " << frho_S0_S2;
    LOG(INFO) << "R3BSofFrsAnalysis: B (S0-S2): " << fBfield_S0_S2;
    LOG(INFO) << "R3BSofFrsAnalysis: Rho (S2-Cave-C): " << frho_S2_Cave;
    LOG(INFO) << "R3BSofFrsAnalysis: B (S2-Cave-C): " << fBfield_S2_Cave;
    LOG(INFO) << "R3BSofFrsAnalysis: Pos for focal plane at S2[mm]: " << fPosFocalS2;
    LOG(INFO) << "R3BSofFrsAnalysis: Pos for focal plane at Cave-C[mm]: " << fPosFocalCave;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofFrsAnalysis::Init()
{
    LOG(INFO) << "R3BSofFrsAnalysis: Init FRS analysis from S2 to Cave-C";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    fMwpcHitDataCA = (TClonesArray*)rootManager->GetObject("Mwpc0HitData");
    if (!fMwpcHitDataCA)
    {
        return kFATAL;
    }

    fMusicHitDataCA = (TClonesArray*)rootManager->GetObject("MusicHitData");
    if (!fMusicHitDataCA)
    {
        return kFATAL;
    }

    fSciHitDataCA = (TClonesArray*)rootManager->GetObject("SofSciHitData");
    if (!fSciHitDataCA)
    {
        return kFATAL;
    }

    // OUTPUT DATA
    fFrsDataCA = new TClonesArray("R3BSofFrsData", 5);
    if (!fOnline)
    {
        rootManager->Register("SofFrsData", "Analysis FRS", fFrsDataCA, kTRUE);
    }
    else
    {
        rootManager->Register("SofFrsData", "Analysis FRS", fFrsDataCA, kFALSE);
    }
    SetParameter();
    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofFrsAnalysis::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofFrsAnalysis::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    Double_t fZ = 0., fAq = 0.;

    Int_t nHitMwpc = fMwpcHitDataCA->GetEntries();
    Int_t nHitSci = fSciHitDataCA->GetEntries();
    Int_t nHitMusic = fMusicHitDataCA->GetEntries();
    // LOG(INFO) << nHitMusic << " " << nHitSci << " " << nHitMwpc ;
    if (nHitMwpc < 1 || nHitSci < 1 || nHitMusic < 1)
        return; // FIXME:include here warning!

    R3BSofSciHitData** HitSci = new R3BSofSciHitData*[nHitSci];
    R3BMusicHitData** HitMusic = new R3BMusicHitData*[nHitMusic];
    R3BSofMwpcHitData** HitMwpc = new R3BSofMwpcHitData*[nHitMwpc];

    // Z from r3b-music ------------------------------------
    Double_t countz = 0;
    for (Int_t i = 0; i < nHitMusic; i++)
    {
        HitMusic[i] = (R3BMusicHitData*)(fMusicHitDataCA->At(i));
        if (HitMusic[i]->GetZcharge() > 1)
        {
            fZ = fZ + HitMusic[i]->GetZcharge();
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
    double x_pos_s2 = 0.;
    double x_pos_cave = 0.;
    double ToF_S2_Cave = 0.;
    // Position at Cave-C
    for (Int_t i = 0; i < nHitMwpc; i++)
    {
        HitMwpc[i] = (R3BSofMwpcHitData*)(fMwpcHitDataCA->At(i));
        x_pos_cave = HitMwpc[i]->GetX();
    }
    // FIXME: This could need hit level from SCI
    for (Int_t i = 0; i < nHitSci; i++)
    {
        HitSci[i] = (R3BSofSciHitData*)(fSciHitDataCA->At(i));
        if (HitSci[i]->GetSciId() == 2) // Sci at S2
            x_pos_s2 = HitSci[i]->GetX();
        else
        {
            ToF_S2_Cave = HitSci[i]->GetTof(); // Sci at Cave-C
        }
    }

    // Velocity
    double Beta_S2_Cave = (fPathS2Cave / (fTOFS2Cave + ToF_S2_Cave)) / c;
    double Gamma_S2_Cave = 1. / (TMath::Sqrt(1. - (Beta_S2_Cave) * (Beta_S2_Cave)));

    // Brho and A/q
    // double deltax =  (x_pos_s2 / 100.) / fDispS2;
    double deltax = (x_pos_s2 * fMagS2Cave - x_pos_cave) / (fDispS2 * 1000.);
    double Brho_Cave = fBfield_S2_Cave * frho_S2_Cave * (1.0 + deltax);
    fAq = Brho_Cave / (3.10716 * Gamma_S2_Cave * Beta_S2_Cave);

    // Fill the data
    if (fZ > 1 && fAq > 0. && Brho_Cave > 0.)
        AddData(fZ + fOffsetZ, fAq + fOffsetAq, Beta_S2_Cave, Brho_Cave, x_pos_s2, x_pos_cave);

    if (HitSci)
        delete HitSci;
    if (HitMwpc)
        delete HitMwpc;
    if (HitMusic)
        delete HitMusic;
    return;
}

// -----   Protected method Finish   --------------------------------------------
void R3BSofFrsAnalysis::Finish() {}

// -----   Public method Reset   ------------------------------------------------
void R3BSofFrsAnalysis::Reset()
{
    LOG(DEBUG) << "Clearing FrsData Structure";
    if (fFrsDataCA)
        fFrsDataCA->Clear();
}

// -----   Private method AddData  --------------------------------------------
R3BSofFrsData* R3BSofFrsAnalysis::AddData(Double_t z,
                                          Double_t aq,
                                          Double_t beta,
                                          Double_t brho,
                                          Double_t xs2,
                                          Double_t xc)
{
    // It fills the R3BSofFrsData
    TClonesArray& clref = *fFrsDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofFrsData(z, aq, beta, brho, xs2, xc);
}
