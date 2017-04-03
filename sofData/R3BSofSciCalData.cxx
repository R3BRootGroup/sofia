#include "R3BSofSciCalData.h"
#include "math.h"

/* for the data analysis of the Los detector. 
 * Introduced by Ralf, Feb 2016
 *
 * March 2016
 * Rewrote the Cal structure to provide individual leafs for the
 * left, top, right and bottom signals. This allows to plot
 * the time differences via cbmsim->Draw(...) interactively (aka without
 * looping over all channels) which is crucial for a quick check of the
 * detector status during the experiment.
 *  
 * There is a fifth channel "ref" available which holds the master 
 * trigger, as time reference.
 *  
 */


R3BSofSciCalData::R3BSofSciCalData()
    : fDetector(0)
{
}

R3BSofSciCalData::R3BSofSciCalData(UChar_t detector )
    : fDetector(detector)
    , fTime_PMT1_ns(std::numeric_limits<double>::quiet_NaN()) 
    , fTime_PMT2_ns(std::numeric_limits<double>::quiet_NaN())
    , fTime_REF_ns(std::numeric_limits<double>::quiet_NaN())
{
}

Double_t R3BSofSciCalData::GetMeanTime()
{
  Double_t mean=0;
  Int_t num_times=0;
  if (!isnan(fTime_PMT1_ns)) {mean+=fTime_PMT1_ns;num_times++;}
  if (!isnan(fTime_PMT2_ns)) {mean+=fTime_PMT2_ns;num_times++;}
  return num_times ? mean/num_times : NAN;
}

Double_t R3BSofSciCalData::GetRawPos()
{
  Double_t rawpos=std::numeric_limits<double>::quiet_NaN();
  if ((!isnan(fTime_PMT1_ns))&&(!isnan(fTime_PMT2_ns))) rawpos=fTime_PMT1_ns-fTime_PMT2_ns;
  return rawpos;
}

ClassImp(R3BSofSciCalData)
