#include "FairLogger.h"

#include "FairRootManager.h"
#include "R3BSofTwimMappedData.h"
#include "R3BSofTwimReader.h"

extern "C"
{
#include "ext_data_client.h"
#include "ext_h101_softwim.h"
}

#include <iostream>

using namespace std;

R3BSofTwimReader::R3BSofTwimReader(EXT_STR_h101_SOFTWIM* data, UInt_t offset)
    : R3BReader("R3BSofTwimReader")
    , fData(data)
    , fOffset(offset)
    , fOnline(kFALSE)
    , fArray(new TClonesArray("R3BSofTwimMappedData"))
{
}

R3BSofTwimReader::~R3BSofTwimReader()
{
    LOG(INFO) << "R3BSofTwimReader: Delete instance";
    if (fArray)
    {
        delete fArray;
    }
}

Bool_t R3BSofTwimReader::Init(ext_data_struct_info* a_struct_info)
{
    Int_t ok;
    LOG(INFO) << "R3BSofTwimReader::Init";
    EXT_STR_h101_SOFTWIM_ITEMS_INFO(ok, *a_struct_info, fOffset, EXT_STR_h101_SOFTWIM, 0);
    if (!ok)
    {
        perror("ext_data_struct_info_item");
        LOG(ERROR) << "R3BSofTwimReader::Failed to setup structure information.";
        return kFALSE;
    }

    // Register output array in tree
    if (!fOnline)
    {
        FairRootManager::Instance()->Register("TwimMappedData", "SofTwim", fArray, kTRUE);
    }
    else
    {
        FairRootManager::Instance()->Register("TwimMappedData", "SofTwim", fArray, kFALSE);
    }

    // clear struct_writer's output struct. Seems ucesb doesn't do that
    // for channels that are unknown to the current ucesb config.
    EXT_STR_h101_SOFTWIM_onion* data = (EXT_STR_h101_SOFTWIM_onion*)fData;
    for (int s = 0; s < NUM_SOFTWIM_SECTIONS; s++)
    {
        data->SOFTWIM_S[s].EM = 0;
        data->SOFTWIM_S[s].TM = 0;
        data->SOFTWIM_S[s].TREFM = 0;
        data->SOFTWIM_S[s].TRIGM = 0;
    }
    for (int mult = 0; mult < NUM_SOFTWIM_ANODES; mult++)
        multPerAnode[mult] = 0;

    return kTRUE;
}

Bool_t R3BSofTwimReader::Read()
{
    // Convert plain raw data to multi-dimensional array
    EXT_STR_h101_SOFTWIM_onion* data = (EXT_STR_h101_SOFTWIM_onion*)fData;

    // loop over all planes and sections
    for (UShort_t s = 0; s < NUM_SOFTWIM_SECTIONS; s++)
    {
        ReadData(data, s);
    }
    return kTRUE;
}

void R3BSofTwimReader::Reset()
{
    // Reset the output array
    fArray->Clear();
}

Bool_t R3BSofTwimReader::ReadData(EXT_STR_h101_SOFTWIM_onion* data, UShort_t section)
{

    Bool_t pileupFLAG;
    Bool_t overflowFLAG;

    // --- ------------------------------------- --- //
    // --- NUMBER OF CHANNELS TREF, TRIG, ANODES --- //
    // --- ------------------------------------- --- //
    UShort_t nAnodesTref = data->SOFTWIM_S[section].TREFM;
    UShort_t nAnodesTrig = data->SOFTWIM_S[section].TRIGM;
    UShort_t nAnodesEnergy = data->SOFTWIM_S[section].EM;
    UShort_t nAnodesTime = data->SOFTWIM_S[section].TM;
    /* if (nAnodesTref>0) {
      std::cout << "------------------------------" << std::endl;
      std::cout << "------------------------------" << std::endl;
      std::cout <<" * nAnodesTref = "<< nAnodesTref <<std::endl;
      std::cout <<"  * nAnodesTrig = "<< nAnodesTrig <<std::endl;
      std::cout << "   * nAnodesEnergy = " << nAnodesEnergy << std::endl;
      std::cout << "   * nAnodesTime = " << nAnodesTime << std::endl;
      std::cout << "------------------------------" << std::endl;
    }
    */

    // --- ----------------- --- //
    // --- TWIM MAPPED DATA --- //
    // --- ----------------- --- //

    // --- TREF --- //
    uint32_t curTref = 0;
    uint32_t nextTref = 0;
    UShort_t idAnodeTref = 0;

    // TREF id Anode = 16 (ch0-7) and 17 (ch8-15)
    for (UShort_t a = 0; a < nAnodesTref; a++)
    {
        // TREFMI gives the 1-based Tref number
        // Tref1 --> should be assigned to index 16
        // Tref2 --> should be assigned to index 17

        idAnodeTref = data->SOFTWIM_S[section].TREFMI[a] + 15;
        nextTref = data->SOFTWIM_S[section].TREFME[a];
        multPerAnode[idAnodeTref] = nextTref - curTref;
        // std::cout << " * idAnodeTref = " << idAnodeTref << std::endl;
        // std::cout << "   multPerAnode[" << idAnodeTref << "] = " << multPerAnode[idAnodeTref] << std::endl;
        for (int hit = curTref; hit < nextTref; hit++)
        {
            pileupFLAG = (data->SOFTWIM_S[section].TREFv[hit] & 0x00040000) >> 18;
            overflowFLAG = (data->SOFTWIM_S[section].TREFv[hit] & 0x00080000) >> 19;
            new ((*fArray)[fArray->GetEntriesFast()]) R3BSofTwimMappedData(
                section, idAnodeTref, data->SOFTWIM_S[section].TREFv[hit], 0, pileupFLAG, overflowFLAG);
            // std::cout << "valTimeTref = " << data->SOFTWIM_S[section].TREFv[hit] << std::endl;
        }
        curTref = nextTref;
    }

    // --- TRIG --- //
    uint32_t curTrig = 0;
    uint32_t nextTrig = 0;
    UShort_t idAnodeTrig = 0;

    // TRIG id Anode = 18 (ch0-7) and 19 (ch8-15)
    for (UShort_t a = 0; a < nAnodesTrig; a++)
    {
        // again TRIGMI is 1 based
        // Trig1 --> should be assigned to index 18
        // Trig2 --> should be assigned to index 19
        idAnodeTrig = data->SOFTWIM_S[section].TRIGMI[a] + 17;
        nextTrig = data->SOFTWIM_S[section].TRIGME[a];
        multPerAnode[idAnodeTrig] = nextTrig - curTrig;
        // std::cout << "  * idAnodeTrig = " << idAnodeTrig << std::endl;
        // std::cout << "    multPerAnode[" << idAnodeTrig << "] = " << multPerAnode[idAnodeTrig] << std::endl;
        for (int hit = curTrig; hit < nextTrig; hit++)
        {
            pileupFLAG = (data->SOFTWIM_S[section].TRIGv[hit] & 0x00040000) >> 18;
            overflowFLAG = (data->SOFTWIM_S[section].TRIGv[hit] & 0x00080000) >> 19;
            new ((*fArray)[fArray->GetEntriesFast()]) R3BSofTwimMappedData(
                section, idAnodeTrig, data->SOFTWIM_S[section].TRIGv[hit], 0, pileupFLAG, overflowFLAG);
            // std::cout << "valTimeTrig = " << data->SOFTWIM_S[section].TRIGv[hit] << std::endl;
        }
        curTrig = nextTrig;
    }

    // --- ANODES --- //
    // return the number of anodes in the section with data
    // 0<=nAnodesEnergy<=16
    // 0<=nAnodesTime<=16

    // mail from R. Schneider from June 16, 2016: "if you have several hits of one channel in the window of interest,
    // you get an energy and a timing for each hit."
    // --> multiplicity per anode (for anode 1 to anode 16) should also be the same in energy and time
    // mail from R. Schneider from May 21st 2019 : "the hits from one channel are kept in the chronological order."
    // --> for one anode with multi-hit, the first hit in energy correspond to the first hit in time
    if (nAnodesEnergy != nAnodesTime)
        LOG(ERROR)
            << "R3BSofTwimReader::ReadData ERROR ! NOT THE SAME NUMBER OF ANODES HITTED IN ENERGY () AND TIME ()";

    // ENERGY AND TIME ARE SORTED
    uint32_t curAnodeTimeStart = 0;
    uint32_t curAnodeEnergyStart = 0;
    for (UShort_t a = 0; a < nAnodesTime; a++)
    {
        // EMI and TMI give the 1-based anode number
        UShort_t idAnodeTime = data->SOFTWIM_S[section].TMI[a] - 1;
        UShort_t idAnodeEnergy = data->SOFTWIM_S[section].EMI[a] - 1;

        if (idAnodeEnergy != idAnodeTime)
            LOG(ERROR) << "R3BSofTwimReader::ReadData ERROR ! MISMATCH FOR ANODE ID IN ENERGY #" << idAnodeEnergy
                       << " AND TIME #" << idAnodeTime;
        uint32_t nextAnodeTimeStart = data->SOFTWIM_S[section].TME[a];
        uint32_t nextAnodeEnergyStart = data->SOFTWIM_S[section].EME[a];
        multPerAnode[idAnodeTime] = nextAnodeTimeStart - curAnodeTimeStart;
        if (multPerAnode[idAnodeTime] != (nextAnodeEnergyStart - curAnodeEnergyStart))
            LOG(ERROR) << "R3BSofTwimReader::ReadData ERROR ! MISMATCH FOR MULTIPLICITY PER ANODE IN ENERGY AND TIME";
        for (int hit = curAnodeTimeStart; hit < nextAnodeTimeStart; hit++)
        {
            pileupFLAG = (data->SOFTWIM_S[section].Ev[hit] & 0x00040000) >> 18;
            overflowFLAG = (data->SOFTWIM_S[section].Ev[hit] & 0x00080000) >> 19;
            new ((*fArray)[fArray->GetEntriesFast()]) R3BSofTwimMappedData(section,
                                                                           idAnodeEnergy,
                                                                           data->SOFTWIM_S[section].Tv[hit],
                                                                           data->SOFTWIM_S[section].Ev[hit],
                                                                           pileupFLAG,
                                                                           overflowFLAG);
            // std::cout << "valTimeAnode = " << data->SOFTWIM_S[section].Tv[hit] << std::endl;
        }
        curAnodeEnergyStart = nextAnodeEnergyStart;
        curAnodeTimeStart = nextAnodeTimeStart;
    } // end of loop over the anodes from 1 to 16
    // std::cout << "------------------------------" << std::endl;

    return kTRUE;
}

ClassImp(R3BSofTwimReader)
