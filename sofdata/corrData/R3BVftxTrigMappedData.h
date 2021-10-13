#ifndef R3BVftxTrigMappedData_H
#define R3BVftxTrigMappedData_H
#include "TObject.h"

class R3BVftxTrigMappedData : public TObject
{

  public:
    /** Default constructor **/
    R3BVftxTrigMappedData();

    R3BVftxTrigMappedData(UShort_t moduleID, UShort_t tt);

    /** Destructor **/
    virtual ~R3BVftxTrigMappedData() {}

    /** Accessors **/
    inline const UShort_t& GetModuleID() const { return fModuleID; }
    inline const UShort_t& GetTimeTrig() const { return fTimeTrig; }

    /** Modifiers **/
    void SetModuleID(UShort_t i) { fModuleID = i; }
    void SetTimeTrig(UShort_t t) { fTimeTrig = t; }

  protected:
    UShort_t fModuleID;
    UShort_t fTimeTrig;

    ClassDef(R3BVftxTrigMappedData, 1)
};

#endif
