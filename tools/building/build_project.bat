:<<BATCH
    :;@echo off
    :; # ########## WINDOWS SECTION #########################
       

    SET SCRIPT_PATH=%~dp0
    SET BASE_PATH=%SCRIPT_PATH%..\..

    cmake -B %BASE_PATH%\build -S %BASE_PATH%\src -DCMAKE_BUILD_TYPE=Release
    cmake --build %BASE_PATH%\build --config Release

    :; # ########## WINDOWS SECTION ENDS ####################
    :; # ####################################################
    exit /b
BATCH

SCRIPT_PATH="$(
        cd "$(dirname "$0")" >/dev/null 2>&1
        pwd -P
    )"
BASE_PATH=${SCRIPT_PATH}/../..

cmake -B ${BASE_PATH}/build -S ${BASE_PATH}/src -DCMAKE_BUILD_TYPE=Release

if [ "$(uname)" == "Darwin" ]; then
    cmake --build ${BASE_PATH}/build --parallel $(sysctl -n hw.physicalcpu)
else 
    cmake --build ${BASE_PATH}/build --parallel $(expr $(nproc --all) / 2)
fi
