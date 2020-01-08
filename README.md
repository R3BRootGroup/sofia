# Purpose

SOFIA folder inside R3BRoot describes the SOFIA detectors which are going to be employed
for the fission setup within the R3B/GLAD magnet.
The R3BRoot software is based on the FairRoot framework and can be used to perform Monte Carlo simulations and experimental data analysis of the R3B (Reactions with Relativistic Radioactive Beams) nuclear physics experiments at the FAIR research center (Facility for Antiproton and Ion Research).

For the software-related user support you can post a new topic on our [forum](https://forum.gsi.de/index.php?t=index&cat=40&).

# Install

### Required software

First, you will need to install FairSoft and FairRoot. For more details: [instructions](https://www.r3broot.gsi.de/installation).

### Installation of R3BRoot and sofia (as part of R3BRoot)

~~~bash
export SIMPATH=%PATH_TO_FAIRSOFT%
export FAIRROOTPATH=%PATH_TO_FAIRROOT%
git clone https://github.com/R3BRootGroup/R3BRoot.git
cd R3BRoot
git checkout dev
git clone https://github.com/R3BRootGroup/macros.git
cd macros
git checkout dev
cd ..
git clone https://github.com/R3BRootGroup/sofia.git
cd sofia
git checkout dev
cd ..
cd ..
mkdir build
cd build
cmake ../R3BRoot/
. config.sh
make -j4
~~~

# Simulations (ongoing work)

~~~bash
cd %BUILD_DIRECTORY_FOR_SOFIA%
. ./config.sh
cd ../sofia/macros/coulexsim/
root -l 
.L run_sim.C
run_sim(100)
~~~
where 100 is the number of events.

This will create output file `sim.root` with the simulation results and parameter file `par.root` with geometry and magnetic field parameters.

After the simulation run:

1. To start an event display:

~~~bash
root -l eventDisplay.C
~~~

2. To perform a quick analysis with GUI:

~~~bash
root -l sim.root
[] evt->StartViewer();
~~~

3. To convert the MC results into detector-like signals and open the output file: (not yet!)

~~~bash
root -l run_digi.C
[] .q
root -l sofiahits.root
[] evt->StartViewer();
~~~

# Data Analysis (ongoing work)

Data analysis is included inside the sofia directory. You can find the subdirectories :

### sofdata

This directory contains all the data containers for the different levels of the detection system.

### sofsource

This directory contains all the readers related to SOFIA detectors :

- R3BSofMwpcReader for the MWPC tracking detectors
- R3BSofTrimReader for the triple ionization chamber
- R3BSofSciReader for the plastic scintillator located at the FRS and at the entrance of Cave-C (Start)
- R3BSofTwimReader for the double ionization chamber located in front of target
- R3BSofToFWReader for the large area ToF-Wall located behind GLAD

and UCESB data structures :
- ext_h101_sofmwpc.h
- ext_h101_softrim.h
- ext_h101_sofsci.h
- ext_h101_softwim.h
- ext_h101_softofw.h

that allow to get data at mapped level (raw data of each detector).

### some details of the sofia set-up

- Active target (SofAT)
The active target is a gaseous detector which can be seen as a stack of ionisation chambers.
Four targets are mounted as cathode, and signal are readout on 5 anodes.
Each event, there will always be 5 energies collected (one per anode).
The comparison of the energy losses collected on two following anodes give the layer (anode or cathode) where the fission occures
AT in the October 2014 beam time :
---
       ANODE 1  :   ALUMINIUM
      CATHODE 1 :   URANIUM 238
       ANODE 2  :   ALUMINIUM
      CATHODE 2 :   URANIUM 238
       ANODE 3  :   ALUMINIUM
      CATHODE 3 :   ALUMINIUM 
       ANODE 4  :   ALUMINIUM
      CATHODE 4 :   LEAD
       ANODE 5  :   ALUMINIUM
---

- Single scintillators (SofSci)
There are one scintillators at S2 and one at cave C.
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

- Twin-MUSIC (SofTwiM)
SofTwiM is a double detector with a common central cathod.
There are 2 anodes planes (plane 1: right plane, plane 2: left plane).
Each anode plane is vertically segmented (section 1: lower section, section 2: upper section), 
and has 16 anodes along the beam axis.
This defines 4 different detection volumes.
With this geometry, we can simultaneously measure both fission fragments in coincidence, as long as each fragment are going through the SofTwiM in two different detection volumes

- MWPC (SofMWPC)
There are 3 MWMPCs. 
The first is coupled to SofTriM, to get the (x,y) position of the beam. In x, there are 64 vertical pads and in y, 40 horizontal pads.
The second is located before the SofTwiM to get the (x,y) position of each fission fragment before the magnet. This second MWPC is vertically segmented into to. Therefore, in x, there are 64 down pads and 64 up pads, and in y, there are 40 vertically pads.
The third is located after the magnet and just before the SofToFW. In x, there are 288 vertical pads and in y, 120 horizontal pads.



