// ------------------------------------------------------------
// -----              R3BSofToFWOnlineSpectra             -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofToFWOnlineSpectra_H
#define R3BSofToFWOnlineSpectra_H

#include "FairTask.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2F.h"
#include "TMath.h"
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#define NbDets 28
#define NbChs 2

class TClonesArray;
class R3BEventHeader;

/**
 * This taks reads SCI data and plots online histograms
 */
class R3BSofToFWOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofToFWOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofToFWOnlineSpectra(const char* name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofToFWOnlineSpectra();

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

    inline void Set_TwimvsTof_range(Float_t min, Float_t max)
    {
        fTwimTofRangeMin = min;
        fTwimTofRangeMax = max;
    }

  private:
    TClonesArray* fMappedItemsToFW;    /**< Array with mapped items. */
    TClonesArray* fTcalItemsToFW;      /**< Array with tcal items. */
    TClonesArray* fSingleTcalItemsSci; /**< Array with single tcal items of Sci */
    TClonesArray* fHitItemsTwim;       /**< Array with hit items of twim. */
    TClonesArray* fCalItemsMwpc;       /**< Array with cal items of mwpc3. */
    Float_t fTwimTofRangeMax;          // Range for Twim vs ToF histograms
    Float_t fTwimTofRangeMin;

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */

    // Canvas
    TCanvas* cToFWFineTime[NbChs];
    TCanvas* cToFWMult;
    TCanvas* cToFWRawPos;
    TCanvas* cToFWEneRaw[NbChs];
    TCanvas* cToFWRawTof[NbDets];
    TCanvas* cTwimvsTof[NbDets];
    TCanvas* cMwpc3XvsTof;
    TCanvas* cMwpc3YvsPosTof[NbDets];

    // Histograms for Mapped data : Fine Time and Mult
    TH1I* fh1_finetime[NbDets * NbChs];
    TH2I* fh2_mult[NbChs];

    // Histograms for PosRaw Data
    TH1F* fh1_RawPos_AtTcalMult1[NbDets];

    // Histograms for ToFraw
    TH1D* fh1_RawTof_AtTcalMult1[NbDets];

    // Histograms for EneRaw
    TH1D* fh1_EneRaw[NbDets * NbChs];

    // Histograms for Twim vs ToFraw
    TH2F* fh2_Twim_Tof[NbDets];

    // Histograms for Mwpc3 vs ToF-plastic
    TH2F* fh2_Mwpc3X_Tof;
    TH2F* fh2_Mwpc3Y_PosTof[NbDets];

  public:
    ClassDef(R3BSofToFWOnlineSpectra, 1)
};

#endif
