#include "R3BSofSciReader.h"

#include "FairLogger.h"
#include "FairRootManager.h"
#include "R3BSofSciMappedData.h"

extern "C"
{
#include "ext_data_client.h"
#include "ext_h101_sofsci.h"
}

#include <iostream>

using namespace std;

R3BSofSciReader::R3BSofSciReader(EXT_STR_h101_SOFSCI* data, size_t offset)
    : R3BSofSciReader(data, offset, 1)
{
}

R3BSofSciReader::R3BSofSciReader(EXT_STR_h101_SOFSCI* data, size_t offset, Int_t NumSci)
    : R3BReader("R3BSofSciReader")
    , fData(data)
    , fOffset(offset)
    , fOnline(kFALSE)
    , fArray(new TClonesArray("R3BSofSciMappedData")) // class name
    , fNumSci(NumSci)
{
}

R3BSofSciReader::~R3BSofSciReader()
{
    LOG(debug) << "R3BSofSciReader: Delete instance";
    if (fArray)
    {
        delete fArray;
    }
}

Bool_t R3BSofSciReader::Init(ext_data_struct_info* a_struct_info)
{
    Int_t ok;
    LOG(info) << "R3BSofSciReader::Init()";
    EXT_STR_h101_SOFSCI_ITEMS_INFO(ok, *a_struct_info, fOffset, EXT_STR_h101_SOFSCI, 0);
    if (!ok)
    {
        perror("ext_data_struct_info_item");
        LOG(error) << "R3BSofSciReader::Failed to setup structure information.";
        return kFALSE;
    }

    // Register output array in tree
    FairRootManager::Instance()->Register("SofSciMappedData", "SofSci", fArray, !fOnline);
    fArray->Clear();

    // clear struct_writer's output struct. Seems ucesb doesn't do that
    // for channels that are unknown to the current ucesb config.
    EXT_STR_h101_SOFSCI_onion* data = (EXT_STR_h101_SOFSCI_onion*)fData;
    for (int d = 0; d < fNumSci; d++)
        data->SOFSCI[d].TFM = 0;

    return kTRUE;
}

Bool_t R3BSofSciReader::Read()
{
    // Convert plain raw data to multi-dimensional array
    EXT_STR_h101_SOFSCI_onion* data = (EXT_STR_h101_SOFSCI_onion*)fData;

    /* REMINDER :
       typedef struct EXT_STR_h101_SOFSCI_onion_t
       {

       struct {
       uint32_t TFM;             : number of PTMTs with FT hits - this is not the number of hits
       uint32_t TFMI[3 // TFM ]; : PMT number [1: RIGHT, 2:LEFT, 3:TREF]
       uint32_t TFME[3 // TFM ]; : index (address?) of the first hit for the next PMT in the data word
       uint32_t TF;              : total number of hits
       uint32_t TFv[30 // TF ];  : fine time value for this hit
       uint32_t TCM;             : this is equal to TFM
       uint32_t TCMI[3 // TCM ]; : this is equal to TFMI
       uint32_t TCME[3 // TCM ]; : this is equal to TFME
       uint32_t TC;              : this is equal to TF
       uint32_t TCv[30 // TC ];  : coarse time value for this hit
       } SOFSCI[fNumSci]; [fNumSci=1] for primary beam experiment, [fNumSci>1] for secondary beam experiment

       } EXT_STR_h101_SOFSCI_onion;
    */

    // loop over all detectors
    for (Int_t d = 0; d < fNumSci; d++)
    {
        uint32_t numberOfPMTsWithHits_TF = data->SOFSCI[d].TFM;
        uint32_t numberOfPMTsWithHits_TC = data->SOFSCI[d].TCM;
        if (numberOfPMTsWithHits_TF != numberOfPMTsWithHits_TC)
        {
            LOG(error) << "R3BSofSciReader::Read() Error in unpacking, unconsistency between TF and TC for SofSci !";
        }
        else
        {
            // loop over channels with hits
            uint32_t curChannelStart = 0;
            for (Int_t pmmult = 0; pmmult < numberOfPMTsWithHits_TF; pmmult++)
            {
                uint32_t pmtid_TF = data->SOFSCI[d].TFMI[pmmult];
                uint32_t pmtid_TC = data->SOFSCI[d].TCMI[pmmult];
                if (pmtid_TF != pmtid_TC)
                {
                    LOG(error) << "R3BSofSciReader::Read() Error in unpacking, unconsistency between the PMT id for TF "
                                  "and TC for SofSci !";
                }
                uint32_t nextChannelStart = data->SOFSCI[d].TFME[pmmult];
                // put the mapped items {det,pmt,finetime, coarsetime} one after the other in the fArray
                for (Int_t hit = curChannelStart; hit < nextChannelStart; hit++)
                {
                    auto item =
                        new ((*fArray)[fArray->GetEntriesFast()]) R3BSofSciMappedData(d + 1, // 1-based numbering
                                                                                      pmtid_TF,
                                                                                      data->SOFSCI[d].TCv[hit],
                                                                                      data->SOFSCI[d].TFv[hit]);
                }
                curChannelStart = nextChannelStart;
            }
        }
    } // end of for(d)
    return kTRUE;
}

void R3BSofSciReader::Reset()
{
    // Reset the output array
    fArray->Clear();
}

ClassImp(R3BSofSciReader);
