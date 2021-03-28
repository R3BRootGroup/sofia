#include "FairLogger.h"

#include "FairRootManager.h"
#include "R3BVftxTrigMappedData.h"
#include "R3BVftxTrigReader.h"

extern "C"
{
#include "ext_data_client.h"
#include "ext_h101_vftx_tr.h"
}

#include <iostream>

using namespace std;

R3BVftxTrigReader::R3BVftxTrigReader(EXT_STR_h101_VFTX_TR* data, UInt_t offset)
    : R3BReader("R3BVftxTrigReader")
    , fData(data)
    , fOffset(offset)
    , fOnline(kFALSE)
    , fArray(new TClonesArray("R3BVftxTrigMappedData")) // class name
{
}

R3BVftxTrigReader::~R3BVftxTrigReader()
{
    LOG(INFO) << "R3BVftxTrigReader: Delete instance";
    if (fArray)
    {
        delete fArray;
    }
}

Bool_t R3BVftxTrigReader::Init(ext_data_struct_info* a_struct_info)
{
    Int_t ok;
    LOG(INFO) << "R3BVftxTrigReader::Init";
    EXT_STR_h101_VFTX_TR_ITEMS_INFO(ok, *a_struct_info, fOffset, EXT_STR_h101_VFTX_TR, 0);
    if (!ok)
    {
        perror("ext_data_struct_info_item");
        LOG(ERROR) << "R3BVftxTrigReader::Failed to setup structure information.";
        return kFALSE;
    }

    // Register output array in tree
    if (!fOnline)
    {
        FairRootManager::Instance()->Register("VftxTrigMappedData", "VftxTrig", fArray, kTRUE);
    }
    else
    {
        FairRootManager::Instance()->Register("VftxTrigMappedData", "VftxTrig", fArray, kFALSE);
    }
    fArray->Clear();

    // clear struct_writer's output struct. Seems ucesb doesn't do that
    // for channels that are unknown to the current ucesb config.
    EXT_STR_h101_VFTX_TR_onion* data = (EXT_STR_h101_VFTX_TR_onion*)fData;
    data->VFTX_TR = 0; // clock count of the trigger signal

    return kTRUE;
}

Bool_t R3BVftxTrigReader::Read()
{
    // Convert plain raw data to multi-dimensional array
    EXT_STR_h101_VFTX_TR_onion* data = (EXT_STR_h101_VFTX_TR_onion*)fData;

    // There is only one correlation signal
    for (int i = 0; i < data->VFTX_TR; i++)
    {
        new ((*fArray)[fArray->GetEntriesFast()])
            R3BVftxTrigMappedData((uint16_t)data->VFTX_TRI[i], (uint16_t)data->VFTX_TRv[i]);
    }

    return kTRUE;
}

void R3BVftxTrigReader::Reset()
{
    // Reset the output array
    fArray->Clear();
}

ClassImp(R3BVftxTrigReader)
