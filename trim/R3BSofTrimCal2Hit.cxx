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
            fTrimHitPar->SetNumAlignGainsPerSection(3);
        }
        else
        {
            LOG(INFO) << "Triple-MUSIC has rectangular shape anodes";
            fTrimHitPar->SetNumAlignGainsPerSection(6);
        }
        LOG(INFO) << "R3BSofTrimCal2Hit:: trimHitPar container open ";
    }
}

// -----   Public method Init   --------------------------------------------
InitStatus R3BSofTrimCal2Hit::Init()
{
    LOG(INFO) << "R3BSofTrimCal2Hit: Init";

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

    if (!fOnline)
    {
        rootManager->Register("TrimHitData", "Trim Hit", fTrimHitData, kTRUE);
    }
    else
    {
        rootManager->Register("TrimHitData", "Trim Hit", fTrimHitData, kFALSE);
    }

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
    // Reset entries in output arrays, local arrays
    Reset();

    if (!fTrimHitData)
    {
        return;
    }

    // Get the parameters
    if (!fTrimHitPar)
    {
        LOG(ERROR) << "R3BSofTrimCal2Hit::Exec() --->  no TrimHitPar Container found";
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
    Float_t sumRaw, sumBeta, sumDT, sumTheta, zval, ePair, dtPair, dtSection,Ddt;
    Double_t correction;
    Float_t EpairVsDT_p0[fNumSections*nAligned];
    Float_t EpairVsDT_p1[fNumSections*nAligned];
    Float_t EpairVsDT_p2[fNumSections*nAligned];
    Float_t EpairVsDT_p3[fNumSections*nAligned];

	EpairVsDT_p0[0]=27549.8;
	EpairVsDT_p0[1]=29515.2;
	EpairVsDT_p0[2]=32100;
	EpairVsDT_p0[3]=28230.2;
	EpairVsDT_p0[4]=29915.9;
	EpairVsDT_p0[5]=31560.7;
	EpairVsDT_p0[6]=28905.1;
	EpairVsDT_p0[7]=26874.7;
	EpairVsDT_p0[8]=27567.5;

	EpairVsDT_p1[0]=-0.475275;
	EpairVsDT_p1[1]=-0.899539;
	EpairVsDT_p1[2]=-0.751308;
	EpairVsDT_p1[3]=0.15374;
	EpairVsDT_p1[4]=0.0112768;
	EpairVsDT_p1[5]=0.00240945;
	EpairVsDT_p1[6]=-0.230107;
	EpairVsDT_p1[7]=-0.473468;
	EpairVsDT_p1[8]=-0.785311;

	EpairVsDT_p2[0]=0.000496605;
	EpairVsDT_p2[1]=0.000415774;
	EpairVsDT_p2[2]=0.000265675;
	EpairVsDT_p2[3]=-6.07408e-05;
	EpairVsDT_p2[4]=1.35224e-05;
	EpairVsDT_p2[5]=4.9109e-05;
	EpairVsDT_p2[6]=0.000258047;
	EpairVsDT_p2[7]=0.000458344;
	EpairVsDT_p2[8]=0.000600714;

	EpairVsDT_p3[0]=-5.87221e-08;
	EpairVsDT_p3[1]=-3.92877e-08;
	EpairVsDT_p3[2]=-2.27698e-08;
	EpairVsDT_p3[3]=1.3715e-08;
	EpairVsDT_p3[4]=1.46428e-09;
	EpairVsDT_p3[5]=-4.32464e-09;
	EpairVsDT_p3[6]=-2.99696e-08;
	EpairVsDT_p3[7]=-5.77546e-08;
	EpairVsDT_p3[8]=-7.44682e-08;

	Float_t EsectionVsTheta_p0[3];
	Float_t EsectionVsTheta_p1[3];
	Float_t EsectionVsTheta_p2[3];
	EsectionVsTheta_p0[0] =19801.;
	EsectionVsTheta_p1[0] =-0.152059;
	EsectionVsTheta_p2[0] =3.1925e-05;
	EsectionVsTheta_p0[1] =19980.8;
	EsectionVsTheta_p1[1] =0.250359;
	EsectionVsTheta_p2[1] =0.000259866;
	EsectionVsTheta_p0[2] =19888.1;
	EsectionVsTheta_p1[2] =0.325335;
	EsectionVsTheta_p2[2] =0.000579842;


	Float_t gain[3];
	gain[0] = 0.1817686086;
        gain[1] = 0.0458905053;
        gain[2] = 0.0382460368;

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
            eal[ch + s * nAligned] = 0;
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
    Ddt = 0.5*(dt[2]+dt[3]) - 0.5*(dt[14]+dt[15]);
    for (Int_t s = 0; s < fNumSections; s++)
    {
	dtSection = 0.5*(dt[2+s*fNumAnodes]+dt[3+s*fNumAnodes]);
        if( dtSection < 0 || dtSection > 5000 )
        AddHitData(s + 1, -1, -1, -1, -1, -1);
	else{
	
	// === fEnergyRaw: sum of Aligned Energy === //
        sumRaw = 0.;
        nRaw = 0;
        // if Rectangular shape:
        if (fTriShape == kFALSE)
        {
            for (Int_t a = 0; a < fNumAnodes; a++)
            {
                if (mult[a + s * fNumAnodes] == 1)
                {
                    eal[a + s * fNumAnodes] = e[a + s * fNumAnodes] * fTrimHitPar->GetEnergyAlignGain(s + 1, a);
                    sumRaw += eal[a + s * fNumAnodes];
                    nRaw++;
                }
            } // end of loop over the anodes
        }     // end calculation of fEnergyRaw for rectangular shape anodes
        else
        {
            for (Int_t ch = 0; ch < nAligned; ch++)
            {
                if (mult[2 * ch + s * fNumAnodes] == 1 && mult[2 * ch + 1 + s * fNumAnodes] == 1 && fTrimHitPar->GetEnergyAlignGain(s + 1, ch) > 0)
                {
                    ePair = (e[2 * ch + s * fNumAnodes] + e[2 * ch + 1 + s * fNumAnodes]) *
                                             fTrimHitPar->GetEnergyAlignGain(s + 1, ch);
		    dtPair = 0.5*(dt[2 * ch + s * fNumAnodes] + dt[2 * ch + 1 + s * fNumAnodes]);
                    eal[ch+s*nAligned] = ePair*EpairVsDT_p0[ch+s*nAligned] / 
			               (
					EpairVsDT_p0[ch+s*nAligned]+
				    	EpairVsDT_p1[ch+s*nAligned]*(TMath::Power(dtPair,1))+
					EpairVsDT_p2[ch+s*nAligned]*(TMath::Power(dtPair,2))+
					EpairVsDT_p3[ch+s*nAligned]*(TMath::Power(dtPair,3))
					);
		    sumRaw += eal[ch + s * nAligned];
                    nRaw++;
                }
            } // end of loop over the anodes
        }     // end of calculation of fEnergyRaw for triangular shape anodes
        if (nRaw > 0)
            sumRaw = sumRaw / (Float_t)nRaw;

        // === fEnergyBeta: fEnergyRaw corrected from the beam velocity === //
        correction = 0;
        for (Int_t deg = 0; deg < fTrimHitPar->GetNumCorrBetaParsPerSection(); deg++)
        {
            correction += TMath::Power(betaFromS2, deg) * fTrimHitPar->GetEnergyCorrBetaPar(s + 1, deg);
        }
        if (correction != 0)
        {
            sumBeta = gain[s] * fTrimHitPar->GetEnergyCorrBetaPar(s + 1, 0) * sumRaw / correction;
        }
        else
        {
            sumBeta = sumRaw;
        }

        // TO DO : === fEnergyDT: fEnergyBeta corrected from the X position in the Triple-MUSIC ===
        sumDT = sumBeta;

        // TO DO : === fEnergyTheta: fEnergyDT corrected from the theta angle in the Triple-MUSIC ===
	//sumTheta = EsectionVsTheta_p0[s]*sumDT / (
	//		EsectionVsTheta_p0[s]+
	//		EsectionVsTheta_p1[s]*Ddt+
	//		EsectionVsTheta_p1[s]*TMath::Power(Ddt,2)
	//		);
        if(s==1){
	sumTheta = sumDT * 17461./17177.;
	}
	else{
	sumTheta = sumDT;
	}
	// TO DO : === fZ ===
        zval = 0.695*TMath::Sqrt(sumTheta) - 5.17 ;
        // FILL HIT DATA
        AddHitData(s + 1, sumRaw, sumBeta, sumDT, sumTheta, zval);
	}
    } // end of loop over the sections

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
                                                 Float_t Eraw,
                                                 Float_t Ebeta,
                                                 Float_t Edt,
                                                 Float_t Etheta,
                                                 Float_t Z)
{
    TClonesArray& clref = *fTrimHitData;
    Int_t size = clref.GetEntriesFast();
    return new (clref[size]) R3BSofTrimHitData(secID, Eraw, Ebeta, Edt, Etheta, Z);
}

ClassImp(R3BSofTrimCal2Hit)
