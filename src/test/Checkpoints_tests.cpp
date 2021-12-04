// SPDX-FileCopyrightText: © 2020 Alias Developers
// SPDX-FileCopyrightText: © 2016 SpectreCoin Developers
//
// SPDX-License-Identifier: MIT

// Unit tests for block-chain checkpoints
//
#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>

#include "../checkpoints.h"
#include "../util.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(Checkpoints_tests)

BOOST_AUTO_TEST_CASE(sanity)
{
    uint256 p15000 = uint256("0xb54f2cc3bc31a0f7110ef5f95a1c00744793f5419b1d43044fe393a777edca39");
    uint256 p1245000 = uint256("0xa0cd983150296cb14d2e4006464a1b0814bd53bf76f35f00cf8d0530dd0f511e");
    BOOST_CHECK(Checkpoints::CheckHardened(15000, p15000));
    BOOST_CHECK(Checkpoints::CheckHardened(1245000, p1245000));

    
    // Wrong hashes at checkpoints should fail:
    BOOST_CHECK(!Checkpoints::CheckHardened(15000, p1245000));
    BOOST_CHECK(!Checkpoints::CheckHardened(1245000, p15000));

    // ... but any hash not at a checkpoint should succeed:
    BOOST_CHECK(Checkpoints::CheckHardened(15000+1, p1245000));
    BOOST_CHECK(Checkpoints::CheckHardened(1245000+1, p15000));

    BOOST_CHECK(Checkpoints::GetTotalBlocksEstimate() >= 1245000);
}    

BOOST_AUTO_TEST_SUITE_END()
