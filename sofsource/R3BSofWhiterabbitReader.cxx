

#include "R3BSofWhiterabbitReader.h"
#include "FairLogger.h"
#include "FairRootManager.h"
#include "R3BEventHeader.h"
#include "R3BWRMasterData.h"
#include "TClonesArray.h"

extern "C"
{
#include "ext_data_client.h"
#include "ext_h101_wrsofia.h"
}

R3BSofWhiterabbitReader::R3BSofWhiterabbitReader(EXT_STR_h101_WRSOFIA* data,
                                                       UInt_t offset,
                                                       UInt_t whiterabbit_id)
    : R3BReader("R3BSofWhiterabbitReader")
    , fNEvent(0)
    , fData(data)
    , fOffset(offset)
    , fOnline(kFALSE)
    , fLogger(FairLogger::GetLogger())
    , fWhiterabbitId(whiterabbit_id)
    , fEventHeader(nullptr)
    , fArray(new TClonesArray("R3BWRMasterData"))
{
}

R3BSofWhiterabbitReader::~R3BSofWhiterabbitReader()
{
    if (fArray)
    {
        delete fArray;
    }
}

Bool_t R3BSofWhiterabbitReader::Init(ext_data_struct_info* a_struct_info)
{
    Int_t ok;
    LOG(INFO) << "R3BSofWhiterabbitReader::Init";
    EXT_STR_h101_WRSOFIA_ITEMS_INFO(ok, *a_struct_info, fOffset, EXT_STR_h101_WRSOFIA, 0);

    if (!ok)
    {
        LOG(ERROR) << "R3BSofWhiterabbitReader::Failed to setup structure information.";
        return kFALSE;
    }

    FairRootManager* mgr = FairRootManager::Instance();
    fEventHeader = (R3BEventHeader*)mgr->GetObject("R3BEventHeader");

    // Register output array in tree
    if (!fOnline)
    {
        FairRootManager::Instance()->Register("SofWRData", "SofWR", fArray, kTRUE);
    }
    else
    {
        FairRootManager::Instance()->Register("SofWRData", "SofWR", fArray, kFALSE);
    }

    fData->TIMESTAMP_SOFIA_ID = 0;
    return kTRUE;
}

Bool_t R3BSofWhiterabbitReader::Read()
{
    if (!fData->TIMESTAMP_SOFIA_ID)
    {
        return kTRUE;
    }

    if (fWhiterabbitId != fData->TIMESTAMP_SOFIA_ID)
    {
        char strMessage[1000];
        snprintf(strMessage,
                 sizeof strMessage,
                 "Event %u: Whiterabbit ID mismatch: expected 0x%x, got 0x%x.\n",
                 fEventHeader->GetEventno(),
                 fWhiterabbitId,
                 fData->TIMESTAMP_SOFIA_ID);
        LOG(error) << strMessage;
    }


    if (fEventHeader != nullptr)
    {
        uint64_t timestamp = ((uint64_t)fData->TIMESTAMP_SOFIA_WR_T4 << 48) |
                              ((uint64_t)fData->TIMESTAMP_SOFIA_WR_T3 << 32) |
                              ((uint64_t)fData->TIMESTAMP_SOFIA_WR_T2 << 16) | (uint64_t)fData->TIMESTAMP_SOFIA_WR_T1;

        // fEventHeader->SetTimeStamp(timestamp);
        fNEvent = fEventHeader->GetEventno();
        new ((*fArray)[fArray->GetEntriesFast()]) R3BWRMasterData(timestamp);
    }
    else
    {
        fNEvent++;
    }

    fData->TIMESTAMP_SOFIA_ID = 0;
    return kTRUE;
}

void R3BSofWhiterabbitReader::Reset()
{
    // Reset the output array
    fArray->Clear();
    fNEvent = 0;
}

ClassImp(R3BSofWhiterabbitReader)
