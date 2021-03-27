// ------------------------------------------------------------
// -----                R3BSofCorrOnlineSpectra           -----
// ------------------------------------------------------------

#ifndef R3BSofCorrOnlineSpectra_H
#define R3BSofCorrOnlineSpectra_H

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

class TClonesArray;
class R3BEventHeader;

/**
 * This taks reads FRS data and plots online histograms
 */
class R3BSofCorrOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofCorrOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofCorrOnlineSpectra(const char* name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofCorrOnlineSpectra();

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

    void SetFirstXCorrv(Int_t x) { fFirstX_Corrv = x; }
    void SetLastXCorrv(Int_t x) { fLastX_Corrv = x; }
    void SetNsPerBin_Corrv() { fNsPerBin_Corrv = (fLastX_Corrv - fFirstX_Corrv) / 5.; }
    void SetVftxModuleId_Corrv(Int_t id) { fVftxModuleId_Corrv = id; }

  private:
    TClonesArray* fCorrmMapped;    /**< Array with mapped items. */
    TClonesArray* fCorrvMapped;    /**< Array with mapped items. */
    TClonesArray* fVftxTrigMapped; /**< Array with mapped items. */

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */
    Int_t fReset;

    Int_t fFirstX_Corrv;
    Int_t fLastX_Corrv;
    Double_t fNsPerBin_Corrv;
    Int_t fVftxModuleId_Corrv;

    // Canvas
    TCanvas* cMapFT_CorrV;
    TCanvas* cMap_DeltaT;
		TCanvas* cMap_Corr;

    // Histograms for Mapped data
    TH1F* fh1_FineTime_CorrV;
    TH1F** fh1_DeltaCorrTrig;
    TH2F** fh2_Correlation;

  public:
    ClassDef(R3BSofCorrOnlineSpectra, 0)
};

#endif
