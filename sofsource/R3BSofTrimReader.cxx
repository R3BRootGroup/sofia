#include "FairLogger.h"

#include "FairRootManager.h"
#include "R3BSofTrimMappedData.h"
#include "R3BSofTrimReader.h"

extern "C"
{
#include "ext_data_client.h"
#include "ext_h101_softrim.h"
}

#include <iostream>

using namespace std;

R3BSofTrimReader::R3BSofTrimReader(EXT_STR_h101_SOFTRIM* data, UInt_t offset)
    : R3BReader("R3BSofTrimReader")
    , fData(data)
    , fOffset(offset)
    , fOnline(kFALSE)
    , fLogger(FairLogger::GetLogger())
    , fArray(new TClonesArray("R3BSofTrimMappedData"))
    , fSections(3)
{
}

R3BSofTrimReader::R3BSofTrimReader(EXT_STR_h101_SOFTRIM* data, UInt_t offset, Int_t num)
    : R3BReader("R3BSofTrimReader")
    , fData(data)
    , fOffset(offset)
    , fOnline(kFALSE)
    , fLogger(FairLogger::GetLogger())
    , fArray(new TClonesArray("R3BSofTrimMappedData"))
    , fSections(num)
{
}

R3BSofTrimReader::~R3BSofTrimReader() {}

Bool_t R3BSofTrimReader::Init(ext_data_struct_info* a_struct_info)
{
    int ok;
    LOG(INFO) << "R3BSofTrimReader::Init";
    EXT_STR_h101_SOFTRIM_ITEMS_INFO(ok, *a_struct_info, fOffset, EXT_STR_h101_SOFTRIM, 0);
    if (!ok)
    {
        perror("ext_data_struct_info_item");
        LOG(ERROR) << "R3BSofTrimReader::Failed to setup structure information.";
        return kFALSE;
    }

    // Register output array in tree
    if (!fOnline)
    {
        FairRootManager::Instance()->Register("TrimMappedData", "SofTrim", fArray, kTRUE);
    }
    else
    {
        FairRootManager::Instance()->Register("TrimMappedData", "SofTrim", fArray, kFALSE);
    }

    // clear struct_writer's output struct. Seems ucesb doesn't do that
    // for channels that are unknown to the current ucesb config.
    EXT_STR_h101_SOFTRIM_onion* data = (EXT_STR_h101_SOFTRIM_onion*)fData;
    for (int s = 0; s < fSections; s++)
    {
        data->SOFTRIM_S[s].EM = 0;
        data->SOFTRIM_S[s].TM = 0;
        data->SOFTRIM_S[s].TREFM = 0;
        data->SOFTRIM_S[s].TTRIGM = 0;
    }

    return kTRUE;
}

Bool_t R3BSofTrimReader::Read()
{
    // Convert plain raw data to multi-dimensional array
    EXT_STR_h101_SOFTRIM_onion* data = (EXT_STR_h101_SOFTRIM_onion*)fData;

    // loop over all planes and sections
    for (UShort_t s = 0; s < fSections; s++)
    {
        ReadData(data, s);
    }
    return kTRUE;
}

void R3BSofTrimReader::Reset()
{
    // Reset the output array
    fArray->Clear();
}

Bool_t R3BSofTrimReader::ReadData(EXT_STR_h101_SOFTRIM_onion* data, UShort_t section)
{
    Bool_t pileupFLAG;
    Bool_t overflowFLAG;

    // --- ------------------------------------- --- //
    // --- NUMBER OF CHANNELS TREF, TRIG, ANODES --- //
    // --- ------------------------------------- --- //
    UShort_t nAnodesTref = data->SOFTRIM_S[section].TREFM;
    UShort_t nAnodesTtrig = data->SOFTRIM_S[section].TTRIGM;
    UShort_t nAnodesEnergy = data->SOFTRIM_S[section].EM;
    UShort_t nAnodesTime = data->SOFTRIM_S[section].TM;

    /*
        if ((nAnodesEnergy>0)||(nAnodesTref>0)) {
        std::cout << std::endl;
        std::cout << "------------------------------" << std::endl;
        std::cout << "R3BSofTrimReader::ReadData section = " << section <<std::endl;
        std::cout << "------------------------------" << std::endl;
        LOG(INFO) << "nAnodesTref = " << nAnodesTref;
        LOG(INFO) << "nAnodesTtrig = " << nAnodesTtrig;
        LOG(INFO) << "nAnodesEnergy = " << nAnodesEnergy;
        LOG(INFO) << "nAnodesTime = " << nAnodesTime;
        std::cout << "------------------------------" << std::endl;
        }
    */

    // --- ----------------- --- //
    // --- TRIM MAPPED DATA --- //
    // --- ----------------- --- //

    // --- TIME OF THE REFERENCE SIGNAL --- //
    uint32_t curTref = 0;
    uint32_t nextTref = 0;
    UShort_t idAnodeTref = 0;

    // TREF id Anode = 7
    // Attention section 1 and 2 are connected to the same MDPP16
    // Therefore, it is mandatory to duplicate the section 1 Tref
    for (UShort_t a = 0; a < nAnodesTref; a++)
    {
        // TREFMI gives the 1-based Tref number
        idAnodeTref = data->SOFTRIM_S[section].TREFMI[a] + 6; // idAnodeTref = 7
        nextTref = data->SOFTRIM_S[section].TREFME[a];
        for (int hit = curTref; hit < nextTref; hit++)
        {
            pileupFLAG = (data->SOFTRIM_S[section].TREFv[hit] & 0x00040000) >> 18;
            overflowFLAG = (data->SOFTRIM_S[section].TREFv[hit] & 0x00080000) >> 19;
            // section is 0-based
            new ((*fArray)[fArray->GetEntriesFast()]) R3BSofTrimMappedData(
                section + 1, idAnodeTref, data->SOFTRIM_S[section].TREFv[hit], 0, pileupFLAG, overflowFLAG);
            if (section == 0)
                new ((*fArray)[fArray->GetEntriesFast()]) R3BSofTrimMappedData(
                    section + 2, idAnodeTref, data->SOFTRIM_S[section].TREFv[hit], 0, pileupFLAG, overflowFLAG);
        }
        curTref = nextTref;
    }

    // --- TIME OF THE TRIGGER SIGNAL --- //
    uint32_t curTtrig = 0;
    uint32_t nextTtrig = 0;
    UShort_t idAnodeTtrig = 0;

    // TRIG id Anode = 8
    for (UShort_t a = 0; a < nAnodesTtrig; a++)
    {
        // again TTRIGMI is 1 based
        idAnodeTtrig = data->SOFTRIM_S[section].TTRIGMI[a] + 7; // idAnodeTtrig = 8
        nextTtrig = data->SOFTRIM_S[section].TTRIGME[a];
        for (int hit = curTtrig; hit < nextTtrig; hit++)
        {
            pileupFLAG = (data->SOFTRIM_S[section].TTRIGv[hit] & 0x00040000) >> 18;
            overflowFLAG = (data->SOFTRIM_S[section].TTRIGv[hit] & 0x00080000) >> 19;
            new ((*fArray)[fArray->GetEntriesFast()]) R3BSofTrimMappedData(
                section + 1, idAnodeTtrig, data->SOFTRIM_S[section].TTRIGv[hit], 0, pileupFLAG, overflowFLAG);
            if (section == 0)
                new ((*fArray)[fArray->GetEntriesFast()]) R3BSofTrimMappedData(
                    section + 2, idAnodeTtrig, data->SOFTRIM_S[section].TTRIGv[hit], 0, pileupFLAG, overflowFLAG);
        }
        curTtrig = nextTtrig;
    }

    // --- ENERGY and TIME OF THE ANODE SIGNALS --- //
    if (nAnodesEnergy != nAnodesTime)
        LOG(ERROR)
            << "R3BSofTrimReader::ReadData ERROR ! NOT THE SAME NUMBER OF ANODES HITTED IN ENERGY () AND TIME ()";

    // ENERGY AND TIME ARE SORTED
    uint32_t curAnodeTimeStart = 0;
    uint32_t curAnodeEnergyStart = 0;
    for (UShort_t a = 0; a < nAnodesTime; a++)
    {
        // EMI and TMI give the 1-based anode number
        UShort_t idAnodeTime = data->SOFTRIM_S[section].TMI[a];
        UShort_t idAnodeEnergy = data->SOFTRIM_S[section].EMI[a];
        if (idAnodeEnergy != idAnodeTime)
            LOG(ERROR) << "R3BSofTrimReader::ReadData ERROR ! MISMATCH FOR ANODE ID IN ENERGY #" << idAnodeEnergy
                       << " AND TIME #" << idAnodeTime;
        uint32_t nextAnodeTimeStart = data->SOFTRIM_S[section].TME[a];
        uint32_t nextAnodeEnergyStart = data->SOFTRIM_S[section].EME[a];
        if ((nextAnodeTimeStart - curAnodeTimeStart) != (nextAnodeEnergyStart - curAnodeEnergyStart))
            LOG(ERROR) << "R3BSofTrimReader::ReadData ERROR ! MISMATCH FOR MULTIPLICITY PER ANODE IN ENERGY AND TIME";
        for (int hit = curAnodeTimeStart; hit < nextAnodeTimeStart; hit++)
        {
            pileupFLAG = (data->SOFTRIM_S[section].Ev[hit] & 0x00040000) >> 18;
            overflowFLAG = (data->SOFTRIM_S[section].Ev[hit] & 0x00080000) >> 19;
            new ((*fArray)[fArray->GetEntriesFast()]) R3BSofTrimMappedData(section + 1,
                                                                           idAnodeEnergy,
                                                                           data->SOFTRIM_S[section].Tv[hit],
                                                                           data->SOFTRIM_S[section].Ev[hit],
                                                                           pileupFLAG,
                                                                           overflowFLAG);
        }
        curAnodeEnergyStart = nextAnodeEnergyStart;
        curAnodeTimeStart = nextAnodeTimeStart;
    } // end of loop over the anodes from 1 to 6

    return kTRUE;
}

ClassImp(R3BSofTrimReader)
