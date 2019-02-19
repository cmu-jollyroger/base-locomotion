cmake_minimum_required(VERSION 3.9)
project(stm_com)

set(CMAKE_CXX_STANDARD 11)

set(STMCOM_SOURCE
        TOF_API.h TOF_interface.c )

set(STMCOM_TEST_SOURCE
        TOF_API.h TOF_interface.c TOF_interface_test.c
        )
# Communication Library
add_library(STM_com ${STMCOM_SOURCE})

add_executable(STM_com_test ${STMCOM_TEST_SOURCE})
