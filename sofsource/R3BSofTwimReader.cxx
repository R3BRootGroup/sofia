#include "FairLogger.h"

#include "FairRootManager.h"
#include "R3BSofTwimReader.h"
#include "R3BSofAnodeMappedData.h"
#include "R3BSofTrefMusicMappedData.h"

extern "C" {
#include "ext_data_client.h"
#include "ext_h101_softwim.h"
}

#include <iostream>

using namespace std;

R3BSofTwimReader::R3BSofTwimReader(EXT_STR_h101_SOFTWIM* data, UInt_t offset)
  : R3BReader("R3BSofTwimReader")
  , fData(data)
  , fOffset(offset)
  , fLogger(FairLogger::GetLogger())
  , fArrayAnodes(new TClonesArray("R3BSofAnodeMappedData"))
  , fArrayTref(new TClonesArray("R3BSofTrefMusicMappedData"))
{
}

R3BSofTwimReader::~R3BSofTwimReader()
{}

Bool_t R3BSofTwimReader::Init(ext_data_struct_info *a_struct_info)
{
  int ok;
  EXT_STR_h101_SOFTWIM_ITEMS_INFO(ok, *a_struct_info, fOffset,EXT_STR_h101_SOFTWIM, 0);
  if (!ok) {
    perror("ext_data_struct_info_item");
    LOG(ERROR) << "Failed to setup structure information." <<FairLogger::endl;
    return kFALSE;
  }

  // Register output array in tree
  FairRootManager::Instance()->Register("SofTwim","MappedDim", fArrayAnodes, kTRUE);
  FairRootManager::Instance()->Register("SofTrefTwim","MappedDim", fArrayTref, kTRUE);

  // clear struct_writer's output struct. Seems ucesb doesn't do that
  // for channels that are unknown to the current ucesb config.
  EXT_STR_h101_SOFTWIM_onion* data = (EXT_STR_h101_SOFTWIM_onion*)fData;
  for (int p=0;p<NUM_SOFTWIM_PLANES;p++)
    for(int s=0;s<NUM_SOFTWIM_SECTIONS;s++){
      data->SOFTWIM_P[p].S[s].EM=0;
      data->SOFTWIM_P[p].S[s].TM=0;
      data->SOFTWIM_P[p].S[s].TREFM=0;
    }
    
    
  return kTRUE;
}

Bool_t R3BSofTwimReader::Read()
{
  // Convert plain raw data to multi-dimensional array
  EXT_STR_h101_SOFTWIM_onion* data = (EXT_STR_h101_SOFTWIM_onion*)fData;

  // loop over all planes and sections
  for (UShort_t p=0;p<NUM_SOFTWIM_PLANES;p++)
    for(UShort_t s=0;s<NUM_SOFTWIM_SECTIONS;s++){
      ReadData(data,p,s);
    }
  return kTRUE;
}

void R3BSofTwimReader::Reset()
{
    // Reset the output array
    fArrayAnodes->Clear();
    fArrayTref->Clear();
}

Bool_t R3BSofTwimReader::ReadData(EXT_STR_h101_SOFTWIM_onion* data, UShort_t plane, UShort_t section)
{

  // --- ----------------- --- //
  // --- ANODE MAPPED DATA --- //
  // --- ----------------- --- //

  // return the number of anodes with data on it
  // nAnodes = 0 : no ion in this gaseous section
  // 0 < nAnodes < 16 : ion escape from this gas section
  // nAnodes=16 : ion stay within the same section
  UShort_t nAnodesEnergy = data->SOFTWIM_P[plane].S[section].EM;
  UShort_t nAnodesTime = data->SOFTWIM_P[plane].S[section].TM;

  // mail from R. Schneider from June 16, 2016: "if you have several hits of one channel in the window of interest, you get an energy and a timing for each hit."
  // --> number of anodes should be the same in energy and time
  // --> multiplicity per anode should also be the same in energy and time
  // mail from R. Schneider from May 21st 2019 : "the hits from one channel are kept in the chronological order."
  // --> for one anode with multi-hit, the first hit in energy correspond to the first hit in time 
  if (nAnodesEnergy!=nAnodesTime)
    LOG(ERROR) << "R3BSofTwimReader::ReadData ERROR ! NOT THE SAME NUMBER OF ANODES HITTED IN ENERGY AND TIME" <<FairLogger::endl;

  // ENERGY AND TIME ARE SORTED 
  // DATA BLOCK FOR THE INPUTS DATA : [DATA TIME OF CH=INPUT+16] [DATA ENERGY OF CH=INPUT]
  uint32_t curAnodeEnergyStart=0;
  uint32_t curAnodeTimeStart=0;
  uint32_t multPerAnode[NUM_SOFTWIM_ANODES];
  for(int mult=0; mult<NUM_SOFTWIM_ANODES;mult++) multPerAnode[mult] = 0;
  for(UShort_t a=0; a<nAnodesEnergy; a++){
    // EMI and TMI give the 1-based anode number
    UShort_t idAnodeEnergy = data->SOFTWIM_P[plane].S[section].EMI[a];
    UShort_t idAnodeTime = data->SOFTWIM_P[plane].S[section].TMI[a];
    if(idAnodeEnergy!=idAnodeTime)
      LOG(ERROR) << "R3BSofTwimReader::ReadData ERROR ! MISMATCH FOR ANODE ID IN ENERGY #" << idAnodeEnergy<< " AND TIME #" << idAnodeTime <<FairLogger::endl;
    uint32_t nextAnodeEnergyStart = data->SOFTWIM_P[plane].S[section].EME[a];
    uint32_t nextAnodeTimeStart = data->SOFTWIM_P[plane].S[section].TME[a];
    multPerAnode[idAnodeEnergy-1] = nextAnodeEnergyStart-curAnodeEnergyStart;
    if(multPerAnode[idAnodeEnergy-1]!=(nextAnodeEnergyStart-curAnodeEnergyStart))
      LOG(ERROR) << "R3BSofTwimReader::ReadData ERROR ! MISMATCH FOR MULTIPLICITY PER ANODE IN ENERGY AND TIME" <<FairLogger::endl;
    for(int hit=curAnodeEnergyStart; hit<nextAnodeEnergyStart;hit++){
      R3BSofAnodeMappedData * mappedAnode = new ((*fArrayAnodes)[fArrayAnodes->GetEntriesFast()])R3BSofAnodeMappedData(plane+1,section+1,idAnodeEnergy);
      mappedAnode->fEnergy = data->SOFTWIM_P[plane].S[section].Ev[hit];
      mappedAnode->fTime = data->SOFTWIM_P[plane].S[section].Tv[hit];
    }
    curAnodeEnergyStart=nextAnodeEnergyStart;
    curAnodeTimeStart=nextAnodeTimeStart;
  }

  // --- ---------------- --- //
  // --- TREF MAPPED DATA --- //
  // --- ---------------- --- //

  // First check the validity of the data : Tref mult should be equal to one
  // TREFM = 0 or 1
  // if TREFM == 0 : multTref=0
  // if TREFM == 1 : multTref=TREF
  UShort_t multTref = 0;
  if(data->SOFTWIM_P[plane].S[section].TREFM==1)     multTref = data->SOFTWIM_P[plane].S[section].TREF;
  UShort_t valTref = 0;
  if (multTref>=1) {
    valTref = data->SOFTWIM_P[plane].S[section].TREFv[0];
    R3BSofTrefMusicMappedData * mappedTref = new ((*fArrayTref)[fArrayTref->GetEntriesFast()])R3BSofTrefMusicMappedData(plane+1,section+1,valTref,multTref,nAnodesEnergy);
  }
  
  return kTRUE;
}

ClassImp(R3BSofTwimReader)
