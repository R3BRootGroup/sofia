// ---------------------------------------------------------------------------
// -----                                                                 -----
// -----                      R3BSofTrackingData                         -----
// -----                  Created 09/02/2020 by J.L. Rodriguez           -----
// -----                                                                 -----
// ---------------------------------------------------------------------------

#ifndef R3BSofTrackingData_H
#define R3BSofTrackingData_H
#include "TObject.h"

class R3BSofTrackingData : public TObject
{

  public:
    // Default Constructor
    R3BSofTrackingData();

    /** Standard Constructor
     *@param fZ      Z of fragments
     *@param fAq     A/q of fragments
     *@param fBeta   Beta of fragments
     *@param fLength Path length of fragments
     *@param fBrho   Brho of fragments
     *@param fPaddle Paddle ID of TofW
     **/
    R3BSofTrackingData(Double_t z, Double_t aq, Double_t beta, Double_t length, Double_t brho, Int_t paddle = 0);

    // Destructor
    virtual ~R3BSofTrackingData() {}

    // Getters
    inline const Double_t GetZ() const { return fZ; }
    inline const Double_t GetAq() const { return fAq; }
    inline const Double_t GetBeta() const { return fBeta; }
    inline const Double_t GetBrho() const { return fBrho; }
    inline const Double_t GetLength() const { return fLength; }
    inline const Int_t GetPaddle() const { return fPaddle; }

  protected:
    Double_t fZ, fAq; // ID
    Double_t fBeta, fBrho, fLength;
    Int_t fPaddle;
  public:
    ClassDef(R3BSofTrackingData, 1)
};

#endif
