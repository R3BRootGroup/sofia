#ifndef R3BSOFTOFWSINGLETCALITEM_H
#define R3BSOFTOFWSINGLETCALITEM_H

#include "TObject.h"

class R3BSofTofWSingleTcalData : public TObject
{
  public:
    // Default Constructor
    R3BSofTofWSingleTcalData();

    // Standard Constructor
    R3BSofTofWSingleTcalData(Int_t plastic, Double_t time, Double_t tof, Double_t pos);

    // Destructor
    virtual ~R3BSofTofWSingleTcalData() {}

    // Getters
    inline const Int_t& GetDetector() const { return fDetector; }
    inline const Double_t& GetRawTofNs() const { return fRawTofNs; }
    inline const Double_t& GetRawTimeNs() const { return fRawTimeNs; }
    inline const Double_t& GetRawPosNs() const { return fRawPosNs; }

    // Modifiers
    void SetDetector(Int_t det) { fDetector = det; }
    void SetRawTimeNs(Double_t time) { fRawTimeNs = time; }
    void SetRawPosNs(Double_t pos) { fRawPosNs = pos; }
    void SetRawTofNs(Double_t tof) { fRawTofNs = tof; }

  private:
    Int_t fDetector; // 1..28
    Double_t fRawTofNs;
    Double_t fRawPosNs; // RawTimeDOWN - RawTimeUP
    Double_t fRawTimeNs;

  public:
    ClassDef(R3BSofTofWSingleTcalData, 2)
};

#endif
