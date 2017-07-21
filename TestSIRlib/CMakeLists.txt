cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
project(TestSIRlib)

find_package(SIRlib REQUIRED)
find_package(SimulationLib REQUIRED)
find_package(StatisticalDistributionsLib REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# add_executable(SerialBirthDeathSim run-SerialBirthDeathSim.cpp SerialBirthDeathSim.cpp)
# add_executable(ParallelBirthDeathSim run-ParallelBirthDeathSim.cpp ParallelBirthDeathSim.cpp)
# add_executable(PyramidBirthDeathSim run-PyramidBirthDeathSim.cpp PyramidBirthDeathSim.cpp)

# target_link_libraries(SerialBirthDeathSim PUBLIC SimulationLib StatisticalDistributionsLib)
# target_link_libraries(ParallelBirthDeathSim PUBLIC SimulationLib StatisticalDistributionsLib)
# target_link_libraries(PyramidBirthDeathSim PUBLIC SimulationLib StatisticalDistributionsLib)
