# SPDX-FileCopyrightText: © 2020 Alias Developers
# SPDX-FileCopyrightText: © 2019 SpectreCoin Developers
# SPDX-License-Identifier: MIT

add_compile_definitions(ALIAS_QT_TEST)
add_executable(test_alias_qt ${CMAKE_CURRENT_LIST_DIR}/test_main.cpp)
target_link_libraries(test_alias_qt
        aliaswallet_lib
        ${QT_LIBS}
        Qt5::Test
        Boost::unit_test_framework
)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/qt)

target_sources(test_alias_qt
    PRIVATE
    ${ALIAS_MAIN_FILES}
    ${PROJECT_SOURCE_DIR}/src/qt/guiutil.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/bitcoinaddressvalidator.cpp
    ${PROJECT_SOURCE_DIR}/src/qt/bitcoinunits.cpp

    "${CMAKE_CURRENT_LIST_DIR}/uritests.cpp"
)
