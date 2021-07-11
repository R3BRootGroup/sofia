#include "FairLogger.h"

#include "FairRootManager.h"
#include "R3BSofCorrmMappedData.h"
#include "R3BSofCorrmReader.h"

extern "C"
{
#include "ext_data_client.h"
#include "ext_h101_sofcorrm.h"
}

#include <iostream>

using namespace std;

R3BSofCorrmReader::R3BSofCorrmReader(EXT_STR_h101_SOFCORRM* data, UInt_t offset)
    : R3BReader("R3BSofCorrmReader")
    , fData(data)
    , fOffset(offset)
    , fOnline(kFALSE)
    , fLogger(FairLogger::GetLogger())
    , fArray(new TClonesArray("R3BSofCorrmMappedData"))
{
}

R3BSofCorrmReader::~R3BSofCorrmReader() {}

Bool_t R3BSofCorrmReader::Init(ext_data_struct_info* a_struct_info)
{
    int ok;
    LOG(INFO) << "R3BSofCorrmReader::Init";
    EXT_STR_h101_SOFCORRM_ITEMS_INFO(ok, *a_struct_info, fOffset, EXT_STR_h101_SOFCORRM, 0);
    if (!ok)
    {
        perror("ext_data_struct_info_item");
        LOG(ERROR) << "R3BSofCorrmReader::Failed to setup structure information.";
        return kFALSE;
    }

    // Register output array in tree
    if (!fOnline)
    {
        FairRootManager::Instance()->Register("CorrmMappedData", "SofCorrm", fArray, kTRUE);
    }
    else
    {
        FairRootManager::Instance()->Register("CorrmMappedData", "SofCorrm", fArray, kFALSE);
    }

    // clear struct_writer's output struct. Seems ucesb doesn't do that
    // for channels that are unknown to the current ucesb config.
    EXT_STR_h101_SOFCORRM_onion* data = (EXT_STR_h101_SOFCORRM_onion*)fData;
    data->SOFCORRM_TRM = 0;
    data->SOFCORRM_TTM = 0;

    return kTRUE;
}

Bool_t R3BSofCorrmReader::Read()
{
    // Convert plain raw data to multi-dimensional array
    EXT_STR_h101_SOFCORRM_onion* data = (EXT_STR_h101_SOFCORRM_onion*)fData;

    // only one TR=correlation signal and TT=trig signal
    if (data->SOFCORRM_TRM == 0 || data->SOFCORRM_TTM == 0)
    {
        return kTRUE;
    }

    // only one TR=correlation signal and TT=trig signal
    if (data->SOFCORRM_TRM > 1 || data->SOFCORRM_TTM > 1)
    {
        LOG(FATAL) << "R3BSofCorrmReader::Read(), SOFCORRM_TRM=" << data->SOFCORRM_TRM
                   << ", SOFCORRM_TTM=" << data->SOFCORRM_TTM;
        return kFALSE;
    }

    // signal ids are 1-based
    if (data->SOFCORRM_TRMI[0] != 1 || data->SOFCORRM_TTMI[0] != 1)
    {
        LOG(INFO) << "R3BSofCorrmReader::Read(), SOFCORRM_TRM=" << data->SOFCORRM_TRM
                  << ", SOFCORRM_TTM=" << data->SOFCORRM_TTM;
        LOG(FATAL) << "R3BSofCorrmReader::Read(), SOFCORRM_TRMI=" << data->SOFCORRM_TRMI[0]
                   << ", SOFCORRM_TTMI=" << data->SOFCORRM_TTMI[0];
        return kFALSE;
    }

    // MDPP16 is multi-hit capability, but we should only have one trigger hit per event
    if (data->SOFCORRM_TT != 1)
    {
        LOG(FATAL) << "R3BSofCorrmReader::Read(), SOFCORRN_TT=" << data->SOFCORRM_TT;
        return kFALSE;
    }

    // read the correlation signal of the sofia_mesy DAQ
    // if the searching window is too large, might have several signals
    for (int i = 0; i < data->SOFCORRM_TRME[0]; i++)
    {
        new ((*fArray)[fArray->GetEntriesFast()]) R3BSofCorrmMappedData(data->SOFCORRM_TRv[i], data->SOFCORRM_TTv[0]);
    }

    return kTRUE;
}

void R3BSofCorrmReader::Reset()
{
    // Reset the output array
    fArray->Clear();
}

ClassImp(R3BSofCorrmReader)
