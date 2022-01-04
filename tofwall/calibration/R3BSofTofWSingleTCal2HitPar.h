// -----------------------------------------------------------------
// -----                                                       -----
// -----                R3BSofTofWSingleTCal2HitPar            -----
// -----        Created 29/01/20  by J.L. Rodriguez-Sanchez    -----
// -----------------------------------------------------------------

#ifndef R3BSofTofWSingleTCal2HitPar_H
#define R3BSofTofWSingleTCal2HitPar_H

#include "FairTask.h"
#include "TH1F.h"

class TClonesArray;
class R3BSofTofWHitPar;

class R3BSofTofWSingleTCal2HitPar : public FairTask
{
  public:
    /** Default constructor **/
    R3BSofTofWSingleTCal2HitPar();

    /** Standard constructor **/
    R3BSofTofWSingleTCal2HitPar(const TString& name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofTofWSingleTCal2HitPar();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* option);

    /** Virtual method FinishEvent **/
    virtual void FinishEvent();

    /** Virtual method FinishTask **/
    virtual void FinishTask();

    /** Virtual method Reset **/
    virtual void Reset();

    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /** Method to set up the limits for fit tof **/
    void SetFitLimitsTof(Int_t left, Int_t right)
    {
        fLimit_left_tof = left;
        fLimit_right_tof = right;
    }

    /** Method to set up the limits for fit pos **/
    void SetFitLimitsPos(Int_t left, Int_t right)
    {
        fLimit_left_pos = left;
        fLimit_right_pos = right;
    }

    void SetMinStatistics(Int_t minstat) { fMinStatistics = minstat; }

  private:
    Int_t fNumSci;
    Int_t fMinStatistics;
    Int_t fLimit_left_tof;
    Int_t fLimit_right_tof;
    Int_t fNbBinsTof;
    Int_t fLimit_left_pos;
    Int_t fLimit_right_pos;
    Int_t fNbBinsPos;
    Int_t fMaxSigma;
    TArrayF* TofParams;
    TArrayF* PosParams;

    TH1F** htof;
    TH1F** hpos;

    R3BSofTofWHitPar* fHit_Par;  /**< Parameter container. >*/
    TClonesArray* fTofCalDataCA; /**< Array with Tof-Tcal data. >*/

  public:
    // Class definition
    ClassDef(R3BSofTofWSingleTCal2HitPar, 1)
};

#endif
