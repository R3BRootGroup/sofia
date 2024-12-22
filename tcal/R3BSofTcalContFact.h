/******************************************************************************
 *   Copyright (C) 2017 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2017-2025 Members of R3B Collaboration                     *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU Lesser General Public Licence (LGPL) version 3,        *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#pragma once

#include "FairContFact.h"

class FairParSet;

class R3BSofTcalContFact : public FairContFact
{
  public:
    R3BSofTcalContFact();
    ~R3BSofTcalContFact() {}

    FairParSet* createContainer(FairContainer* c);

  private:
    void setAllContainers();

    ClassDef(R3BSofTcalContFact, 1);
};
