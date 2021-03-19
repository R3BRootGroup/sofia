#ifndef __R3BSOFTRIMMATCHGAINPAR_H__
#define __R3BSOFTRIMMATCHGAINPAR_H__

#include "FairTask.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"

class TClonesArray;
class R3BSofTrimCalPar;
class R3BEventHeader;

class R3BSofTrimCalculateMatchGainPar : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofTrimCalculateMatchGainPar();

    /** Standard constructor **/
    R3BSofTrimCalculateMatchGainPar(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofTrimCalculateMatchGainPar();

    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* opt);

    /** Virtual method FinishEvent **/
    virtual void FinishEvent();

    /** Virtual method FinishTask **/
    virtual void FinishTask();

    /** Virtual method Reset **/
    virtual void Reset();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /** Virtual method calculate the gain matching of both anodes  **/
    virtual void PlotEvsY();

    void SetOutputFile(const char* outFile);

    void SetEpsilon(Float_t e) { fEpsilon = e; }
    void SetGainMin(Float_t gain, Int_t rank) { fGainMin->AddAt(gain, rank); }
    Float_t GetGainMin(Int_t rank) { return fGainMin->GetAt(rank); }

  protected:
    Int_t fNumSections;
    Int_t fNumAnodes;
    Int_t fNumPairsPerSection;
    Float_t fEpsilon;
    TArrayF* fGainMin;

    // calibration parameters
    R3BSofTrimCalPar* fCalPar;

    // input data
    TClonesArray* fCalData;

    // histograms
    TH2D** fh2_TrimPerPair_Esub_vs_Y;
    char* fOutputFile;

  public:
    ClassDef(R3BSofTrimCalculateMatchGainPar, 0);
};

#endif //__R3BSOFTRIMMATCHGAINPAR_H__
