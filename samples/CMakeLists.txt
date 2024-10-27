###############################################################################
# Author: Alexander Sidorenko                                                 #
# Mail:   <my last name>.<my first name> at google's email service            #
#                                                                             #
# This Source Code Form is subject to the terms of the Mozilla Public         #
# License, v. 2.0. If a copy of the MPL was not distributed with this         #
# file, You can obtain one at http://mozilla.org/MPL/2.0/.                    #
###############################################################################

cmake_minimum_required(VERSION 2.8)

set(MSP_TOOLCHAIN "ti")

set(MSP_FAMILY "msp430")

set(MSP_MCU "msp430fr6047")

set(MSP_AUTO_UPLOAD "FALSE")

set(TI_BASE_DIR "/home/dinesh/ti/ccs1250/")

set(CCS_PATH "/home/dinesh/ti/ccs1250/ccs")

set(CCS_COMPILER_PATH "/home/dinesh/ti/ccs1250/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin")

set(CMAKE_TOOLCHAIN_FILE "/home/dinesh/workspace/git/test_ms430fr6047_cmake/msp-toolchain.cmake")

set(CMAKE_C_COMPILER_WORKS TRUE)

set(CMAKE_CXX_COMPILER_WORKS TRUE)

#set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../msp-toolchain.cmake)

# For some reason CMake would ignore CMAKE_TOOLCHAIN_FILE if there is no project
# in this file.
project(samples C CXX)

link_directories("/home/dinesh/ti/ccs1250/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/lib")

# Set the linker command file for MSP430
set(LINKER_CMD_FILE "/home/dinesh/workspace/git/test_ms430fr6047_cmake/samples/lnk_msp430fr6047.cmd")

# Common link flags with linker command file
set(CMAKE_EXE_LINKER_FLAGS "--library=${LINKER_CMD_FILE}")

add_subdirectory(01_blink_c)
add_subdirectory(02_blink_cpp)
