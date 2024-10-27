###############################################################################
# Author: Alexander Sidorenko                                                 #
# Mail:   <my last name>.<my first name> at google's email service            #
#                                                                             #
# This Source Code Form is subject to the terms of the Mozilla Public         #
# License, v. 2.0. If a copy of the MPL was not distributed with this         #
# file, You can obtain one at http://mozilla.org/MPL/2.0/.                    #
###############################################################################

cmake_minimum_required(VERSION 2.8)

project(evm_uartA3 C)


# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    /home/dinesh/ti/ccs1250/ccs/ccs_base/msp430/include
    /home/dinesh/ti/ccs1250/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include
)
add_msp_executable(evm_uartA3 msp430fr60x7_eusci_uart_standard_transceiver.c)
