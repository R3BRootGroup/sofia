#ifndef R3BSofVftxTrigMappedData_H
#define R3BSofVftxTrigMappedData_H
#include "TObject.h"

class R3BSofVftxTrigMappedData : public TObject
{

  public:
    /** Default constructor **/
    R3BSofVftxTrigMappedData();

    R3BSofVftxTrigMappedData(UShort_t moduleID, UShort_t tt);

    /** Destructor **/
    virtual ~R3BSofVftxTrigMappedData() {}

    /** Accessors **/
    inline const UShort_t& GetModuleID() const { return fModuleID; }
    inline const UShort_t& GetTimeTrig() const { return fTimeTrig; }

    /** Modifiers **/
    void SetModuleID(UShort_t i) { fModuleID = i; }
    void SetTimeTrig(UShort_t t) { fTimeTrig = t; }

  protected:
    UShort_t fModuleID;
    UShort_t fTimeTrig;

    ClassDef(R3BSofVftxTrigMappedData, 1)
};

#endif
