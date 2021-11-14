
#include "FairLogger.h"
#include "FairRootManager.h"

#include "R3BEventHeader.h"
#include "R3BSofWhiterabbitReader.h"
#include "R3BWRData.h"
#include "TClonesArray.h"

extern "C"
{
#include "ext_data_client.h"
#include "ext_h101_wrsofia.h"
}

R3BSofWhiterabbitReader::R3BSofWhiterabbitReader(EXT_STR_h101_WRSOFIA* data,
                                                 size_t offset,
                                                 UInt_t whiterabbit_id1,
                                                 UInt_t whiterabbit_id2)
    : R3BReader("R3BSofWhiterabbitReader")
    , fNEvent(0)
    , fData(data)
    , fOffset(offset)
    , fOnline(kFALSE)
    , fWhiterabbitId1(whiterabbit_id1)
    , fWhiterabbitId2(whiterabbit_id2)
    , fEventHeader(nullptr)
    , fArray(new TClonesArray("R3BWRData"))
{
}

R3BSofWhiterabbitReader::~R3BSofWhiterabbitReader()
{
    LOG(DEBUG) << "R3BSofWhiterabbitReader: Delete instance";
    if (fArray)
        delete fArray;
    if (fEventHeader)
        delete fEventHeader;
}

Bool_t R3BSofWhiterabbitReader::Init(ext_data_struct_info* a_struct_info)
{
    Int_t ok;
    LOG(INFO) << "R3BSofWhiterabbitReader::Init()";
    EXT_STR_h101_WRSOFIA_ITEMS_INFO(ok, *a_struct_info, fOffset, EXT_STR_h101_WRSOFIA, 0);

    if (!ok)
    {
        LOG(ERROR) << "R3BSofWhiterabbitReader::Failed to setup structure information.";
        return kFALSE;
    }

    // Look for the R3BEventHeader
    FairRootManager* frm = FairRootManager::Instance();
    fEventHeader = (R3BEventHeader*)frm->GetObject("EventHeader.");
    if (!fEventHeader)
    {
        LOG(WARNING) << "R3BSofWhiterabbitReader::Init() EventHeader. not found";
    }
    else
        LOG(INFO) << "R3BSofWhiterabbitReader::Init() R3BEventHeader found";

    // Register output array in tree
    FairRootManager::Instance()->Register("SofWRData", "SofWR", fArray, !fOnline);
    fArray->Clear();

    fData->TIMESTAMP_SOFIA1ID = 0;
    fData->TIMESTAMP_SOFIA2ID = 0;
    return kTRUE;
}

Bool_t R3BSofWhiterabbitReader::Read()
{
    if (!fData->TIMESTAMP_SOFIA1ID)
    {
        return kTRUE;
    }
    else if (fData->TIMESTAMP_SOFIA1ID && !fData->TIMESTAMP_SOFIA2ID)
    {

        if (fWhiterabbitId1 != fData->TIMESTAMP_SOFIA1ID)
        {
            char strMessage[1000];
            snprintf(strMessage,
                     sizeof strMessage,
                     "Event %u: Whiterabbit ID mismatch: expected 0x%x, got 0x%x.\n",
                     fEventHeader->GetEventno(),
                     fWhiterabbitId1,
                     fData->TIMESTAMP_SOFIA2ID);
            LOG(error) << strMessage;
        }

        if (fEventHeader != nullptr)
        {
            uint64_t timestamp =
                ((uint64_t)fData->TIMESTAMP_SOFIA1WR_T4 << 48) | ((uint64_t)fData->TIMESTAMP_SOFIA1WR_T3 << 32) |
                ((uint64_t)fData->TIMESTAMP_SOFIA1WR_T2 << 16) | (uint64_t)fData->TIMESTAMP_SOFIA1WR_T1;

            // fEventHeader->SetTimeStamp(timestamp);
            fNEvent = fEventHeader->GetEventno();
            new ((*fArray)[fArray->GetEntriesFast()]) R3BWRData(timestamp);
        }
        else
        {
            fNEvent++;
        }

        fData->TIMESTAMP_SOFIA1ID = 0;
        return kTRUE;
    }
    else if (fData->TIMESTAMP_SOFIA1ID && fData->TIMESTAMP_SOFIA2ID)
    {

        if (fWhiterabbitId1 != fData->TIMESTAMP_SOFIA1ID)
        {
            char strMessage[1000];
            snprintf(strMessage,
                     sizeof strMessage,
                     "Event %u: Whiterabbit ID mismatch: expected 0x%x, got 0x%x.\n",
                     fEventHeader->GetEventno(),
                     fWhiterabbitId1,
                     fData->TIMESTAMP_SOFIA1ID);
            LOG(error) << strMessage;
        }

        if (fWhiterabbitId2 != fData->TIMESTAMP_SOFIA2ID)
        {
            char strMessage[1000];
            snprintf(strMessage,
                     sizeof strMessage,
                     "Event %u: Whiterabbit ID mismatch: expected 0x%x, got 0x%x.\n",
                     fEventHeader->GetEventno(),
                     fWhiterabbitId2,
                     fData->TIMESTAMP_SOFIA2ID);
            LOG(error) << strMessage;
        }

        if (fEventHeader != nullptr)
        {
            uint64_t timestamp1 =
                ((uint64_t)fData->TIMESTAMP_SOFIA1WR_T4 << 48) | ((uint64_t)fData->TIMESTAMP_SOFIA1WR_T3 << 32) |
                ((uint64_t)fData->TIMESTAMP_SOFIA1WR_T2 << 16) | (uint64_t)fData->TIMESTAMP_SOFIA1WR_T1;

            // fEventHeader->SetTimeStamp(timestamp);
            fNEvent = fEventHeader->GetEventno();
            new ((*fArray)[fArray->GetEntriesFast()]) R3BWRData(timestamp1);

            uint64_t timestamp2 =
                ((uint64_t)fData->TIMESTAMP_SOFIA2WR_T4 << 48) | ((uint64_t)fData->TIMESTAMP_SOFIA2WR_T3 << 32) |
                ((uint64_t)fData->TIMESTAMP_SOFIA2WR_T2 << 16) | (uint64_t)fData->TIMESTAMP_SOFIA2WR_T1;
            new ((*fArray)[fArray->GetEntriesFast()]) R3BWRData(timestamp2);
        }
        else
        {
            fNEvent++;
        }

        fData->TIMESTAMP_SOFIA1ID = 0;
        fData->TIMESTAMP_SOFIA2ID = 0;
        return kTRUE;
    }

    return kTRUE;
}

void R3BSofWhiterabbitReader::Reset()
{
    // Reset the output array
    fArray->Clear();
    fNEvent = 0;
}

ClassImp(R3BSofWhiterabbitReader);
