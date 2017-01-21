# ============================================================================
#
# This file hosts all tests.
#
# If INCLUDE_PILE_SOURCES_DIRECTLY is ON then REGEXLITE_SOURCES and
# REGEXLITE_HEADERS are defined we add them to the executable. REGEXLITE_LIBRARY
# is empty.
#
# On the other hand if INCLUDE_PILE_SOURCES_DIRECTLY is
# OFF then REGEXLITE_SOURCES and REGEXLITE_HEADERS are empty and we link
# against REGEXLITE_LIBRARY that is now the name of the target that
# builds the library.
#
# ============================================================================

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

if (NOT INCLUDE_PILE_SOURCES_DIRECTLY)
    set (REGEXLITE_LIBRARY regexlite)
else()
    unset (REGEXLITE_LIBRARY CACHE)
endif()

add_executable(test_basic
    ${REGEXLITE_SOURCES}
    ${REGEXLITE_HEADERS}
    basic.cc)

if (UNIX)
    find_package(Threads REQUIRED)
    list(APPEND GTEST_BOTH_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
    add_definitions(-fPIC)
endif()

find_library(ICU_UC_LIB
    NAMES icuuc
    PATHS "${MY_ICU_PATH}/lib64")

find_library(ICU_IN_LIB
    NAMES icuin
    PATHS "${MY_ICU_PATH}/lib64")

target_link_libraries(test_basic
    ${GTEST_BOTH_LIBRARIES}
    ${REGEXLITE_LIBRARY}
    ${ICU_UC_LIB} ${ICU_IN_LIB} )

qt5_use_modules(test_basic Core Sql)

add_test(
    NAME test_basic
    COMMAND $<TARGET_FILE:test_basic>)
