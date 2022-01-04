// ----------------------------------------------------------------
// -----          R3BSofTofWDigitizer source file             -----
// -----         Created 03/11/19  by JL Rodriguez            -----
// ----------------------------------------------------------------

#ifndef R3BSofTofWDigitizer_H
#define R3BSofTofWDigitizer_H 1

#include "FairTask.h"
#include "R3BSofTofWHitData.h"
#include "TRandom3.h"
#include "TRotation.h"
#include "TVector3.h"
#include <map>
#include <string>

class TClonesArray;
class R3BTGeoPar;

class R3BSofTofWDigitizer : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofTofWDigitizer();

    /** Standard constructor **/
    R3BSofTofWDigitizer(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    ~R3BSofTofWDigitizer();

    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* opt);

    // Fair specific
    virtual void SetParContainers();

    virtual void Finish();

    virtual void Reset();

    /** Setters for sigmas **/
    void SetSigma_t(Float_t sigma_t) { fsigma_t = sigma_t; }
    void SetSigma_ELoss(Float_t sigma_ELoss) { fsigma_ELoss = sigma_ELoss; }

  private:
    void SetParameter();

    TClonesArray* fMCTrack;
    TClonesArray* fTofPoints;
    TClonesArray* fTofHits;
    R3BTGeoPar* fTofWGeoPar;
    TRandom3* rand;
    Float_t fsigma_y;
    Float_t fsigma_t;
    Float_t fsigma_ELoss;
    TVector3 fTrans;
    TRotation fRot;

    /** Private method AddHitData **/
    // Adds a R3BSofTofWHitData to the TofWHitCollection
    R3BSofTofWHitData* AddHitData(Int_t paddle, Double_t x, Double_t y, Double_t time);

  public:
    // Class definition
    ClassDef(R3BSofTofWDigitizer, 1);
};

#endif
