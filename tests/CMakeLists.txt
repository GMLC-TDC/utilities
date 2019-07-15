if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/gtest/CMakeLists.txt")
    message(FATAL_ERROR "You have requested tests be built, but googletest is not downloaded. Please run:
    git submodule update --init")
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/config/cmake")


set(GOOGLE_TEST_INDIVIDUAL OFF)
include(AddGoogletest)

set(UTILITIES_TESTS
    StringOpTests
	StringConversionTests
	String_viewOpsTests
	String_viewConversionTests
    )

# Only affects current directory, so safe
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

foreach(T ${UTILITIES_TESTS})

    add_executable(${T} ${T}.cpp )
    target_link_libraries(${T} PUBLIC gmlc_utilities)
    add_gtest(${T})
	set_target_properties(${T} PROPERTIES FOLDER tests)
   
endforeach()

if(CMAKE_BUILD_TYPE STREQUAL Coverage)
    include(CodeCoverage)
    setup_target_for_coverage(
        NAME CONTAINERS_coverage
        EXECUTABLE ctest
        DEPENDENCIES
          ${CONTAINERS_TESTS}
          )
endif()

