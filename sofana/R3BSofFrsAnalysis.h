// ----------------------------------------------------------------------
// -----                                                            -----
// -----                     R3BSofFrsAnalysis                      -----
// -----             Created 23/01/20  by J.L. Rodriguez-Sanchez    -----
// ----------------------------------------------------------------------

#ifndef R3BSofFrsAnalysis_H
#define R3BSofFrsAnalysis_H

#include "FairTask.h"
#include "R3BSofFrsData.h"
#include "TF1.h"
#include "TH1F.h"
#include <TRandom.h>

class TClonesArray;
class R3BSofFrsAnaPar;

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
    void SetOffsetAq(Double_t theAq) { fOffsetAq = theAq; }
    void SetOffsetZ(Double_t theZ) { fOffsetZ = theZ; }

  private:
    void SetParameter();

    R3BSofFrsAnaPar* fFrs_Par; // Parameter container

    Double_t fOffsetAq, fOffsetZ; // Offsets in A/q and Z
    Double_t fMagS2Cave;
    Double_t fDispS2Cave;
    Double_t fPathS2Cave, fTOFS2Cave;
    Double_t fPosFocalS2, fPosFocalCave;
    Double_t frho_S0_S2, fBfield_S0_S2, frho_S2_Cave, fBfield_S2_Cave;

    Bool_t fOnline; // Don't store data for online

    TClonesArray* fMwpcHitDataCA;  /**< Array with Mwpc Hit-input data. >*/
    TClonesArray* fMusicHitDataCA; /**< Array with Music Hit-input data. >*/
    TClonesArray* fSciHitDataCA;   /**< Array with Sci Hit-input data. >*/
    TClonesArray* fFrsDataCA;      /**< Array with FRS-output data. >*/

    /** Private method FrsData **/
    //** Adds a FrsData to the analysis
    R3BSofFrsData* AddData(Double_t z, Double_t aq, Double_t beta, Double_t brho);

  public:
    // Class definition
    ClassDef(R3BSofFrsAnalysis, 1)
};

#endif
