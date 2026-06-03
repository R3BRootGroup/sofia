##############################################################################
#   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    #
#   Copyright (C) 2019 University of Santiago de Compostela                  #
#   Copyright (C) 2019 CEA-DAM-DIF, Université Paris-Saclay                  #
#   Copyright (C) 2023 University of Coruña                                  #
#   Copyright (C) 2019-2026 Members of R3B Collaboration                     #
#                                                                            #
#             This software is distributed under the terms of the            #
#                 GNU General Public Licence (GPL) version 3,                #
#                    copied verbatim in the file "LICENSE".                  #
#                                                                            #
# In applying this license GSI does not waive the privileges and immunities  #
# granted to it by virtue of its status as an Intergovernmental Organization #
# or submit itself to any jurisdiction.                                      #
##############################################################################

# Create root geometries

message(STATUS "Starting to generate the standard root geometries if needed")

set(R3BSOF_GEO false)

if(NOT EXISTS "${R3BROOT_SOURCE_DIR}/geometry/trim_v21.geo.root")
execute_process(COMMAND root -l ${R3BSOF_SOURCE_DIR}/geobase/create_trim_geo.C)
set(R3BSOF_GEO true)
endif(NOT EXISTS "${R3BROOT_SOURCE_DIR}/geometry/trim_v21.geo.root")

if(NOT EXISTS "${R3BROOT_SOURCE_DIR}/geometry/sof_at_v21.geo.root")
execute_process(COMMAND root -l ${R3BSOF_SOURCE_DIR}/geobase/create_at_geo.C)
set(R3BSOF_GEO true)
endif(NOT EXISTS "${R3BROOT_SOURCE_DIR}/geometry/sof_at_v21.geo.root")

if(NOT EXISTS "${R3BROOT_SOURCE_DIR}/geometry/sof_sci_v21.geo.root")
execute_process(COMMAND root -l ${R3BSOF_SOURCE_DIR}/geobase/create_sci_geo.C)
set(R3BSOF_GEO true)
endif(NOT EXISTS "${R3BROOT_SOURCE_DIR}/geometry/sof_sci_v21.geo.root")

if(NOT EXISTS "${R3BROOT_SOURCE_DIR}/geometry/target_area_s455_v21.geo.root")
execute_process(COMMAND root -l ${R3BSOF_SOURCE_DIR}/geobase/create_target_area_s455_geo.C)
set(R3BSOF_GEO true)
endif(NOT EXISTS "${R3BROOT_SOURCE_DIR}/geometry/target_area_s455_v21.geo.root")

if(R3BSOF_GEO)
message(STATUS "${BYellow}SOFIA standard root geometries were created${CR} ")
else(R3BSOF_GEO)
message(STATUS "${BYellow}SOFIA standard root geometries already created${CR} ")
endif(R3BSOF_GEO)
