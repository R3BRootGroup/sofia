// ------------------------------------------------------------
// -----              R3BSofTofWOnlineSpectra             -----
// -----           Fill SOFIA online histograms           -----
// ------------------------------------------------------------

#ifndef R3BSofTofWOnlineSpectra_H
#define R3BSofTofWOnlineSpectra_H

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

// to be fixed
#define NbDets 28
#define NbChs 2

class TClonesArray;
class R3BEventHeader;

/**
 * This taks reads SCI data and plots online histograms
 */
class R3BSofTofWOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofTofWOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofTofWOnlineSpectra(const char* name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofTofWOnlineSpectra();

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
    inline void Set_IdSofSciCaveC(Int_t id) { fIdSofSciCaveC = id; }

  private:
    TClonesArray* fMappedItemsTofW;     /**< Array with tofw mapped items. */
    TClonesArray* fTcalItemsTofW;       /**< Array with tofw tcal items. */
    TClonesArray* fSingleTcalItemsTofW; /**< Array with tofw single tcal items. */
    TClonesArray* fHitItemsTofW;        /**< Array with tofw hit items. */
    TClonesArray* fSingleTcalItemsSci;  /**< Array with single tcal items of Sci */
    TClonesArray* fHitItemsTwim;        /**< Array with hit items of twim. */
    TClonesArray* fCalItemsMwpc;        /**< Array with cal items of mwpc3. */
    Float_t fTwimTofRangeMax;           // Range for Twim vs ToF histograms
    Float_t fTwimTofRangeMin;
    Int_t fIdSofSciCaveC;

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */

    // Canvas
    TCanvas* cTofWFineTime[NbChs];
    TCanvas* cTofWMult;
    TCanvas* cTofWRawPos;
    TCanvas* cTofWRawPosST;
    TCanvas* cTofWEneRaw[NbChs];
    TCanvas* cTofWRawTof[NbDets];
    TCanvas* cTwimvsTof[NbDets];
    TCanvas* cMwpc3XvsTof;
    TCanvas* cMwpc3YvsPosTof[NbDets];
    TCanvas* cTofvsplas;
    TCanvas* cPosvsplas;

    // Histograms for Mapped data : Fine Time and Mult
    TH1I* fh1_finetime[NbDets * NbChs];
    TH2I* fh2_mult[NbChs];

    // Histograms for PosRaw Data
    TH1F* fh1_RawPos_AtTcalMult1[NbDets];
    TH1F* fh1_RawPos_AtSingleTcal[NbDets];

    // Histograms for ToFraw
    TH1D* fh1_RawTof_AtTcalMult1[NbDets];
    TH1D* fh1_RawTof_AtSingleTcal[NbDets];

    // Histograms for EneRaw
    TH1D* fh1_EneRaw[NbDets * NbChs];

    // Histograms for ToF-hit data
    TH2F* fh2_Tof_hit;
    TH2F* fh2_Pos_hit;

    // Histograms for Twim vs ToFraw
    TH2F* fh2_Twim_Tof[NbDets];

    // Histograms for Mwpc3 vs ToF-plastic
    TH2F* fh2_Mwpc3X_Tof;
    TH2F* fh2_Mwpc3Y_PosTof[NbDets];

  public:
    ClassDef(R3BSofTofWOnlineSpectra, 1)
};

#endif
