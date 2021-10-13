#ifndef R3BSofTrimOnlineSpectra_H
#define R3BSofTrimOnlineSpectra_H

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
 * This taks reads TWIM data and plots online histograms
 */
class R3BSofTrimOnlineSpectra : public FairTask
{

  public:
    /**
     * Default constructor.
     * Creates an instance of the task with default parameters.
     */
    R3BSofTrimOnlineSpectra();

    /**
     * Standard constructor.
     * Creates an instance of the task.
     * @param name a name of the task.
     * @param iVerbose a verbosity level.
     */
    R3BSofTrimOnlineSpectra(const TString& name, Int_t iVerbose = 1);

    /**
     * Destructor.
     * Frees the memory used by the object.
     */
    virtual ~R3BSofTrimOnlineSpectra();

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

    void SetNumSections(Int_t num) { fNumSections = num; }
    void SetNumAnodes(Int_t num) { fNumAnodes = num; }
    void SetNumPairs(Int_t num) { fNumPairs = num; }
    void SetNumTref(Int_t num) { fNumTref = num; }
    void SetNumTtrig(Int_t num) { fNumTtrig = num; }

  private:
    Int_t fNumSections;
    Int_t fNumAnodes;
    Int_t fNumPairs;
    Int_t fNumTref;
    Int_t fNumTtrig;

    TClonesArray* fMappedItemsTrim; /**< Array with mapped items. */
    TClonesArray* fCalItemsTrim;    /**< Array with cal items. */
    TClonesArray* fHitItemsTrim;    /**< Array with hit items. */

    // check for trigger should be done globablly (somewhere else)
    R3BEventHeader* header; /**< Event header.      */
    Int_t fNEvents;         /**< Event counter.     */

    // Canvas for Mapped data
    TCanvas** cTrimMap_E;    //[NbSectionsTrim];
    TCanvas** cTrimMap_DT;   //[NbSectionsTrim];
    TCanvas** cTrimMap_Mult; //[NbSectionsTrim];
    TCanvas* cTrimMap_DeltaTrefTtrig;
    TCanvas** cTrimMap_EvsDT;  //[NbSectionsTrim];
    TCanvas** cTrimMap_DTvsDT; //[NbSectionsTrim];

    // Histograms for Mapped data
    TH1F** fh1_trimmap_E;              //[NbSectionsTrim][NbAnodesTrim];
    TH1F** fh1_trimmap_DT;             //[NbSectionsTrim][NbAnodesTrim];
    TH1F** fh1_trimmap_Mult;           //[NbSectionsTrim];
    TH1F** fh1_trimmap_DeltaTrefTtrig; //[NbSectionsTrim];
    TH2F** fh2_trimmap_EvsDT;          //[NbSectionsTrim][NbAnodesTrim];
    TH2F** fh2_trimmap_DTvsDT;         //[NbSectionsTrim][NbAnodesTrim-1];

    // Canvas for Cal data
    TCanvas** cTrimCal_Ene;
    TCanvas** cTrimCal_DT;
    TCanvas* cTrimCal_EnePair;
    TCanvas* cTrimCal_EnePairVsDT;
    TCanvas* cTrimCal_EnePairVsDeltaDT;

    // Histograms for Cal data
    TH1F** fh1_trimcal_Esub;
    TH1F** fh1_trimcal_Ematch;
    TH1D** fh1_trimcal_DTraw;
    TH1D** fh1_trimcal_DTalign;
    TH1F** fh1_trimcal_EmatchPair;
    TH2F** fh2_trimcal_EnePairVsDT;
    TH2F** fh2_trimcal_EnePairVsDeltaDT;

    // Canvas for Hit data
    TCanvas* cTrimHit_ErawPair;
    TCanvas* cTrimHit_E;
    TCanvas* cTrimHit_Z;
    TCanvas** cTrimHit_CorrDep;
    TCanvas* cTrimHit_EvsE;
    TCanvas* cTrimHit_ZvsZ;
    TCanvas* cTrimHit_Emax;

    // Histograms for Hit data
    TH1F** fh1_trimhit_ErawPair;
    TH1F** fh1_trimhit_Eraw;
    TH1F** fh1_trimhit_Ebeta;
    TH1F** fh1_trimhit_Edt;
    TH1F** fh1_trimhit_Etheta;
    TH1F** fh1_trimhit_Z;
    TH2F** fh2_trimhit_Eraw_vs_DT;
    TH2F** fh2_trimhit_Ebeta_vs_DT;
    TH2F** fh2_trimhit_Edt_vs_DT;
    TH2F** fh2_trimhit_Edt_vs_theta;
    TH2F** fh2_trimhit_Etheta_vs_theta;
    TH2F** fh2_trimhit_EvsE;
    TH2F** fh2_trimhit_ZvsZ;
    TH1F* fh1_trimhit_Emax;

  public:
    ClassDef(R3BSofTrimOnlineSpectra, 1)
};

#endif
