// ------------------------------------------------------------
// -----         R3BSofTrimCal2Hit source file         -----
// ------------------------------------------------------------

// ROOT headers
#include "TClonesArray.h"
#include "TMath.h"
#include "TRandom.h"

// Fair headers
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include <iomanip>

// Trim headers
#include "R3BSofTrimCal2Hit.h"
#include "R3BSofTrimCalData.h"
#include "R3BSofTrimHitPar.h"

// Sci headers
#include "R3BSofSciCalData.h"

// R3BSofTrimCal2Hit: Default Constructor --------------------------
R3BSofTrimCal2Hit::R3BSofTrimCal2Hit()
    : FairTask("R3BSof Trim Hit Calibrator", 1)
    , fExpId(455)
    , fCoulex(true)
    , fNumSections(3)
    , fNumAnodes(6)
    , fTriShape(kTRUE)
    , fIdCaveC(2)
    , fOnline(kFALSE)
    , fTrimHitPar(NULL)
    , fTrimCalData(NULL)
    , fSciCalData(NULL)
    , fTrimHitData(NULL)
{
}

// R3BSofTrimCal2HitPar: Standard Constructor --------------------------
R3BSofTrimCal2Hit::R3BSofTrimCal2Hit(const char* name, Int_t iVerbose)
    : FairTask(name, iVerbose)
    , fExpId(455)
    , fCoulex(true)
    , fNumSections(3)
    , fNumAnodes(6)
    , fTriShape(kTRUE)
    , fIdCaveC(2)
    , fOnline(kFALSE)
    , fTrimHitPar(NULL)
    , fTrimCalData(NULL)
    , fSciCalData(NULL)
    , fTrimHitData(NULL)
{
}

// Virtual R3BSofTrimCal2Hit: Destructor
R3BSofTrimCal2Hit::~R3BSofTrimCal2Hit()
{
    LOG(INFO) << "R3BSofTrimCal2Hit: Delete instance";
    if (fTrimCalData)
        delete fTrimCalData;
    if (fTrimHitData)
        delete fTrimHitData;
    if (fSciCalData)
        delete fSciCalData;
}

void R3BSofTrimCal2Hit::SetParContainers()
{
    FairRuntimeDb* rtdb = FairRuntimeDb::instance();
    if (!rtdb)
    {
        LOG(ERROR) << "FairRuntimeDb not opened!";
    }

    fTrimHitPar = (R3BSofTrimHitPar*)rtdb->getContainer("trimHitPar");
    if (!fTrimHitPar)
    {
        LOG(ERROR) << "R3BSofTrimCal2HitPar::Init() Couldn't get handle on trimHitPar container";
        return;
    }
    else
    {
        if (fTriShape == kTRUE)
        {
            LOG(INFO) << "Triple-MUSIC has triangular shape anodes";
            fTrimHitPar->SetNumSignalsPerSection(3);
        }
        else
        {
            LOG(INFO) << "Triple-MUSIC has rectangular shape anodes";
            fTrimHitPar->SetNumSignalsPerSection(6);
        }
        LOG(INFO) << "R3BSofTrimCal2Hit:: trimHitPar container open ";
    }
    return;
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTrimCal2Hit::Init()
{
    LOG(INFO) << "R3BSofTrimCal2Hit::Init()";

    FairRootManager* rootManager = FairRootManager::Instance();
    if (!rootManager)
    {
        return kFATAL;
    }

    // --- ------------------------------- --- //
    // --- INPUT CAL DATA FOR TRIPLE-MUSIC --- //
    // --- ------------------------------- --- //
    fTrimCalData = (TClonesArray*)rootManager->GetObject("TrimCalData");
    if (!fTrimCalData)
    {
        LOG(WARNING) << "R3BSofTrimCal2Hit::Init() TrimCalData not found";
    }

    // --- ---------------------- --- //
    // --- INPUT CAL DATA FOR SCI --- //
    // --- ---------------------- --- //
    fSciCalData = (TClonesArray*)rootManager->GetObject("SofSciCalData");
    if (!fSciCalData)
    {
        LOG(WARNING) << "R3BSofTrimCal2Hit::Init() SofSciCalData not found";
    }

    // --- --------------- --- //
    // --- OUTPUT HIT DATA --- //
    // --- --------------- --- //
    fTrimHitData = new TClonesArray("R3BSofTrimHitData", fNumSections);
    rootManager->Register("TrimHitData", "Trim Hit", fTrimHitData, !fOnline);

    return kSUCCESS;
}

// -----   Public method ReInit   ----------------------------------------------
InitStatus R3BSofTrimCal2Hit::ReInit()
{
    SetParContainers();
    return kSUCCESS;
}

// -----   Public method Execution   --------------------------------------------
void R3BSofTrimCal2Hit::Exec(Option_t* option)
{
    if (fExpId == 455 && fCoulex)
        S455_Coulex();
    else if (fExpId == 455 && !fCoulex)
        S455_P2p();

    return;
}

// -----   Coulex experiment with exotic beams   --------------------------------
void R3BSofTrimCal2Hit::S455_Coulex()
{
    // Reset entries in output arrays, local arrays
    Reset();

    // Get the parameters
    if (!fTrimHitPar)
    {
        LOG(ERROR) << "R3BSofTrimCal2Hit::Exec() TrimHitPar Container not found";
    }

    // Local variables at Cal Level
    Int_t iSec, iAnode;
    UInt_t mult[fNumSections * fNumAnodes];
    Float_t e[fNumSections * fNumAnodes];
    Double_t dt[fNumSections * fNumAnodes];
    Double_t betaFromS2 = 0.;

    // Local variables at Hit Level
    Int_t nAligned, nRaw;
    if (fTriShape == kTRUE)
        nAligned = fNumAnodes / 2;
    else
        nAligned = fNumAnodes;
    Float_t eal[fNumSections * nAligned];
    Float_t dtal[fNumSections * nAligned];
    Float_t sumRaw, sumBeta, sumDT, sumTheta, zval, dtSection, Ddt;
    Double_t correction;

    // Initialization of the local variables
    for (Int_t s = 0; s < fNumSections; s++)
    {
        for (Int_t a = 0; a < fNumAnodes; a++)
        {
            mult[a + s * fNumAnodes] = 0.;
            e[a + s * fNumAnodes] = 0.;
            dt[a + s * fNumAnodes] = -1000000.;
        }
        for (Int_t ch = 0; ch < nAligned; ch++)
        {
            eal[ch + s * nAligned] = 0;
            dtal[ch + s * nAligned] = 0;
        }
    }

    // Get the number of entries of the SciCalData TClonesArray and extract the beam velocity
    if (fSciCalData)
    {
        Int_t nHitsCalSci = fSciCalData->GetEntries();
        if (!nHitsCalSci)
        {
            return;
        }
        for (Int_t entry = 0; entry < nHitsCalSci; entry++)
        {
            R3BSofSciCalData* iSciCalData = (R3BSofSciCalData*)fSciCalData->At(entry);
            if (iSciCalData->GetDetector() == fIdCaveC)
            {
                betaFromS2 = iSciCalData->GetBeta_S2();
            }
        }
    }

    // Get the number of entries of the TrimCalData TClonesArray and loop over it
    if (fTrimCalData)
    {
        Int_t nHitsCalTrim = fTrimCalData->GetEntries();
        if (!nHitsCalTrim)
        {
            return;
        }
        for (Int_t entry = 0; entry < nHitsCalTrim; entry++)
        {
            R3BSofTrimCalData* iCalData = (R3BSofTrimCalData*)fTrimCalData->At(entry);
            iSec = iCalData->GetSecID() - 1;
            iAnode = iCalData->GetAnodeID() - 1;
            mult[iAnode + iSec * fNumAnodes]++;
            e[iAnode + iSec * fNumAnodes] = iCalData->GetEnergyMatch();
            dt[iAnode + iSec * fNumAnodes] = iCalData->GetDriftTimeAligned();
        }

        // --- Fill the HIT level --- //
        Ddt = 0.5 * (dt[14] + dt[15]) - 0.5 * (dt[2] + dt[3]);
        for (Int_t s = 0; s < fNumSections; s++)
        {
            dtSection = 0.5 * (dt[2 + s * fNumAnodes] + dt[3 + s * fNumAnodes]);

            // === fEnergyRaw: sum of CorrDeltaDT Energy === //
            sumRaw = 0.;
            nRaw = 0;
            // if Rectangular shape:
            if (fTriShape == kFALSE)
            {
                for (Int_t a = 0; a < fNumAnodes; a++)
                {
                    if (mult[a + s * fNumAnodes] == 1)
                    {
                        eal[a + s * fNumAnodes] = e[a + s * fNumAnodes];
                        sumRaw += eal[a + s * fNumAnodes];
                        nRaw++;
                    }
                } // end of loop over the anodes
            }     // end calculation of fEnergyRaw for rectangular shape anodes
            else
            {
                for (Int_t ch = 0; ch < nAligned; ch++)
                {
                    if (mult[2 * ch + s * fNumAnodes] == 1 && mult[2 * ch + 1 + s * fNumAnodes] == 1)
                    {
                        // raw energy per pair: mean value of the match gain anodes
                        eal[ch + s * nAligned] = 0.5 * (e[2 * ch + s * fNumAnodes] + e[2 * ch + 1 + s * fNumAnodes]);
                        // correction of the DeltaDT dependency per pair
                        correction = 0;
                        for (Int_t deg = 0; deg < fTrimHitPar->GetNumCorrDeltaDTParsPerSignal(); deg++)
                        {
                            correction +=
                                TMath::Power(Ddt, deg) * fTrimHitPar->GetEnergyCorrDeltaDTPar(s + 1, ch + 1, deg);
                        }
                        if (correction != 0 && fTrimHitPar->GetEnergyCorrDeltaDTPar(s + 1, ch + 1, 0) != 0)
                        {
                            eal[ch + s * nAligned] = fTrimHitPar->GetEnergyCorrDeltaDTPar(s + 1, ch + 1, 0) *
                                                     eal[ch + s * nAligned] / correction;
                        }
                        dtal[ch + s * nAligned] = 0.5 * (dt[2 * ch + s * fNumAnodes] + dt[2 * ch + 1 + s * fNumAnodes]);
                        // sum Raw per section
                        sumRaw += eal[ch + s * nAligned];
                        nRaw++;
                    }
                } // end of loop over the pairs
            }     // end of calculation of fEnergyRaw for triangular shape anodes
            if (nRaw > 0)
                sumRaw = fTrimHitPar->GetEnergyAlignOffset(s + 1) +
                         fTrimHitPar->GetEnergyAlignGain(s + 1) * (sumRaw / (Float_t)nRaw);

            // === fEnergyBeta: fEnergyRaw corrected from the beam velocity === //
            correction = 0;
            double p0[3];
            double p1[3];
            // Run 228Th Zmax=92
            /*p0[0] = 48.58;
            p0[1] = 28.5266;
            p0[2] = 43.60;
            p1[0] = 0.2484;
            p1[1] = 0.365484;
            p1[2] = 0.27658;*/
            // Run 228Th Zmax=91
            /*
            p0[0] = 47.58;
            p0[1] = 27.5161;
            p0[2] = 42.60;
            p1[0] = 0.2484;
            p1[1] = 0.3655;
            p1[2] = 0.27658;
                */
            // Run 228Th +run 392 using Hg data for calibration
            p0[0] = 11.78;
            p0[1] = 9.706;
            p0[2] = 11.690;
            p1[0] = 0.463;
            p1[1] = 0.47543;
            p1[2] = 0.4622;

            for (Int_t deg = 0; deg < fTrimHitPar->GetNumCorrBetaParsPerSection(); deg++)
            {
                correction += TMath::Power(betaFromS2, deg) * fTrimHitPar->GetEnergyCorrBetaPar(s + 1, deg);
            }
            if (correction != 0 && fTrimHitPar->GetEnergyCorrBetaPar(s + 1, 0) != 0)
            {
                sumBeta = 30000. * sumRaw / correction;
            }
            else
            {
                sumBeta = sumRaw;
            }

            // TO DO : === fEnergyTheta: fEnergyBeta corrected from the theta angle in the Triple-MUSIC ===
            sumTheta = sumBeta;

            // TO DO : === fEnergyDT: fEnergyDT corrected from the X position in the Triple-MUSIC ===
            sumDT = sumTheta;

            // TO DO : === fZ ===
            zval = p0[s] + p1[s] * TMath::Sqrt(sumDT);

            // FILL HIT DATA
            AddHitData(s + 1, eal[0], eal[1], eal[2], sumRaw, sumBeta, sumTheta, sumDT, zval);
        } // end of loop over the sections
    }
    return;
}

// -----   Primary beam   -------------------------------------------------------
void R3BSofTrimCal2Hit::S455_P2p()
{
    // Reset entries in output arrays, local arrays
    Reset();

    // Get the parameters
    if (!fTrimHitPar)
    {
        LOG(ERROR) << "R3BSofTrimCal2Hit::Exec() TrimHitPar Container not found";
    }

    // Local variables at Cal Level
    Int_t iSec, iAnode;
    UInt_t mult[fNumSections * fNumAnodes];
    Float_t e[fNumSections * fNumAnodes];
    Double_t dt[fNumSections * fNumAnodes];
    Double_t betaFromS2 = 0.;

    // Local variables at Hit Level
    Int_t nAligned, nRaw;
    if (fTriShape == kTRUE)
        nAligned = fNumAnodes / 2;
    else
        nAligned = fNumAnodes;
    Float_t eal[fNumSections * nAligned];
    Float_t dtal[fNumSections * nAligned];
    Float_t sumRaw, sumDT, sumTheta, zval, dtSection, Ddt;
    Double_t correction;

    // Initialization of the local variables
    for (Int_t s = 0; s < fNumSections; s++)
    {
        for (Int_t a = 0; a < fNumAnodes; a++)
        {
            mult[a + s * fNumAnodes] = 0.;
            e[a + s * fNumAnodes] = 0.;
            dt[a + s * fNumAnodes] = -1000000.;
        }
        for (Int_t ch = 0; ch < nAligned; ch++)
        {
            eal[ch + s * nAligned] = 0;
            dtal[ch + s * nAligned] = 0;
        }
    }

    // Get the number of entries of the TrimCalData TClonesArray and loop over it
    if (fTrimCalData)
    {
        Int_t nHitsCalTrim = fTrimCalData->GetEntries();
        if (!nHitsCalTrim)
        {
            return;
        }
        for (Int_t entry = 0; entry < nHitsCalTrim; entry++)
        {
            R3BSofTrimCalData* iCalData = (R3BSofTrimCalData*)fTrimCalData->At(entry);
            iSec = iCalData->GetSecID() - 1;
            iAnode = iCalData->GetAnodeID() - 1;
            mult[iAnode + iSec * fNumAnodes]++;
            e[iAnode + iSec * fNumAnodes] = iCalData->GetEnergyMatch();
            dt[iAnode + iSec * fNumAnodes] = iCalData->GetDriftTimeAligned();
        }

        // --- Fill the HIT level --- //
        Ddt = 0.5 * (dt[14] + dt[15]) - 0.5 * (dt[2] + dt[3]);
        for (Int_t s = 0; s < fNumSections; s++)
        {
            dtSection = 0.5 * (dt[2 + s * fNumAnodes] + dt[3 + s * fNumAnodes]);

            // === fEnergyRaw: sum of CorrDeltaDT Energy === //
            sumRaw = 0.;
            nRaw = 0;
            // if Rectangular shape:
            if (fTriShape == kFALSE)
            {
                for (Int_t a = 0; a < fNumAnodes; a++)
                {
                    if (mult[a + s * fNumAnodes] == 1)
                    {
                        eal[a + s * fNumAnodes] = e[a + s * fNumAnodes];
                        sumRaw += eal[a + s * fNumAnodes];
                        nRaw++;
                    }
                } // end of loop over the anodes
            }     // end calculation of fEnergyRaw for rectangular shape anodes
            else
            {
                for (Int_t ch = 0; ch < nAligned; ch++)
                {
                    if (mult[2 * ch + s * fNumAnodes] == 1 && mult[2 * ch + 1 + s * fNumAnodes] == 1)
                    {
                        // raw energy per pair: mean value of the match gain anodes
                        eal[ch + s * nAligned] = 0.5 * (e[2 * ch + s * fNumAnodes] + e[2 * ch + 1 + s * fNumAnodes]);
                        // correction of the DeltaDT dependency per pair
                        correction = 0;
                        for (Int_t deg = 0; deg < fTrimHitPar->GetNumCorrDeltaDTParsPerSignal(); deg++)
                        {
                            correction +=
                                TMath::Power(Ddt, deg) * fTrimHitPar->GetEnergyCorrDeltaDTPar(s + 1, ch + 1, deg);
                        }
                        if (correction != 0 && fTrimHitPar->GetEnergyCorrDeltaDTPar(s + 1, ch + 1, 0) != 0)
                        {
                            eal[ch + s * nAligned] = fTrimHitPar->GetEnergyCorrDeltaDTPar(s + 1, ch + 1, 0) *
                                                     eal[ch + s * nAligned] / correction;
                        }
                        dtal[ch + s * nAligned] = 0.5 * (dt[2 * ch + s * fNumAnodes] + dt[2 * ch + 1 + s * fNumAnodes]);
                        // sum Raw per section
                        sumRaw += eal[ch + s * nAligned];
                        nRaw++;
                    }
                } // end of loop over the pairs
            }     // end of calculation of fEnergyRaw for triangular shape anodes
            if (nRaw > 0)
                sumRaw = fTrimHitPar->GetEnergyAlignOffset(s + 1) +
                         fTrimHitPar->GetEnergyAlignGain(s + 1) * (sumRaw / (Float_t)nRaw);

            double p0[3];
            double p1[3];

            // FIXME
            p0[0] = 11.78;
            p0[1] = 9.706;
            p0[2] = 11.690;
            p1[0] = 0.463;
            p1[1] = 0.47543;
            p1[2] = 0.4622;

            // TO DO : === fEnergyTheta: fEnergyBeta corrected from the theta angle in the Triple-MUSIC ===
            sumTheta = sumRaw;

            // TO DO : === fEnergyDT: fEnergyDT corrected from the X position in the Triple-MUSIC ===
            sumDT = sumTheta;

            // TO DO : === fZ ===
            zval = p0[s] + p1[s] * TMath::Sqrt(sumDT);

            // FILL HIT DATA
            AddHitData(s + 1, eal[0], eal[1], eal[2], sumRaw, sumRaw, sumTheta, sumDT, zval);
        } // end of loop over the sections
    }

    return;
}

// -----   Protected method Finish   --------------------------------------------
void R3BSofTrimCal2Hit::Finish() {}

// -----   Public method Reset   ------------------------------------------------
void R3BSofTrimCal2Hit::Reset()
{
    LOG(DEBUG) << "Clearing TrimHiData Structure";
    if (fTrimHitData)
        fTrimHitData->Clear();
}

// -----   Private method AddCalData  --------------------------------------------
R3BSofTrimHitData* R3BSofTrimCal2Hit::AddHitData(Int_t secID,
                                                 Float_t e1,
                                                 Float_t e2,
                                                 Float_t e3,
                                                 Float_t Eraw,
                                                 Float_t Ebeta,
                                                 Float_t Etheta,
                                                 Float_t Edt,
                                                 Float_t Z)
{
    TClonesArray& clref = *fTrimHitData;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTrimHitData(secID, e1, e2, e3, Eraw, Ebeta, Edt, Etheta, Z);
}

ClassImp(R3BSofTrimCal2Hit)
