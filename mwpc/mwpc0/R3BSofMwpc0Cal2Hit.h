// ----------------------------------------------------------------------
// -----                                                            -----
// -----                     R3BSofMwpc0Cal2Hit                     -----
// -----             Created 09/10/19  by J.L. Rodriguez-Sanchez    -----
// ----------------------------------------------------------------------

#ifndef R3BSofMwpc0Cal2Hit_H
#define R3BSofMwpc0Cal2Hit_H

#include "FairTask.h"
#include "R3BSofMwpcCalData.h"
#include "R3BSofMwpcHitData.h"
#include "TH1F.h"
#include <TRandom.h>

#define Mw0PadsX 64
#define Mw0PadsY 64

class TClonesArray;

class R3BSofMwpc0Cal2Hit : public FairTask
{

  public:
    /** Default constructor **/
    R3BSofMwpc0Cal2Hit();

    /** Standard constructor **/
    R3BSofMwpc0Cal2Hit(const char* name, Int_t iVerbose = 1);

    /** Destructor **/
    virtual ~R3BSofMwpc0Cal2Hit();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* option);

    /** Virtual method Reset **/
    virtual void Reset();

    // Fair specific
    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /** Virtual method Finish **/
    virtual void Finish();

    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    Double_t fSize; // Detector size in X and Y
    Double_t fwx;   // Pad width in X
    Double_t fwy;   // Pad width in Y
    Int_t fx[Mw0PadsX], fy[Mw0PadsY];

    Bool_t fOnline; // Don't store data for online

    TClonesArray* fMwpcCalDataCA; /**< Array with Cal input data. >*/
    TClonesArray* fMwpcHitDataCA; /**< Array with Hit output data. >*/

    /** Private method AddHitData **/
    // Adds a SofMwpcHitData to the MwpcHitCollection
    R3BSofMwpcHitData* AddHitData(Double_t x, Double_t y);

    /** Private method to obtain the position X **/
    Double_t GetPostionX(Double_t qmax, Int_t padmax, Double_t qleft, Double_t qright);
    /** Private method to obtain the position Y **/
    Double_t GetPostionY(Double_t qmax, Int_t padmax, Double_t qdown, Double_t qup);

  public:
    // Class definition
    ClassDef(R3BSofMwpc0Cal2Hit, 1)
};

#endif
