// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017-2018 The Fac developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

        using namespace std;
        using namespace boost::assign;

        struct SeedSpec6 {
            uint8_t addr[16];
            uint16_t port;
        };

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
        static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
        {
            // It'll only connect to one or two seed nodes because once it connects,
            // it'll get a pile of addresses with newer timestamps.
            // Seed nodes are given a random 'last seen time' of between one and two
            // weeks ago.
            const int64_t nOneWeek = 7 * 24 * 60 * 60;
            for (unsigned int i = 0; i < count; i++) {
                struct in6_addr ip;
                memcpy(&ip, data[i].addr, sizeof(ip));
                CAddress addr(CService(ip, data[i].port));
                addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
                vSeedsOut.push_back(addr);
            }
        }
//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions

        static Checkpoints::MapCheckpoints mapCheckpoints =
                boost::assign::map_list_of
                        (0, uint256("0x000003fea1b6d9cb0e4267479d97eab7a80031803bf9d42298f3581da13edd0b"))
        ;

        static const Checkpoints::CCheckpointData data = {
                &mapCheckpoints,
                1535263200,// * UNIX timestamp of last checkpoint block
                0,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
                0        // * estimated number of transactions per day after checkpoint
        };

        static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
                boost::assign::map_list_of(0, uint256("0x000009e01bfc09d316bc12ea9e81d5ec57b74d7fb36f97981739f61fa763926e"));
        static const Checkpoints::CCheckpointData dataTestnet = {
                &mapCheckpointsTestnet,
                1535263600,
                0,
                0};
        static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
                boost::assign::map_list_of(0, uint256("0x433d7062fd717e5f38e9d8865fe09b9fb885e0b29d6c2adb708f55427d500af8"));
        static const Checkpoints::CCheckpointData dataRegtest = {
                &mapCheckpointsRegtest,
                1535263900,
                0,
                0};
        class CMainParams : public CChainParams
        {
        public:
            CMainParams()
            {
                networkID = CBaseChainParams::MAIN;
                strNetworkID = "main";
                /**
                 * The message start string is designed to be unlikely to occur in normal data.
                 * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
                 * a large 4-byte int at any alignment.
                 */
                pchMessageStart[0] = 0xf8;
                pchMessageStart[1] = 0xd2;
                pchMessageStart[2] = 0xb3;
                pchMessageStart[3] = 0x47;
                vAlertPubKey = ParseHex("04579f18934b3ef39094a9999e45506a1935662d0cd4e504d07beb53b8a1bfd78d81bee47e65119318397809420d5320e3c7b2aaae58580db48c38a4e6d4f0f919");
                nDefaultPort = 8433;
                bnProofOfWorkLimit = ~uint256(0) >> 20; // Fac starting difficulty is 1 / 2^12
                nMaxReorganizationDepth = 100;
                nMinerThreads = 0;
                nTargetTimespan = 1 * 60; // Fac: 1.5 minutes
                nTargetSpacingSlowLaunch = 5 * 90;  // Fac: 7.5 minutes (Slow launch - Block 300)
                nTargetSpacing = 1 * 60; // Fac: 1.5min after block 300
                nLastPOWBlock = 550000;
                nLastPOWBlockOld = 550000; //
                nLastSeeSawBlock = 0; // last block for seesaw rewards
                nRampToBlock = 960; // Slow start, ramp linearly to this block
                nMaturity = 66; // 99 Minutes
                nMasternodeCountDrift = 4;
                nModifierUpdateBlock = 1;
                nMaxMoneyOut = 1000000000 * COIN; // Year 2

                nEnforceBlockUpgradeMajority = 750;
                nRejectBlockOutdatedMajority = 950;
                nToCheckBlockUpgradeMajority = 1000;

                const char* pszTimestamp = "Fac Launched in August,2018";
                CMutableTransaction txNew;
                txNew.vin.resize(1);
                txNew.vout.resize(1);
                txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
                txNew.vout[0].nValue = 50 * COIN;
                txNew.vout[0].scriptPubKey = CScript() << ParseHex("04243e8da79e117dba99d89a2da6ed761af43175227d19caaffea72398514962af9701478a69410b8158e190ae36d50a1f7325eba3df9559ad345db0cb72bfe2e2") << OP_CHECKSIG;
                genesis.vtx.push_back(txNew);
                genesis.hashPrevBlock = 0;
                genesis.hashMerkleRoot = genesis.BuildMerkleTree();
                genesis.nVersion = 1;
                genesis.nTime = 1535263200;
                genesis.nBits = bnProofOfWorkLimit.GetCompact();;
                genesis.nNonce = 4629701;

                hashGenesisBlock = genesis.GetHash();
                /**if(genesis.GetHash() != uint256S("0x"))
                {
                    bool fNegative;
                    bool fOverflow;
                    uint256 bigNum;
                    //printf("Searching for genesis block...\n");
                    bigNum.SetCompact(genesis.nBits, &fNegative, &fOverflow);
                    while(uint256(genesis.GetHash()) > bigNum)
                    {
                        ++genesis.nNonce;
                        if (genesis.nNonce == 0)
                        {
                            //printf("NONCE WRAPPED, incrementing time");
                            std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
                            ++genesis.nTime;
                        }
                        if (genesis.nNonce % 10000 == 0)
                        {
                            //printf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                        }
                    }
                    printf("Mainnet block.nBits = %u \n", genesis.nBits);
                    printf("Mainnet block.nTime = %u \n", genesis.nTime);
                    printf("Mainnet block.nNonce = %u \n", genesis.nNonce);
                    printf("Mainnet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
                    printf("Mainnet block.GetHash = %s\n", genesis.GetHash().GetHex().c_str());
                    printf("Mainnet block.Merkleroot = %s\n", genesis.hashMerkleRoot.ToString().c_str());
                }
                std::cout << std::string("Finished calculating Testnet Genesis Block:\n");
                */

                assert(hashGenesisBlock == uint256("0x000003fea1b6d9cb0e4267479d97eab7a80031803bf9d42298f3581da13edd0b"));
                assert(genesis.hashMerkleRoot == uint256("0x77c47b0e69b3033758c72063adf02c83fd4e752be878bfc0e3e1291938aa0753"));

                vSeeds.push_back(CDNSSeedData("seed1", "seed1.factrading.com"));      // Single node address
                vSeeds.push_back(CDNSSeedData("seed2", "seed2.factrading.com"));      // Single node address
                vSeeds.push_back(CDNSSeedData("seed3", "seed3.factrading.com"));      // Single node address
                vSeeds.push_back(CDNSSeedData("seed4", "seed4.factrading.com"));      // Single node address
                vSeeds.push_back(CDNSSeedData("seed5", "seed5.factrading.com"));      // Single node address
                vSeeds.push_back(CDNSSeedData("seed6", "seed6.factrading.com"));      // Single node address
                vSeeds.push_back(CDNSSeedData("seed7", "seed7.factrading.com"));      // Single node address
                vSeeds.push_back(CDNSSeedData("seed8", "seed8.factrading.com"));      // Single node address

                base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 35); // F
                base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 18);
                base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
                base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
                base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
                //BIP44 as defined by https://github.com/bitcoin/bips/blob/master/bip-0044.mediawiki
                base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

                convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

                fRequireRPCPassword = true;
                fMiningRequiresPeers = true;
                fAllowMinDifficultyBlocks = false;
                fDefaultConsistencyChecks = false;
                fRequireStandard = true;
                fMineBlocksOnDemand = false;
                fSkipProofOfWorkCheck = false;
                fTestnetToBeDeprecatedFieldRPC = false;
                fHeadersFirstSyncingActive = false;

                nPoolMaxTransactions = 3;
                strSporkKey = "0453748e298a34e32d760a3d64b7b517c952c10024a4160a3a746d9bce572f85e13ac6d4f518ac110ba807ce19fb657bc2696ca02013290e3fbe517adf09c95787";
                strObfuscationPoolDummyAddress = "bDiJwVuKv9dcKBN4KCfX6UmXbkpqLfzGyf";
                nStartMasternodePayments = 1534507200;
            }
            const Checkpoints::CCheckpointData& Checkpoints() const
            {
                return data;
            }
        };
        static CMainParams mainParams;
/**
 * Testnet (v3)
 */
        class CTestNetParams : public CMainParams
        {
        public:
            CTestNetParams()
            {
                networkID = CBaseChainParams::TESTNET;
                strNetworkID = "test";
                pchMessageStart[0] = 0xc5;
                pchMessageStart[1] = 0xe9;
                pchMessageStart[2] = 0xa4;
                pchMessageStart[3] = 0xab;

                vAlertPubKey = ParseHex("04795fde7bfc6347248a901aca81dd6a9f3acdeb5272f1c831f5147b139a4e1bacaa253541d9ebdfba982fb5cc45df3e34a8e98cdce9329037f009af217bc64ed9");
                nDefaultPort = 8434;
                nMinerThreads = 0;
                nTargetTimespan = 1 * 30; // 30 Seconds
                nTargetSpacing = 1 * 30;  // 30 Seconds
                nTargetSpacingSlowLaunch = 1 * 30; // Kludgy but don't want to check for testnet each time in GetNextWorkRequired

                nLastPOWBlock = 1000;
                nLastPOWBlockOld = 1100;
                nLastSeeSawBlock = 1200;
                nMaturity = 15;
                nMaxMoneyOut = 1000000000 * COIN; // 2032 Maximum
                nRampToBlock = 100;

                nEnforceBlockUpgradeMajority = 51;
                nRejectBlockOutdatedMajority = 75;
                nToCheckBlockUpgradeMajority = 100;

                //! Modify the testnet genesis block so the timestamp is valid for a later start.
                genesis.nTime = 1535263600;
                genesis.nNonce = 2555163;
                genesis.nBits = bnProofOfWorkLimit.GetCompact();

                hashGenesisBlock = genesis.GetHash();
                /**if(genesis.GetHash() != uint256S("0x"))
                {
                    bool fNegative;
                    bool fOverflow;
                    uint256 bigNum;
                    //printf("Searching for genesis block...\n");
                    bigNum.SetCompact(genesis.nBits, &fNegative, &fOverflow);
                    while(uint256(genesis.GetHash()) > bigNum)
                    {
                        ++genesis.nNonce;
                        if (genesis.nNonce == 0)
                        {
                            //printf("NONCE WRAPPED, incrementing time");
                            std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
                            ++genesis.nTime;
                        }
                        if (genesis.nNonce % 10000 == 0)
                        {
                            //printf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                        }
                    }
                    printf("Testnet block.nBits = %u \n", genesis.nBits);
                    printf("Testnet block.nTime = %u \n", genesis.nTime);
                    printf("Testnet block.nNonce = %u \n", genesis.nNonce);
                    printf("Testnet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
                    printf("Testnet block.GetHash = %s\n", genesis.GetHash().GetHex().c_str());
                    printf("Testnet block.Merkleroot = %s\n", genesis.hashMerkleRoot.ToString().c_str());
                }
                std::cout << std::string("Finished calculating Testnet Genesis Block:\n");*/

                assert(hashGenesisBlock == uint256("0x000009e01bfc09d316bc12ea9e81d5ec57b74d7fb36f97981739f61fa763926e"));
                assert(genesis.hashMerkleRoot == uint256("0x77c47b0e69b3033758c72063adf02c83fd4e752be878bfc0e3e1291938aa0753"));

                vFixedSeeds.clear();
                vSeeds.clear();
                vSeeds.push_back(CDNSSeedData("seed1", "seed1.factrading.com"));
                vSeeds.push_back(CDNSSeedData("seed2", "seed2.factrading.com"));
                vSeeds.push_back(CDNSSeedData("seed3", "seed3.factrading.com"));

                base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 95); // Testnet fac addresses start with 'T'
                base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 12);  // Testnet fac script addresses start with '5' or '6'
                base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
                // Testnet fac BIP32 pubkeys start with 'DRKV'
                base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
                // Testnet fac BIP32 prvkeys start with 'DRKP'
                base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
                // Testnet fac BIP44 coin type is '1' (All coin's testnet default)
                base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x01)(0x00)(0x00)(0x80).convert_to_container<std::vector<unsigned char> >();
                convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));
                fRequireRPCPassword = true;
                fMiningRequiresPeers = false;
                fAllowMinDifficultyBlocks = true;
                fDefaultConsistencyChecks = false;
                fRequireStandard = false;
                fMineBlocksOnDemand = false;
                fTestnetToBeDeprecatedFieldRPC = true;
                nPoolMaxTransactions = 2;
                strSporkKey = "04af2f9be75132357063236a59f9bf71ccb9ef8373c33146714ff4f6b1a3a2b97933c633f805c7b598ba023088bf13474e735b694d4959432bb58053910341bf66";
                strObfuscationPoolDummyAddress = "TUQ57Fbh1crybrDhV6X9SDH95H4oSq4v6p";
                nStartMasternodePayments = 1534501200; //Fri, 09 Jan 2015 21:05:58 GMT
            }
            const Checkpoints::CCheckpointData& Checkpoints() const
            {
                return dataTestnet;
            }
        };
        static CTestNetParams testNetParams;
/**
 * Regression test
 */
        class CRegTestParams : public CTestNetParams
        {
        public:
            CRegTestParams()
            {
                networkID = CBaseChainParams::REGTEST;
                strNetworkID = "regtest";
                strNetworkID = "regtest";
                pchMessageStart[0] = 0xa1;
                pchMessageStart[1] = 0xcf;
                pchMessageStart[2] = 0x7e;
                pchMessageStart[3] = 0xac;
                nMinerThreads = 1;
                nTargetTimespan = 24 * 60 * 60; // Fac: 1 day
                nTargetSpacing = 1 * 60;        // Fac: 1 minutes
                bnProofOfWorkLimit = ~uint256(0) >> 1;
                genesis.nTime = 1535263900;
                genesis.nBits = 0x207fffff;
                genesis.nNonce = 12347;
                hashGenesisBlock = genesis.GetHash();
                nDefaultPort = 8435;
                /**if(genesis.GetHash() != uint256S("0x"))
                {
                    bool fNegative;
                    bool fOverflow;
                    uint256 bigNum;
                    //printf("Searching for genesis block...\n");
                    bigNum.SetCompact(genesis.nBits, &fNegative, &fOverflow);
                    while(uint256(genesis.GetHash()) > bigNum)
                    {
                        ++genesis.nNonce;
                        if (genesis.nNonce == 0)
                        {
                            //printf("NONCE WRAPPED, incrementing time");
                            std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
                            ++genesis.nTime;
                        }
                        if (genesis.nNonce % 10000 == 0)
                        {
                            //printf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                        }
                    }
                    printf("Testnet block.nBits = %u \n", genesis.nBits);
                    printf("Testnet block.nTime = %u \n", genesis.nTime);
                    printf("Testnet block.nNonce = %u \n", genesis.nNonce);
                    printf("Testnet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
                    printf("Testnet block.GetHash = %s\n", genesis.GetHash().GetHex().c_str());
                    printf("Testnet block.Merkleroot = %s\n", genesis.hashMerkleRoot.ToString().c_str());
                }
                std::cout << std::string("Finished calculating Testnet Genesis Block:\n");
                 */
                assert(hashGenesisBlock == uint256("0x433d7062fd717e5f38e9d8865fe09b9fb885e0b29d6c2adb708f55427d500af8"));
                vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
                vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.
                fRequireRPCPassword = false;
                fMiningRequiresPeers = false;
                fAllowMinDifficultyBlocks = true;
                fDefaultConsistencyChecks = true;
                fRequireStandard = false;
                fMineBlocksOnDemand = true;
                fTestnetToBeDeprecatedFieldRPC = false;
            }
            const Checkpoints::CCheckpointData& Checkpoints() const
            {
                return dataRegtest;
            }
        };
        static CRegTestParams regTestParams;
/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 8436;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.
        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }
    //! Published setters to allow changing values in unit test cases
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
};
static CUnitTestParams unitTestParams;
static CChainParams* pCurrentParams = 0;
CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}
const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}
CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        case CBaseChainParams::UNITTEST:
            return unitTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}
void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;
    SelectParams(network);
    return true;
}
