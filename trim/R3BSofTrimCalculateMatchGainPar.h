#ifndef __R3BSOFTRIMMATCHGAINPAR_H__
#define __R3BSOFTRIMMATCHGAINPAR_H__

#include "FairTask.h"
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

    /** Accessor functions **/
    const Int_t GetNumSections() { return fNumSections; }
    const Int_t GetNumAnodes() { return fNumAnodes; }
    const Int_t GetNumPairsPerSection() { return fNumPairsPerSection; }
    const Int_t GetMinStatistics() { return fMinStatistics; }

    const Float_t GetEpsilon() { return fEpsilon; }
    const Float_t GetGainMin() { return fGainMin; }
    const Float_t GetGainMax() { return fGainMax; }

    void SetNumSections(Int_t n) { fNumSections = n; }
    void SetNumAnodes(Int_t n) { fNumAnodes = n; }
    void SetNumPairsPerSection() { fNumPairsPerSection = fNumAnodes / 2; }
    void SetMinStatistics(Int_t minstat) { fMinStatistics = minstat; }

    void SetEpsilon(Float_t epsilon) { fEpsilon = epsilon; }
    void SetGainMin(Float_t gain) { fGainMin = gain; }
    void SetGainMax(Float_t gain) { fGainMax = gain; }
    void SetNumHistosPerPair(Float_t gain_min, Float_t gain_max, Float_t epsilon)
    {
        fNumHistosPerPair = (Int_t)((fGainMax - fGainMin) / fEpsilon);
    }

  protected:
    Int_t fNumSections;
    Int_t fNumAnodes;
    Int_t fNumPairsPerSection;
    Int_t fNumHistosPerPair;
    Int_t fMinStatistics; // minimum statistics to proceed to the calibration

    // calibration parameters
    R3BSofTrimCalPar* fCalPar;

    // input data
    TClonesArray* fCalData;

    // histograms
    TH2D** fh2_TrimPerPair_Esub_vs_Y;
    Float_t fEpsilon;
    Float_t fGainMin;
    Float_t fGainMax;
    char* fOutputFile;

  public:
    ClassDef(R3BSofTrimCalculateMatchGainPar, 0);
};

#endif //__R3BSOFTRIMMATCHGAINPAR_H__
