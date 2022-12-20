# SOFIA Software

## The SOFIA framework
SOFIA folder inside R3BRoot describes the SOFIA detectors which are going to be employed
for the fission setup within the R3B/GLAD magnet. The R3BRoot software is based on the FairRoot framework and can be used to perform Monte Carlo simulations and experimental data analysis of the R3B (Reactions with Relativistic Radioactive Beams) nuclear physics experiments at the FAIR research center (Facility for Antiproton and Ion Research).

## Discussion Forum
For the software-related user support you can post a new topic on our [forum](https://forum.gsi.de/index.php?t=index&cat=40&).

## Release information
Please see [Releases](https://github.com/R3BRootGroup/sofia/releases)

## Step by step installation

### Required software

First, you will need to install FairSoft and FairRoot. For more details:

1. Install [FairSoft](https://github.com/FairRootGroup/FairSoft)

2. Install [FairRoot](http://fairroot.gsi.de)

### Installation of R3BRoot and sofia (as part of R3BRoot)

~~~bash
export SIMPATH=%PATH_TO_FAIRSOFT%
export FAIRROOTPATH=%PATH_TO_FAIRROOT%
git clone -b dev https://github.com/R3BRootGroup/R3BRoot.git
cd R3BRoot
git clone -b dev https://github.com/R3BRootGroup/macros.git
git clone -b dev https://github.com/R3BRootGroup/sofia.git
cd ..
mkdir build
cd build
cmake ../R3BRoot/
. config.sh
make -j8
~~~

### Some details of the sofia set-up

- Active target (SofAT)
The active target is a gaseous detector which can be seen as a stack of ionisation chambers.
Three targets are mounted as cathode, and signal are readout on four anodes.
Each event, there will always be four energies collected (one per anode).
The comparison of the energy losses collected on two following anodes give the layer (anode or cathode) where the fission occures
AT configuration in the April 2021 beam time :
---
       ANODE 1  :   ALUMINIUM
      CATHODE 1 :   LEAD
       ANODE 2  :   ALUMINIUM
      CATHODE 2 :   LEAD
       ANODE 3  :   ALUMINIUM
      CATHODE 3 :   LEAD 
       ANODE 4  :   ALUMINIUM
---

- Single scintillators (SofSci)
There are one scintillators at S2 and one at cave C. Another scintillator could be inserted at S8 to study the FRS transmission.
Each scintillator is read out by two PMTs: right and left.
At S2, the scintillator gives the position of the beam and make the start of the time of flight of the beam.
At cave C, the scintillator make the stop of the beam, and the start for the times of flight of the fission fragments.

- Common reference signal (SofComRef)
Deux independant DAQ are running at S2 and cave C.
The clock of the VFTX-TDC are not synchronised.
To calculate the ToF from S2 to Cave C, one common signal is sent to each cave.

- Time-of-flight wall (SofToFW)
made with 28 vertical scintillators. 
Each scintillator are readout by 2 PMTs : down and up.
In order to follow the coordinate system, plastic 1 is on the right and plastic 28 is on the left, looking in the same direction than the beam.

- Triple-MUSIC (SofTriM)
SofTriM has 3 sections along the beam direction (z axis).
Each section has 6 anodes (3 pairs of trapezoidal anodes) which are readout in energy and time.
Energy losses obtained from each section are independant.
We need to combine them to get rid, as much as possible, of the charge states of the secondary beam.

- Twin-MUSIC
SofTwiM is a double detector with a common central cathod.
There are 2 anodes planes (plane 1: right plane, plane 2: left plane).
Each anode plane is vertically segmented (section 1: lower section, section 2: upper section), 
and has 16 anodes along the beam axis.
This defines 4 different detection volumes.
With this geometry, we can simultaneously measure both fission fragments in coincidence, as long as each fragment are going through the SofTwiM in two different detection volumes

- MWPC
There are 3 types of MWMPCs. 
The first type (MWPC0) is coupled to SofTriM, to get the (x,y) position of the beam. In x, there are 64 vertical pads and in y, 64 horizontal pads.
The second type is located before and after the SofTwiM detector to get the (x,y) position of each fission fragment before the magnet. This second kind of MWPC is vertically segmented into two sections. Therefore, in x, there are 64 down pads and 64 up pads, and in y, there are 40 vertically pads.
The third type is located after the magnet and just before the SofToFW. In x, there are 288 vertical pads and in y, 120 horizontal pads.

## Simulations (ongoing work)

Some simulations of two experimental configurations are included inside the sofia directory. You can find the subdirectories :

### geometry

This directory contains all the detector geometries generated from the macros located at ./sofia/macros/geo. The existing geometries are :

- sof_at_v19a.geo.root for the Active target detector
- mwpc_0.geo.root for the MWPC0 detector
- targetvacuumchamber_ams_s455.geo.root for the vacuum chamber and AMS silicon detectors
- mwpc_1.geo.root for the MWPC1 detector
- twinmusic_v22.geo.root for the twin-Music detector
- mwpc_2.geo.root for the MWPC2 detector
- mwpc_3.geo.root for the MWPC3 detector
- glad_s455.geo.root for GLAD magnet
- sof_tof_v21.geo.root for the ToF-Wall detector

### sofia/macros/coulexsim

This directory contains the macros to simulate the coulex experiment. 

~~~bash
cd %BUILD_DIRECTORY_FOR_R3BROOT%
. ./config.sh
cd ./sofia/macros/coulexsim/
root -l 
.L runsim.C
runsim(100)
~~~
where 100 is the number of events.

This will create an output file `sim.root` with the simulation results and a parameter file `par.root` with geometry and magnetic field parameters.

After the simulation :

1. To start an event display:

~~~bash
root -l eventDisplay.C
~~~

2. To perform a quick analysis with GUI :

~~~bash
root -l sim.root
[] evt->StartViewer();
~~~

### sofia/macros/p2psim

This directory contains the macros to simulate the (p,2p)-fission experiment. 

<p align="center">
  <img src="/input/p2p_r3bsetup.jpg" width="650" title="(p,2p)-fission experiment">
</p>

First, load the environment variables

~~~bash
cd %BUILD_DIRECTORY_FOR_R3BROOT%
. ./config.sh
~~~
Execute the (p,2p)-fission simulation as follows. First one must go into the folder

~~~bash
cd ./sofia/macros/p2psim/
~~~
and then :

~~~bash
root -l 'runsim.C(1000)'
~~~
where 1000 is the number of events.

This will create an output file `sim.root` with the simulation results and a parameter file `par.root` with geometry and magnetic field parameters.

After the simulation :

1. To start an event display :

~~~bash
root -l eventDisplay.C
~~~

2. To perform a quick analysis with GUI :

~~~bash
root -l sim.root
[] evt->StartViewer();
~~~

## Data Analysis (ongoing work)

Data analysis is included inside the sofia directory. You can find the subdirectories :

### sofdata

This directory contains all the data TCloneArray structures for the different levels of the detection system.

### sofsource

This directory contains all the readers related to SOFIA detectors :

- R3BSofAtReader for the Active target detector
- R3BSofTrimReader for the triple ionization chamber
- R3BSofSciReader for the plastic scintillator located at the FRS and at the entrance of Cave-C (Start)
- R3BSofToFWReader for the large area ToF-Wall located behind GLAD
- R3BSofScalersReader for the scalers
- R3BSofCorrvReader for time correlations of DAQ sub-systems
- Readers for the TWIM and MWPC detectors can be found on [R3BRoot](https://github.com/R3BRootGroup/R3BRoot)

and UCESB data structures are located at sofsource/ext :

- ext_h101_sofat.h
- ext_h101_softrim.h
- ext_h101_sofsci.h
- ext_h101_softofw.h
- ext_h101_sofscalers.h
- ext_h101_sofcorrv.h

this allows to get data at mapped level (raw data of each detector).

### sci

This directory contains all the classes to analyse the single scintillator data and move the raw data into cal and hit data levels.

### tcal

This directory contains some classes to obtain the calibration parameters of the VFTX channels, this will allow to get the time in ns.

### at

This directory contains all the classes to analyse the active target data and move the raw data into cal and hit data levels.

### trim

This directory contains all the classes to analyse the triple-Music data and move the raw data into cal and hit data levels.

### mwpc

This directory contains all the classes to analyse the MWPC data and move the raw data into cal and hit data levels.

### twim

This directory contains all the classes to analyse the twin-Music data and move the raw data into cal and hit data levels.

### tofwall

This directory contains all the classes to analyse the ToF-Wall data and move the raw data into cal and hit data levels.

### sofonline

This directory contains all the online analysis classes to correlate the data collected by the SOFIA detectors.

### sofana

This directory contains all the analysis classes to correlate the data collected by the SOFIA detectors.


### macros/s455Up2p

This directory contains all the macros for online and offline analysis of the (p,2p)-fission experiment.

#### macros/s455Up2p/online

This directory contains the online macro for the analysis of the (p,2p)-fission experiment. Execute it as:

~~~bash
root -b main_online.C
~~~
after defining the stream data server and the port number for data visualization.

#### macros/s455Up2p/unpack

This directory contains the offline macros for the analysis of the (p,2p)-fission experiment. There are two macros:

- unpack_offline.C for producing a root file with the mapped/raw data of all the detectors
- cal_offline.C for producing a root file with the cal and hit data levels of all the detectors

After defining the paths to the input files of each macro, one can execute it as follows:

~~~bash
1) If all the parameters are right by default
   root -l unpack_offline.C
2) If one wants to select a RunId, for instance 'RunId = 273'
   root -l 'unpack_offline.C(273)'
3) If one wants to select a RunId and max number of events, for instance 'RunId = 273' and 'nev = 200'
   root -l 'unpack_offline.C(273,200)'
~~~
or

~~~bash
1) If all the parameters are right by default
   root -l cal_offline.C
2) If one wants to select a RunId, for instance 'RunId = 273'
   root -l 'cal_offline.C(273)'
3) If one wants to select a RunId and max number of events, for instance 'RunId = 273' and 'nev = 200'
   root -l 'cal_offline.C(273,200)'
~~~
