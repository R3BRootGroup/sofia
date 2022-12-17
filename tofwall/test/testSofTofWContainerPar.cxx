/******************************************************************************
 *   Copyright (C) 2019 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2019 Members of R3B Collaboration                          *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#include "R3BSofTofWHitPar.h"

#include "gtest/gtest.h"
#include <map>

namespace {
TEST(testSofTofWHitPar, GetNumSci)
{
    R3BSofTofWHitPar par;
    par.SetNumSci(28);

    EXPECT_EQ(par.GetNumSci(), 28);
}

TEST(testSofTofWHitPar, GetInUse)
{
    R3BSofTofWHitPar par;
    par.SetInUse(1, 14);

    EXPECT_EQ(par.GetInUse(14), 1);
}

TEST(testSofTofWHitPar, GetTofPar)
{
    R3BSofTofWHitPar par;
    par.SetTofPar(33., 15);

    EXPECT_EQ(par.GetTofPar(15), 33.);
}
}   // namespace
