#include "R3BSofTrimCalculateDriftTimeOffsetPar.h"

#include "R3BMwpcHitData.h"
#include "R3BSofTrimCalData.h"

#include "R3BEventHeader.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "TGeoManager.h"

#include "TClonesArray.h"
#include "TF1.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TRandom.h"
#include "TVector3.h"

#include <iostream>
#include <stdlib.h>

// R3BSofTrimCalculateDriftTimeOffsetPar: Default Constructor --------------------------
R3BSofTrimCalculateDriftTimeOffsetPar::R3BSofTrimCalculateDriftTimeOffsetPar()
    : FairTask("R3BSofTrimCalculateDriftTimeOffsetPar", 1)
    , fNumSections(3)
    , fNumAnodes(6)
    , fMinStatistics(0)
    , fTrimCalData(NULL)
    , fMwpc0HitData(NULL)
    , fMwpc1HitData(NULL)
    , fCalPar(NULL)
    , fMwpc0GeoPar(NULL)
    , fMwpc1GeoPar(NULL)
    , fTrimGeoPar(NULL)
    , fWidthAnode(25)       // mm
    , fDistInterSection(50) // mm 2*edge anodes of 20 mm + 1*10 mm gap
    , fDriftVelocity(60)    // mm/micros, distance from anode to FG: 80 mm with a difference of V = 2700V
    , fOutputFile(NULL)
{
}

// R3BSofTrimCalculateDriftTimeOffsetPar: Standard Constructor --------------------------
R3BSofTrimCalculateDriftTimeOffsetPar::R3BSofTrimCalculateDriftTimeOffsetPar(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNumSections(0)
    , fNumAnodes(0)
    , fMinStatistics(0)
    , fTrimCalData(NULL)
    , fMwpc0HitData(NULL)
    , fMwpc1HitData(NULL)
    , fCalPar(NULL)
    , fMwpc0GeoPar(NULL)
    , fMwpc1GeoPar(NULL)
    , fTrimGeoPar(NULL)
    , fWidthAnode(25)       // mm
    , fDistInterSection(50) // mm
    , fDriftVelocity(60)    // mm/micros
    , fOutputFile(NULL)
{
}

// R3BSofTrimCalculateDriftTimeOffsetPar: Destructor ----------------------------------------
R3BSofTrimCalculateDriftTimeOffsetPar::~R3BSofTrimCalculateDriftTimeOffsetPar()
{
    if (fCalPar)
        delete fCalPar;

    if (fMwpc0GeoPar)
        delete fMwpc0GeoPar;

    if (fMwpc1GeoPar)
        delete fMwpc1GeoPar;

    if (fTrimGeoPar)
        delete fTrimGeoPar;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTrimCalculateDriftTimeOffsetPar::Init()
{

    LOG(INFO) << "R3BSofTrimCalculateDriftTimeOffsetPar: Init";

    FairRootManager* rm = FairRootManager::Instance();
    if (!rm)
    {
        return kFATAL;
    }

    // --- ------------------- --- //
    // --- INPUT TRIM CAL DATA --- //
    // --- ------------------- --- //

    fTrimCalData = (TClonesArray*)rm->GetObject("TrimCalData");
    if (!fTrimCalData)
    {
        LOG(ERROR) << "R3BSofTrimCalculateDriftTimeOffsetPar::Init() Couldn't get handle on TrimCalData";
        return kFATAL;
    }

    // --- -------------------- --- //
    // --- INPUT MWPC0 HIT DATA --- //
    // --- -------------------- --- //

    fMwpc0HitData = (TClonesArray*)rm->GetObject("Mwpc0HitData");
    if (!fMwpc0HitData)
    {
        LOG(ERROR) << "R3BSofTrimCalculateDriftTimeOffsetPar::Init() Couldn't get handle on Mwpc0HitData";
        return kFATAL;
    }

    // --- -------------------- --- //
    // --- INPUT MWPC1 HIT DATA --- //
    // --- -------------------- --- //

    fMwpc1HitData = (TClonesArray*)rm->GetObject("Mwpc1HitData");
    if (!fMwpc1HitData)
    {
        LOG(ERROR) << "R3BSofTrimCalculateDriftTimeOffsetPar::Init() Couldn't get handle on Mwpc1HitData";
        return kFATAL;
    }

    // --- ------------------ --- //
    // --- RUN TIME DATA BASE --- //
    // --- ------------------ --- //

    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        return kFATAL;
    }

    // --- ------------------------------------------ --- //
    // --- SOF TRIM TRIANGLE CAL PARAMETERS CONTAINER --- //
    // --- ------------------------------------------ --- //

    fCalPar = (R3BSofTrimCalPar*)rtdb->getContainer("trimCalPar");
    if (!fCalPar)
    {
        LOG(ERROR) << "R3BSofTrimCalculateDriftTimeOffsetPar::Init() Couldn't get handle on trimCalPar container";
        return kFATAL;
    }
    else
    {
        fNumSections = fCalPar->GetNumSections();
        fNumAnodes = fCalPar->GetNumAnodes();
        LOG(INFO) << "R3BSofTrimCalculateDriftTimeOffsetPar::Init() trimCalPar container found with fNumSections = "
                  << fNumSections;
    }

    // --- ---------------------- --- //
    // ---  GEOMETRY OF THE MWPC0 --- //
    // --- ---------------------- --- //
    fMwpc0GeoPar = (R3BTGeoPar*)rtdb->getContainer("Mwpc0GeoPar");
    if (!fMwpc0GeoPar)
    {
        LOG(ERROR) << "R3BSofTrimCalculateDriftTimeOffsetPar::SetParContainers() : Could not get access to mwpc0GeoPar "
                      "container.";
        return kFATAL;
    }
    else
        LOG(INFO) << "R3BSofSciCalculateDriftTimeOffsetPar::SetParContainers() : Container mwpc0GeoPar found.";

    // --- ---------------------- --- //
    // ---  GEOMETRY OF THE MWPC1 --- //
    // --- ---------------------- --- //
    fMwpc1GeoPar = (R3BTGeoPar*)rtdb->getContainer("Mwpc1GeoPar");
    if (!fMwpc1GeoPar)
    {
        LOG(ERROR) << "R3BSofTrimCalculateDriftTimeOffsetPar::SetParContainers() : Could not get access to mwpc1GeoPar "
                      "container.";
        return kFATAL;
    }
    else
        LOG(INFO) << "R3BSofSciCalculateDriftTimeOffsetPar::SetParContainers() : Container mwpc1GeoPar found.";

    // --- ----------------------------- --- //
    // ---  GEOMETRY OF THE TRIPLE-MUSIC --- //
    // --- ----------------------------- --- //
    fTrimGeoPar = (R3BTGeoPar*)rtdb->getContainer("TrimGeoPar");
    if (!fTrimGeoPar)
    {
        LOG(ERROR) << "R3BSofTrimCalculateDriftTimeOffsetPar::SetParContainers() : Could not get access to trimGeoPar "
                      "container.";
        return kFATAL;
    }
    else
        LOG(INFO) << "R3BSofSciCalculateDriftTimeOffsetPar::SetParContainers() : Container trimGeoPar found.";

    // --- ---------------------- --- //
    // --- HISTOGRAMS DECLARATION --- //
    // --- ---------------------- --- //

    char name[100];
    fh1_DeltaDT = new TH1D*[fNumSections * fNumAnodes];
    for (Int_t section = 0; section < fNumSections; section++)
    {
        for (Int_t anode = 0; anode < fNumAnodes; anode++)
        {
            sprintf(name, "DeltaDT_S%iA%i", section + 1, anode + 1);
            fh1_DeltaDT[anode + section * fNumAnodes] = new TH1D(name, name, 4000, -20000, 20000);
            fh1_DeltaDT[anode + section * fNumAnodes]->GetXaxis()->SetTitle(
                "Delta Drift Time [channels, 100 ps TDC resolution]");
        }
    }

    return kSUCCESS;
}

// -----   Public method ReInit  --------------------------------------------
InitStatus R3BSofTrimCalculateDriftTimeOffsetPar::ReInit() { return kSUCCESS; }

// -----   Public method Exec   --------------------------------------------
void R3BSofTrimCalculateDriftTimeOffsetPar::Exec(Option_t* opt)
{

    Int_t iSec, iAnode;
    Double_t geoX0, geoX1, geoZ0, geoZ1, geoDZ;
    Double_t X0, X1, DX;
    Double_t geoZtrim, geoZfirstanode, Xanode, DTraw, ZposAnode;
    UInt_t nHitsMw0, nHitsMw1, nHits;

    // FIX ME:
    geoX0 = 3;      // mm
    geoX1 = 0.;     // mm
    geoZ0 = -2620.; // mm
    geoZ1 = 300.;   // mm
    geoDZ = geoZ1 - geoZ0;
    geoZtrim = -2175; // mm
    // geoX0 = 10. * (Double_t)fMwpc0GeoPar->GetPosX();
    // geoX1 = 10. * (Double_t)fMwpc1GeoPar->GetPosX();
    // geoZ0 = 10. * (Double_t)fMwpc0GeoPar->GetPosZ();
    // geoZ1 = 10. * (Double_t)fMwpc1GeoPar->GetPosZ();
    // geoZtrim = 10. * (Double_t)fTrimGeoPar->GetPosZ();
    // geoZtrim is the center of the Triple-MUSIC in cm
    // The active volume of the Triple-MUSIC is 590 mm = 59 cm
    // The width of the screening anode is 20 mm = 2 cm each
    // Therefore, the rim of the first anode of the first section is located 275 mm downstream
    geoZfirstanode = geoZtrim - 275 + 0.5 * fWidthAnode;

    // --- -------------- --- //
    // --- MWPCs HIT DATA --- //
    // --- -------------- --- //
    nHitsMw0 = fMwpc0HitData->GetEntries();
    nHitsMw1 = fMwpc1HitData->GetEntries();
    // if (nHitsMw0 != 0)
    //{
    //    std::cout << std::endl;
    //    std::cout << "nHitsMw0=" << nHitsMw0 << ", nHitsMw1=" << nHitsMw1 << std::endl;
    //}
    if (nHitsMw0 == 1 && nHitsMw1 == 1)
    {
        R3BMwpcHitData* hitMwpc0 = (R3BMwpcHitData*)fMwpc0HitData->At(0);
        R3BMwpcHitData* hitMwpc1 = (R3BMwpcHitData*)fMwpc1HitData->At(0);
        X0 = hitMwpc0->GetX() + geoX0;
        X1 = hitMwpc1->GetX() + geoX1;
        DX = X1 - X0;

        // --- --------------------------------------- --- //
        // --- LOOP OVER TRIANGLE CAL HITS FOR SofTrim --- //
        // --- --------------------------------------- --- //
        nHits = fTrimCalData->GetEntries();
        if (!nHits)
            return;
        for (Int_t ihit = 0; ihit < nHits; ihit++)
        {
            R3BSofTrimCalData* hit = (R3BSofTrimCalData*)fTrimCalData->At(ihit);
            iSec = hit->GetSecID() - 1;
            iAnode = hit->GetAnodeID() - 1;
            DTraw = hit->GetDriftTimeRaw();
            // std::cout << "Section:" << iSec + 1 << ", Anode:" << iAnode + 1 << ", DTraw=" << DTraw << std::endl;
            ZposAnode = geoZfirstanode + (Double_t)(iAnode + iSec * fNumAnodes) * fWidthAnode +
                        (Double_t)iSec * fDistInterSection;
            Xanode = (ZposAnode * DX) / geoDZ;
            // std::cout << "ZposAnode=" << ZposAnode << " mm, Xanode=" << Xanode << " mm or "
            //          << 1000. * Xanode / fDriftVelocity << " [100 ps]" << std::endl;
            fh1_DeltaDT[iAnode + iSec * fNumAnodes]->Fill(10000. * Xanode / fDriftVelocity -
                                                          DTraw); // 10000 : mm/micros -> mm/100ps
        }                                                         // end of loop over the mapped data
    }
}

// ---- Public method Reset   --------------------------------------------------
void R3BSofTrimCalculateDriftTimeOffsetPar::Reset() {}

void R3BSofTrimCalculateDriftTimeOffsetPar::FinishEvent() {}

// ---- Public method Finish   --------------------------------------------------
void R3BSofTrimCalculateDriftTimeOffsetPar::FinishTask()
{
    CalculateOffsets();
    fCalPar->printParams();
}

// ------------------------------
void R3BSofTrimCalculateDriftTimeOffsetPar::CalculateOffsets()
{
    LOG(INFO) << "R3BSofTrimCalculateDriftTimeOffsetPar: CalculateOffsets()";

    Double_t BinMax, X_BinMax;
    TF1** fit_max = new TF1*[fNumSections * fNumAnodes];

    for (Int_t section = 0; section < fNumSections; section++)
    {
        for (Int_t anode = 0; anode < fNumAnodes; anode++)
        {
            fh1_DeltaDT[anode + section * fNumAnodes]->Write();
            if (fh1_DeltaDT[anode + section * fNumAnodes]->Integral() > fMinStatistics)
            {
                BinMax = fh1_DeltaDT[anode + section * fNumAnodes]->GetMaximumBin();
                X_BinMax = fh1_DeltaDT[anode + section * fNumAnodes]->GetBinCenter(BinMax);
                fit_max[anode + section * fNumAnodes] = new TF1("fit_gaus", "gaus", X_BinMax - 500, X_BinMax + 500);
                fh1_DeltaDT[anode + section * fNumAnodes]->Fit(fit_max[anode + section * fNumAnodes], "R");
                fCalPar->SetDriftTimeOffset(
                    section + 1, anode + 1, fit_max[anode + section * fNumAnodes]->GetParameter(1));
            }
        }
    }

    fCalPar->setChanged();
    fCalPar->printParams();
    for (Int_t section = 0; section < fNumSections; section++)
    {
        for (Int_t anode = 0; anode < fNumAnodes; anode++)
        {
            std::cout << fit_max[anode + section * fNumAnodes]->GetParameter(1) << std::endl;
        }
    }
    return;
}

ClassImp(R3BSofTrimCalculateDriftTimeOffsetPar)
