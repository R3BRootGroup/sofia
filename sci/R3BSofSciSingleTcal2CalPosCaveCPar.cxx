#include "R3BSofSciSingleTcal2CalPosCaveCPar.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "R3BMwpcHitData.h"
#include "R3BSofSciSingleTcalData.h"
#include "TClonesArray.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TRandom.h"
#include "TVector3.h"

// R3BSofSciSingleTcal2CalPosCaveCPar: Default Constructor --------------------------
R3BSofSciSingleTcal2CalPosCaveCPar::R3BSofSciSingleTcal2CalPosCaveCPar()
    : FairTask("R3BSofSciSingleTcal2CalPosCaveCPar", 1)
    , fNumDets(2)
    , fNumParamsPerDet(2)
    , fMinStatistics(0)
    , fFitMin(-2)
    , fFitMax(2)
    , fSTcalSci(NULL)
    , fHitMw0(NULL)
    , fCalPosPar(NULL)
    , fMwpc0GeoPar(NULL)
    , fOutputFile(NULL)
{}

// R3BSofSciSingleTcal2CalPosCaveCPar: Standard Constructor --------------------------
R3BSofSciSingleTcal2CalPosCaveCPar::R3BSofSciSingleTcal2CalPosCaveCPar(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fNumDets(2)
    , fNumParamsPerDet(2)
    , fMinStatistics(0)
    , fFitMin(-2)
    , fFitMax(2)
    , fSTcalSci(NULL)
    , fHitMw0(NULL)
    , fCalPosPar(NULL)
    , fMwpc0GeoPar(NULL)
    , fOutputFile(NULL)

{}

// R3BSofSciSingleTcal2CalPosCaveCPar: Destructor ----------------------------------------
R3BSofSciSingleTcal2CalPosCaveCPar::~R3BSofSciSingleTcal2CalPosCaveCPar()
{
    if (fCalPosPar)
        delete fCalPosPar;

    if (fMwpc0GeoPar)
        delete fMwpc0GeoPar;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofSciSingleTcal2CalPosCaveCPar::Init()
{

    LOG(info) << "R3BSofSciSingleTcal2CalPosCaveCPar: Init";

    FairRootManager* rm = FairRootManager::Instance();
    if (!rm) {
        return kFATAL;
    }

    // --- -------------------- --- //
    // --- INPUT MWPC0 Hit data --- //
    // --- -------------------- --- //

    fHitMw0 = (TClonesArray*)rm->GetObject("Mwpc0HitData");
    if (!fHitMw0) {
        LOG(error)
            << "R3BSofSciSingleTcal2CalPosCaveCPar::Init() Couldn't get handle on SofSciSingleTcalData container";
        return kFATAL;
    }
    // --- ------------------------- --- //
    // --- INPUT Sci SingleTcal data --- //
    // --- ------------------------- --- //

    fSTcalSci = (TClonesArray*)rm->GetObject("SofSciSingleTcalData");
    if (!fSTcalSci) {
        LOG(error)
            << "R3BSofSciSingleTcal2CalPosCaveCPar::Init() Couldn't get handle on SofSciSingleTcalData container";
        return kFATAL;
    }

    // --- ----------------------------------- --- //
    // --- SOF SCI CALPOS PARAMETERS CONTAINER --- //
    // --- ----------------------------------- --- //

    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb) {
        return kFATAL;
    }

    fCalPosPar = (R3BSofSciCalPosPar*)rtdb->getContainer("SofSciCalPosPar");
    if (!fCalPosPar) {
        LOG(error) << "R3BSofSciSingleTcal2CalPosCaveCPar::Init() Couldn't get handle on SofSciCalPosPar container";
        return kFATAL;
    }

    // --- ---------------------- --- //
    // ---  GEOMETRY OF THE MWPC0 --- //
    // --- ---------------------- --- //
    fMwpc0GeoPar = (R3BTGeoPar*)rtdb->getContainer("mwpc0GeoPar");
    if (!fMwpc0GeoPar) {
        LOG(error) << "R3BSofSciSingleTcal2CalPosCaveCPar::SetParContainers() : Could not get access to mwpc0GeoPar "
                      "container.";
        return kFATAL;
    } else
        LOG(info) << "R3BSofSciSingleTcal2CalPosCaveCPar::SetParContainers() : Container mwpc0GeoPar found.";

    // --- ---------------------- --- //
    // --- HISTOGRAMS DECLARATION --- //
    // --- ---------------------- --- //
    fh2_X0_vs_RawPosCaveC = new TH2D("X0vsRawPosCaveC", "X0vsRawPosCaveC", 1400, -7, 7, 2000, -50, 50);
    fh2_X0_vs_RawPosCaveC->GetXaxis()->SetTitle(
        "(RIGHT,Wix. side) -->  SofSci-RawPos Cave C [ns] --> (LEFT,Mes. side) -->");
    fh2_X0_vs_RawPosCaveC->GetYaxis()->SetTitle("Mwpc0-X [mm]");
    fh2_X0_vs_RawPosCaveC->GetYaxis()->SetTitleOffset(1.1);
    fh2_X0_vs_RawPosCaveC->GetXaxis()->CenterTitle(true);
    fh2_X0_vs_RawPosCaveC->GetYaxis()->CenterTitle(true);
    fh2_X0_vs_RawPosCaveC->GetXaxis()->SetLabelSize(0.045);
    fh2_X0_vs_RawPosCaveC->GetXaxis()->SetTitleSize(0.045);
    fh2_X0_vs_RawPosCaveC->GetYaxis()->SetLabelSize(0.045);
    fh2_X0_vs_RawPosCaveC->GetYaxis()->SetTitleSize(0.045);
    fh2_X0_vs_RawPosCaveC->Draw("col");

    // TF1: pol1 for the ProfileX
    char NamePol[255];
    sprintf(NamePol, "pol%i", fNumParamsPerDet - 1);
    fit_Xmm_vs_RawPosNs = new TF1("fit_X_vs_RawPos", NamePol, fFitMin, fFitMax);

    return kSUCCESS;
}

// -----   Public method ReInit  --------------------------------------------
InitStatus R3BSofSciSingleTcal2CalPosCaveCPar::ReInit() { return kSUCCESS; }

// -----   Public method Exec   --------------------------------------------
void R3BSofSciSingleTcal2CalPosCaveCPar::Exec(Option_t* opt)
{
    Int_t nHits;

    if (fHitMw0 && fHitMw0->GetEntriesFast() == 1) {
        // --- -------------------------- --- //
        // --- read the hit data of Mwpc0 --- //
        // --- -------------------------- --- //
        R3BMwpcHitData* hitMwpc0 = (R3BMwpcHitData*)fHitMw0->At(0);

        // --- ------------------------------ --- //
        // --- loop over sci single tcal data --- //
        // --- ------------------------------ --- //
        if (fSTcalSci && fSTcalSci->GetEntriesFast()) {
            nHits = fSTcalSci->GetEntriesFast();
            for (Int_t ihit = 0; ihit < nHits; ihit++) {
                R3BSofSciSingleTcalData* hitstcal = (R3BSofSciSingleTcalData*)fSTcalSci->At(ihit);
                if (!hitstcal)
                    continue;
                if (hitstcal->GetDetector() == fNumDets && hitMwpc0) {
                    fh2_X0_vs_RawPosCaveC->Fill(hitstcal->GetRawPosNs(),
                                                hitMwpc0->GetX()
                                                    + 10. * fMwpc0GeoPar->GetPosX());   // attention GetPosX() in cm !
                }
            }
        }
    }   // end of HitMwpc0
}

// ---- Public method Finish   --------------------------------------------------
void R3BSofSciSingleTcal2CalPosCaveCPar::FinishTask()
{
    CalculateCalPosCaveCParams();
    fCalPosPar->printParams();
}

// ------------------------------
void R3BSofSciSingleTcal2CalPosCaveCPar::CalculateCalPosCaveCParams()
{
    LOG(info) << "R3BSofSciSingleTcal2CalPosCaveCPar: CalculateCalPosCaveCParams()";
    fCalPosPar->SetNumDets(fNumDets);
    fCalPosPar->SetNumParamsPerDet(fNumParamsPerDet);
    fCalPosPar->SetNumParams(fNumDets * fNumParamsPerDet);
    LOG(info) << "Number of entries for the 2D histo: " << fh2_X0_vs_RawPosCaveC->GetEntries();

    if (fh2_X0_vs_RawPosCaveC->GetEntries() > fMinStatistics) {
        fh2_X0_vs_RawPosCaveC->Write();
        fh1_pfx = (TH1D*)fh2_X0_vs_RawPosCaveC->ProfileX("X_vs_RawPos_pfx");
        fh1_pfx->Fit(fit_Xmm_vs_RawPosNs, "R");
        fh1_pfx->Write();
        fit_Xmm_vs_RawPosNs->Write();
        LOG(info) << "Fit Done: p0 = " << fit_Xmm_vs_RawPosNs->GetParameter(0)
                  << ", p1=" << fit_Xmm_vs_RawPosNs->GetParameter(1);

        for (Int_t degree = 0; degree < fNumParamsPerDet; degree++) {
            fCalPosPar->SetParam(fit_Xmm_vs_RawPosNs->GetParameter(degree), (fNumDets - 1) * fNumParamsPerDet + degree);
        }
    }

    fCalPosPar->setChanged();

    return;
}

ClassImp(R3BSofSciSingleTcal2CalPosCaveCPar);
