// SPDX-FileCopyrightText: © 2020 Alias Developers
// SPDX-FileCopyrightText: © 2016 SpectreCoin Developers
// SPDX-FileCopyrightText: © 2014 ShadowCoin Developers
//
// SPDX-License-Identifier: MIT

#include <boost/test/unit_test.hpp>

#include "base58.h"
#include "key.h"
#include "extkey.h"
#include "uint256.h"
#include "util.h"
#include "chainparams.h"
#include "state.h"
#include "serialize.h"


#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <inttypes.h>
#include <initializer_list>


#include <boost/assign/list_of.hpp>

// test_spectre --log_level=all  --run_test=bip32_tests,extkey_tests

namespace ba = boost::assign;

class FailTest
{
public:
    FailTest(std::string _sTest, int _rv) : sTest(_sTest), rv(_rv) {};
    std::string sTest;
    int rv;
};

FailTest failTests[] = {
    FailTest("", 3),
    FailTest("  ", 4),
    FailTest("abcd", 4),
    FailTest("M/3h/1111111111111111111111", 5),
    FailTest("0/8/0/0", 6),
    FailTest("/1/1", 7),
    FailTest("0/1/1/", 7),
    FailTest("0/1//1", 7),
    FailTest("m/2147483648h", 8),
    FailTest("m/4294967296", 5),
    FailTest("m/4294967297", 5),
    FailTest("0b0012", 4),
    FailTest("0x3Dg", 4),
    FailTest("m/4294967296", 5),
};

class PassTest
{
public:
    PassTest(std::string _sTest, int _rv, std::initializer_list<uint32_t> expect) : sTest(_sTest), rv(_rv), vExpect(expect.begin(), expect.end()) {};
    std::string sTest;
    int rv;
    std::vector<uint32_t> vExpect;
};

PassTest passTests[] = {
    PassTest("m", 0, { 0 }),
    PassTest("0", 0, { 0 }),
    PassTest("1", 0, { 1 }),
    PassTest("0/1", 0, { 0, 1 }),
    PassTest("1/0", 0, { 1, 0 }),
    PassTest("M/3", 0, { 0, 3 }),
    PassTest("m/0h", 0, { 0, 2147483648 }),
    PassTest("m/1H", 0, { 0, 2147483649 }),
    PassTest("m/2'", 0, { 0, 2147483650 }),
    PassTest("m/4294967295", 0, { 0, 4294967295 }),
    PassTest("m/4/0b001/0xFe/3/0b010/0b010h", 0, { 0, 4, 1, 254, 3, 2, 2147483650 }),
    PassTest("m/2147483647h", 0, { 0, 4294967295 }),
    PassTest("0800/0xFh", 0, { 800, 2147483663 }),

};


void RunPathTest()
{
    int rv;

    std::vector<uint32_t> vPath;
    std::vector<uint32_t> vExpect;
    std::string sTest;

    int al = sizeof(failTests)/sizeof(FailTest);

    BOOST_TEST_MESSAGE("Running " << al << " tests expected to fail.");

    for (int i = 0; i < al; ++i)
    {
        FailTest &ft = failTests[i];
        BOOST_TEST_MESSAGE("Fail test " << i << ", path '"  << ft.sTest << "', expect return " << ft.rv);
        rv = ExtractExtKeyPath(ft.sTest, vPath);
        BOOST_CHECK(rv == ft.rv);
        //BOOST_CHECK_MESSAGE(rv != ft.rv, "rv   " << rv);
    };

    char tooMuchData[513];
    memset(tooMuchData, '/', 512);
    tooMuchData[512] = '\0';
    sTest = std::string(tooMuchData);
    BOOST_TEST_MESSAGE("Testing Path 'tooMuchData'");
    rv = ExtractExtKeyPath(sTest, vPath);
    BOOST_CHECK(rv == 2);


    al = sizeof(passTests)/sizeof(PassTest);

    BOOST_TEST_MESSAGE("Running " << al << " tests expected to pass.");

    std::stringstream ss, ssE;
    for (int i = 0; i < al; ++i)
    {
        PassTest &pt = passTests[i];
        BOOST_TEST_MESSAGE("Pass test " << i << ", path '" << pt.sTest << "', expect return " << pt.rv);
        rv = ExtractExtKeyPath(pt.sTest, vPath);

        ss.str("");
        for (std::vector<uint32_t>::iterator it = vPath.begin(); it != vPath.end(); ++it)
        {
            ss << *it;
            if (it != vPath.end()-1)
                ss << ", ";
        };
        BOOST_TEST_MESSAGE("vPath   " << ss.str());

        ssE.str("");
        for (std::vector<uint32_t>::iterator it = pt.vExpect.begin(); it != pt.vExpect.end(); ++it)
        {
            ssE << *it;
            if (it != pt.vExpect.end()-1)
                ssE << ", ";
        };
        BOOST_TEST_MESSAGE("vExpect " << ssE.str());

        BOOST_CHECK(rv == pt.rv);
        BOOST_CHECK(vPath == pt.vExpect);
    };

    //for (std::vector<uint32_t>::iterator it = vPath.begin(); it != vPath.end(); ++it)
    //    BOOST_TEST_MESSAGE("vPath " << *it);
}


class DeriveTestData
{
public:
    DeriveTestData(uint32_t _nDerives, std::string _vKey58, std::string _pKey58) : nDerives(_nDerives), vKey58(_vKey58), pKey58(_pKey58) { };

    uint32_t nDerives;
    // private key
    std::string vKey58;
    // public key
    std::string pKey58;
};

void RunDeriveTest(std::vector<DeriveTestData> &vData)
{
    int rv;
    CBitcoinExtKey extKey58;
    CExtKey evkeyM;
    CExtPubKey epkeyM;

    for (uint32_t k = 0; k < vData.size(); ++k)
    {
        DeriveTestData &dt = vData[k];

        if (dt.nDerives == 0)
        {
            // - set master

            BOOST_CHECK(0 == (rv = extKey58.Set58(dt.vKey58.c_str())));
            BOOST_CHECK(0 == (rv += abs(strcmp(extKey58.ToString().c_str(), dt.vKey58.c_str()))));

            evkeyM = extKey58.GetKey();
            BOOST_CHECK(0 == (rv += abs(strcmp(CBitcoinExtKey(evkeyM).ToString().c_str(), dt.vKey58.c_str()))));
            epkeyM = evkeyM.Neutered();

            BOOST_CHECK(0 == (rv += abs(strcmp(CBitcoinExtPubKey(epkeyM).ToString().c_str(), dt.pKey58.c_str()))));

            BOOST_CHECK(CBitcoinExtPubKey(epkeyM).ToString().c_str());

            if (rv != 0)
            {
                BOOST_TEST_MESSAGE("Set master failed, aborting test.");
                break;
            }
            continue;
        };


        CExtKey evkey[2], evkeyOut;
        CExtPubKey epkeyOut;
        evkey[0] = evkeyM;
        rv = 0;
        for (uint32_t d = 0; d < dt.nDerives; ++d)
        {
            rv += evkey[d % 2].Derive(evkey[(d+1) % 2], 1);
        }
        BOOST_CHECK(dt.nDerives == (uint32_t)rv);
        evkeyOut = evkey[dt.nDerives % 2];

        BOOST_CHECK(CBitcoinExtKey(evkeyOut).ToString().c_str());
        BOOST_TEST_MESSAGE("evkeyOut.nDepth " << (int)evkeyOut.nDepth);
        BOOST_CHECK(evkeyOut.nDepth == dt.nDerives % 256);

        BOOST_CHECK(0 == strcmp(CBitcoinExtKey(evkeyOut).ToString().c_str(), dt.vKey58.c_str()));

        epkeyOut = evkeyOut.Neutered();
        BOOST_CHECK(0 == strcmp(CBitcoinExtPubKey(epkeyOut).ToString().c_str(), dt.pKey58.c_str()));
    };

}

void RunDeriveTests()
{
    /*
        Must be able to derive deeper than the arbitrary nDepth field (255)
    */

    std::vector<DeriveTestData> vMainNetPairs = {
        DeriveTestData(0,
            std::string("AQhQmof7ALfFfPQWxY9USpAk3s76VuBwEoNm7XzmgM1693xyDGP5XhbWrgJ3AzA1X6Ly29VhqZNZqpgWjJa6rHUewfaHCzLAPb5eGtEU3ErcC3tT"),
            std::string("AQfxPcJcoZDPtpVGjjdUdXXr1dHNwripBSqFVWjovbCYMLY8fW8vfY9YTfwE6Z4aMqX5PH3pESWTNFtS5BvAaEa1XxbvsDwHLg8759PmhmdUPYtR")),
        DeriveTestData(1,
            std::string("AQhQmohNaLNYz916yZiV4CjdSqaXFFSNeTmWCRvW4ym6gAfk2rvpSAryT9ixvo7WG4ErhiTA4gURuZdT3XRD4edjoiqTff5Yv1kTU4Hx8eGVRsq5"),
            std::string("AQfxPcLtDYvhDa5rkmCVEv6jQbkohCyFb7DzaQfYKDxYtTEuV6gfa1R149N9rN3kdr47Xp1YkogL1FHqEmpeTsYzeizCqyD6ugFTeehgPD7wbes8")),
        DeriveTestData(350,
            std::string("AQhQmrj2CNpBMFMDXwudziLZvNWKLDnug4rumnAJJA6qf7dX2DrmjhV7aakf99GGtmGNMPa7XFwHGNWTms87vHAJ7z1oWWh9eY2hJ3BvuNEZTSi6"),
            std::string("AQfxPfNXqbNKagRyK9PeBRhft8gbnBKnciKQ9kuLYQJHsQCgUTccsY39BaPr4i9QD3sTstufRnavUqPAqKzQTm6NMNJpjCXdnPSQvTpBN1LFijY4")),
    };

    std::vector<DeriveTestData> vTestNetPairs = {
        DeriveTestData(0,
            std::string("SDCVTZWtnQrSZ1LBHTik3cZFUn4ynqci2nFQXde9GnJmCQ3XeYUKrdyV7ZrJdY6Wevp4UdNxvca5RE5ZP3pVCUdZixz1xHReX38X3takPc1MDVnJ"),
            std::string("SDCPTTad968GGU17vaZudPBFmbTbyA2TV21R4HfvogQ4UQZWPS5pTCzoEYjdu219WULUqhQuq4jrSm35ofoTZ5E589Kzx1TiXFxANyfBa2oHXDzA")),
        DeriveTestData(1,
            std::string("SDCVTZZACQZjskvmJVHkf188skYQYBs9SSe9cXZsfR4mjWkJU924m7Ewi3HEPM41PthxACLR9jfwUy2VhGfbQqneb2FCQxB33ToLF4eEV1Q3VAzb"),
            std::string("SDCPTTctZ5qZbDbhwc8vEmk9AZw2iWGttgQA9BbfCKA51XGHD2dZMgGFq2AZepzKnUsWzENeMRuj5kSUyFhwSiD4EuiGvkjY6G5WxUy6FUCM5fSJ")),

        DeriveTestData(350,
            std::string("SDCVTcaopT1NEsGsrsUubWj5MHUCdADgU3jZBsoftbQWiTi5TVx24ds5qUJvbhCn2bjTosTNcK8nqmuWRcNWGUKCuHRYFondmz5a53YDFjMMqopV"),
            std::string("SDCPTWeYB8HBxKwpVzL5BHM5e6rpoUdRvHVZiXqTRVVozUE4CPZWfCtPxTCFsB5yMggsLKGm2QpKZLXpZoshSbkRwZ2toz44xyGUEJ5bEGXc5jDr")),
    };
    CBitcoinExtKey extKey58;

    // - valid string
    BOOST_CHECK(0 == extKey58.Set58(vMainNetPairs[0].vKey58.c_str()));
    BOOST_CHECK(strcmp(extKey58.ToString().c_str(), vMainNetPairs[0].vKey58.c_str()) == 0);
    // - invalid string
    BOOST_CHECK(0 != extKey58.Set58(vMainNetPairs[0].vKey58.c_str()+3));

    RunDeriveTest(vMainNetPairs);


    // - fail testnet key on main
    BOOST_CHECK(0 != extKey58.Set58(vTestNetPairs[0].vKey58.c_str()));






    // - switch to testnet
    BOOST_TEST_MESSAGE("Entering Testnet");
    fTestNet = true;
    SelectParams(CChainParams::TESTNET);

    // - pass testnet key on testnet
    BOOST_CHECK(0 == extKey58.Set58(vTestNetPairs[0].vKey58.c_str()));
    BOOST_CHECK(strcmp(extKey58.ToString().c_str(), vTestNetPairs[0].vKey58.c_str()) == 0);

    RunDeriveTest(vTestNetPairs);


    // -return to mainnet
    fTestNet = false;
    SelectParams(CChainParams::MAIN);
}


void RunSerialiseTests()
{
    int64_t nTest;
    int64_t nTest0      = 0l;
    int64_t nTest4      = 1432035740l;
    int64_t nTest4_1    = 2189410940l; // 2039
    int64_t nTest5      = 4294967298l; // 2106
    int64_t nTest8      = -3l;

    BOOST_CHECK(0 == GetNumBytesReqForInt(nTest0));
    BOOST_CHECK(4 == GetNumBytesReqForInt(nTest4));
    BOOST_CHECK(4 == GetNumBytesReqForInt(nTest4_1)); // expect 4, no sign bit
    BOOST_CHECK(5 == GetNumBytesReqForInt(nTest5));
    BOOST_CHECK(8 == GetNumBytesReqForInt(nTest8));

    //BOOST_TEST_MESSAGE(GetNumBytesReqForInt(nTest5));

    std::vector<uint8_t> v;
    SetCompressedInt64(v, nTest0);
    GetCompressedInt64(v, (uint64_t&)nTest);
    BOOST_CHECK(nTest0 == nTest);

    SetCompressedInt64(v, nTest5);
    GetCompressedInt64(v, (uint64_t&)nTest);
    BOOST_CHECK(nTest5 == nTest);

    SetCompressedInt64(v, nTest8);
    GetCompressedInt64(v, (uint64_t&)nTest);
    BOOST_CHECK(nTest8 == nTest);


    CStoredExtKey sk, sk_;
    CStoredExtKey skInvalid, skInvalid_;

    CExtKey58 eKey58;
    BOOST_CHECK(0 == eKey58.Set58("AQhQmof7ALfFfPQWxY9USpAk3s76VuBwEoNm7XzmgM1693xyDGP5XhbWrgJ3AzA1X6Ly29VhqZNZqpgWjJa6rHUewfaHCzLAPb5eGtEU3ErcC3tT"));

    sk.kp = eKey58.GetKey();
    sk.sLabel = "sk label";
    sk.nGenerated = 5;
    sk.nHGenerated = 6;
    sk.mapValue[EKVT_CREATED_AT] = SetCompressedInt64(v, nTest8);

    eKey58.SetKey(sk.kp, CChainParams::EXT_PUBLIC_KEY);
    BOOST_CHECK(eKey58.ToString() == "AQfxPcJcoZDPtpVGjjdUdXXr1dHNwripBSqFVWjovbCYMLY8fW8vfY9YTfwE6Z4aMqX5PH3pESWTNFtS5BvAaEa1XxbvsDwHLg8759PmhmdUPYtR");

    eKey58.SetKeyV(sk.kp);
    BOOST_CHECK(eKey58.ToString() == "AQhQmof7ALfFfPQWxY9USpAk3s76VuBwEoNm7XzmgM1693xyDGP5XhbWrgJ3AzA1X6Ly29VhqZNZqpgWjJa6rHUewfaHCzLAPb5eGtEU3ErcC3tT");

    CDataStream ss(SER_NETWORK, PROTOCOL_VERSION);
    ss << sk << skInvalid;

    ss >> sk_;
    ss >> skInvalid_;

    BOOST_CHECK(sk.kp == sk_.kp);
    BOOST_CHECK(1 == sk_.kp.IsValidV());
    BOOST_CHECK(1 == sk_.kp.IsValidP());
    BOOST_CHECK(sk.sLabel == sk_.sLabel);
    BOOST_CHECK(sk.nGenerated == sk_.nGenerated);
    BOOST_CHECK(sk.nHGenerated == sk_.nHGenerated);
    BOOST_CHECK(nTest8 == GetCompressedInt64(sk_.mapValue[EKVT_CREATED_AT], (uint64_t&)nTest));

    BOOST_CHECK(0 == skInvalid.kp.IsValidV());
    BOOST_CHECK(0 == skInvalid.kp.IsValidP());



    // path

    std::vector<uint8_t> vPath;

    PushUInt32(vPath, 1);
    PushUInt32(vPath, 3);
    PushUInt32(vPath, 2);
    PushUInt32(vPath, 4294967295);

    std::string sPath;
    BOOST_CHECK(0 == PathToString(vPath, sPath, 'h'));
    BOOST_CHECK(sPath == "m/1/3/2/2147483647h");

    vPath.resize(0);
    PushUInt32(vPath, 1);
    PushUInt32(vPath, 4294967294);
    PushUInt32(vPath, 30);
    BOOST_CHECK(0 == PathToString(vPath, sPath));
    BOOST_CHECK(sPath == "m/1/2147483646'/30");


    // id
    CBitcoinAddress addr;
    CKeyID id = sk.GetID();
    CKeyID idTest;


    BOOST_CHECK(true == addr.Set(id, CChainParams::EXT_KEY_HASH)
        && addr.IsValid(CChainParams::EXT_KEY_HASH)
        && addr.GetKeyID(idTest, CChainParams::EXT_KEY_HASH));

    BOOST_CHECK(id == idTest);
    BOOST_CHECK_MESSAGE(addr.ToString() == "x9S4Xj1DZwFsdFno1uHknNNGqdMWgXdhX6", addr.ToString());


    // - test DeriveNextKey

    CExtKey ev;
    CExtPubKey ep;
    uint32_t nChild=0;

    sk.nGenerated = 0;
    sk.nHGenerated = 0;
    BOOST_CHECK(0 == sk.DeriveNextKey(ev, nChild));
    BOOST_CHECK_MESSAGE(1 == sk.nGenerated, "nGenerated " << sk.nGenerated);
    sk.nGenerated = 0;
    BOOST_CHECK(0 == sk.DeriveNextKey(ep, nChild));

    BOOST_CHECK(ep.pubkey == ev.key.GetPubKey());


    id = ev.key.GetPubKey().GetID();
    addr.Set(id, CChainParams::EXT_KEY_HASH);
    BOOST_CHECK_MESSAGE(addr.ToString() == "xS8vxP6PVm3ycqm4NqvUkhiDWqeBhhekzn", addr.ToString());

    sk.nGenerated = 1;
    BOOST_CHECK(0 == sk.DeriveNextKey(ev, nChild));
    id = ev.key.GetPubKey().GetID();
    addr.Set(id, CChainParams::EXT_KEY_HASH);
    BOOST_CHECK_MESSAGE(addr.ToString() == "xRfAtU1u43VJBTt4agxpnPvXjf28hSmwrL", addr.ToString());

    sk.nHGenerated = 0;
    BOOST_CHECK(0 == sk.DeriveNextKey(ev, nChild, true));
    id = ev.key.GetPubKey().GetID();
    addr.Set(id, CChainParams::EXT_KEY_HASH);
    BOOST_CHECK_MESSAGE(addr.ToString() == "xEcqVH3fRnNZabMHVctAeScJY9ySkg6BSF", addr.ToString());
    BOOST_CHECK_MESSAGE(1 == sk.nHGenerated, "nHGenerated " << sk.nHGenerated);

    sk.nHGenerated = 1;
    BOOST_CHECK(0 == sk.DeriveNextKey(ev, nChild, true));
    id = ev.key.GetPubKey().GetID();
    addr.Set(id, CChainParams::EXT_KEY_HASH);
    BOOST_CHECK_MESSAGE(addr.ToString() == "xRfBk2tuann5qTVKyW2HmA9CJZBAE1sRvJ", addr.ToString());
    BOOST_CHECK_MESSAGE(2 == sk.nHGenerated, "nHGenerated " << sk.nHGenerated);

    sk.nHGenerated = 1;
    BOOST_CHECK(0 == sk.DeriveNextKey(ep, nChild, true));
    id = ev.key.GetPubKey().GetID();
    addr.Set(id, CChainParams::EXT_KEY_HASH);
    BOOST_CHECK_MESSAGE(addr.ToString() == "xRfBk2tuann5qTVKyW2HmA9CJZBAE1sRvJ", addr.ToString());
    BOOST_CHECK(ep.pubkey == ev.key.GetPubKey());



    CStoredExtKey skp = sk;
    skp.kp = skp.kp.Neutered();

    CKey k;

    sk.nGenerated = 1;
    BOOST_CHECK(0 == sk.DeriveNextKey(k, nChild, false));
    BOOST_CHECK_MESSAGE(nChild == 1, "nChild " << nChild);
    BOOST_CHECK_MESSAGE(HexStr(k.GetPubKey()) == "0245a12d2ce075d947b6232b3e424ffa5d2208b6ff69800a1f2501ac6392499bf8", "HexStr(k.GetPubKey()) " << HexStr(k.GetPubKey()));


    sk.nGenerated = 2;
    BOOST_CHECK(0 == sk.DeriveNextKey(k, nChild, false));
    BOOST_CHECK_MESSAGE(nChild == 2, "nChild " << nChild);
    BOOST_CHECK_MESSAGE(HexStr(k.GetPubKey()) == "02f430d7efc4d1ecbac888fb49446ec0b13ec4196512be93054a9b5b30df238910", "HexStr(k.GetPubKey()) " << HexStr(k.GetPubKey()));

    sk.nHGenerated = 2;
    BOOST_CHECK(0 == sk.DeriveNextKey(k, nChild, true));
    BOOST_CHECK_MESSAGE(nChild == 2147483650, "nChild " << nChild);
    BOOST_CHECK_MESSAGE(HexStr(k.GetPubKey()) == "0355825cbaf4365a2f7015d9c9bae4ecaf9b57a05e063237256f1565b20104c183", "HexStr(k.GetPubKey()) " << HexStr(k.GetPubKey()));

    // - can't derive keys from pubkeys
    skp.nGenerated = 1;
    BOOST_CHECK(1 == skp.DeriveNextKey(k, nChild, false));

    skp.nHGenerated = 1;
    BOOST_CHECK(1 == skp.DeriveNextKey(k, nChild, true));



    CPubKey pk;
    sk.nGenerated = 1;
    BOOST_CHECK(0 == sk.DeriveNextKey(pk, nChild, false));
    BOOST_CHECK_MESSAGE(nChild == 1, "nChild " << nChild);
    BOOST_CHECK_MESSAGE(HexStr(pk) == "0245a12d2ce075d947b6232b3e424ffa5d2208b6ff69800a1f2501ac6392499bf8", "HexStr(pk) " << HexStr(pk));

    sk.nHGenerated = 2;
    BOOST_CHECK(0 == sk.DeriveNextKey(pk, nChild, true));
    BOOST_CHECK_MESSAGE(nChild == 2147483650, "nChild " << nChild);
    BOOST_CHECK_MESSAGE(HexStr(pk) == "0355825cbaf4365a2f7015d9c9bae4ecaf9b57a05e063237256f1565b20104c183", "HexStr(pk) " << HexStr(pk));

    skp.nGenerated = 2;
    BOOST_CHECK(0 == skp.DeriveNextKey(pk, nChild, false));
    BOOST_CHECK_MESSAGE(nChild == 2, "nChild " << nChild);
    BOOST_CHECK_MESSAGE(HexStr(pk) == "02f430d7efc4d1ecbac888fb49446ec0b13ec4196512be93054a9b5b30df238910", "HexStr(pk) " << HexStr(pk));

    // - can't derive hardened pubkeys from pubkeys
    skp.nHGenerated = 1;
    BOOST_CHECK(1 == skp.DeriveNextKey(pk, nChild, true));


    // - CBitcoinAddress tests
    // CBitcoinAddress always deals in public keys - should never expose a secret in an address

    CExtKeyPair kp, kpT;
    CTxDestination dest;

    BOOST_CHECK(0 == eKey58.Set58("specpmphCJNSUos9rNqn6FNi3ztvMW1wft1PVbifvBrwhm6JnhD9yk8rSNFTGfozGbmBsr8vZv9mGYSTfmEMpbfTTMb8TQfj7JRABmvBFKgA2xG8J"));
    kp = eKey58.GetKey();
    CBitcoinAddress addrB(kp);
    BOOST_CHECK(addrB.IsValid() == true);

    BOOST_CHECK(addr.Set(kp) == true);
    BOOST_CHECK(addr.IsValid() == true);
    BOOST_CHECK(addr.IsValid(CChainParams::EXT_SECRET_KEY) == false);
    BOOST_CHECK(addr.IsValid(CChainParams::EXT_PUBLIC_KEY) == true);
    BOOST_CHECK(addr.ToString() == "specpmphCJNSUos9rNqn6FNi3ztvMW1wft1PVbifvBrwhm6JnhD9yk8rSNFTGfozGbmBsr8vZv9mGYSTfmEMpbfTTMb8TQfj7JRABmvBFKgA2xG8J");
    dest = addr.Get();
    BOOST_CHECK(dest.type() == typeid(CExtKeyPair));
    kpT = boost::get<CExtKeyPair>(dest);
    BOOST_CHECK(kpT == kp);


    // - switch to testnet
    BOOST_TEST_MESSAGE("Entering Testnet");
    fTestNet = true;
    SelectParams(CChainParams::TESTNET);

    id = sk.GetID();
    BOOST_CHECK(true == addr.Set(id, CChainParams::EXT_KEY_HASH)
        && addr.IsValid(CChainParams::EXT_KEY_HASH)
        && addr.GetKeyID(idTest, CChainParams::EXT_KEY_HASH));

    BOOST_CHECK(id == idTest);
    BOOST_CHECK_MESSAGE(addr.ToString() == "XCUfUzXMYkXYvP9RVtdzibVVpMP2bhfWRQ", addr.ToString());


    BOOST_CHECK(0 == eKey58.Set58("SPECPTTad968GGU17vZThYUhb4WKgaLQ22ffBVjTnZGGCAcU1vfVFcJEroz4QeZjFZLs5a1dkpZxsKfB2Adnun1axAGuzrfBweXWSxuXu2Wj3AaGp"));
    kp = eKey58.GetKey();
    CBitcoinAddress addrC("SPECVTZWtnQrSZ1LBHScXxi5amgw4Q1zGaRuAy5S12NAttbx3Bmsm1jDYh1B5P5qTPZaWpUZZ5mmubGTYjXPB1cQ9btJmhDoBLHZnwAGUBVH42gB3");
    BOOST_CHECK(addrC.IsValid() == true);
    BOOST_CHECK(addrC.IsValid(CChainParams::EXT_PUBLIC_KEY) == true);

    BOOST_CHECK(addr.Set(kp) == true);
    BOOST_CHECK(addr.IsValid() == true);
    BOOST_CHECK(addr.IsValid(CChainParams::EXT_SECRET_KEY) == false);
    BOOST_CHECK(addr.IsValid(CChainParams::EXT_PUBLIC_KEY) == true);
    BOOST_CHECK(addr.ToString() == "SPECPTTad968GGU17vZThYUhb4WKgaLQ22ffBVjTnZGGCAcU1vfVFcJEroz4QeZjFZLs5a1dkpZxsKfB2Adnun1axAGuzrfBweXWSxuXu2Wj3AaGp");
    dest = addr.Get();
    BOOST_CHECK(dest.type() == typeid(CExtKeyPair));
    kpT = boost::get<CExtKeyPair>(dest);
    BOOST_CHECK(kpT == kp);

    // -return to mainnet
    fTestNet = false;
    SelectParams(CChainParams::MAIN);

}

BOOST_AUTO_TEST_SUITE(extkey_tests)

BOOST_AUTO_TEST_CASE(extkey_path)
{
    RunPathTest();
}

BOOST_AUTO_TEST_CASE(extkey_derive)
{
    RunDeriveTests();
}

BOOST_AUTO_TEST_CASE(extkey_serialise)
{
    RunSerialiseTests();
}

BOOST_AUTO_TEST_SUITE_END()
