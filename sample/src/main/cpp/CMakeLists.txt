cmake_minimum_required(VERSION 3.22.1)

project("pcl_android_sample")

add_library(native-lib SHARED native-lib.cpp)

find_package(pclibrary REQUIRED CONFIG)

set(PCL_ANDROID_TARGET pclibrary::pclibrary)
get_target_property(PCL_ANDROID_TARGET_TYPE pclibrary::pclibrary TYPE)

if(PCL_ANDROID_TARGET_TYPE STREQUAL "INTERFACE_LIBRARY")
    get_filename_component(REPO_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../.." ABSOLUTE)
    file(GLOB LOCAL_PCL_LIBRARY
            "${REPO_ROOT}/pclibrary/build/intermediates/cxx/Debug/*/obj/${ANDROID_ABI}/libpclibrary.so")

    if(LOCAL_PCL_LIBRARY)
        list(GET LOCAL_PCL_LIBRARY 0 LOCAL_PCL_LIBRARY_PATH)
        add_library(pclibrary_local SHARED IMPORTED)
        set_target_properties(pclibrary_local PROPERTIES
                IMPORTED_LOCATION "${LOCAL_PCL_LIBRARY_PATH}")
        set(PCL_ANDROID_TARGET pclibrary::pclibrary pclibrary_local)
    endif()
endif()

target_link_options(native-lib PRIVATE -Wl,-z,max-page-size=16384)

target_link_libraries(native-lib
        ${PCL_ANDROID_TARGET}
        log)
