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
    , fSections(4)
    , fAnodes(16)
    , fTref(1)
    , fTtrig(1)
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
    for (int s = 0; s < fSections; s++)
    {
        data->SOFTWIM_S[s].EM = 0;
        data->SOFTWIM_S[s].TM = 0;
        data->SOFTWIM_S[s].TREFM = 0;
        data->SOFTWIM_S[s].TTRIGM = 0;
    }

    return kTRUE;
}

Bool_t R3BSofTwimReader::Read()
{
    // Convert plain raw data to multi-dimensional array
    EXT_STR_h101_SOFTWIM_onion* data = (EXT_STR_h101_SOFTWIM_onion*)fData;

    // loop over all planes and sections
    for (UShort_t s = 0; s < fSections; s++)
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
    UInt_t multPerAnode[fAnodes + fTref + fTtrig];
    for (int ch = 0; ch < fAnodes + fTref + fTtrig; ch++)
        multPerAnode[ch] = 0;

    // --- -------------------------------------- --- //
    // --- NUMBER OF CHANNELS TREF, TTRIG, ANODES --- //
    // --- -------------------------------------- --- //
    UShort_t nAnodesTref = data->SOFTWIM_S[section].TREFM;
    UShort_t nAnodesTtrig = data->SOFTWIM_S[section].TTRIGM;
    UShort_t nAnodesEnergy = data->SOFTWIM_S[section].EM;
    UShort_t nAnodesTime = data->SOFTWIM_S[section].TM;
    /*
    if ((nAnodesEnergy>0)||(nAnodesTref>0)) {
      std::cout << "------------------------------" << std::endl;
      std::cout << "R3BSofTwimReader::ReadDatai=()" << std::endl;
      std::cout << "------------------------------" << std::endl;
      std::cout <<" * nAnodesTref = "<< nAnodesTref <<std::endl;
      std::cout <<"  * nAnodesTtrig = "<< nAnodesTtrig <<std::endl;
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

    for (UShort_t a = 0; a < nAnodesTref; a++)
    {
        // TREFMI gives the 1-based Tref number
        idAnodeTref = data->SOFTWIM_S[section].TREFMI[a] + fAnodes - 1;
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

    // --- TTRIG --- //
    uint32_t curTtrig = 0;
    uint32_t nextTtrig = 0;
    UShort_t idAnodeTtrig = 0;

    // s467 : TTRIG id Anode = 18 (ch0-7) and 19 (ch8-15)
    // s455 : TTRIG id Anode = 17
    for (UShort_t a = 0; a < nAnodesTtrig; a++)
    {
        // again TTRIGMI is 1 based
        idAnodeTtrig = data->SOFTWIM_S[section].TTRIGMI[a] + fAnodes - 1 + fTref;
        nextTtrig = data->SOFTWIM_S[section].TTRIGME[a];
        multPerAnode[idAnodeTtrig] = nextTtrig - curTtrig;
        // std::cout << "  * idAnodeTtrig = " << idAnodeTtrig << std::endl;
        // std::cout << "    multPerAnode[" << idAnodeTtrig << "] = " << multPerAnode[idAnodeTtrig] << std::endl;
        for (int hit = curTtrig; hit < nextTtrig; hit++)
        {
            pileupFLAG = (data->SOFTWIM_S[section].TTRIGv[hit] & 0x00040000) >> 18;
            overflowFLAG = (data->SOFTWIM_S[section].TTRIGv[hit] & 0x00080000) >> 19;
            new ((*fArray)[fArray->GetEntriesFast()]) R3BSofTwimMappedData(
                section, idAnodeTtrig, data->SOFTWIM_S[section].TTRIGv[hit], 0, pileupFLAG, overflowFLAG);
            // std::cout << "valTimeTtrig = " << data->SOFTWIM_S[section].TTRIGv[hit] << std::endl;
        }
        curTtrig = nextTtrig;
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

        //if (idAnodeEnergy != idAnodeTime)
        //    LOG(ERROR) << "R3BSofTwimReader::ReadData ERROR ! MISMATCH FOR ANODE ID IN ENERGY #" << idAnodeEnergy
          //             << " AND TIME #" << idAnodeTime;
        uint32_t nextAnodeTimeStart = data->SOFTWIM_S[section].TME[a];
        uint32_t nextAnodeEnergyStart = data->SOFTWIM_S[section].EME[a];
        multPerAnode[idAnodeTime] = nextAnodeTimeStart - curAnodeTimeStart;
        //if (multPerAnode[idAnodeTime] != (nextAnodeEnergyStart - curAnodeEnergyStart))
        //    LOG(ERROR) << "R3BSofTwimReader::ReadData ERROR ! MISMATCH FOR MULTIPLICITY PER ANODE IN ENERGY AND TIME";
        for (int hit = curAnodeTimeStart; hit < nextAnodeTimeStart; hit++)
        {
            // Attention, here the numbering is 0-based for section and anodes
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
