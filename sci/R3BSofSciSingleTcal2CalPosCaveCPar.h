#ifndef __R3BSofSciSingleTcal2CalPosCaveCPar_H__
#define __R3BSofSciSingleTcal2CalPosCaveCPar_H__ 1

#include "FairTask.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2D.h"

#include "R3BSofSciCalPosPar.h"
#include "R3BTGeoPar.h"
#include "TClonesArray.h"

class R3BSofSciSingleTcal2CalPosCaveCPar : public FairTask
{
  public:
    /** Default constructor **/
    R3BSofSciSingleTcal2CalPosCaveCPar();

    /** Standard constructor **/
    R3BSofSciSingleTcal2CalPosCaveCPar(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofSciSingleTcal2CalPosCaveCPar();

    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* opt);

    /** Virtual method FinishTask **/
    virtual void FinishTask();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /** Virtual method calculate the PosRaw Single Tcal Parameters **/
    virtual void CalculateCalPosCaveCParams();

    void SetOutputFile(const char* outFile);

    /** Accessor functions **/
    const Int_t GetNumDets() { return fNumDets; }
    const Int_t GeNumParamsPerDet() { return fNumParamsPerDet; }
    const Int_t GetMinStatistics() { return fMinStatistics; }

    void SetNumDets(Int_t n) { fNumDets = n; }
    void SetNumParamsPerDet(Int_t n) { fNumParamsPerDet = n; }
    void SetMinStatistics(Int_t minstat) { fMinStatistics = minstat; }
    void SetFitMin(Float_t RawPosNsMin) { fFitMin = RawPosNsMin; }
    void SetFitMax(Float_t RawPosNsMax) { fFitMax = RawPosNsMax; }

  protected:
    Int_t fNumDets;         // number of detectors 2 if FRS, 1 if Cave C only
    Int_t fNumParamsPerDet; // 2 if pol1
    Int_t fMinStatistics;   // minimum statistics to proceed to the calibration
    Float_t fFitMin;
    Float_t fFitMax;

    // Geometry for Mwpc0
    R3BTGeoPar* fMwpc0GeoPar;

    // calibration parameters
    R3BSofSciCalPosPar* fCalPosPar;

    // input data
    TClonesArray* fSTcalSci;
    TClonesArray* fHitMw0;

    // histogram
    TH2D* fh2_X0_vs_RawPosCaveC;

    // Profile
    TH1D* fh1_pfx;

    // TF1
    TF1* fit_Xmm_vs_RawPosNs;

    char* fOutputFile;

    // fit functions

  public:
    ClassDef(R3BSofSciSingleTcal2CalPosCaveCPar, 0);
};

#endif //__R3BSOFSCITCAL2RAWPOSPAR_H__
