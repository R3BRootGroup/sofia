#include "FairLogger.h"
#include "FairRootManager.h"

#include "R3BSofAtMappedData.h"
#include "R3BSofAtReader.h"

extern "C"
{
#include "ext_data_client.h"
#include "ext_h101_sofat.h"
}

#include <iostream>

using namespace std;

R3BSofAtReader::R3BSofAtReader(EXT_STR_h101_SOFAT* data, size_t offset)
    : R3BReader("R3BSofAtReader")
    , fData(data)
    , fOffset(offset)
    , fOnline(kFALSE)
    , fArray(new TClonesArray("R3BSofAtMappedData"))
{
}

R3BSofAtReader::~R3BSofAtReader()
{

    LOG(DEBUG) << "R3BSofAtReader: Delete instance";
    if (fArray)
        delete fArray;
}

Bool_t R3BSofAtReader::Init(ext_data_struct_info* a_struct_info)
{
    Int_t ok;
    LOG(INFO) << "R3BSofAtReader::Init()";
    EXT_STR_h101_SOFAT_ITEMS_INFO(ok, *a_struct_info, fOffset, EXT_STR_h101_SOFAT, 0);
    if (!ok)
    {
        perror("ext_data_struct_info_item");
        LOG(ERROR) << "R3BSofAtReader::Failed to setup structure information.";
        return kFALSE;
    }

    // Register output array in tree
    FairRootManager::Instance()->Register("AtMappedData", "SofAt", fArray, !fOnline);
    fArray->Clear();

    // clear struct_writer's output struct. Seems ucesb doesn't do that
    // for channels that are unknown to the current ucesb config.
    EXT_STR_h101_SOFAT_onion* data = (EXT_STR_h101_SOFAT_onion*)fData;
    data->SOFAT_EM = 0;
    data->SOFAT_TM = 0;

    return kTRUE;
}

Bool_t R3BSofAtReader::Read()
{
    // Convert plain raw data to multi-dimensional array
    EXT_STR_h101_SOFAT_onion* data = (EXT_STR_h101_SOFAT_onion*)fData;

    ReadData(data);
    return kTRUE;
}

Bool_t R3BSofAtReader::ReadData(EXT_STR_h101_SOFAT_onion* data)
{
    Bool_t pileupFLAG;
    Bool_t overflowFLAG;

    // --- number of entries in energy and time
    UShort_t nAnodesEnergy = data->SOFAT_EM;
    UShort_t nAnodesTime = data->SOFAT_TM;
    if (nAnodesEnergy != nAnodesTime)
        LOG(ERROR) << "R3BSofAtReader::ReadData ERROR ! NOT THE SAME NUMBER OF ANODES HITTED IN ENERGY ("
                   << nAnodesEnergy << ") AND TIME (" << nAnodesTime << ")";

    // --- energy and time are sorted
    uint32_t curAnodeTimeStart = 0;
    uint32_t curAnodeEnergyStart = 0;
    for (UShort_t a = 0; a < nAnodesTime; a++)
    {
        // EMI and TMI give the 1-based anode number
        UShort_t idAnodeTime = data->SOFAT_TMI[a];
        UShort_t idAnodeEnergy = data->SOFAT_EMI[a];
        if (idAnodeEnergy != idAnodeTime)
        {
            LOG(ERROR) << "R3BSofAtReader::ReadData ERROR ! MISMATCH FOR ANODE ID ";
            LOG(ERROR) << "IN ENERGY #" << idAnodeEnergy << " AND TIME #" << idAnodeTime;
        }
        uint32_t nextAnodeTimeStart = data->SOFAT_TME[a];
        uint32_t nextAnodeEnergyStart = data->SOFAT_EME[a];
        if ((nextAnodeTimeStart - curAnodeTimeStart) != (nextAnodeEnergyStart - curAnodeEnergyStart))
            LOG(ERROR) << "R3BSofAtReader::ReadData ERROR ! MISMATCH FOR MULTIPLICITY PER ANODE IN ENERGY AND TIME";
        for (int hit = curAnodeTimeStart; hit < nextAnodeTimeStart; hit++)
        {
            pileupFLAG = (data->SOFAT_Ev[hit] & 0x00040000) >> 18;
            overflowFLAG = (data->SOFAT_Ev[hit] & 0x00080000) >> 19;
            new ((*fArray)[fArray->GetEntriesFast()])
                R3BSofAtMappedData(idAnodeEnergy, data->SOFAT_Ev[hit], data->SOFAT_Tv[hit], pileupFLAG, overflowFLAG);
        }
        curAnodeEnergyStart = nextAnodeEnergyStart;
        curAnodeTimeStart = nextAnodeTimeStart;
    } // end of loop over the anodes from 1 to 6

    return kTRUE;
}

void R3BSofAtReader::Reset()
{
    // Reset the output array
    fArray->Clear();
}

ClassImp(R3BSofAtReader);
