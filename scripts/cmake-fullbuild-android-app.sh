#!/bin/bash
# SPDX-FileCopyrightText: © 2021 Alias Developers
# SPDX-License-Identifier: MIT

ownLocation="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${ownLocation}" || exit 1
. ./include/helpers_console.sh
_init
. ./include/handle_buildconfig.sh

if [[ -z ${KEYSTORE_PASS} ]] ; then
    read -r -s -p "Please enter sign keystore password: " KEYSTORE_PASS
    echo
    export KEYSTORE_PASS=${KEYSTORE_PASS}
fi

cd ../ || exit 1

./scripts/cmake-build-android_arm64.sh -g -t || exit 1
./scripts/cmake-build-android_armv7.sh -g -t || exit 1
./scripts/cmake-build-android_x86-64.sh -g -t || exit 1
./scripts/android-prepare-env_NEW.sh || exit 1
./scripts/android-build_NEW.sh
