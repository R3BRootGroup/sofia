#ifndef R3BSOFSCISINGLETCALITEM_H
#define R3BSOFSCISINGLETCALITEM_H

#include "FairLogger.h"
#include "TObject.h"

class R3BSofSciSingleTcalData : public TObject
{
  public:
    // Default Constructor
    R3BSofSciSingleTcalData();
    R3BSofSciSingleTcalData(UShort_t d, Double_t t, Double_t p, Double_t tS2, Double_t tS8);

    // Destructor
    virtual ~R3BSofSciSingleTcalData() {}

    // Getters
    inline const UShort_t& GetDetector() const { return fDetector; }
    inline const Double_t& GetRawTimeNs() const { return fRawTimeNs; }
    inline const Double_t& GetRawPosNs() const { return fRawPosNs; }
    inline const Double_t& GetRawTofNs_FromS2() const { return fS2RawTofNs; }
    inline const Double_t& GetRawTofNs_FromS8() const { return fS8RawTofNs; }

    // Modifiers
    void SetDetector(UShort_t det) { fDetector = det; }
    void SetRawTimeNs(Double_t time) { fRawTimeNs = time; }
    void SetRawPosNs(Double_t pos) { fRawPosNs = pos; }
    void SetS2RawTofNs(Double_t tof) { fS2RawTofNs = tof; }
    void SetS8RawTofNs(Double_t tof) { fS8RawTofNs = tof; }

  private:
    UShort_t fDetector;
    Double_t fRawTimeNs; // 0.5*(TrawRIGHT + TrawLEFT)
    Double_t fRawPosNs;  //      TrawRIGHT - TrawLEFT
    Double_t fS2RawTofNs;
    Double_t fS8RawTofNs;
    // Remark for fRawPosNs:
    //    * TrawRIGHT = 5*CCright - FTright
    //    * TrawLEFT  = 5*CCleft  - FTleft
    //    * fRawPosNs = TrawRIGHT - TrawLEFT = 5*(CCright-CCleft) + (FTleft-FTright)
    //                  x is increasing from right to left
  public:
    ClassDef(R3BSofSciSingleTcalData, 2)
};

#endif
