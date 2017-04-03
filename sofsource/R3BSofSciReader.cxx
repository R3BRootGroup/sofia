#include "FairLogger.h"

#include "TClonesArray.h"
#include "FairRootManager.h"
#include "R3BSofSciReader.h"
#include "R3BSofSciMappedData.h"

extern "C" {
#include "ext_data_client.h"
#include "ext_h101_sofsci.h"
}

#define NUM_SOFSCI_DETECTORS 2
#define NUM_SOFSCI_CHANNELS  2
#include <iostream>

using namespace std;

R3BSofSciReader::R3BSofSciReader(EXT_STR_h101_SOFSCI* data, UInt_t offset)
  : R3BReader("R3BSofSciReader")
  , fData(data)
  , fOffset(offset)
  , fLogger(FairLogger::GetLogger())
  , fArray(new TClonesArray("R3BSofSciMappedData"))
{
}

R3BSofSciReader::~R3BSofSciReader()
{}

Bool_t R3BSofSciReader::Init(ext_data_struct_info *a_struct_info)
{
  int ok;
  EXT_STR_h101_SOFSCI_ITEMS_INFO(ok, *a_struct_info, fOffset,EXT_STR_h101_SOFSCI, 0);
  if (!ok) {
    perror("ext_data_struct_info_item");
    fLogger->Error(MESSAGE_ORIGIN,"Failed to setup structure information.");
    return kFALSE;
  }

  // Register output array in tree
  FairRootManager::Instance()->Register("SofSci","MappedDim", fArray, kTRUE);
  
  // clear struct_writer's output struct. Seems ucesb doesn't do that
  // for channels that are unknown to the current ucesb config.
  EXT_STR_h101_SOFSCI_onion* data = (EXT_STR_h101_SOFSCI_onion*)fData;
  for (int d=0;d<NUM_SOFSCI_DETECTORS;d++)
    data->SOFSCI[d].TFM=0;
  
  return kTRUE;
}

Bool_t R3BSofSciReader::Read()
{
  // Convert plain raw data to multi-dimensional array
  EXT_STR_h101_SOFSCI_onion* data = (EXT_STR_h101_SOFSCI_onion*)fData;

  /*
    typedef struct EXT_STR_h101_SOFSCI_onion_t
    {
      struct {
        uint32_t TFM;             : number of PTMTs with FT hits - this is not the number of hits
	uint32_t TFMI[2 / TFM /]; : PMT number
	uint32_t TFME[2 / TFM /]; : index (address?) of the first hit for the next PMT in the data word 
	uint32_t TF;              : total number of hits
	uint32_t TFv[80 / TF /];  : fine time value for this hit
	uint32_t TCM;             : this is = TFM
	uint32_t TCMI[2 / TCM /]; : this is = TFMI[]
	uint32_t TCME[2 / TCM /]; : this is = TFME[]
	uint32_t TC;              : this is = TF
	uint32_t TCv[80 / TC /];  : coarse time value for this hit
      } SOFSCI[2];
    } EXT_STR_h101_SOFSCI_onion;
*/

  // loop over all detectors
  for (int d=0;d<NUM_SOFSCI_DETECTORS;d++){		

    uint32_t numberOfPMTsWithHits = data->SOFSCI[d].TFM; // also = data->SOFSCI[d].TCM; 

    // loop over channels with hits
    uint32_t curChannelStart=0;
    for (int pmmult=0;pmmult<numberOfPMTsWithHits;pmmult++) {
      uint32_t pmtval=data->SOFSCI[d].TFMI[pmmult]; 
      uint32_t nextChannelStart=data->SOFSCI[d].TFME[pmmult];  
      // put the mapped items {det,pmt,finetime, coarsetime} one after the other in the fArray
      for (int hit=curChannelStart;hit<nextChannelStart;hit++){  
	new ((*fArray)[fArray->GetEntriesFast()])R3BSofSciMappedData(d+1,pmtval,data->SOFSCI[d].TCv[hit],data->SOFSCI[d].TFv[hit]);
      }
      curChannelStart=nextChannelStart;
    }
  }
  return kTRUE;
}

void R3BSofSciReader::Reset()
{
    // Reset the output array
    fArray->Clear();
}

ClassImp(R3BSofSciReader)

