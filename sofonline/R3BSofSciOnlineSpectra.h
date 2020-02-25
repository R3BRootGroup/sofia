// ------------------------------------------------------------
// -----                R3BSofSciOnlineSpectra            -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofSciOnlineSpectra_H
#define R3BSofSciOnlineSpectra_H

#include "FairTask.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TMath.h"
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "R3BSofSciHitData.h"

#include "detectors_cfg.h"

#define NbDetectors NUMBER_OF_SOFSCI_DETECTORS
#define NbChannels  NUMBER_OF_SOFSCI_CHANNELS

#ifdef NUMBER_OF_SOFSCI_TOF
#define NbTof NUMBER_OF_SOFSCI_TOF
#else
#define NbTof 0
#endif

class TClonesArray;
class R3BEventHeader;

/**
 * This taks reads SCI data and plots online histograms
 */
class R3BSofSciOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofSciOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofSciOnlineSpectra(const char* name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofSciOnlineSpectra();

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

    /**
     * Methods to clean histograms.
     */
    virtual void Reset_Histo();

    /** Virtual method Reset **/
    virtual void Reset();

  private:
    TClonesArray* fMappedItemsSci; /**< Array with mapped items. */
    TClonesArray* fTcalItemsSci;   /**< Array with tcal items. */
    TClonesArray* fSingleTcalItemsSci; /**< Array with tcal items. */
    TClonesArray* fMusHitItems;   /**< Array with MUSIC Hit items. */
    TClonesArray* fMusCalItems;   /**< Array with MUSIC Cal items. */
    TClonesArray* fCalItemsMwpc0; /**< Array with cal items of mwpc0. */
    TClonesArray* fTofwHitData;

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */

    // Canvas
    TCanvas* cSciMult[NbDetectors];
    TCanvas* cSciRawPos[NbDetectors];
    TCanvas* cMusicZvsRawPos[NbDetectors];
#ifdef NUMBER_OF_SOFSCI_TOF
    TCanvas* cSciRawTof[NbTof];
    TCanvas* cMusicZvsRawTof[NbTof];
    TCanvas* cAqvsq; 
#endif

    // Histograms for Mapped data : Fine Time and Mult
    TH1I* fh1_finetime[NbDetectors * NbChannels];
    TH2I* fh2_mult[NbDetectors];

    // Histograms for PosRaw Data at Tcal and SingleTcal
    TH1F* fh1_RawPos_AtTcalMult1[NbDetectors];
    TH1F* fh1_RawPos_AtSingleTcal[NbDetectors];
#ifdef NUMBER_OF_SOFSCI_TOF
    TH1D* fh1_RawTof_AtTcalMult1[NbTof];
    TH1D* fh1_RawTof_AtTcalMult1_wTref[NbTof];
    TH1D* fh1_RawTof_AtSingleTcal_wTref[NbTof];
#endif
    //    TH1F * fh1_RawPos_AtSingleTcal[NbDetectors];

    // Histogram for correlation with R3B-Music
    TH2F* fh2_MusZvsRawPos[NbDetectors];
    TH2F* fh2_MusDTvsRawPos;
#ifdef NUMBER_OF_SOFSCI_TOF
    TH2F* fh2_MusZvsRawTof[NbTof];
    TH2F* fh2_Aqvsq;
#endif

    // Histogram for correlation with Mwpc0
    TH2F* fh2_Mwpc0vsRawPos;

    // check how many raw pos found


    /** Private method TofwHitData **/
    //** Adds a TofwHitData to the detector
    R3BSofSciHitData* AddHitData(Int_t paddle, Double_t X, Double_t time);

  public:
    ClassDef(R3BSofSciOnlineSpectra, 1)
};

#endif
