cmake_minimum_required(VERSION 3.6)
project(navio)
project(gps_serial_transmit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR}/../Navio2/C++/Navio)

SET(TARGET_SRC
    ${CMAKE_SOURCE_DIR}/../Navio2/C++/Navio/Common/gpio.cpp
    ${CMAKE_SOURCE_DIR}/../Navio2/C++/Navio/Common/Util.cpp
    ${CMAKE_SOURCE_DIR}/../Navio2/C++/Navio/Common/I2Cdev.cpp
    ${CMAKE_SOURCE_DIR}/../Navio2/C++/Navio/Common/MPU9250.cpp
    ${CMAKE_SOURCE_DIR}/../Navio2/C++/Navio/Common/MS5611.cpp
    ${CMAKE_SOURCE_DIR}/../Navio2/C++/Navio/Common/Ublox.cpp
    ${CMAKE_SOURCE_DIR}/../Navio2/C++/Navio/Navio2/Led_Navio2.cpp
    ${CMAKE_SOURCE_DIR}/../Navio2/C++/Navio/Navio2/RGBled.cpp
    ${CMAKE_SOURCE_DIR}/../Navio2/C++/Navio/Navio2/ADC_Navio2.cpp
    ${CMAKE_SOURCE_DIR}/../Navio2/C++/Navio/Navio2/LSM9DS1.cpp
    ${CMAKE_SOURCE_DIR}/../Navio2/C++/Navio/Navio2/PWM.cpp
    ${CMAKE_SOURCE_DIR}/../Navio2/C++/Navio/Navio2/RCInput_Navio2.cpp
    ${CMAKE_SOURCE_DIR}/../Navio2/C++/Navio/Navio2/RCOutput_Navio2.cpp
)

#sets location of executable to top level
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)

add_executable(gps_serial_transmit main.cpp )
target_link_libraries(gps_serial_transmit ${CMAKE_SOURCE_DIR}/../Navio2/C++/Navio/libnavio.a)
qt5_use_modules(gps_serial_transmit Core SerialPort)
target_link_libraries(gps_serial_transmit Qt5::Widgets)

#for debug purposes
add_executable(serial_debug qserial_test.cpp )
qt5_use_modules(serial_debug Core SerialPort)
target_link_libraries(serial_debug Qt5::Widgets)
