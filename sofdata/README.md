
#DATA CONTAINERS FOR THE SOFIA SET - UP

### R3BSofXXXXMappedData
------------------------
Data container of the Mapped Data, obtained from the UCESB to feed R3Broot analysis.

### R3BSofXXXXCalData
-----------------
Data container of the Cal Data, obtained from the Mapped data, after applying calibration procedure

#R3BSofATMappedData
--------------------------
Mapped data of the SOFIA ACTIVE TARGET, in case the readout is done with a MDPP16 module.

#R3BSofComRefMappedData
------------------------
     fDetector   // 1 : signal at S2, 2 : signal at Cave C
     fPmt        // 1..2    1: PMT right, 2: PMT left
     fTimeCoarse // coarse time of the reference signal
     fTimeFine   // fine time of the reference signal

#R3BSofTofWMappedData
----------------------
the arguments are the following:
     fDetector   1..28   1: first plastic on the right, 28: first plastic on the left
     fPmt        1..2    1: PMT down, 2: PMT up
     fTimeCoarse coarse time given by the VFTX
     fTimeFine   fine time given by the VFTX
     fValid      kTRUE: if multiplicity 1

#R3BSofMusicMappedData
--------------------------
Twin-MUSIC readout by the MDPP16 module
    fSection       1: right/down, 2: right/up, 3: left/down, 4: left/up
    fAnode         1..16: anodes id along beam axis, 17: time reference to calculate the drift time
    fTime          time given by the MDPP16
    fEnergy        energy given by MDPP16, energy=0 for anode 17


Triple-MUSIC readout by the MDPP16 module
    fSection       1: first, 2: middle, 3: last in the beam direction
    fAnode         1..6: anodes id along beam axis, 7: time reference to calculate the drift time
    fTime          time given by the MDPP16
    fEnergy        energy given by MDPP16, energy=0 for anode 7

#MWPCs
-------
Mwpc are readout by VMMR8
