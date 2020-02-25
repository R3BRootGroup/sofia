// -----------------------------------------------------------------
// -----                                                       -----
// -----                R3BSofTofWTcal2HitPar                  -----
// -----        Created 29/01/20  by J.L. Rodriguez-Sanchez    -----
// -----------------------------------------------------------------

#ifndef R3BSofTofWTcal2HitPar_H
#define R3BSofTofWTcal2HitPar_H

#include "FairTask.h"
#include "R3BSofToFWSingleTcalData.h"
#include "TH1F.h"

class TClonesArray;
class R3BSofTofWHitPar;

class R3BSofTofWTcal2HitPar : public FairTask
{
  public:
    /** Default constructor **/
    R3BSofTofWTcal2HitPar();

    /** Standard constructor **/
    R3BSofTofWTcal2HitPar(const TString& name,
                            Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofTofWTcal2HitPar();

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

    /** Method to set up the limits for fit **/
    void SetFitLimits(Int_t left, Int_t right)
    {
        fLimit_left = left;
        fLimit_right = right;
    }

  private:
    Int_t fNumSci;
    Int_t fMinStadistics;
    Int_t fLimit_left;
    Int_t fLimit_right;
    Int_t fMaxSigma;
    TArrayF* TofParams;
    TArrayF* PosParams;

    R3BSofTofWHitPar* fHit_Par;   /**< Parameter container. >*/
    TClonesArray* fTofCalDataCA; /**< Array with Tof-Tcal data. >*/



  public:
    // Class definition
    ClassDef(R3BSofTofWTcal2HitPar, 1)
};

#endif
