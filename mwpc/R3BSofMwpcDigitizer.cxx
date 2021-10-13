// ----------------------------------------------------------------
// -----          R3BSofMwpcDigitizer source file             -----
// -----         Created 03/11/19  by JL Rodriguez            -----
// ----------------------------------------------------------------

#include "R3BSofMwpcDigitizer.h"
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "R3BTGeoPar.h"
#include "TClonesArray.h"

// includes for modeling
#include "TGeoManager.h"
#include "TParticle.h"
#include "TVirtualMC.h"

#include "TMath.h"
#include "TRandom.h"
#include "TVector3.h"
#include <iostream>
#include <string>

#include "R3BMCTrack.h"
#include "R3BSofMwpcPoint.h"

// R3BSofMwpcDigitizer: Default Constructor --------------------------
R3BSofMwpcDigitizer::R3BSofMwpcDigitizer()
    : FairTask("R3BSof MWPC Digitization scheme", 1)
    , fName("Mwpc")
    , fMCTrack(NULL)
    , fMwpcPoints(NULL)
    , fMwpcHits(NULL)
    , fsigma_x(0.125)
    , fsigma_y(0.25)
{
}

// R3BSofMwpcDigitizer: Standard Constructor --------------------------
R3BSofMwpcDigitizer::R3BSofMwpcDigitizer(const TString& name, Int_t iVerbose)
    : FairTask(name + "Digi", iVerbose)
    , fName(name)
    , fMCTrack(NULL)
    , fMwpcPoints(NULL)
    , fMwpcHits(NULL)
    , fsigma_x(0.125)
    , fsigma_y(0.25)
{
}

// Virtual R3BSofMwpcDigitizer: Destructor ----------------------------
R3BSofMwpcDigitizer::~R3BSofMwpcDigitizer()
{
    LOG(INFO) << "R3BSof" + fName + "Digitizer: Delete instance";
    if (fMwpcPoints)
        delete fMwpcPoints;
    if (fMwpcHits)
        delete fMwpcHits;
}

void R3BSofMwpcDigitizer::SetParContainers()
{
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();

    fMwpcGeoPar = (R3BTGeoPar*)rtdb->getContainer(fName + "GeoPar");
    if (!fMwpcGeoPar)
    {
        LOG(ERROR) << "R3BSofMwpcDigitizer::SetParContainers() : Could not get access to " + fName +
                          "GeoPar container.";
        return;
    }
    else
        LOG(INFO) << "R3BSofMwpcDigitizer::SetParContainers() : Container " + fName + "GeoPar found.";
}

void R3BSofMwpcDigitizer::SetParameter()
{
    fsigma_x = fMwpcGeoPar->GetSigmaX();
    fsigma_y = fMwpcGeoPar->GetSigmaY();

    fRot.RotateX(-fMwpcGeoPar->GetRotX() * TMath::DegToRad());
    fRot.RotateY(-fMwpcGeoPar->GetRotY() * TMath::DegToRad());
    fRot.RotateZ(-fMwpcGeoPar->GetRotZ() * TMath::DegToRad());

    fTrans.SetXYZ(fMwpcGeoPar->GetPosX(), fMwpcGeoPar->GetPosY(), fMwpcGeoPar->GetPosZ());
}

// ----   Public method Init  -----------------------------------------
InitStatus R3BSofMwpcDigitizer::Init()
{
    LOG(INFO) << "R3BSof" + fName + "Digitizer: Init";

    // Get input array
    FairRootManager* ioman = FairRootManager::Instance();
    if (!ioman)
        LOG(fatal) << "Init: No FairRootManager";

    fMCTrack = (TClonesArray*)ioman->GetObject("MCTrack");
    fMwpcPoints = (TClonesArray*)ioman->GetObject("Sof" + fName + "Point");

    // Register output array fMwpcHits
    fMwpcHits = new TClonesArray("R3BSofMwpcHitData", 10);
    ioman->Register(fName + "HitData", "Digital response in " + fName, fMwpcHits, kTRUE);

    SetParameter();
    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofMwpcDigitizer::Exec(Option_t* opt)
{
    Reset();
    // Reading the Input -- Point Data --
    Int_t nHits = fMwpcPoints->GetEntries();
    if (!nHits)
        return;
    // Data from Point level
    R3BSofMwpcPoint** pointData;
    pointData = new R3BSofMwpcPoint*[nHits];
    Int_t TrackId = 0, PID = 0;
    Double_t x = 0., y = 0., z = 0.;
    TVector3 vpos;
    for (Int_t i = 0; i < nHits; i++)
    {
        pointData[i] = (R3BSofMwpcPoint*)(fMwpcPoints->At(i));
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

            x = ((fX_in + fX_out) / 2.);
            y = ((fY_in + fY_out) / 2.);
            z = ((fZ_in + fZ_out) / 2.);
            vpos.SetXYZ(x, y, z);

            vpos = fRot * (vpos - fTrans);

            AddHitData(vpos.X()*10. + gRandom->Gaus(0., fsigma_x), vpos.Y()*10. + gRandom->Gaus(0., fsigma_y));//mm
        }
    }
    if (pointData)
        delete pointData;
    return;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofMwpcDigitizer::ReInit()
{
    SetParContainers();
    SetParameter();
    return kSUCCESS;
}

// -----   Public method Reset   -----------------------------------------------
void R3BSofMwpcDigitizer::Reset()
{
    LOG(DEBUG) << "Clearing R3BSof" + fName + "Digitizer Structure";
    if (fMwpcHits)
        fMwpcHits->Clear();
}

// -----   Private method AddHitData  -------------------------------------------
R3BSofMwpcHitData* R3BSofMwpcDigitizer::AddHitData(Double_t x, Double_t y)
{
    // It fills the R3BSofMwpcHitData
    TClonesArray& clref = *fMwpcHits;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofMwpcHitData(x, y);
}

// -----   Public method Finish  ------------------------------------------------
void R3BSofMwpcDigitizer::Finish() {}

ClassImp(R3BSofMwpcDigitizer)
