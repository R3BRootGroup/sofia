#ifndef R3BVFTXTRIGREADER_H
#define R3BVFTXTRIGREADER_H

#include "R3BReader.h"
#include "TClonesArray.h"

struct EXT_STR_h101_VFTX_TR_t;
typedef struct EXT_STR_h101_VFTX_TR_t EXT_STR_h101_VFTX_TR;
class FairLogger;

class R3BVftxTrigReader : public R3BReader
{
  public:
    R3BVftxTrigReader(EXT_STR_h101_VFTX_TR*, UInt_t);
    ~R3BVftxTrigReader();

    Bool_t Init(ext_data_struct_info*);
    Bool_t Read();
    void Reset();

    /** Accessor to select online mode **/
    void SetOnline(Bool_t option) { fOnline = option; }

  private:
    /* Reader specific data structure from ucesb */
    EXT_STR_h101_VFTX_TR* fData;
    /* Data offset */
    UInt_t fOffset;
    // Don't store data for online
    Bool_t fOnline;
    /* the structs of type R3BVftxTrigMapped Item */
    TClonesArray* fArray; /**< Output array. */

  public:
    ClassDef(R3BVftxTrigReader, 0);
};

#endif
