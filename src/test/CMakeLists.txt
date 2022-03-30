# SPDX-FileCopyrightText: © 2020 Alias Developers
# SPDX-FileCopyrightText: © 2019 SpectreCoin Developers
# SPDX-License-Identifier: MIT

add_compile_definitions(TEST_DATA_DIR=${CMAKE_CURRENT_LIST_DIR}/data)

add_executable(test_alias ${CMAKE_CURRENT_LIST_DIR}/test_alias.cpp)
add_test(
        NAME performAliasTests
        COMMAND test_alias
        )

target_link_libraries(test_alias
    aliaswallet_lib
    Boost::unit_test_framework
)

include_directories(${PROJECT_SOURCE_DIR}/src)

target_sources(test_alias
        PRIVATE
            "${CMAKE_CURRENT_LIST_DIR}/accounting_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/allocator_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/base32_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/base58_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/base64_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/basic_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/bignum_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/bip32_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/Checkpoints_tests.cpp"
#            "${CMAKE_CURRENT_LIST_DIR}/extkey_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/getarg_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/hash_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/hmac_tests.cpp"
#            "${CMAKE_CURRENT_LIST_DIR}/key_tests.cpp"
#            "${CMAKE_CURRENT_LIST_DIR}/mnemonic_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/mruset_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/multisig_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/netbase_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/ringsig_tests.cpp"
#            "${CMAKE_CURRENT_LIST_DIR}/rpc_tests.cpp"
#            "${CMAKE_CURRENT_LIST_DIR}/script_P2SH_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/script_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/sigopcount_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/stealth_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/uint160_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/uint256_tests.cpp"
            "${CMAKE_CURRENT_LIST_DIR}/util_tests.cpp"
#            "${CMAKE_CURRENT_LIST_DIR}/wallet_tests.cpp"

            "${CMAKE_CURRENT_LIST_DIR}/other/DoS_tests.cpp"
#            "${CMAKE_CURRENT_LIST_DIR}/other/miner_tests.cpp"
#            "${CMAKE_CURRENT_LIST_DIR}/other/transaction_tests.cpp"
        )
