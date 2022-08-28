cmake_minimum_required(VERSION 3.15...3.22)

project(olab VERSION "0.0.1")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -O2 -lfftw3_omp -lfftw3 -lfftw3f_omp -lfftw3f -lm")


# SKBUILD
if(SKBUILD)
  # Scikit-Build does not add your site-packages to the search path
  # automatically, so we need to add it _or_ the pybind11 specific directory
  # here.
  execute_process(
    COMMAND "${PYTHON_EXECUTABLE}" -c
            "import pybind11; print(pybind11.get_cmake_dir())"
    OUTPUT_VARIABLE _tmp_dir
    OUTPUT_STRIP_TRAILING_WHITESPACE COMMAND_ECHO STDOUT)
  list(APPEND CMAKE_PREFIX_PATH "${_tmp_dir}")
endif()


include_directories("src")


# Now we can find pybind11
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(_core MODULE src/main.cpp)

target_compile_definitions(_core PRIVATE VERSION_INFO=${PROJECT_VERSION})


# OpenMP
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()


# FFTW
find_library (
    FFTW_OMP
    NAMES fftw3_omp libfftw3_omp fftw3f_omp libfftw3f_omp # what to look for
    # NAMES fftw3_omp libfftw3_omp # what to look for
    # HINTS "${PROJECT_SOURCE_DIR}/src/fftw" # where to look
    # NO_DEFAULT_PATH # do not search system default paths
    REQUIRED
)
if(FFTW_OMP)
    target_link_libraries(_core PRIVATE ${FFTW_OMP})
    # target_link_libraries(${fcwt_library_target} PRIVATE ${FFTW_OMP})
else()
    # message(SEND_ERROR "Did not find lib fftw3f_omp")
    message(SEND_ERROR "Did not find lib fftw3_omp")
endif()


# Catch2
add_subdirectory(lib/Catch2)
add_executable(cpp_tests tests/test.cpp tests/test_mdv.cpp tests/test_qr.cpp)
target_link_libraries(cpp_tests PRIVATE Catch2::Catch2WithMain)
include(CTest)
include(Catch)
catch_discover_tests(cpp_tests)



install(TARGETS _core DESTINATION .)