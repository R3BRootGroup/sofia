#include "FairLogger.h"

#include "TClonesArray.h"
#include "FairRootManager.h"
#include "R3BSofComRefReader.h"
#include "R3BSofComRefMappedData.h"

extern "C" {
#include "ext_data_client.h"
#include "ext_h101_sofcomref.h"
}

#define NUM_SOFCOMREF_SIGNALS 2
#include <iostream>

using namespace std;

R3BSofComRefReader::R3BSofComRefReader(EXT_STR_h101_SOFCOMREF* data, UInt_t offset)
  : R3BReader("R3BSofComRefReader")
  , fData(data)
  , fOffset(offset)
  , fLogger(FairLogger::GetLogger())
  , fArray(new TClonesArray("R3BSofComRefMappedData"))
{
}

R3BSofComRefReader::~R3BSofComRefReader()
{}

Bool_t R3BSofComRefReader::Init(ext_data_struct_info *a_struct_info)
{
  int ok;
  EXT_STR_h101_SOFCOMREF_ITEMS_INFO(ok, *a_struct_info, fOffset,EXT_STR_h101_SOFCOMREF, 0);
  if (!ok) {
    perror("ext_data_struct_info_item");
    fLogger->Error(MESSAGE_ORIGIN,"Failed to setup structure information.");
    return kFALSE;
  }

  // Register output array in tree
  FairRootManager::Instance()->Register("SofComRef","MappedDim", fArray, kTRUE);
  // clear struct_writer's output struct. Seems ucesb doesn't do that
  // for channels that are unknown to the current ucesb config.
  EXT_STR_h101_SOFCOMREF_onion* data = (EXT_STR_h101_SOFCOMREF_onion*)fData;
  for (int d=0;d<NUM_SOFCOMREF_SIGNALS;d++)
    data->SOFCOMREF_S[d].TFM=0;
  
  return kTRUE;
}

Bool_t R3BSofComRefReader::Read()
{
  // Convert plain raw data to multi-dimensional array
  EXT_STR_h101_SOFCOMREF_onion* data = (EXT_STR_h101_SOFCOMREF_onion*)fData;
  
  /*
    REMINDER OF THE DATA STRUCTURE

    typedef struct EXT_STR_h101_SOFCOMREF_onion_t
    {
    // RAW 
    struct {
    uint32_t TFM;
    uint32_t TFMI[1 // TFM ];
    uint32_t TFME[1 // TFM ];
    uint32_t TF;
    uint32_t TFv[2 // TF ];
    uint32_t TCM;
    uint32_t TCMI[1 // TCM ];
    uint32_t TCME[1 // TCM];
    uint32_t TC;
    uint32_t TCv[2 // TC ];
    } SOFCOMREF_S[2];
    
    } EXT_STR_h101_SOFCOMREF_onion;
  */
  

  // loop over all reference signals
  for (int d=0;d<NUM_SOFCOMREF_SIGNALS;d++){		
    uint32_t NumberOfSignalWithHits = data->SOFCOMREF_S[d].TFM; 
    uint32_t TotalNumberOfHits = data->SOFCOMREF_S[d].TF;       
    Bool_t FLAG_SIGNAL = kFALSE;
    if ((TotalNumberOfHits==1) && (NumberOfSignalWithHits==TotalNumberOfHits)) FLAG_SIGNAL = kTRUE;

    // put the mapped items {signal,finetime, coarsetime, valid_flag} one after the other in the fArray
    // no need to use the curChannelStart / nextChannelStart, since there is only one channel per signal
    for(int signalmult=0; signalmult<NumberOfSignalWithHits; signalmult++){
      for (int hit=0;hit<data->SOFCOMREF_S[d].TFME[0];hit++)
	new ((*fArray)[fArray->GetEntriesFast()])R3BSofComRefMappedData(d+1,data->SOFCOMREF_S[d].TCv[hit],data->SOFCOMREF_S[d].TFv[hit],FLAG_SIGNAL);
    }
  }
  return kTRUE;
}

void R3BSofComRefReader::Reset()
{
    // Reset the output array
    fArray->Clear();
}

ClassImp(R3BSofComRefReader)

