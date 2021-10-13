#ifndef R3BSofSciCalData_H
#define R3BSofSciCalData_H

#include "TArrayD.h"
#include "TObject.h"

class R3BSofSciCalData : public TObject
{
  public:
    // Default Constructor
    R3BSofSciCalData();
    R3BSofSciCalData(UShort_t d, Double_t x, Double_t b2, Double_t b8, Double_t t2, Double_t t8);

    // Destructor
    virtual ~R3BSofSciCalData();

    // Getters
    inline const UShort_t& GetDetector() const { return fDetector; }
    inline const Double_t& GetPosMm() const { return fPosMm; }
    inline const Double_t& GetBeta_S2() const { return fBeta_S2; }
    inline const Double_t& GetBeta_S8() const { return fBeta_S8; }
    inline const Double_t& GetTofNs_S2() const { return fTofNs_S2; }
    inline const Double_t& GetTofNs_S8() const { return fTofNs_S8; }

    // Modifiers
    void SetDetector(UShort_t det) { fDetector = det; }
    void SetPosMm(Double_t pos) { fPosMm = pos; }
    void SetBeta_S2(Double_t beta) { fBeta_S2 = beta; }
    void SetBeta_S8(Double_t beta) { fBeta_S8 = beta; }
    void SetTofNs_S2(Double_t tof) { fTofNs_S2 = tof; }
    void SetTofNs_S8(Double_t tof) { fTofNs_S8 = tof; }

  private:
    UShort_t fDetector;
    Double_t fPosMm;    // attention, x is increasing from right to left
    Double_t fBeta_S2;  // Beta from RawTof from S2
    Double_t fBeta_S8;  // Beta from RawTof from S8
    Double_t fTofNs_S2; // ToF from S2 to fDetector
    Double_t fTofNs_S8; // ToF from S8 to fDetector

    /*
       Reminder: fRawPosNs:
             TrawRIGHT = 5*CCright - FTright
             TrawLEFT  = 5*CCleft  - FTleft
             fRawPosNs = TrawRIGHT - TrawLEFT = 5*(CCright-CCleft) + (FTleft-FTright)

       fPosMm: x is increasing from right to left
               this should be taken into account :
                    - in the calibration parameters
                    - in Brho calculation (while correcting from position at Cave C and S2)
  */

  public:
    ClassDef(R3BSofSciCalData, 2)
};

#endif
