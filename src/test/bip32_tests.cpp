// SPDX-FileCopyrightText: © 2020 Alias Developers
// SPDX-FileCopyrightText: © 2016 SpectreCoin Developers
// SPDX-FileCopyrightText: © 2013 Bitcoin Developers
//
// SPDX-License-Identifier: MIT

#include <boost/test/unit_test.hpp>

#include "base58.h"
#include "key.h"
#include "uint256.h"
#include "util.h"

#include <string>
#include <vector>

struct TestDerivation {
    std::string pub;
    std::string prv;
    unsigned int nChild;
};

struct TestVector {
    std::string strHexMaster;
    std::vector<TestDerivation> vDerive;

    TestVector(std::string strHexMasterIn) : strHexMaster(strHexMasterIn) {}

    TestVector& operator()(std::string pub, std::string prv, unsigned int nChild) {
        vDerive.push_back(TestDerivation());
        TestDerivation &der = vDerive.back();
        der.pub = pub;
        der.prv = prv;
        der.nChild = nChild;
        return *this;
    }
};

TestVector test1 =
  TestVector("000102030405060708090a0b0c0d0e0f")
    ("AQfxPcJcoZDPtpVGjjdUdXXr1dHNwripBSqFVWjovbCYMLY8fW8vfY9YTfwE6Z4aMqX5PH3pESWTNFtS5BvAaEa1XxbvsDwHLg8759PmhmdUPYtR",
     "AQhQmof7ALfFfPQWxY9USpAk3s76VuBwEoNm7XzmgM1693xyDGP5XhbWrgJ3AzA1X6Ly29VhqZNZqpgWjJa6rHUewfaHCzLAPb5eGtEU3ErcC3tT",
     0x80000000)
    ("AQfxPcLtDYvhMukPitEcLjHieWKV4BYNXpujWCMEpZHvUJjKwsRDqxP5Yo5aWLL94apyvHG6oJjznzXWAAK8iBbjv2VPbRa5bbS1HfKuG5FUggLY",
     "AQhQmohNaLNZ8UfdwgkcA1vcgk9CcE1VbBTF8DcCaK6UG2AAVdfNi7q3woSPamRMrW4LyrYbXvftiD3Mc2L8Zw4w8TeMj4CqXq3MAbF6tg4MABvp",
     1)
    ("AQfxPcP4LkiFFJTGnigenTYcX74NiEvMdpwTo8fuf9ZhHFN8A6xiygBVrkeUjkQbdzmhGEyir6ASu77zdkcsXevuoi9DXimvT551ZJ4S3NCRqRow",
     "AQhQmojYhYA71sNX1XCebkBWZLt6GHPUhBUyR9vsQuNF4xnxhsCsqqdUFm1HpBUYv6z82JdMDjVV9HkK2AGeo4Qj7EorAZyoFto82G3RGCz98fHe",
     0x80000002)
    ("AQfxPcRfcoF571L8CarSsi91tU4QrU5EA6V3nUD9LPtzSREVbWov6UwLHf62fJRnUSe8abVpahYRWV4ZcCx1pN2TLmbNNJftbA25F1kMBTUy3YoV",
     "AQhQmon9yagvsaFNRPNSgzmuvht8QWYMDT2ZQVU769hYE8fL9H44xePJgfSqjjWmY5DdWp6NcKxS4iA8vPUPYeReJMuHpvbMp3jMxsKAUqY7y8mT",
     2)
    ("AQfxPcTu1dgC4BooBfKvbQQDyMZaAYtWtj5Myuh2derWujg4CsuYTYi2erfVaykmrrSHK4owkqYuTXoTimNMpTKtYVBqdDTSmzCy52SG2c5UwQ3k",
     "AQhQmopPNR83pkj3QTqvQh381bPHibMdx5csbvwzPQf4hT6tke9hKiA13s2JfQqAzXt2wstWJwsfdcyb5rfHzqrBeZHssKqSubpBg2HdeM6kRgCM",
     1000000000)
    ("AQfxPcVcn7MoJJwBPBfi3WwnhfGaAM86CYaB1hwRBu8u3wkShb2wr58HK7meecagohhN2YWEJLtqG51AKMnkqZBPPLSzk5zrrngYhqPswrFReZm9",
     "AQhQmor78tof4srRbzBhroagju6HiPbDFu7gdjCNwewSqfBHFMH6iEaFi88Tj3gxCSoA2VHuZjEeaU28CrqJNNvCNgFXQfLD8iiwPJdC16D5LN3c",
     0);

TestVector test2 =
  TestVector("fffcf9f6f3f0edeae7e4e1dedbd8d5d2cfccc9c6c3c0bdbab7b4b1aeaba8a5a29f9c999693908d8a8784817e7b7875726f6c696663605d5a5754514e4b484542")
    ("AQfxPcJcoZDPtpVGjjEyxdgvJ5V6i6uxc8TWKCKbet9HBJvFXo7wriJKQ6qPguAfLYmSMBc42MB7XexwKuhtoMkvh6Wvp5sUoWcbjTmSp3W6Mcoc",
     "AQhQmof7ALfFfPQWxXkymvKpLKJpG9P5fV11wDaZQdwpy2M65ZN6iskHo7CCmLDnaGrX7PYTGJf2J6nYvQ6pxKu3eGwbySkEdjRZ1TV6GboLgNuR",
     0)
    ("AQfxPcMtYpw5rJW6pFWaFmUJB9HWETshrqRzbkVM23xJdPL7VV6PqHZfUFfxwUddELrSnPvYCDQ7H43nJ4sKVEud7S7b6MvDUQxk7bFZUUhR886v",
     "AQhQmoiNucNwcsRM342a547CDP7DnWLpvByWDmkJmokrR6kx3FLYhT1dsG2n1uj4NuvMwoCh6CfS2Xwy3kcTmtBYhDAmNSveh7uikatzpy6hVmum",
     0xFFFFFFFF)
    ("AQfxPcP3c5Y7NgQGvRNnSBvHpcdBnrcvmNvAHqeRYyMDQ34sswPdHuT5nTjkY61sM6b3tBSTjsExtMGCtjYBq1p7EAG3itzqXmh18Tdq5KZY1GhT",
     "AQhQmojXxryy9FKX9DtnFUZBrrSuLu63pjTfuruPJj9mBkViRhdnA4u4BU6ZcX5YMeB6RhS1yygdfaxUABrCYcZ9pnUCvset2tRT1NgLMjWQj1Ld",
     1)
    ("AQfxPcRraVZ6Jr3a7WzTaFaSPaa82C9oGV75ZA5vQpQsjLxEKcDVTCxcVMe8uvp4UtTkAmcvHht47zPWipLSqviYBRbWdVH3NzESLPQW2a35iTFY",
     "AQhQmonLwGzx5QxpLKWTPYDLRpPqaEcvKqebBBLtAaDRX4P4sNTeKNQatMzwzMsjyftJ81PNgiyi3zW9NWYyhfSgCnx2E1RQqjXRPrUm6DDWZGzi",
     0xFFFFFFFE)
    ("AQfxPcT2cQX2fTSXpowhAz4Fvx5pD6tGdY5i4BneJM8a9BiVa4ZpPJdYVtAW1ZUBz5qGcj4ZvWP7q2zqaTycEcbeh8xD5mBMsYXXkFdac6tu8TKo",
     "AQhQmooWyBxtS2Mn3cTgzGh9yBuXm9MPgtdDgD3c46w7vu9L7poyFU5WttXK5zaUNJJgQYxfP2oV3NsixKg55BPahUnhPqMx9sjT2C4mcGv5J11S",
     2)
    ("AQfxPcUPeMxFAycr4ZCt4i8PcvqCBVwojnQd8qxGBkuvdpWMA8m9xUGUG4jT1dsGGDQdKF7n9nJdEvhrZxQfjShLbTD4fQMBxUtpUvvmnmYVuC2k",
     "AQhQmopt19Q6wYY6HMisszmHfAeujYQvo8x8ksDDwWiURXwBhu1JpdiSf56G64z9NTKmUbGX7KZ7rpyZeNDxauhwvXmS7eDTew9ajtS7AMYjxyLn",
     0);

void RunTest(const TestVector &test) {
    std::vector<unsigned char> seed = ParseHex(test.strHexMaster);
    CExtKey key;
    CExtPubKey pubkey;
    key.SetMaster(&seed[0], seed.size());
    pubkey = key.Neutered();
    BOOST_FOREACH(const TestDerivation &derive, test.vDerive) {
        unsigned char data[74];
        key.Encode(data);
        pubkey.Encode(data);
        // Test private key
        CBitcoinExtKey b58key;
        b58key.SetKey(key);
        BOOST_CHECK(b58key.ToString() == derive.prv);
        // Test public key
        CBitcoinExtPubKey b58pubkey;
        b58pubkey.SetKey(pubkey);
        BOOST_CHECK(b58pubkey.ToString() == derive.pub);
        // Derive new keys
        CExtKey keyNew;
        BOOST_CHECK(key.Derive(keyNew, derive.nChild));
        CExtPubKey pubkeyNew = keyNew.Neutered();
        if (!(derive.nChild & 0x80000000)) {
            // Compare with public derivation
            CExtPubKey pubkeyNew2;
            BOOST_CHECK(pubkey.Derive(pubkeyNew2, derive.nChild));
            BOOST_CHECK(pubkeyNew == pubkeyNew2);
        }
        key = keyNew;
        pubkey = pubkeyNew;
    }
}


BOOST_AUTO_TEST_SUITE(bip32_tests)

BOOST_AUTO_TEST_CASE(bip32_test1) {
    RunTest(test1);
}

BOOST_AUTO_TEST_CASE(bip32_test2) {
    RunTest(test2);
}

BOOST_AUTO_TEST_SUITE_END()
