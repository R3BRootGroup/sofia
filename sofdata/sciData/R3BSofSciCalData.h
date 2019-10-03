#ifndef R3BSOFSCICALITEM_H
#define R3BSOFSCICALITEM_H

#include "TObject.h"
#include "TArrayD.h"

#define NUM_SOFSCI     2
#define NUM_SOFSCI_PMT 2

// for the data analysis of the SOFIA SINGLE SCINTILLATORS
// Introduced by Audrey, October 2018

class R3BSofSciCalData : public TObject
{
  public:
    // Default Constructor
    R3BSofSciCalData();

    // Destructor
    virtual ~R3BSofSciCalData();
    
    void SetRawTimeNs(UShort_t det, UShort_t pmt, Double_t tns)   {fRawTimeNs->AddAt(tns,(det-1)*NUM_SOFSCI_PMT+pmt-1);}
    void SetMeanTimeNs(UShort_t det, Double_t mean)               {fMeanTimeNs->AddAt(mean,det-1);}
    void SetRawPos(UShort_t det, Double_t pos)                    {fRawPos->AddAt(pos,det-1);}
    void SetRawToF(Double_t tof)                                  {fRawTof = tof;}

    Double_t GetRawTimeNs(UShort_t det, UShort_t pmt) {return (Double_t)fRawTimeNs->At((det-1)*NUM_SOFSCI_PMT+pmt-1);}
    Double_t GetMeanTimeNs(UShort_t det)              {return (Double_t)fMeanTimeNs->At(det-1);}
    Double_t GetRawPos(UShort_t det)                  {return (Double_t)fRawPos->At(det-1);}
    Double_t GetRawToF()                              {return fRawTof;}

  private:
    TArrayD* fRawTimeNs;
    TArrayD* fMeanTimeNs;
    TArrayD* fRawPos;
    Double_t fRawTof;


  public:
    ClassDef(R3BSofSciCalData, 2)
};

#endif
