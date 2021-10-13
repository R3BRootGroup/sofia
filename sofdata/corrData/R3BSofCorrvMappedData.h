#ifndef R3BSofCorrvMappedData_H
#define R3BSofCorrvMappedData_H
#include "TObject.h"

class R3BSofCorrvMappedData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofCorrvMappedData();

    R3BSofCorrvMappedData(UShort_t ctc, UShort_t ftc);

    /** Destructor **/
    virtual ~R3BSofCorrvMappedData() {}

    /** Accessors **/
    inline const UShort_t& GetCoarseTimeCorr() const { return fCoarseTimeCorr; }
    inline const UShort_t& GetFineTimeCorr() const { return fFineTimeCorr; }

    /** Modifiers **/
    void SetCoarseTimeCorr(UShort_t t) { fCoarseTimeCorr = t; }
    void SetFineTimeCorr(UShort_t t) { fFineTimeCorr = t; }

  protected:
    UShort_t fCoarseTimeCorr;
    UShort_t fFineTimeCorr;

    ClassDef(R3BSofCorrvMappedData, 1)
};

#endif
