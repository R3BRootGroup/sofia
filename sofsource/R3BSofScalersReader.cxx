#include "FairLogger.h"

#include "FairRootManager.h"
#include "R3BSofScalersMappedData.h"
#include "R3BSofScalersReader.h"

extern "C"
{
#include "ext_data_client.h"
#include "ext_h101_sofscalers.h"
}

#define NUM_CHANNELS_SOFSCALERS_UPSTREAM 7
#define NUM_CHANNELS_SOFSCALERS_TOFW 56

#include <iostream>

using namespace std;

R3BSofScalersReader::R3BSofScalersReader(EXT_STR_h101_SOFSCALERS* data, UInt_t offset)
    : R3BReader("R3BSofScalersReader")
    , fData(data)
    , fOffset(offset)
    , fOnline(kFALSE)
    , fArray(new TClonesArray("R3BSofScalersMappedData")) // class name
    , fNumEntries(0)
{
}

R3BSofScalersReader::~R3BSofScalersReader()
{
    LOG(INFO) << "R3BSofScalersReader: Delete instance";
    if (fArray)
    {
        delete fArray;
    }
}

Bool_t R3BSofScalersReader::Init(ext_data_struct_info* a_struct_info)
{
    Int_t ok;
    LOG(INFO) << "R3BSofScalersReader::Init";
    EXT_STR_h101_SOFSCALERS_ITEMS_INFO(ok, *a_struct_info, fOffset, EXT_STR_h101_SOFSCALERS, 0);
    if (!ok)
    {
        perror("ext_data_struct_info_item");
        LOG(ERROR) << "R3BSofScalersReader::Failed to setup structure information.";
        return kFALSE;
    }

    // Register output array in tree
    if (!fOnline)
    {
        FairRootManager::Instance()->Register("SofScalersMappedData", "SofScalers", fArray, kTRUE);
    }
    else
    {
        FairRootManager::Instance()->Register("SofScalersMappedData", "SofScalers", fArray, kFALSE);
    }
    fArray->Clear();

    // clear struct_writer's output struct. Seems ucesb doesn't do that
    // for channels that are unknown to the current ucesb config.
    EXT_STR_h101_SOFSCALERS_onion* data = (EXT_STR_h101_SOFSCALERS_onion*)fData;
    for (int ch = 0; ch < NUM_CHANNELS_SOFSCALERS_UPSTREAM; ch++)
    {
        data->SOFSCALERS_UPSTREAM[ch] = 0;
    }
    for (int ch = 0; ch < NUM_CHANNELS_SOFSCALERS_TOFW; ch++)
    {
        data->SOFSCALERS_TOFW[ch] = 0;
    }

    return kTRUE;
}

Bool_t R3BSofScalersReader::Read()
{
    // Convert plain raw data to multi-dimensional array
    EXT_STR_h101_SOFSCALERS_onion* data = (EXT_STR_h101_SOFSCALERS_onion*)fData;

    for (int ch = 0; ch < NUM_CHANNELS_SOFSCALERS_UPSTREAM; ch++)
    {
        auto item = new ((*fArray)[fNumEntries++]) R3BSofScalersMappedData(1, ch + 1, data->SOFSCALERS_UPSTREAM[ch]);
    }

    for (int ch = 0; ch < NUM_CHANNELS_SOFSCALERS_TOFW; ch++)
    {
        auto item = new ((*fArray)[fNumEntries++]) R3BSofScalersMappedData(2, ch + 1, data->SOFSCALERS_TOFW[ch]);
    }

    return kTRUE;
}

void R3BSofScalersReader::Reset()
{
    // Reset the output array
    fArray->Clear();
    fNumEntries = 0;
}

ClassImp(R3BSofScalersReader)
