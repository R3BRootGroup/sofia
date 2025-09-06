##############################################################################
#   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    #
#   Copyright (C) 2019 University of Santiago de Compostela                  #
#   Copyright (C) 2019 CEA-DAM-DIF, Université Paris-Saclay                  #
#   Copyright (C) 2023 University of Coruña                                  #
#   Copyright (C) 2019-2025 Members of R3B Collaboration                     #
#                                                                            #
#             This software is distributed under the terms of the            #
#                 GNU General Public Licence (GPL) version 3,                #
#                    copied verbatim in the file "LICENSE".                  #
#                                                                            #
# In applying this license GSI does not waive the privileges and immunities  #
# granted to it by virtue of its status as an Intergovernmental Organization #
# or submit itself to any jurisdiction.                                      #
##############################################################################

#!/usr/bin/env bash
find . -type f \( -name "*.cpp" -o -name "*.cxx" -o -name "*.C" -o -name "*.h" \) \
    ! -path "./macros" ! -path "./sofia" ! -path "./frs" ! -path "./asyeos" ! -path "./glad-tpc" -print0 |
    xargs -0 -L 1 clang-format-16 -i

echo "Use git add -A ; git commit -m \"clang-format all files\" --author=\"white space <whitespace@example.com>\" to commit changes."
