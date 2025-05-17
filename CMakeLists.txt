message("INSTALLING UJSON...")

cmake_minimum_required(VERSION 3.31.6)

project(ujson 
    VERSION 1.0
    DESCRIPTION "a library that parses the JSON string")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(UJSON_LIB ujson)
set(SOURCES ujson.c)
set(HEADERS ujson.h)

include(GNUInstallDirs)

add_library("${UJSON_LIB}" "${SOURCES}")
target_include_directories(${UJSON_LIB} PUBLIC 
                  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${UJSON_LIB}>
                  $<INSTALL_INTERFACE:include>)

install(FILES ujson.h DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/ujson")
install(TARGETS "${UJSON_LIB}"
        EXPORT "${UJSON_LIB}Targets"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

# a instruction to tell the user how to use this project
install(EXPORT ${UJSON_LIB}Targets
        FILE ujsontargets.cmake
        NAMESPACE ujson::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ujson)

# what if someday a new version has been built and u want to tell the user
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ujsonConfigVersion.cmake"
    VERSION 1.0
    COMPATIBILITY SameMajorVersion
)

# tells where to find the header, how to link it, what the name of the library is, etc.
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/ujsonConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ujsonConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ujson
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ujsonConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ujsonConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ujson)
