#ifndef __DETCFG_H__
#define __DETCFG_H__

// --- -------------------- --- //`
// --- SINGLE SCINTILLATORS --- //
// --- -------------------- --- //
#define NUMBER_OF_SOFSCI_DETECTORS 1
#define NUMBER_OF_SOFSCI_CHANNELS  3
#define ID_SOFSCI_CAVEC            1
#if NUMBER_OF_SOFSCI_DETECTORS > 1
#define NUMBER_OF_SOFSCI_TOF (NUMBER_OF_SOFSCI_DETECTORS*(NUMBER_OF_SOFSCI_DETECTORS-1)/2)
#endif

// --- ------------------- --- //
// --- TIME-OF-FLIGHT WALL --- //
// --- ------------------- --- //
#define NUMBER_OF_SOFTOFW_PLASTICS         28
#define NUMBER_OF_SOFTOFW_PMTS_PER_PLASTIC 2


#endif
