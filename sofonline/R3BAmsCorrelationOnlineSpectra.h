// ------------------------------------------------------------
// -----          R3BAmsCorrelationOnlineSpectra          -----
// -----    Created 31/01/20  by J.L. Rodriguez-Sanchez   -----
// -----     Fill AMS-CALIFA-MUSICs online histograms     -----
// ------------------------------------------------------------

#ifndef R3BAmsCorrelationOnlineSpectra_H
#define R3BAmsCorrelationOnlineSpectra_H

#include "FairTask.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TMath.h"
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

class TClonesArray;
class R3BEventHeader;

/**
 * This taks reads AMS-CALIFA-MUSICs data and plots correlated online histograms
 */
class R3BAmsCorrelationOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BAmsCorrelationOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BAmsCorrelationOnlineSpectra(const TString& name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BAmsCorrelationOnlineSpectra();

    /**
     * Method for task initialization.
     * This function is called by the framework before
     * the event loop.
     * @return Initialization status. kSUCCESS, kERROR or kFATAL.
     */
    virtual InitStatus Init();

    /**
     * Method for event loop implementation.
     * Is called by the framework every time a new event is read.
     * @param option an execution option.
     */
    virtual void Exec(Option_t* option);

    /**
     * A method for finish of processing of an event.
     * Is called by the framework for each event after executing
     * the tasks.
     */
    virtual void FinishEvent();

    /**
     * Method for finish of the task execution.
     * Is called by the framework after processing the event loop.
     */
    virtual void FinishTask();

    /** Accessor to set up the atomic number of projectile **/
    void SetZproj(Float_t zproj) { fZproj = zproj; }

    /**
     * Method to select binning and max range for CALIFA histograms
     */
    inline void SetCalifa_bins_maxrange(Int_t Hist_bins, Int_t Hist_max)
    {
        fHitCalifaHist_bins = Hist_bins;
        fHitCalifaHist_max = Hist_max;
    }

    /**
     * Method for setting min proton energy (in keV) for opening angle histogram
     */
    inline void SetMinProtonEnergyForOpening(Float_t min) { fMinProtonE = min; }

    void Reset_Histo();

  private:
    TClonesArray* fHitItemsAms;    /**< Array with ams-hit items. */
    TClonesArray* fHitItemsTwim;   /**< Array with twim-hit items. */
    TClonesArray* fHitItemsMus;    /**< Array with music-hit items. */
    TClonesArray* fHitItemsCalifa; /**< Array with califa-hit items. */
    TClonesArray* fHitItemsTrim;   /**< Array with triple music-hit items. */

    Int_t fHitCalifaHist_max;
    Int_t fHitCalifaHist_bins;

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* fEventHeader; /**< Event header.      */
    Int_t fTrigger;               /**< Trigger value. */
    Int_t fNEvents;               /**< Event counter. */
    Float_t fZproj;               // Atomic number of projectile
    Float_t fMinProtonE;          /**< Min proton energy (in keV) to calculate the opening angle */

    TCanvas* cHit[6];
    TCanvas* cHitAngles;
    TCanvas* cHitEnergyCor;
    TCanvas* cHitThetaCor;
    TCanvas* cHitPhiCor;

    TCanvas* cCalifaMult;
    TCanvas* cCalifaCoinE;
    TCanvas* cCalifaCoinPhi;
    TCanvas* cCalifaCoinTheta;
    TCanvas* cCalifa_angles;
    TCanvas* cCalifa_theta_energy;
    TCanvas* cCalifa_hitenergy;
    TCanvas* cCalifa_opening;

    TH2F* fh_Ams_hit_Pos[6];
    TH2F* fh_Ams_hit_E[6];
    TH2F* fh_Ams_hit_E_theta[6];
    TH1F* fh_Ams_hit_Mul[6];
    TH2F* fh2_ams_theta_phi[2];
    TH2F* fh2_ams_e1_e2[2];
    TH2F* fh2_ams_theta1_theta2[2];
    TH2F* fh2_ams_phi1_phi2[2];

    TH1F* fh1_Califa_MultHit;
    TH2F* fh2_Califa_coinE;
    TH2F* fh2_Califa_coinTheta;
    TH2F* fh2_Califa_coinPhi;
    TH2F* fh2_Califa_theta_phi;
    TH2F* fh2_Califa_theta_energy;
    TH1F* fh1_Califa_total_energy;
    TH1F* fh1_openangle;

    // TString fAmsFile;        	      /**< Config file name. */
    Int_t fNbDet; /**< Number of AMS detectors. */

  public:
    ClassDef(R3BAmsCorrelationOnlineSpectra, 1)
};

#endif
