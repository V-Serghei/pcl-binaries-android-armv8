cmake_minimum_required(VERSION 3.22.1)

project("pclibrary")
set(BOOST_ROOT "pclib/jniLibs/arm64-v8a/boost")
set(EIGEN_ROOT "pclib/jniLibs/arm64-v8a/eigen")
set(FLANN_ROOT "pclib/jniLibs/arm64-v8a/flann")
set(LZ4_ROOT "pclib/jniLibs/arm64-v8a/lz4")
set(PCL_ROOT "pclib/jniLibs/arm64-v8a/pcl")
include_directories(${BOOST_ROOT}/include ${EIGEN_ROOT}/include/eigen3 ${FLANN_ROOT}/include ${LZ4_ROOT}/include ${PCL_ROOT}/include)
link_directories(${BOOST_ROOT}/lib ${FLANN_ROOT}/lib ${LZ4_ROOT}/lib ${PCL_ROOT}/lib)
add_library(${CMAKE_PROJECT_NAME} SHARED
        pclibrary.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
        -lpcl_common
        -lpcl_io
        -lpcl_io_ply
        -lpcl_kdtree
        -lpcl_features
        -lpcl_filters
        -lpcl_segmentation
        -lpcl_tracking
        -lpcl_recognition
        -lpcl_ml
        -lpcl_search
        -lpcl_registration
        -lpcl_keypoints
        -lpcl_sample_consensus
        -lpcl_surface
        -lpcl_octree
        -lpcl_stereo
        -lboost_system
        -lboost_filesystem
        -lboost_thread
        -lboost_chrono
        -lboost_date_time
        -lboost_graph
        -lboost_log
        -lboost_log_setup
        -lboost_program_options
        -lboost_random
        -lboost_math_tr1
        -lboost_exception
        -lboost_type_erasure
        -lboost_container
        -lboost_iostreams
        -lboost_stacktrace_noop
        -lboost_fiber
        -lboost_math_c99
        -lboost_stacktrace_basic
        -lboost_wserialization
        -lboost_wave
        -lboost_serialization
        -lboost_unit_test_framework
        -lboost_prg_exec_monitor
        -lboost_timer
        -lboost_context
        -lboost_log_setup
        -lboost_log
        -lboost_program_options
        -landroid
        -llog
        -latomic
        -lm
)
