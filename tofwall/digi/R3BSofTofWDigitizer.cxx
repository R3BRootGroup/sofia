// ----------------------------------------------------------------
// -----          R3BSofTofWDigitizer source file             -----
// -----        Created 03/11/19 by J.L. Rodriguez            -----
// ----------------------------------------------------------------

#include "R3BSofTofWDigitizer.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "R3BLogger.h"
#include "R3BMCTrack.h"
#include "R3BSofTofWPoint.h"
#include "R3BTGeoPar.h"
#include "TClonesArray.h"
#include "TMath.h"
#include "TVector3.h"

// R3BSofTofWDigitizer: Default Constructor --------------------------
R3BSofTofWDigitizer::R3BSofTofWDigitizer()
    : FairTask("R3BSofTofWDigitizer", 1)
    , fMCTrack(NULL)
    , fTofPoints(NULL)
    , fTofHits(NULL)
    , fsigma_y(1)     // sigma=1mm
    , fsigma_t(0.017) // sigma=17ps
    , fsigma_ELoss(0.)
{
    rand = new TRandom3();
}

// R3BSofTofWDigitizer: Standard Constructor --------------------------
R3BSofTofWDigitizer::R3BSofTofWDigitizer(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fMCTrack(NULL)
    , fTofPoints(NULL)
    , fTofHits(NULL)
    , fsigma_y(1)
    , fsigma_t(0.017)
    , fsigma_ELoss(0.)
{
    rand = new TRandom3();
}

// Virtual R3BSofTofWDigitizer: Destructor ----------------------------
R3BSofTofWDigitizer::~R3BSofTofWDigitizer()
{
    R3BLOG(debug, "");
    if (fTofHits)
    {
        delete fTofHits;
    }
}

void R3BSofTofWDigitizer::SetParContainers()
{
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    fTofWGeoPar = (R3BTGeoPar*)rtdb->getContainer("TofwGeoPar");
    if (!fTofWGeoPar)
    {
        R3BLOG(error, "Could not get access to TofwGeoPar container.");
        return;
    }
    else
    {
        R3BLOG(info, "Container TofwGeoPar found.");
    }
}

void R3BSofTofWDigitizer::SetParameter()
{
    fsigma_y = fTofWGeoPar->GetSigmaY();

    fRot.RotateX(-fTofWGeoPar->GetRotX() * TMath::DegToRad());
    fRot.RotateY(-fTofWGeoPar->GetRotY() * TMath::DegToRad());
    fRot.RotateZ(-fTofWGeoPar->GetRotZ() * TMath::DegToRad());

    fTrans.SetXYZ(fTofWGeoPar->GetPosX(), fTofWGeoPar->GetPosY(), fTofWGeoPar->GetPosZ());
}

// ----   Public method Init  -----------------------------------------
InitStatus R3BSofTofWDigitizer::Init()
{
    R3BLOG(info, "");

    // Get input array
    FairRootManager* ioman = FairRootManager::Instance();
    R3BLOG_IF(fatal, !ioman, "FairRootManager not found.");

    fMCTrack = (TClonesArray*)ioman->GetObject("MCTrack");
    fTofPoints = (TClonesArray*)ioman->GetObject("SofTofWPoint");

    // Register output array fTofHits
    fTofHits = new TClonesArray("R3BSofTofWHitData");
    ioman->Register("TofWHitData", "Digital response in TofW", fTofHits, kTRUE);

    SetParameter();
    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofTofWDigitizer::Exec(Option_t* opt)
{
    Reset();
    // Reading the Input -- Point Data --
    Int_t nHits = fTofPoints->GetEntries();
    if (nHits == 0)
    {
        return;
    }
    // Data from Point level
    R3BSofTofWPoint** pointData = new R3BSofTofWPoint*[nHits];
    Int_t paddle = 0;
    Int_t TrackId = 0, PID = 0, mother = -1;
    Double_t x = 0., y = 0., z = 0., time = 0.;
    TVector3 vpos;
    for (Int_t i = 0; i < nHits; i++)
    {
        pointData[i] = (R3BSofTofWPoint*)(fTofPoints->At(i));
        TrackId = pointData[i]->GetTrackID();

        R3BMCTrack* Track = (R3BMCTrack*)fMCTrack->At(TrackId);
        PID = Track->GetPdgCode();

        if (PID > 1000080160) // Z=8 and A=16
        {
            Double_t fX_in = pointData[i]->GetXIn();
            Double_t fY_in = pointData[i]->GetYIn();
            Double_t fZ_in = pointData[i]->GetZIn();
            Double_t fX_out = pointData[i]->GetXOut();
            Double_t fY_out = pointData[i]->GetYOut();
            Double_t fZ_out = pointData[i]->GetZOut();
            paddle = pointData[i]->GetDetCopyID() + 1;
            // std::cout<<paddle<<std::endl;

            x = ((fX_in + fX_out) / 2.);
            y = ((fY_in + fY_out) / 2.);
            z = ((fZ_in + fZ_out) / 2.);
            vpos.SetXYZ(x, y, z);

            vpos = fRot * (vpos - fTrans);
            time = pointData[i]->GetTime() + rand->Gaus(0., fsigma_t);

            // Add hit data
            AddHitData(paddle, vpos.X() * 10., vpos.Y() * 10. + rand->Gaus(0., fsigma_y), time);
        }
    }
    if (pointData)
    {
        delete[] pointData;
    }
    LOG(info) << "R3BSofTofWDigitizer: " << fTofHits->GetEntriesFast() << " points registered in this event";
    return;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofTofWDigitizer::ReInit()
{
    SetParContainers();
    SetParameter();
    return kSUCCESS;
}

// -----   Public method Reset   -----------------------------------------------
void R3BSofTofWDigitizer::Reset()
{
    R3BLOG(debug, "");
    if (fTofHits)
    {
        fTofHits->Clear();
    }
}

// -----   Private method AddHitData  -------------------------------------------
R3BSofTofWHitData* R3BSofTofWDigitizer::AddHitData(Int_t paddle, Double_t x, Double_t y, Double_t time)
{
    // It fills the R3BSofTofWHitData
    TClonesArray& clref = *fTofHits;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTofWHitData(paddle, x, y, time);
}

ClassImp(R3BSofTofWDigitizer);
