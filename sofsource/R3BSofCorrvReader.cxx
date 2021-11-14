#include "FairLogger.h"

#include "FairRootManager.h"
#include "R3BSofCorrvMappedData.h"
#include "R3BSofCorrvReader.h"

extern "C"
{
#include "ext_data_client.h"
#include "ext_h101_sofcorrv.h"
}

#include <iostream>

using namespace std;

R3BSofCorrvReader::R3BSofCorrvReader(EXT_STR_h101_SOFCORRV* data, UInt_t offset)
    : R3BReader("R3BSofCorrvReader")
    , fData(data)
    , fOffset(offset)
    , fOnline(kFALSE)
    , fArray(new TClonesArray("R3BSofCorrvMappedData")) // class name
{
}

R3BSofCorrvReader::~R3BSofCorrvReader()
{
    LOG(DEBUG) << "R3BSofCorrvReader: Delete instance";
    if (fArray)
    {
        delete fArray;
    }
}

Bool_t R3BSofCorrvReader::Init(ext_data_struct_info* a_struct_info)
{
    Int_t ok;
    LOG(INFO) << "R3BSofCorrvReader::Init";
    EXT_STR_h101_SOFCORRV_ITEMS_INFO(ok, *a_struct_info, fOffset, EXT_STR_h101_SOFCORRV, 0);
    if (!ok)
    {
        perror("ext_data_struct_info_item");
        LOG(ERROR) << "R3BSofCorrvReader::Failed to setup structure information.";
        return kFALSE;
    }

    // Register output array in tree
    FairRootManager::Instance()->Register("CorrvMappedData", "SofCorrv", fArray, !fOnline);
    fArray->Clear();

    // clear struct_writer's output struct. Seems ucesb doesn't do that
    // for channels that are unknown to the current ucesb config.
    EXT_STR_h101_SOFCORRV_onion* data = (EXT_STR_h101_SOFCORRV_onion*)fData;
    data->SOFCORRV_TRFM = 0; // fine time
    data->SOFCORRV_TRCM = 0; // clock count

    return kTRUE;
}

Bool_t R3BSofCorrvReader::Read()
{
    // Convert plain raw data to multi-dimensional array
    EXT_STR_h101_SOFCORRV_onion* data = (EXT_STR_h101_SOFCORRV_onion*)fData;

    // Stop if no correlation signals
    if (data->SOFCORRV_TRCM == 0 || data->SOFCORRV_TRFM == 0)
    {
        return kTRUE;
    }

    // There is only one correlation signal
    if (data->SOFCORRV_TRCM > 1 || data->SOFCORRV_TRFM > 1)
    {
        LOG(FATAL) << "R3BSofCorrvReader::Read(), SOFCORRV_TRCM=" << data->SOFCORRV_TRCM
                   << ", SOFCORRV_TRFM=" << data->SOFCORRV_TRFM;
        return kFALSE;
    }

    // Signal is 1-based
    if (data->SOFCORRV_TRFMI[0] != 1 || data->SOFCORRV_TRCMI[0] != 1)
    {
        LOG(FATAL) << "R3BSofCorrvReader::Read(), SOFCORRV_TRFMI=" << data->SOFCORRV_TRFMI[0]
                   << ", SOFCORRV_TRCMI=" << data->SOFCORRV_TRCMI[0];
        return kFALSE;
    }

    // There are as many fine times than clock counts data words:
    if (data->SOFCORRV_TRF != data->SOFCORRV_TRC)
    {
        LOG(FATAL) << "R3BSofCorrvReader::Read(), SOFCORRV_TRF=" << data->SOFCORRV_TRF
                   << ", SOFCORRV_TCF=" << data->SOFCORRV_TRC;
        return kFALSE;
    }

    for (int i = 0; i < data->SOFCORRV_TRCME[0]; i++)
    {
        new ((*fArray)[fArray->GetEntriesFast()]) R3BSofCorrvMappedData(data->SOFCORRV_TRCv[i], data->SOFCORRV_TRFv[i]);
    }

    return kTRUE;
}

void R3BSofCorrvReader::Reset()
{
    // Reset the output array
    fArray->Clear();
}

ClassImp(R3BSofCorrvReader);
