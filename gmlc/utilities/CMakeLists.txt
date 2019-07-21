set(utilities_source_files
  base64.cpp
  charMapper.cpp
  string_viewOps.cpp
  stringOps.cpp
  stringConversion.cpp
  vectorOps.cpp
)

set(utilities_header_files
  base64.h
  charMapper.h
  generic_string_ops.hpp
  string_viewConversion.h
  string_viewDef.h
  string_viewOps.h
  stringConversion.h
  stringOps.h
  timeRepresentation.hpp
  vectorOps.hpp
  TimeSeries.hpp
  TimeSeriesMulti.hpp
)

if (NOT DEFINED UTILITIES_LIBRARY_TYPE)
   set (UTILITIES_LIBRARY_TYPE STATIC CACHE STRING "set the utilities library type" )
   set_property(CACHE UTILITIES_LIBRARY_TYPE PROPERTY STRINGS STATIC SHARED OBJECT)
endif()


  add_library(gmlc_utilities ${UTILITIES_LIBRARY_TYPE} ${utilities_source_files} ${utilities_header_files})
  target_include_directories(gmlc_utilities PUBLIC $<BUILD_INTERFACE:${GMLC_UTILITIES_SOURCE_DIR}>)

  target_include_directories(
  gmlc_utilities SYSTEM
  PRIVATE $<BUILD_INTERFACE:${BOOST_INCLUDE_DIR}>
)

add_library(gmlc::utilities ALIAS gmlc_utilities)

set (UTILTIIES_LIBRARY_EXPORT utilitiesConfig)

if (NOT UTILITIES_LIBRARY_TYPE STREQUAL "OBJECT")
target_link_libraries(gmlc_utilities utilities_base)
install(TARGETS gmlc_utilities
  EXPORT ${UTILITIES_LIBRARY_EXPORT}
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
install (FILES ${utilities_header_files} DESTINATION include)
else()
target_include_directories(
  gmlc_utilities
  PRIVATE $<TARGET_PROPERTY:utilities_base,INTERFACE_INCLUDE_DIRECTORIES>
)

target_include_directories(
  gmlc_utilities SYSTEM
  PRIVATE $<TARGET_PROPERTY:utilities_base,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>
)

target_compile_definitions(
  gmlc_utilities PRIVATE
  $<TARGET_PROPERTY:utilities_base,INTERFACE_COMPILE_DEFINITIONS>
)
endif()