// ----------------------------------------------------------------
// -----          R3BSofTofWDigitizer source file             -----
// -----         Created 03/11/19  by JL Rodriguez            -----
// ----------------------------------------------------------------

#ifndef R3BSofTofWDigitizer_H
#define R3BSofTofWDigitizer_H 1

#include "FairTask.h"
#include "TRandom1.h"
#include "R3BSofTofWHitData.h"
#include <map>
#include <string>

class TClonesArray;

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

    virtual void Finish();
    virtual void Reset();

    /** Setters for sigmas **/
    void SetSigma_y(Float_t sigma_y) { fsigma_y = sigma_y; }
    void SetSigma_t(Float_t sigma_t) { fsigma_t = sigma_t; }
    void SetPosX(Float_t x) { fPosX = x; }
    void SetPosZ(Float_t z) { fPosZ = z; }
    void SetAngle(Float_t a) { fangle = a; }
    void SetSigma_ELoss(Float_t sigma_ELoss) { fsigma_ELoss = sigma_ELoss; }

  private:
    TClonesArray* fMCTrack;
    TClonesArray* fTofPoints;
    TClonesArray* fTofHits;
    TRandom1* rand;
    Float_t fsigma_y;
    Float_t fsigma_t;
    Float_t fsigma_ELoss;
    Float_t fangle;
    Float_t fPosX, fPosZ;

    /** Private method AddHitData **/
    // Adds a R3BSofTofWHitData to the TofWHitCollection
    R3BSofTofWHitData* AddHitData(Int_t paddle, Double_t x, Double_t y, Double_t time, Double_t brho, Double_t vel);

  public:
    // Class definition
    ClassDef(R3BSofTofWDigitizer, 1);
};

#endif
