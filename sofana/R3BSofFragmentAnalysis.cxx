// ----------------------------------------------------------------------
// -----                                                            -----
// -----                     R3BSofFragmentAnalysis                 -----
// -----             Created 09/02/20  by J.L. Rodriguez-Sanchez    -----
// ----------------------------------------------------------------------

#include "R3BSofFragmentAnalysis.h"
#include "R3BMwpcHitData.h"
#include "R3BSofTofWHitData.h"
#include "R3BSofTrackingData.h"
#include "R3BTwimHitData.h"

Double_t const c = 29.9792458;

// R3BSofFragmentAnalysis: Default Constructor --------------------------
R3BSofFragmentAnalysis::R3BSofFragmentAnalysis()
    : FairTask("R3BSof Tracking Analysis", 1)
    , fExpId(0)
    , fOffsetAq(0)
    , fOffsetZ(0)
    //, fDist_mw3_tof(0.)
    //, fDist_start_glad(0.)
    , frho_Cave(0.)
    , fBfield_Glad(0.)
    , fTimeOffset(0.)
    , fTofWPos(560.) // 525: Centre
    , fMwpc0HitDataCA(NULL)
    , fMwpc1HitDataCA(NULL)
    , fMwpc2HitDataCA(NULL)
    , fMwpc3HitDataCA(NULL)
    , fTofWHitDataCA(NULL)
    , fMusicHitDataCA(NULL)
    , fTwimHitDataCA(NULL)
    , fTrackingDataCA(NULL)
    , fOnline(kFALSE)
{
}

// R3BSofFragmentAnalysisPar: Standard Constructor --------------------------
R3BSofFragmentAnalysis::R3BSofFragmentAnalysis(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fExpId(0)
    , fOffsetAq(0)
    , fOffsetZ(0)
    //, fDist_mw3_tof(0.)
    //, fDist_start_glad(0.)
    , frho_Cave(0.)
    , fBfield_Glad(0.)
    , fTimeOffset(0.)
    , fTofWPos(560.) // 525: Centre, 560: 50Ca setting in s467
    , fMwpc0HitDataCA(NULL)
    , fMwpc1HitDataCA(NULL)
    , fMwpc2HitDataCA(NULL)
    , fMwpc3HitDataCA(NULL)
    , fTofWHitDataCA(NULL)
    , fMusicHitDataCA(NULL)
    , fTwimHitDataCA(NULL)
    , fTrackingDataCA(NULL)
    , fOnline(kFALSE)
{
}

// Virtual R3BSofFragmentAnalysis: Destructor
R3BSofFragmentAnalysis::~R3BSofFragmentAnalysis()
{
    R3BLOG(INFO, "R3BSofFragmentAnalysis: Delete instance");
    if (fMwpc0HitDataCA)
    {
        delete fMwpc0HitDataCA;
    }
    if (fMwpc1HitDataCA)
    {
        delete fMwpc1HitDataCA;
    }
    if (fMwpc2HitDataCA)
    {
        delete fMwpc2HitDataCA;
    }
    if (fMwpc3HitDataCA)
    {
        delete fMwpc3HitDataCA;
    }
    if (fTofWHitDataCA)
    {
        delete fTofWHitDataCA;
    }
    if (fMusicHitDataCA)
    {
        delete fMusicHitDataCA;
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
    R3BLOG_IF(ERROR, !rtdb, "FairRuntimeDb not opened!");
    //
    fFragPar = (R3BSofFragmentAnaPar*)rtdb->getContainer("soffragmentAnaPar");
    R3BLOG_IF(ERROR, !fFragPar, "R3BSofFragmentAnaPar::Init() Couldn't get handle on soffragmentAnaPar container");
    // fFragPar->printParams();
    //
    // Getting Twim Parameters
    fTwimPar = (R3BTwimHitPar*)rtdb->getContainer("twimHitPar");
    R3BLOG_IF(ERROR, !fTwimPar, "Couldn't get handle on twimHitPar container");
    //
    // Geometries
    fMw0GeoPar = (R3BTGeoPar*)rtdb->getContainer("Mwpc0GeoPar");
    R3BLOG_IF(ERROR, !fMw0GeoPar, "Could not get access to Mwpc0GeoPar container.");

    fMw1GeoPar = (R3BTGeoPar*)rtdb->getContainer("Mwpc1GeoPar");
    R3BLOG_IF(ERROR, !fMw1GeoPar, "Could not get access to Mwpc1GeoPar container.");

    fMw2GeoPar = (R3BTGeoPar*)rtdb->getContainer("Mwpc2GeoPar");
    R3BLOG_IF(ERROR, !fMw2GeoPar, "Could not get access to Mwpc2GeoPar container.");

    fRoluGeoPar = (R3BTGeoPar*)rtdb->getContainer("RoluGeoPar");
    R3BLOG_IF(WARNING, !fRoluGeoPar, "Could not get access to RoluGeoPar container.");
}

void R3BSofFragmentAnalysis::SetParameter()
{
    //--- Parameter Container ---
    Int_t fNumSec = fTwimPar->GetNumSec();        // Number of Sections
    Int_t fNumAnodes = fTwimPar->GetNumAnodes();  // Number of anodes
    Int_t fNumParams = fTwimPar->GetNumParZFit(); // Number of TwimParameters

    // Anodes that don't work set to zero
    TArrayF* TwimCalZParams = new TArrayF();
    Int_t array_size = fNumSec * fNumParams;
    TwimCalZParams->Set(array_size);
    TwimCalZParams = fTwimPar->GetZHitPar(); // Array with the Cal parameters

    // Parameters detector
    for (Int_t s = 0; s < fNumSec; s++)
        // Parameters detector
        if (fNumParams == 2)
        {
            fTwimZ0 = TwimCalZParams->GetAt(0);
            fTwimZ1 = TwimCalZParams->GetAt(1);
        }
        else if (fNumParams == 3)
        {
            fTwimZ0 = TwimCalZParams->GetAt(0);
            fTwimZ1 = TwimCalZParams->GetAt(1);
            fTwimZ2 = TwimCalZParams->GetAt(2);
        }
        else
            R3BLOG(WARNING,
                   "R3BTwimCal2Hit parameters for charge-Z cannot be used here, number of parameters: " << fNumParams);
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofFragmentAnalysis::Init()
{
    R3BLOG(INFO, "R3BSofFragmentAnalysis: Init tracking analysis at Cave-C");

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    header = (R3BEventHeader*)rootManager->GetObject("EventHeader.");
    if (!header)
        header = (R3BEventHeader*)rootManager->GetObject("R3BEventHeader");
    fExpId = header->GetExpId();
    R3BLOG(INFO, "fExpId :" << fExpId);

    fMwpc0HitDataCA = (TClonesArray*)rootManager->GetObject("Mwpc0HitData");
    if (!fMwpc0HitDataCA)
    {
        return kFATAL;
    }

    fMwpc1HitDataCA = (TClonesArray*)rootManager->GetObject("Mwpc1HitData");
    if (!fMwpc1HitDataCA)
    {
        return kFATAL;
    }

    fMwpc2HitDataCA = (TClonesArray*)rootManager->GetObject("Mwpc2HitData");
    if (!fMwpc2HitDataCA)
    {
        return kFATAL;
    }

    fMwpc3HitDataCA = (TClonesArray*)rootManager->GetObject("Mwpc3HitData");
    if (!fMwpc3HitDataCA)
    {
        return kFATAL;
    }

    fMusicHitDataCA = (TClonesArray*)rootManager->GetObject("MusicHitData");
    if (!fMusicHitDataCA)
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
    rootManager->Register("SofTrackingData", "GLAD Tracking Analysis", fTrackingDataCA, !fOnline);

    if (fExpId == 444 || fExpId == 467)
    {
        // ROLU position stored as MWPC class
        fRoluPosDataCA = new TClonesArray("R3BMwpcHitData", 2);
        rootManager->Register("RoluPosData", "ROLU position", fRoluPosDataCA, !fOnline);
    }
    //
    ReInit();
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
    Double_t fZ = NAN, fE = NAN, fAq = NAN;
    Double_t Beta = NAN, Brho_Cave = NAN, Length = NAN;
    Double_t ToF_Cave = NAN, MusicTheta = NAN, TwimTheta = NAN;
    Double_t mw[4][4] = { { NAN } }; // mwpc[ID:0-4][x,y,a,b]
    Int_t Paddle = 0;

    Int_t nHitMwpc0 = fMwpc0HitDataCA->GetEntries();
    Int_t nHitMwpc1 = fMwpc1HitDataCA->GetEntries();
    Int_t nHitMwpc2 = fMwpc2HitDataCA->GetEntries();
    Int_t nHitMwpc3 = fMwpc3HitDataCA->GetEntries();
    Int_t nHitTofW = fTofWHitDataCA->GetEntries();
    Int_t nHitMusic = fMusicHitDataCA->GetEntries();
    Int_t nHitTwim = fTwimHitDataCA->GetEntries();
    HitTofW = new R3BSofTofWHitData*[nHitTofW];
    HitMusic = new R3BMusicHitData*[nHitMusic];
    HitTwim = new R3BTwimHitData*[nHitTwim];
    HitMwpc0 = new R3BMwpcHitData*[nHitMwpc0];
    HitMwpc1 = new R3BMwpcHitData*[nHitMwpc1];
    HitMwpc2 = new R3BMwpcHitData*[nHitMwpc2];
    HitMwpc3 = new R3BMwpcHitData*[nHitMwpc3];

    if (nHitMwpc0 < 1 || nHitMwpc1 < 1 || nHitMwpc2 < 1 || nHitMwpc3 < 1 || nHitTofW < 1 || nHitMusic < 1 ||
        nHitTwim < 1)
        return;

    for (Int_t i = 0; i < nHitMwpc0; i++)
    {
        HitMwpc0[i] = (R3BMwpcHitData*)(fMwpc0HitDataCA->At(i));
        mw[0][0] = HitMwpc0[i]->GetX() + fMw0GeoPar->GetPosX() * 10.; // mm
        mw[0][1] = HitMwpc0[i]->GetY() + fMw0GeoPar->GetPosY() * 10.; // mm
    }
    for (Int_t i = 0; i < nHitMwpc1; i++)
    {
        HitMwpc1[i] = (R3BMwpcHitData*)(fMwpc1HitDataCA->At(i));
        mw[1][0] = HitMwpc1[i]->GetX() + fMw1GeoPar->GetPosX() * 10.; // mm
        mw[1][1] = HitMwpc1[i]->GetY() + fMw1GeoPar->GetPosY() * 10.; // mm
    }
    for (Int_t i = 0; i < nHitMwpc2; i++)
    {
        HitMwpc2[i] = (R3BMwpcHitData*)(fMwpc2HitDataCA->At(i));
        mw[2][0] = HitMwpc2[i]->GetX() + fMw2GeoPar->GetPosX() * 10.; // mm
        mw[2][1] = HitMwpc2[i]->GetY() + fMw2GeoPar->GetPosY() * 10.; // mm
    }
    // Calculate raw angle /mm
    // mw[1][2] = mw[2][0] - mw[1][0];
    // mw[1][3] = mw[2][1] - mw[1][1];
    //
    for (Int_t i = 0; i < nHitMwpc3; i++)
    {
        HitMwpc3[i] = (R3BMwpcHitData*)(fMwpc3HitDataCA->At(i));
        mw[3][0] = HitMwpc3[i]->GetX();
        mw[3][1] = HitMwpc3[i]->GetY();
    }
    ////
    // Time from TofW
    for (Int_t i = 0; i < nHitTofW; i++)
    {
        HitTofW[i] = (R3BSofTofWHitData*)(fTofWHitDataCA->At(i));
        Paddle = HitTofW[i]->GetPaddle();
        if (fFragPar->GetInUse(Paddle) != 1)
            continue;
        ToF_Cave = HitTofW[i]->GetTof() - fFragPar->GetTofWOffset(Paddle);
        Beta = fFragPar->GetEffectivLength(Paddle) / ToF_Cave;
        Length = fFragPar->GetEffectivLength(Paddle) * 2.998e2; // in mm. 0th order approx. To be modified later.
        // std::cout <<" init: "<< HitTofW[i]->GetPaddle() << " "<< ToF_Cave << std::endl;
    }

    if (isnan(ToF_Cave))
        return;

    double gamma = 1. / sqrt(1. - Beta * Beta);

    // Getting Music angle
    for (Int_t ihit = 0; ihit < nHitMusic; ihit++)
    {
        HitMusic[ihit] = (R3BMusicHitData*)fMusicHitDataCA->At(ihit);
        if (!HitMusic[ihit])
            continue;
        // In case the MusicHitData container has several "realistic" values,
        // it's not possible to distinguish which is the "correct" event. Thus skipping events having several hits.
        if (TMath::Abs(MusicTheta) < 0.1)
            return;
        MusicTheta = HitMusic[ihit]->GetTheta();
    }
    //
    if (fExpId == 444 || fExpId == 467)
    {
        // Calculate estimated ROLU position
        Double_t rolux = mw[0][0] + MusicTheta * (fRoluGeoPar->GetPosZ() - fMw0GeoPar->GetPosZ()) * 10.; // mm
        Double_t roluy = mw[0][1] +
                         (mw[1][1] - mw[0][1]) / (fMw1GeoPar->GetPosZ() - fMw0GeoPar->GetPosZ()) *
                             (fRoluGeoPar->GetPosZ() - fMw0GeoPar->GetPosZ());
        AddRoluPos(rolux, roluy);
    }
    //
    // Z from twim-music ------------------------------------
    Double_t countz = 0;
    for (Int_t i = 0; i < nHitTwim; i++)
    {
        HitTwim[i] = (R3BTwimHitData*)(fTwimHitDataCA->At(i));
        if (HitTwim[i]->GetEave() > 1)
        {
            fE = HitTwim[i]->GetEave();
            TwimTheta = HitTwim[i]->GetTheta();
            countz++;
            fZ = fTwimZ0 + fTwimZ1 * TMath::Sqrt(fE) * Beta + fTwimZ2 * fE * Beta * Beta;
            HitTwim[i]->SetZcharge(fZ); // Upate Z
        }
    }
    //
    // Calculate brho and aoq
    Double_t x_in = (mw[1][0] + mw[2][0]) / 2.;
    Double_t theta_in = TwimTheta;
    Double_t x_out = mw[3][0];
    // Double_t theta_out = 0;
    if (fFragPar->GetNumBrhoParameters() < 4)
        return;
    Brho_Cave = fFragPar->GetBrhoParameter(0) + fFragPar->GetBrhoParameter(1) * x_in +
                fFragPar->GetBrhoParameter(2) * theta_in + fFragPar->GetBrhoParameter(3) * x_out;
    fAq = Brho_Cave / (3.10716 * Beta * gamma); //  m_u * c_0 / e = 3.107

    // Fill the data
    if (fZ > 1 && fAq > 1. && Brho_Cave > 0. && Beta > 0.)
        AddData(fZ + fOffsetZ, fAq + fOffsetAq, Beta, Length, Brho_Cave, Paddle);
    return;
}

// -----   Protected method Finish   --------------------------------------------
void R3BSofFragmentAnalysis::Finish() {}

// -----   Public method Reset also called by FinishEvent()   -------------------
void R3BSofFragmentAnalysis::Reset()
{
    R3BLOG(DEBUG, "Clearing SofTrackingData Structure");

    if (HitTofW)
        delete HitTofW;
    if (HitMwpc0)
        delete HitMwpc0;
    if (HitMwpc1)
        delete HitMwpc1;
    if (HitMwpc2)
        delete HitMwpc2;
    if (HitMwpc3)
        delete HitMwpc3;
    if (HitMusic)
        delete HitMusic;
    if (HitTwim)
        delete HitTwim;

    if (fTrackingDataCA)
        fTrackingDataCA->Clear();
    if (fRoluPosDataCA)
        fRoluPosDataCA->Clear();
}

// -----   Private method AddData  --------------------------------------------
R3BSofTrackingData* R3BSofFragmentAnalysis::AddData(Double_t z,
                                                    Double_t aq,
                                                    Double_t beta,
                                                    Double_t length,
                                                    Double_t brho,
                                                    Int_t paddle)
{
    // It fills the R3BSofTrackingData
    TClonesArray& clref = *fTrackingDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTrackingData(z, aq, beta, length, brho, paddle);
}

R3BMwpcHitData* R3BSofFragmentAnalysis::AddRoluPos(Double_t x, Double_t y)
{
    // It fills the R3BMwpcHitData
    TClonesArray& clref = *fRoluPosDataCA;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BMwpcHitData(x, y);
}
ClassImp(R3BSofFragmentAnalysis);
