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

# The name of our project is “R3BSOF”.CMakeLists files in this project can
# refer to the R3BROOT root source directory of the project as ${R3BROOT_SOURCE_DIR},
# the SOFIA root source directory as ${R3BSOF_SOURCE_DIR}(for the sofia specifics) or
# as ${CMAKE_SOURCE_DIR} and to the root binary directory of the project as
# ${R3BSOF_BINARY_DIR} or ${CMAKE_BINARY_DIR}.

# Recurse into the given subdirectories.This does not actually cause
# another cmake executable to run.The same process will walk through
# the project's entire directory structure.

Set(R3BSOF_SOURCE_DIR ${R3BROOT_SOURCE_DIR}/sofia)
message(STATUS "${BBlue}SOFIA project was FOUND${CR} ")
message("    - ${Cyan}SOFIA_DIR${CR} = ${BGreen}${R3BSOF_SOURCE_DIR}${CR}")

# SOFIA specific
add_subdirectory(sofdata) 
if(WITH_UCESB) 
   add_subdirectory(sofsource) 
endif(WITH_UCESB) 
add_subdirectory(at)
add_subdirectory(tcal)
add_subdirectory(sci) 
add_subdirectory(tofwall) 
add_subdirectory(trim) 
add_subdirectory(sofonline)
add_subdirectory(sofana)
if(BUILD_GEOMETRY)
  add_subdirectory(geobase)
endif()
