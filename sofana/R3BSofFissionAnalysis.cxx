// ----------------------------------------------------------------------
// -----                                                            -----
// -----                     R3BSofFissionAnalysis                  -----
// -----             Created 14/02/21  by J.L. Rodriguez-Sanchez    -----
// ----------------------------------------------------------------------

#include "R3BSofFissionAnalysis.h"
#include "R3BSofMwpcHitData.h"
#include "R3BSofTofWHitData.h"

// R3BSofFissionAnalysis: Default Constructor --------------------------
R3BSofFissionAnalysis::R3BSofFissionAnalysis()
    : FairTask("R3BSof fission tracking Analysis", 1)
    , fOffsetAq(0)
    , fOffsetZ(0)
    , frho_Cave(0.)
    , fBfield_Glad(0.)
    , fMwpc0HitDataCA(NULL)
    , fMwpc1HitDataCA(NULL)
    , fMwpc2HitDataCA(NULL)
    , fMwpc3HitDataCA(NULL)
    , fTofWHitDataCA(NULL)
    , fTwimHitDataCA(NULL)
    , fTrackingDataCA(NULL)
    , fOnline(kFALSE)
{
}

// R3BSofFissionAnalysis: Standard Constructor --------------------------
R3BSofFissionAnalysis::R3BSofFissionAnalysis(const TString& name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fOffsetAq(0)
    , fOffsetZ(0)
    , frho_Cave(0.)
    , fBfield_Glad(0.)
    , fMwpc0HitDataCA(NULL)
    , fMwpc1HitDataCA(NULL)
    , fMwpc2HitDataCA(NULL)
    , fMwpc3HitDataCA(NULL)
    , fTofWHitDataCA(NULL)
    , fTwimHitDataCA(NULL)
    , fTrackingDataCA(NULL)
    , fOnline(kFALSE)
{
}

// Virtual R3BSofFissionAnalysis: Destructor
R3BSofFissionAnalysis::~R3BSofFissionAnalysis()
{
    LOG(INFO) << "R3BSofFissionAnalysis: Delete instance";
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
    if (fTwimHitDataCA)
    {
        delete fTwimHitDataCA;
    }
    if (fTrackingDataCA)
    {
        delete fTrackingDataCA;
    }
}

// -----   Public method SetParContainers   --------------------------------
void R3BSofFissionAnalysis::SetParContainers()
{
    // Parameter Container
    // Reading softrackingAnaPar from FairRuntimeDb
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    /*
    fFragPar = (R3BSofFragmentAnaPar*)rtdb->getContainer("soffragmentAnaPar");
    if (!fFragPar)
    {
        LOG(ERROR) << "R3BSofFragmentAnaPar::Init() Couldn't get handle on soffragmentAnaPar container";
    }
    */
}

// -----   Private method SetParameter   -----------------------------------
void R3BSofFissionAnalysis::SetParameter() {}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofFissionAnalysis::Init()
{
    LOG(INFO) << "R3BSofFissionAnalysis: Init tracking analysis at Cave-C";

    // INPUT DATA
    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

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
InitStatus R3BSofFissionAnalysis::ReInit()
{
    SetParContainers();
    SetParameter();
    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofFissionAnalysis::Exec(Option_t* option)
{
    // Reset entries in output arrays, local arrays
    Reset();

    Int_t nHitMwpc0 = fMwpc0HitDataCA->GetEntries();
    Int_t nHitMwpc1 = fMwpc1HitDataCA->GetEntries();
    Int_t nHitMwpc2 = fMwpc2HitDataCA->GetEntries();
    Int_t nHitMwpc3 = fMwpc3HitDataCA->GetEntries();
    Int_t nHitTofW = fTofWHitDataCA->GetEntries();

    R3BSofMwpcHitData** HitMwpc0 = new R3BSofMwpcHitData*[nHitMwpc0];
    R3BSofMwpcHitData** HitMwpc1 = new R3BSofMwpcHitData*[nHitMwpc1];
    R3BSofMwpcHitData** HitMwpc2 = new R3BSofMwpcHitData*[nHitMwpc2];
    R3BSofMwpcHitData** HitMwpc3 = new R3BSofMwpcHitData*[nHitMwpc3];
    R3BSofTofWHitData** HitTofW = new R3BSofTofWHitData*[nHitTofW];

    TVector3 pos;

    for (Int_t i = 0; i < nHitMwpc1; i++)
    {

        HitMwpc1[i] = (R3BSofMwpcHitData*)(fMwpc1HitDataCA->At(i));

        pos.SetXYZ(HitMwpc1[i]->GetX(), HitMwpc1[i]->GetY(), 0.);

        // std::cout<<i<<" "<< HitMwpc1[i]->GetX()<<" "<< HitMwpc1[i]->GetY()<<" "<< pos.Phi() <<std::endl;
    }

    for (Int_t i = 0; i < nHitMwpc2; i++)
    {

        HitMwpc2[i] = (R3BSofMwpcHitData*)(fMwpc2HitDataCA->At(i));

        pos.SetXYZ(HitMwpc2[i]->GetX(), HitMwpc2[i]->GetY(), 0.);

        // std::cout<<i<<" "<< HitMwpc2[i]->GetX() <<" "<< HitMwpc2[i]->GetY()<<" "<< pos.Phi() <<std::endl;
    }

    for (Int_t i = 0; i < nHitMwpc3; i++)
    {

        HitMwpc3[i] = (R3BSofMwpcHitData*)(fMwpc3HitDataCA->At(i));

        pos.SetXYZ(HitMwpc3[i]->GetX(), HitMwpc3[i]->GetY(), 0.);

        // std::cout<<i<<" "<< HitMwpc3[i]->GetX() <<" "<< HitMwpc3[i]->GetY()<<" "<< pos.Phi() <<std::endl;
    }

    for (Int_t i = 0; i < nHitTofW; i++)
    {

        HitTofW[i] = (R3BSofTofWHitData*)(fTofWHitDataCA->At(i));

        // std::cout<<i<<" "<< HitTofW[i]->GetPaddle() <<" "<< HitTofW[i]->GetY()<<" "<< HitTofW[i]->GetTof()
        // <<std::endl;
    }

    // AddData(fZ + fOffsetZ, fAq + fOffsetAq, Beta, Length, Brho_Cave, Paddle);

    if (HitMwpc0)
        delete HitMwpc0;
    if (HitMwpc1)
        delete HitMwpc1;
    if (HitMwpc2)
        delete HitMwpc2;
    if (HitMwpc3)
        delete HitMwpc3;
    return;
}

// -----   Private method for length  -----------------------------------------
Double_t R3BSofFissionAnalysis::GetLength(Double_t mw1, Double_t mw2, Double_t mw3)
{

    Double_t length = 0.;

    return length;
}

// -----   Private method for Brho  --------------------------------------------
Double_t R3BSofFissionAnalysis::GetBrho(Double_t mw1, Double_t mw2, Double_t mw3)
{

    Double_t brho = 0.;

    return brho;
}

// -----   Protected method Finish   ------------------------------------------
void R3BSofFissionAnalysis::Finish() {}

// -----   Protected method FinishEvent   -------------------------------------
void R3BSofFissionAnalysis::FinishEvent() {}

// -----   Public method Reset   ----------------------------------------------
void R3BSofFissionAnalysis::Reset()
{
    LOG(DEBUG) << "Clearing SofTrackingData Structure";
    if (fTrackingDataCA)
        fTrackingDataCA->Clear();
}

// -----   Private method AddData  --------------------------------------------
R3BSofTrackingData* R3BSofFissionAnalysis::AddData(Double_t z,
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

ClassImp(R3BSofFissionAnalysis);
