# Triple-MUSIC

Today (May 2020), due to the COVID-19 pandemia, the refurbishment of the Triple-MUSIC is still pending.
Therefore, the analysis is developped in order to deal either with the "triangular" shape anodes or with the "rectangular" shape anodes.
Only some calibration parameters will remain to 0 or 1 default value, in case of triangular shape.

# Geometry

For the triangular shape, each section has 3 pairs of anodes, each pair is made of 2 right-angled triangle: one up and one down.
For the rectangular shape, each section has 6 rectangular anodes.

# Calibration of the drift time: from Mapped to Cal 

This step is the same wether we deal with triangular or rectangular shape.
at the begining the calibration parameters of the drift time are given with default values in CalibParams.par

trimDriftTimeOffsets:  Double_t \
  0. 0. 0. 0. 0. 0.  \
  0. 0. 0. 0. 0. 0.  \
  0. 0. 0. 0. 0. 0. 


### Step 1: calculation of raw drift time per anode: DTraw
* This is the first step in Mapped2Cal class

### Step 2: alignment of the drift time per anode: DTalign
* This offset is obtained from the difference of drift time between the raw measured drift-time in Triple-MUSIC and the reconstructed drift-time inferred from the tracking between MWPC0 and MWPC1. 
* IMPORTANT: For this calibration, the input data should be a run with primary beam [no fragments] and an empty target.
* Since the DTraw is calculated in Mapped2Cal, the calculation of the trimDriftTimeOffset take as input SofTrimCalData, and of course the Hit level of the Mwpc0 and Mwpc1 data
* In the R3BSofTrimCalculateDriftTimeOffset, the theoretical value of the drift velocity is actually used.
We need to check if this should be changed to a more "real" drift velocity, which would need to be calibrated before.

# Calibration of the energy loss: from Mapped to Cal

at the begining the calibration parameters of the energy loss are given with default values in CalibParams.par

trimEnergyPedestals: Float_t \
  0.0 0.0 0.0 0.0 0.0 0.0  \
  0.0 0.0 0.0 0.0 0.0 0.0  \
  0.0 0.0 0.0 0.0 0.0 0.0 

trimEnergyMatchGains: Float_t \
  1.0 1.0 1.0 1.0 1.0 1.0  \
  1.0 1.0 1.0 1.0 1.0 1.0  \
  1.0 1.0 1.0 1.0 1.0 1.0

### Step 1: subtraction of the pedestal for the raw energy of each anode Esub
* All parameters of the MDPP16 should be adjusted first: gain, shaping-time, integration/differention, etc...
* Once this is is done and without beam, put the threshold to zero and trig the DAQ with pulser.
* Look at the raw data, and fit the pedestal.
* Replace the value in the CalibParams.par

trimEnergyPedestals: Float_t \
  val_S1_A1 val_S1_A2 val_S1_A3 val_S1_A4 val_S1_A5 val_S1_A6  \
  val_S2_A1 val_S2_A2 val_S2_A3 val_S2_A4 val_S2_A5 val_S2_A6  \
  val_S3_A1 val_S3_A2 val_S3_A3 val_S3_A4 val_S3_A5 val_S3_A6

This subtraction is negligible with MDPP16, if it is not possible to do it, it won't harm.
In the latter case, let the parameters for the pedestal to 0.

### Step 2: gain matching of the energy losses Esub for the two triangular anodes in a pair : Ematch
* This step is only mandatory for the triangular shape.
For rectangular shape, the calibration parameters trimEnergyMatchGains will remain to 1.
For the triangular shape, the calibration parameters correspond to a gain for the "down" anode, therefore the calibration parameter for the up anode is always equal to 1
* Since the Esub is calculated in Mapped2Cal, the calculation of the trimEnergyMatchGains take as input SofTrimCalData.

# Calibration of the energy loss: from Cal to Hit

### Step 1: alignement of the energy loss on primary beam : Ealign and calculation of the energy loss per section Eraw_sum
* This step is slightly different for rectangular or triangular anodes.
* For the rectangular anode, each anode is aligned to the same channel, whereas for the triangular anode, the sum of Ealign per pair is aligned to the same channel. 
* Since the Ematch is calculated in Mapped2Cal, the calculation of the trimEnergyAlignGains take as input SofTrimCalData.
* Then, it is quite simple to get the raw energy loss per section : Eraw_sum = Sum_i {Ealign_i}

In case of rectangular anode shape :
trimEnergyAlignGains: Float_t \
  val_S1_A1 val_S1_A2 val_S1_A3 val_S1_A4 val_S1_A5 val_S1_A6  \
  val_S2_A1 val_S2_A2 val_S2_A3 val_S2_A4 val_S2_A5 val_S2_A6  \
  val_S3_A1 val_S3_A2 val_S3_A3 val_S3_A4 val_S3_A5 val_S3_A6

In case of triangular anode shape :
trimEnergyAlignGains: Float_t \
  val_S1_P1  val_S1_P2  val_S1_P3   \
  val_S2_P1  val_S2_P2  val_S2_P3  \
  val_S3_P1  val_S3_P2  val_S3_P3 

### Step 2: correction from beta: Ecorr_beta

### Step 3: correction from the DT: Ecorr_dt

### Step 4: correction from the theta angle: Ecorr_theta

### Step 5: conversion from energy loss to Z
