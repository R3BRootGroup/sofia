// -----------------------------------------------------------------
// -----                                                       -----
// -----                R3BSofFrsAnalysis                      -----
// -----        Created 23/01/20  by J.L. Rodriguez-Sanchez    -----
// -----        Revised 07/08/20  by R. Taniuchi               -----
// -----------------------------------------------------------------

#ifndef R3BSofFrsAnalysis_H
#define R3BSofFrsAnalysis_H

// ROOT headers
#include "TClonesArray.h"
#include "TMath.h"

// Fair headers
#include "FairTask.h"
#include "FairLogger.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

// R3B headers
#include "R3BMusicHitPar.h"
#include "R3BMusicHitData.h"

// SOFIA headers
#include "R3BSofFrsAnaPar.h"
#include "R3BSofFrsData.h"
#include "R3BSofSciSingleTcalData.h"

class TClonesArray;

class R3BSofFrsAnalysis : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofFrsAnalysis();

    /** Standard constructor **/
    R3BSofFrsAnalysis(const TString& name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofFrsAnalysis();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* option);

    /** Virtual method Reset **/
    virtual void Reset();

    /**
     * A method for finish of processing of an event.
     * Is called by the framework for each event after executing
     * the tasks.
     */
    virtual void FinishEvent() {Reset();}
    
    virtual void SetParContainers();

    // Fair specific
    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /** Virtual method Finish **/
    virtual void Finish();

    /** Accessor to select online mode **/
    void SetOnline(Bool_t option) { fOnline = option; }

    /** Accessor functions **/
    void SetNbSci(UChar_t ndets) {fNbSci = ndets;}
    void SetIdS2(UChar_t id) {fIdS2 = id;}
    void SetIdS8(UChar_t id) {fIdS8 = id;}
    UChar_t GetNbSci() {return fNbSci;}
    UChar_t GetIdS2() {return fIdS2;}
    UChar_t GetIdS8() {return fIdS8;}

  private:
    TClonesArray* fSingleTcalItemsSci; /**< Array with tcal items. */
    //TClonesArray* fMwpcHitDataCA;  /**< Array with Mwpc Hit-input data. >*/
    TClonesArray* fMusicHitDataCA; /**< Array with Music Hit-input data. >*/
    //TClonesArray* fSciHitDataCA;   /**< Array with Sci Hit-input data. >*/
    TClonesArray* fFrsDataCA;      /**< Array with FRS-output data. >*/

    void SetParameter();

    // Parameters set at the construction
    R3BSofFrsAnaPar* fFrs_Par; // Parameter container
    Bool_t fOnline; // Don't store data for online    UChar_t fNbSci;
    UChar_t fNbTof;
    UChar_t fNbSci;
    UChar_t fIdS2;
    UChar_t fIdS8;
    UChar_t fIdCave;

    // Parameter containers for FRSAnaPar
    Double_t fBrho0;  //Brho setting in FRS S2-S8
    UChar_t* fStaId;
    UChar_t* fStoId;
    Double_t* fPathLength;
    Double_t* fTofOffset;
    Int_t* fUseS2x;
    Double_t fS2SciCoef0, fS2SciCoef1;

    // Parameter containers for R3BMusicPar
    UChar_t fNumMusicParams;
    TArrayF* CalZParams;
    Float_t fZ0 = 0., fZ1 = 0. , fZ2 = 0.; // CalibPar for R3BMUSIC
    Double_t MusicZ = -5000., MusicE = -5000.;
    
    Double_t* xpos;
    Float_t Tof_wTref_S2_Cave = -5000., Tof_wTref_S2_S8 = -5000., Tof_wTref_S8_Cave = -5000.;
    
    /** Private method FrsData **/
    //** Adds a FrsData to the analysis
    R3BSofFrsData* AddData(Int_t StaId, Int_t StoId, Double_t z, Double_t aq, Double_t betaval, Double_t brhoval, Double_t xs2, Double_t xc);

  public:
    // Class definition
    ClassDef(R3BSofFrsAnalysis, 1)
};

#endif
