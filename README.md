# Purpose

SOFIA folder inside R3BRoot describes the SOFIA detectors which are going to be employed
for the fission setup within the R3B/GLAD magnet.
The R3BRoot software is based on the FairRoot framework and can be used to perform Monte Carlo simulations and experimental data analysis of the R3B (Reactions with Relativistic Radioactive Beams) nuclear physics experiments at the FAIR research center (Facility for Antiproton and Ion Research).

For the software-related user support you can post a new topic on our [forum](https://forum.gsi.de/index.php?t=index&cat=40&).

# Download

~~~bash
git clone https://github.com/R3BRootGroup/sofia.git
~~~

# Install

### Required software

First, you will need to install FairSoft, FairRoot and R3BRoot. For more details: [instructions](https://www.r3broot.gsi.de/installation).

### Configure and compile (first R3BRoot and then sofia)

~~~bash
mkdir build
cd build
export SIMPATH=%PATH_TO_FAIRSOFT%
export FAIRROOTPATH=%PATH_TO_FAIRROOT%
cmake ../R3BRoot/
make
cd ..
mkdir build-sofia
cd build-sofia
cmake ../sofia/
make
~~~

# Simulations (ongoing work)

~~~bash
cd %BUILD_DIRECTORY_FOR_SOFIA%
. ./config.sh
cd ../sofia/macros/sofia/
root -l sofiasim.C
~~~

This will create output file `sofiasim.root` with the simulation results and parameter file `sofiapar.root` with geometry and magnetic field parameters.

After the simulation run:

1. To start an event display:

~~~bash
root -l eventDisplay.C
~~~

2. To perform a quick analysis with GUI:

~~~bash
root -l sofiasim.root
[] evt->StartViewer();
~~~

3. To convert the MC results into detector-like signals and open the output file:

~~~bash
root -l run_digi.C
[] .q
root -l sofiahits.root
[] evt->StartViewer();
~~~

# Data Analysis

> ...Under development...
