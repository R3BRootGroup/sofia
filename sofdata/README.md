
# DATA CONTAINERS FOR THE SOFIA SET-UP

### R3BSofXXXXMappedData
------------------------
Data container of the Mapped Data, obtained from the UCESB to feed R3Broot analysis.

### R3BSofXXXXCalData
-----------------
Data container of the Cal Data, obtained from the Mapped data, after applying calibration procedure




# R3BSofATMadcMappedData
------------------------
Mapped data of the SOFIA ACTIVE TARGET, in case the readout is done with a MADC32 module.
The argument of this class is a vector of <NumberOfAnodes> energies.

# R3BSofATMdpp16MappedData
--------------------------
under developpment
Mapped data of the SOFIA ACTIVE TARGET, in case the readout is done with a MDPP16 module.
The argument of this class is a vector of <NumberOfAnodes> energies.

# R3BSofComRefMappedData
------------------------
     fDetector   // 1 : signal at S2, 2 : signal at Cave C
     fTimeCoarse // coarse time of the reference signal
     fTimeFine   // fine time of the reference signal

# R3BSofToFWMappedData
----------------------
the arguments are the following:
     fDetector   1..28   1: first plastic on the right, 28: first plastic on the left
     fPmt        1..2    1: PMT down, 2: PMT up
     fTimeCoarse coarse time given by the VFTX
     fTimeFine   fine time given by the VFTX
     fEnergy     energy given by the MADC32
     fValid      kTRUE: if multiplicity 1 

# R3BSofMusicMdpp16MappedData
-----------------------------
Both MUSICs can be readout by MDPP16 module (cf. 2016 beam test)
Arguments of the class:
                   TriM                       TwiM
    fPlane         1 one plane only           1: right plane,  2:left plane
    fSection       1..3 along beam axis       1: down section, 2: up section
    fAnode         1..6 along beam axis       1..16 along beam axis
    fTime          time given by MDPP16       time given by MDPP16
    fEnergy        energy given by MDPP16     energy given by MDPP16

R3BSofMusicMadcMappedData
--------------------------
Both MUSICs can be readout by MADC+VFTX2 modules (cf. 2014 beam time).
Arguments of the class:
                   TriM                       TwiM
    fPlane         1 one plane only           1: right plane, 2:left plane
    fSection       1..3 along beam axis       1: down section, 2: up section
    fAnode         1..6 along beam axis       1..16 along beam axis
    fTimeCoarse    TC given by VFTX2          TC given by VFTX
    fTimeFine      TF given by VFTX2          TF given by VFTX2
    fEnergy        energy given by MADC       energy given by MADC
    fValid         kTRUE: mult1 in time       kTRUE: mult1 in time
                   kFALSE: mult<>1 in time    kFALSE: mult<>1 in time



# MWPCs
-------

under developpement