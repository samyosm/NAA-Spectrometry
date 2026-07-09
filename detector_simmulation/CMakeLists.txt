cmake_minimum_required(VERSION 3.15)
project(DetectorSimulation)

if(DEFINED ENV{CONDA_PREFIX})
    list(APPEND CMAKE_PREFIX_PATH $ENV{CONDA_PREFIX})
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

set(SOURCES
    main.cc
    src/DetectorConstruction.cc
    src/ActionInitialization.cc
    src/PrimaryGeneratorAction.cc
    src/SteppingAction.cc
    src/EventAction.cc
    src/RunAction.cc
)

add_executable(simulate_detection ${SOURCES})

target_include_directories(simulate_detection PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(simulate_detection PRIVATE
    ${Geant4_LIBRARIES}
)

# Copy macros into the build directory so they sit next to the executable
# file(COPY ${PROJECT_SOURCE_DIR}/macros DESTINATION ${PROJECT_BINARY_DIR})
