#ifndef R3BSofCorrmMappedData_H
#define R3BSofCorrmMappedData_H
#include "TObject.h"

class R3BSofCorrmMappedData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofCorrmMappedData();

    R3BSofCorrmMappedData(UShort_t tc, UShort_t tt);

    /** Destructor **/
    virtual ~R3BSofCorrmMappedData() {}

    /** Accessors **/
    inline const UShort_t& GetTimeCorr() const { return fTimeCorr; }
    inline const UShort_t& GetTimeTrig() const { return fTimeTrig; }

    /** Modifiers **/
    void SetTimeCorr(UShort_t t) { fTimeCorr = t; }
    void SetTimeTrig(UShort_t t) { fTimeTrig = t; }

  protected:
    UShort_t fTimeCorr;
    UShort_t fTimeTrig;

    ClassDef(R3BSofCorrmMappedData, 1)
};

#endif
